#pragma once

namespace fast_io
{

namespace details
{
/*

https://www.ibm.com/docs/en/aix/7.1?topic=ff-lc-time-category-locale-definition-source-file-format
*/
template<std::integral char_type>
inline constexpr std::size_t lc_print_reserve_size_time_format_common_impl(lc_time const& t,basic_io_scatter_t<char_type> const& format_str) noexcept
{
/*
Change it thank you!
*/
//	__builtin_printf("here done\n");
	return print_reserve_size(io_reserve_type<char_type,iso8601_timestamp>);
}

template<::fast_io::freestanding::random_access_iterator Iter,std::integral char_type>
requires std::same_as<::fast_io::freestanding::iter_value_t<Iter>,char_type>
inline constexpr Iter lc_print_reserve_define_time_fmt_common_impl(lc_time const& t,Iter iter,iso8601_timestamp const& tsp,basic_io_scatter_t<char_type> const& format_str)
{
/*
Change it thank you!
*/
//	__builtin_printf("here okay!\n");
	return print_reserve_define(io_reserve_type<char_type,iso8601_timestamp>,iter,tsp);
}
}


template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* all,iso8601_timestamp const& /*timestamp does not matter it looks like in this case*/) noexcept
{
	return details::lc_print_reserve_size_time_format_common_impl(all->time,all->time.d_t_fmt);
}

template<::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* all,Iter iter,iso8601_timestamp const& tsp) noexcept
{
	return details::lc_print_reserve_define_time_fmt_common_impl(all->time,iter,tsp,all->time.d_t_fmt);
}

static_assert(lc_dynamic_reserve_printable<char8_t,fast_io::iso8601_timestamp>);

}