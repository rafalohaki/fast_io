#pragma once

namespace fast_io
{

template<std::integral CharT,typename Traits = std::char_traits<CharT>,typename Allocator = std::allocator<CharT>>
class basic_ostring_ref
{
public:
	using string_type = std::basic_string<CharT,Traits,Allocator>;
	using native_handle_type = string_type*;
	using char_type = CharT;
	using allocator_type = Allocator;
	using pointer = typename string_type::pointer;
	native_handle_type ptr{};
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{ptr};
		ptr=nullptr;
		return temp;
	}
	inline constexpr native_handle_type native_handle() const noexcept
	{
		return ptr;
	}
};

template<std::integral CharT,typename Traits,typename Allocator>
[[nodiscard]] inline constexpr basic_ostring_ref<CharT,Traits,Allocator> io_value_handle(basic_ostring_ref<CharT,Traits,Allocator> bref) noexcept
{
	return bref;
}

template<std::integral CharT,typename Traits,typename Allocator>
[[nodiscard]] constexpr auto obuffer_begin(basic_ostring_ref<CharT,Traits,Allocator> ob) noexcept
{
	return ob.ptr->data();
}

template<std::integral CharT,typename Traits,typename Allocator>
[[nodiscard]] constexpr auto obuffer_curr(basic_ostring_ref<CharT,Traits,Allocator> ob) noexcept
{
	return ob.ptr->data()+ob.ptr->size();
}

template<std::integral CharT,typename Traits,typename Allocator>
[[nodiscard]] constexpr auto obuffer_end(basic_ostring_ref<CharT,Traits,Allocator> ob) noexcept
{
	return ob.ptr->data()+ob.ptr->capacity();
}

template<std::integral CharT,typename Traits,typename Allocator>
inline constexpr void obuffer_set_curr(basic_ostring_ref<CharT,Traits,Allocator> ob,
	CharT* ptr)
{
	Traits::assign(*ptr, CharT());
	details::string_hack::set_end_ptr(*ob.ptr,ptr);
}
#if 0
template<typename T>
inline constexpr void otakeover(basic_ostring_ref<CharT,Traits,Allocator> ob,char_type* beg_ptr,char_type* end_ptr,char_type* cap_ptr)
{
	*end_ptr={};
#if defined(_MSVC_STL_UPDATE)
	details::string_hack::set_begin_end_cap_ptrs(*ob.ptr,beg_ptr,end_ptr,cap_ptr-1);
#else
	details::string_hack::set_begin_ptr(*ob.ptr,beg_ptr);
	details::string_hack::set_end_ptr(*ob.ptr,end_ptr);
	details::string_hack::set_cap_ptr(*ob.ptr,cap_ptr-1);
#endif
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr bool ocan_takeover(ostring_ref<char_type,traits_type,allocator_type> ob)
{
	return details::string_hack::is_local_and_null(ob.reference);
}
#endif

template<std::integral CharT,typename Traits,typename Allocator>
inline constexpr void overflow(basic_ostring_ref<CharT,Traits,Allocator> ob,CharT ch)
{
	ob.ptr.push_back(ch);
}

template<std::integral CharT,typename Traits,typename Allocator>
inline constexpr void oshrink_to_fit(basic_ostring_ref<CharT,Traits,Allocator> ob)
{
	ob.ptr->shrink_to_fit();
}

template<std::integral CharT,typename Traits,typename Allocator>
inline constexpr void oreserve(basic_ostring_ref<CharT,Traits,Allocator> ob,std::size_t n)
{
	ob.ptr->reserve(n);
}

template<std::integral char_type,typename Traits,typename Allocator,std::input_or_output_iterator Iter>
requires (std::same_as<char_type,char>||
	std::same_as<char_type,typename std::iterator_traits<Iter>::value_type>)
inline constexpr void write(basic_ostring_ref<char_type,Traits,Allocator> ob,Iter begin,Iter end)
{
	if constexpr(std::same_as<char_type,std::iter_value_t<Iter>>)
		ob.ptr->append(begin,end);
	else
		write(ob,reinterpret_cast<char const*>(std::to_address(begin)),reinterpret_cast<char const*>(std::to_address(end)));
}

using ostring_ref = basic_ostring_ref<char>;
using wostring_ref = basic_ostring_ref<wchar_t>;
using u8ostring_ref = basic_ostring_ref<char8_t>;
using u16ostring_ref = basic_ostring_ref<char16_t>;
using u32ostring_ref = basic_ostring_ref<char32_t>;

static_assert(value_based_stream<ostring_ref>);

}