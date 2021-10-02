#pragma once

namespace fast_io
{

namespace details
{

template<std::integral char_type>
inline constexpr std::size_t dummy_cstr_len(char_type const* cstr) noexcept
{
	std::size_t n{};
	for(;*cstr;++cstr)
		++n;
	return n;

}

}

template<std::integral char_type>
inline constexpr std::size_t cstr_len(char_type const* cstr) noexcept
{
	if(std::is_constant_evaluated())
	{
		return details::dummy_cstr_len(cstr);
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
		{
#if defined(__has_builtin)
#if __has_builtin(__builtin_strlen)
			return __builtin_strlen(cstr);
#else
			return std::strlen(cstr);
#endif
#else
			return std::strlen(cstr);
#endif
		}
		else
		{
			return details::dummy_cstr_len(cstr);
		}

	}
}

}