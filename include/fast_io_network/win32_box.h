#pragma once
#include"win32_library.h"

namespace fast_io
{

template<std::integral ch_type>
requires (std::same_as<ch_type,char>||std::same_as<ch_type,wchar_t>)
class basic_win32_box
{
public:
	using char_type = ch_type;
	using string_view_type = std::basic_string_view<char_type>;
private:
	std::basic_string_view<char_type> view;
public:
	constexpr basic_win32_box(std::basic_string_view<char_type> view):view(view.data()){}
	constexpr std::basic_string_view<char_type> title() const noexcept
	{
		return view;
	}
};

namespace details
{

template<typename func,std::integral char_type,std::contiguous_iterator Iter>
inline void win32_box_write_impl(func func_ptr,char_type const* title,Iter begin,Iter end)
{
	if(func_ptr==nullptr)
		throw_win32_error();
	std::basic_string<char_type> str(begin,end);
	std::erase_if(str,[](auto ch)
	{
		return ch==0;
	});
	if(!func_ptr(nullptr,str.c_str(),title,0x00000040L/*MB_ICONINFORMATION*/))
		throw_win32_error();
}

}

template<std::integral ch_type,std::contiguous_iterator Iter>
requires (std::same_as<ch_type,std::iter_value_t<Iter>>||std::same_as<ch_type,char>)
inline void write(basic_win32_box<ch_type> box,Iter b,Iter e)
{
	if constexpr(std::same_as<ch_type,std::iter_value_t<Iter>>)
	{
		fast_io::details::win32_library const user32_dll(L"user32.dll");
		if constexpr(std::same_as<ch_type,char>)
			details::win32_box_write_impl(bit_cast<int(*)(void*,char const*,char const*,std::uint32_t)>
				(::GetProcAddress(user32_dll.get(),"MessageBoxA")),box.title().data(),b,e);
		else
			details::win32_box_write_impl(bit_cast<int(*)(void*,wchar_t const*,wchar_t const*,std::uint32_t)>
				(::GetProcAddress(user32_dll.get(),"MessageBoxW")),box.title().data(),b,e);
	}
	else
		write(box,reinterpret_cast<ch_type const*>(std::to_address(b)),
			reinterpret_cast<ch_type const*>(std::to_address(e)));
}

template<std::integral ch_type>
using basic_native_box = basic_win32_box<ch_type>;

using win32_box = basic_native_box<char>;
using wwin32_box = basic_native_box<wchar_t>;

using native_box = win32_box;
using wnative_box = wwin32_box;

}