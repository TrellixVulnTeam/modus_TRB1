#ifndef _ML_EVENTS_HPP_
#define _ML_EVENTS_HPP_

#include <core/detail/FlatMap.hpp>
#include <core/system/Memory.hpp>

// event declarator helper
#define ML_decl_event(Type) \
	struct Type final : _ML impl::event_helper<Type>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TYPES
	struct event			; // event base
	struct event_listener	; // event listener
	struct event_bus		; // event bus

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// EVENT BASE
	struct event : non_copyable
	{
		ML_NODISCARD bool consume() noexcept { return !m_used && (m_used = true); }

		ML_NODISCARD bool used() const noexcept { return m_used; }

		ML_NODISCARD explicit constexpr operator bool() const noexcept { return !m_used; }

		ML_NODISCARD constexpr operator hash_t() const noexcept { return m_id; }

	protected:
		constexpr explicit event(hash_t id) noexcept : m_id{ id }, m_used{}
		{
		}

		hash_t const	m_id	; // 
		bool			m_used	; // 
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT HELPER
	namespace impl
	{
		template <class Derived
		> struct event_helper : event
		{
			enum : hash_t { ID = hashof_v<Derived> };

			constexpr event_helper() noexcept : event{ ID }
			{
			}
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT LISTENER
	struct ML_CORE_API event_listener
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit event_listener(event_bus * bus, bool enabled = true) noexcept
			: m_bus		{ bus }
			, m_enabled	{ enabled }
		{
			ML_assert_msg(bus, "INVALID EVENT BUS");
		}

		virtual ~event_listener() noexcept; // EOF

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual void on_event(event &&) = 0;

		ML_NODISCARD event_bus * get_bus() const noexcept { return m_bus; }

		ML_NODISCARD bool is_listening() const noexcept { return m_enabled; }

		bool enable_listening(bool value) noexcept { return m_enabled = value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		template <class Ev
		> bool subscribe() noexcept
		{
			return ML_check(m_bus)->add_listener<Ev>(this);
		}

		template <class ... Args
		> bool subscribe(Args && ... args) noexcept
		{
			return ML_check(m_bus)->add_listener(ML_forward(args)..., this);
		}

		template <class Ev
		> void unsubscribe() noexcept
		{
			return ML_check(m_bus)->remove_listener<Ev>(this);
		}

		template <class ... Args
		> void unsubscribe(Args && ... args) noexcept
		{
			return ML_check(m_bus)->remove_listener(ML_forward(args)..., this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		event_bus * const	m_bus		; // 
		bool				m_enabled	; // 

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// EVENT BUS
	struct event_bus final : trackable, non_copyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type	= typename pmr::polymorphic_allocator<byte_t>;
		using category_type		= typename ds::set<event_listener *>;
		using categories_type	= typename ds::map<hash_t, category_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		event_bus(allocator_type alloc = {}) noexcept : m_categories{ alloc }
		{
		}

		~event_bus() noexcept = default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void fire(event && ev) noexcept
		{
			if (!ev) { return; }
			else if (auto const cat{ m_categories.find(ev) })
			{
				for (auto const listener : (*cat->second))
				{
					if (!ev) { return; }
					else if (listener->is_listening())
					{
						listener->on_event(ML_forward(ev));
					}
				}
			}
		}

		template <class Ev, class ... Args
		> void fire(Args && ... args) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			this->fire(Ev{ ML_forward(args)... });
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool add_listener(hash_t id, event_listener * value) noexcept
		{
			return value
				&& this == value->get_bus()
				&& m_categories[id].insert(value).second;
		}

		template <class Ev
		> bool add_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			return this->add_listener(Ev::ID, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void remove_listener(hash_t id, event_listener * value) noexcept
		{
			if (!value || (this != value->get_bus())) { return; }
			else if (auto const cat{ m_categories.find(id) })
			{
				if (auto const listener{ cat->second->find(value) }; listener != cat->second->end())
				{
					cat->second->erase(listener);
				}
			}
		}

		template <class Ev
		> void remove_listener(event_listener * value) noexcept
		{
			static_assert(std::is_base_of_v<event, Ev>, "invalid event type");

			this->remove_listener(Ev::ID, value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void remove_listener(event_listener * value) noexcept
		{
			if (!value) { return; }

			m_categories.for_each([&](hash_t, category_type & cat) noexcept
			{
				if (auto const listener{ cat.find(value) }; listener != cat.end())
				{
					cat.erase(listener);
				}
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		categories_type m_categories{}; // categories

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ~event_listener
	inline event_listener::~event_listener() noexcept
	{
		unsubscribe(); // remove listener from all events
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_EVENTS_HPP_