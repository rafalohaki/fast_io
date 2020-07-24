#pragma once

namespace fast_io
{

class malformed_input:public fast_io_error
{
public:
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void report(error_reporter& err) const override
	{
		print(err,"malformed input");
	}
};
inline void throw_malformed_input()
{
#ifdef __cpp_exceptions
	throw malformed_input();
#else
	fast_terminate();
#endif
}

class input_overflow_error:public malformed_input
{
public:
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void report(error_reporter& err) const override
	{
		print(err,"input overflow");
	}
};

inline void throw_input_overflow_error()
{
#ifdef __cpp_exceptions
	throw input_overflow_error();
#else
	fast_terminate();
#endif
}

}