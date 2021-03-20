#pragma once

namespace fast_io::win32::nt
{

[[gnu::const]]
inline auto get_nt_module_handle() noexcept
{
/*
Guard against EBCDIC exec charset
*/
	using wchar_t_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= wchar_t const*;
	auto mod(GetModuleHandleW(reinterpret_cast<wchar_t_alias_const_ptr>(u"ntdll.dll")));
	if(mod==nullptr)[[unlikely]]
		fast_terminate();
	return mod;
}

[[gnu::const]]
inline auto ntdll_proc_addr(char8_t const* funname) noexcept
{
	auto proc_addr(GetProcAddress(get_nt_module_handle(),reinterpret_cast<char const*>(funname)));
	if(proc_addr==nullptr)[[unlikely]]
		fast_terminate();
	return proc_addr;
}

template<typename func>
[[gnu::const]]
inline func* get_nt_module_fptr(char8_t const* funname) noexcept
{
	return bit_cast<func*>(ntdll_proc_addr(funname));
}

/*
Guard against EBCDIC execution charset
*/

using rtl_nt_status_to_dos_error_prototype = std::uint32_t __stdcall(std::uint32_t status) noexcept;

inline rtl_nt_status_to_dos_error_prototype* const rtl_nt_status_to_dos_error{get_nt_module_fptr<rtl_nt_status_to_dos_error_prototype>(u8"RtlNtStatusToDosError")};

using nt_close_prototype = std::uint32_t __stdcall(void*) noexcept;

template<bool zw>
inline nt_close_prototype* const nt_close{get_nt_module_fptr<nt_close_prototype>(zw?u8"ZwClose":u8"NtClose")};

using nt_create_file_prototype = std::uint32_t __stdcall(void**,std::uint32_t,object_attributes*,io_status_block*,std::int64_t*,
			std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,void*,std::uint32_t) noexcept;

template<bool zw>
inline nt_create_file_prototype* const nt_create_file{get_nt_module_fptr<nt_create_file_prototype>(zw?u8"ZwCreateFile":u8"NtCreateFile")};

/*
Referenced From
https://github.com/Mattiwatti/BSOD10/blob/b43bc139e97fd7019315e8771fa809f58f7bd53e/src/ntdll.h

NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateSection(
	_Out_ PHANDLE (void**) SectionHandle,
	_In_ ACCESS_MASK(std::uint32_t) DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES(object_attributes*) ObjectAttributes,
	_In_opt_ PLARGE_INTEGER(std::uint64_t*) MaximumSize,
	_In_ ULONG(std::uint32_t) SectionPageProtection,
	_In_ ULONG(std::uint32_t) AllocationAttributes,
	_In_opt_ HANDLE(void*) FileHandle
	);
*/
using nt_create_section_protoype = std::uint32_t __stdcall(void** __restrict,std::uint32_t,object_attributes* __restrict,
		std::uint64_t*,std::uint32_t,std::uint32_t,void* __restrict) noexcept;
template<bool zw>
inline nt_create_section_protoype* const nt_create_section{get_nt_module_fptr<nt_create_section_protoype>(zw?u8"ZwCreateSection":u8"NtCreateSection")};

/*
__kernel_entry NTSTATUS NtQueryInformationProcess(
  HANDLE           ProcessHandle,
  PROCESSINFOCLASS ProcessInformationClass,
  PVOID            ProcessInformation,
  ULONG            ProcessInformationLength,
  PULONG           ReturnLength
);
*/
using nt_query_information_process_prototype = std::uint32_t __stdcall(void* __restrict,
		process_information_class,process_basic_information*,
		std::uint32_t,std::uint32_t*) noexcept;

template<bool zw>
inline nt_query_information_process_prototype* const nt_query_information_process{get_nt_module_fptr<nt_query_information_process_prototype>(zw?u8"ZwQueryInformationProcess":u8"NtQueryInformationProcess")};

/*
Referenced From
https://github.com/Mattiwatti/BSOD10/blob/b43bc139e97fd7019315e8771fa809f58f7bd53e/src/ntdll.h

NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateProcess(
	_Out_ PHANDLE(void**) ProcessHandle,
	_In_ ACCESS_MASK(std::uint32_t) DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES(object_attributes*) ObjectAttributes,
	_In_ HANDLE(void*) ParentProcess,
	_In_ BOOLEAN(int) InheritObjectTable,
	_In_opt_ HANDLE(void*) SectionHandle,
	_In_opt_ HANDLE(void*) DebugPort,
	_In_opt_ HANDLE(void*) ExceptionPort
	);
*/

using nt_create_process_prototype = std::uint32_t __stdcall(void**,std::uint32_t,object_attributes*,
		void*,std::uint32_t,void*,void*,void*) noexcept;

template<bool zw>
inline nt_create_process_prototype* const nt_create_process{get_nt_module_fptr<nt_create_process_prototype>(zw?u8"ZwCreateProcess":u8"NtCreateProcess")};

using nt_write_file_prototype = std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept;

template<bool zw>
inline nt_write_file_prototype* const nt_write_file{get_nt_module_fptr<nt_write_file_prototype>(zw?u8"ZwWriteFile":u8"NtWriteFile")};

using nt_read_file_prototype = std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept;

template<bool zw>
inline nt_read_file_prototype* const nt_read_file{get_nt_module_fptr<nt_read_file_prototype>(zw?u8"ZwReadFile":u8"NtReadFile")};


/*
__kernel_entry NTSYSCALLAPI NTSTATUS NtQueryDirectoryFile(
	HANDLE                 FileHandle,
	HANDLE                 Event,
	PIO_APC_ROUTINE        ApcRoutine,
	PVOID                  ApcContext,
	PIO_STATUS_BLOCK       IoStatusBlock,
	PVOID                  FileInformation,
	ULONG                  Length,
	FILE_INFORMATION_CLASS FileInformationClass,
	BOOLEAN                ReturnSingleEntry,
	PUNICODE_STRING        FileName,
	BOOLEAN                RestartScan
);
*/

using nt_query_directory_file_prototype = std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,file_information_class,int,unicode_string*,int) noexcept;

