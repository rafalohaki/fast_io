#pragma once

#include<variant>
namespace fast_io
{
struct io_observer
{
public:
	using native_handle_type = 
#if defined(__WINNT__) || defined(_MSC_VER)
		void*
#else
		int
#endif
;
	using variant_type = std::variant<std::monostate,native_handle_type,std::array<
#if defined(__WINNT__) || defined(_MSC_VER)
native_handle_type
#else
int*
#endif
,2>>;
	variant_type variant;
	template<redirect_stream T>
	io_observer(T&& hd):variant(redirect_handle(hd)){}
	io_observer()=default;
};

struct process_io
{
	io_observer in,out,err;
};
}
