#pragma once

namespace fast_io::win32::nt
{

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
	if(mod==nullptr)
		fast_terminate();
	return mod;
}

template<typename func>
inline func* get_funcptr(void* module,char8_t const* funname) noexcept
{
	auto proc_addr(GetProcAddress(module,reinterpret_cast<char const*>(funname)));
	if(proc_addr==nullptr)
		fast_terminate();
	return bit_cast<func*>(proc_addr);
}

/*
Guard against EBCDIC execution charset
*/
template<typename func>
inline func* get_nt_module_handle(char8_t const* funname) noexcept
{
	auto hd(get_nt_module_handle());
	auto proc_addr(GetProcAddress(hd,reinterpret_cast<char const*>(funname)));
	if(proc_addr==nullptr)
		fast_terminate();
	return bit_cast<func*>(proc_addr);
}

inline std::uint32_t rtl_nt_status_to_dos_error(std::uint32_t status) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(std::uint32_t status) noexcept >(u8"RtlNtStatusToDosError"))(status);
}

template<bool zw>
inline std::uint32_t nt_close(void* handle) noexcept
{
	constexpr char8_t const* func_name{zw?u8"ZwClose":u8"NtClose"};
	auto func_ptr{get_nt_module_handle<std::uint32_t __stdcall(void*) noexcept >(func_name)};
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



template<bool zw,typename... Args>
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
	constexpr char8_t const* func_name{zw?u8"ZwCreateFile":u8"NtCreateFile"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void**,std::uint32_t,object_attributes*,io_status_block*,std::int64_t*,
			std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,void*,std::uint32_t) noexcept>(func_name))(std::forward<Args>(args)...);
}

enum class ps_attribute_num
{
	PsAttributeParentProcess, // in HANDLE
	PsAttributeDebugPort, // in HANDLE
	PsAttributeToken, // in HANDLE
	PsAttributeClientId, // out PCLIENT_ID
	PsAttributeTebAddress, // out PTEB
	PsAttributeImageName, // in PWSTR
	PsAttributeImageInfo, // out PSECTION_IMAGE_INFORMATION
	PsAttributeMemoryReserve, // in PPS_MEMORY_RESERVE
	PsAttributePriorityClass, // in UCHAR
	PsAttributeErrorMode, // in ULONG
	PsAttributeStdHandleInfo, // 10, in PPS_STD_HANDLE_INFO
	PsAttributeHandleList, // in PHANDLE
	PsAttributeGroupAffinity, // in PGROUP_AFFINITY
	PsAttributePreferredNode, // in PUSHORT
	PsAttributeIdealProcessor, // in PPROCESSOR_NUMBER
	PsAttributeUmsThread, // see UpdateProceThreadAttributeList in msdn (CreateProcessA/W...) in PUMS_CREATE_THREAD_ATTRIBUTES
	PsAttributeMitigationOptions, // in UCHAR
	PsAttributeProtectionLevel,
	PsAttributeSecureProcess, // since THRESHOLD (Virtual Secure Mode, Device Guard)
	PsAttributeJobList,
	PsAttributeMax
};

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

	std::uint32_t EnvironmentSize;
	std::uint32_t EnvironmentVersion;
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

/*
Referenced from MSDN
https://docs.microsoft.com/en-us/windows/win32/procthread/zwqueryinformationprocess

struct ps_protection
{
union
{
char unsigned Level;
struct
{
	char unsigned Type   : 3;
	char unsigned Audit  : 1;                  // Reserved
	char unsigned Signer : 4;
};
};
}
*/

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
	std::uintptr_t Attribute;
	std::size_t Size;
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
	ps_attribute Attributes[2];
};

struct section_image_information
{
	void* TransferAddress;
	std::uint32_t ZeroBits;
	std::size_t MaximumStackSize;
	std::size_t CommittedStackSize;
	std::uint32_t SubSystemType;
	union U
	{
		struct S
		{
			std::uint16_t SubSystemMinorVersion;
			std::uint16_t SubSystemMajorVersion;
		}s;
		std::uint32_t SubSystemVersion;
	}u;
	std::uint32_t GpValue;
	std::uint16_t ImageCharacteristics;
	std::uint16_t DllCharacteristics;
	std::uint16_t Machine;
	int ImageContainsCode;
	union U1
	{
		char unsigned ImageFlags;
		struct S
		{
			char unsigned ComPlusNativeReady : 1;
			char unsigned ComPlusILOnly : 1;
			char unsigned ImageDynamicallyRelocated : 1;
			char unsigned ImageMappedFlat : 1;
			char unsigned BaseBelow4gb : 1;
			char unsigned Reserved : 3;
		}s;
	}u1;
	std::uint32_t LoaderFlags;
	std::uint32_t ImageFileSize;
	std::uint32_t CheckSum;
};

