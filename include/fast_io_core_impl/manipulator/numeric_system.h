#pragma once

namespace fast_io
{

namespace manipulators
{

template<typename T,char32_t sep,std::size_t N>
struct seperator
{
	T reference;
};

template<typename T,char32_t dec>
struct decimal_point
{
	T reference;
};

template<typename T>
using dot = decimal_point<T,u8'.'>;

template<typename T>
using comma = decimal_point<T,u8','>;

template<typename T,char32_t sep=u8','>
using thousand_sep = seperator<T,sep,3>;

template<typename T>
using dot_thousand_sep = seperator<T,u8'.',3>;

template<typename T>
using comma_decimal_point_dot_thousand_sep = seperator<decimal_point<T,u8','>,u8'.',3>;

template<typename T,char32_t sep=u8','>
using ten_thousand_sep = seperator<T,sep,4>;

template<typename T,char32_t sep=u8','>
struct three_two_two_sep
{
	T reference;
};

}

}