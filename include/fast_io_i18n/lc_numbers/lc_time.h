#pragma once

namespace fast_io
{

namespace details
{
/*
https://www.ibm.com/docs/en/aix/7.1?topic=ff-lc-time-category-locale-definition-source-file-format
*/

template<std::integral char_type>
inline constexpr std::size_t lc_format_alt_digits_len(basic_io_scatter_t<basic_io_scatter_t<char_type>> const& alt_digits,std::uint_least8_t value) noexcept
{
	std::size_t size_value{static_cast<std::size_t>(value)};
	std::size_t alt_digits_len{alt_digits.len};
	if (size_value<alt_digits_len) [[likely]]
		return alt_digits.base[size_value].len;
	else
	{
		constexpr std::size_t uint_least8_reserve_size{ print_reserve_size(io_reserve_type<char_type, std::uint_least8_t>) };
		return uint_least8_reserve_size;
	}
}

template<std::integral char_type>
inline constexpr std::size_t lc_print_reserve_size_time_format_common_impl(basic_lc_time<char_type> const& t, iso8601_timestamp const& tsp, basic_io_scatter_t<char_type> const& format_str) noexcept
{
	constexpr std::size_t uint_least8_reserve_size{ print_reserve_size(io_reserve_type<char_type, std::uint_least8_t>) };
	constexpr std::size_t int_least64_reserve_size{ print_reserve_size(io_reserve_type<char_type, std::int_least64_t>) };
	std::size_t value{};
	for (char_type const* i{format_str.base},*end_it{i+format_str.len};i!=end_it;++i)
	{
		auto p{::fast_io::freestanding::find(i,end_it,char_literal_v<u8'%', char_type>)};
		if(p==end_it)
			return value+static_cast<std::size_t>(end_it-i);
		if((++p)==end_it)[[unlikely]]
			return value+static_cast<std::size_t>(end_it-i);
		switch(*p)
		{
		case char_literal_v<u8'%', char_type>:
		case char_literal_v<u8'n', char_type>:
		case char_literal_v<u8'w', char_type>:
		{
			++value;
			break;
		}
		case char_literal_v<u8'a', char_type>:
		case char_literal_v<u8'A', char_type>:
		{
			value += t.day[c_weekday(tsp)].len;
			break;
		}
		case char_literal_v<u8'b', char_type>:
		case char_literal_v<u8'B', char_type>:
		case char_literal_v<u8'h', char_type>:
		{
			std::uint_least8_t month_minus_1{ tsp.month };
			--month_minus_1;
			if (month_minus_1 > 11u) [[unlikely]]
				value += uint_least8_reserve_size;
			else
				value += t.mon[month_minus_1].len;
			break;
		}
		case char_literal_v<u8'C', char_type>:
		case char_literal_v<u8'd', char_type>:
		case char_literal_v<u8'e', char_type>:
		case char_literal_v<u8'H', char_type>:
		case char_literal_v<u8'm', char_type>:
		case char_literal_v<u8'M', char_type>:
		case char_literal_v<u8'S', char_type>:
		case char_literal_v<u8'U', char_type>:
		case char_literal_v<u8'W', char_type>:
		{
			value+=uint_least8_reserve_size;
			break;
		}
		case char_literal_v<u8'D', char_type>:
		case char_literal_v<u8'F', char_type>:
		{
			constexpr std::size_t int_least64_reserve_size_plus6{ static_cast<std::size_t>(6) + int_least64_reserve_size };
			value += int_least64_reserve_size_plus6;
			break;
		}
		case char_literal_v<u8'E', char_type>:
		{
			if((++p)==end_it)[[unlikely]]
			{
				value+=static_cast<std::size_t>(p-i);
				return value;
			}
			switch(*p)
			{
			case char_literal_v<u8'C', char_type>:
			case char_literal_v<u8'y', char_type>:
			case char_literal_v<u8'Y', char_type>:
			{
				value += int_least64_reserve_size;
				break;
			}
			default:
				return value+static_cast<std::size_t>(end_it-i);
			}
			break;
		}
		case char_literal_v<u8'O', char_type>:
		{
			if((++p)==end_it)[[unlikely]]
			{
				value+=static_cast<std::size_t>(p-i);
				return value;
			}
			char_type const ch{*p};
			if (ch == char_literal_v<u8'U', char_type>)
			{
				value += uint_least8_reserve_size;
				break;
			}
			std::uint_least8_t altvalue{};
			switch(ch)
			{
				case char_literal_v<u8'd', char_type>:
				case char_literal_v<u8'e', char_type>:
				{
					altvalue = tsp.day;
					break;
				}
				case char_literal_v<u8'H', char_type>:
				{
					altvalue = tsp.hours;
					break;
				}
				case char_literal_v<u8'I', char_type>:
				{
					altvalue = (tsp.hours > 12u) ? tsp.hours - 12u : tsp.hours;
					break;
				}
				case char_literal_v<u8'm', char_type>:
				case char_literal_v<u8'M', char_type>:
				{
					altvalue = tsp.month;
					break;
				}
				case char_literal_v<u8'S', char_type>:
				{
					altvalue = tsp.seconds;
					break;
				}
				case char_literal_v<u8'w', char_type>:
				{
					altvalue = c_weekday(tsp);
					break;
				}
				default:
				{
					return value+static_cast<std::size_t>(end_it-i);
				}
			}
			value += lc_format_alt_digits_len(t.alt_digits,altvalue);
			break;
		}
		case char_literal_v<u8'j', char_type>:
		{
			value += uint_least8_reserve_size;
			break;
		}
		case char_literal_v<u8'N', char_type>:
		{
			value += 20u;//20 Represents the alternate era name.??? to fix
			break;
		}
		case char_literal_v<u8'p', char_type>:
		{
			std::uint_least8_t const hours{tsp.hours};
			if(hours<12u)
				value += t.am_pm[0].len;
			else if(hours<24u)
				value += t.am_pm[1].len;
			//if hours >=24, ignore am pm
			break;
		}
		case char_literal_v<u8'T', char_type>:
		{
			constexpr std::size_t uint_least8_reserve_size_result{ 2 + 3 * uint_least8_reserve_size };
			value += uint_least8_reserve_size_result;
			break;
		}
		case char_literal_v<u8'o', char_type>:
		case char_literal_v<u8'y', char_type>: // print full year
		case char_literal_v<u8'Y', char_type>:
		{
			value += int_least64_reserve_size;
			break;
		}
		case char_literal_v<u8'z',char_type>:
		case char_literal_v<u8'Z',char_type>:
		{
			value += print_reserve_size_timezone_impl_v<char_type>;
			break;
		}
		default:	// the format string is invalid
				// or it contains other string. %c %r %x %X %Ec %Ex %EX
			return value+static_cast<std::size_t>(end_it-i);
		}
		i=p;
	}
	return value;
}


template<::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter lc_copy_12_impl(Iter iter) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
		return copy_string_literal("12",iter);
	else if constexpr(std::same_as<char_type,wchar_t>)
		return copy_string_literal(L"12",iter);
	else if constexpr(std::same_as<char_type,char16_t>)
		return copy_string_literal(u"12",iter);
	else if constexpr(std::same_as<char_type,char32_t>)
		return copy_string_literal(U"12",iter);
	else
		return copy_string_literal(u8"12",iter);
}

template<::fast_io::freestanding::random_access_iterator Iter,std::integral char_type>
requires std::same_as<::fast_io::freestanding::iter_value_t<Iter>,char_type>
inline constexpr Iter lc_print_reserve_define_time_fmt_common_impl(basic_lc_time<char_type> const& t,Iter iter,iso8601_timestamp const& tsp,basic_io_scatter_t<char_type> const& format_str)
{

	return itr;
}

template<::fast_io::manipulators::lc_time_flag flag>
inline constexpr ::fast_io::manipulators::scalar_flags base_lc_time_flags_cache{.time_flag=flag};

}

