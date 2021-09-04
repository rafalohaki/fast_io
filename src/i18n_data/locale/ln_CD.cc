#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Lingala locale for Democratic Republic of the Congo"),.source=tsc("Ubuntu\t\t;\t\tfast_io"),.address=tsc("Ubuntu DR Congo Team\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("René Manassé GALEKWA\t\t;\t\tfast_io"),.email=tsc("renemanasse@gmail.com;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Lingala"),.territory=tsc("Democratic Republic of the Congo"),.revision=tsc("1.1"),.date=tsc("2016-03-03")},.monetary={.int_curr_symbol=tsc("CDF "),.currency_symbol=tsc("FC"),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc("."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc(""),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("m1."),tsc("m2."),tsc("m3."),tsc("m4."),tsc("m5."),tsc("m6."),tsc("m7.")},.day={tsc("Lomíngo"),tsc("Mosálá mɔ̌kɔ́"),tsc("Misálá míbalé"),tsc("Misálá mísáto"),tsc("Misálá mínei"),tsc("Misálá mítáno"),tsc("Mpɔ́sɔ")},.abmon={tsc("yan"),tsc("fbl"),tsc("msi"),tsc("apl"),tsc("mai"),tsc("yun"),tsc("yul"),tsc("agt"),tsc("stb"),tsc("ɔtb"),tsc("nvb"),tsc("dsb")},.mon={tsc("sánzá ya yambo"),tsc("sánzá ya míbalé"),tsc("sánzá ya mísáto"),tsc("sánzá ya mínei"),tsc("sánzá ya mítáno"),tsc("sánzá ya motóbá"),tsc("sánzá ya nsambo"),tsc("sánzá ya mwambe"),tsc("sánzá ya libwa"),tsc("sánzá ya zómi"),tsc("sánzá ya zómi na mɔ̌kɔ́"),tsc("sánzá ya zómi na míbalé")},.d_t_fmt=tsc("%a %d %b %Y %T %Z"),.d_fmt=tsc("%d//%m//%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%A %-e %B %Y, %H:%M:%S (UTC%z)"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[yYiIíÍ]"),.noexpr=tsc("^[nNtT]"),.yesstr=tsc("Íyo"),.nostr=tsc("Tɛ̂")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a%t%l"),.tel_dom_fmt=tsc("%a %l"),.int_select=tsc("00"),.int_prefix=tsc("243")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f%m"),.name_gen=tsc("Mbɔ́tɛ"),.name_miss=tsc("Mamá"),.name_mr=tsc("Tatá"),.name_mrs=tsc("Mamá"),.name_ms=tsc("Mamá")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Repibiki demokratiki ya Kongó"),.country_post=tsc("CD"),.country_ab2=tsc("CD"),.country_ab3=tsc("COD"),.country_num=180,.country_car=tsc("CGO"),.country_isbn=tsc("2"),.lang_name=tsc("lingála"),.lang_ab=tsc("ln"),.lang_term=tsc("lin"),.lang_lib=tsc("lin")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Lingala locale for Democratic Republic of the Congo"),.source=tsc(L"Ubuntu\t\t;\t\tfast_io"),.address=tsc(L"Ubuntu DR Congo Team\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"René Manassé GALEKWA\t\t;\t\tfast_io"),.email=tsc(L"renemanasse@gmail.com;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Lingala"),.territory=tsc(L"Democratic Republic of the Congo"),.revision=tsc(L"1.1"),.date=tsc(L"2016-03-03")},.monetary={.int_curr_symbol=tsc(L"CDF "),.currency_symbol=tsc(L"FC"),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L""),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"m1."),tsc(L"m2."),tsc(L"m3."),tsc(L"m4."),tsc(L"m5."),tsc(L"m6."),tsc(L"m7.")},.day={tsc(L"Lomíngo"),tsc(L"Mosálá mɔ̌kɔ́"),tsc(L"Misálá míbalé"),tsc(L"Misálá mísáto"),tsc(L"Misálá mínei"),tsc(L"Misálá mítáno"),tsc(L"Mpɔ́sɔ")},.abmon={tsc(L"yan"),tsc(L"fbl"),tsc(L"msi"),tsc(L"apl"),tsc(L"mai"),tsc(L"yun"),tsc(L"yul"),tsc(L"agt"),tsc(L"stb"),tsc(L"ɔtb"),tsc(L"nvb"),tsc(L"dsb")},.mon={tsc(L"sánzá ya yambo"),tsc(L"sánzá ya míbalé"),tsc(L"sánzá ya mísáto"),tsc(L"sánzá ya mínei"),tsc(L"sánzá ya mítáno"),tsc(L"sánzá ya motóbá"),tsc(L"sánzá ya nsambo"),tsc(L"sánzá ya mwambe"),tsc(L"sánzá ya libwa"),tsc(L"sánzá ya zómi"),tsc(L"sánzá ya zómi na mɔ̌kɔ́"),tsc(L"sánzá ya zómi na míbalé")},.d_t_fmt=tsc(L"%a %d %b %Y %T %Z"),.d_fmt=tsc(L"%d//%m//%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%A %-e %B %Y, %H:%M:%S (UTC%z)"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[yYiIíÍ]"),.noexpr=tsc(L"^[nNtT]"),.yesstr=tsc(L"Íyo"),.nostr=tsc(L"Tɛ̂")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a%t%l"),.tel_dom_fmt=tsc(L"%a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"243")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f%m"),.name_gen=tsc(L"Mbɔ́tɛ"),.name_miss=tsc(L"Mamá"),.name_mr=tsc(L"Tatá"),.name_mrs=tsc(L"Mamá"),.name_ms=tsc(L"Mamá")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Repibiki demokratiki ya Kongó"),.country_post=tsc(L"CD"),.country_ab2=tsc(L"CD"),.country_ab3=tsc(L"COD"),.country_num=180,.country_car=tsc(L"CGO"),.country_isbn=tsc(L"2"),.lang_name=tsc(L"lingála"),.lang_ab=tsc(L"ln"),.lang_term=tsc(L"lin"),.lang_lib=tsc(L"lin")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Lingala locale for Democratic Republic of the Congo"),.source=tsc(u8"Ubuntu\t\t;\t\tfast_io"),.address=tsc(u8"Ubuntu DR Congo Team\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"René Manassé GALEKWA\t\t;\t\tfast_io"),.email=tsc(u8"renemanasse@gmail.com;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Lingala"),.territory=tsc(u8"Democratic Republic of the Congo"),.revision=tsc(u8"1.1"),.date=tsc(u8"2016-03-03")},.monetary={.int_curr_symbol=tsc(u8"CDF "),.currency_symbol=tsc(u8"FC"),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8""),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"m1."),tsc(u8"m2."),tsc(u8"m3."),tsc(u8"m4."),tsc(u8"m5."),tsc(u8"m6."),tsc(u8"m7.")},.day={tsc(u8"Lomíngo"),tsc(u8"Mosálá mɔ̌kɔ́"),tsc(u8"Misálá míbalé"),tsc(u8"Misálá mísáto"),tsc(u8"Misálá mínei"),tsc(u8"Misálá mítáno"),tsc(u8"Mpɔ́sɔ")},.abmon={tsc(u8"yan"),tsc(u8"fbl"),tsc(u8"msi"),tsc(u8"apl"),tsc(u8"mai"),tsc(u8"yun"),tsc(u8"yul"),tsc(u8"agt"),tsc(u8"stb"),tsc(u8"ɔtb"),tsc(u8"nvb"),tsc(u8"dsb")},.mon={tsc(u8"sánzá ya yambo"),tsc(u8"sánzá ya míbalé"),tsc(u8"sánzá ya mísáto"),tsc(u8"sánzá ya mínei"),tsc(u8"sánzá ya mítáno"),tsc(u8"sánzá ya motóbá"),tsc(u8"sánzá ya nsambo"),tsc(u8"sánzá ya mwambe"),tsc(u8"sánzá ya libwa"),tsc(u8"sánzá ya zómi"),tsc(u8"sánzá ya zómi na mɔ̌kɔ́"),tsc(u8"sánzá ya zómi na míbalé")},.d_t_fmt=tsc(u8"%a %d %b %Y %T %Z"),.d_fmt=tsc(u8"%d//%m//%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%A %-e %B %Y, %H:%M:%S (UTC%z)"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[yYiIíÍ]"),.noexpr=tsc(u8"^[nNtT]"),.yesstr=tsc(u8"Íyo"),.nostr=tsc(u8"Tɛ̂")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a%t%l"),.tel_dom_fmt=tsc(u8"%a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"243")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f%m"),.name_gen=tsc(u8"Mbɔ́tɛ"),.name_miss=tsc(u8"Mamá"),.name_mr=tsc(u8"Tatá"),.name_mrs=tsc(u8"Mamá"),.name_ms=tsc(u8"Mamá")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Repibiki demokratiki ya Kongó"),.country_post=tsc(u8"CD"),.country_ab2=tsc(u8"CD"),.country_ab3=tsc(u8"COD"),.country_num=180,.country_car=tsc(u8"CGO"),.country_isbn=tsc(u8"2"),.lang_name=tsc(u8"lingála"),.lang_ab=tsc(u8"ln"),.lang_term=tsc(u8"lin"),.lang_lib=tsc(u8"lin")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Lingala locale for Democratic Republic of the Congo"),.source=tsc(u"Ubuntu\t\t;\t\tfast_io"),.address=tsc(u"Ubuntu DR Congo Team\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"René Manassé GALEKWA\t\t;\t\tfast_io"),.email=tsc(u"renemanasse@gmail.com;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Lingala"),.territory=tsc(u"Democratic Republic of the Congo"),.revision=tsc(u"1.1"),.date=tsc(u"2016-03-03")},.monetary={.int_curr_symbol=tsc(u"CDF "),.currency_symbol=tsc(u"FC"),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u""),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"m1."),tsc(u"m2."),tsc(u"m3."),tsc(u"m4."),tsc(u"m5."),tsc(u"m6."),tsc(u"m7.")},.day={tsc(u"Lomíngo"),tsc(u"Mosálá mɔ̌kɔ́"),tsc(u"Misálá míbalé"),tsc(u"Misálá mísáto"),tsc(u"Misálá mínei"),tsc(u"Misálá mítáno"),tsc(u"Mpɔ́sɔ")},.abmon={tsc(u"yan"),tsc(u"fbl"),tsc(u"msi"),tsc(u"apl"),tsc(u"mai"),tsc(u"yun"),tsc(u"yul"),tsc(u"agt"),tsc(u"stb"),tsc(u"ɔtb"),tsc(u"nvb"),tsc(u"dsb")},.mon={tsc(u"sánzá ya yambo"),tsc(u"sánzá ya míbalé"),tsc(u"sánzá ya mísáto"),tsc(u"sánzá ya mínei"),tsc(u"sánzá ya mítáno"),tsc(u"sánzá ya motóbá"),tsc(u"sánzá ya nsambo"),tsc(u"sánzá ya mwambe"),tsc(u"sánzá ya libwa"),tsc(u"sánzá ya zómi"),tsc(u"sánzá ya zómi na mɔ̌kɔ́"),tsc(u"sánzá ya zómi na míbalé")},.d_t_fmt=tsc(u"%a %d %b %Y %T %Z"),.d_fmt=tsc(u"%d//%m//%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%A %-e %B %Y, %H:%M:%S (UTC%z)"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[yYiIíÍ]"),.noexpr=tsc(u"^[nNtT]"),.yesstr=tsc(u"Íyo"),.nostr=tsc(u"Tɛ̂")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a%t%l"),.tel_dom_fmt=tsc(u"%a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"243")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f%m"),.name_gen=tsc(u"Mbɔ́tɛ"),.name_miss=tsc(u"Mamá"),.name_mr=tsc(u"Tatá"),.name_mrs=tsc(u"Mamá"),.name_ms=tsc(u"Mamá")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Repibiki demokratiki ya Kongó"),.country_post=tsc(u"CD"),.country_ab2=tsc(u"CD"),.country_ab3=tsc(u"COD"),.country_num=180,.country_car=tsc(u"CGO"),.country_isbn=tsc(u"2"),.lang_name=tsc(u"lingála"),.lang_ab=tsc(u"ln"),.lang_term=tsc(u"lin"),.lang_lib=tsc(u"lin")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Lingala locale for Democratic Republic of the Congo"),.source=tsc(U"Ubuntu\t\t;\t\tfast_io"),.address=tsc(U"Ubuntu DR Congo Team\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"René Manassé GALEKWA\t\t;\t\tfast_io"),.email=tsc(U"renemanasse@gmail.com;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Lingala"),.territory=tsc(U"Democratic Republic of the Congo"),.revision=tsc(U"1.1"),.date=tsc(U"2016-03-03")},.monetary={.int_curr_symbol=tsc(U"CDF "),.currency_symbol=tsc(U"FC"),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U""),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"m1."),tsc(U"m2."),tsc(U"m3."),tsc(U"m4."),tsc(U"m5."),tsc(U"m6."),tsc(U"m7.")},.day={tsc(U"Lomíngo"),tsc(U"Mosálá mɔ̌kɔ́"),tsc(U"Misálá míbalé"),tsc(U"Misálá mísáto"),tsc(U"Misálá mínei"),tsc(U"Misálá mítáno"),tsc(U"Mpɔ́sɔ")},.abmon={tsc(U"yan"),tsc(U"fbl"),tsc(U"msi"),tsc(U"apl"),tsc(U"mai"),tsc(U"yun"),tsc(U"yul"),tsc(U"agt"),tsc(U"stb"),tsc(U"ɔtb"),tsc(U"nvb"),tsc(U"dsb")},.mon={tsc(U"sánzá ya yambo"),tsc(U"sánzá ya míbalé"),tsc(U"sánzá ya mísáto"),tsc(U"sánzá ya mínei"),tsc(U"sánzá ya mítáno"),tsc(U"sánzá ya motóbá"),tsc(U"sánzá ya nsambo"),tsc(U"sánzá ya mwambe"),tsc(U"sánzá ya libwa"),tsc(U"sánzá ya zómi"),tsc(U"sánzá ya zómi na mɔ̌kɔ́"),tsc(U"sánzá ya zómi na míbalé")},.d_t_fmt=tsc(U"%a %d %b %Y %T %Z"),.d_fmt=tsc(U"%d//%m//%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%A %-e %B %Y, %H:%M:%S (UTC%z)"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[yYiIíÍ]"),.noexpr=tsc(U"^[nNtT]"),.yesstr=tsc(U"Íyo"),.nostr=tsc(U"Tɛ̂")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a%t%l"),.tel_dom_fmt=tsc(U"%a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"243")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f%m"),.name_gen=tsc(U"Mbɔ́tɛ"),.name_miss=tsc(U"Mamá"),.name_mr=tsc(U"Tatá"),.name_mrs=tsc(U"Mamá"),.name_ms=tsc(U"Mamá")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Repibiki demokratiki ya Kongó"),.country_post=tsc(U"CD"),.country_ab2=tsc(U"CD"),.country_ab3=tsc(U"COD"),.country_num=180,.country_car=tsc(U"CGO"),.country_isbn=tsc(U"2"),.lang_name=tsc(U"lingála"),.lang_ab=tsc(U"ln"),.lang_term=tsc(U"lin"),.lang_lib=tsc(U"lin")},.measurement={.measurement=1}};


}
}

#include"../main.h"