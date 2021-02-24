#pragma once

namespace fast_io
{

namespace details
{
#if 0
inline constexpr void report_win32_error(error_reporter& report,std::uint32_t ec)
{
	constexpr std::size_t buffer_size{32768};
	reserve_write(report,buffer_size,[&](auto ptr)
	{
		auto const buffer_length(win32::FormatMessageA(
		0x00000200 | 0x00001000,
		nullptr,
		ec,
		(1 << 10),
		ptr,
		buffer_size,
		nullptr));
		return ptr+buffer_length;
	});
}
#endif
}

class win32_error : public std::exception
{
public:
	std::uint32_t ec;
	explicit win32_error(std::uint32_t error = win32::GetLastError()):ec(error){}
	constexpr auto code() const noexcept
	{
		return ec;
	}
#if 0
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void report(error_reporter& report) const override
	{
		details::report_win32_error(report,ec);
	}
#endif
};

namespace details
{

inline std::size_t print_reserve_define_win32_error_impl(wchar_t* ptr, std::uint32_t ec) noexcept
{
	constexpr std::size_t char16_buffer_size{32768};
	return win32::FormatMessageW(
	0x00000200 | 0x00001000,
	nullptr,
	ec,
	(1 << 10),
	ptr,
	char16_buffer_size,
	nullptr);
}

template<std::integral char_type>
inline constexpr char_type* print_reserve_define_win32_error_ptr_impl(char_type* ptr, std::uint32_t ec) noexcept
{
	if constexpr(sizeof(char_type)==2)
	{
		using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= wchar_t*;
		return print_reserve_define_win32_error_impl(reinterpret_cast<char_type_may_alias_ptr>(ptr),ec);
	}
	else
	{
		std::array<char16_t,32768> buffer;
		using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= wchar_t*;
		std::size_t sz{print_reserve_define_win32_error_impl(reinterpret_cast<char_type_may_alias_ptr>(buffer.data()),ec)};
		return ::fast_io::details::codecvt::general_code_cvt_full(buffer.data(),buffer.data()+sz,ptr);
	}
}

template<std::contiguous_iterator Iter>
inline constexpr Iter print_reserve_define_win32_error_impl(Iter iter,std::uint32_t ec) noexcept
{
	if constexpr(std::is_pointer_v<std::iter_value_t<Iter>>)
		return print_reserve_define_win32_error_ptr_impl(iter,ec);
	else
	{
		return print_reserve_define_win32_error_ptr_impl(std::to_address(iter),ec)-std::to_address(iter)+iter;
	}
}

}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,win32_error>) noexcept
{
	if constexpr(sizeof(char_type)==2)
		return 32768;
	else
	{
		constexpr std::size_t sz{::fast_io::details::cal_decorated_reserve_size<2,sizeof(char_type)>(32768)};
		return sz;
	}
}

template<std::contiguous_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,win32_error>,Iter iter,win32_error const& e) noexcept
{
	return details::print_reserve_define_win32_error_impl(iter,e.code());
}

[[noreturn]] inline void throw_win32_error()
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw win32_error();
#endif
#else
	fast_terminate();
#endif
}
[[noreturn]] inline void throw_win32_error([[maybe_unused]] std::uint32_t err)
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw win32_error(err);
#endif
#else
	fast_terminate();
#endif
}

}