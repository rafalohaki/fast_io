#pragma once

namespace fast_io
{

class c_locale_thread_local_guard
{
#if defined(__WINNT__) || defined(_MSC_VER)
	int old_status{};
#else
	c_locale_observer old_locale_value;
#endif
public:
	c_locale_thread_local_guard(c_locale_observer clob)
#if defined(__WINNT__) || defined(_MSC_VER)
	:old_status(_configthreadlocale(0))
	{
		if(old_status==-1)
			throw_posix_error();
		_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
	}
#else
	:old_locale_value(uselocale(clob.native_handle()))
	{
		if(!old_locale_value)
			throw_posix_error();
	}
#endif
	c_locale_thread_local_guard(c_locale_thread_local_guard const&)=delete;
	c_locale_thread_local_guard& operator=(c_locale_thread_local_guard const&)=delete;
	~c_locale_thread_local_guard()
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		_configthreadlocale(old_status);
#else
		uselocale(old_locale_value.native_handle());
#endif
	}
};

}