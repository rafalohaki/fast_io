#pragma once

namespace fast_io
{

class win32_process
{
	void* handle;
	void close_impl()
	{
		if(handle&&handle!=(void*)(-1))
			win32::CloseHandle(handle);
	}
public:
	using native_handle_t = void*;
	win32_process():handle(win32::nt_fork()){}
	void detach()
	{
		if(handle&&handle!=(void*)(-1))
			if(!win32::CloseHandle(handle))
				throw_win32_error();
		handle=(void*)(-1);
	}
	void join()
	{
		if(!win32::WaitForSingleObject(handle,-1))
			throw_win32_error();
	}
	auto native_handle() const
	{
		return handle;
	}
	win32_process(win32_process const&) = delete;
	win32_process& operator=(win32_process const&) = delete;
	win32_process(win32_process&& other) noexcept : handle(other.handle)
	{
		other.handle = (void*)(-1);
	}
	win32_process& operator=(win32_process&& other) noexcept
	{
		if(other.handle!=handle)
		{
			close_impl();
			handle=other.handle;
			other.handle = (void*)(-1);
		}
		return *this;
	}
	~win32_process()
	{
		close_impl();
	}
};

inline bool is_parent(win32_process const& proc)
{
	return proc.native_handle()&&(proc.native_handle()!=(void*)(-1));
}
inline bool is_child(win32_process const& proc)
{
	return !proc.native_handle();
}
inline bool has_detached(win32_process const& proc)
{
	return proc.native_handle()==(void*)(-1);
}

class win32_jprocess
{
	void* handle;
	void close_impl()
	{
		if(handle&&handle!=(void*)(-1))
		{
			win32::WaitForSingleObject(handle,-1);
			win32::CloseHandle(handle);
		}
	}
public:
	using native_handle_t = void*;
	win32_jprocess():handle(win32::nt_fork()){}
	void detach()
	{
		if(handle&&handle!=(void*)(-1))
			if(!win32::CloseHandle(handle))
				throw_win32_error();
		handle=(void*)(-1);
	}
	void join()
	{
		if(!win32::WaitForSingleObject(handle,-1))
			throw_win32_error();
	}
	auto native_handle() const
	{
		return handle;
	}
	win32_jprocess(win32_jprocess const&) = delete;
	win32_jprocess& operator=(win32_jprocess const&) = delete;
	win32_jprocess(win32_jprocess&& other) noexcept : handle(other.handle)
	{
		other.handle = (void*)(-1);
	}
	win32_jprocess& operator=(win32_jprocess&& other) noexcept
	{
		if(other.handle!=handle)
		{
			close_impl();
			handle=other.handle;
			other.handle = (void*)(-1);
		}
		return *this;
	}
	~win32_jprocess()
	{
		close_impl();
	}
};

inline bool is_parent(win32_jprocess const& proc)
{
	return proc.native_handle()&&(proc.native_handle()!=(void*)(-1));
}
inline bool is_child(win32_jprocess const& proc)
{
	return !proc.native_handle();
}
inline bool has_detached(win32_jprocess const& proc)
{
	return proc.native_handle()==(void*)(-1);
}
}