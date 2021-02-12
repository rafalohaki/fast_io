#pragma once

namespace fast_io
{

class openssl_error:public std::exception
{
public:
#if 0
	virtual	void report(error_reporter& err) const override
#ifdef __cpp_exceptions
	;
#else
	{}
#endif
#endif
};


inline void throw_openssl_error()
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw openssl_error();
#endif
#else
	fast_terminate();
#endif
}

}