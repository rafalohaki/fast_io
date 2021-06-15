#pragma once

namespace fast_io
{

namespace details
{
#if 0
inline constexpr void report_posix_error_impl(error_reporter& report,int ec)
{
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
		print_freestanding(report,get_posix_errno_scatter<char>(ec));
	else
	{
#endif
/*
If execution charset is none-ASCII based charset. Use our own implementation. This is to correctly deal with EBCDIC exec-charset
*/
		if constexpr('A'!=u8'A')
		{
			print_freestanding(report,get_posix_errno_scatter<char>(ec));
		}
		else
		{
/*
#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
			constexpr std::size_t buffer_size{1024};
			char buffer[buffer_size];
			if(::strerror_r(ec,buffer,buffer_size))
				buffer[0]=0;
			print_freestanding(report,fast_io::manipulators::chvw(buffer));
*/
#if defined(_GNU_SOURCE)
/*
Based on man. glibc provides a special function strerrordesc_np() which will not get affected by locale. good! No locale garbage please.
https://man7.org/linux/man-pages/man3/strerrordesc_np.3.html
strerrordesc_np not found
*/
#if 0
			print_freestanding(report,fast_io::manipulators::chvw(strerrordesc_np(ec)));
#endif
			constexpr std::size_t buffer_size{1024};
			char buffer[buffer_size];
			print_freestanding(report,fast_io::manipulators::chvw(::strerror_r(ec,buffer,buffer_size)));
#elif defined(_WIN32)
/*
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-s-strerror-s-wcserror-s-wcserror-s?view=msvc-160
*/
			constexpr std::size_t buffer_size{256};
			char buffer[buffer_size];
			int failed{::strerror_s(buffer,buffer_size,ec)};
			if(failed)
				buffer[0]=0;
			print_freestanding(report,fast_io::manipulators::chvw(buffer));
#else
/*
Any other platforms. Particularly BSDs will avoid strerror_xxx functions.
WTF is this garbage? FUCK FreeBSD.
ALL C programmers are GARBAGE and Braindeath.

LET APPLE AND BSD DIE!

https://github.com/freebsd/freebsd/blob/e8142e44905fa62147ba10703e7ce2c5464fb2b7/lib/libc/string/strerror.c#L120
*/
			print_freestanding(report,get_posix_errno_scatter<char>(ec));
#endif
		}
#if __cpp_lib_is_constant_evaluated >= 201811L
	}
#endif
}
#endif
}

enum class posix_code:int
{};

#if 0
template<>
inline constexpr std::size_t error_code_domain_value<posix_code>
{
#if SIZE_MAX==UINT16_MAX

#elif SIZE_MAX==UINT32_MAX

#elif SIZE_MAX==UINT64_MAX

#elif defined(UINT128_MAX) && SIZE_MAX==UINT128_MAX

#else
#error "unsupport architecture"
#endif
};

inline constexpr bool operator==(error e,posix_code code) noexcept
{

}
#endif

class posix_error
{
public:
	int ec{};
	explicit posix_error(int errn=errno):ec(errn){}
	constexpr auto code() const noexcept
	{
		return ec;
	}
};

[[noreturn]] inline void throw_posix_error()
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw posix_error();
#endif
#else
	fast_terminate();
#endif
}
[[noreturn]] inline void throw_posix_error([[maybe_unused]] int err)
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw posix_error(err);
#endif
#else
	fast_terminate();
#endif
}

template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_alias_define(io_alias_type_t<char_type>,posix_error const& perr) noexcept
{
	return get_posix_errno_scatter<char_type>(perr.ec);
}


template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,posix_error const& perr) noexcept
{
	return get_posix_errno_scatter<char_type>(static_cast<int>(perr.ec));
}


}
