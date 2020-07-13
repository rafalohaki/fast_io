#pragma once

namespace fast_io
{

template<stream T>
class posix_dev_urandom
{
public:
	using char_type = typename T::char_type;
	T device{"/dev/urandom",fast_io::open_interface<fast_io::open_mode::in|fast_io::open_mode::binary>};
};

template<stream T,std::contiguous_iterator Iter>
inline constexpr Iter read(posix_dev_urandom<T>& t,Iter begin,Iter end)
{
	return read(t.device,begin,end);
}

template<typename T>
inline constexpr void require_secure_clear(posix_dev_urandom<T>){}

}