template<bool zw>
inline nt_query_directory_file_prototype* const nt_query_directory_file{get_nt_module_fptr<nt_query_directory_file_prototype>(zw?u8"ZwQueryDirectoryFile":u8"NtQueryDirectoryFile")};

using nt_query_section_prototype = std::uint32_t __stdcall(void*,section_information_class,void*,std::size_t,std::size_t*) noexcept;

template<bool zw>
inline nt_query_section_prototype* const nt_query_section{get_nt_module_fptr<nt_query_section_prototype>(zw?u8"ZwQuerySection":u8"NtQuerySection")};

using nt_query_information_file_prototype = std::uint32_t __stdcall(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept;

template<bool zw>
inline nt_query_information_file_prototype* const nt_query_information_file{get_nt_module_fptr<nt_query_information_file_prototype>(zw?u8"ZwQueryInformationFile":u8"NtQueryInformationFile")};

using nt_set_information_file_prototype = std::uint32_t __stdcall(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept;

template<bool zw>
inline nt_set_information_file_prototype* const nt_set_information_file{get_nt_module_fptr<nt_set_information_file_prototype>(zw?u8"ZwSetInformationFile":u8"NtSetInformationFile")};

using nt_duplicate_object_prototype = std::uint32_t __stdcall(void*,void*,void*,void**,std::uint32_t,std::uint32_t,std::uint32_t) noexcept;

template<bool zw>
inline nt_duplicate_object_prototype* const nt_duplicate_object{get_nt_module_fptr<nt_duplicate_object_prototype>(zw?u8"ZwDuplicateObject":u8"NtDuplicateObject")};

using nt_wait_for_single_object_prototype = std::uint32_t __stdcall(void*,int,std::uint64_t*) noexcept;

template<bool zw>
inline nt_wait_for_single_object_prototype* const nt_wait_for_single_object{get_nt_module_fptr<nt_wait_for_single_object_prototype>(zw?u8"ZwWaitForSingleObject":u8"NtWaitForSingleObject")};

using nt_set_system_time_prototype = std::uint32_t __stdcall(std::uint64_t*,std::uint64_t*) noexcept;

template<bool zw>
inline nt_set_system_time_prototype* const nt_set_system_time{get_nt_module_fptr<nt_set_system_time_prototype>(zw?u8"ZwSetSystemTime":u8"NtSetSystemTime")};

//https://github.com/mirror/reactos/blob/master/rostests/apitests/ntdll/RtlDosPathNameToNtPathName_U.c
using rtl_dos_path_name_to_nt_path_name_u_prototype = int __stdcall(wchar_t const*,unicode_string*,wchar_t const**,rtl_relative_name_u*) noexcept ;

inline rtl_dos_path_name_to_nt_path_name_u_prototype* const rtl_dos_path_name_to_nt_path_name_u{get_nt_module_fptr<rtl_dos_path_name_to_nt_path_name_u_prototype>(u8"RtlDosPathNameToNtPathName_U")};

//RtlDosPathNameToNtPathName_U

using rtl_free_unicode_string_prototype = void __stdcall(unicode_string*) noexcept;

inline rtl_free_unicode_string_prototype* const rtl_free_unicode_string{get_nt_module_fptr<void __stdcall(unicode_string*) noexcept>(u8"RtlFreeUnicodeString")};


}