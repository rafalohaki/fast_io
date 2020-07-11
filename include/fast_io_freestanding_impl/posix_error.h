#pragma once

namespace fast_io
{

class posix_error:public fast_io_error
{
	int ecd;
public:
	explicit posix_error(int errn=errno):ecd(errn){}
	constexpr auto code() const noexcept
	{
		return ecd;
	}
#if __cpp_constexpr >= 201907L
	//constexpr
#endif
	void report(error_reporter& report) const override
	{
		print(report,chvw(strerror(ecd)));
	}
};

inline void throw_posix_error()
{
#ifdef __cpp_exceptions
	throw posix_error();
#else
	fast_terminate();
#endif
}
inline void throw_posix_error(int err)
{
#ifdef __cpp_exceptions
	throw posix_error(err);
#else
	fast_terminate();
#endif
}
}
