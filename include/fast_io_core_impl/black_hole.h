#pragma once

namespace fast_io
{

//black_hole is a helper class which helps you remove device requirement for iobuf or iotransform.
//sha256 for example. You do not need a real device. You want to send all your bits to black hole

template<std::integral ch_type=char>
struct black_hole
{
public:
	using char_type = ch_type;
};

template<std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
inline constexpr void write(black_hole<ch_type>,Iter,Iter){}

template<std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
inline constexpr Iter read(black_hole<ch_type>,Iter b,Iter){return b;}

}