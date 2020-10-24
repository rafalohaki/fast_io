#pragma once

namespace fast_io::details
{

template<std::integral char_type>
inline constexpr bool exec_charset_is_ebcdic() noexcept
{
	if constexpr(std::same_as<char_type,char>)
		return static_cast<char8_t>('A')!=u8'A';
#ifndef __MSDOS__
	else if constexpr(std::same_as<char_type,wchar_t>)
		return static_cast<char32_t>(L'A')!=U'A';
#endif
	else
		return false;
}

}