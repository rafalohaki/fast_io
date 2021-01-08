#pragma once

namespace fast_io
{
template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(basic_lc_all<char_type> const* __restrict all,manipulators::blvw_t<bool> val) noexcept
{
	if(val.reference)
		return all->messages.yesstr;
	else
		return all->messages.nostr;
}
static_assert(lc_scatter_printable<char8_t,manipulators::blvw_t<bool>>);
static_assert(lc_scatter_type_printable<char8_t,manipulators::blvw_t<bool>>);

}