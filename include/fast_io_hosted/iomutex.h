#pragma once
#include <mutex>

namespace fast_io
{

template<stream T,typename mutex_type = std::mutex>
class basic_iomutex
{
public:
	T handler;
	mutex_type mutex;
	using native_handle_type = T;
	using char_type = typename native_handle_type::char_type;
	template<typename ...Args>
	requires std::constructible_from<T,Args...>
	constexpr basic_iomutex(Args&& ...args):handler(std::forward<Args>(args)...){}
	constexpr native_handle_type const& native_handle() const noexcept
	{
		return handler;
	}
	constexpr native_handle_type& native_handle() noexcept
	{
		return handler;
	}
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
	inline auto& unlocked_handle() noexcept
	{
		return handler;
	}
};

template<stream T,typename mutex_type,typename Iter>
inline constexpr decltype(auto) read(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	std::lock_guard gd{mtx};
	return read(mtx.unlocked_handle(),begin,end);
}
template<stream T,typename mutex_type,typename Iter>
inline constexpr decltype(auto) write(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	std::lock_guard gd{mtx};
	return write(mtx.unlocked_handle(),begin,end);
}
template<stream T,typename mutex_type,typename Iter>
inline constexpr decltype(auto) seek(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	std::lock_guard gd{mtx};
	return seek(mtx.unlocked_handle(),begin,end);
}

template<typename T,typename mutex_type>
inline constexpr decltype(auto) redirect_handle(basic_iomutex<T,mutex_type>& t)
{
	return redirect_handle(t.native_handle());
}
}
