#pragma once

namespace fast_io::win32::nt
{

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

using pio_apc_routine = void (*)(void*,io_status_block*,std::uint32_t) noexcept;

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

struct client_id
{
	void* hprocess;
	void* hthread;
};

enum class section_information_class
{
SectionBasicInformation,
SectionImageInformation,
SectionRelocationInformation,
MaxSectionInfoClass
};

struct rtl_critical_section
{
	void* debug_info;
	std::int32_t lock_count;
	std::int32_t recursion_count;
	void* owning_thread;
	void* lock_semaphore;
	std::uintptr_t spin_count;
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

}