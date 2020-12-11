#pragma once

namespace fast_io
{

class posix_error:public fast_io_error
{
public:
	int ec{};
	explicit posix_error(int errn=errno):ec(errn){}
	constexpr auto code() const noexcept
	{
		return ec;
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void report(error_reporter& report) const override
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
#ifdef _GNU_SOURCE
/*
Based on man. glibc provides a special function strerrordesc_np() which will not get affected by locale. good! No locale garbage please.
https://man7.org/linux/man-pages/man3/strerrordesc_np.3.html
*/
				print_freestanding(report,fast_io::chvw(strerrordesc_np(ec)));
#elif defined(_WIN32)
/*
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strerror-s-strerror-s-wcserror-s-wcserror-s?view=msvc-160
*/
				constexpr std::size_t buffer_size{256};
				char buffer[buffer_size];
				int failed{::strerror_s(buffer,buffer_size,ec)};
				if(failed)
					buffer[0]=0;
				print_freestanding(report,fast_io::chvw(buffer));
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
};

[[noreturn]] inline void throw_posix_error()
{
#ifdef __cpp_exceptions
	throw posix_error();
#else
	fast_terminate();
#endif
}
[[noreturn]] inline void throw_posix_error(int err)
{
#ifdef __cpp_exceptions
	throw posix_error(err);
#else
	fast_terminate();
#endif
}

template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_alias_define(io_alias_type_t<char_type>,posix_error const& perr) noexcept
{
	return get_posix_errno_scatter<char_type>(perr.ec);
}

}
