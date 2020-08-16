#pragma once

namespace fast_io
{

namespace details
{

inline void append_to_cmdline(std::string& cmdline,std::string_view str)
{
	bool has_space{};
	for(auto const& e : str)
		if(e==u8' ')
		{
			has_space=true;
			cmdline.push_back(u8'\"');
			break;
		}
	for(auto const& e : str)
	{
		if(e==u8'|')
			cmdline.push_back(u8'^');
		else if(e=='\"')
			cmdline.push_back(u8'\\');
		cmdline.push_back(e);
	}
	if(has_space)
		cmdline.push_back(u8'\"');
}
template<int number>
inline void* redirect_to_std_handle(io_observer& ob)
{
	return std::visit([](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;
		if constexpr(std::same_as<T,void*>)
		{
			return arg;
		}
		else if constexpr(std::same_as<T,std::array<void*,2>>)
		{
			if constexpr(number==0)
				return arg.front();
			else
				return arg.back();
		}
		else if constexpr(std::same_as<T,std::monostate>)
		{
			if constexpr(number==0)
				return fast_io::win32::GetStdHandle(win32_stdin_number);
			else if constexpr(number==1)
				return fast_io::win32::GetStdHandle(win32_stdout_number);
			else
				return fast_io::win32::GetStdHandle(win32_stderr_number);
		}
	},ob.variant);
}

class priviledge_guard
{
	void* hobject{};
	std::uint32_t dw_flags{};
public:
	priviledge_guard(void* hd)
	{
		std::uint32_t dw_flags{};
		if(!fast_io::win32::GetHandleInformation(hd,std::addressof(dw_flags)))
			throw_win32_error();
		if(!(dw_flags&1))
		{
			if(!fast_io::win32::SetHandleInformation(hd,1,1))
				throw_win32_error();
			hobject=hd;
		}
	}
	priviledge_guard(priviledge_guard const&)=delete;
	priviledge_guard& operator=(priviledge_guard const&)=delete;
	~priviledge_guard()
	{
		if(hobject)
			fast_io::win32::SetHandleInformation(hobject,1,0);
	}
};

template<bool jn=false>
class basic_win32_process
{
	win32::process_information pinfo{};
public:
	using native_handle_type = win32::process_information;
/*
	basic_win32_process(native_interface_t,std::string_view path,
		std::string cmdline,
		process_io io)
	{
		if(!win32::CreateProcessA(
			nullptr,cmdline.data(),
			nullptr,nullptr,true,
			0x00000080,
			nullptr,nullptr,std::addressof(sup),std::addressof(pinfo)))
			throw_win32_error();
	}*/
	basic_win32_process(std::string_view path,
				std::vector<std::string_view> args,
				process_io io,char const* directory_folder=nullptr)
	{
		std::string pth(path.data(),path.data()+path.size());
		if(pth.starts_with("./"))
			pth.erase(pth.begin(),pth.begin()+2);
		if(!pth.ends_with(".exe"))
			pth.append(".exe");
		std::string cmdline;//(path.data(),path.data()+path.size());
		append_to_cmdline(cmdline,path);
		for(auto iter(args.cbegin());iter!=args.cend();++iter)
		{
			cmdline.push_back(u8' ');
			append_to_cmdline(cmdline,*iter);
		}
		win32::startupinfo sup{.cb=sizeof(win32::startupinfo),
		.dwFlags=0x00000100,//|0x00000001,
//		.wShowWindow=0x00000001,
		.hStdInput=redirect_to_std_handle<0>(io.in),
		.hStdOutput=redirect_to_std_handle<1>(io.out),
		.hStdError=redirect_to_std_handle<2>(io.err)};

		priviledge_guard stdin_guard(sup.hStdInput),
		stdout_guard(sup.hStdOutput),
		stderr_guard(sup.hStdError);
		if(!win32::CreateProcessA(
			pth.c_str(),cmdline.data(),
			nullptr,nullptr,true,
			0x00000080,
			nullptr,directory_folder,std::addressof(sup),std::addressof(pinfo)))
			throw_win32_error();
	}
	void detach()
	{
		if(pinfo.hThread)
		{
			win32::CloseHandle(pinfo.hThread);
			pinfo.hThread=nullptr;
		}
		if(pinfo.hProcess)
		{
			win32::CloseHandle(pinfo.hProcess);
			pinfo.hProcess=nullptr;
		}
	}
	inline auto& native_handle()
	{
		return pinfo;
	}
	inline auto const& native_handle() const
	{
		return pinfo;
	}
	void join()
	{
		if(static_cast<std::uint32_t>(0xFFFFFFFF)==win32::WaitForSingleObject(pinfo.hProcess,-1))
			throw_win32_error();
		win32::CloseHandle(pinfo.hProcess);
		pinfo.hProcess={};
	}
	basic_win32_process(basic_win32_process const&)=delete;
	basic_win32_process& operator=(basic_win32_process const&)=delete;
	basic_win32_process(basic_win32_process&& other) noexcept:pinfo(other.pinfo)
	{
		other.pinfo={};
	}
	basic_win32_process& operator=(basic_win32_process&& other) noexcept
	{
		if(std::addressof(other)!=this)
		{
			if constexpr(jn)
			{
				if(pinfo.hProcess)
					win32::WaitForSingleObject(pinfo.hProcess,-1);
			}
			detach();
			pinfo=other.pinfo;
			other.pinfo={};
		}
		return *this;
	}
	~basic_win32_process()
	{
		if constexpr(jn)
		{
			if(pinfo.hProcess)
				win32::WaitForSingleObject(pinfo.hProcess,-1);
		}
		detach();
	}
	auto id() const
	{
		return pinfo.dwProcessId;
	}
	auto thread_id() const
	{
		return pinfo.dwThreadId;
	}
};
}

using win32_process = details::basic_win32_process<false>;
using win32_jprocess = details::basic_win32_process<true>;

}