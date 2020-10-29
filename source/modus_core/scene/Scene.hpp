#ifndef _ML_SCENE_HPP_
#define _ML_SCENE_HPP_

#include <modus_core/Export.hpp>
#include <modus_core/system/Memory.hpp>
#include <modus_core/detail/Matrix.hpp>
#include <entt/entt.hpp>

namespace ml
{
	struct entity;

	struct ML_CORE_API scene : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type = typename pmr::polymorphic_allocator<byte_t>;

		using entity_list = typename ds::list<shared<entity>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		scene(allocator_type alloc = {}) noexcept;

		~scene() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		shared<entity> & create_entity(ds::string const & name, allocator_type alloc = {}) noexcept;

		void destroy_entity(shared<entity> const & value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path);

		bool load_from_memory(json const & j);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto get_entities() noexcept -> entity_list & { return m_entities; }

		ML_NODISCARD auto get_registry() noexcept -> entt::registry & { return m_registry; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		entity_list		m_entities	; // entities
		entt::registry	m_registry	; // registry

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_SCENE_HPP_