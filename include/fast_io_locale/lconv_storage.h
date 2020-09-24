#pragma once

namespace fast_io
{

template<std::integral ch_type,std::ranges::contiguous_range stg_type=std::vector<ch_type>>
struct basic_lconv_storage
{
	using char_type = ch_type;
	using storage_type = stg_type;
	using size_type = typename storage_type::size_type;
	using string_view_type = std::basic_string_view<char_type>;
	storage_type storage;

	char_type decimal_point{};
	char_type thousands_sep{};

	char_type mon_decimal_point{}; 
	char_type mon_thousands_sep{};

	char_type frac_digits{};
	char_type p_cs_precedes{};
	char_type n_cs_precedes{};
	char_type p_sep_by_space{};
	char_type n_sep_by_space{};
	char_type p_sign_posn{};
	char_type n_sign_posn{};
	char_type int_frac_digits{};
	char_type int_p_cs_precedes{};
	char_type int_n_cs_precedes{};
	char_type int_p_sep_by_space{};
	char_type int_n_sep_by_space{};
	char_type int_p_sign_posn{};
	char_type int_n_sign_posn{};

	size_type mon_grouping_start{};
	size_type positive_sign_start{};
	size_type negative_sign_start{};
	size_type currency_symbol_start{};
	size_type int_curr_symbol_start{};
	constexpr string_view_type grouping() const noexcept
	{
		return string_view_type(storage.data(),mon_grouping_start);
	}
	constexpr string_view_type mon_grouping() const noexcept
	{
		return string_view_type(storage.data()+mon_grouping_start,positive_sign_start-mon_grouping_start);
	}
	constexpr string_view_type positive_sign() const noexcept
	{
		return string_view_type(storage.data()+positive_sign_start,negative_sign_start-positive_sign_start);
	}
	constexpr string_view_type negative_sign() const noexcept
	{
		return string_view_type(storage.data()+negative_sign_start,currency_symbol_start-negative_sign_start);
	}
	constexpr string_view_type currency_symbol() const noexcept
	{
		return string_view_type(storage.data()+currency_symbol_start,int_curr_symbol_start-currency_symbol_start);
	}
	constexpr string_view_type int_curr_symbol() const noexcept
	{
		return string_view_type(storage.data()+int_curr_symbol_start,storage.size()-int_curr_symbol_start);
	}
};

}