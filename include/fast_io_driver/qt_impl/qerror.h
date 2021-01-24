#pragma once

namespace fast_io
{
/*
To do:
*/
class qt_error:public fast_io_error
{
public:
	QString qstr;
	qt_error(QString str):qstr(std::move(str)){}

	void report(error_reporter& reporter) const override
	{
		print_freestanding(reporter,fast_io::manipulators::cdcvt(qstr));
	}
};

inline void throw_qt_error(QString qstr)
{
#ifdef __cpp_exceptions
	throw qt_error(std::move(qstr));
#else
	fast_terminate();
#endif
}

}