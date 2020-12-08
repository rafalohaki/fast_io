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
		print_freestanding(report,get_posix_errno_scatter<char>(ec));
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
