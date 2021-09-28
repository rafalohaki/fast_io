#pragma once

namespace fast_io
{

template<stream T,typename Mutex = native_mutex>
requires requires(Mutex&& m)
{
	m.lock();
	m.unlock();
	m.try_lock();
}
struct basic_iomutex
{
	using mutex_type = Mutex;
	using unlocked_handle_type = T;
	using char_type = typename unlocked_handle_type::char_type;
#ifndef __INTELLISENSE__
#if __has_cpp_attribute(msvc::no_unique_address)
[[msvc::no_unique_address]]
#elif __has_cpp_attribute(no_unique_address) >= 201803
[[no_unique_address]]
#endif
#endif
	T handle;
#ifndef __INTELLISENSE__
#if __has_cpp_attribute(msvc::no_unique_address)
[[msvc::no_unique_address]]
#elif __has_cpp_attribute(no_unique_address) >= 201803
[[no_unique_address]]
#endif
#endif
	mutex_type mutex;
	inline constexpr void lock() noexcept(noexcept(mutex.lock()))
	{
		mutex.lock();
	}
	inline constexpr bool try_lock() noexcept(noexcept(mutex.try_lock()))
	{
		return mutex.try_lock();
	}
	inline constexpr void unlock() noexcept
	{
		mutex.unlock();
	}
	inline constexpr unlocked_handle_type const& unlocked_handle() const noexcept
	{
		return handle;
	}
	inline constexpr unlocked_handle_type& unlocked_handle() noexcept
	{
		return handle;
	}
};

template<input_stream T,typename mutex_type,std::input_or_output_iterator Iter>
inline constexpr decltype(auto) read(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	io_lock_guard gd{mtx};
	return read(mtx.handle,begin,end);
}
template<output_stream T,typename mutex_type,std::input_or_output_iterator Iter>
inline constexpr decltype(auto) write(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	io_lock_guard gd{mtx};
	return write(mtx.handle,begin,end);
}
template<random_access_stream T,typename mutex_type,std::input_or_output_iterator Iter>
inline constexpr decltype(auto) seek(basic_iomutex<T,mutex_type>& mtx,Iter begin,Iter end)
{
	io_lock_guard gd{mtx};
	return seek(mtx.handle,begin,end);
}

}
