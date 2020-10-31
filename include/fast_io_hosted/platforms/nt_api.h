#pragma once

namespace fast_io::win32::nt
{

inline auto get_nt_module_handle() noexcept
{
	auto mod(GetModuleHandleW(L"ntdll.dll"));
	if(mod==nullptr)
		fast_terminate();
	return mod;
}


template<typename func>
inline func* get_nt_module_handle(char const* funname) noexcept
{
	auto hd(get_nt_module_handle());
	auto proc_addr(GetProcAddress(hd,funname));
	if(proc_addr==nullptr)
		fast_terminate();
	return bit_cast<func*>(proc_addr);
}

inline std::uint32_t rtl_nt_status_to_dos_error(std::uint32_t status) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(std::uint32_t status) noexcept >("RtlNtStatusToDosError"))(status);
}

inline std::uint32_t nt_close(void* handle) noexcept
{
	auto func_ptr{get_nt_module_handle<std::uint32_t __stdcall(void*) noexcept >("NtClose")};
	return func_ptr(handle);
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
void*           SecurityQualityOfService{};
};

struct io_status_block
{
union
{
	std::uint32_t Status;
	void*    Pointer;
} DUMMYUNIONNAME;
std::uintptr_t Information;
};



template<typename... Args>
requires (sizeof...(Args)==11)
inline auto nt_create_file(Args&& ...args) noexcept
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
				std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,void*,std::uint32_t) noexcept>("NtCreateFile"))(std::forward<Args>(args)...);
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
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept>("NtWriteFile"))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==9)
inline auto nt_read_file(Args&& ...args) noexcept
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
				void*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept>("NtReadFile"))(std::forward<Args>(args)...);
}

/*
https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/ne-wdm-_file_information_class
*/

enum class file_information_class
{
FileDirectoryInformation                         = 1,
FileFullDirectoryInformation,                   // 2
FileBothDirectoryInformation,                   // 3
FileBasicInformation,                           // 4
FileStandardInformation,                        // 5
FileInternalInformation,                        // 6
FileEaInformation,                              // 7
FileAccessInformation,                          // 8
FileNameInformation,                            // 9
FileRenameInformation,                          // 10
FileLinkInformation,                            // 11
FileNamesInformation,                           // 12
FileDispositionInformation,                     // 13
FilePositionInformation,                        // 14
FileFullEaInformation,                          // 15
FileModeInformation,                            // 16
FileAlignmentInformation,                       // 17
FileAllInformation,                             // 18
FileAllocationInformation,                      // 19
FileEndOfFileInformation,                       // 20
FileAlternateNameInformation,                   // 21
FileStreamInformation,                          // 22
FilePipeInformation,                            // 23
FilePipeLocalInformation,                       // 24
FilePipeRemoteInformation,                      // 25
FileMailslotQueryInformation,                   // 26
FileMailslotSetInformation,                     // 27
FileCompressionInformation,                     // 28
FileObjectIdInformation,                        // 29
FileCompletionInformation,                      // 30
FileMoveClusterInformation,                     // 31
FileQuotaInformation,                           // 32
FileReparsePointInformation,                    // 33
FileNetworkOpenInformation,                     // 34
FileAttributeTagInformation,                    // 35
FileTrackingInformation,                        // 36
FileIdBothDirectoryInformation,                 // 37
FileIdFullDirectoryInformation,                 // 38
FileValidDataLengthInformation,                 // 39
FileShortNameInformation,                       // 40
FileIoCompletionNotificationInformation,        // 41
FileIoStatusBlockRangeInformation,              // 42
FileIoPriorityHintInformation,                  // 43
FileSfioReserveInformation,                     // 44
FileSfioVolumeInformation,                      // 45
FileHardLinkInformation,                        // 46
FileProcessIdsUsingFileInformation,             // 47
FileNormalizedNameInformation,                  // 48
FileNetworkPhysicalNameInformation,             // 49
FileIdGlobalTxDirectoryInformation,             // 50
FileIsRemoteDeviceInformation,                  // 51
FileUnusedInformation,                          // 52
FileNumaNodeInformation,                        // 53
FileStandardLinkInformation,                    // 54
FileRemoteProtocolInformation,                  // 55

		//
		//  These are special versions of these operations (defined earlier)
		//  which can be used by kernel mode drivers only to bypass security
		//  access checks for Rename and HardLink operations.  These operations
		//  are only recognized by the IOManager, a file system should never
		//  receive these.
		//

FileRenameInformationBypassAccessCheck,         // 56
FileLinkInformationBypassAccessCheck,           // 57