template<bool zw,typename... Args>
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
	constexpr char8_t const* func_name{zw?u8"ZwCreateUserProcess":u8"NtCreateUserProcess"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void**,void**,std::uint32_t,std::uint32_t,
		object_attributes*,object_attributes*,std::uint32_t,std::uint32_t,rtl_user_process_parameters*,
		ps_create_info*,ps_attribute_list*) noexcept>(func_name))(std::forward<Args>(args)...);
}


template<bool zw,typename... Args>
requires (sizeof...(Args)==7)
inline auto nt_create_section(Args&& ...args) noexcept
{
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
	constexpr char8_t const* func_name{zw?u8"ZwCreateSection":u8"NtCreateSection"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void** __restrict,std::uint32_t,object_attributes* __restrict,
		std::uint64_t*,std::uint32_t,std::uint32_t,void* __restrict) noexcept>(func_name))(std::forward<Args>(args)...);
}

enum class process_information_class
{
ProcessBasicInformation, // q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
ProcessQuotaLimits, // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
ProcessIoCounters, // q: IO_COUNTERS
ProcessVmCounters, // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
ProcessTimes, // q: KERNEL_USER_TIMES
ProcessBasePriority, // s: KPRIORITY
ProcessRaisePriority, // s: ULONG
ProcessDebugPort, // q: HANDLE
ProcessExceptionPort, // s: PROCESS_EXCEPTION_PORT
ProcessAccessToken, // s: PROCESS_ACCESS_TOKEN
ProcessLdtInformation, // qs: PROCESS_LDT_INFORMATION // 10
ProcessLdtSize, // s: PROCESS_LDT_SIZE
ProcessDefaultHardErrorMode, // qs: ULONG
ProcessIoPortHandlers, // (kernel-mode only) // PROCESS_IO_PORT_HANDLER_INFORMATION
ProcessPooledUsageAndLimits, // q: POOLED_USAGE_AND_LIMITS
ProcessWorkingSetWatch, // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
ProcessUserModeIOPL, // qs: ULONG (requires SeTcbPrivilege)
ProcessEnableAlignmentFaultFixup, // s: BOOLEAN
ProcessPriorityClass, // qs: PROCESS_PRIORITY_CLASS
ProcessWx86Information, // qs: ULONG (requires SeTcbPrivilege) (VdmAllowed)
ProcessHandleCount, // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
ProcessAffinityMask, // s: KAFFINITY
ProcessPriorityBoost, // qs: ULONG
ProcessDeviceMap, // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
ProcessSessionInformation, // q: PROCESS_SESSION_INFORMATION
ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND
ProcessWow64Information, // q: ULONG_PTR
ProcessImageFileName, // q: UNICODE_STRING
ProcessLUIDDeviceMapsEnabled, // q: ULONG
ProcessBreakOnTermination, // qs: ULONG
ProcessDebugObjectHandle, // q: HANDLE // 30
ProcessDebugFlags, // qs: ULONG
ProcessHandleTracing, // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
ProcessIoPriority, // qs: IO_PRIORITY_HINT
ProcessExecuteFlags, // qs: ULONG
ProcessResourceManagement, // ProcessTlsInformation // PROCESS_TLS_INFORMATION
ProcessCookie, // q: ULONG
ProcessImageInformation, // q: SECTION_IMAGE_INFORMATION
ProcessCycleTime, // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
ProcessPagePriority, // q: PAGE_PRIORITY_INFORMATION
ProcessInstrumentationCallback, // qs: PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION // 40
ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
ProcessWorkingSetWatchEx, // q: PROCESS_WS_WATCH_INFORMATION_EX[]
ProcessImageFileNameWin32, // q: UNICODE_STRING
ProcessImageFileMapping, // q: HANDLE (input)
ProcessAffinityUpdateMode, // qs: PROCESS_AFFINITY_UPDATE_MODE
ProcessMemoryAllocationMode, // qs: PROCESS_MEMORY_ALLOCATION_MODE
ProcessGroupInformation, // q: USHORT[]
ProcessTokenVirtualizationEnabled, // s: ULONG
ProcessConsoleHostProcess, // q: ULONG_PTR // ProcessOwnerInformation
ProcessWindowInformation, // q: PROCESS_WINDOW_INFORMATION // 50
ProcessHandleInformation, // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
ProcessMitigationPolicy, // s: PROCESS_MITIGATION_POLICY_INFORMATION
ProcessDynamicFunctionTableInformation,
ProcessHandleCheckingMode, // qs: ULONG; s: 0 disables, otherwise enables
ProcessKeepAliveCount, // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
ProcessRevokeFileHandles, // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
ProcessWorkingSetControl, // s: PROCESS_WORKING_SET_CONTROL
ProcessHandleTable, // q: ULONG[] // since WINBLUE
ProcessCheckStackExtentsMode,
ProcessCommandLineInformation, // q: UNICODE_STRING // 60
ProcessProtectionInformation, // q: PS_PROTECTION
ProcessMemoryExhaustion, // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
ProcessFaultInformation, // PROCESS_FAULT_INFORMATION
ProcessTelemetryIdInformation, // PROCESS_TELEMETRY_ID_INFORMATION
ProcessCommitReleaseInformation, // PROCESS_COMMIT_RELEASE_INFORMATION
ProcessDefaultCpuSetsInformation,
ProcessAllowedCpuSetsInformation,
ProcessSubsystemProcess,
ProcessJobMemoryInformation, // PROCESS_JOB_MEMORY_INFO
ProcessInPrivate, // since THRESHOLD2 // 70
ProcessRaiseUMExceptionOnInvalidHandleClose, // qs: ULONG; s: 0 disables, otherwise enables
ProcessIumChallengeResponse,
ProcessChildProcessInformation, // PROCESS_CHILD_PROCESS_INFORMATION
ProcessHighGraphicsPriorityInformation,
ProcessSubsystemInformation, // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
ProcessEnergyValues, // PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
ProcessActivityThrottleState, // PROCESS_ACTIVITY_THROTTLE_STATE
ProcessActivityThrottlePolicy, // PROCESS_ACTIVITY_THROTTLE_POLICY
ProcessWin32kSyscallFilterInformation,
ProcessDisableSystemAllowedCpuSets, // 80
ProcessWakeInformation, // PROCESS_WAKE_INFORMATION
ProcessEnergyTrackingState, // PROCESS_ENERGY_TRACKING_STATE
ProcessManageWritesToExecutableMemory, // MANAGE_WRITES_TO_EXECUTABLE_MEMORY // since REDSTONE3
ProcessCaptureTrustletLiveDump,
ProcessTelemetryCoverage,
ProcessEnclaveInformation,
ProcessEnableReadWriteVmLogging, // PROCESS_READWRITEVM_LOGGING_INFORMATION
ProcessUptimeInformation, // PROCESS_UPTIME_INFORMATION
ProcessImageSection, // q: HANDLE
ProcessDebugAuthInformation, // since REDSTONE4 // 90
ProcessSystemResourceManagement, // PROCESS_SYSTEM_RESOURCE_MANAGEMENT
ProcessSequenceNumber, // q: ULONGLONG
ProcessLoaderDetour, // since REDSTONE5
ProcessSecurityDomainInformation, // PROCESS_SECURITY_DOMAIN_INFORMATION
ProcessCombineSecurityDomainsInformation, // PROCESS_COMBINE_SECURITY_DOMAINS_INFORMATION
ProcessEnableLogging, // PROCESS_LOGGING_INFORMATION
ProcessLeapSecondInformation, // PROCESS_LEAP_SECOND_INFORMATION
ProcessFiberShadowStackAllocation, // PROCESS_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION // since 19H1
ProcessFreeFiberShadowStackAllocation, // PROCESS_FREE_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
ProcessAltSystemCallInformation, // qs: BOOLEAN (kernel-mode only) // since 20H1 // 100
ProcessDynamicEHContinuationTargets, // PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION
MaxProcessInfoClass
};
struct process_basic_information
{
	std::uint32_t ExitStatus;
	void* PebBaseAddress;
	std::uintptr_t AffinityMask;
	std::uint32_t BasePriority;
	void* UniqueProcessId;
	void* InheritedFromUniqueProcessId;
};
template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline auto nt_query_information_process(Args&&...args) noexcept
{
/*
__kernel_entry NTSTATUS NtQueryInformationProcess(
  HANDLE           ProcessHandle,
  PROCESSINFOCLASS ProcessInformationClass,
  PVOID            ProcessInformation,
  ULONG            ProcessInformationLength,
  PULONG           ReturnLength
);
*/
	constexpr char8_t const* func_name{zw?u8"ZwQueryInformationProcess":u8"NtQueryInformationProcess"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void* __restrict,
		process_information_class,process_basic_information*,
		std::uint32_t,std::uint32_t*) noexcept>(func_name))(std::forward<Args>(args)...);
}


