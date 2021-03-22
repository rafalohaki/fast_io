#pragma once

namespace fast_io::win32
{

__declspec(dllimport) extern std::uint32_t __stdcall GetLastError(void) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetLastError@0")
#else
asm("_GetLastError@0")
#endif
#else
asm("GetLastError")
#endif
#endif
;

__declspec(dllimport) extern void * __stdcall LoadLibraryW(wchar_t const*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("LoadLibraryW@4")
#else
asm("_LoadLibraryW@4")
#endif
#else
asm("LoadLibraryW")
#endif
#endif
;

// Used to retrieve a locale-specific message string for some error code
__declspec(dllimport) extern std::uint32_t __stdcall FormatMessageW(std::uint32_t, void const*, std::uint32_t,std::uint32_t, wchar_t*, std::uint32_t, void /*va_list*/ *) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("FormatMessageW@28")
#else
asm("_FormatMessageW@28")
#endif
#else
asm("FormatMessageW")
#endif
#endif
;

__declspec(dllimport) extern void* __stdcall CreateFileMappingW(void*,security_attributes*,std::uint32_t,std::uint32_t,std::uint32_t,wchar_t const*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("CreateFileMappingW@24")
#else
asm("_CreateFileMappingW@24")
#endif
#else
asm("CreateFileMappingW")
#endif
#endif
;

__declspec(dllimport) extern void* __stdcall MapViewOfFile(void*,std::uint32_t,std::uint32_t,std::uint32_t,std::size_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("MapViewOfFile@20")
#else
asm("_MapViewOfFile@20")
#endif
#else
asm("MapViewOfFile")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall SetEndOfFile(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("SetEndOfFile@4")
#else
asm("_SetEndOfFile@4")
#endif
#else
asm("SetEndOfFile")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall UnmapViewOfFile(void const*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("UnmapViewOfFile@4")
#else
asm("_UnmapViewOfFile@4")
#endif
#else
asm("UnmapViewOfFile")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall WriteFile(void*,void const*,std::uint32_t,std::uint32_t*,overlapped*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("WriteFile@20")
#else
asm("_WriteFile@20")
#endif
#else
asm("WriteFile")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall ReadFile(void*,void const*,std::uint32_t,std::uint32_t*,overlapped*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
asm("ReadFile@20")
#else
asm("ReadFile")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall SetFilePointerEx(void*,std::int64_t,std::int64_t*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("SetFilePointerEx@20")
#else
asm("_SetFilePointerEx@20")
#endif
#else
asm("SetFilePointerEx")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall DuplicateHandle(void*,void*,void*,void**,std::uint32_t,int,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("DuplicateHandle@28")
#else
asm("_DuplicateHandle@28")
#endif
#else
asm("DuplicateHandle")
#endif
#endif
;

__declspec(dllimport) extern void* __stdcall GetStdHandle(std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetStdHandle@4")
#else
asm("_GetStdHandle@4")
#endif
#else
asm("GetStdHandle")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall CreatePipe(void**,void**,security_attributes*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("CreatePipe@16")
#else
asm("_CreatePipe@16")
#endif
#else
asm("CreatePipe")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall FreeLibrary(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("FreeLibrary@4")
#else
asm("_FreeLibrary@4")
#endif
#else
asm("FreeLibrary")
#endif
#endif
;

using farproc = intptr_t(__stdcall*)() noexcept;

__declspec(dllimport) extern farproc __stdcall GetProcAddress(void*,char const*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetProcAddress@8")
#else
asm("_GetProcAddress@8")
#endif
#else
asm("GetProcAddress")
#endif
#endif
;

__declspec(dllimport) extern void* __stdcall GetModuleHandleW(wchar_t const*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetModuleHandleW@4")
#else
asm("_GetModuleHandleW@4")
#endif
#else
asm("GetModuleHandleW")
#endif
#endif
;

__declspec(dllimport) extern void* __stdcall LoadLibraryA(char const*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("LoadLibraryA@4")
#else
asm("_LoadLibraryA@4")
#endif
#else
asm("LoadLibraryA")
#endif
#endif
;

__declspec(dllimport) extern std::uint32_t __stdcall WaitForSingleObject(void*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("WaitForSingleObject@8")
#else
asm("_WaitForSingleObject@8")
#endif
#else
asm("WaitForSingleObject")
#endif
#endif
;

__declspec(dllimport) extern std::uint32_t __stdcall CancelIo(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("CancelIo@4")
#else
asm("_CancelIo@4")
#endif
#else
asm("CancelIo")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall GetFileInformationByHandle(void* __restrict,by_handle_file_information* __restrict) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetFileInformationByHandle@8")
#else
asm("_GetFileInformationByHandle@8")
#endif
#else
asm("GetFileInformationByHandle")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall GetUserDefaultLocaleName(wchar_t*,int) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetUserDefaultLocaleName@8")
#else
asm("_GetUserDefaultLocaleName@8")
#endif
#else
asm("GetUserDefaultLocaleName")
#endif
#endif
;

__declspec(dllimport) extern void __stdcall GetSystemTimePreciseAsFileTime(filetime*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetSystemTimePreciseAsFileTime@4")
#else
asm("_GetSystemTimePreciseAsFileTime@4")
#endif
#else
asm("GetSystemTimePreciseAsFileTime")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall QueryUnbiasedInterruptTime(std::uint64_t* unbiasedtime) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("QueryUnbiasedInterruptTime@4")
#else
asm("_QueryUnbiasedInterruptTime@4")
#endif
#else
asm("QueryUnbiasedInterruptTime")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall GetProcessTimes(void*,filetime*,filetime*,filetime*,filetime*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetProcessTimes@20")
#else
asm("_GetProcessTimes@20")
#endif
#else
asm("GetProcessTimes")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall GetThreadTimes(void*,filetime*,filetime*,filetime*,filetime*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetThreadTimes@20")
#else
asm("_GetThreadTimes@20")
#endif
#else
asm("GetThreadTimes")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall GetHandleInformation(void*,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetHandleInformation@8")
#else
asm("_GetHandleInformation@8")
#endif
#else
asm("GetHandleInformation")
#endif
#endif
;
__declspec(dllimport) extern int __stdcall SetHandleInformation(void*,std::uint32_t,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("SetHandleInformation@12")
#else
asm("_SetHandleInformation@12")
#endif
#else
asm("SetHandleInformation")
#endif
#endif
;

__declspec(dllimport) extern std::uint32_t __stdcall GetTempPathW(std::uint32_t,wchar_t* buffer) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetTempPathW@8")
#else
asm("_GetTempPathW@8")
#endif
#else
asm("GetTempPathW")
#endif
#endif
;

__declspec(dllimport) extern void* __stdcall CreateFileW(wchar_t const*,std::uint32_t,std::uint32_t,security_attributes*,std::uint32_t,std::uint32_t,void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("CreateFileW@28")
#else
asm("_CreateFileW@28")
#endif
#else
asm("CreateFileW")
#endif
#endif
;

__declspec(dllimport) extern void* __stdcall CreateIoCompletionPort(void*,void*,std::uintptr_t,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("CreateIoCompletionPort@16")
#else
asm("_CreateIoCompletionPort@16")
#endif
#else
asm("CreateIoCompletionPort")
#endif
#endif
;
__declspec(dllimport) extern int __stdcall SystemFunction036(void*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("SystemFunction036@8")
#else
asm("_SystemFunction036@8")
#endif
#else
asm("SystemFunction036")
#endif
#endif
;
__declspec(dllimport) extern int __stdcall CloseHandle(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("CloseHandle@4")
#else
asm("_CloseHandle@4")
#endif
#else
asm("CloseHandle")
#endif
#endif
;
__declspec(dllimport) extern int __stdcall LockFileEx(void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,overlapped*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("LockFileEx@24")
#else
asm("_LockFileEx@24")
#endif
#else
asm("LockFileEx")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall UnlockFileEx(void*,std::uint32_t,std::uint32_t,std::uint32_t,overlapped*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("UnlockFileEx@20")
#else
asm("_UnlockFileEx@20")
#endif
#else
asm("UnlockFileEx")
#endif
#endif
;

__declspec(dllimport) extern int __stdcall DeviceIoControl(void*,std::uint32_t,void*,std::uint32_t,void*,std::uint32_t,void*,overlapped*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("DeviceIoControl@32")
#else
asm("_DeviceIoControl@32")
#endif
#else
asm("DeviceIoControl")
#endif
#endif
;

__declspec(dllimport) extern std::uint32_t __stdcall GetFileType(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetFileType@4")
#else
asm("_GetFileType@4")
#endif
#else
asm("GetFileType")
#endif
#endif
;


__declspec(dllimport) extern std::uint32_t __stdcall GetACP() noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("GetACP@0")
#else
asm("_GetACP@0")
#endif
#else
asm("GetACP")
#endif
#endif
;
#if !defined(__CYGWIN__)
__declspec(dllimport) extern errno_t __cdecl getenv_s(std::size_t *,char* buffer,std::size_t ,char const *) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("getenv_s")
#else
asm("_getenv_s")
#endif
#else
asm("getenv_s")
#endif
#endif
;
#endif
}