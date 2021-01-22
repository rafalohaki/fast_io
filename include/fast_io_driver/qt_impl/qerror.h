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

	void report(error_reporter& reporter) override
	{

	}
};

inline void throw_qt_error(QString qstr)
{
#if __cpp_exceptions

#else

#endif
}

}