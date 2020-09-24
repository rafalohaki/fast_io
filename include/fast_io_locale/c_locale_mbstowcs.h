#pragma once

namespace fast_io
{
struct c_locale_multi_to_wide
{
	using manip_tag = manip_tag_t;
	c_locale_observer cloc_ob;
	char const *begin_ptr;
	char const *end_ptr;
	template<std::contiguous_iterator Iter>
	requires std::same_as<std::iter_value_t<Iter>,char>
	constexpr c_locale_multi_to_wide(c_locale_observer c,Iter begin,Iter end):
		cloc_ob(c),begin_ptr(std::to_address(begin)),end_ptr(std::to_address(end)){}
};
namespace details
{
#if defined(__WINNT__) || defined(_MSC_VER)
extern "C" errno_t __stdcall _mbstowcs_s_l(std::size_t*,wchar_t*,std::size_t,char const*,std::size_t,_locale_t);
#endif

template<typename... Args>
requires (sizeof...(Args)==5)
inline std::size_t c_mbstowcs_s_l(Args&& ...args)
{
#if defined(__WINNT__) || defined(_MSC_VER)
	std::size_t sz{};
	auto err{_mbstowcs_s_l(std::addressof(sz),std::forward<Args>(args)...)};
	if(err)
		throw posix_error(static_cast<int>(err));
	return sz;
#else
	std::size_t sz{mbstowcs_l(std::forward<Args>(args)...)};
	if(sz==std::numeric_limits<std::size_t>::max())
		throw posix_error();
	return sz;
#endif
}
}

}