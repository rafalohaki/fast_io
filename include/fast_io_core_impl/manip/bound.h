#pragma once

namespace fast_io
{

namespace manip
{

template<typename T>
struct bound
{
    T& reference;
    T lower,upper;
};

}

template<typename T,typename U>
requires ((std::integral<T>||std::floating_point<T>)&&(std::integral<U>||std::floating_point<U>))
constexpr manip::bound<T> bound(T& value,U a,U b){return {value,static_cast<T>(a),static_cast<T>(b)};}
/*
template<character_input_stream input,typename T>
requires space_scanable<input,T>
inline constexpr void space_scan_define(input& in,manip::bound<T> a)
{
	space_scan_define(in,a.reference);
	if(a.lower<=a.reference&&a.reference<=a.upper)[[likely]]
		return;
#ifdef __cpp_exceptions
	throw fast_io_text_error("malformed input");
#else
	fast_terminate();
#endif
}
*/
}