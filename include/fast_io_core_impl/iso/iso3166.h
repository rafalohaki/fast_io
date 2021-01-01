#pragma once

namespace fast_io
{

template<std::integral char_type,std::size_t alpha>
requires (alpha==2||alpha==3)
struct basic_iso3166_alpha
{
	std::array<char_type,alpha> data{};
};

using iso3166_alpha2 = basic_iso3166_alpha<char,2>;
#ifndef __MSDOS__
using wiso3166_alpha2 = basic_iso3166_alpha<wchar_t,2>;
#endif
using u8iso3166_alpha2 = basic_iso3166_alpha<char8_t,2>;
using u16iso3166_alpha2 = basic_iso3166_alpha<char16_t,2>;
using u32iso3166_alpha2 = basic_iso3166_alpha<char32_t,2>;

using iso3166_alpha3 = basic_iso3166_alpha<char,3>;
#ifndef __MSDOS__
using wiso3166_alpha3 = basic_iso3166_alpha<wchar_t,3>;
#endif
using u8iso3166_alpha3 = basic_iso3166_alpha<char8_t,3>;
using u16iso3166_alpha3 = basic_iso3166_alpha<char16_t,3>;
using u32iso3166_alpha3 = basic_iso3166_alpha<char32_t,3>;

template<std::integral char_type,std::size_t numeric>
requires (alpha==3)
struct basic_iso3166_numeric
{
	using unsigned_integer_type = std::uint16_t;
	unsigned_integer_type code{};
};

using iso3166_numeric3 = basic_iso3166_numeric<char,3>;
#ifndef __MSDOS__
using wiso3166_numeric3 = basic_iso3166_numeric<wchar_t,3>;
#endif
using u8iso3166_numeric3 = basic_iso3166_numeric<char8_t,3>;
using u16iso3166_numeric3 = basic_iso3166_numeric<char16_t,3>;
using u32iso3166_numeric3 = basic_iso3166_numeric<char32_t,3>;

}