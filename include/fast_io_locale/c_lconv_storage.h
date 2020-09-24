#pragma once

namespace fast_io
{

template<std::integral ch_type,std::ranges::contiguous_range stg_type=std::vector<ch_type>>
struct basic_c_lconv_storage:public basic_lconv_storage<ch_type,stg_type>
{
	using char_type = ch_type;
	using storage_type = stg_type;
	using iterator = typename storage_type::iterator;
	using string_view_type = std::basic_string_view<char_type>;
	constexpr basic_c_lconv_storage()=default;
	basic_c_lconv_storage(c_locale_observer clob)
	{
#ifdef _MSC_VER
		auto& lcv{*(reinterpret_cast<win32::__crt_locale_data*>(clob.native_handle()->locinfo)->lconv)};
#elif defined(_WIN32)
		auto& lcv{*(clob.native_handle()->locinfo->lconv)};
#else
		c_locale_thread_local_guard guard(clob);
		auto& lcv{*localeconv()};
#endif
		this->decimal_point=*lcv.decimal_point;
		this->thousands_sep=*lcv.thousands_sep;

		this->mon_decimal_point=*lcv.mon_decimal_point;
		this->mon_thousands_sep=*lcv.mon_thousands_sep;

		this->frac_digits=lcv.frac_digits;

		this->p_cs_precedes=lcv.p_cs_precedes;
		this->n_cs_precedes=lcv.n_cs_precedes;
		this->p_sep_by_space=lcv.p_sep_by_space;
		this->n_sep_by_space=lcv.n_sep_by_space;
		this->p_sign_posn=lcv.p_sign_posn;
		this->n_sign_posn=lcv.n_sign_posn;

		this->int_frac_digits=lcv.int_frac_digits;
#ifndef _WIN32
		this->int_p_cs_precedes=lcv.int_p_cs_precedes;
		this->int_n_cs_precedes=lcv.int_n_cs_precedes;
		this->int_p_sep_by_space=lcv.int_p_sep_by_space;
		this->int_n_sep_by_space=lcv.int_n_sep_by_space;
		this->int_p_sign_posn=lcv.int_p_sign_posn;
		this->int_n_sign_posn=lcv.int_n_sign_posn;
#endif
		auto& stog{this->storage};
		auto insert_lmd([&](char const* itemptr)
		{
			std::size_t const size{std::strlen(itemptr)};
			stog.insert(stog.end(),itemptr,itemptr+size);
			return stog.size();
		});
		this->mon_grouping_start=insert_lmd(lcv.grouping);
		this->positive_sign_start=insert_lmd(lcv.mon_grouping);
		this->negative_sign_start=insert_lmd(lcv.positive_sign);
		this->currency_symbol_start=insert_lmd(lcv.negative_sign);
		this->int_curr_symbol_start=insert_lmd(lcv.currency_symbol);
		insert_lmd(lcv.int_curr_symbol);
	}
};

using c_lconv_storage = basic_c_lconv_storage<char>;
using wc_lconv_storage = basic_c_lconv_storage<wchar_t>;
using u8c_lconv_storage = basic_c_lconv_storage<char8_t>;

}