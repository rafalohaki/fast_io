#pragma once

namespace fast_io
{

namespace details
{
/*
https://www.ibm.com/docs/en/aix/7.1?topic=ff-lc-time-category-locale-definition-source-file-format
*/

inline constexpr std::uint_least16_t month_accum[]{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

inline constexpr std::uint_least16_t day_of_the_year(iso8601_timestamp const& tsp) noexcept
{
	std::uint_least8_t month_minus_1 = static_cast<std::uint_least8_t>(tsp.month - 1u);
	if (month_minus_1 >= 12u) return 0;
	std::uint_least16_t value{ static_cast<uint_least16_t>(month_accum[month_minus_1] + static_cast<uint_least16_t>(tsp.day))};
	std::int_least64_t year{tsp.year};
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		++value;
	return value;
}

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

template<std::integral char_type,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter lc_format_alt_digits_print(basic_io_scatter_t<basic_io_scatter_t<char_type>> const& alt_digits,std::uint_least8_t value,Iter iter) noexcept
{
	std::size_t size_value{static_cast<std::size_t>(value)};
	std::size_t alt_digits_len{alt_digits.len};
	if (size_value<alt_digits_len) [[likely]]
		return copy_scatter(alt_digits.base[size_value],iter);
	else
		return print_reserve_integral_define<10>(iter,value);
}

/*
compress them to force them to be passed by value on ms abi
*/
struct lc_month_day_pass_aux
{
	std::uint_least8_t month;
	std::uint_least8_t day;
};

inline constexpr bool test_era_started(std::int_least64_t y0,std::uint_least8_t m0,std::uint_least8_t d0,
				std::int_least64_t y1,std::uint_least8_t m1,std::uint_least8_t d1) noexcept
{
	if(y0<y1)
		return true;
	if(y0==y1)
		return (m0<m1)||((m0==m1)&&(d0<=d1));
	return false;
}

template<std::integral char_type>
inline constexpr basic_lc_time_era<char_type> const* lc_time_find_era_impl(
		basic_lc_time_era<char_type> const* first,basic_lc_time_era<char_type> const* last,
		std::int_least64_t year,lc_month_day_pass_aux month_day) noexcept
{
	std::uint_least8_t month{month_day.month};
	std::uint_least8_t day{month_day.day};
	for(;first!=last;++first)
	{
		basic_lc_time_era<char_type> const& e{*first};
		bool has_era_start{};
		bool direction{e.direction};
		if(direction)
		{
			has_era_start=test_era_started(
				e.start_date_year,e.start_date_month,e.start_date_day,
				year,month,day);
		}
		else
		{
			has_era_start=test_era_started(year,month,day,
				e.start_date_year,e.start_date_month,e.start_date_day);
		}
		if(has_era_start)
		{
			std::int_least8_t end_special{e.end_date_special};
			if(0<end_special)
			{
				return first;
			}
			if(end_special==0)
			{
				bool has_era_end{};
				if(direction)
				{
					has_era_end=test_era_started(year,month,day,
						e.end_date_year,e.end_date_month,e.end_date_day);
				}
				else
				{
					has_era_end=test_era_started(
						e.end_date_year,e.end_date_month,e.end_date_day,
						year,month,day);
				}
				if(has_era_end)
					return first;
			}
			else
			{
				return last;
			}
		}
	}
	return first;
}

template<std::integral char_type>
inline constexpr std::size_t lc_print_reserve_size_time_format_common_impl(basic_lc_time<char_type> const& t, iso8601_timestamp const& tsp, basic_io_scatter_t<char_type> const& format_str) noexcept
{
	constexpr std::size_t uint_least8_reserve_size{ print_reserve_size(io_reserve_type<char_type, std::uint_least8_t>) };
	constexpr std::size_t uint_least16_reserve_size{ print_reserve_size(io_reserve_type<char_type, std::uint_least16_t>) };
	constexpr std::size_t int_least64_reserve_size{ print_reserve_size(io_reserve_type<char_type, std::int_least64_t>) };
	std::size_t value{};
	for (char_type const* i{format_str.base},*end_it{i+format_str.len};i!=end_it;++i)
	{
		auto p{::fast_io::freestanding::find(i,end_it,char_literal_v<u8'%', char_type>)};
		value+=static_cast<std::size_t>(p-i);
		if(p==end_it)
			return value;
		if((++p)==end_it)
			return value+1;
		bool omit{*p==char_literal_v<u8'-', char_type>};
		if(omit)
		{
			++value;
			if((++p)==end_it)
				return value+1;
		}
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
			basic_io_scatter_t<char> const* base_ptr;
			if(*p==char_literal_v<u8'a', char_type>)
				base_ptr=t.abday;
			else
				base_ptr=t.day;
			value += base_ptr[c_weekday(tsp)].len;
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
		case char_literal_v<u8'c', char_type>:
		{
			value += lc_print_reserve_size_time_format_common_impl(t,tsp,t.d_t_fmt);
			break;
		}
		case char_literal_v<u8'C', char_type>:
		case char_literal_v<u8'y', char_type>:
		case char_literal_v<u8'Y', char_type>:
		{
			value += int_least64_reserve_size;
			break;
		}
		case char_literal_v<u8'd', char_type>:
		case char_literal_v<u8'e', char_type>:
		case char_literal_v<u8'H', char_type>:
		case char_literal_v<u8'm', char_type>:
		case char_literal_v<u8'M', char_type>:
		case char_literal_v<u8'S', char_type>:
		case char_literal_v<u8'U', char_type>:
		case char_literal_v<u8'W', char_type>:
		case char_literal_v<u8'l', char_type>:
		case char_literal_v<u8'I', char_type>:
		{
			value+=uint_least8_reserve_size;
			break;
		}
		case char_literal_v<u8'j', char_type>:
		{
			value += uint_least16_reserve_size;
			break;
		}
		case char_literal_v<u8'D', char_type>:
		case char_literal_v<u8'F', char_type>:
		{
			constexpr std::size_t int_least64_reserve_size_plus6{ static_cast<std::size_t>(2) + uint_least8_reserve_size*2 + int_least64_reserve_size };
			value += int_least64_reserve_size_plus6;
			break;
		}
		case char_literal_v<u8'E', char_type>:
		{
			if((++p)==end_it)[[unlikely]]
			{
				value+=static_cast<std::size_t>(end_it-i);
				return value;
			}
			switch(*p)
			{
			case char_literal_v<u8'c', char_type>:
			case char_literal_v<u8'x', char_type>:
			case char_literal_v<u8'X', char_type>:
			{
				basic_io_scatter_t<char_type> const* scatter_ptr{};
				switch(*p)
				{
				case char_literal_v<u8'c', char_type>:
				{
					scatter_ptr=__builtin_addressof(t.era_d_t_fmt);
					break;
				}
				case char_literal_v<u8'x', char_type>:
				{
					scatter_ptr=__builtin_addressof(t.era_d_fmt);
					break;
				}
				default:
				{
					scatter_ptr=__builtin_addressof(t.era_t_fmt);
					break;
				}
				}
				value += lc_print_reserve_size_time_format_common_impl(t,tsp,*scatter_ptr);
				break;
			}
			case char_literal_v<u8'C', char_type>:
			case char_literal_v<u8'y', char_type>:
			case char_literal_v<u8'Y', char_type>:
			{
				auto era_scatter_start{t.era.base};
				auto era_scatter_end{era_scatter_start+t.era.len};
				auto era_result{lc_time_find_era_impl(era_scatter_start,era_scatter_end,tsp.year,{tsp.month,tsp.day})};
				if(era_result!=era_scatter_end)
				{
					switch(*p)
					{
						case char_literal_v<u8'C', char_type>:
						{
							value += era_result->era_name.len;
							break;
						}
						case char_literal_v<u8'y', char_type>:
						{
							value += int_least64_reserve_size;
							break;
						}
						default:
						{
							value += lc_print_reserve_size_time_format_common_impl(t,tsp,era_result->era_format);
							break;
						}
					}
				}
				else
				{
					value += int_least64_reserve_size;
				}
				break;
			}
			default:
				return value+static_cast<std::size_t>(end_it-p+2);
			}
			break;
		}
		case char_literal_v<u8'O', char_type>:
		{
			if((++p)==end_it)[[unlikely]]
				return value+2;
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
				case char_literal_v<u8'B', char_type>://ab_alt_mon
				case char_literal_v<u8'b', char_type>://ab_alt_mon
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
					return value+static_cast<std::size_t>(end_it-p+2);
				}
			}
			value += lc_format_alt_digits_len(t.alt_digits,altvalue);
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
		case char_literal_v<u8'z',char_type>:
		case char_literal_v<u8'Z',char_type>:
		{
			value += print_reserve_size_timezone_impl_v<char_type>;
			break;
		}
		default:	// the format string is invalid
				// or it contains other string. %c %r %x %X %Ec %Ex %EX
			return value+static_cast<std::size_t>(end_it-p+1);
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

template<std::integral char_type,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter lc_print_month_fmt_common(basic_io_scatter_t<char_type> const* month_array,Iter iter,std::uint_least8_t month)
{
	constexpr std::uint_least8_t constant{12};
	std::uint_least8_t monthm1{month};
	--monthm1;
	if(monthm1 < constant) [[likely]]
		return copy_scatter(month_array[monthm1], iter);
	else
		return print_reserve_integral_define<10>(iter, month);
}

template<::fast_io::freestanding::random_access_iterator Iter,std::integral char_type>
requires std::same_as<::fast_io::freestanding::iter_value_t<Iter>,char_type>
inline constexpr Iter lc_print_reserve_define_time_fmt_common_impl(basic_lc_time<char_type> const& t,Iter iter,iso8601_timestamp const& tsp,basic_io_scatter_t<char_type> const& format_str)
{
	for (char_type const* i{format_str.base},*end_it{i+format_str.len};i!=end_it;++i)
	{
		char_type const* p{::fast_io::freestanding::find(i,end_it,char_literal_v<u8'%', char_type>)};
		iter=non_overlapped_copy(i,p,iter);
		if(p==end_it)
			return iter;
		if((++p)==end_it)
		{
			*iter=*(--p);
			++iter;
			return iter;
		}
		bool omit{*p==char_literal_v<u8'-', char_type>};
		if(omit)
		{
			if((++p)==end_it)
			{
				*iter=char_literal_v<u8'-', char_type>;
				++iter;
				return iter;
			}
		}
		char_type const first_format_ch{*p};
		switch(first_format_ch)
		{
		case char_literal_v<u8'%', char_type>:
		{
			*iter = char_literal_v<u8'%', char_type>;
			++iter;
			break;
		}
		case char_literal_v<u8'a', char_type>:
		case char_literal_v<u8'A', char_type>:
		{
			basic_io_scatter_t<char> const* base_ptr;
			if(*p==char_literal_v<u8'a', char_type>)
				base_ptr=t.abday;
			else
				base_ptr=t.day;

			iter=copy_scatter(base_ptr[c_weekday(tsp)],iter);
			break;
		}
		case char_literal_v<u8'B', char_type>:
		{
			iter=lc_print_month_fmt_common(t.mon,iter,tsp.month);
			break;
		}
		case char_literal_v<u8'b', char_type>:
		case char_literal_v<u8'h', char_type>:
		{
			iter=lc_print_month_fmt_common(t.abmon,iter,tsp.month);
			break;
		}
		case char_literal_v<u8'c', char_type>:
		{
			iter=lc_print_reserve_define_time_fmt_common_impl(t, iter, tsp, t.d_t_fmt);
			break;
		}
		case char_literal_v<u8'C', char_type>:
		{
			iter = print_reserve_integral_define<10>(iter, tsp.year / 100);
			break;
		}
		case char_literal_v<u8'd', char_type>:
		{
			if(omit)
				iter = print_reserve_integral_define<10>(iter, tsp.day);
			else
				iter = chrono_two_digits_impl<false>(iter, tsp.day);
			break;
		}
		case char_literal_v<u8'D', char_type>:
		{
			iter = chrono_two_digits_impl<false>(iter, tsp.month);
			*iter = char_literal_v<u8'/', char_type>;
			++iter;
			iter = chrono_two_digits_impl<false>(iter, tsp.day);
			*iter = char_literal_v<u8'/', char_type>;
			++iter;
			iter = chrono_year_impl(iter, tsp.year);
			break;
		}
		case char_literal_v<u8'e', char_type>:
		{
			iter = chrono_two_digits_impl<false,true>(iter, tsp.day);
			break;
		}
		case char_literal_v<u8'E', char_type>:
		{
			if((++p)==end_it)[[unlikely]]
			{
				if(omit)
					return non_overlapped_copy_n(end_it-3,3,iter);
				else
					return non_overlapped_copy_n(end_it-2,2,iter);
			}
			switch(*p)
			{
			case char_literal_v<u8'c', char_type>:
			{
				iter = lc_print_reserve_define_time_fmt_common_impl(t, iter, tsp, t.era_d_t_fmt);
				break;
			}
			case char_literal_v<u8'C', char_type>:
			case char_literal_v<u8'y', char_type>:
			case char_literal_v<u8'Y', char_type>:
			{
				auto era_scatter_start{t.era.base};
				auto era_scatter_end{era_scatter_start+t.era.len};
				auto era_result{lc_time_find_era_impl(era_scatter_start,era_scatter_end,tsp.year,{tsp.month,tsp.day})};
				if(era_result!=era_scatter_end)[[likely]]
				{
					switch(*p)
					{
						case char_literal_v<u8'Y', char_type>:
						{
							iter = lc_print_reserve_define_time_fmt_common_impl(t,iter,tsp,era_result->era_format);
							break;
						}
						case char_literal_v<u8'C', char_type>:
						{
							iter = copy_scatter(era_result->era_name,iter);
							break;
						}
						default:
						{
							std::int_least64_t real_era_year_offset{era_result->start_date_year-era_result->offset};
							std::int_least64_t era_year;
							if(::fast_io::details::intrinsics::sub_underflow(tsp.year,real_era_year_offset,era_year))[[unlikely]]
							{
								if(omit)
									iter=non_overlapped_copy_n(p-4,4,iter);
								else
									iter=non_overlapped_copy_n(p-3,3,iter);
							}
							else
								iter=print_reserve_integral_define<10>(iter,era_year);
						}
					}
				}
				else
				{
					if(*p==char_literal_v<u8'C', char_type>)
						iter = print_reserve_integral_define<10>(iter, tsp.year / 100);
					else
						iter = chrono_year_impl(iter, tsp.year);
				}
				break;
			}
			case char_literal_v<u8'x', char_type>:
			{
				iter = lc_print_reserve_define_time_fmt_common_impl(t, iter, tsp, t.era_d_fmt);
				break;
			}
			case char_literal_v<u8'X', char_type>:
			{
				iter = lc_print_reserve_define_time_fmt_common_impl(t, iter, tsp, t.era_t_fmt);
				break;
			}
			default:
			{
				if(omit)
					--p;
				return non_overlapped_copy(p-2,end_it,iter);
			}
			}
			break;
		}
		case char_literal_v<u8'F', char_type>:
		{
			iter = chrono_year_impl(iter, tsp.year);
			*iter = char_literal_v<u8'-', char_type>;
			++iter;
			iter = chrono_two_digits_impl<false>(iter, tsp.month);
			*iter = char_literal_v<u8'-', char_type>;
			++iter;
			iter = chrono_two_digits_impl<false>(iter, tsp.day);
			break;
		}
#if 0
		case char_literal_v<u8'g', char_type>:case char_literal_v<u8'G', char_type>:
		{
			// refuse to print 2-digit year. Unfinished for week-base: need implication of week numbers
			iter = chrono_year_impl(iter, tsp.year);
			break;
		}
#endif
		case char_literal_v<u8'H', char_type>:
		{
			iter = chrono_two_digits_impl<false>(iter, tsp.hours);
			break;
		}
		case char_literal_v<u8'l', char_type>:
		case char_literal_v<u8'I', char_type>:
		{
			std::uint_least8_t hours = tsp.hours;
			if (hours == 0u)
				iter = lc_copy_12_impl(iter);
			else if (hours <= 12u)[[likely]]
				iter = chrono_two_digits_impl<false>(iter, hours);
			else if (hours <= 24u)[[likely]]
				iter = chrono_two_digits_impl<false>(iter, static_cast<std::uint_least8_t>(hours-12u));
			break;	
		}
		case char_literal_v<u8'j', char_type>:
		{
			iter = chrono_two_digits_impl<false>(iter, day_of_the_year(tsp));
			break;
		}
		case char_literal_v<u8'm', char_type>:
		{
			if(omit)
				iter = print_reserve_integral_define<10>(iter, tsp.month);
			else
				iter = chrono_two_digits_impl<false>(iter, tsp.month);
			break;
		}
		case char_literal_v<u8'M', char_type>:
		{
			iter = chrono_two_digits_impl<false>(iter, tsp.minutes);
			break;
		}
		case char_literal_v<u8'n', char_type>:
		{
			*iter = char_literal_v<u8'\n',char_type>;
			++iter;
			break;
		}
		case char_literal_v<u8'O', char_type>:
		{
			if((++p)==end_it)[[unlikely]]
			{
				if(omit)
					return non_overlapped_copy_n(end_it-3,3,iter);
				else
					return non_overlapped_copy_n(end_it-2,2,iter);
			}
			std::uint_least8_t altvalue{};

			switch(*p)
			{
			case char_literal_v<u8'd', char_type>:case char_literal_v<u8'e', char_type>:
			{
				altvalue=tsp.day;
				break;
			}
			case char_literal_v<u8'H', char_type>:
			{
				altvalue=tsp.hours;
				break;
			}
			case char_literal_v<u8'I', char_type>:
			{
				std::uint_least8_t hours{tsp.hours};
				if (hours == 0u)
				{
					hours=12u;
				}
				else if(12u<hours)
				{
					hours-=12u;
				}
				altvalue=hours;
				break;
			}
			case char_literal_v<u8'm', char_type>:
			{
				altvalue=tsp.month;
				break;
			}
			case char_literal_v<u8'M', char_type>:
			{
				altvalue=tsp.minutes;
				break;
			}
			case char_literal_v<u8'S', char_type>:
			{
				altvalue=tsp.seconds;
				break;
			}
#if 0
			case char_literal_v<u8'u', char_type>:
			{
				altvalue=tsp.seconds;
				break;
			}
#endif
			case char_literal_v<u8'w', char_type>:
			{
				altvalue=c_weekday(tsp);
				break;
			}
#if 0
			case char_literal_v<u8'y', char_type>:
			{
				break;
			}
#endif
			default:
			{
				if(omit)
					--p;
				return non_overlapped_copy(p-2,end_it,iter);
			}
			}
			iter=lc_format_alt_digits_print(t.alt_digits,altvalue,iter);
			break;
		}
		case char_literal_v<u8'p', char_type>:
		{
			if (tsp.hours < 12u)
				iter = copy_scatter(t.am_pm[0], iter);
			else if (tsp.hours < 24u)
				iter = copy_scatter(t.am_pm[1], iter);
			break;
		}
		case char_literal_v<u8'r', char_type>:
		{
			iter = lc_print_reserve_define_time_fmt_common_impl(t, iter, tsp, t.t_fmt_ampm);
			break;
		}
		case char_literal_v<u8'R', char_type>:
		{
			iter = chrono_two_digits_impl<false>(iter, tsp.hours);
			*iter = char_literal_v<u8':', char_type>;
			++iter;
			iter = chrono_two_digits_impl<false>(iter, tsp.minutes);
			break;
		}
		case char_literal_v<u8'S', char_type>:
		{
			iter = chrono_two_digits_impl<false>(iter, tsp.seconds);
			break;
		}
		case char_literal_v<u8't', char_type>:
		{
			*iter = char_literal_v<u8'\t',char_type>;
			++iter;
			break;
		}
		case char_literal_v<u8'T', char_type>:
		{
			iter = chrono_two_digits_impl<false>(iter, tsp.hours);
			*iter = char_literal_v<u8':', char_type>;
			++iter;
			iter = chrono_two_digits_impl<false>(iter, tsp.minutes);
			*iter = char_literal_v<u8':', char_type>;
			++iter;
			iter = chrono_two_digits_impl<false>(iter, tsp.seconds);
			break;
		}
		case char_literal_v<u8'u', char_type>:
		{
			iter = chrono_one_digit_impl<true>(iter, weekday(tsp));
			break;
		}
		case char_literal_v<u8'w', char_type>:
		{
			iter = lc_format_alt_digits_print(t.alt_digits,c_weekday(tsp),iter);
			break;
		}
		case char_literal_v<u8'y', char_type>:case char_literal_v<u8'Y', char_type>:
		{
			iter = chrono_year_impl(iter, tsp.year);
			break;
		}
		case char_literal_v<u8'z', char_type>:case char_literal_v<u8'Z', char_type>:
		{
			iter = print_reserve_timezone_impl(iter,tsp.timezone);
			break;
		}
		default:
		{
			if(omit)
				--p;
			return non_overlapped_copy(p-1,end_it,iter);
		}
		}
		i=p;
	}
	return iter;
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
	{
		auto* e{__builtin_addressof(all->time.era_d_t_fmt)};
		if(e->len==0)
			e=__builtin_addressof(all->time.d_t_fmt);
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, *e);
	}
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_d_fmt)
	{
		auto* e{__builtin_addressof(all->time.era_d_fmt)};
		if(e->len==0)
			e=__builtin_addressof(all->time.d_fmt);
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, *e);
	}
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_t_fmt)
	{
		auto* e{__builtin_addressof(all->time.era_t_fmt)};
		if(e->len==0)
			e=__builtin_addressof(all->time.t_fmt);
		return details::lc_print_reserve_size_time_format_common_impl(all->time, tsp.reference, *e);
	}
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
	{
		auto* e{__builtin_addressof(all->time.era_d_t_fmt)};
		if(e->len==0)
			e=__builtin_addressof(all->time.d_t_fmt);
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,*e);
	}
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_d_fmt)
	{
		auto* e{__builtin_addressof(all->time.era_d_fmt)};
		if(e->len==0)
			e=__builtin_addressof(all->time.d_fmt);
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,*e);
	}
	else if constexpr(current_lc_time_flag==manipulators::lc_time_flag::era_t_fmt)
	{
		auto* e{__builtin_addressof(all->time.era_t_fmt)};
		if(e->len==0)
			e=__builtin_addressof(all->time.t_fmt);
		return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp.reference,*e);
	}	
}

}