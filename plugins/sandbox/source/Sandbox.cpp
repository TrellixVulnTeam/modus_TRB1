/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <modus_core/detail/HashMap.hpp>
#include <modus_core/detail/StreamSniper.hpp>
#include <modus_core/embed/Python.hpp>
#include <modus_core/graphics/Font.hpp>
#include <modus_core/graphics/Mesh.hpp>
#include <modus_core/imgui/ImGuiEvents.hpp>
#include <modus_core/imgui/ImGuiExt.hpp>
#include <modus_core/runtime/Application.hpp>
#include <modus_core/runtime/RuntimeEvents.hpp>
#include <modus_core/window/WindowEvents.hpp>
#include <modus_core/window/Viewport.hpp>
#include <modus_core/scene/Scene.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	struct ML_PLUGIN_API sandbox final : plugin
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// panels
		enum : size_t
		{
			imgui_about_panel,
			imgui_demo_panel,
			imgui_metrics_panel,
			imgui_style_panel,

			database_panel,
			terminal_panel,
			viewport_panel,

			MAX_PANEL
		};
		ImGuiExt::Panel m_panels[MAX_PANEL]
		{
			{ "About Dear ImGui" },
			{ "Dear ImGui Demo" },
			{ "Dear ImGui Metrics" },
			{ "Style Editor" },
			
			{ "database", 0, ImGuiWindowFlags_None },
			{ "terminal", 1, ImGuiWindowFlags_MenuBar },
			{ "viewport", 1, ImGuiWindowFlags_MenuBar },
		};

		// command line
		basic_stream_sniper<>	m_cout{ &std::cout };
		ImGuiExt::Terminal		m_term{};

		// rendering
		vec2 m_resolution{ 1280, 720 };
		color m_clear_color{ 0.223f, 0.f, 0.46f, 1.f };
		ds::list<ds::ref<gfx::framebuffer>> m_fb{};

		// icon
		db_var<bitmap> m_icon{ get_database(), "icon" };

		// resources
		ds::hashmap<ds::string, ds::ref<font>>			m_fonts		{};
		ds::hashmap<ds::string, ds::ref<bitmap>>		m_images	{};
		ds::hashmap<ds::string, ds::ref<mesh>>			m_meshes	{};
		ds::hashmap<ds::string, ds::ref<gfx::program>>	m_programs	{};
		ds::hashmap<ds::string, ds::ref<scene>>			m_scenes	{};
		ds::hashmap<ds::string, ds::ref<gfx::shader>>	m_shaders	{};
		ds::hashmap<ds::string, ds::ref<gfx::texture>>	m_textures	{};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		~sandbox() noexcept override {}

		sandbox(plugin_manager * manager, void * userptr) noexcept : plugin{ manager, userptr }
		{
			subscribe<app_enter_event>();
			subscribe<app_exit_event>();
			subscribe<app_idle_event>();
			subscribe<imgui_dockspace_event>();
			subscribe<imgui_render_event>();
		}

		void on_event(event && value) override
		{
			switch (value)
			{
			case app_enter_event		::ID: return on_app_enter		((app_enter_event &&)value);
			case app_exit_event			::ID: return on_app_exit		((app_exit_event &&)value);
			case app_idle_event			::ID: return on_app_idle		((app_idle_event &&)value);
			case imgui_dockspace_event	::ID: return on_imgui_dockspace	((imgui_dockspace_event &&)value);
			case imgui_render_event		::ID: return on_imgui_render	((imgui_render_event &&)value);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_app_enter(app_enter_event && ev)
		{
			// set icon
			if (auto & i = m_icon = bitmap{ get_io()->path2("resource/modus_launcher.png"), false })
			{
				get_window()->set_icons(i.width(), i.height(), 1, i.data());
			}

			// framebuffers
			m_fb.push_back(gfx::framebuffer::create({ m_resolution }));
		}

		void on_app_exit(app_exit_event && ev)
		{
		}

		void on_app_idle(app_idle_event && ev)
		{
			m_term.Output.Dump(m_cout.sstr());

			for (auto & fb : m_fb) { fb->resize(m_resolution); }

			get_window()->execute(
				gfx::command::bind_framebuffer(m_fb[0]),
				gfx::command::set_clear_color(m_clear_color),
				gfx::command::clear(gfx::clear_color | gfx::clear_depth),
				gfx::command([&](gfx::render_context * ctx) noexcept
				{
					// custom rendering...
				}),
				gfx::command::bind_framebuffer(nullptr));
		}

		void on_imgui_dockspace(imgui_dockspace_event && ev)
		{
			if (ImGuiID const root{ ev->GetID() }; !ImGui::DockBuilderGetNode(root))
			{
				ImGui::DockBuilderRemoveNode(root);
				ImGui::DockBuilderAddNode(root, ev->DockFlags);
				ImGui::DockBuilderDockWindow(m_panels[viewport_panel].Title, root);
				ImGui::DockBuilderFinish(root);
			}
		}

		void on_imgui_render(imgui_render_event && ev)
		{
			// MAIN MENU BAR
			if (ImGui::BeginMainMenuBar()) {
				// FILE
				if (ImGui::BeginMenu("file")) {
					if (ImGui::MenuItem("quit", "alt+f4")) {
						get_window()->set_should_close(true);
					}
					ImGui::EndMenu();
				}
				// TOOLS
				if (ImGui::BeginMenu("tools")) {
					ImGuiExt::MenuItem(m_panels + database_panel);
					ImGuiExt::MenuItem(m_panels + terminal_panel);
					ImGuiExt::MenuItem(m_panels + viewport_panel);
					ImGui::EndMenu();
				}
				// HELP
				if (ImGui::BeginMenu("help")) {
					ImGuiExt::MenuItem(m_panels + imgui_demo_panel);
					ImGuiExt::MenuItem(m_panels + imgui_metrics_panel);
					ImGuiExt::MenuItem(m_panels + imgui_about_panel);
					ImGuiExt::MenuItem(m_panels + imgui_style_panel);
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			// IMGUI
			if (m_panels[imgui_about_panel].IsOpen) {
				ImGui::ShowAboutWindow(&m_panels[imgui_about_panel].IsOpen);
			}
			if (m_panels[imgui_demo_panel].IsOpen) {
				ImGui::ShowDemoWindow(&m_panels[imgui_demo_panel].IsOpen);
			}
			if (m_panels[imgui_metrics_panel].IsOpen) {
				ImGui::ShowMetricsWindow(&m_panels[imgui_metrics_panel].IsOpen);
			}
			m_panels[imgui_style_panel](
				&ImGui::ShowStyleEditor, &ImGui::GetStyle()
			);

			// SANDBOX
			draw_database_panel(); // DATABASE
			draw_terminal_panel(); // TERMINAL
			draw_viewport_panel(); // VIEWPORT
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// DATABASE
		void draw_database_panel()
		{
			if (m_panels[database_panel].IsOpen) {
				auto const winsize{ (vec2)get_window()->get_size() };
				ImGui::SetNextWindowSize({ 960, 329 }, ImGuiCond_Once);
				ImGui::SetNextWindowPos(winsize / 2, ImGuiCond_Once, { 0.5f, 0.5f });
			}
			m_panels[database_panel]([&]() noexcept
			{
				ML_defer() { ImGui::EndTabBar(); };
				if (!ImGui::BeginTabBar("tabs")) { return; }

				// plugins
				if (ImGui::BeginTabItem("plugins")) {
					ImGui::Columns(3);
					ImGui::TextDisabled("name"); ImGui::NextColumn();
					ImGui::TextDisabled("path"); ImGui::NextColumn();
					ImGui::TextDisabled("ID"); ImGui::NextColumn();
					ImGui::Separator();
					for (auto const & e : get_manager()->get_data<plugin_details>())
					{
						ImGui::Text(e.name.c_str()); ImGui::NextColumn();
						ImGui::Text(e.path.c_str()); ImGui::NextColumn();
						ImGui::Text("%u", e.hash); ImGui::NextColumn();
					}
					ImGui::Columns(1);
					ImGui::Separator();
					ImGui::EndTabItem();
				}
			});
		}

		// TERMINAL
		void draw_terminal_panel()
		{
			if (m_panels[terminal_panel].IsOpen) {
				auto const winsize{ (vec2)get_window()->get_size() };
				ImGui::SetNextWindowSize(winsize / 2, ImGuiCond_Once);
				ImGui::SetNextWindowPos(winsize / 2, ImGuiCond_Once, { 0.5f, 0.5f });
			}
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4, 4 });
			if (!m_panels[terminal_panel]([&]() noexcept
			{
				ImGui::PopStyleVar(1);

				// menubar
				if (ImGui::BeginMenuBar())
				{
					// filter
					ImGui::TextDisabled("filter"); ImGui::SameLine();
					m_term.Output.Filter.Draw("##filter", 256);
					ImGui::Separator();

					// options
					if (ImGui::BeginMenu("options"))
					{
						// auto scroll
						ImGui::Checkbox("auto scroll", &m_term.Output.AutoScroll);
						ImGui::Separator();

						// USER
						char username[32]{}; std::strcpy(username, m_term.User.c_str());
						ImGui::TextDisabled("user"); ImGui::SameLine();
						if (ImGui::InputText("##username", username, ML_arraysize(username), ImGuiInputTextFlags_EnterReturnsTrue)) {
							m_term.User = username;
						} ImGui::SameLine();
						ImGui::ColorEdit4("##usercolor", m_term.Colors.User, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

						// HOST
						char hostname[32]{}; std::strcpy(hostname, m_term.Host.c_str());
						ImGui::TextDisabled("host"); ImGui::SameLine();
						if (ImGui::InputText("##hostname", hostname, ML_arraysize(hostname), ImGuiInputTextFlags_EnterReturnsTrue)) {
							m_term.Host = hostname;
						} ImGui::SameLine();
						ImGui::ColorEdit4("##hostcolor", m_term.Colors.Host, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

						// PATH
						char pathname[32]{}; std::strcpy(pathname, m_term.Path.c_str());
						ImGui::TextDisabled("path"); ImGui::SameLine();
						if (ImGui::InputText("##pathname", pathname, ML_arraysize(pathname), ImGuiInputTextFlags_EnterReturnsTrue)) {
							m_term.Path = pathname;
						} ImGui::SameLine();
						ImGui::ColorEdit4("##pathcolor", m_term.Colors.Path, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

						// MODE
						char modename[32]{}; std::strcpy(modename, m_term.Mode.c_str());
						ImGui::TextDisabled("mode"); ImGui::SameLine();
						if (ImGui::InputText("##modename", modename, ML_arraysize(modename), ImGuiInputTextFlags_EnterReturnsTrue)) {
							m_term.Mode = modename;
						} ImGui::SameLine();
						ImGui::ColorEdit4("##modecolor", m_term.Colors.Mode, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

						ImGui::EndMenu();
					} ImGui::Separator();

					// clear
					if (ImGui::MenuItem("clear")) { m_term.Output.Lines.clear(); }
					ImGui::Separator();

					ImGui::EndMenuBar();
				}
				
				// draw
				ImGuiExt::ChildWindow("##output", { 0, -ImGui::GetFrameHeightWithSpacing() }, false, ImGuiWindowFlags_HorizontalScrollbar, [&]() noexcept
				{
					m_term.Output.Draw();
				});
				ImGuiExt::ChildWindow("##input", {}, false, ImGuiWindowFlags_NoScrollbar, [&]() noexcept
				{
					m_term.DrawPrefix(); ImGui::SameLine();
					m_term.DrawInput();
				});

				// setup
				if (!m_term.Commands.empty()) { return; }
				m_term.User = "root";
				m_term.Host = "localhost";
				m_term.Path = "~";
				m_term.Mode = "";

				// clear
				m_term.AddCommand("clear", {}, [&](auto line) {
					m_term.Output.Lines.clear();
				});

				// echo
				m_term.AddCommand("echo", {}, [&](auto line) {
					debug::puts(line);
				});

				// exit
				m_term.AddCommand("exit", {}, [&](auto line) {
					get_window()->set_should_close(true);
				});

				// help
				m_term.AddCommand("help", {}, [&](auto line) {
					for (auto const & name : m_term.Commands.get<ds::string>()) {
						debug::puts("- {0}", name);
					}
				});

				// history
				m_term.AddCommand("history", {}, [&](auto line) {
					for (auto const & str : m_term.History) {
						debug::puts(str);
					}
				});

				// python
				m_term.AddCommand("python", {}, [&](auto line) {
					// lock
					if (m_term.Mode.empty() && line.empty()) {
						m_term.Mode = "python"; return;
					}
					// unlock
					else if (m_term.Mode == line && line == "python") {
						m_term.Mode.clear(); return;
					}
					// evaluate
					PyRun_SimpleString(line.c_str());
				});
			}))
			{
				ImGui::PopStyleVar(1);
			}
		}

		// VIEWPORT
		void draw_viewport_panel()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			if (!m_panels[viewport_panel]([&]() noexcept
			{
				ImGui::PopStyleVar(1);

				if (ImGui::BeginMenuBar()) {
					ImGuiExt::HelpMarker("viewport");
					ImGui::Separator();
					
					ImGui::ColorEdit4("clear color", m_clear_color,
						ImGuiColorEditFlags_NoInputs |
						ImGuiColorEditFlags_NoLabel);
					ImGui::Separator();

					auto const fps{ get_io()->fps };
					ImGui::TextDisabled("%.3f ms/frame ( %.1f fps )", 1000.f / fps, fps);
					ImGui::Separator();
					
					ImGui::EndMenuBar();
				}

				ImGui::Image(
					m_fb[0]->get_color_attachments()[0]->get_handle(),
					m_resolution = ImGui::GetContentRegionAvail(),
					{ 0, 1 },
					{ 1, 0 },
					colors::white,
					colors::clear);
			}))
			{
				ImGui::PopStyleVar(1);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API ml::plugin * ml_plugin_install(ml::plugin_manager * manager, void * userptr)
	{
		return manager->get_memory()->new_object<ml::sandbox>(manager, userptr);
	}

	ML_PLUGIN_API void ml_plugin_uninstall(ml::plugin_manager * manager, ml::plugin * ptr)
	{
		manager->get_memory()->delete_object((ml::sandbox *)ptr);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */