#pragma once

namespace fast_io
{

namespace details
{

inline constexpr std::size_t print_reserve_size_source_location_impl(std::source_location const& location) noexcept
{
	constexpr std::size_t uint32_rsv_size{print_reserve_size(io_reserve_type<char,std::uint_least32_t>)};
	constexpr std::size_t total_uint32_t_rsv_size{uint32_rsv_size*2+3};
	std::size_t const file_name_strlen{std::strlen(location.file_name())};
	std::size_t const function_strlen{std::strlen(location.function_name())};
	return intrinsics::add_or_overflow_die_chain(file_name_strlen,function_strlen,total_uint32_t_rsv_size);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_source_location_impl(Iter iter,std::source_location const& location) noexcept
{
	std::size_t const file_name_strlen{std::strlen(location.file_name())};
	std::size_t const function_strlen{std::strlen(location.function_name())};
	*(iter=non_overlapped_copy_n(location.file_name(),file_name_strlen,iter))=':';
	*(iter=print_reserve_define(io_reserve_type<char,std::uint_least32_t>,++iter,location.line()))=':';
	*(iter=print_reserve_define(io_reserve_type<char,std::uint_least32_t>,++iter,location.column()))=':';
	return non_overlapped_copy_n(location.function_name(),function_strlen,++iter);
}

}

inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char,std::source_location>,std::source_location const& location) noexcept
{
	return details::print_reserve_size_source_location_impl(location);
}

template<std::contiguous_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char,std::source_location>,Iter iter,std::source_location const& location) noexcept
{
	return details::print_reserve_define_source_location_impl(iter,location);
}

}