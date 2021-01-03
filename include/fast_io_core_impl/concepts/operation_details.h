#pragma once

namespace fast_io::details
{
	
template<typename ch_type>
concept character = std::integral<ch_type>&&(std::same_as<ch_type,char>||std::same_as<ch_type,wchar_t>||
std::same_as<ch_type,char8_t>||std::same_as<ch_type,char32_t>);

}