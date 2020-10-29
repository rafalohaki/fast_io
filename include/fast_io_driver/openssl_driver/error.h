#pragma once

namespace fast_io
{

class openssl_error:public fast_io_error
{
public:
	virtual	void report(error_reporter& err) const override;
};


inline void throw_openssl_error()
{
#ifdef __cpp_exceptions
	throw openssl_error();
#else
	fast_terminate();
#endif
}

}