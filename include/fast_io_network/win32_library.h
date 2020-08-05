#pragma once
#include<ws2tcpip.h>
#include"mswsock.h"

#undef interface			//what a joke. Who did this?
#undef min			//what a joke. Who did this?
#undef max			//what a joke. Who did this?

namespace fast_io::details
{

class win32_library
{
	HMODULE plib;
	void closeimpl()
	{
		if(plib)
			::FreeLibrary(plib);
	}
public:
	win32_library(wchar_t const* name):plib(::LoadLibraryW(name))
	{
		if(plib==nullptr)
			throw_win32_error();
	}
	win32_library(win32_library const&)=delete;
	win32_library& operator=(win32_library const&)=delete;
	win32_library(win32_library&& other) noexcept:plib(other.plib)
	{
		other.plib=nullptr;
	}
	win32_library& operator=(win32_library&& other) noexcept
	{
		if(other.plib!=plib)
		{
			closeimpl();
			plib=other.plib;
			other.plib=nullptr;
		}
		return *this;
	}
	auto get() const
	{
		return plib;
	}
	~win32_library()
	{
		closeimpl();
	}
};
}