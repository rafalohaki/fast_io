#pragma once
//https://github.com/kaniini/win32-fork/blob/master/fork.c
namespace fast_io::win32
{

struct client_id
{
	void* unique_process;
	void* unique_thread;
};
struct section_image_information
{
	void* entry_point;
	std::uint32_t stack_zero_bits;
	std::uint32_t stack_reserved;
	std::uint32_t stack_commit;
	std::uint32_t image_subsystem;
	std::uint16_t sub_system_version_low;
	std::uint16_t sub_system_version_high;
	std::uint32_t unknown1;
	std::uint32_t image_characteristics;
	std::uint32_t image_machine_type;
	std::array<std::uint32_t,3> unknown2;
};

struct rtl_user_process_information
{
	std::uint32_t size;
	void* process;
	void* thread;
	client_id cid;
	section_image_information image_information;
};
inline void* nt_fork()
{
	auto mod(GetModuleHandleW(L"ntdll.dll"));
	if(mod==nullptr)
		throw_win32_error();
	auto proc_addr(GetProcAddress(mod,"RtlCloneUserProcess"));
	if(proc_addr==nullptr)
		throw_win32_error();
	auto RtlCloneUserProcess(bit_cast<std::uint32_t __stdcall (*)(std::uint32_t,void*,void*,void*,rtl_user_process_information*)>(proc_addr));
	auto resume_proc_addr(GetProcAddress(mod,"NtResumeProcess"));
	if(resume_proc_addr==nullptr)
		throw_win32_error();
	auto NtResumeProcess(bit_cast<std::uint32_t __stdcall (*)(void*)>(resume_proc_addr));
	rtl_user_process_information process_info{};

	auto v(RtlCloneUserProcess(0x00000001|/*0x00000001RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED
	|*/0x00000002/*RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES*/,nullptr,nullptr,nullptr,std::addressof(process_info)));
	if(v==0/*RTL_CLONE_PARENT*/)
	{
		if(int w=NtResumeProcess(process_info.process))
			throw nt_error(w);
		if(win32::ResumeThread(process_info.thread)==static_cast<std::uint32_t>(-1))
		{
#ifdef __cpp_exceptions
			win32::CloseHandle(process_info.thread);
			win32::CloseHandle(process_info.process);
			throw win32_error();
#else
			fast_terminate();
#endif
		}
		win32::CloseHandle(process_info.thread);
		return process_info.process;
	}
	else if(v==297/*RTL_CLONE_CHILD*/)
	{
		if(!AllocConsole())
			throw win32_error();
		return nullptr;
	}
	throw_nt_error(v);
	return nullptr;
}

}