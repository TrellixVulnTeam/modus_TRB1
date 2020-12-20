#ifndef _ML_RENDER_API_HPP_
#define _ML_RENDER_API_HPP_

#include <modus_core/window/ContextSettings.hpp>
#include <modus_core/graphics/RenderUtility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// render device
namespace ml::gfx
{
	// render device info
	struct ML_NODISCARD device_info final
	{
		// version
		ds::string renderer, vendor, version;
		int32 major_version, minor_version;
		ds::list<ds::string> extensions;

		// textures
		bool texture_edge_clamp_available;
		uint32 max_texture_slots;

		// framebuffers
		uint32 max_color_attachments;
		uint32 max_samples;

		// shaders
		bool shaders_available;
		bool geometry_shaders_available;
		ds::string shading_language_version;
	};

	// render device specification
	template <> struct ML_NODISCARD spec<render_device> final
	{
		int32 api{ context_api_opengl };
	};

	// base render device
	struct ML_CORE_API render_device : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		using spec_type = typename spec<render_device>;

		using allocator_type = typename pmr::polymorphic_allocator<byte>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual ~render_device() override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static render_device * create(spec_type const & desc = {}, allocator_type alloc = {});

		static void destroy(render_device * value = nullptr);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual object_id get_handle() const noexcept = 0;

		ML_NODISCARD virtual device_info const & get_info() const noexcept = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual ds::ref<render_context> const & get_active_context() const noexcept = 0;

