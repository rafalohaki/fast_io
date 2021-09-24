#pragma once

namespace fast_io
{

namespace win32::nt
{

struct utf16_string
{
std::uint16_t Length;
std::uint16_t MaximumLength;
char16_t*  Buffer;
};
struct ansi_string
{
std::uint16_t Length;
std::uint16_t MaximumLength;
char*  Buffer;
};

#if !(__STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1))
struct unicode_string
{
std::uint16_t Length;
std::uint16_t MaximumLength;
wchar_t*  Buffer;
};
#endif

inline constexpr basic_io_scatter_t<wchar_t> print_alias_define(io_alias_t,unicode_string ustr) noexcept
{
	return {ustr.Buffer,static_cast<std::size_t>(ustr.Length>>1)};
}

inline constexpr basic_io_scatter_t<char16_t> print_alias_define(io_alias_t,utf16_string u16str) noexcept
{
	return {u16str.Buffer,static_cast<std::size_t>(u16str.Length>>1)};
}

inline constexpr basic_io_scatter_t<char> print_alias_define(io_alias_t,ansi_string astr) noexcept
{
	return {astr.Buffer,static_cast<std::size_t>(astr.Length)};
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t DbgPrint(char8_t const* fmt,...) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("DbgPrint")
#else
asm("_DbgPrint")
#endif
#else
asm("DbgPrint")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t DbgPrintEx(std::uint32_t comment,std::uint32_t level,char8_t const* fmt,...) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("DbgPrintEx")
#else
asm("_DbgPrintEx")
#endif
#else
asm("DbgPrintEx")
#endif
#endif
;

}

template<std::integral ch_type>
struct basic_nt_dbg
{
	using char_type = ch_type;
	std::uint32_t comment_id{UINT32_MAX};
	std::uint32_t level{};
	static inline constexpr std::size_t output_buffer_alignment_size{512u};
};

template<std::integral char_type>
inline constexpr basic_nt_dbg<char_type> io_value_handle(basic_nt_dbg<char_type> serv) noexcept
{
	return {serv};
}

namespace details
{
struct nt_dbg_carrier
{
	std::uint32_t comment_id;
	std::uint32_t level;
};

struct nt_fmt_parameter
{
	bool wide{};
	bool line{};
};

inline void nt_debug_service_common_with_line_impl([[maybe_unused]] nt_dbg_carrier service,void const* first,void const* last,nt_fmt_parameter para) noexcept
{
	char8_t const* each_fmt;
	char8_t const* last_fmt;
	bool wide{para.wide};
	bool line{para.line};
	if(wide)
	{
		last_fmt=each_fmt=u8"%wZ";
		if(line)
			last_fmt=u8"%wZ\n";
	}
	else
	{
		last_fmt=each_fmt=u8"%Z";
		if(line)
			last_fmt=u8"%Z\n";
	}
	using char_type_const_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char const*;
	using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char*;
	auto first_char_const_ptr{reinterpret_cast<char_type_const_may_alias_ptr>(first)};
	auto last_char_const_ptr{reinterpret_cast<char_type_const_may_alias_ptr>(last)};
	std::size_t n{static_cast<std::size_t>(last_char_const_ptr-first_char_const_ptr)};
	auto first_char_ptr{const_cast<char_type_may_alias_ptr>(first_char_const_ptr)};
	constexpr std::size_t alignment_size{static_cast<std::size_t>(512u)};
	constexpr std::uint16_t u16alignment_size{static_cast<std::uint16_t>(alignment_size)};
	::fast_io::win32::nt::ansi_string nstr
	{
		.Length=u16alignment_size,
		.MaximumLength=u16alignment_size,
		.Buffer=nullptr
	};
	std::size_t i{};
#if _WIN32_WINNT >= 0x0501
	std::uint32_t const comment_id{service.comment_id};
	std::uint32_t const level{service.level};
#endif
	for(;i+alignment_size<=n;i+=alignment_size)
	{
		nstr.Buffer=first_char_ptr+i;
#if _WIN32_WINNT >= 0x0501
		::fast_io::win32::nt::DbgPrintEx(comment_id,level,each_fmt,__builtin_addressof(nstr));
#else
		::fast_io::win32::nt::DbgPrint(each_fmt,__builtin_addressof(nstr));
#endif
	}
	std::size_t const remain_size{static_cast<std::size_t>(n-i)};
	std::uint16_t const u16remain_size{static_cast<std::uint16_t>(remain_size)};
	nstr.MaximumLength=nstr.Length=u16remain_size;
	nstr.Buffer=first_char_ptr+i;
#if _WIN32_WINNT >= 0x0501
	::fast_io::win32::nt::DbgPrintEx(comment_id,level,last_fmt,__builtin_addressof(nstr));
#else
	::fast_io::win32::nt::DbgPrint(last_fmt,__builtin_addressof(nstr));
#endif
}

template<bool line,std::integral char_type>
#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline void nt_debug_service_impl(nt_dbg_carrier service,void const* first,void const* last) noexcept
{
	static_assert(!is_ebcdic<char_type>||(is_ebcdic<char_type>&&!line));
	if constexpr(sizeof(char_type)==sizeof(char16_t)&&(!is_ebcdic<char_type>))
		nt_debug_service_common_with_line_impl(service,first,last,{true,line});
	else
		nt_debug_service_common_with_line_impl(service,first,last,{false,line});
}

}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
inline void write(basic_nt_dbg<char_type> service,Iter first,Iter last) noexcept
{
	::fast_io::details::nt_debug_service_impl<false,char_type>({service.comment_id,service.level},::fast_io::freestanding::to_address(first),::fast_io::freestanding::to_address(last));
}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
requires (!::fast_io::details::is_ebcdic<char_type>)
inline void writeln(basic_nt_dbg<char_type> service,Iter first,Iter last) noexcept
{
	::fast_io::details::nt_debug_service_impl<true,char_type>({service.comment_id,service.level},::fast_io::freestanding::to_address(first),::fast_io::freestanding::to_address(last));
}

using nt_dbg = basic_nt_dbg<char>;
using wnt_dbg = basic_nt_dbg<wchar_t>;
using u8nt_dbg = basic_nt_dbg<char8_t>;
using u16nt_dbg = basic_nt_dbg<char16_t>;
using u32nt_dbg = basic_nt_dbg<char32_t>;

}
