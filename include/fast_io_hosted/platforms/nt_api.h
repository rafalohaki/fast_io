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
void*           SecurityQualityOfService;
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

/*
typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG MaximumLength;
	ULONG Length;

	ULONG Flags;
	ULONG DebugFlags;

	HANDLE ConsoleHandle;
	ULONG ConsoleFlags;
	HANDLE StandardInput;
	HANDLE StandardOutput;
	HANDLE StandardError;

	CURDIR CurrentDirectory;
	UNICODE_STRING DllPath;
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
	PWCHAR Environment;

	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;

	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	UNICODE_STRING WindowTitle;
	UNICODE_STRING DesktopInfo;
	UNICODE_STRING ShellInfo;
	UNICODE_STRING RuntimeData;
	RTL_DRIVE_LETTER_CURDIR CurrentDirectories[RTL_MAX_DRIVE_LETTERS];

	ULONG_PTR EnvironmentSize;
	ULONG_PTR EnvironmentVersion;
	PVOID PackageDependencyData;
	ULONG ProcessGroupId;
	ULONG LoaderThreads;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;
*/

struct curdir
{
	unicode_string DosPath;
	void* Handle;
};

struct rtl_drive_letter_curdir
{
	std::uint16_t Flags;
	std::uint16_t Length;
	std::uint32_t TimeStamp;
	unicode_string DosPath;
};

inline constexpr std::size_t rtl_max_drive_letters{32};

struct rtl_user_process_parameters
{
	std::uint32_t MaximumLength;
	std::uint32_t Length;

	std::uint32_t Flags;
	std::uint32_t DebugFlags;

	void* ConsoleHandle;
	std::uint32_t ConsoleFlags;
	void* StandardInput;
	void* StandardOutput;
	void* StandardError;

	curdir CurrentDirectory;
	unicode_string DllPath;
	unicode_string ImagePathName;
	unicode_string CommandLine;
	wchar_t *Environment;

	std::uint32_t StartingX;
	std::uint32_t StartingY;
	std::uint32_t CountX;
	std::uint32_t CountY;
	std::uint32_t CountCharsX;
	std::uint32_t CountCharsY;
	std::uint32_t FillAttribute;

	std::uint32_t WindowFlags;
	std::uint32_t ShowWindowFlags;
	unicode_string WindowTitle;
	unicode_string DesktopInfo;
	unicode_string ShellInfo;
	unicode_string RuntimeData;
	rtl_drive_letter_curdir CurrentDirectories[rtl_max_drive_letters];

	std::uint32_t* EnvironmentSize;
	std::uint32_t* EnvironmentVersion;
	void* PackageDependencyData;
	std::uint32_t ProcessGroupId;
	std::uint32_t LoaderThreads;
};

enum class ps_create_state
{
	PsCreateInitialState,
	PsCreateFailOnFileOpen,
	PsCreateFailOnSectionCreate,
	PsCreateFailExeFormat,
	PsCreateFailMachineMismatch,
	PsCreateFailExeName, // Debugger specified
	PsCreateSuccess,
	PsCreateMaximumStates
};
enum class ps_protected_signer
{
	none,
	authenticode,
	code_gen,
	antimal_ware,
	lsa,
	windows,
	win_tcb,
	max
};

struct ps_create_info
{
	std::size_t Size;
	ps_create_state State;
	union
	{
		// PsCreateInitialState
		struct
		{
			union
			{
				std::uint32_t InitFlags;
				struct
				{
					unsigned char WriteOutputOnExit : 1;
					unsigned char DetectManifest : 1;
					unsigned char IFEOSkipDebugger : 1;
					unsigned char IFEODoNotPropagateKeyState : 1;
					unsigned char SpareBits1 : 4;
					unsigned char SpareBits2 : 8;
					std::uint16_t ProhibitedImageCharacteristics : 16;
				} s;
			} u;
			std::uint32_t AdditionalFileAccess;
		} InitState;

		// PsCreateFailOnSectionCreate
		struct
		{
			void* FileHandle;
		} FailSection;

		// PsCreateFailExeFormat
		struct
		{
			std::uint32_t DllCharacteristics;
		} ExeFormat;

		// PsCreateFailExeName
		struct
		{
			void* IFEOKey;
		} ExeName;

		// PsCreateSuccess
		struct
		{
			union
			{
				std::uint32_t OutputFlags;
				struct
				{
					unsigned char ProtectedProcess : 1;
					unsigned char AddressSpaceOverride : 1;
					unsigned char DevOverrideEnabled : 1; // From Image File Execution Options
					unsigned char ManifestDetected : 1;
					unsigned char ProtectedProcessLight : 1;
					unsigned char SpareBits1 : 3;
					unsigned char SpareBits2 : 8;
					std::uint16_t SpareBits3 : 16;
				} s;
			} u;
			void* FileHandle;
			void* SectionHandle;
			std::uint64_t UserProcessParametersNative;
			std::uint32_t UserProcessParametersWow64;
			std::uint32_t CurrentParameterFlags;
			std::uint64_t PebAddressNative;
			std::uint32_t PebAddressWow64;
			std::uint64_t ManifestAddress;
			std::uint32_t ManifestSize;
		} SuccessState;
	} u;
};

struct ps_attribute
{
	std::uintptr_t attribute;
	std::size_t size;
	union
	{
		std::uintptr_t Value;				// Reserve 8 bytes for data (such as a Handle or a data pointer)
		void* ValuePtr;					// data pointer
	} u;
	std::size_t *ReturnLength;
};

struct ps_attribute_list
{
	std::size_t TotalLength;
	ps_attribute attribute[2];
};

template<typename... Args>
requires (sizeof...(Args)==11)
inline auto nt_create_user_process(Args&& ...args) noexcept
{
/*
Referenced From
https://github.com/Mattiwatti/BSOD10/blob/b43bc139e97fd7019315e8771fa809f58f7bd53e/src/ntdll.h
NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateUserProcess(
	_Out_ PHANDLE(void**)  ProcessHandle,
	_Out_ PHANDLE(void**) ThreadHandle,
	_In_ ACCESS_MASK(std::uint32_t) ProcessDesiredAccess,
	_In_ ACCESS_MASK(std::uint32_t) ThreadDesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES(object_attributes*) ProcessObjectAttributes,
	_In_opt_ POBJECT_ATTRIBUTES(object_attributes*) ThreadObjectAttributes,
	_In_ ULONG(std::uint32_t) ProcessFlags,
	_In_ ULONG(std::uint32_t) ThreadFlags,
	_In_ PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
	_Inout_ PPS_CREATE_INFO CreateInfo,
	_In_ PPS_ATTRIBUTE_LIST AttributeList
	);
*/
	return (get_nt_module_handle<std::uint32_t __stdcall(void**,void**,std::uint32_t,std::uint32_t,
		object_attributes*,object_attributes*,std::uint32_t,std::uint32_t,rtl_user_process_parameters*,
		ps_create_info*,ps_attribute_list*) noexcept>("NtCreateUserProcess"))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==8)
inline auto nt_create_process(Args&& ...args) noexcept
{
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
	return (get_nt_module_handle<std::uint32_t __stdcall(void**,std::uint32_t,object_attributes*,
		void*,std::uint32_t,void*,void*,void*) noexcept>("NtCreateProcess"))(std::forward<Args>(args)...);
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

template<typename... Args>
requires (sizeof...(Args)==3)
inline auto nt_wait_for_single_object(Args&& ...args) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,int,std::uint64_t*) noexcept >("NtWaitForSingleObject"))(std::forward<Args>(args)...);
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