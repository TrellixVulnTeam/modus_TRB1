#include <modus_core/runtime/Plugin.hpp>
#include <modus_core/runtime/Application.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	plugin::plugin(plugin_manager * manager, void * userptr) noexcept
		: runtime_object	{ manager->get_context() }
		, m_app				{ manager->get_application() }
		, m_manager			{ manager }
		, m_userptr			{ userptr }
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}