		//
		// End of special information classes reserved for IOManager.
		//

FileVolumeNameInformation,                      // 58
FileIdInformation,                              // 59
FileIdExtdDirectoryInformation,                 // 60
FileReplaceCompletionInformation,               // 61
FileHardLinkFullIdInformation,                  // 62
FileIdExtdBothDirectoryInformation,             // 63
FileDispositionInformationEx,                   // 64
FileRenameInformationEx,                        // 65
FileRenameInformationExBypassAccessCheck,       // 66
FileDesiredStorageClassInformation,             // 67
FileStatInformation,                            // 68
FileMemoryPartitionInformation,                 // 69
FileStatLxInformation,                          // 70
FileCaseSensitiveInformation,                   // 71
FileLinkInformationEx,                          // 72
FileLinkInformationExBypassAccessCheck,         // 73
FileStorageReserveIdInformation,                // 74
FileCaseSensitiveInformationForceAccessCheck,   // 75

FileMaximumInformation
};


struct file_full_dir_information
{
std::uint32_t NextEntryOffset;
std::uint32_t FileIndex;
std::int64_t CreationTime;
std::int64_t LastAccessTime;
std::int64_t LastWriteTime;
std::int64_t ChangeTime;
std::int64_t EndOfFile;
std::int64_t AllocationSize;
std::uint32_t FileAttributes;
std::uint32_t FileNameLength;
std::uint32_t EaSize;
wchar_t FileName[1];
};

struct file_both_dir_information
{
std::uint32_t         NextEntryOffset;
std::uint32_t         FileIndex;
std::int64_t CreationTime;
std::int64_t LastAccessTime;
std::int64_t LastWriteTime;
std::int64_t ChangeTime;
std::int64_t EndOfFile;
std::int64_t AllocationSize;
std::uint32_t         FileAttributes;
std::uint32_t         FileNameLength;
std::uint32_t         EaSize;
char         ShortNameLength;
wchar_t         ShortName[12];
wchar_t         FileName[1];
};


union dir_information
{
void* DirInfo;
file_full_dir_information* FullDirInfo;
file_both_dir_information* BothDirInfo;
};

struct file_standard_information
{
std::uint64_t allocation_size;
std::uint64_t end_of_file;
std::uint32_t number_of_links;
int delete_pending;
int directory;
};

template<typename... Args>
requires (sizeof...(Args)==11)
inline auto nt_query_directory_file(Args&& ...args) noexcept
{
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
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,file_information_class,int,unicode_string*,int) noexcept>("NtQueryDirectoryFile"))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==5)
inline auto nt_query_information_file(Args&& ...args) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept >("NtQueryInformationFile"))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==5)
inline auto nt_set_information_file(Args&& ...args) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept >("NtSetInformationFile"))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==7)
inline auto nt_duplicate_object(Args&& ...args) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,void*,void**,std::uint32_t,std::uint32_t,std::uint32_t) noexcept >("NtDuplicateObject"))(std::forward<Args>(args)...);
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
inline auto rtl_dos_path_name_to_nt_path_name_u(Args&& ...args) noexcept
{
//https://github.com/mirror/reactos/blob/master/rostests/apitests/ntdll/RtlDosPathNameToNtPathName_U.c
	return (get_nt_module_handle<int __stdcall(wchar_t const*,unicode_string*,wchar_t const**,rtl_relative_name_u*) noexcept >("RtlDosPathNameToNtPathName_U"))(std::forward<Args>(args)...);
}
//RtlDosPathNameToNtPathName_U

inline void rtl_free_unicode_string(unicode_string* us) noexcept
{
	auto func_ptr{get_nt_module_handle<void __stdcall(unicode_string*) noexcept>("RtlFreeUnicodeString")};
	return func_ptr(us);
}

struct rtl_unicode_string_unique_ptr
{
	unicode_string* heap_ptr{};
	constexpr rtl_unicode_string_unique_ptr()=default;
	constexpr rtl_unicode_string_unique_ptr(unicode_string* ptr):heap_ptr(ptr){}
	rtl_unicode_string_unique_ptr(rtl_unicode_string_unique_ptr const&)=delete;
	rtl_unicode_string_unique_ptr& operator=(rtl_unicode_string_unique_ptr const&)=delete;
	constexpr rtl_unicode_string_unique_ptr(rtl_unicode_string_unique_ptr&& other) noexcept:heap_ptr(other.heap_ptr)
	{
		other.heap_ptr=nullptr;
	}
	rtl_unicode_string_unique_ptr& operator=(rtl_unicode_string_unique_ptr&& other) noexcept
	{
		if(other.heap_ptr==heap_ptr)
			return *this;
		if(heap_ptr)[[likely]]
			rtl_free_unicode_string(heap_ptr);
		heap_ptr=other.heap_ptr;
		other.heap_ptr=nullptr;
		return *this;
	}
	~rtl_unicode_string_unique_ptr()
	{
		if(heap_ptr)[[likely]]
			rtl_free_unicode_string(heap_ptr);
	}
};

}