#pragma once

#ifdef __GNUC__
#pragma GCC system_header
#endif

namespace fast_io
{

namespace details
{

template<std::size_t size,::fast_io::freestanding::forward_iterator Iter>
inline constexpr Iter print_reserve_df_mm_impl_internal(Iter iter,char unsigned* mm) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using uhex_manip_type = ::fast_io::mnp::base_full_t<16,true,char unsigned>;
	if constexpr(std::same_as<char_type,char>)
		iter=copy_string_literal("(0x",iter);
	else if constexpr(std::same_as<char_type,wchar_t>)
		iter=copy_string_literal(L"(0x",iter);
	else if constexpr(std::same_as<char_type,char16_t>)
		iter=copy_string_literal(u"(0x",iter);
	else if constexpr(std::same_as<char_type,char32_t>)
		iter=copy_string_literal(U"(0x",iter);
	else
		iter=copy_string_literal(u8"(0x",iter);
	iter=print_reserve_define(io_reserve_type<char_type,
	uhex_manip_type>,iter,{*mm});
	for(std::size_t i{1};i!=size;++i)
	{
		if constexpr(std::same_as<char_type,char>)
			iter=copy_string_literal(",0x",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			iter=copy_string_literal(L",0x",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			iter=copy_string_literal(u",0x",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			iter=copy_string_literal(U",0x",iter);
		else
			iter=copy_string_literal(u8",0x",iter);
		iter=print_reserve_define(io_reserve_type<char_type,
		uhex_manip_type>,iter,{mm[i]});
	}
	if constexpr(std::same_as<char_type,char>)
		*iter=')';		
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L')';
	else 
		*iter=u8')';
	++iter;
	return iter;
}

template<::fast_io::freestanding::forward_iterator Iter,typename T>
inline constexpr Iter print_reserve_df_mm_impl(Iter iter,T mm) noexcept
{
	return print_reserve_df_mm_impl_internal<sizeof(T)>(
		iter,bit_cast<::fast_io::freestanding::array<char unsigned,sizeof(T)>>(mm).data());
}

}

namespace intrinsics
{
using x86_64_m128 [[gnu::__vector_size__(16),gnu::may_alias]] = float ;
using x86_64_m128i [[gnu::__vector_size__ (16),gnu::may_alias]] = long long;
using x86_64_v16qi [[gnu::__vector_size__ (16)]] = char;
using x86_64_v8hi [[gnu::__vector_size__ (16)]] = short;

using x86_64_m256 [[gnu::__vector_size__(32),gnu::may_alias]]  = float;
using x86_64_m256i [[gnu::__vector_size__ (32),gnu::may_alias]] = long long;
using x86_64_m512 [[gnu::__vector_size__(64),gnu::may_alias]]  = float ;
using x86_64_m512i [[gnu::__vector_size__ (64),gnu::may_alias]] = long long;
using x86_64_m1024 [[gnu::__vector_size__(128),gnu::may_alias]]  = float ;
using x86_64_m1024i [[gnu::__vector_size__ (128),gnu::may_alias]] = long long;
using x86_64_m2048 [[gnu::__vector_size__(256),gnu::may_alias]]  = float ;
using x86_64_m2048i [[gnu::__vector_size__ (256),gnu::may_alias]] = long long;
using x86_64_m4096 [[gnu::__vector_size__(512),gnu::may_alias]]  = float ;
using x86_64_m4096i [[gnu::__vector_size__ (512),gnu::may_alias]] = long long;
}

template<std::integral char_type,typename T>
requires
(
std::same_as<T,intrinsics::x86_64_m128>||std::same_as<T,intrinsics::x86_64_m128i>
||std::same_as<T,intrinsics::x86_64_m256>||std::same_as<T,intrinsics::x86_64_m256i>
||std::same_as<T,intrinsics::x86_64_m512>||std::same_as<T,intrinsics::x86_64_m512i>
||std::same_as<T,intrinsics::x86_64_m1024>||std::same_as<T,intrinsics::x86_64_m1024i>
||std::same_as<T,intrinsics::x86_64_m2048>||std::same_as<T,intrinsics::x86_64_m2048i>
||std::same_as<T,intrinsics::x86_64_m4096>||std::same_as<T,intrinsics::x86_64_m4096i>
)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,T>) noexcept
{
	constexpr std::size_t sz{sizeof(T)*5+1};
	return sz;
}

template<std::integral char_type,::fast_io::freestanding::forward_iterator Iter,typename T>
requires
(
std::same_as<T,intrinsics::x86_64_m128>||std::same_as<T,intrinsics::x86_64_m128i>
||std::same_as<T,intrinsics::x86_64_m256>||std::same_as<T,intrinsics::x86_64_m256i>
||std::same_as<T,intrinsics::x86_64_m512>||std::same_as<T,intrinsics::x86_64_m512i>
||std::same_as<T,intrinsics::x86_64_m1024>||std::same_as<T,intrinsics::x86_64_m1024i>
||std::same_as<T,intrinsics::x86_64_m2048>||std::same_as<T,intrinsics::x86_64_m2048i>
||std::same_as<T,intrinsics::x86_64_m4096>||std::same_as<T,intrinsics::x86_64_m4096i>
)
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,T>,Iter iter,T t) noexcept
{
	return details::print_reserve_df_mm_impl(iter,t);
}

}