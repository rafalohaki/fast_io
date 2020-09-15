#pragma once
#include"../include/fast_io.h"
namespace fast_io
{

class timer
{
	std::string_view s;
	std::chrono::high_resolution_clock::time_point t0;
public:
	template<typename ...Args>
	requires std::constructible_from<std::string_view,Args...>
	timer(Args&& ...args):s(std::forward<Args>(args)...),t0(std::chrono::high_resolution_clock::now()){}
	timer(const timer&) = delete;
	timer& operator=(const timer&) = delete;
	~timer()
	{
		perrln(s,":\t",std::chrono::high_resolution_clock::now()-t0);
	}
};

}
