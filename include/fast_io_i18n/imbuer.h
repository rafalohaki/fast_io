#pragma once

namespace fast_io
{

template<stream stm>
requires (value_based_stream<stm>&&requires(stm sm)
{
	{io_value_handle(sm)}->std::same_as<stm>;
}&&std::is_trivially_copyable_v<stm>)
struct lc_imbuer
{
	using handle_type = stm;
	using char_type = typename handle_type::char_type;
	using lc_all_type = basic_lc_all<char_type>;
	lc_all_type const* all{};
	[[no_unique_address]] handle_type handle{};
};

template<stream stm>
inline constexpr lc_imbuer<stm> io_value_handle(lc_imbuer<stm> sm) noexcept
{
	return sm;
}

template<stream stm>
requires (std::is_lvalue_reference_v<stm>||std::is_trivially_copyable_v<stm>)
inline constexpr auto imbue(basic_lc_all<typename std::remove_cvref_t<stm>::char_type> const* all,stm&& out) noexcept
{
	using char_type = typename std::remove_cvref_t<stm>::char_type;
	if constexpr(value_based_stream<stm>)
		return lc_imbuer<decltype(io_value_handle(out))>{all,io_value_handle(out)};
	else
		return lc_imbuer<decltype(io_ref(out))>{all,io_ref(out)};
}

template<stream stm>
requires (std::is_lvalue_reference_v<stm>||std::is_trivially_copyable_v<stm>)
inline constexpr auto imbue(l10n& loc,stm&& out) noexcept
{
	using char_type = typename std::remove_cvref_t<stm>::char_type;
	return imbue(get_all<char_type>(loc.loc),out);
}

}