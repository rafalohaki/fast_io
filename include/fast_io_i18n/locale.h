#pragma once

#include"lc.h"

namespace fast_io
{

namespace details
{

struct close_dll
{
	void operator()(void* handle) const noexcept
	{
		if(handle)[[likely]]
			fast_io::win32::FreeLibrary(handle);
	}
};

}

class i18n_locale
{
public:
	lc_locale loc{};
	void* dll_handle{};
	constexpr i18n_locale() noexcept=default;
	i18n_locale(cstring_view locale_name)
	{
		std::unique_ptr<char[]> arrptr;
		char const* loc_name{};
#ifdef _WIN32
		if(locale_name=="C"||locale_name=="POSIX")
			loc_name="fast_io_i18n_data\\locale\\POSIX.dll";
		else if(locale_name.empty())
			loc_name="fast_io_i18n_data\\locale\\fast_io.dll";
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
			loc_name="fast_io_i18n_data/locale/POSIX.so";
		else
		{
			if(locale_name.empty())
			{
				loc_name=
#if defined(_GNU_SOURCE)
				secure_getenv("LC_ALL");
#else
				getenv("LC_ALL");
#endif
				if(loc_name==nullptr)
				{
					loc_name="fast_io_i18n_data/locale/POSIX.so";
					goto loading;
				}
				locale_name=cstring_view(loc_name);
			}
			{
				constexpr std::size_t sz{sizeof("fast_io_i18n_data/locale/")-1};
				std::size_t size{sz+locale_name.size()+sizeof(".so")};
				arrptr.reset(new char[size]);
				memcpy(arrptr.get(),"fast_io_i18n_data/locale/",sz);
				memcpy(arrptr.get()+sz,locale_name.data(),locale_name.size());
				memcpy(arrptr.get()+sz+locale_name.size(),".so",sizeof(".so"));
				loc_name=arrptr.get();
			}
			loading:;
		}
#endif
		std::unique_ptr<void,details::close_dll> ptr{fast_io::win32::LoadLibraryA(loc_name)};
		if(!ptr)
#ifdef _WIN32
			throw_win32_error();
#else
			throw_posix_error();
#endif

#ifdef _WIN32
		auto func(bit_cast<bool __stdcall(*)(lc_locale*) noexcept >(fast_io::win32::GetProcAddress(ptr.get(),"export_locale_data")));
#else

#endif

		if(func==nullptr)
#ifdef _WIN32
			throw_win32_error();
#else
			throw_posix_error();
#endif
		if(!func(std::addressof(loc)))
#ifdef _WIN32
			throw_win32_error();
#else
			throw_posix_error();
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
			fast_io::win32::FreeLibrary(dll_handle);
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