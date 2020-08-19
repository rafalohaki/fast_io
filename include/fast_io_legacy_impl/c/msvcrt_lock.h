#pragma once

namespace fast_io::win32
{

/*
Referenced from ReactOS
https://doxygen.reactos.org/d2/d1b/sdk_2lib_2crt_2stdio_2file_8c_source.html
Referenced from MinGW-w64 CRT
https://github.com/Alexpux/mingw-w64/blob/d0d7f784833bbb0b2d279310ddc6afb52fe47a46/mingw-w64-crt/stdio/mingw_lock.c#L36
*/

extern "C" void __stdcall EnterCriticalSection(void*) noexcept;
extern "C" void __stdcall LeaveCriticalSection(void*) noexcept;
extern "C" void __cdecl _lock(int) noexcept;
extern "C" void __cdecl _unlock(int) noexcept;


inline void my_msvcrt_lock_file(std::FILE* fp) noexcept
{
	auto iob{_iob};
	std::size_t entry(fp-iob);
	if(entry<_IOB_ENTRIES)
	{
		/*
		FILE lies in _iob[] so the lock lies in _locktable[]
https://github.com/Alexpux/mingw-w64/blob/d0d7f784833bbb0b2d279310ddc6afb52fe47a46/mingw-w64-crt/stdio/mingw_lock.c#L17
		#define _STREAM_LOCKS   16
		#define _IOLOCKED       0x8000
		*/
		_lock(static_cast<int>(16+entry));
		fp->_flag |= 0x8000;		//_IOLOCKED We set _IOLOCKED to indicate we locked the stream
	}
	else
		EnterCriticalSection(fp+1);
}

inline void my_msvcrt_unlock_file(std::FILE* fp) noexcept
{
	auto iob{_iob};
	std::size_t entry(fp-iob);
	if(entry<_IOB_ENTRIES)
	{
		fp->_flag &= ~0x8000;
		_unlock(static_cast<int>(16+entry));
	}
	else
		LeaveCriticalSection(fp+1);
}


}

