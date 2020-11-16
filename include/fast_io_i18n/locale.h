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
		if(locale_name.size()==1&&locale_name.front()=='C')
			loc_name="POSIX";
		else if(locale_name.empty())
			loc_name="fast_io";
		else
		{
			std::size_t size{locale_name.size()+sizeof(".dll")};
			arrptr.reset(new char[size]);
			memcpy(arrptr.get(),locale_name.data(),locale_name.size());
			memcpy(arrptr.get()+locale_name.size(),".dll",sizeof(".dll"));
			loc_name=arrptr.get();
		}
		std::unique_ptr<void,details::close_dll> ptr{fast_io::win32::LoadLibraryA(loc_name)};
		if(!ptr)
			throw_win32_error();
		auto func(bit_cast<bool __stdcall(*)(lc_locale*) noexcept >(fast_io::win32::GetProcAddress(ptr.get(),"export_locale_data")));
		if(func==nullptr)
			throw_win32_error();
		if(!func(std::addressof(loc)))
			throw_win32_error();
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