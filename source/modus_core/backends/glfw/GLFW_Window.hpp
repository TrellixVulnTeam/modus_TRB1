#ifndef _ML_GLFW_WINDOW_HPP_
#define _ML_GLFW_WINDOW_HPP_

#include <modus_core/window/BaseWindow.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace ml
{
	// glfw window
	struct glfw_window final : base_window
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_window::allocator_type;

		glfw_window(allocator_type alloc = {}) noexcept;

		~glfw_window() final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool open(
			string			const & title,
			video_mode			const & vm		= {},
			context_settings	const & cs		= {},
			window_hints_				hints	= window_hints_default
		) final;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void focus() final;

		void hide() final;

		void iconify() final;

		void maximize() final;

		void restore() final;

		void request_attention() final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_callbacks const & get_callbacks() const final;

		string get_clipboard() const final;

		vec2 get_content_scale() const final;

		vec2i get_framebuffer_size() const final;

		window_handle get_handle() const final;

		window_hints_ get_hints() const final;

		int32 get_input_mode(int32 mode) const final;

		int32 get_key(int32 key) const final;

		int32 get_mouse_button(int32 button) const final;

		vec2 get_mouse_pos() const final;

		window_handle get_native_handle() const final;

		float32 get_opacity() const final;

		vec2i get_position() const final;

		vec2i get_size() const final;

		string const & get_title() const final;

		void * get_user_pointer() const final;

		int_rect get_window_frame_size() const final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool is_auto_iconify() const final;

		bool is_decorated() const final;

		bool is_floating() const final;

		bool is_focus_on_show() const final;

		bool is_focused() const final;

		bool is_hovered() const final;

		bool is_iconified() const final;

		bool is_maximized() const final;

		bool is_open() const final;

		bool is_resizable() const final;

		bool is_transparent() const final;

		bool is_visible() const final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void set_auto_iconify(bool value) final;

		void set_clipboard(string const & value) final;
		
		void set_cursor(cursor_handle value) final;
		
		void set_cursor_mode(int32 value) final;
		
		void set_mouse_pos(vec2d const & value) final;

		void set_decorated(bool value) final;

		void set_floating(bool value) final;

		void set_focus_on_show(bool value) final;
		
		void set_icons(int32 n, size_t w, size_t h, byte const * p) final;

		void set_input_mode(int32 mode, int32 value) final;

		void set_opacity(float32 value) final;
		
		void set_position(vec2i const & value) final;
		
		void set_monitor(monitor_handle value, int_rect const & bounds = {}) final;

		void set_resizable(bool value) final;

		void set_should_close(bool value) final;

		void set_size(vec2i const & value) final;
		
		void set_title(string const & value) final;

		void set_user_pointer(void * value) final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_char_callback get_char_callback() const final;
		
		window_char_mods_callback get_char_mods_callback() const final;
		
		window_close_callback get_close_callback() const final;
		
		window_content_scale_callback get_content_scale_callback() const final;
		
		window_drop_callback get_drop_callback() const final;
		
		window_focus_callback get_focus_callback() const final;
		
		window_framebuffer_resize_callback get_framebuffer_resize_callback() const final;
		
		window_iconify_callback get_iconify_callback() const final;
		
		window_key_callback get_key_callback() const final;
		
		window_maximize_callback get_maximize_callback() const final;
		
		window_mouse_button_callback get_mouse_button_callback() const final;

		window_mouse_enter_callback get_mouse_enter_callback() const final;

		window_mouse_pos_callback get_mouse_pos_callback() const final;
		
		window_position_callback get_position_callback() const final;
		
		window_refresh_callback get_refresh_callback() const final;
		
		window_resize_callback get_resize_callback() const final;
		
		window_scroll_callback get_scroll_callback() const final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		window_char_callback set_char_callback(window_char_callback value) final;
		
		window_char_mods_callback set_char_mods_callback(window_char_mods_callback value) final;
		
		window_close_callback set_close_callback(window_close_callback value) final;
		
		window_content_scale_callback set_content_scale_callback(window_content_scale_callback value) final;
		
		window_drop_callback set_drop_callback(window_drop_callback value) final;
		
		window_focus_callback set_focus_callback(window_focus_callback value) final;
		
		window_framebuffer_resize_callback set_framebuffer_resize_callback(window_framebuffer_resize_callback value) final;
		
		window_iconify_callback set_iconify_callback(window_iconify_callback value) final;
		
		window_key_callback set_key_callback(window_key_callback value) final;
		
		window_maximize_callback set_maximize_callback(window_maximize_callback value) final;
		
		window_mouse_button_callback set_mouse_button_callback(window_mouse_button_callback value) final;

		window_mouse_enter_callback set_mouse_enter_callback(window_mouse_enter_callback value) final;

		window_mouse_pos_callback set_mouse_pos_callback(window_mouse_pos_callback value) final;
		
		window_position_callback set_position_callback(window_position_callback value) final;
		
		window_refresh_callback set_refresh_callback(window_refresh_callback value) final;
		
		window_resize_callback set_resize_callback(window_resize_callback value) final;
		
		window_scroll_callback set_scroll_callback(window_scroll_callback value) final;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		string			m_title		; // 
		GLFWwindow	*		m_window	; // 
		GLFWmonitor	*		m_monitor	; // 
		window_hints_		m_hints		; // 
		window_callbacks	m_clbk		; //

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // _ML_GLFW_WINDOW_HPP_