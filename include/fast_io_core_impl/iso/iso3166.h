#pragma once

namespace fast_io
{

template<std::integral char_type,std::size_t alpha>
requires (alpha==2||alpha==3)
struct basic_iso3166_alpha
{
	::fast_io::freestanding::array<char_type,alpha> data{};
};

using iso3166_alpha2 = basic_iso3166_alpha<char,2>;
using wiso3166_alpha2 = basic_iso3166_alpha<wchar_t,2>;
using u8iso3166_alpha2 = basic_iso3166_alpha<char8_t,2>;
using u16iso3166_alpha2 = basic_iso3166_alpha<char16_t,2>;
using u32iso3166_alpha2 = basic_iso3166_alpha<char32_t,2>;

using iso3166_alpha3 = basic_iso3166_alpha<char,3>;
using wiso3166_alpha3 = basic_iso3166_alpha<wchar_t,3>;
using u8iso3166_alpha3 = basic_iso3166_alpha<char8_t,3>;
using u16iso3166_alpha3 = basic_iso3166_alpha<char16_t,3>;
using u32iso3166_alpha3 = basic_iso3166_alpha<char32_t,3>;

template<std::integral char_type,std::size_t numeric>
requires (numeric==3)
struct basic_iso3166_numeric
{
	using unsigned_integer_type = std::uint16_t;
	unsigned_integer_type code{};
};

using iso3166_numeric3 = basic_iso3166_numeric<char,3>;
using wiso3166_numeric3 = basic_iso3166_numeric<wchar_t,3>;
using u8iso3166_numeric3 = basic_iso3166_numeric<char8_t,3>;
using u16iso3166_numeric3 = basic_iso3166_numeric<char16_t,3>;
using u32iso3166_numeric3 = basic_iso3166_numeric<char32_t,3>;

template<std::integral char_type,std::size_t alpha>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,basic_iso3166_alpha<char_type,alpha>>) noexcept
{
	return alpha;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator Iter,std::size_t alpha>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,basic_iso3166_alpha<char_type,alpha>>,Iter iter,basic_iso3166_alpha<char_type,alpha> v) noexcept
{
	return non_overlapped_copy_n(v.data.data(),alpha,iter);
}


template<std::integral char_type,std::size_t alpha>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,basic_iso3166_numeric<char_type,alpha>>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::uint16_t>);
}

namespace details
{
template<std::integral char_type,::fast_io::freestanding::random_access_iterator Iter,std::size_t alpha>
inline constexpr Iter print_reserve_define_iso3166_numeric_impl(Iter iter,basic_iso3166_numeric<char_type,alpha> v) noexcept
{
	if(1000<=v.code)
		return print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,v.code);
#ifdef __OPTIMIZE_SIZE__
	optimize_size::with_length::output_unsigned(iter,v.code,3);
	iter+=3;
	return iter;
#elif defined(FAST_IO_OPTIMIZE_TIME)
	return non_overlapped_copy_n(jiaendu::static_tables<char_type>::table3[v.code].data(),3,iter);
#else
	if constexpr(std::same_as<char_type,char>)
		*iter='0';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'0';
	else
		*iter=u8'0';
	*iter+=v.code/100;
	v.code%=100;
	++iter;
	constexpr auto table(details::get_shared_inline_constexpr_base_table<char_type,10,false,false>().data());
	return non_overlapped_copy_n(table[v.code].data(),2,iter);
#endif
}

}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator Iter,std::size_t alpha>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,basic_iso3166_numeric<char_type,alpha>>,Iter iter,basic_iso3166_numeric<char_type,alpha> v) noexcept
{
	return details::print_reserve_define_iso3166_numeric_impl(iter,v);
}



}