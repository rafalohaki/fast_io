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

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t DbgPrint(char const* fmt,...) noexcept
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
extern std::uint32_t DbgPrintEx(std::uint32_t comment,std::uint32_t level,char const* fmt,...) noexcept
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
	std::uint32_t comment_id{101};
	std::uint32_t level{3};
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
	std::uint32_t comment_id{};
	std::uint32_t level{};
};

template<std::integral char_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,char16_t>)
inline void nt_debug_service_common_impl([[maybe_unused]] nt_dbg_carrier service,void const* first,void const* last) noexcept
{
/*
BREAKPOINT_PRINT
*/
	using char_type_const_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type const*;
	using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type*;
	auto first_char_const_ptr{reinterpret_cast<char_type_const_may_alias_ptr>(first)};
	auto last_char_const_ptr{reinterpret_cast<char_type_const_may_alias_ptr>(last)};
	std::size_t n{static_cast<std::size_t>(last_char_const_ptr-first_char_const_ptr)};
	auto first_char_ptr{const_cast<char_type_may_alias_ptr>(first_char_const_ptr)};
	constexpr std::size_t alignment_size{static_cast<std::size_t>(512u)};
	constexpr std::uint16_t u16alignment_size{static_cast<std::uint16_t>(alignment_size)};
	std::conditional_t<std::same_as<char_type,char16_t>,::fast_io::win32::nt::utf16_string,::fast_io::win32::nt::ansi_string> nstr
	{
		.Length=u16alignment_size,
		.MaximumLength=u16alignment_size,
		.Buffer=nullptr
	};
	constexpr char const* fmt{std::same_as<char_type,char16_t>?"%wZ":"%Z"};
	std::size_t i{};
#if _WIN32_WINNT >= 0x0501
	std::uint32_t const comment_id{service.comment_id};
	std::uint32_t const level{service.level};
#endif
	for(;i+alignment_size<=n;i+=alignment_size)
	{
		nstr.Buffer=first_char_ptr+i;
#if _WIN32_WINNT >= 0x0501
		::fast_io::win32::nt::DbgPrintEx(comment_id,level,fmt,__builtin_addressof(nstr));
#else
		::fast_io::win32::nt::DbgPrint(fmt,__builtin_addressof(nstr));
#endif
	}
	std::size_t const remain_size{static_cast<std::size_t>(n-i)};
	std::uint16_t const u16remain_size{static_cast<std::uint16_t>(remain_size)};
	nstr.MaximumLength=nstr.Length=u16remain_size;
	nstr.Buffer=first_char_ptr+i;
	::fast_io::win32::nt::DbgPrint("u16remain_size:%u\n",static_cast<std::uint16_t>(u16remain_size));
#if _WIN32_WINNT >= 0x0501
	::fast_io::win32::nt::DbgPrintEx(comment_id,level,fmt,__builtin_addressof(nstr));
#else
	::fast_io::win32::nt::DbgPrint(fmt,__builtin_addressof(nstr));
#endif

}

template<std::integral char_type>
#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline void nt_debug_service_impl(nt_dbg_carrier service,void const* first,void const* last) noexcept
{
	if constexpr(sizeof(char_type)==sizeof(char16_t))
		nt_debug_service_common_impl<char16_t>(service,first,last);
	else
		nt_debug_service_common_impl<char>(service,first,last);
}

}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
inline void write(basic_nt_dbg<char_type> service,Iter first,Iter last) noexcept
{
	::fast_io::details::nt_debug_service_impl<char_type>({service.comment_id,service.level},::fast_io::freestanding::to_address(first),::fast_io::freestanding::to_address(last));
}

using nt_dbg = basic_nt_dbg<char>;
using wnt_dbg = basic_nt_dbg<wchar_t>;
using u8nt_dbg = basic_nt_dbg<char8_t>;
using u16nt_dbg = basic_nt_dbg<char16_t>;
using u32nt_dbg = basic_nt_dbg<char32_t>;

}
