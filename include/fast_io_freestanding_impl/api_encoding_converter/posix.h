﻿#pragma once

namespace fast_io
{

struct posix_api_encoding_converter
{
	using char_type = char8_t;
	std::size_t buffer_capacity;
	char_type* buffer_data;
	char_type* buffer_data_end;
	template<std::integral from_char_type>
	requires (sizeof(from_char_type)!=1)
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	constexpr
#endif
	posix_api_encoding_converter(from_char_type const* p,std::size_t sz) noexcept:
		buffer_capacity(details::cal_decorated_reserve_size<sizeof(from_char_type),sizeof(char_type)>(sz)),
		buffer_data(details::allocate_iobuf_space<char_type>(::fast_io::details::intrinsics::add_or_overflow_die(buffer_capacity,1))),
		buffer_data_end(details::codecvt::general_code_cvt_full(p,p+sz,buffer_data))
	{
		*buffer_data_end=0;
	}
	template<std::integral from_char_type>
	requires (sizeof(from_char_type)==1)
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	constexpr
#endif
	posix_api_encoding_converter(from_char_type const* p,std::size_t sz) noexcept:
		buffer_capacity(sz),
		buffer_data(details::allocate_iobuf_space<char_type>(::fast_io::details::intrinsics::add_or_overflow_die(buffer_capacity,1))),
		buffer_data_end(details::non_overlapped_copy_n(p,sz,buffer_data))
	{
		*buffer_data_end=0;
	}
	inline constexpr char_type const* c_str() const noexcept
	{
		return buffer_data;
	}
	inline char const* native_c_str() const noexcept
	{
		return reinterpret_cast<char const*>(buffer_data);
	}
	inline constexpr std::size_t capacity() const noexcept
	{
		return buffer_capacity;
	}
	inline constexpr std::size_t size() const noexcept
	{
		return static_cast<std::size_t>(buffer_data_end-buffer_data);
	}
 	posix_api_encoding_converter(posix_api_encoding_converter const&)=delete;
	posix_api_encoding_converter& operator=(posix_api_encoding_converter const&)=delete;
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	constexpr
#endif
	~posix_api_encoding_converter()
	{
		details::deallocate_iobuf_space<false>(buffer_data,buffer_capacity+1);
	}
};
namespace details
{
template<std::integral char_type,typename Func>
requires (sizeof(char_type)!=sizeof(char))
inline auto posix_api_common_codecvt_impl(char_type const* filename_c_str,std::size_t filename_c_str_len,Func callback)
{
	posix_api_encoding_converter converter(filename_c_str,filename_c_str_len);
	return callback(converter.native_c_str());
}
}
template<typename T,typename Func>
requires (::fast_io::constructible_to_os_c_str<T>)
inline auto posix_api_common(T const& t,Func callback)
{
	if constexpr(::std::is_array_v<T>)
	{
		using cstr_char_type = std::remove_extent_t<T>;
		static_assert(::std::integral<cstr_char_type>);
		auto p{t};
		if constexpr(sizeof(cstr_char_type)==sizeof(char))
		{
			return callback(reinterpret_cast<char const*>(p));
		}
		else
		{
			return ::fast_io::details::posix_api_common_codecvt_impl(p,::fast_io::details::cal_array_size(t),callback);
		}
	}
	else
	{
		using cstr_char_type = std::remove_pointer_t<decltype(t.c_str())>;
		if constexpr(sizeof(cstr_char_type)==sizeof(char))
		{
			return callback(reinterpret_cast<char const*>(t.c_str()));
		}
		else
		{
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1) && __has_include(<ranges>)
			if constexpr(::std::ranges::contiguous_range<std::remove_cvref_t<T>>)
			{
				return ::fast_io::details::posix_api_common_codecvt_impl(::std::ranges::data(t),::std::ranges::size(t),callback);
			}
			else
#endif
			{
				auto ptr{t.c_str()};
				return ::fast_io::details::posix_api_common_codecvt_impl(ptr,::fast_io::cstr_len(ptr),callback);
			}
		}
	}
}
}