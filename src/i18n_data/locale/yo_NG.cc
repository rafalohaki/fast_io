#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Yoruba locale for Nigeria"),.source=tsc("fast_io"),.address=tsc("https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("pablo@mandriva.com;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Yoruba"),.territory=tsc("Nigeria"),.revision=tsc("0.2"),.date=tsc("2005-11-20")},.monetary={.int_curr_symbol=tsc("NGN "),.currency_symbol=tsc("₦"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Àìkú"),tsc("Ajé"),tsc("Ìsẹ́gun"),tsc("Ọjọ́rú"),tsc("Ọjọ́bọ"),tsc("Ẹtì"),tsc("Àbámẹ́ta")},.day={tsc("Ọjọ́ Àìkú"),tsc("Ọjọ́ Ajé"),tsc("Ọjọ́ Ìsẹ́gun"),tsc("Ọjọ́rú"),tsc("Ọjọ́bọ"),tsc("Ọjọ́ Ẹtì"),tsc("Ọjọ́ Àbámẹ́ta")},.abmon={tsc("Ṣẹ́rẹ́"),tsc("Èrèlè"),tsc("Ẹrẹ̀nà"),tsc("Ìgbé"),tsc("Ẹ̀bibi"),tsc("Òkúdu"),tsc("Agẹmọ"),tsc("Ògún"),tsc("Owewe"),tsc("Ọ̀wàrà"),tsc("Bélú"),tsc("Ọ̀pẹ̀")},.mon={tsc("Oṣù Ṣẹ́rẹ́"),tsc("Oṣù Èrèlè"),tsc("Oṣù Ẹrẹ̀nà"),tsc("Oṣù Ìgbé"),tsc("Oṣù Ẹ̀bibi"),tsc("Oṣù Òkúdu"),tsc("Oṣù Agẹmọ"),tsc("Oṣù Ògún"),tsc("Oṣù Owewe"),tsc("Oṣù Ọ̀wàrà"),tsc("Oṣù Bélú"),tsc("Oṣù Ọ̀pẹ̀")},.d_t_fmt=tsc("ọjọ́ %a, %d oṣù %b ọdún %Y %T"),.d_fmt=tsc("%d//%m//%y"),.t_fmt=tsc("%r"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("ọjọ́ %a, %d oṣù %b ọdún %Y %T %Z"),.am_pm={tsc("AM"),tsc("PM")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1EeyYNn]"),.noexpr=tsc("^[-0rROoKk]"),.yesstr=tsc("Bẹ́ẹ̀ni"),.nostr=tsc("Bẹ́ẹ̀kọ́")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("(%A) %l"),.int_select=tsc("009"),.int_prefix=tsc("234")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Orilẹ̀-èdè Nàìjíríà"),.country_ab2=tsc("NG"),.country_ab3=tsc("NGA"),.country_num=566,.country_car=tsc("WAN"),.country_isbn=tsc("978"),.lang_name=tsc("Èdè Yorùbá"),.lang_ab=tsc("yo"),.lang_term=tsc("yor"),.lang_lib=tsc("yor")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Yoruba locale for Nigeria"),.source=tsc(L"fast_io"),.address=tsc(L"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Yoruba"),.territory=tsc(L"Nigeria"),.revision=tsc(L"0.2"),.date=tsc(L"2005-11-20")},.monetary={.int_curr_symbol=tsc(L"NGN "),.currency_symbol=tsc(L"₦"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Àìkú"),tsc(L"Ajé"),tsc(L"Ìsẹ́gun"),tsc(L"Ọjọ́rú"),tsc(L"Ọjọ́bọ"),tsc(L"Ẹtì"),tsc(L"Àbámẹ́ta")},.day={tsc(L"Ọjọ́ Àìkú"),tsc(L"Ọjọ́ Ajé"),tsc(L"Ọjọ́ Ìsẹ́gun"),tsc(L"Ọjọ́rú"),tsc(L"Ọjọ́bọ"),tsc(L"Ọjọ́ Ẹtì"),tsc(L"Ọjọ́ Àbámẹ́ta")},.abmon={tsc(L"Ṣẹ́rẹ́"),tsc(L"Èrèlè"),tsc(L"Ẹrẹ̀nà"),tsc(L"Ìgbé"),tsc(L"Ẹ̀bibi"),tsc(L"Òkúdu"),tsc(L"Agẹmọ"),tsc(L"Ògún"),tsc(L"Owewe"),tsc(L"Ọ̀wàrà"),tsc(L"Bélú"),tsc(L"Ọ̀pẹ̀")},.mon={tsc(L"Oṣù Ṣẹ́rẹ́"),tsc(L"Oṣù Èrèlè"),tsc(L"Oṣù Ẹrẹ̀nà"),tsc(L"Oṣù Ìgbé"),tsc(L"Oṣù Ẹ̀bibi"),tsc(L"Oṣù Òkúdu"),tsc(L"Oṣù Agẹmọ"),tsc(L"Oṣù Ògún"),tsc(L"Oṣù Owewe"),tsc(L"Oṣù Ọ̀wàrà"),tsc(L"Oṣù Bélú"),tsc(L"Oṣù Ọ̀pẹ̀")},.d_t_fmt=tsc(L"ọjọ́ %a, %d oṣù %b ọdún %Y %T"),.d_fmt=tsc(L"%d//%m//%y"),.t_fmt=tsc(L"%r"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"ọjọ́ %a, %d oṣù %b ọdún %Y %T %Z"),.am_pm={tsc(L"AM"),tsc(L"PM")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1EeyYNn]"),.noexpr=tsc(L"^[-0rROoKk]"),.yesstr=tsc(L"Bẹ́ẹ̀ni"),.nostr=tsc(L"Bẹ́ẹ̀kọ́")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"(%A) %l"),.int_select=tsc(L"009"),.int_prefix=tsc(L"234")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Orilẹ̀-èdè Nàìjíríà"),.country_ab2=tsc(L"NG"),.country_ab3=tsc(L"NGA"),.country_num=566,.country_car=tsc(L"WAN"),.country_isbn=tsc(L"978"),.lang_name=tsc(L"Èdè Yorùbá"),.lang_ab=tsc(L"yo"),.lang_term=tsc(L"yor"),.lang_lib=tsc(L"yor")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Yoruba locale for Nigeria"),.source=tsc(u8"fast_io"),.address=tsc(u8"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Yoruba"),.territory=tsc(u8"Nigeria"),.revision=tsc(u8"0.2"),.date=tsc(u8"2005-11-20")},.monetary={.int_curr_symbol=tsc(u8"NGN "),.currency_symbol=tsc(u8"₦"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Àìkú"),tsc(u8"Ajé"),tsc(u8"Ìsẹ́gun"),tsc(u8"Ọjọ́rú"),tsc(u8"Ọjọ́bọ"),tsc(u8"Ẹtì"),tsc(u8"Àbámẹ́ta")},.day={tsc(u8"Ọjọ́ Àìkú"),tsc(u8"Ọjọ́ Ajé"),tsc(u8"Ọjọ́ Ìsẹ́gun"),tsc(u8"Ọjọ́rú"),tsc(u8"Ọjọ́bọ"),tsc(u8"Ọjọ́ Ẹtì"),tsc(u8"Ọjọ́ Àbámẹ́ta")},.abmon={tsc(u8"Ṣẹ́rẹ́"),tsc(u8"Èrèlè"),tsc(u8"Ẹrẹ̀nà"),tsc(u8"Ìgbé"),tsc(u8"Ẹ̀bibi"),tsc(u8"Òkúdu"),tsc(u8"Agẹmọ"),tsc(u8"Ògún"),tsc(u8"Owewe"),tsc(u8"Ọ̀wàrà"),tsc(u8"Bélú"),tsc(u8"Ọ̀pẹ̀")},.mon={tsc(u8"Oṣù Ṣẹ́rẹ́"),tsc(u8"Oṣù Èrèlè"),tsc(u8"Oṣù Ẹrẹ̀nà"),tsc(u8"Oṣù Ìgbé"),tsc(u8"Oṣù Ẹ̀bibi"),tsc(u8"Oṣù Òkúdu"),tsc(u8"Oṣù Agẹmọ"),tsc(u8"Oṣù Ògún"),tsc(u8"Oṣù Owewe"),tsc(u8"Oṣù Ọ̀wàrà"),tsc(u8"Oṣù Bélú"),tsc(u8"Oṣù Ọ̀pẹ̀")},.d_t_fmt=tsc(u8"ọjọ́ %a, %d oṣù %b ọdún %Y %T"),.d_fmt=tsc(u8"%d//%m//%y"),.t_fmt=tsc(u8"%r"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"ọjọ́ %a, %d oṣù %b ọdún %Y %T %Z"),.am_pm={tsc(u8"AM"),tsc(u8"PM")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1EeyYNn]"),.noexpr=tsc(u8"^[-0rROoKk]"),.yesstr=tsc(u8"Bẹ́ẹ̀ni"),.nostr=tsc(u8"Bẹ́ẹ̀kọ́")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"(%A) %l"),.int_select=tsc(u8"009"),.int_prefix=tsc(u8"234")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Orilẹ̀-èdè Nàìjíríà"),.country_ab2=tsc(u8"NG"),.country_ab3=tsc(u8"NGA"),.country_num=566,.country_car=tsc(u8"WAN"),.country_isbn=tsc(u8"978"),.lang_name=tsc(u8"Èdè Yorùbá"),.lang_ab=tsc(u8"yo"),.lang_term=tsc(u8"yor"),.lang_lib=tsc(u8"yor")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Yoruba locale for Nigeria"),.source=tsc(u"fast_io"),.address=tsc(u"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Yoruba"),.territory=tsc(u"Nigeria"),.revision=tsc(u"0.2"),.date=tsc(u"2005-11-20")},.monetary={.int_curr_symbol=tsc(u"NGN "),.currency_symbol=tsc(u"₦"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Àìkú"),tsc(u"Ajé"),tsc(u"Ìsẹ́gun"),tsc(u"Ọjọ́rú"),tsc(u"Ọjọ́bọ"),tsc(u"Ẹtì"),tsc(u"Àbámẹ́ta")},.day={tsc(u"Ọjọ́ Àìkú"),tsc(u"Ọjọ́ Ajé"),tsc(u"Ọjọ́ Ìsẹ́gun"),tsc(u"Ọjọ́rú"),tsc(u"Ọjọ́bọ"),tsc(u"Ọjọ́ Ẹtì"),tsc(u"Ọjọ́ Àbámẹ́ta")},.abmon={tsc(u"Ṣẹ́rẹ́"),tsc(u"Èrèlè"),tsc(u"Ẹrẹ̀nà"),tsc(u"Ìgbé"),tsc(u"Ẹ̀bibi"),tsc(u"Òkúdu"),tsc(u"Agẹmọ"),tsc(u"Ògún"),tsc(u"Owewe"),tsc(u"Ọ̀wàrà"),tsc(u"Bélú"),tsc(u"Ọ̀pẹ̀")},.mon={tsc(u"Oṣù Ṣẹ́rẹ́"),tsc(u"Oṣù Èrèlè"),tsc(u"Oṣù Ẹrẹ̀nà"),tsc(u"Oṣù Ìgbé"),tsc(u"Oṣù Ẹ̀bibi"),tsc(u"Oṣù Òkúdu"),tsc(u"Oṣù Agẹmọ"),tsc(u"Oṣù Ògún"),tsc(u"Oṣù Owewe"),tsc(u"Oṣù Ọ̀wàrà"),tsc(u"Oṣù Bélú"),tsc(u"Oṣù Ọ̀pẹ̀")},.d_t_fmt=tsc(u"ọjọ́ %a, %d oṣù %b ọdún %Y %T"),.d_fmt=tsc(u"%d//%m//%y"),.t_fmt=tsc(u"%r"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"ọjọ́ %a, %d oṣù %b ọdún %Y %T %Z"),.am_pm={tsc(u"AM"),tsc(u"PM")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1EeyYNn]"),.noexpr=tsc(u"^[-0rROoKk]"),.yesstr=tsc(u"Bẹ́ẹ̀ni"),.nostr=tsc(u"Bẹ́ẹ̀kọ́")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"(%A) %l"),.int_select=tsc(u"009"),.int_prefix=tsc(u"234")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Orilẹ̀-èdè Nàìjíríà"),.country_ab2=tsc(u"NG"),.country_ab3=tsc(u"NGA"),.country_num=566,.country_car=tsc(u"WAN"),.country_isbn=tsc(u"978"),.lang_name=tsc(u"Èdè Yorùbá"),.lang_ab=tsc(u"yo"),.lang_term=tsc(u"yor"),.lang_lib=tsc(u"yor")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Yoruba locale for Nigeria"),.source=tsc(U"fast_io"),.address=tsc(U"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Yoruba"),.territory=tsc(U"Nigeria"),.revision=tsc(U"0.2"),.date=tsc(U"2005-11-20")},.monetary={.int_curr_symbol=tsc(U"NGN "),.currency_symbol=tsc(U"₦"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Àìkú"),tsc(U"Ajé"),tsc(U"Ìsẹ́gun"),tsc(U"Ọjọ́rú"),tsc(U"Ọjọ́bọ"),tsc(U"Ẹtì"),tsc(U"Àbámẹ́ta")},.day={tsc(U"Ọjọ́ Àìkú"),tsc(U"Ọjọ́ Ajé"),tsc(U"Ọjọ́ Ìsẹ́gun"),tsc(U"Ọjọ́rú"),tsc(U"Ọjọ́bọ"),tsc(U"Ọjọ́ Ẹtì"),tsc(U"Ọjọ́ Àbámẹ́ta")},.abmon={tsc(U"Ṣẹ́rẹ́"),tsc(U"Èrèlè"),tsc(U"Ẹrẹ̀nà"),tsc(U"Ìgbé"),tsc(U"Ẹ̀bibi"),tsc(U"Òkúdu"),tsc(U"Agẹmọ"),tsc(U"Ògún"),tsc(U"Owewe"),tsc(U"Ọ̀wàrà"),tsc(U"Bélú"),tsc(U"Ọ̀pẹ̀")},.mon={tsc(U"Oṣù Ṣẹ́rẹ́"),tsc(U"Oṣù Èrèlè"),tsc(U"Oṣù Ẹrẹ̀nà"),tsc(U"Oṣù Ìgbé"),tsc(U"Oṣù Ẹ̀bibi"),tsc(U"Oṣù Òkúdu"),tsc(U"Oṣù Agẹmọ"),tsc(U"Oṣù Ògún"),tsc(U"Oṣù Owewe"),tsc(U"Oṣù Ọ̀wàrà"),tsc(U"Oṣù Bélú"),tsc(U"Oṣù Ọ̀pẹ̀")},.d_t_fmt=tsc(U"ọjọ́ %a, %d oṣù %b ọdún %Y %T"),.d_fmt=tsc(U"%d//%m//%y"),.t_fmt=tsc(U"%r"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"ọjọ́ %a, %d oṣù %b ọdún %Y %T %Z"),.am_pm={tsc(U"AM"),tsc(U"PM")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1EeyYNn]"),.noexpr=tsc(U"^[-0rROoKk]"),.yesstr=tsc(U"Bẹ́ẹ̀ni"),.nostr=tsc(U"Bẹ́ẹ̀kọ́")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"(%A) %l"),.int_select=tsc(U"009"),.int_prefix=tsc(U"234")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Orilẹ̀-èdè Nàìjíríà"),.country_ab2=tsc(U"NG"),.country_ab3=tsc(U"NGA"),.country_num=566,.country_car=tsc(U"WAN"),.country_isbn=tsc(U"978"),.lang_name=tsc(U"Èdè Yorùbá"),.lang_ab=tsc(U"yo"),.lang_term=tsc(U"yor"),.lang_lib=tsc(U"yor")},.measurement={.measurement=1}};


}
}

#include"../main.h"