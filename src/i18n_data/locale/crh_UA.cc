#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Crimean Tatar (Crimean Turkish) language locale for Ukraine"),.source=tsc("fast_io"),.address=tsc("https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("Reşat SABIQ\t\t;\t\tfast_io"),.email=tsc("tilde.birlik@gmail.com;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Crimean Tatar"),.territory=tsc("Ukraine"),.revision=tsc("0.4"),.date=tsc("2009-08-16")},.monetary={.int_curr_symbol=tsc("UAH "),.currency_symbol=tsc("₴"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc("."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Baz"),tsc("Ber"),tsc("Sal"),tsc("Çar"),tsc("Caq"),tsc("Cum"),tsc("Cer")},.day={tsc("Bazar"),tsc("Bazarertesi"),tsc("Salı"),tsc("Çarşembe"),tsc("Cumaaqşamı"),tsc("Cuma"),tsc("Cumaertesi")},.abmon={tsc("Yan"),tsc("Fev"),tsc("Mar"),tsc("Apr"),tsc("May"),tsc("İyn"),tsc("İyl"),tsc("Avg"),tsc("Sen"),tsc("Okt"),tsc("Noy"),tsc("Dek")},.mon={tsc("Yanvar"),tsc("Fevral"),tsc("Mart"),tsc("Aprel"),tsc("Mayıs"),tsc("İyun"),tsc("İyul"),tsc("Avgust"),tsc("Sentâbr"),tsc("Oktâbr"),tsc("Noyabr"),tsc("Dekabr")},.d_t_fmt=tsc("%a %d %b %Y %T"),.d_fmt=tsc("%d.%m.%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %d %b %Y %T %Z"),.am_pm={tsc("ÜE"),tsc("ÜS")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1yYeE]"),.noexpr=tsc("^[-0nNhH]"),.yesstr=tsc("ebet"),.nostr=tsc("hayır")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00"),.int_prefix=tsc("380")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc("UA"),.country_ab3=tsc("UKR"),.country_num=804,.country_car=tsc("UA"),.lang_name=tsc("qırımtatar tili"),.lang_term=tsc("crh"),.lang_lib=tsc("crh")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Crimean Tatar (Crimean Turkish) language locale for Ukraine"),.source=tsc(L"fast_io"),.address=tsc(L"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"Reşat SABIQ\t\t;\t\tfast_io"),.email=tsc(L"tilde.birlik@gmail.com;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Crimean Tatar"),.territory=tsc(L"Ukraine"),.revision=tsc(L"0.4"),.date=tsc(L"2009-08-16")},.monetary={.int_curr_symbol=tsc(L"UAH "),.currency_symbol=tsc(L"₴"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Baz"),tsc(L"Ber"),tsc(L"Sal"),tsc(L"Çar"),tsc(L"Caq"),tsc(L"Cum"),tsc(L"Cer")},.day={tsc(L"Bazar"),tsc(L"Bazarertesi"),tsc(L"Salı"),tsc(L"Çarşembe"),tsc(L"Cumaaqşamı"),tsc(L"Cuma"),tsc(L"Cumaertesi")},.abmon={tsc(L"Yan"),tsc(L"Fev"),tsc(L"Mar"),tsc(L"Apr"),tsc(L"May"),tsc(L"İyn"),tsc(L"İyl"),tsc(L"Avg"),tsc(L"Sen"),tsc(L"Okt"),tsc(L"Noy"),tsc(L"Dek")},.mon={tsc(L"Yanvar"),tsc(L"Fevral"),tsc(L"Mart"),tsc(L"Aprel"),tsc(L"Mayıs"),tsc(L"İyun"),tsc(L"İyul"),tsc(L"Avgust"),tsc(L"Sentâbr"),tsc(L"Oktâbr"),tsc(L"Noyabr"),tsc(L"Dekabr")},.d_t_fmt=tsc(L"%a %d %b %Y %T"),.d_fmt=tsc(L"%d.%m.%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %d %b %Y %T %Z"),.am_pm={tsc(L"ÜE"),tsc(L"ÜS")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1yYeE]"),.noexpr=tsc(L"^[-0nNhH]"),.yesstr=tsc(L"ebet"),.nostr=tsc(L"hayır")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"380")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(L"UA"),.country_ab3=tsc(L"UKR"),.country_num=804,.country_car=tsc(L"UA"),.lang_name=tsc(L"qırımtatar tili"),.lang_term=tsc(L"crh"),.lang_lib=tsc(L"crh")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Crimean Tatar (Crimean Turkish) language locale for Ukraine"),.source=tsc(u8"fast_io"),.address=tsc(u8"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"Reşat SABIQ\t\t;\t\tfast_io"),.email=tsc(u8"tilde.birlik@gmail.com;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Crimean Tatar"),.territory=tsc(u8"Ukraine"),.revision=tsc(u8"0.4"),.date=tsc(u8"2009-08-16")},.monetary={.int_curr_symbol=tsc(u8"UAH "),.currency_symbol=tsc(u8"₴"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Baz"),tsc(u8"Ber"),tsc(u8"Sal"),tsc(u8"Çar"),tsc(u8"Caq"),tsc(u8"Cum"),tsc(u8"Cer")},.day={tsc(u8"Bazar"),tsc(u8"Bazarertesi"),tsc(u8"Salı"),tsc(u8"Çarşembe"),tsc(u8"Cumaaqşamı"),tsc(u8"Cuma"),tsc(u8"Cumaertesi")},.abmon={tsc(u8"Yan"),tsc(u8"Fev"),tsc(u8"Mar"),tsc(u8"Apr"),tsc(u8"May"),tsc(u8"İyn"),tsc(u8"İyl"),tsc(u8"Avg"),tsc(u8"Sen"),tsc(u8"Okt"),tsc(u8"Noy"),tsc(u8"Dek")},.mon={tsc(u8"Yanvar"),tsc(u8"Fevral"),tsc(u8"Mart"),tsc(u8"Aprel"),tsc(u8"Mayıs"),tsc(u8"İyun"),tsc(u8"İyul"),tsc(u8"Avgust"),tsc(u8"Sentâbr"),tsc(u8"Oktâbr"),tsc(u8"Noyabr"),tsc(u8"Dekabr")},.d_t_fmt=tsc(u8"%a %d %b %Y %T"),.d_fmt=tsc(u8"%d.%m.%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %d %b %Y %T %Z"),.am_pm={tsc(u8"ÜE"),tsc(u8"ÜS")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1yYeE]"),.noexpr=tsc(u8"^[-0nNhH]"),.yesstr=tsc(u8"ebet"),.nostr=tsc(u8"hayır")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"380")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(u8"UA"),.country_ab3=tsc(u8"UKR"),.country_num=804,.country_car=tsc(u8"UA"),.lang_name=tsc(u8"qırımtatar tili"),.lang_term=tsc(u8"crh"),.lang_lib=tsc(u8"crh")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Crimean Tatar (Crimean Turkish) language locale for Ukraine"),.source=tsc(u"fast_io"),.address=tsc(u"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"Reşat SABIQ\t\t;\t\tfast_io"),.email=tsc(u"tilde.birlik@gmail.com;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Crimean Tatar"),.territory=tsc(u"Ukraine"),.revision=tsc(u"0.4"),.date=tsc(u"2009-08-16")},.monetary={.int_curr_symbol=tsc(u"UAH "),.currency_symbol=tsc(u"₴"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Baz"),tsc(u"Ber"),tsc(u"Sal"),tsc(u"Çar"),tsc(u"Caq"),tsc(u"Cum"),tsc(u"Cer")},.day={tsc(u"Bazar"),tsc(u"Bazarertesi"),tsc(u"Salı"),tsc(u"Çarşembe"),tsc(u"Cumaaqşamı"),tsc(u"Cuma"),tsc(u"Cumaertesi")},.abmon={tsc(u"Yan"),tsc(u"Fev"),tsc(u"Mar"),tsc(u"Apr"),tsc(u"May"),tsc(u"İyn"),tsc(u"İyl"),tsc(u"Avg"),tsc(u"Sen"),tsc(u"Okt"),tsc(u"Noy"),tsc(u"Dek")},.mon={tsc(u"Yanvar"),tsc(u"Fevral"),tsc(u"Mart"),tsc(u"Aprel"),tsc(u"Mayıs"),tsc(u"İyun"),tsc(u"İyul"),tsc(u"Avgust"),tsc(u"Sentâbr"),tsc(u"Oktâbr"),tsc(u"Noyabr"),tsc(u"Dekabr")},.d_t_fmt=tsc(u"%a %d %b %Y %T"),.d_fmt=tsc(u"%d.%m.%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %d %b %Y %T %Z"),.am_pm={tsc(u"ÜE"),tsc(u"ÜS")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1yYeE]"),.noexpr=tsc(u"^[-0nNhH]"),.yesstr=tsc(u"ebet"),.nostr=tsc(u"hayır")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"380")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(u"UA"),.country_ab3=tsc(u"UKR"),.country_num=804,.country_car=tsc(u"UA"),.lang_name=tsc(u"qırımtatar tili"),.lang_term=tsc(u"crh"),.lang_lib=tsc(u"crh")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Crimean Tatar (Crimean Turkish) language locale for Ukraine"),.source=tsc(U"fast_io"),.address=tsc(U"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"Reşat SABIQ\t\t;\t\tfast_io"),.email=tsc(U"tilde.birlik@gmail.com;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Crimean Tatar"),.territory=tsc(U"Ukraine"),.revision=tsc(U"0.4"),.date=tsc(U"2009-08-16")},.monetary={.int_curr_symbol=tsc(U"UAH "),.currency_symbol=tsc(U"₴"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Baz"),tsc(U"Ber"),tsc(U"Sal"),tsc(U"Çar"),tsc(U"Caq"),tsc(U"Cum"),tsc(U"Cer")},.day={tsc(U"Bazar"),tsc(U"Bazarertesi"),tsc(U"Salı"),tsc(U"Çarşembe"),tsc(U"Cumaaqşamı"),tsc(U"Cuma"),tsc(U"Cumaertesi")},.abmon={tsc(U"Yan"),tsc(U"Fev"),tsc(U"Mar"),tsc(U"Apr"),tsc(U"May"),tsc(U"İyn"),tsc(U"İyl"),tsc(U"Avg"),tsc(U"Sen"),tsc(U"Okt"),tsc(U"Noy"),tsc(U"Dek")},.mon={tsc(U"Yanvar"),tsc(U"Fevral"),tsc(U"Mart"),tsc(U"Aprel"),tsc(U"Mayıs"),tsc(U"İyun"),tsc(U"İyul"),tsc(U"Avgust"),tsc(U"Sentâbr"),tsc(U"Oktâbr"),tsc(U"Noyabr"),tsc(U"Dekabr")},.d_t_fmt=tsc(U"%a %d %b %Y %T"),.d_fmt=tsc(U"%d.%m.%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %d %b %Y %T %Z"),.am_pm={tsc(U"ÜE"),tsc(U"ÜS")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1yYeE]"),.noexpr=tsc(U"^[-0nNhH]"),.yesstr=tsc(U"ebet"),.nostr=tsc(U"hayır")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"380")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(U"UA"),.country_ab3=tsc(U"UKR"),.country_num=804,.country_car=tsc(U"UA"),.lang_name=tsc(U"qırımtatar tili"),.lang_term=tsc(U"crh"),.lang_lib=tsc(U"crh")},.measurement={.measurement=1}};


}
}

#include"../main.h"