template<bool zw,typename... Args>
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
	constexpr char8_t const* func_name{zw?u8"ZwCreateProcess":u8"NtCreateProcess"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void**,std::uint32_t,object_attributes*,
		void*,std::uint32_t,void*,void*,void*) noexcept>(func_name))(std::forward<Args>(args)...);
}


struct client_id
{
	void* hprocess;
	void* hthread;
};

using pio_apc_routine = void (*)(void*,io_status_block*,std::uint32_t);
//typedef VOID (NTAPI *PIO_APC_ROUTINE)(PVOID ApcContext,PIO_STATUS_BLOCK IoStatusBlock,ULONG Reserved);

template<bool zw,typename... Args>
requires (sizeof...(Args)==9)
inline auto nt_write_file(Args&& ...args) noexcept
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
	constexpr char8_t const* func_name{zw?u8"ZwWriteFile":u8"NtWriteFile"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void const*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept>(func_name))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==10)
inline auto rtl_create_user_thread(Args&& ...args) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,int,std::uint32_t,std::size_t,std::size_t,void*,void*,void**,client_id*) noexcept>(u8"RtlCreateUserThread"))(std::forward<Args>(args)...);
}

template<typename... Args>
requires (sizeof...(Args)==3)
inline auto rtl_p_init_environment(Args&& ...args) noexcept
{
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,rtl_user_process_parameters*) noexcept>(u8"RtlpInitEnvironment"))(std::forward<Args>(args)...);
}

