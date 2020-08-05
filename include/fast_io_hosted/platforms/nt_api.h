#pragma once

namespace fast_io::win32::nt
{

template<bool no_exception=false>
inline auto get_nt_module_handle() noexcept(no_exception)
{
	auto mod(GetModuleHandleW(L"ntdll.dll"));
	if constexpr(no_exception)
		return mod;
	else
	{
		if(mod==nullptr)
			throw_win32_error();
		return mod;
	}
}


template<typename func,bool no_exception=false>
inline func* get_nt_module_handle(char const* funname) noexcept(no_exception)
{
	auto hd(get_nt_module_handle<no_exception>());
	if constexpr(no_exception)
	{
		if(hd==nullptr)
			return nullptr;
	}
	auto proc_addr(GetProcAddress(hd,funname));
	if constexpr(no_exception)
	{
		return nullptr;
	}
	else
	{
		if(proc_addr==nullptr)
			throw_win32_error();
		return bit_cast<func*>(proc_addr);
	}
}

inline std::uint32_t rtl_nt_status_to_dos_error(std::uint32_t status)
{
	return (get_nt_module_handle<std::uint32_t __stdcall(std::uint32_t status)>("RtlNtStatusToDosError"))(status);
}

inline void nt_close(void* handle) noexcept
{
	auto func_ptr{get_nt_module_handle<std::uint32_t __stdcall(void*),true>("NtClose")};
	if(func_ptr==nullptr)
		return;
	func_ptr(handle);
}

struct unicode_string
{
std::uint16_t Length;
std::uint16_t MaximumLength;
wchar_t*  Buffer;
};

struct object_attributes
{
std::uint32_t Length;
void*          RootDirectory;
unicode_string *ObjectName;
std::uint32_t   Attributes;
void*           SecurityDescriptor;
void*           SecurityQualityOfService;
};

struct io_status_block
{
union
{
	std::uint32_t Status;
	void*    Pointer;
} DUMMYUNIONNAME;
std::uint32_t* Information;
};

template<typename... Args>
requires (sizeof...(Args)==11)
inline auto nt_create_file(Args&& ...args)
{
/*
__kernel_entry NTSYSCALLAPI NTSTATUS NtCreateFile(
PHANDLE(void**)            FileHandle,
ACCESS_MASK(std::uint32_t)        DesiredAccess,
POBJECT_ATTRIBUTES(object_attributes*) ObjectAttributes,
PIO_STATUS_BLOCK(io_status_block*)   IoStatusBlock,
PLARGE_INTEGER(std::int64_t*)     AllocationSize,
ULONG(std::uint32_t)              FileAttributes,
ULONG(std::uint32_t)              ShareAccess,
ULONG(std::uint32_t)              CreateDisposition,
ULONG(std::uint32_t)              CreateOptions,
PVOID(void*)              EaBuffer,
ULONG(std::uint32_t)              EaLength
);
*/
	return (get_nt_module_handle<std::uint32_t __stdcall(void**,std::uint32_t,object_attributes*,io_status_block*,std::int64_t*,
				std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,void*,std::uint32_t)>("NtCreateFile"))(std::forward<Args>(args)...);
}

using pio_apc_routine = void (*)(void*,io_status_block*,std::uint32_t);
//typedef VOID (NTAPI *PIO_APC_ROUTINE)(PVOID ApcContext,PIO_STATUS_BLOCK IoStatusBlock,ULONG Reserved);

template<typename... Args>
requires (sizeof...(Args)==9)
inline auto nt_write_file(Args&& ...args)
{
/*
__kernel_entry NTSYSCALLAPI NTSTATUS NtWriteFile(
  HANDLE           FileHandle,
  HANDLE           Event,
  PIO_APC_ROUTINE  ApcRoutine,
  PVOID            ApcContext,
  PIO_STATUS_BLOCK IoStatusBlock,
  PVOID            Buffer,
  ULONG            Length,
  PLARGE_INTEGER   ByteOffset,
  PULONG           Key
);
*/
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*)>("NtWriteFile"))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==9)
inline auto nt_read_file(Args&& ...args)
{
/*
__kernel_entry NTSYSCALLAPI NTSTATUS NtReadFile(
  HANDLE           FileHandle,
  HANDLE           Event,
  PIO_APC_ROUTINE  ApcRoutine,
  PVOID            ApcContext,
  PIO_STATUS_BLOCK IoStatusBlock,
  PVOID            Buffer,
  ULONG            Length,
  PLARGE_INTEGER   ByteOffset,
  PULONG           Key
);
*/
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,std::int64_t*,std::uint32_t*)>("NtReadFile"))(std::forward<Args>(args)...);
}

struct rtlp_curdir_def
{
	std::int32_t ref_count;
	void* handle;
};

struct rtl_relative_name_u
{
	unicode_string relative_name;
	void* containing_directory;
	rtlp_curdir_def cur_dir_ref;
};

template<typename... Args>
requires (sizeof...(Args)==4)
inline auto rtl_dos_path_name_to_nt_path_name_u(Args&& ...args)
{
//https://github.com/mirror/reactos/blob/master/rostests/apitests/ntdll/RtlDosPathNameToNtPathName_U.c
	return (get_nt_module_handle<int __stdcall(wchar_t const*,unicode_string*,wchar_t const**,rtl_relative_name_u*)>("RtlDosPathNameToNtPathName_U"))(std::forward<Args>(args)...);
}
//RtlDosPathNameToNtPathName_U

}