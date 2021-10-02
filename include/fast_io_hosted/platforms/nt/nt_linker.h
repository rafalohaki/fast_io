﻿#pragma once

#if defined(_MSC_VER)
#pragma comment(lib,"ntdll.lib")
#endif

namespace fast_io::win32::nt
{

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall rtl_nt_status_to_dos_error(std::uint32_t status) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlNtStatusToDosError@4")
#else
asm("_RtlNtStatusToDosError@4")
#endif
#else
asm("RtlNtStatusToDosError")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtClose(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtClose@4")
#else
asm("_NtClose@4")
#endif
#else
asm("NtClose")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwClose(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwClose@4")
#else
asm("_ZwClose@4")
#endif
#else
asm("ZwClose")
#endif
#endif
;

template<bool zw>
inline std::uint32_t nt_close(void* handle) noexcept
{
	if constexpr(zw)
		return ZwClose(handle);
	else
		return NtClose(handle);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtCreateFile(void**,std::uint32_t,object_attributes*,io_status_block*,std::int64_t*,
			std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,void*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtCreateFile@44")
#else
asm("_NtCreateFile@44")
#endif
#else
asm("NtCreateFile")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwCreateFile(void**,std::uint32_t,object_attributes*,io_status_block*,std::int64_t*,
			std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,void*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwCreateFile@44")
#else
asm("_ZwCreateFile@44")
#endif
#else
asm("ZwCreateFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==11)
inline std::uint32_t nt_create_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwCreateFile(args...);
	else
		return NtCreateFile(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtCreateSection(void** __restrict,std::uint32_t,object_attributes* __restrict,
		std::uint64_t*,std::uint32_t,std::uint32_t,void* __restrict) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtCreateSection@28")
#else
asm("_NtCreateSection@28")
#endif
#else
asm("NtCreateSection")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwCreateSection(void** __restrict,std::uint32_t,object_attributes* __restrict,
		std::uint64_t*,std::uint32_t,std::uint32_t,void* __restrict) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwCreateSection@28")
#else
asm("_ZwCreateSection@28")
#endif
#else
asm("ZwCreateSection")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==7)
inline std::uint32_t nt_create_section(Args... args) noexcept
{
	if constexpr(zw)
		return ZwCreateSection(args...);
	else
		return NtCreateSection(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtQueryInformationProcess(void* __restrict,
		process_information_class,process_basic_information*,
		std::uint32_t,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtQueryInformationProcess@20")
#else
asm("_NtQueryInformationProcess@20")
#endif
#else
asm("NtQueryInformationProcess")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwQueryInformationProcess(void* __restrict,
		process_information_class,process_basic_information*,
		std::uint32_t,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwQueryInformationProcess@20")
#else
asm("_ZwQueryInformationProcess@20")
#endif
#else
asm("ZwQueryInformationProcess")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline std::uint32_t nt_query_information_process(Args... args) noexcept
{
	if constexpr(zw)
		return ZwQueryInformationProcess(args...);
	else
		return NtQueryInformationProcess(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtWriteFile(void*,void*,pio_apc_routine,void*,io_status_block*,
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtWriteFile@36")
#else
asm("_NtWriteFile@36")
#endif
#else
asm("NtWriteFile")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwWriteFile(void*,void*,pio_apc_routine,void*,io_status_block*,
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwWriteFile@36")
#else
asm("_ZwWriteFile@36")
#endif
#else
asm("ZwWriteFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==9)
inline std::uint32_t nt_write_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwWriteFile(args...);
	else
		return NtWriteFile(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtReadFile(void*,void*,pio_apc_routine,void*,io_status_block*,
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtReadFile@36")
#else
asm("_NtReadFile@36")
#endif
#else
asm("NtReadFile")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwReadFile(void*,void*,pio_apc_routine,void*,io_status_block*,
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwReadFile@36")
#else
asm("_ZwReadFile@36")
#endif
#else
asm("ZwReadFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==9)
inline std::uint32_t nt_read_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwReadFile(args...);
	else
		return NtReadFile(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtQueryDirectoryFile(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,file_information_class,int,unicode_string*,int) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtQueryDirectoryFile@44")
#else
asm("_NtQueryDirectoryFile@44")
#endif
#else
asm("NtQueryDirectoryFile")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwQueryDirectoryFile(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,file_information_class,int,unicode_string*,int) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwQueryDirectoryFile@44")
#else
asm("_ZwQueryDirectoryFile@44")
#endif
#else
asm("ZwQueryDirectoryFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==11)
inline std::uint32_t nt_query_directory_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwQueryDirectoryFile(args...);
	else
		return NtQueryDirectoryFile(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtQuerySection(void*,section_information_class,void*,std::size_t,std::size_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtQuerySection@20")
#else
asm("_NtQuerySection@20")
#endif
#else
asm("NtQuerySection")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwQuerySection(void*,section_information_class,void*,std::size_t,std::size_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwQuerySection@20")
#else
asm("_ZwQuerySection@20")
#endif
#else
asm("ZwQuerySection")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline std::uint32_t nt_query_section(Args... args) noexcept
{
	if constexpr(zw)
		return ZwQuerySection(args...);
	else
		return NtQuerySection(args...);
}


#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtQueryInformationFile(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtQueryInformationFile@20")
#else
asm("_NtQueryInformationFile@20")
#endif
#else
asm("NtQueryInformationFile")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwQueryInformationFile(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwQueryInformationFile@20")
#else
asm("_ZwQueryInformationFile@20")
#endif
#else
asm("ZwQueryInformationFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline std::uint32_t nt_query_information_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwQueryInformationFile(args...);
	else
		return NtQueryInformationFile(args...);
}


#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtSetInformationFile(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtSetInformationFile@28")
#else
asm("_NtSetInformationFile@28")
#endif
#else
asm("NtSetInformationFile")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwSetInformationFile(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwSetInformationFile@28")
#else
asm("_ZwSetInformationFile@28")
#endif
#else
asm("ZwSetInformationFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline std::uint32_t nt_set_information_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwSetInformationFile(args...);
	else
		return NtSetInformationFile(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtDuplicateObject(void*,void*,void*,void**,std::uint32_t,std::uint32_t,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtDuplicateObject@28")
#else
asm("_NtDuplicateObject@28")
#endif
#else
asm("NtDuplicateObject")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwDuplicateObject(void*,void*,void*,void**,std::uint32_t,std::uint32_t,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwDuplicateObject@28")
#else
asm("_ZwDuplicateObject@28")
#endif
#else
asm("ZwDuplicateObject")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==7)
inline std::uint32_t nt_duplicate_object(Args... args) noexcept
{
	if constexpr(zw)
		return ZwDuplicateObject(args...);
	else
		return NtDuplicateObject(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtWaitForSingleObject(void*,int,std::uint64_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtWaitForSingleObject@12")
#else
asm("_NtWaitForSingleObject@12")
#endif
#else
asm("NtWaitForSingleObject")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwWaitForSingleObject(void*,int,std::uint64_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwWaitForSingleObject@12")
#else
asm("_ZwWaitForSingleObject@12")
#endif
#else
asm("ZwWaitForSingleObject")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==3)
inline std::uint32_t nt_wait_for_single_object(Args... args) noexcept
{
	if constexpr(zw)
		return ZwWaitForSingleObject(args...);
	else
		return NtWaitForSingleObject(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtSetSystemTime(std::uint64_t*,std::uint64_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtSetSystemTime@8")
#else
asm("_NtSetSystemTime@8")
#endif
#else
asm("NtSetSystemTime")
#endif
#endif
;
#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwSetSystemTime(std::uint64_t*,std::uint64_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwSetSystemTime@8")
#else
asm("_ZwSetSystemTime@8")
#endif
#else
asm("ZwSetSystemTime")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==2)
inline std::uint32_t nt_set_system_time(Args... args) noexcept
{
	if constexpr(zw)
		return ZwSetSystemTime(args...);
	else
		return NtSetSystemTime(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
std::uint32_t __stdcall NtCreateProcess(void**,std::uint32_t,object_attributes*,void*,std::uint32_t,void*,void*,void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtCreateProcess@32")
#else
asm("_NtCreateProcess@32")
#endif
#else
asm("NtCreateProcess")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
std::uint32_t __stdcall ZwCreateProcess(void**,std::uint32_t,object_attributes*,void*,std::uint32_t,void*,void*,void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwCreateProcess@32")
#else
asm("_ZwCreateProcess@32")
#endif
#else
asm("ZwCreateProcess")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==8)
inline std::uint32_t nt_create_process(Args... args) noexcept
{
	if constexpr(zw)
		return NtCreateProcess(args...);
	else
		return ZwCreateProcess(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern char unsigned __stdcall rtl_dos_path_name_to_nt_path_name_u(wchar_t const*,unicode_string*,wchar_t const**,rtl_relative_name_u*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlDosPathNameToNtPathName_U@16")
#else
asm("_RtlDosPathNameToNtPathName_U@16")
#endif
#else
asm("RtlDosPathNameToNtPathName_U")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall rtl_dos_path_name_to_nt_path_name_u_with_status(wchar_t const*,unicode_string*,wchar_t const**,rtl_relative_name_u*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlDosPathNameToNtPathName_U_WithStatus@16")
#else
asm("_RtlDosPathNameToNtPathName_U_WithStatus@16")
#endif
#else
asm("RtlDosPathNameToNtPathName_U_WithStatus")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern void __stdcall rtl_free_unicode_string(unicode_string*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlFreeUnicodeString@4")
#else
asm("_RtlFreeUnicodeString@4")
#endif
#else
asm("RtlFreeUnicodeString")
#endif
#endif
;


#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern void __stdcall RtlInitializeCriticalSection(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlInitializeCriticalSection@4")
#else
asm("_RtlInitializeCriticalSection@4")
#endif
#else
asm("RtlInitializeCriticalSection")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern void __stdcall RtlEnterCriticalSection(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlEnterCriticalSection@4")
#else
asm("_RtlEnterCriticalSection@4")
#endif
#else
asm("RtlEnterCriticalSection")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern int __stdcall RtlTryEnterCriticalSection(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlTryEnterCriticalSection@4")
#else
asm("_RtlTryEnterCriticalSection@4")
#endif
#else
asm("RtlTryEnterCriticalSection")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern void __stdcall RtlLeaveCriticalSection(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlLeaveCriticalSection@4")
#else
asm("_RtlLeaveCriticalSection@4")
#endif
#else
asm("RtlLeaveCriticalSection")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern void __stdcall RtlDeleteCriticalSection(void*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlDeleteCriticalSection@4")
#else
asm("_RtlDeleteCriticalSection@4")
#endif
#else
asm("RtlDeleteCriticalSection")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall RtlCreateUserThread(void*,void*,int,std::uint32_t,std::size_t,std::size_t,void*,void*,void**,client_id*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("RtlCreateUserThread@40")
#else
asm("_RtlCreateUserThread@40")
#endif
#else
asm("RtlCreateUserThread")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtResumeThread(void*,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtResumeThread@8")
#else
asm("_NtResumeThread@8")
#endif
#else
asm("NtResumeThread")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwResumeThread(void*,std::uint32_t*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwResumeThread@8")
#else
asm("_ZwResumeThread@8")
#endif
#else
asm("ZwResumeThread")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==2)
inline std::uint32_t nt_resume_thread(Args... args) noexcept
{
	if constexpr(zw)
		return ZwResumeThread(args...);
	else
		return NtResumeThread(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtLockFile(void*,void*,pio_apc_routine,void*,io_status_block*,std::int64_t*,std::int64_t*,std::uint32_t,char unsigned,char unsigned) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtLockFile@40")
#else
asm("_NtLockFile@40")
#endif
#else
asm("NtLockFile")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwLockFile(void*,void*,pio_apc_routine,void*,io_status_block*,std::int64_t*,std::int64_t*,std::uint32_t,char unsigned,char unsigned) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwLockFile@40")
#else
asm("_ZwLockFile@40")
#endif
#else
asm("ZwLockFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==10)
inline std::uint32_t nt_lock_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwLockFile(args...);
	else
		return NtLockFile(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtUnlockFile(void*,io_status_block*,std::int64_t*,std::int64_t*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtUnlockFile@20")
#else
asm("_NtUnlockFile@20")
#endif
#else
asm("NtUnlockFile")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwUnlockFile(void*,io_status_block*,std::int64_t*,std::int64_t*,std::uint32_t) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwUnlockFile@20")
#else
asm("_ZwUnlockFile@20")
#endif
#else
asm("ZwUnlockFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline std::uint32_t nt_unlock_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwUnlockFile(args...);
	else
		return NtUnlockFile(args...);
}

/*
Available starting with Windows Vista???
https://hfiref0x.github.io/syscalls.html
Actually this shows that it is available since the start of NT
*/

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtFlushBuffersFile(void*,io_status_block*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtFlushBuffersFile@8")
#else
asm("_NtFlushBuffersFile@8")
#endif
#else
asm("NtFlushBuffersFile")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwFlushBuffersFile(void*,io_status_block*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwFlushBuffersFile@8")
#else
asm("_ZwFlushBuffersFile@8")
#endif
#else
asm("ZwFlushBuffersFile")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==2)
inline std::uint32_t nt_flush_buffers_file(Args... args) noexcept
{
	if constexpr(zw)
		return ZwFlushBuffersFile(args...);
	else
		return NtFlushBuffersFile(args...);
}

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall NtFlushBuffersFileEx(void*,std::uint32_t,void*,std::uint32_t,io_status_block*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("NtFlushBuffersFileEx@20")
#else
asm("_NtFlushBuffersFileEx@20")
#endif
#else
asm("NtFlushBuffersFileEx")
#endif
#endif
;

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern std::uint32_t __stdcall ZwFlushBuffersFileEx(void*,std::uint32_t,void*,std::uint32_t,io_status_block*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("ZwFlushBuffersFileEx@20")
#else
asm("_ZwFlushBuffersFileEx@20")
#endif
#else
asm("ZwFlushBuffersFileEx")
#endif
#endif
;

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline std::uint32_t nt_flush_buffers_file_ex(Args... args) noexcept
{
	if constexpr(zw)
		return ZwFlushBuffersFileEx(args...);
	else
		return NtFlushBuffersFileEx(args...);
}

}
