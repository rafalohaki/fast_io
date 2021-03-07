#pragma once

namespace fast_io
{

template<std::integral CharT,typename Traits = std::char_traits<CharT>,typename Allocator = std::allocator<CharT>>
class ostring_ref
{
public:
	using char_type = CharT;
	using allocator_type = Allocator;
	using string_type = std::basic_string<CharT,Traits,Allocator>;
	string_type& reference;
};

template<std::integral char_type,typename traits_type,typename allocator_type>
[[nodiscard]] constexpr auto obuffer_begin(ostring_ref<char_type,traits_type,allocator_type> ob)
{
	return std::to_address(ob.reference.data());
}

template<std::integral char_type,typename traits_type,typename allocator_type>
[[nodiscard]] constexpr auto obuffer_curr(ostring_ref<char_type,traits_type,allocator_type> ob)
{
	return std::to_address(ob.reference.data()+ob.reference.size());
}

template<std::integral char_type,typename traits_type,typename allocator_type>
[[nodiscard]] constexpr auto obuffer_end(ostring_ref<char_type,traits_type,allocator_type> ob)
{
	return std::to_address(ob.reference.data()+ob.reference.capacity());
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void obuffer_set_curr(ostring_ref<char_type,traits_type,allocator_type> ob,char_type* ptr)
{
	traits_type::assign(*ptr, char_type());
	details::string_hack::set_end_ptr(ob.reference,ptr);
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void otakeover(ostring_ref<char_type,traits_type,allocator_type> ob,char_type* beg_ptr,char_type* end_ptr,char_type* cap_ptr)
{
	traits_type::assign(*end_ptr, char_type());
#if defined(_MSVC_STL_UPDATE)
	details::string_hack::set_begin_end_cap_ptrs(ob.reference,beg_ptr,end_ptr,cap_ptr-1);
#else
	details::string_hack::set_begin_ptr(ob.reference,beg_ptr);
	details::string_hack::set_end_ptr(ob.reference,end_ptr);
	details::string_hack::set_cap_ptr(ob.reference,cap_ptr-1);
#endif
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr bool ocan_takeover(ostring_ref<char_type,traits_type,allocator_type> ob)
{
	return details::string_hack::is_local_and_null(ob.reference);
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void oreserve(ostring_ref<char_type,traits_type,allocator_type> ob,std::size_t newsz)
{
	ob.reference.reserve(newsz);
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void overflow(ostring_ref<char_type,traits_type,allocator_type> ob,char_type ch)
{
	ob.reference.push_back(ch);
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void oshrink_to_fit(ostring_ref<char_type,traits_type,allocator_type> ob)
{
	ob.reference.shrink_to_fit();
}

template<std::integral char_type,typename traits_type,typename allocator_type,std::contiguous_iterator Iter>
requires (std::same_as<char_type,char>||
	std::same_as<char_type,typename std::iterator_traits<Iter>::value_type>)
inline constexpr void write(ostring_ref<char_type,traits_type,allocator_type> ob,Iter begin,Iter end)
{
	if constexpr(std::same_as<char_type,std::iter_value_t<Iter>>)
	{
#ifndef _MSVC_STL_UPDATE
		auto curr{obuffer_curr(ob)};
		auto ed{obuffer_end(ob)};
		std::size_t const to_write(end-begin);
		if(curr+to_write<=ed)[[likely]]
		{
			::fast_io::details::my_memcpy(curr,std::to_address(begin),to_write*sizeof(char_type));
			obuffer_set_curr(ob,curr+to_write);
			return;
		}
#endif
		ob.reference.append(std::to_address(begin),end-begin);
	}
	else
		write(ob,reinterpret_cast<char const*>(std::to_address(begin)),reinterpret_cast<char const*>(std::to_address(end)));
}

template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr void flush(ostring_ref<char_type,traits_type,allocator_type>){}
template<std::integral char_type,typename traits_type,typename allocator_type>
inline constexpr decltype(auto) oallocator(ostring_ref<char_type,traits_type,allocator_type> ref)
{
	return ref.reference.get_allocator();
}

static_assert(dynamic_buffer_output_stream<ostring_ref<char>>);

template<std::integral char_type,typename traits_type,typename allocator_type> ostring_ref(std::basic_string<char_type,traits_type,allocator_type>&) -> ostring_ref<char_type,traits_type,allocator_type>;

}