template<bool zw,typename... Args>
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
	constexpr char8_t const* func_name{zw?u8"ZwReadFile":u8"NtReadFile"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,std::int64_t*,std::uint32_t*) noexcept>(func_name))(std::forward<Args>(args)...);
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

template<bool zw,typename... Args>
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
	constexpr char8_t const* func_name{zw?u8"ZwQueryDirectoryFile":u8"NtQueryDirectoryFile"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,pio_apc_routine,void*,io_status_block*,
				void*,std::uint32_t,file_information_class,int,unicode_string*,int) noexcept>(func_name))(std::forward<Args>(args)...);
}

enum class section_information_class
{
SectionBasicInformation,
SectionImageInformation,
SectionRelocationInformation,
MaxSectionInfoClass
};

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline auto nt_query_section(Args&& ...args) noexcept
{
	constexpr char8_t const* func_name{zw?u8"ZwQuerySection":u8"NtQuerySection"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,section_information_class,void*,std::size_t,std::size_t*) noexcept>(func_name))(std::forward<Args>(args)...);
}

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline auto nt_query_information_file(Args&& ...args) noexcept
{
	constexpr char8_t const* func_name{zw?u8"ZwQueryInformationFile":u8"NtQueryInformationFile"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept >(func_name))(std::forward<Args>(args)...);
}

template<bool zw,typename... Args>
requires (sizeof...(Args)==5)
inline auto nt_set_information_file(Args&& ...args) noexcept
{
	constexpr char8_t const* func_name{zw?u8"ZwSetInformationFile":u8"NtSetInformationFile"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void* __restrict,io_status_block* __restrict,void* __restrict,std::uint32_t,file_information_class) noexcept >(func_name))(std::forward<Args>(args)...);
}

template<bool zw,typename... Args>
requires (sizeof...(Args)==7)
inline auto nt_duplicate_object(Args&& ...args) noexcept
{
	constexpr char8_t const* func_name{zw?u8"ZwDuplicateObject":u8"NtDuplicateObject"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,void*,void*,void**,std::uint32_t,std::uint32_t,std::uint32_t) noexcept >(func_name))(std::forward<Args>(args)...);
}

template<bool zw,typename... Args>
requires (sizeof...(Args)==3)
inline auto nt_wait_for_single_object(Args&& ...args) noexcept
{
	constexpr char8_t const* func_name{zw?u8"ZwWaitForSingleObject":u8"NtWaitForSingleObject"};
	return (get_nt_module_handle<std::uint32_t __stdcall(void*,int,std::uint64_t*) noexcept >(func_name))(std::forward<Args>(args)...);
}


template<bool zw,typename... Args>
requires (sizeof...(Args)==2)
inline auto nt_set_system_time(Args&& ...args) noexcept
{
	constexpr char8_t const* func_name{zw?u8"ZwSetSystemTime":u8"NtSetSystemTime"};
	return (get_nt_module_handle<std::uint32_t __stdcall(std::uint64_t*,std::uint64_t*) noexcept >(func_name))(std::forward<Args>(args)...);
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
	return (get_nt_module_handle<int __stdcall(wchar_t const*,unicode_string*,wchar_t const**,rtl_relative_name_u*) noexcept >(u8"RtlDosPathNameToNtPathName_U"))(std::forward<Args>(args)...);
}
//RtlDosPathNameToNtPathName_U

inline void rtl_free_unicode_string(unicode_string* us) noexcept
{
	auto func_ptr{get_nt_module_handle<void __stdcall(unicode_string*) noexcept>(u8"RtlFreeUnicodeString")};
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