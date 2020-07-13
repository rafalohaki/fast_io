#pragma once

namespace fast_io
{

inline void secure_clear(void*,std::size_t);

template<typename T>
class secure_clear_guard
{
public:
	T* region;
	std::size_t count;
	secure_clear_guard(T* rg,std::size_t bts):region(rg),count(bts){}
	secure_clear_guard(secure_clear_guard const&)=delete;
	secure_clear_guard& operator=(secure_clear_guard const&)=delete;
	~secure_clear_guard()
	{
		secure_clear(region,count*sizeof(T));
	}
};

template<typename T>
class secure_clear_no_op
{
	constexpr secure_clear_no_op(T*,std::size_t){};
};

template<typename T,bool condition>
using conditional_secure_clear_guard=std::conditional_t<condition,secure_clear_guard<T>,secure_clear_no_op<T>>;

}