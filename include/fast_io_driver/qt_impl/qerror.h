#pragma once

namespace fast_io
{
/*
To do:
*/
class qt_error:public std::exception
{
public:
	QString qstr;
	qt_error(QString str):qstr(std::move(str)){}
#if 0
	void report(error_reporter& reporter) const override
	{
		print_freestanding(reporter,fast_io::manipulators::cdcvt(qstr));
	}
#endif
};

inline void throw_qt_error(QString qstr)
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw qt_error(std::move(qstr));
#endif
#else
	fast_terminate();
#endif
}



}