#pragma once
#include <mutex>

namespace fast_io
{

template<stream T,typename mutex_type = std::mutex>
class basic_iomutex
{
public:
	[[no_unique_address]] T handle;
	[[no_unique_address]] mutex_type mutex;
	using native_handle_type = T;
	using char_type = typename native_handle_type::char_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr basic_iomutex(Args&& ...args):handle(std::forward<Args>(args)...){}
	inline void lock()
	{
		mutex.lock();
	}
	inline bool try_lock()
	{
		return mutex.try_lock();
	}
	inline void unlock()
	{
		mutex.unlock();
	}
};

template<input_stream T,typename mutex_type,typename Iter>
inline constexpr decltype(auto) read(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	std::lock_guard gd{mtx};
	return read(mtx.handle,begin,end);
}
template<output_stream T,typename mutex_type,typename Iter>
inline constexpr decltype(auto) write(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	std::lock_guard gd{mtx};
	return write(mtx.handle,begin,end);
}
template<random_access_stream T,typename mutex_type,typename Iter>
inline constexpr decltype(auto) seek(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	std::lock_guard gd{mtx};
	return seek(mtx.handle,begin,end);
}

template<typename T,typename mutex_type>
inline constexpr decltype(auto) redirect_handle(basic_iomutex<T,mutex_type>& t)
{
	return redirect_handle(t.handle);
}
}
