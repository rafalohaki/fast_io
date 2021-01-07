#pragma once

namespace fast_io
{
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,manipulators::bool_view_t<bool> val) noexcept
{
	if(val.reference)
		return all->messages.yesstr.len;
	else
		return all->messages.nostr.len;
}

namespace details
{
template<std::random_access_iterator Iter,std::integral char_type>
inline constexpr Iter print_reserve_define_lc_yes_no_impl(basic_lc_all<char_type> const* __restrict all,Iter iter,bool val) noexcept
{
	if(val)
		return non_overlapped_copy_n(all->messages.yesstr.base,all->messages.yesstr.len,iter);
	else
		return non_overlapped_copy_n(all->messages.nostr.base,all->messages.nostr.len,iter);
}
}

template<std::random_access_iterator Iter,std::integral char_type>
inline constexpr Iter print_reserve_define(basic_lc_all<char_type> const* __restrict all,Iter iter,manipulators::bool_view_t<bool> val,std::size_t) noexcept
{
	return details::print_reserve_define_lc_yes_no_impl(all,iter,val.reference);
}

}