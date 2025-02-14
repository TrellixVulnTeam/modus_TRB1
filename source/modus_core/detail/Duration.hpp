#ifndef _ML_DURATION_HPP_
#define _ML_DURATION_HPP_

#include <modus_core/detail/Utility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias nanoseconds_t	= typename chrono::duration<float32, std::nano>;
	ML_alias microseconds_t	= typename chrono::duration<float32, std::micro>;
	ML_alias milliseconds_t	= typename chrono::duration<float32, std::milli>;
	ML_alias seconds_t		= typename chrono::duration<float32, std::ratio<1>>;
	ML_alias minutes_t		= typename chrono::duration<float32, std::ratio<60>>;
	ML_alias hours_t		= typename chrono::duration<float32, std::ratio<60 * 60>>;
	ML_alias days_t			= typename chrono::duration<float32, std::ratio<60 * 60 * 24>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct duration final : public chrono::duration<float32>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using chrono::duration<float32>::duration;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator float32() const noexcept
		{
			return count();
		}

		template <class T
		> ML_NODISCARD constexpr operator T() const noexcept
		{
			return static_cast<T>(count());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto nanoseconds() const noexcept { return chrono::duration_cast<nanoseconds_t>(*this); }

		ML_NODISCARD constexpr auto microseconds() const noexcept { return chrono::duration_cast<microseconds_t>(*this); }

		ML_NODISCARD constexpr auto milliseconds() const noexcept { return chrono::duration_cast<milliseconds_t>(*this); }

		ML_NODISCARD constexpr auto seconds() const noexcept { return chrono::duration_cast<seconds_t>(*this); }

		ML_NODISCARD constexpr auto minutes() const noexcept { return chrono::duration_cast<minutes_t>(*this); }

		ML_NODISCARD constexpr auto hours() const noexcept { return chrono::duration_cast<hours_t>(*this); }

		ML_NODISCARD constexpr auto days() const noexcept { return chrono::duration_cast<days_t>(*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// to json
	inline void to_json(json & j, duration const & v)
	{
		j = v.count();
	}

	// from json
	inline void from_json(json const & j, duration & v)
	{
		j.get_to((float32 &)v);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = float32
	> ML_NODISCARD constexpr auto operator+(duration const & lhs, T const & rhs) noexcept
	{
		return duration{ (float32)lhs + (float32)rhs };
	}

	template <class T = float32
	> ML_NODISCARD constexpr auto operator-(duration const & lhs, T const & rhs) noexcept
	{
		return duration{ (float32)lhs - (float32)rhs };
	}

	template <class T = float32
	> ML_NODISCARD constexpr auto operator*(duration const & lhs, T const & rhs) noexcept
	{
		return duration{ (float32)lhs * (float32)rhs };
	}

	template <class T = float32
	> ML_NODISCARD constexpr auto operator/(duration const & lhs, T const & rhs) noexcept
	{
		return duration{ (float32)lhs / (float32)rhs };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T = float32
	> constexpr auto operator+=(duration & lhs, T const & rhs) noexcept -> duration &
	{
		return lhs = lhs + rhs;
	}

	template <class T = float32
	> constexpr auto operator-=(duration & lhs, T const & rhs) noexcept -> duration &
	{
		return lhs = lhs - rhs;
	}

	template <class T = float32
	> constexpr auto operator*=(duration & lhs, T const & rhs) noexcept -> duration &
	{
		return lhs = lhs * rhs;
	}

	template <class T = float32
	> constexpr auto operator/=(duration & lhs, T const & rhs) noexcept -> duration &
	{
		return lhs = lhs / rhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DURATION_HPP_