#pragma once

#include"lc.h"
#include"lc_print.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif

namespace fast_io
{


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

inline void load_lc_locale(void* dll_handle,lc_locale& loc)
{
#ifdef _WIN32
	auto func(bit_cast<void __stdcall(*)(lc_locale*) noexcept >(fast_io::win32::GetProcAddress(dll_handle,"export_locale_data")));
#else
	auto func(bit_cast<void (*)(lc_locale*) noexcept >(dlsym(dll_handle,"export_locale_data")));
#endif

	if(func==nullptr)
#ifdef _WIN32
		throw_win32_error();
#else
		throw_posix_dl_error();
#endif
	func(std::addressof(loc));
}

}

class l10n
{
public:
	lc_locale loc{};
	void* dll_handle{};
	constexpr l10n() noexcept=default;
	l10n(std::string_view locale_name)
	{
		std::unique_ptr<char[]> arrptr;
		char const* loc_name{};
//There is no way to get environmental variables without thread-safety issues

#ifdef _WIN32
		if(locale_name=="C"||locale_name=="POSIX")
			loc_name="fast_io_i18n_data\\locale\\POSIX.dll";
		else
		{
			if(locale_name.empty())
			{
				constexpr std::size_t prefix_length(sizeof(L"fast_io_i18n_data\\locale\\")/sizeof(wchar_t)-1);
				constexpr std::size_t total_length(prefix_length+256+sizeof(L".dll"));
				wchar_t buffer[total_length]=L"fast_io_i18n_data\\locale\\";
				wchar_t* start{buffer+prefix_length};
				std::size_t sz{static_cast<std::size_t>(win32::GetUserDefaultLocaleName(start,256))};
				if(!sz)
				{
					loc_name="fast_io_i18n_data\\locale\\POSIX.dll";
					goto loading;
				}
				for(std::size_t i{};i!=sz;++i)
					if(start[i]==L'-')
						start[i]=L'_';			
				memcpy(start+sz,L".dll",sizeof(L".dll"));
				std::unique_ptr<void,details::close_dll> ptr{win32::LoadLibraryW(buffer)};
				if(!ptr)
				{
					loc_name="fast_io_i18n_data\\locale\\POSIX.dll";
					goto loading;
				}
				details::load_lc_locale(ptr.get(),loc);
				dll_handle=ptr.release();
				return;
			}
			{
				constexpr std::size_t sz{sizeof("fast_io_i18n_data\\locale\\")-1};
				std::size_t size{sz+locale_name.size()+sizeof(".dll")};
				arrptr.reset(new char[size]);
				memcpy(arrptr.get(),"fast_io_i18n_data\\locale\\",sz);
				memcpy(arrptr.get()+sz,locale_name.data(),locale_name.size());
				memcpy(arrptr.get()+sz+locale_name.size(),".dll",sizeof(".dll"));
				loc_name=arrptr.get();
			}
			loading:;
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
				secure_getenv("LANG");
#else
				std::getenv("LANG");
#endif
				if(loc_name==nullptr)
				{
					loc_name="/usr/local/lib/fast_io_i18n_data/locale/POSIX.so";
					goto loading;
				}
				locale_name=loc_name;
				if(locale_name.size()>6&&locale_name.substr(locale_name.size()-6,6)==".UTF-8")
				{
					locale_name=locale_name.substr(0,locale_name.size()-6);
				}
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
		details::load_lc_locale(ptr.get(),loc);
		dll_handle=ptr.release();
	}
	constexpr l10n(l10n const&)=delete;
	constexpr l10n(l10n&& other) noexcept : loc(std::move(other.loc)),dll_handle(other.dll_handle)
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
	l10n& operator=(l10n&& other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		close();
		loc=std::move(other.loc);
		dll_handle=other.dll_handle;
		other.dll_handle=nullptr;
		return *this;
	}
	~l10n()
	{
		close();
	}
};

}