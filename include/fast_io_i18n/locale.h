#pragma once

#include"lc.h"
#include"lc_print.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif

namespace fast_io
{

namespace details
{

struct close_dll
{
	void operator()(void* handle) const noexcept
	{
		if(handle)[[likely]]
#ifdef _WIN32
			fast_io::win32::FreeLibrary(handle);
#else
			dlclose(handle);	
#endif
	}
};

}

#ifndef _WIN32

class posix_dl_error:public fast_io_error
{
public:
	virtual	void report(error_reporter& err) const override
	{
		print(err,fast_io::chvw(dlerror()));
	}
};

[[noreturn]] inline void throw_posix_dl_error()
{
#ifdef __cpp_exceptions
	throw posix_dl_error();
#else
	fast_terminate();
#endif
}

#endif

class i18n_locale
{
public:
	lc_locale loc{};
	void* dll_handle{};
	constexpr i18n_locale() noexcept=default;
	i18n_locale(std::string_view locale_name)
	{
		if(locale_name.size()>6&&locale_name.substr(locale_name.size()-6,6)==".UTF-8")
			locale_name=locale_name.substr(0,locale_name.size()-6);
		std::unique_ptr<char[]> arrptr;
		char const* loc_name{};
#ifdef _WIN32
		if(locale_name=="C"||locale_name=="POSIX")
			loc_name="fast_io_i18n_data\\locale\\POSIX.dll";
		else if(locale_name.empty())
			loc_name="fast_io_i18n_data\\locale_local.dll";
		else
		{
			constexpr std::size_t sz{sizeof("fast_io_i18n_data\\locale\\")-1};
			std::size_t size{sz+locale_name.size()+sizeof(".dll")};
			arrptr.reset(new char[size]);
			memcpy(arrptr.get(),"fast_io_i18n_data\\locale\\",sz);
			memcpy(arrptr.get()+sz,locale_name.data(),locale_name.size());
			memcpy(arrptr.get()+sz+locale_name.size(),".dll",sizeof(".dll"));
			loc_name=arrptr.get();
		}
#else
		if(locale_name=="C"||locale_name=="POSIX")
			loc_name="/usr/local/lib/fast_io_i18n_data/locale/POSIX.so";
		else
		{
			if(locale_name.empty())
			{
				loc_name=
#if defined(_GNU_SOURCE)
					secure_getenv("FAST_IO_I18N_LOCALE");
#else
					getenv("FAST_IO_I18N_LOCALE");
#endif
				if(loc_name==nullptr)
				{
					loc_name=
#if defined(_GNU_SOURCE)
					secure_getenv("LC_ALL");
#else
					getenv("LC_ALL");
#endif
					if(loc_name==nullptr)
					{
						loc_name=
#if defined(_GNU_SOURCE)
						secure_getenv("LANG");
#else
						getenv("LANG");
#endif
						if(loc_name==nullptr)
						{
							loc_name="/usr/local/lib/fast_io_i18n_data/locale/POSIX.so";
							goto loading;
						}
					}
				}
				locale_name=loc_name;
				if(locale_name.size()>6&&locale_name.substr(locale_name.size()-6,6)==".UTF-8")
					locale_name=locale_name.substr(0,locale_name.size()-6);
				if(locale_name=="C"||locale_name=="POSIX")
				{
					loc_name="/usr/local/lib/fast_io_i18n_data/locale/POSIX.so";
					goto loading;
				}
			}
			{
				constexpr std::size_t sz{sizeof("/usr/local/lib/fast_io_i18n_data/locale/")-1};
				std::size_t size{sz+locale_name.size()+sizeof(".so")};
				arrptr.reset(new char[size]);
				memcpy(arrptr.get(),"/usr/local/lib/fast_io_i18n_data/locale/",sz);
				memcpy(arrptr.get()+sz,locale_name.data(),locale_name.size());
				memcpy(arrptr.get()+sz+locale_name.size(),".so",sizeof(".so"));
				loc_name=arrptr.get();
			}
			loading:;
		}
#endif
		std::unique_ptr<void,details::close_dll> ptr{
#ifdef _WIN32
			fast_io::win32::LoadLibraryA(loc_name)
#else
			dlopen(loc_name,RTLD_GLOBAL | RTLD_NOW)
#endif
};
		if(!ptr)
#ifdef _WIN32
			throw_win32_error();
#else
			throw_posix_dl_error();
#endif

#ifdef _WIN32
		auto func(bit_cast<bool __stdcall(*)(lc_locale*) noexcept >(fast_io::win32::GetProcAddress(ptr.get(),"export_locale_data")));
#else
		auto func(bit_cast<void (*)(lc_locale*) noexcept >(dlsym(ptr.get(),"export_locale_data")));
#endif

		if(func==nullptr)
#ifdef _WIN32
			throw_win32_error();
#else
			throw_posix_dl_error();
#endif
#ifdef _WIN32
		if(!func(std::addressof(loc)))
			throw_win32_error();
#else
		func(std::addressof(loc));
#endif
		dll_handle=ptr.release();
	}
	constexpr i18n_locale(i18n_locale const&)=delete;
	constexpr i18n_locale(i18n_locale&& other) noexcept : loc(std::move(other.loc)),dll_handle(other.dll_handle)
	{
		dll_handle=nullptr;
	}
	explicit constexpr operator bool() noexcept
	{
		return dll_handle;
	}
	void close() noexcept
	{
		if(dll_handle)[[likely]]
#ifdef _WIN32
			fast_io::win32::FreeLibrary(dll_handle);
#else
			dlclose(dll_handle);
#endif
	}
	i18n_locale& operator=(i18n_locale&& other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		close();
		loc=std::move(other.loc);
		dll_handle=other.dll_handle;
		other.dll_handle=nullptr;
		return *this;
	}
	~i18n_locale()
	{
		close();
	}
};

}