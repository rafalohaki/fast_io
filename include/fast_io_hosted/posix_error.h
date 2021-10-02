#pragma once

namespace fast_io
{

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
inline constexpr basic_io_scatter_t<char_type> status_io_print_forward(io_alias_type_t<char_type>,posix_error const& perr) noexcept
{
	return get_posix_errno_scatter<char_type>(perr.ec);
}

}