		virtual void set_active_context(ds::ref<render_context> const & value) noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual ds::ref<render_context> new_context(context_settings const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<vertexarray> new_vertexarray(spec<vertexarray> const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<vertexbuffer> new_vertexbuffer(spec<vertexbuffer> const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<indexbuffer> new_indexbuffer(spec<indexbuffer> const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<texture2d> new_texture2d(spec<texture2d> const & desc, allocator_type alloc = {}) noexcept = 0;
		
		ML_NODISCARD virtual ds::ref<texture3d> new_texture3d(spec<texture3d> const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<texturecube> new_texturecube(spec<texturecube> const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<framebuffer> new_framebuffer(spec<framebuffer> const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<program> new_program(spec<program> const & desc, allocator_type alloc = {}) noexcept = 0;

		ML_NODISCARD virtual ds::ref<shader> new_shader(spec<shader> const & desc, allocator_type alloc = {}) noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual ds::list<ds::unown<render_context>> const & get_contexts() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<vertexarray>> const & get_vertexarrays() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<vertexbuffer>> const & get_vertexbuffers() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<indexbuffer>> const & get_indexbuffers() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<texture2d>> const & get_texture2ds() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<texture3d>> const & get_texture3ds() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<texturecube>> const & get_texturecubes() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<framebuffer>> const & get_framebuffers() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<program>> const & get_programs() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::unown<shader>> const & get_shaders() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// allocate device
	template <class ... Args
	> ML_NODISCARD auto alloc_device(render_device::allocator_type alloc, Args && ... args) noexcept
	{
		return render_device::create(render_device::spec_type{ ML_forward(args)... }, alloc);
	}

	// make device
	template <class ... Args
	> ML_NODISCARD auto make_device(Args && ... args) noexcept
	{
		return render_device::create(render_device::spec_type{ ML_forward(args)... });
	}

	// destroy device
	inline void destroy_device(render_device * value) noexcept
	{
		render_device::destroy(value);
	}
}

// global render device
namespace ml::globals
{
	ML_decl_global(gfx::render_device) get() noexcept;

	ML_decl_global(gfx::render_device) set(gfx::render_device * value) noexcept;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// render object
namespace ml::gfx
{
	// base render object
	template <class Derived
	> class render_object : non_copyable, trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static constexpr typeof_t<> s_base_type{ typeof_v<Derived> };

		render_device * const m_parent;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		using allocator_type = typename pmr::polymorphic_allocator<byte>;

		explicit render_object(render_device * parent) noexcept : m_parent{ ML_check(parent) }
		{
		}

		virtual ~render_object() override = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual object_id get_handle() const noexcept = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto get_base_type() const noexcept -> typeof_t<> const & { return s_base_type; }

		ML_NODISCARD inline auto get_device() const noexcept -> render_device * { return m_parent; }

		ML_NODISCARD inline auto get_context() const noexcept -> ds::ref<render_context> const &
		{
			return m_parent->get_active_context();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// render context
namespace ml::gfx
{
	// base render context
	struct ML_CORE_API render_context : public render_object<render_context>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		using spec_type = typename context_settings;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_context(ML_forward(desc), alloc);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		explicit render_context(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~render_context() override = default;

		virtual bool revalue() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

		ML_NODISCARD virtual spec_type const & get_spec() const noexcept = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Arg0, class ... Args
		> void execute(Arg0 && arg0, Args && ... args) noexcept
		{
			std::invoke(ML_forward(arg0), this);

			meta::for_args([&](auto && e) noexcept
			{
				std::invoke(ML_forward(e), this);
			}
			, ML_forward(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual alpha_state * get_alpha_state(alpha_state * value = {}) const = 0;
		
		virtual blend_state * get_blend_state(blend_state * value = {}) const = 0;

		virtual color * get_clear_color(color * value = {}) const = 0;

		virtual cull_state * get_cull_state(cull_state * value = {}) const = 0;

		virtual depth_state * get_depth_state(depth_state * value = {}) const = 0;

		virtual stencil_state * get_stencil_state(stencil_state * value = {}) const = 0;

		virtual int_rect * get_viewport(int_rect * value = {}) const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void set_alpha_state(alpha_state const & value) = 0;
		
		virtual void set_blend_state(blend_state const & value) = 0;

		virtual void set_clear_color(color const & value) = 0;

		virtual void set_cull_state(cull_state const & value) = 0;

		virtual void set_depth_state(depth_state const & value) = 0;

		virtual void set_stencil_state(stencil_state const & value) = 0;

		virtual void set_viewport(int_rect const & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void clear(uint32 mask) = 0;

		virtual void draw(ds::ref<vertexarray> const & value) = 0;

		virtual void draw_arrays(uint32 prim, size_t first, size_t count) = 0;

		virtual void draw_indexed(uint32 prim, size_t count) = 0;

		virtual void flush() = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void bind_vertexarray(vertexarray const * value) = 0;

		virtual void bind_vertexbuffer(vertexbuffer const * value) = 0;

		virtual void bind_indexbuffer(indexbuffer const * value) = 0;

		virtual void bind_texture(texture const * value, uint32 slot = 0) = 0;

		virtual void bind_framebuffer(framebuffer const * value) = 0;

		virtual void bind_program(program const * value) = 0;

		virtual void bind_shader(shader const * value) = 0; // WIP

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void upload(uniform_id loc, bool value) = 0;

		virtual void upload(uniform_id loc, int32 value) = 0;

		virtual void upload(uniform_id loc, float32 value) = 0;

		virtual void upload(uniform_id loc, vec2f const & value) = 0;

		virtual void upload(uniform_id loc, vec3f const & value) = 0;

		virtual void upload(uniform_id loc, vec4f const & value) = 0;

		virtual void upload(uniform_id loc, mat2f const & value) = 0;

		virtual void upload(uniform_id loc, mat3f const & value) = 0;

		virtual void upload(uniform_id loc, mat4f const & value) = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	// allocate context
	template <class ... Args
	> ML_NODISCARD auto alloc_context(render_context::allocator_type alloc, Args && ... args) noexcept
	{
		return render_context::create(render_context::spec_type{ ML_forward(args)... }, alloc);
	}

	// make context
	template <class ... Args
	> ML_NODISCARD auto make_context(Args && ... args) noexcept
	{
		return render_context::create(render_context::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexarray
namespace ml::gfx
{
	// vertexarray specification
	template <> struct ML_NODISCARD spec<vertexarray> final
	{
		uint32 prim{ primitive_triangles };
	};

	static void from_json(json const & j, spec<vertexarray> & v)
	{
		j["prim"].get_to(v.prim);
	}

	static void to_json(json & j, spec<vertexarray> const & v)
	{
		j["prim"] = v.prim;
	}


	// base vertexarray
	struct ML_CORE_API vertexarray : public render_object<vertexarray>
	{
	public:
		using spec_type = typename spec<vertexarray>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_vertexarray(ML_forward(desc), alloc);
		}

	public:
		explicit vertexarray(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~vertexarray() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual void add_vertices(ds::ref<vertexbuffer> const & value) = 0;

		virtual void set_layout(buffer_layout const & value) = 0;

		virtual void set_indices(ds::ref<indexbuffer> const & value) = 0;

		ML_NODISCARD virtual buffer_layout const & get_layout() const noexcept = 0;

		ML_NODISCARD virtual ds::ref<indexbuffer> const & get_indices() const noexcept = 0;

		ML_NODISCARD virtual uint32 get_mode() const noexcept = 0;
		
		ML_NODISCARD virtual ds::list<ds::ref<vertexbuffer>> const & get_vertices() const noexcept = 0;

	public:
		inline void bind() const noexcept
		{
			get_context()->bind_vertexarray(this);
		}

		inline void unbind() const noexcept
		{
			get_context()->bind_vertexarray(nullptr);
		}
	};

	// allocate vertexarray
	template <class ... Args
	> ML_NODISCARD auto alloc_vertexarray(vertexarray::allocator_type alloc, Args && ... args) noexcept
	{
		return vertexarray::create(vertexarray::spec_type{ ML_forward(args)... }, alloc);
	}

	// make vertexarray
	template <class ... Args
	> ML_NODISCARD auto make_vertexarray(Args && ... args) noexcept
	{
		return vertexarray::create(vertexarray::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// vertexbuffer
namespace ml::gfx
{
	// vertexbuffer specification
	template <> struct ML_NODISCARD spec<vertexbuffer> final
	{
		uint32	usage	{ usage_static };
		size_t	count	{};
		addr_t	data	{ nullptr };
	};

	static void from_json(json const & j, spec<vertexbuffer> & v)
	{
		j["usage"].get_to(v.usage);
	}

	static void to_json(json & j, spec<vertexbuffer> const & v)
	{
		j["usage"] = v.usage;
	}


	// base vertexbuffer
	struct ML_CORE_API vertexbuffer : public render_object<vertexbuffer>
	{
	public:
		using spec_type = typename spec<vertexbuffer>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_vertexbuffer(ML_forward(desc), alloc);
		}

	public:
		explicit vertexbuffer(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~vertexbuffer() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual void set_data(size_t count, addr_t data, size_t offset = 0) = 0;

		ML_NODISCARD virtual buffer_t const & get_buffer() const noexcept = 0;

		ML_NODISCARD virtual size_t get_count() const noexcept = 0;

		ML_NODISCARD virtual size_t get_size() const noexcept = 0;

		ML_NODISCARD virtual uint32 get_usage() const noexcept = 0;

	public:
		inline void bind() const noexcept
		{
			get_context()->bind_vertexbuffer(this);
		}

		inline void unbind() const noexcept
		{
			get_context()->bind_vertexbuffer(nullptr);
		}
	};

	// allocate vertexbuffer
	template <class ... Args
	> ML_NODISCARD auto alloc_vertexbuffer(vertexbuffer::allocator_type alloc, Args && ... args) noexcept
	{
		return vertexbuffer::create(vertexbuffer::spec_type{ ML_forward(args)... }, alloc);
	}

	// make vertexbuffer
	template <class ... Args
	> ML_NODISCARD auto make_vertexbuffer(Args && ... args) noexcept
	{
		return vertexbuffer::create(vertexbuffer::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// indexbuffer
namespace ml::gfx
{
	// indexbuffer specification
	template <> struct ML_NODISCARD spec<indexbuffer> final
	{
		uint32	usage	{ usage_static };
		size_t	count	{};
		addr_t	data	{ nullptr };
	};

	static void from_json(json const & j, spec<indexbuffer> & v)
	{
		j["usage"].get_to(v.usage);
		j["count"].get_to(v.count);
	}

	static void to_json(json & j, spec<indexbuffer> const & v)
	{
		j["usage"] = v.usage;
		j["count"] = v.count;
	}


	// base indexbuffer
	struct ML_CORE_API indexbuffer : public render_object<indexbuffer>
	{
	public:
		using spec_type = typename spec<indexbuffer>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_indexbuffer(ML_forward(desc), alloc);
		}

	public:
		explicit indexbuffer(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~indexbuffer() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual void set_data(size_t count, addr_t data, size_t offset = 0) = 0;

		ML_NODISCARD virtual buffer_t const & get_buffer() const noexcept = 0;

		ML_NODISCARD virtual size_t get_count() const noexcept = 0;
		
		ML_NODISCARD virtual size_t get_size() const noexcept = 0;

		ML_NODISCARD virtual uint32 get_usage() const noexcept = 0;

	public:
		inline void bind() const noexcept
		{
			get_context()->bind_indexbuffer(this);
		}

		inline void unbind() const noexcept
		{
			get_context()->bind_indexbuffer(nullptr);
		}
	};

	// allocate indexbuffer
	template <class ... Args
	> ML_NODISCARD auto alloc_indexbuffer(indexbuffer::allocator_type alloc, Args && ... args) noexcept
	{
		return indexbuffer::create(indexbuffer::spec_type{ ML_forward(args)... }, alloc);
	}

	// make indexbuffer
	template <class ... Args
	> ML_NODISCARD auto make_indexbuffer(Args && ... args) noexcept
	{
		return indexbuffer::create(indexbuffer::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture
namespace ml::gfx
{
	// texture flags
	enum texture_flags_ : int32
	{
		texture_flags_none		= 0 << 0, // none
		texture_flags_smooth	= 1 << 0, // smooth
		texture_flags_repeat	= 1 << 1, // repeat
		texture_flags_mipmap	= 1 << 2, // mipmap

		// smooth / repeat
		texture_flags_default
			= texture_flags_smooth
			| texture_flags_repeat,
	};

	ML_NODISCARD constexpr auto & operator&=(texture_flags_ & lhs, int32 const rhs) noexcept
	{
		return (texture_flags_ &)((int32 &)lhs &= rhs);
	}

	ML_NODISCARD constexpr auto & operator|=(texture_flags_ & lhs, int32 const rhs) noexcept
	{
		return (texture_flags_ &)((int32 &)lhs |= rhs);
	}

	static void from_json(json const & j, texture_flags_ & v)
	{
		ML_flag_write((int32 &)v, texture_flags_repeat, j["repeat"].get<bool>());
		ML_flag_write((int32 &)v, texture_flags_smooth, j["smooth"].get<bool>());
		ML_flag_write((int32 &)v, texture_flags_mipmap, j["mipmap"].get<bool>());
	}

	static void to_json(json & j, texture_flags_ const & v)
	{
		j["repeat"] = ML_flag_read(v, texture_flags_repeat);
		j["smooth"] = ML_flag_read(v, texture_flags_smooth);
		j["mipmap"] = ML_flag_read(v, texture_flags_mipmap);
	}


	// texture format
	struct ML_NODISCARD texture_format final
	{
		uint32
			color	{ format_rgba },
			pixel	{ color },
			type	{ type_ubyte };
	};

	static void from_json(json const & j, texture_format & v)
	{
		j["color"	].get_to(v.color);
		j["pixel"	].get_to(v.pixel);
		j["type"	].get_to(v.type);
	}

	static void to_json(json & j, texture_format const & v)
	{
		j["color"	] = v.color;
		j["pixel"	] = v.pixel;
		j["type"	] = v.type;
	}

	// base texture
	struct ML_CORE_API texture : public render_object<texture>
	{
	public:
		explicit texture(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~texture() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual void lock() = 0;

		virtual void unlock() = 0;

		ML_NODISCARD virtual uint32 get_type() const noexcept = 0;

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;
		
		ML_NODISCARD virtual texture_format const & get_format() const noexcept = 0;
		
		ML_NODISCARD virtual texture_flags_ get_flags() const noexcept = 0;

	public:
		inline void bind(uint32 slot = 0) const noexcept
		{
			get_context()->bind_texture(this, slot);
		}

		inline void unbind(uint32 slot = 0) const noexcept
		{
			get_context()->bind_texture(nullptr, slot);
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture2d
namespace ml::gfx
{
	// texture2d specification
	template <> struct ML_NODISCARD spec<texture2d> final
	{
		vec2i			size	{};
		texture_format	format	{ format_rgba };
		texture_flags_	flags	{ texture_flags_default };
		addr_t			data	{ nullptr };
	};

	static void from_json(json const & j, spec<texture2d> & v)
	{
		j["size"	].get_to(v.size);
		j["format"	].get_to(v.format);
		j["flags"	].get_to(v.flags);
	}

	static void to_json(json & j, spec<texture2d> const & v)
	{
		j["size"	] = v.size;
		j["format"	] = v.format;
		j["flags"	] = v.flags;
	}


	// base texture2d
	struct ML_CORE_API texture2d : public texture
	{
	public:
		using spec_type = typename spec<texture2d>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_texture2d(ML_forward(desc), alloc);
		}

		ML_NODISCARD static auto create(bitmap const & img, texture_flags_ flags = texture_flags_default, allocator_type alloc = {}) noexcept
		{
			return create({ img.size(), { calc_channel_format(img.channels()) }, flags, img.data() }, alloc);
		}

		ML_NODISCARD static auto create(fs::path const & path, texture_flags_ flags = texture_flags_default, allocator_type alloc = {}) noexcept
		{
			return create(bitmap{ path, alloc }, flags, alloc);
		}

	public:
		explicit texture2d(render_device * parent) noexcept : texture{ parent } {}

		virtual ~texture2d() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual void lock() override = 0;

		virtual void unlock() override = 0;

		virtual void update(vec2i const & size, addr_t data = {}) = 0;

		virtual void update(vec2i const & pos, vec2i const & size, addr_t data) = 0;

		virtual void set_mipmapped(bool value) = 0;

		virtual void set_repeated(bool value) = 0;

		virtual void set_smooth(bool value) = 0;

		ML_NODISCARD virtual bitmap copy_to_image() const = 0;

		ML_NODISCARD inline uint32 get_type() const noexcept { return texture_type_2d; }

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;

		ML_NODISCARD virtual texture_format const & get_format() const noexcept = 0;

		ML_NODISCARD virtual texture_flags_ get_flags() const noexcept = 0;
	};

	// allocate texture2d
	template <class ... Args
	> ML_NODISCARD auto alloc_texture2d(texture2d::allocator_type alloc, Args && ... args) noexcept
	{
		return texture2d::create(texture2d::spec_type{ ML_forward(args)... }, alloc);
	}

	// make texture2d
	template <class ... Args
	> ML_NODISCARD auto make_texture2d(Args && ... args) noexcept
	{
		return texture2d::create(texture2d::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texture3d (WIP)
namespace ml::gfx
{
	// texture3d specification
	template <> struct ML_NODISCARD spec<texture3d> final
	{
		vec2i			size	{};
		texture_format	format	{ format_rgba };
		texture_flags_	flags	{ texture_flags_default };
	};

	static void from_json(json const & j, spec<texture3d> & v)
	{
		j["size"	].get_to(v.size);
		j["format"	].get_to(v.format);
		j["flags"	].get_to(v.flags);
		
	}

	static void to_json(json & j, spec<texture3d> const & v)
	{
		j["size"	] = v.size;
		j["format"	] = v.format;
		j["flags"	] = v.flags;
	}


	// base texture3d
	struct ML_CORE_API texture3d : public texture
	{
	public:
		using spec_type = typename spec<texture3d>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_texture3d(ML_forward(desc), alloc);
		}

	public:
		explicit texture3d(render_device * parent) noexcept : texture{ parent }
		{
		}

		virtual ~texture3d() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual void lock() override = 0;

		virtual void unlock() override = 0;

		ML_NODISCARD inline uint32 get_type() const noexcept override { return texture_type_cube; }

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;

		ML_NODISCARD virtual texture_format const & get_format() const noexcept = 0;

		ML_NODISCARD virtual texture_flags_ get_flags() const noexcept = 0;
	};

	// allocate texture3d
	template <class ... Args
	> ML_NODISCARD auto alloc_texture3d(texture3d::allocator_type alloc, Args && ... args) noexcept
	{
		return texture3d::create(texture3d::spec_type{ ML_forward(args)... }, alloc);
	}

	// make texture3d
	template <class ... Args
	> ML_NODISCARD auto make_texture3d(Args && ... args) noexcept
	{
		return texture3d::create(texture3d::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// texturecube (WIP)
namespace ml::gfx
{
	// texturecube specification
	template <> struct ML_NODISCARD spec<texturecube> final
	{
		vec2i			size	{};
		texture_format	format	{ format_rgba };
		texture_flags_	flags	{ texture_flags_default };
	};

	static void from_json(json const & j, spec<texturecube> & v)
	{
		j["size"	].get_to(v.size);
		j["format"	].get_to(v.format);
		j["flags"	].get_to(v.flags);
		
	}

	static void to_json(json & j, spec<texturecube> const & v)
	{
		j["size"	] = v.size;
		j["format"	] = v.format;
		j["flags"	] = v.flags;
	}


	// base texturecube
	struct ML_CORE_API texturecube : public texture
	{
	public:
		using spec_type = typename spec<texturecube>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_texturecube(ML_forward(desc), alloc);
		}

	public:
		explicit texturecube(render_device * parent) noexcept : texture{ parent } {}

		virtual ~texturecube() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual void lock() override = 0;

		virtual void unlock() override = 0;

		ML_NODISCARD inline uint32 get_type() const noexcept override { return texture_type_cube; }

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;

		ML_NODISCARD virtual texture_format const & get_format() const noexcept = 0;

		ML_NODISCARD virtual texture_flags_ get_flags() const noexcept = 0;
	};

	// allocate texturecube
	template <class ... Args
	> ML_NODISCARD auto alloc_texturecube(texturecube::allocator_type alloc, Args && ... args) noexcept
	{
		return texturecube::create(texturecube::spec_type{ ML_forward(args)... }, alloc);
	}

	// make texturecube
	template <class ... Args
	> ML_NODISCARD auto make_texturecube(Args && ... args) noexcept
	{
		return texturecube::create(texturecube::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// framebuffer
namespace ml::gfx
{
	// framebuffer specification
	template <> struct ML_NODISCARD ML_CORE_API spec<framebuffer> final
	{
		vec2i			size			{ 1280, 720 };
		texture_format	format			{ format_rgba };
		texture_flags_	flags			{ texture_flags_default };
		vec4i			bits_per_pixel	{ 8, 8, 8, 8 };
		int32			stencil_bits	{ 24 },
						depth_bits		{ 8 };
		int32			samples			{};
		bool			stereo			{};
	};

	static void from_json(json const & j, spec<framebuffer> & v)
	{
		j["size"			].get_to(v.size);
		j["format"			].get_to(v.format);
		j["flags"			].get_to(v.flags);
		j["bits_per_pixel"	].get_to(v.bits_per_pixel);
		j["stencil_bits"	].get_to(v.stencil_bits);
		j["depth_bits"		].get_to(v.depth_bits);
		j["samples"			].get_to(v.samples);
		j["stereo"			].get_to(v.stereo);
	}

	static void to_json(json & j, spec<framebuffer> const & v)
	{
		j["size"			] = v.size;
		j["format"			] = v.format;
		j["flags"			] = v.flags;
		j["bits_per_pixel"	] = v.bits_per_pixel;
		j["stencil_bits"	] = v.stencil_bits;
		j["depth_bits"		] = v.depth_bits;
		j["samples"			] = v.samples;
		j["stereo"			] = v.stereo;
	}


	// base framebuffer
	struct ML_CORE_API framebuffer : public render_object<framebuffer>
	{
	public:
		using spec_type = typename spec<framebuffer>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_framebuffer(ML_forward(desc), alloc);
		}

	public:
		explicit framebuffer(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~framebuffer() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual bool attach(ds::ref<texture2d> const & value) = 0;

		virtual bool detach(ds::ref<texture2d> const & value) = 0;

		virtual void resize(vec2i const & value) = 0;

		ML_NODISCARD virtual ds::list<ds::ref<texture2d>> const & get_color_attachments() const noexcept = 0;
		
		ML_NODISCARD virtual ds::ref<texture2d> const & get_depth_attachment() const noexcept = 0;

		ML_NODISCARD virtual vec2i const & get_size() const noexcept = 0;

		ML_NODISCARD virtual texture_format const & get_format() const noexcept = 0;

		ML_NODISCARD virtual texture_flags_ get_flags() const noexcept = 0;

		ML_NODISCARD virtual vec4i const & get_bits_per_pixel() const noexcept = 0;

		ML_NODISCARD virtual int32 get_stencil_bits() const noexcept = 0;
		
		ML_NODISCARD virtual int32 get_depth_bits() const noexcept = 0;
		
		ML_NODISCARD virtual int32 get_sample_count() const noexcept = 0;
		
		ML_NODISCARD virtual bool is_stereo() const noexcept = 0;

	public:
		inline void bind() const noexcept
		{
			get_context()->bind_framebuffer(this);
		}

		inline void unbind() const noexcept
		{
			get_context()->bind_framebuffer(nullptr);
		}

		inline void bind_texture(uint32 slot = 0) const noexcept
		{
			get_context()->bind_texture
			(
				get_color_attachments()[(size_t)slot].get(), slot
			);
		}
	};

	// allocate framebuffer
	template <class ... Args
	> ML_NODISCARD auto alloc_framebuffer(framebuffer::allocator_type alloc, Args && ... args) noexcept
	{
		return framebuffer::create(framebuffer::spec_type{ ML_forward(args)... }, alloc);
	}

	// make framebuffer
	template <class ... Args
	> ML_NODISCARD auto make_framebuffer(Args && ... args) noexcept
	{
		return framebuffer::create(framebuffer::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// program
namespace ml::gfx
{
	// program specification
	template <> struct ML_NODISCARD spec<program> final
	{
	};

	// base program
	struct ML_CORE_API program : public render_object<program>
	{
	public:
		using spec_type = typename spec<program>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_program(ML_forward(desc), alloc);
		}

	public:
		explicit program(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~program() override = default;

		virtual bool revalue() = 0;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual bool attach(uint32 type, size_t count, cstring * str, int32 const * len = {}) = 0;

		inline bool attach(uint32 type, ds::string const & str) noexcept
		{
			if (str.empty()) { return false; }
			cstring temp{ str.c_str() };
			return attach(type, 1, &temp, nullptr);
		}

		inline bool attach(uint32 type, ds::list<ds::string> const & str) noexcept
		{
			if (str.empty()) { return false; }
			cstring temp{ str.front().c_str() };
			return attach(type, str.size(), &temp, nullptr);
		}

		virtual bool detach(uint32 type) = 0;

		virtual bool link() = 0;

		virtual bool bind_uniform(cstring name, ds::method<void(uniform_id)> const & fn) = 0;

		ML_NODISCARD virtual uniform_id get_uniform_location(cstring name) noexcept = 0;

		ML_NODISCARD virtual ds::string const & get_info_log() const noexcept = 0;

		ML_NODISCARD virtual ds::array<object_id, shader_type_MAX> const & get_shaders() const noexcept = 0;

		ML_NODISCARD virtual ds::map<uint32, ds::list<ds::string>> const & get_source() const noexcept = 0;

		ML_NODISCARD virtual ds::map<uniform_id, ds::ref<texture>> const & get_textures() const noexcept = 0;

		ML_NODISCARD virtual ds::map<hash_t, uniform_id> const & get_uniforms() const noexcept = 0;

		ML_NODISCARD virtual uint32 get_mask() const noexcept = 0;

	public:
		inline void bind() const noexcept
		{
			get_context()->bind_program(this);
		}

		inline void unbind() const noexcept
		{
			get_context()->bind_program(nullptr);
		}

		inline void bind_textures() const noexcept
		{
			uint32 slot{};
			get_textures().for_each([&, ctx = ML_check(get_context().get())
			](uniform_id loc, ds::ref<texture> const & tex) noexcept
			{
				ctx->bind_texture(tex.get(), slot);

				ctx->upload(loc, (int32)slot++);
			});
		}

		template <class T> bool set_uniform(cstring name, T && value) noexcept
		{
			return bind_uniform(name, [&](uniform_id loc) noexcept
			{
				if constexpr (std::is_convertible_v<T, ds::ref<texture>>)
				{
					do_cache_texture(loc, ML_forward(value));
				}
				else
				{
					get_context()->upload(loc, ML_forward(value));
				}
			});
		}

	protected:
		virtual void do_cache_texture(uniform_id loc, ds::ref<texture> const & value) noexcept = 0;
	};

	// allocate program
	template <class ... Args
	> ML_NODISCARD auto alloc_program(program::allocator_type alloc, Args && ... args) noexcept
	{
		return program::create(program::spec_type{ ML_forward(args)... }, alloc);
	}

	// make program
	template <class ... Args
	> ML_NODISCARD auto make_program(Args && ... args) noexcept
	{
		return program::create(program::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// shader (WIP)
namespace ml::gfx
{
	// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_separate_shader_objects.txt

	// shader bit
	enum shader_bit_ : uint32
	{
		shader_bit_vertex		= 1,
		shader_bit_pixel		= 2,
		shader_bit_geometry		= 4,
		shader_bit_tess_ctrl	= 8,
		shader_bit_tess_eval	= 10,
		shader_bit_all			= 0xffffffff
	};

	// shader specification
	template <> struct ML_NODISCARD spec<shader> final
	{
		uint32					type	{ shader_type_vertex };
		ds::list<ds::string>	code	{};
	};

	static void from_json(json const & j, spec<shader> & v)
	{
		j["type"].get_to(v.type);
		j["code"].get_to(v.code);
	}

	static void to_json(json & j, spec<shader> const & v)
	{
		j["type"] = v.type;
		j["code"] = v.code;
	}


	// base shader
	struct ML_CORE_API shader : public render_object<shader>
	{
	public:
		using spec_type = typename spec<shader>;

		template <class Desc = spec_type
		> ML_NODISCARD static auto create(Desc && desc, allocator_type alloc = {}) noexcept
		{
			return ML_check(get_global<render_device>())->new_shader(ML_forward(desc), alloc);
		}

	public:
		explicit shader(render_device * parent) noexcept : render_object{ parent } {}

		virtual ~shader() override = default;

		ML_NODISCARD virtual object_id get_handle() const noexcept override = 0;

		ML_NODISCARD virtual typeof_t<> const & get_self_type() const noexcept override = 0;

	public:
		virtual bool compile(uint32 type, size_t count, cstring * str, int32 const * len = {}) = 0;

		virtual bool bind_uniform(cstring name, ds::method<void(uniform_id)> const & fn) = 0;

		ML_NODISCARD virtual ds::string const & get_info_log() const noexcept = 0;

		ML_NODISCARD virtual ds::list<ds::string> const & get_source() const noexcept = 0;

		ML_NODISCARD virtual ds::map<uniform_id, ds::ref<texture>> const & get_textures() const noexcept = 0;

		ML_NODISCARD virtual uint32 get_type() const noexcept = 0;

		ML_NODISCARD virtual uint32 get_mask() const noexcept = 0;

	public:
		inline void bind() const noexcept
		{
			get_context()->bind_shader(this);
		}

		inline void unbind() const noexcept
		{
			get_context()->bind_shader(nullptr);
		}

		inline void bind_textures() noexcept
		{
			uint32 slot{};
			get_textures().for_each(
			[&](uniform_id loc, ds::ref<texture> const & tex) noexcept
			{
				do_upload(loc, tex, slot++);
			});
		}

		template <class T, class ... Extra
		> bool set_uniform(cstring name, T && value, Extra && ... extra) noexcept
		{
			return bind_uniform(name, [&](uniform_id loc) noexcept
			{
				if constexpr (std::is_convertible_v<T, ds::ref<texture>>)
				{
					do_cache(loc, ML_forward(value));
				}
				else
				{
					do_upload(ML_forward(value), ML_forward(extra)...);
				}
			});
		}

	protected:
		virtual void do_cache(uniform_id loc, ds::ref<texture> const & value) = 0;

		virtual void do_upload(uniform_id loc, bool value) = 0;

		virtual void do_upload(uniform_id loc, int32 value) = 0;

		virtual void do_upload(uniform_id loc, uint32 value) = 0;

		virtual void do_upload(uniform_id loc, float32 value) = 0;

		virtual void do_upload(uniform_id loc, vec2f const & value) = 0;

		virtual void do_upload(uniform_id loc, vec3f const & value) = 0;

		virtual void do_upload(uniform_id loc, vec4f const & value) = 0;

		virtual void do_upload(uniform_id loc, mat2f const & value, bool transpose = false) = 0;

		virtual void do_upload(uniform_id loc, mat3f const & value, bool transpose = false) = 0;

		virtual void do_upload(uniform_id loc, mat4f const & value, bool transpose = false) = 0;

		virtual void do_upload(uniform_id loc, ds::ref<texture> const & value, uint32 slot = 0) = 0;
	};

	// allocate shader
	template <class ... Args
	> ML_NODISCARD auto alloc_shader(shader::allocator_type alloc, Args && ... args) noexcept
	{
		return shader::create(shader::spec_type{ ML_forward(args)... }, alloc);
	}

	// make shader
	template <class ... Args
	> ML_NODISCARD auto make_shader(Args && ... args) noexcept
	{
		return shader::create(shader::spec_type{ ML_forward(args)... });
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RENDER_API_HPP_