#pragma once

namespace fast_io
{

template<std::integral char_type>
struct basic_lc_identification
{
	std::basic_string_view<char_type> title{};
	std::basic_string_view<char_type> source{};
	std::basic_string_view<char_type> address{};
	std::basic_string_view<char_type> contact{};
	std::basic_string_view<char_type> email{};
	std::basic_string_view<char_type> tel{};
	std::basic_string_view<char_type> fax{};
	std::basic_string_view<char_type> language{};
	std::basic_string_view<char_type> territory{};
	std::basic_string_view<char_type> audience{};
	std::basic_string_view<char_type> application{};
	std::basic_string_view<char_type> abbreviation{};
	std::basic_string_view<char_type> revision{};
	std::basic_string_view<char_type> date{};
};

using lc_identification=basic_lc_identification<char>;
using wlc_identification=basic_lc_identification<wchar_t>;
using u8lc_identification=basic_lc_identification<char8_t>;
using u16lc_identification=basic_lc_identification<char16_t>;
using u32lc_identification=basic_lc_identification<char32_t>;

template<std::integral char_type>
struct basic_lc_monetary
{
	std::basic_string_view<char_type> int_curr_symbol{};
	std::basic_string_view<char_type> currency_symbol{};
	std::basic_string_view<char_type> mon_decimal_point{};
	std::basic_string_view<char_type> mon_thousands_sep{};
	std::span<std::size_t> mon_grouping{};
	std::basic_string_view<char_type> positive_sign{};
	std::basic_string_view<char_type> negative_sign{};
	std::size_t int_frac_digits{};
	std::size_t frac_digits{};
	std::size_t p_cs_precedes{};
	std::size_t p_sep_by_space{};
	std::size_t n_cs_precedes{};
	std::size_t n_sep_by_space{};
	std::size_t int_p_cs_precedes{};
	std::size_t int_p_sep_by_space{};
	std::size_t int_n_cs_precedes{};
	std::size_t int_n_sep_by_space{};
	std::size_t p_sign_posn{};
	std::size_t n_sign_posn{};
	std::size_t int_p_sign_posn{};
	std::size_t int_n_sign_posn{};
};

using lc_monetary=basic_lc_monetary<char>;
using wlc_monetary=basic_lc_monetary<wchar_t>;
using u8lc_monetary=basic_lc_monetary<char8_t>;
using u16lc_monetary=basic_lc_monetary<char16_t>;
using u32lc_monetary=basic_lc_monetary<char32_t>;

template<std::integral char_type>
struct basic_lc_numeric
{
	std::basic_string_view<char_type> decimal_point{};
	std::basic_string_view<char_type> thousands_sep{};
	std::span<std::size_t> grouping{};
};

using lc_numeric=basic_lc_numeric<char>;
using wlc_numeric=basic_lc_numeric<wchar_t>;
using u8lc_numeric=basic_lc_numeric<char8_t>;
using u16lc_numeric=basic_lc_numeric<char16_t>;
using u32lc_numeric=basic_lc_numeric<char32_t>;

template<std::integral char_type>
struct basic_lc_time
{
	std::array<std::basic_string_view<char_type>,7> abday{};
	std::array<std::basic_string_view<char_type>,7> day{};
	std::array<std::basic_string_view<char_type>,12> abmon{};
	std::array<std::basic_string_view<char_type>,12> ab_alt_mon{};
	std::array<std::basic_string_view<char_type>,12> mon{};
	std::basic_string_view<char_type> d_t_fmt{};
	std::basic_string_view<char_type> d_fmt{};
	std::basic_string_view<char_type> t_fmt{};
	std::basic_string_view<char_type> t_fmt_ampm{};
	std::basic_string_view<char_type> date_fmt{};
	std::array<std::basic_string_view<char_type>,2> am_pm{};
	std::span<std::basic_string_view<char_type>> era{};
	std::basic_string_view<char_type> era_d_fmt{};
	std::basic_string_view<char_type> era_d_t_fmt{};
	std::basic_string_view<char_type> era_t_fmt{};
	std::span<std::basic_string_view<char_type>> alt_digits{};
	struct
	{
		std::size_t ndays{7};
		std::intmax_t first_day{19971201};
		std::size_t first_week{4};
	}week{};
	std::size_t first_weekday{};
	std::size_t first_workday{};
	std::size_t cal_direction{};
	std::span<std::basic_string_view<char_type>> timezone{};
};

using lc_time=basic_lc_time<char>;
using wlc_time=basic_lc_time<wchar_t>;
using u8lc_time=basic_lc_time<char8_t>;
using u16lc_time=basic_lc_time<char16_t>;
using u32lc_time=basic_lc_time<char32_t>;

template<std::integral char_type>
struct basic_lc_messages
{
	std::basic_string_view<char_type> yesexpr{};
	std::basic_string_view<char_type> noexpr{};
	std::basic_string_view<char_type> yesstr{};
	std::basic_string_view<char_type> nostr{};
};

using lc_messages=basic_lc_messages<char>;
using wlc_messages=basic_lc_messages<wchar_t>;
using u8lc_messages=basic_lc_messages<char8_t>;
using u16lc_messages=basic_lc_messages<char16_t>;
using u32lc_messages=basic_lc_messages<char32_t>;

template<std::integral char_type>
struct basic_lc_paper
{
	std::uintmax_t width{};
	std::uintmax_t height{};
};

using lc_paper=basic_lc_paper<char>;
using wlc_paper=basic_lc_paper<wchar_t>;
using u8lc_paper=basic_lc_paper<char8_t>;
using u16lc_paper=basic_lc_paper<char16_t>;
using u32lc_paper=basic_lc_paper<char32_t>;

template<std::integral char_type>
struct basic_lc_telephone
{
	std::basic_string_view<char_type> tel_int_fmt{};
	std::basic_string_view<char_type> tel_dom_fmt{};
	std::basic_string_view<char_type> int_select{};
	std::basic_string_view<char_type> int_prefix{};
};

using lc_telephone=basic_lc_telephone<char>;
using wlc_telephone=basic_lc_telephone<wchar_t>;
using u8lc_telephone=basic_lc_telephone<char8_t>;
using u16lc_telephone=basic_lc_telephone<char16_t>;
using u32lc_telephone=basic_lc_telephone<char32_t>;

template<std::integral char_type>
struct basic_lc_name
{
	std::basic_string_view<char_type> name_fmt{};
	std::basic_string_view<char_type> name_gen{};
	std::basic_string_view<char_type> name_miss{};
	std::basic_string_view<char_type> name_mr{};
	std::basic_string_view<char_type> name_mrs{};
	std::basic_string_view<char_type> name_ms{};
};

using lc_name=basic_lc_name<char>;
using wlc_name=basic_lc_name<wchar_t>;
using u8lc_name=basic_lc_name<char8_t>;
using u16lc_name=basic_lc_name<char16_t>;
using u32lc_name=basic_lc_name<char32_t>;

template<std::integral char_type>
struct basic_lc_address
{
	std::basic_string_view<char_type> postal_fmt{};
	std::basic_string_view<char_type> country_name{};
	std::basic_string_view<char_type> country_post{};
	std::basic_string_view<char_type> country_ab2{};
	std::basic_string_view<char_type> country_ab3{};
	std::uintmax_t country_num{};
	std::basic_string_view<char_type> country_car{};
	std::uintmax_t country_isbn{};
	std::basic_string_view<char_type> lang_name{};
	std::basic_string_view<char_type> lang_ab{};
	std::basic_string_view<char_type> lang_term{};
	std::basic_string_view<char_type> lang_lib{};
};

using lc_address=basic_lc_address<char>;
using wlc_address=basic_lc_address<wchar_t>;
using u8lc_address=basic_lc_address<char8_t>;
using u16lc_address=basic_lc_address<char16_t>;
using u32lc_address=basic_lc_address<char32_t>;

template<std::integral char_type>
struct basic_lc_measurement
{
	std::uintmax_t measurement{};
};

using lc_measurement=basic_lc_measurement<char>;
using wlc_measurement=basic_lc_measurement<wchar_t>;
using u8lc_measurement=basic_lc_measurement<char8_t>;
using u16lc_measurement=basic_lc_measurement<char16_t>;
using u32lc_measurement=basic_lc_measurement<char32_t>;

template<std::integral char_type>
struct basic_lc_all
{
	basic_lc_identification<char_type> identification{};
	basic_lc_monetary<char_type> monetary{};
	basic_lc_numeric<char_type> numeric{};
	basic_lc_time<char_type> time{};
	basic_lc_messages<char_type> messages{};
	basic_lc_paper<char_type> paper{};
	basic_lc_telephone<char_type> telephone{};
	basic_lc_name<char_type> name{};
	basic_lc_address<char_type> address{};
	basic_lc_measurement<char_type> measurement{};
};

using lc_all=basic_lc_all<char>;
using wlc_all=basic_lc_all<wchar_t>;
using u8lc_all=basic_lc_all<char8_t>;
using u16lc_all=basic_lc_all<char16_t>;
using u32lc_all=basic_lc_all<char32_t>;

struct lc_locale
{
	lc_all const* all{};
	wlc_all const* wall{};
	u8lc_all const* u8all{};
	u16lc_all const* u16all{};
	u32lc_all const* u32all{};
};

}