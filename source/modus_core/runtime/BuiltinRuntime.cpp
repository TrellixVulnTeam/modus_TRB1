#include <modus_core/runtime/BuiltinRuntime.hpp>
#include <modus_core/embed/Python.hpp>
#include <modus_core/graphics/RenderWindow.hpp>
#include <modus_core/imgui/ImGuiEvents.hpp>
#include <modus_core/runtime/RuntimeEvents.hpp>
#include <modus_core/window/WindowEvents.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	builtin_runtime::builtin_runtime(runtime_api * api) noexcept
		: runtime_context	{ api }
		, m_imgui			{}
		, m_dock			{ "##MainDockspace" }
	{
		initialize(api);
	}

	builtin_runtime::~builtin_runtime() noexcept
	{
		finalize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void builtin_runtime::initialize(runtime_api * api)
	{
		// singleton
		ML_assert(this == get_global_runtime());

		// events
		subscribe<window_key_event>();
		subscribe<window_mouse_event>();
		subscribe<window_cursor_pos_event>();
		
		// loop condition
		set_loop_condition(&render_window::is_open, get_window());

		// preferences
		ML_assert(api->io->prefs.contains("runtime"));
		auto & runtime_prefs{ api->io->prefs["runtime"] };
		bool const install_callbacks{ runtime_prefs["callbacks"] };

		// python
		PyObject_SetArenaAllocator(([&temp = PyObjectArenaAllocator{}](auto mres) noexcept {
			temp.ctx = mres;
			temp.alloc = [](auto mres, size_t s) {
				return ((pmr::memory_resource *)mres)->allocate(s);
			};
			temp.free = [](auto mres, void * p, size_t s) {
				return ((pmr::memory_resource *)mres)->deallocate(p, s);
			};
			return &temp;
		})(api->mem->get_resource()));
		Py_SetProgramName(api->io->program_name.c_str());
		Py_SetPythonHome(api->io->content_path.c_str());
		Py_InitializeEx(1);
		ML_assert(Py_IsInitialized());

		// window
		ML_assert(api->io->prefs.contains("window"));
		auto & window_prefs{ api->io->prefs["window"] };
		ML_assert(api->win->open(
			window_prefs["title"],
			window_prefs["video"],
			window_prefs["context"],
			window_prefs["hints"]));
		if (install_callbacks)
		{
			// helper
			static struct ML_NODISCARD {
				runtime_api * api;
				ML_NODISCARD auto operator->() const noexcept
				{
					return ML_check(api)->bus;
				}
			} helper; helper.api = api;

			api->win->set_char_callback([](auto w, auto ... x) { helper->fire<window_char_event>(x...); });
			api->win->set_char_mods_callback([](auto w, auto ... x) { helper->fire<window_char_mods_event>(x...); });
			api->win->set_close_callback([](auto w, auto ... x) { helper->fire<window_close_event>(x...); });
			api->win->set_cursor_enter_callback([](auto w, auto ... x) { helper->fire<window_cursor_enter_event>(x...); });
			api->win->set_cursor_pos_callback([](auto w, auto ... x) { helper->fire<window_cursor_pos_event>(x...); });
			api->win->set_content_scale_callback([](auto w, auto ... x) { helper->fire<window_content_scale_event>(x...); });
			api->win->set_drop_callback([](auto w, auto ... x) { helper->fire<window_drop_event>(x...); });
			api->win->set_error_callback([](auto ... x) { helper->fire<window_error_event>(x...); });
			api->win->set_focus_callback([](auto w, auto ... x) { helper->fire<window_focus_event>(x...); });
			api->win->set_framebuffer_resize_callback([](auto w, auto ... x) { helper->fire<window_framebuffer_resize_event>(x...); });
			api->win->set_iconify_callback([](auto w, auto ... x) { helper->fire<window_iconify_event>(x...); });
			api->win->set_key_callback([](auto w, auto ... x) { helper->fire<window_key_event>(x...); });
			api->win->set_maximize_callback([](auto w, auto ... x) { helper->fire<window_maximize_event>(x...); });
			api->win->set_mouse_callback([](auto w, auto ... x) { helper->fire<window_mouse_event>(x...); });
			api->win->set_position_callback([](auto w, auto ... x) { helper->fire<window_position_event>(x...); });
			api->win->set_refresh_callback([](auto w, auto ... x) { helper->fire<window_refresh_event>(x...); });
			api->win->set_resize_callback([](auto w, auto ... x) { helper->fire<window_resize_event>(x...); });
			api->win->set_scroll_callback([](auto w, auto ... x) { helper->fire<window_scroll_event>(x...); });
		}

		// imgui
		ImGui::SetAllocatorFunctions(
			[](size_t s, void * u) { return ((memory_manager *)u)->allocate(s); },
			[](void * p, void * u) { return ((memory_manager *)u)->deallocate(p); },
			api->mem);
		m_imgui.reset(ML_check(ImGui::CreateContext()));
		m_imgui->IO.LogFilename = "";
		m_imgui->IO.IniFilename = "";
		m_imgui->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		m_imgui->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		m_imgui->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ML_assert(ImGui_Startup(api->win, install_callbacks));
		if (runtime_prefs.contains("dockspace")) {
			auto & dock_prefs{ runtime_prefs["dockspace"] };
			dock_prefs["alpha"].get_to(m_dock.Alpha);
			dock_prefs["border"].get_to(m_dock.Border);
			dock_prefs["padding"].get_to(m_dock.Padding);
			dock_prefs["rounding"].get_to(m_dock.Rounding);
			dock_prefs["size"].get_to(m_dock.Size);
		}
		if (runtime_prefs.contains("guistyle")) {
			auto & guistyle{ runtime_prefs["guistyle"] };
			if (guistyle.is_string())
			{
				ImGui_LoadStyle(api->io->path2(guistyle));
			}
		}

		// install plugins
		if (runtime_prefs.contains("plugins"))
		{
			for (auto const & e : runtime_prefs["plugins"])
			{
				get_plugins().install(e["path"]);
			}
		}

		// execute scripts
		if (runtime_prefs.contains("scripts"))
		{
			for (auto const & e : runtime_prefs["scripts"])
			{
				Python_DoFile(api->io->path2(e["path"]));
			}
		}
	}

	void builtin_runtime::finalize()
	{
		ImGui_Shutdown(get_window(), m_imgui.release());

		ML_assert(Py_FinalizeEx() == EXIT_SUCCESS);
	}

	void builtin_runtime::on_enter() noexcept
	{
		get_bus()->fire<runtime_enter_event>(this);
	}

	void builtin_runtime::on_exit() noexcept
	{
		get_bus()->fire<runtime_exit_event>(this);
	}

	void builtin_runtime::on_idle()
	{
		// update
		get_bus()->fire<runtime_idle_event>(this);

		// imgui
		ImGui_DoFrame(get_window(), m_imgui.get(), [&]() noexcept
		{
			ML_ImGui_ScopeID(this);

			ML_flag_write(m_dock.WinFlags, ImGuiWindowFlags_MenuBar, ImGui::FindWindowByName("##MainMenuBar"));
			
			m_dock(m_imgui->Viewports[0], [&]() noexcept
			{
				get_bus()->fire<imgui_dockspace_event>(&m_dock);
			});

			get_bus()->fire<imgui_render_event>(m_imgui.get());
		});
	}

	void builtin_runtime::on_event(event && value)
	{
		switch (value)
		{
		case window_key_event::ID: {
			auto && ev{ (window_key_event &&)value };
			get_io()->keyboard[ev.key] = ev.action;
		} break;

		case window_mouse_event::ID: {
			auto && ev{ (window_mouse_event &&)value };
			get_io()->mouse[ev.button] = ev.action;
		} break;

		case window_cursor_pos_event::ID: {
			auto && ev{ (window_cursor_pos_event &&)value };
			get_io()->cursor = { ev.x, ev.y };
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}