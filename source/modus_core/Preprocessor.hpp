#ifndef _ML_PREPROCESSOR_HPP_
#define _ML_PREPROCESSOR_HPP_

#include <modus_core/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// root namespace
#define _ML						::ml::

// global typedef
#define ML_alias				using

// fixed array size
#define ML_arraysize(arr)		(sizeof(arr) / sizeof(*arr))

// ternary comparison
#define ML_compare(lhs, rhs)	(((lhs) != (rhs)) ? (((lhs) < (rhs)) ? -1 : 1) : 0)

// automatic forward
#define ML_forward(expr)		std::forward<decltype(expr)>(expr)

// unused expression
#define ML_unused(expr)			((void)(expr))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// concat implementation
#define ML_IMPL_CONCAT(a, b)	a##b

// concatenate
#define ML_cat(a, b)			ML_IMPL_CONCAT(a, b)

// crt wide string
#define ML_wide(str)			ML_cat(L, str)

// macro literal to string
#define ML_str(expr)			#expr

// macro contents to string
#define ML_xstr(expr)			ML_str(expr)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// anonymous extended
#if defined(__COUNTER__)
#	define ML_anon_ex(expr)		ML_cat(_, ML_cat(expr, ML_cat(_, ML_cat(__COUNTER__, _))))
#elif defined(__LINE__)
#	define ML_anon_ex(expr)		ML_cat(_, ML_cat(expr, ML_cat(_, ML_cat(__LINE__, _))))
#else
#	define ML_anon_ex(expr)		ML_cat(_, ML_cat(expr, _))
#endif

// anonymous
#define ML_anon					ML_anon_ex(anon)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// handle declarator
#define ML_decl_handle(name)	struct ML_cat(name, __) { int unused; }; \
								ML_alias name = typename ML_cat(name, __) *

// handle caster
#define ML_handle(type, value)	((type)(intptr_t)(value))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PREPROCESSOR_HPP_