namespace manipulators
{
/*
do not use them. they are experimental
*/
inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::d_t_fmt>,iso8601_timestamp const&> d_t_fmt(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::d_fmt>,iso8601_timestamp const&> d_fmt(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::t_fmt>,iso8601_timestamp const&> t_fmt(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::t_fmt_ampm>,iso8601_timestamp const&> t_fmt_ampm(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::date_fmt>,iso8601_timestamp const&> date_fmt(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::era_d_t_fmt>,iso8601_timestamp const&> era_d_t_fmt(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::era_d_fmt>,iso8601_timestamp const&> era_d_fmt(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

inline constexpr scalar_manip_t<::fast_io::details::base_lc_time_flags_cache<lc_time_flag::era_t_fmt>,iso8601_timestamp const&> era_t_fmt(iso8601_timestamp const& tsp) noexcept
{
	return {tsp};
}

}

template<::fast_io::manipulators::scalar_flags flags,std::integral char_type>
requires (static_cast<std::uint_least8_t>(static_cast<std::uint_least8_t>(flags.time_flag)-static_cast<std::uint_least8_t>(1u))<
	static_cast<std::uint_least8_t>(8u))
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* all,
	::fast_io::manipulators::scalar_manip_t<flags,iso8601_timestamp const&> tsp) noexcept
{
	constexpr manipulators::lc_time_flag current_lc_time_flag{flags.time_flag};
	if constexpr(current_lc_time_flag==manipulators::lc_time_flag::d_t_fmt)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.d_t_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::d_fmt)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.d_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::t_fmt)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.t_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::t_fmt_ampm)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.t_fmt_ampm);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::date_fmt)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.date_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_d_t_fmt)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.era_d_t_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_d_fmt)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.era_d_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_t_fmt)
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, all->time.era_t_fmt);
}

template<::fast_io::manipulators::scalar_flags flags,::fast_io::freestanding::random_access_iterator Iter>
requires (static_cast<std::uint_least8_t>(static_cast<std::uint_least8_t>(flags.time_flag)-static_cast<std::uint_least8_t>(1u))<
	static_cast<std::uint_least8_t>(8u))
inline constexpr Iter print_reserve_define(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* all,Iter iter,
	::fast_io::manipulators::scalar_manip_t<flags,iso8601_timestamp const&> tsp) noexcept
{
	constexpr manipulators::lc_time_flag current_lc_time_flag{flags.time_flag};
	if constexpr(current_lc_time_flag==manipulators::lc_time_flag::d_t_fmt)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.d_t_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::d_fmt)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.d_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::t_fmt)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.t_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::t_fmt_ampm)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.t_fmt_ampm);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::date_fmt)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.date_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_d_t_fmt)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.era_d_t_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_d_fmt)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.era_d_fmt);
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_t_fmt)
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,all->time.era_t_fmt);
}

}