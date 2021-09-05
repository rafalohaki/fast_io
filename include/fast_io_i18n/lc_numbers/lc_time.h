#pragma once

namespace fast_io
{

namespace details
{
/*
https://www.ibm.com/docs/en/aix/7.1?topic=ff-lc-time-category-locale-definition-source-file-format
*/

template<std::integral char_type>
inline constexpr std::size_t lc_print_reserve_size_time_format_common_impl(basic_lc_time<char_type> const& t, iso8601_timestamp const& tsp, basic_io_scatter_t<char_type> const& format_str) noexcept
{
	std::size_t value{};
	auto const* fmt{format_str.base};
	std::size_t const fmt_len{format_str.len};
	constexpr std::size_t iso8601_timestamp_reserve_size{print_reserve_size(io_reserve_type<char_type, iso8601_timestamp>)};
	constexpr std::size_t uint_least8_reserve_size{print_reserve_size(io_reserve_type<char_type, std::uint_least8_t>)};
	constexpr std::size_t int_least64_reserve_size{print_reserve_size(io_reserve_type<char_type, std::int_least64_t>)};
	for (std::size_t i{}; i < fmt_len;) {
		while (*fmt++ != char_literal_v<u8'%', char_type> && i++ < fmt_len)
			++value;
		if (i < fmt_len)
		{
			++i; // this ++ replaces the ++i that should be the third sentence in the 'for'
			if (i >= fmt_len) return iso8601_timestamp_reserve_size;
			// else
			if (*fmt == char_literal_v<u8'%', char_type>) [[unlikely]]
				++value;
			else if (*fmt == char_literal_v<u8'a', char_type> || *fmt == char_literal_v<u8'A', char_type>) {
				value += t.day[weekday(tsp)].len;
			}
			else if (*fmt == char_literal_v<u8'b', char_type> || *fmt == char_literal_v<u8'B', char_type> || *fmt == char_literal_v<u8'h', char_type>) {
				if (tsp.month - 1 > 11u) [[unlikely]]
					value += uint_least8_reserve_size;
				else
					value += t.mon[tsp.month - 1].len;
			}
			else if (*fmt == char_literal_v<u8'C', char_type> || *fmt == char_literal_v<u8'd', char_type> || *fmt == char_literal_v<u8'e', char_type> || *fmt == char_literal_v<u8'H', char_type> || *fmt == char_literal_v<u8'I', char_type> || *fmt == char_literal_v<u8'm', char_type> || *fmt == char_literal_v<u8'M', char_type> || *fmt == char_literal_v<u8'S', char_type> || *fmt == char_literal_v<u8'U', char_type> || *fmt == char_literal_v<u8'W', char_type>)
				value += 2;
			else if (*fmt == char_literal_v<u8'D', char_type>)
			{
				constexpr std::size_t int_least64_reserve_size_plus6{static_cast<std::size_t>(6)+int_least64_reserve_size};
				value += int_least64_reserve_size_plus6;
			}
			else if (*fmt == char_literal_v<u8'E', char_type>) {
				if (++i >= fmt_len) return iso8601_timestamp_reserve_size;
				++fmt;
				if (*fmt == char_literal_v<u8'C', char_type> || *fmt == char_literal_v<u8'y', char_type>) // take %EC as a raw string
					value += 3;
				else if (*fmt == char_literal_v<u8'Y', char_type>)
					value += int_least64_reserve_size;
				else
					return iso8601_timestamp_reserve_size;
			}
			else if (*fmt == char_literal_v<u8'O', char_type>) {
				if (++i >= fmt_len) return iso8601_timestamp_reserve_size;
				++fmt;
				if (*fmt == char_literal_v<u8'd', char_type> || *fmt == char_literal_v<u8'e', char_type>) {
					if (tsp.day - 1 > 30u) [[unlikely]]
						value += uint_least8_reserve_size;
					else
						value += (t.alt_digits.base + tsp.day - 1)->len;
				}
				else if (*fmt == char_literal_v<u8'H', char_type>) {
					if (tsp.hours > 24u) [[unlikely]]
						value += uint_least8_reserve_size;
					else
						value += (t.alt_digits.base + tsp.hours)->len;
				}
				else if (*fmt == char_literal_v<u8'I', char_type>) {
					if (tsp.hours > 24u) [[unlikely]]
						value += uint_least8_reserve_size;
					else
						value += (t.alt_digits.base + ((tsp.hours > 12u) ? tsp.hours - 12u : tsp.hours))->len;
				}
				else if (*fmt == char_literal_v<u8'm', char_type>) {
					std::uint_least8_t month{tsp.month};
					--month;
					constexpr std::uint_least8_t month12{static_cast<std::uint_least8_t>(12)};
					if (month < month12) [[likely]]
						value += (t.alt_digits.base + month)->len;
					else
						value += uint_least8_reserve_size;
				}
				else if (*fmt == char_literal_v<u8'M', char_type>) {
					if (tsp.minutes > 60u) [[unlikely]]
						value += uint_least8_reserve_size;
					else
						value += (t.alt_digits.base + tsp.minutes)->len;
				}
				else if (*fmt == char_literal_v<u8'S', char_type>) {
					if (tsp.seconds > 60u) [[unlikely]]
						value += uint_least8_reserve_size;
					else
						value += (t.alt_digits.base + tsp.seconds)->len;
				}
				else if (*fmt == char_literal_v<u8'U', char_type>)
					; // to be done
				else if (*fmt == char_literal_v<u8'w', char_type>)
					value += (t.alt_digits.base + weekday(tsp))->len;
				else
					return iso8601_timestamp_reserve_size;
			}
			else if (*fmt == char_literal_v<u8'j', char_type>)
				value += uint_least8_reserve_size;
			else if (*fmt == char_literal_v<u8'n', char_type>)
				++value;
			else if (*fmt == char_literal_v<u8'N', char_type>)
				value += 20;
			else if (*fmt == char_literal_v<u8'o', char_type>)
				value += int_least64_reserve_size;
			else if (*fmt == char_literal_v<u8'p', char_type>)
				value += t.am_pm[0].len;
			else if (*fmt == char_literal_v<u8't', char_type>)
				++value;
			else if (*fmt == char_literal_v<u8'T', char_type>)
			{
				constexpr std::size_t uint_least8_reserve_size_result{2 + 3 * uint_least8_reserve_size};
				value += uint_least8_reserve_size_result;
			}
			else if (*fmt == char_literal_v<u8'w', char_type>)
				++value;
			else if (*fmt == char_literal_v<u8'y', char_type> || *fmt == char_literal_v<u8'Y', char_type>) // print full year
				value += int_least64_reserve_size;
			else	// the format string is invalid
					// or it contains other string. %c %r %x %X %Ec %Ex %EX
					// or it contains a timezone %Z
				return iso8601_timestamp_reserve_size;
			++fmt; ++i;
		}
	}
	return value;
}

template<::fast_io::freestanding::random_access_iterator Iter,std::integral char_type>
requires std::same_as<::fast_io::freestanding::iter_value_t<Iter>,char_type>
inline constexpr Iter lc_print_reserve_define_time_fmt_common_impl(basic_lc_time<char_type> const& t,Iter iter,iso8601_timestamp const& tsp,basic_io_scatter_t<char_type> const& format_str)
{
	auto const* fmt = format_str.base;
	auto itr = iter;
	const std::size_t fmt_len = format_str.len;
	const auto year = tsp.year;
	const auto month = tsp.month;
	const auto day = tsp.day;
	const auto hours = tsp.hours;
	const auto minutes = tsp.minutes;
	const auto seconds = tsp.seconds;
	for (std::size_t i{}; i < fmt_len;) {
		{
			auto pointer2 = fmt; auto index2 = i;
			for (;; ++index2, ++pointer2) {
				if (index2 >= fmt_len || *pointer2 == char_literal_v<u8'%', char_type>) {
					itr = non_overlapped_copy_n(fmt, index2 - i, itr);
					fmt = pointer2; i = index2;
					break;
				}
			}
		}
		if (i < fmt_len) {
			++i; ++fmt;
			if (i >= fmt_len) print_reserve_define(io_reserve_type<char_type, iso8601_timestamp>, iter, tsp);
			if (*fmt == char_literal_v<u8'%', char_type>) [[unlikely]] {
				*itr = char_literal_v<u8'%', char_type>;
				++itr;
			}
			else if (*fmt == char_literal_v<u8'a', char_type>)
				itr = copy_scatter(t.abday[weekday(tsp)], itr);
			else if (*fmt == char_literal_v<u8'A', char_type>)
				itr = copy_scatter(t.day[weekday(tsp)], itr);
			else if (*fmt == char_literal_v<u8'b', char_type>) {
				if (month - static_cast<std::uint_least8_t>(1u) > static_cast<std::uint_least8_t>(11u)) [[unlikely]]
					itr = print_reserve_integral_define<10>(itr, month);
				else
					itr = copy_scatter(t.abmon[month - 1], itr);
			}
			else if (*fmt == char_literal_v<u8'B', char_type>) {
				if (month - 1u > 11u) [[unlikely]]
					itr = print_reserve_integral_define<10>(itr, month);
				else
					itr = copy_scatter(t.mon[month - 1], itr);
			}
			else if (*fmt == char_literal_v<u8'c', char_type>) {
				if (&format_str == &t.d_t_fmt) [[unlikely]]
					return print_reserve_define(io_reserve_type<char_type, iso8601_timestamp>, iter, tsp);
				else
					itr = lc_print_reserve_define_time_fmt_common_impl(t, itr, tsp, t.d_t_fmt);
			}
			else if (*fmt == char_literal_v<u8'd', char_type>)
				itr = chrono_two_digits_impl<true>(itr, month);
			else
				// the format string is invalid
				// or it contains any of the following: 
				// %C, 
				return print_reserve_define(io_reserve_type<char_type, iso8601_timestamp>, iter, tsp);
		}

	}
	return itr;
}
}


template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* all,iso8601_timestamp const& tsp) noexcept
{
	return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp, all->time.d_t_fmt);
}

template<::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* all,Iter iter,iso8601_timestamp const& tsp) noexcept
{
	return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp,all->time.d_t_fmt);
}

static_assert(lc_dynamic_reserve_printable<char8_t,fast_io::iso8601_timestamp>);

}