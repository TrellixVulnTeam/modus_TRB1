#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

#include <modus_core/engine/GuiApplication.hpp>
#include <modus_core/engine/PluginManager.hpp>

namespace ml
{
	// application
	struct ML_CORE_API application : gui_application
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		using gui_application::allocator_type;

		explicit application(int32_t argc, char * argv[], allocator_type alloc = {});

		virtual ~application() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	protected:
		virtual void on_event(event const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

// global application
namespace ml::globals
{
	ML_decl_global(application) get() noexcept;

	ML_decl_global(application) set(application * value) noexcept;
}

#endif // !_ML_APPLICATION_HPP_