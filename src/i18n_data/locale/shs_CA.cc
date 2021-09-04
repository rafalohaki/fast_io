#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Shuswap locale for Canada"),.source=tsc("Neskie Manuel\t\t;\t\tfast_io"),.address=tsc("745 Ska-Hiish Dr, Chase BC V0E 1M3\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Shuswap"),.territory=tsc("Canada"),.revision=tsc("1.0"),.date=tsc("2008-01-15")},.monetary={.int_curr_symbol=tsc("CAD "),.currency_symbol=tsc("$"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Sxe"),tsc("Spe"),tsc("Sel"),tsc("Ske"),tsc("Sme"),tsc("Sts"),tsc("Stq")},.day={tsc("Sxetspesq̓t"),tsc("Spetkesq̓t"),tsc("Selesq̓t"),tsc("Skellesq̓t"),tsc("Smesesq̓t"),tsc("Stselkstesq̓t"),tsc("Stqmekstesq̓t")},.abmon={tsc("Kwe"),tsc("Tsi"),tsc("Sqe"),tsc("Éwt"),tsc("Ell"),tsc("Tsp"),tsc("Tqw"),tsc("Ct̓é"),tsc("Qel"),tsc("Wél"),tsc("U7l"),tsc("Tet")},.mon={tsc("Pellkwet̓min"),tsc("Pelctsipwen̓ten"),tsc("Pellsqépts"),tsc("Peslléwten"),tsc("Pell7ell7é7llqten"),tsc("Pelltspéntsk"),tsc("Pelltqwelq̓wél̓t"),tsc("Pellct̓éxel̓cten"),tsc("Pesqelqlélten"),tsc("Pesllwélsten"),tsc("Pellc7ell7é7llcwten̓"),tsc("Pelltetétq̓em")},.d_t_fmt=tsc("%a %d %b %Y %r"),.d_fmt=tsc("%d//%m//%y"),.t_fmt=tsc("%r"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %d %b %Y %r %Z"),.am_pm={tsc("AM"),tsc("PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yYoO]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("yes"),.nostr=tsc("no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("011"),.int_prefix=tsc("1")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc("CA"),.country_ab3=tsc("CAN"),.country_num=124,.country_car=tsc("CDN"),.lang_name=tsc("Secwepemctsín"),.lang_term=tsc("shs"),.lang_lib=tsc("shs")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Shuswap locale for Canada"),.source=tsc(L"Neskie Manuel\t\t;\t\tfast_io"),.address=tsc(L"745 Ska-Hiish Dr, Chase BC V0E 1M3\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Shuswap"),.territory=tsc(L"Canada"),.revision=tsc(L"1.0"),.date=tsc(L"2008-01-15")},.monetary={.int_curr_symbol=tsc(L"CAD "),.currency_symbol=tsc(L"$"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Sxe"),tsc(L"Spe"),tsc(L"Sel"),tsc(L"Ske"),tsc(L"Sme"),tsc(L"Sts"),tsc(L"Stq")},.day={tsc(L"Sxetspesq̓t"),tsc(L"Spetkesq̓t"),tsc(L"Selesq̓t"),tsc(L"Skellesq̓t"),tsc(L"Smesesq̓t"),tsc(L"Stselkstesq̓t"),tsc(L"Stqmekstesq̓t")},.abmon={tsc(L"Kwe"),tsc(L"Tsi"),tsc(L"Sqe"),tsc(L"Éwt"),tsc(L"Ell"),tsc(L"Tsp"),tsc(L"Tqw"),tsc(L"Ct̓é"),tsc(L"Qel"),tsc(L"Wél"),tsc(L"U7l"),tsc(L"Tet")},.mon={tsc(L"Pellkwet̓min"),tsc(L"Pelctsipwen̓ten"),tsc(L"Pellsqépts"),tsc(L"Peslléwten"),tsc(L"Pell7ell7é7llqten"),tsc(L"Pelltspéntsk"),tsc(L"Pelltqwelq̓wél̓t"),tsc(L"Pellct̓éxel̓cten"),tsc(L"Pesqelqlélten"),tsc(L"Pesllwélsten"),tsc(L"Pellc7ell7é7llcwten̓"),tsc(L"Pelltetétq̓em")},.d_t_fmt=tsc(L"%a %d %b %Y %r"),.d_fmt=tsc(L"%d//%m//%y"),.t_fmt=tsc(L"%r"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %d %b %Y %r %Z"),.am_pm={tsc(L"AM"),tsc(L"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yYoO]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"yes"),.nostr=tsc(L"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"011"),.int_prefix=tsc(L"1")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(L"CA"),.country_ab3=tsc(L"CAN"),.country_num=124,.country_car=tsc(L"CDN"),.lang_name=tsc(L"Secwepemctsín"),.lang_term=tsc(L"shs"),.lang_lib=tsc(L"shs")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Shuswap locale for Canada"),.source=tsc(u8"Neskie Manuel\t\t;\t\tfast_io"),.address=tsc(u8"745 Ska-Hiish Dr, Chase BC V0E 1M3\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Shuswap"),.territory=tsc(u8"Canada"),.revision=tsc(u8"1.0"),.date=tsc(u8"2008-01-15")},.monetary={.int_curr_symbol=tsc(u8"CAD "),.currency_symbol=tsc(u8"$"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Sxe"),tsc(u8"Spe"),tsc(u8"Sel"),tsc(u8"Ske"),tsc(u8"Sme"),tsc(u8"Sts"),tsc(u8"Stq")},.day={tsc(u8"Sxetspesq̓t"),tsc(u8"Spetkesq̓t"),tsc(u8"Selesq̓t"),tsc(u8"Skellesq̓t"),tsc(u8"Smesesq̓t"),tsc(u8"Stselkstesq̓t"),tsc(u8"Stqmekstesq̓t")},.abmon={tsc(u8"Kwe"),tsc(u8"Tsi"),tsc(u8"Sqe"),tsc(u8"Éwt"),tsc(u8"Ell"),tsc(u8"Tsp"),tsc(u8"Tqw"),tsc(u8"Ct̓é"),tsc(u8"Qel"),tsc(u8"Wél"),tsc(u8"U7l"),tsc(u8"Tet")},.mon={tsc(u8"Pellkwet̓min"),tsc(u8"Pelctsipwen̓ten"),tsc(u8"Pellsqépts"),tsc(u8"Peslléwten"),tsc(u8"Pell7ell7é7llqten"),tsc(u8"Pelltspéntsk"),tsc(u8"Pelltqwelq̓wél̓t"),tsc(u8"Pellct̓éxel̓cten"),tsc(u8"Pesqelqlélten"),tsc(u8"Pesllwélsten"),tsc(u8"Pellc7ell7é7llcwten̓"),tsc(u8"Pelltetétq̓em")},.d_t_fmt=tsc(u8"%a %d %b %Y %r"),.d_fmt=tsc(u8"%d//%m//%y"),.t_fmt=tsc(u8"%r"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %d %b %Y %r %Z"),.am_pm={tsc(u8"AM"),tsc(u8"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yYoO]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"yes"),.nostr=tsc(u8"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"011"),.int_prefix=tsc(u8"1")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(u8"CA"),.country_ab3=tsc(u8"CAN"),.country_num=124,.country_car=tsc(u8"CDN"),.lang_name=tsc(u8"Secwepemctsín"),.lang_term=tsc(u8"shs"),.lang_lib=tsc(u8"shs")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Shuswap locale for Canada"),.source=tsc(u"Neskie Manuel\t\t;\t\tfast_io"),.address=tsc(u"745 Ska-Hiish Dr, Chase BC V0E 1M3\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Shuswap"),.territory=tsc(u"Canada"),.revision=tsc(u"1.0"),.date=tsc(u"2008-01-15")},.monetary={.int_curr_symbol=tsc(u"CAD "),.currency_symbol=tsc(u"$"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Sxe"),tsc(u"Spe"),tsc(u"Sel"),tsc(u"Ske"),tsc(u"Sme"),tsc(u"Sts"),tsc(u"Stq")},.day={tsc(u"Sxetspesq̓t"),tsc(u"Spetkesq̓t"),tsc(u"Selesq̓t"),tsc(u"Skellesq̓t"),tsc(u"Smesesq̓t"),tsc(u"Stselkstesq̓t"),tsc(u"Stqmekstesq̓t")},.abmon={tsc(u"Kwe"),tsc(u"Tsi"),tsc(u"Sqe"),tsc(u"Éwt"),tsc(u"Ell"),tsc(u"Tsp"),tsc(u"Tqw"),tsc(u"Ct̓é"),tsc(u"Qel"),tsc(u"Wél"),tsc(u"U7l"),tsc(u"Tet")},.mon={tsc(u"Pellkwet̓min"),tsc(u"Pelctsipwen̓ten"),tsc(u"Pellsqépts"),tsc(u"Peslléwten"),tsc(u"Pell7ell7é7llqten"),tsc(u"Pelltspéntsk"),tsc(u"Pelltqwelq̓wél̓t"),tsc(u"Pellct̓éxel̓cten"),tsc(u"Pesqelqlélten"),tsc(u"Pesllwélsten"),tsc(u"Pellc7ell7é7llcwten̓"),tsc(u"Pelltetétq̓em")},.d_t_fmt=tsc(u"%a %d %b %Y %r"),.d_fmt=tsc(u"%d//%m//%y"),.t_fmt=tsc(u"%r"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %d %b %Y %r %Z"),.am_pm={tsc(u"AM"),tsc(u"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yYoO]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"yes"),.nostr=tsc(u"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"011"),.int_prefix=tsc(u"1")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(u"CA"),.country_ab3=tsc(u"CAN"),.country_num=124,.country_car=tsc(u"CDN"),.lang_name=tsc(u"Secwepemctsín"),.lang_term=tsc(u"shs"),.lang_lib=tsc(u"shs")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Shuswap locale for Canada"),.source=tsc(U"Neskie Manuel\t\t;\t\tfast_io"),.address=tsc(U"745 Ska-Hiish Dr, Chase BC V0E 1M3\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Shuswap"),.territory=tsc(U"Canada"),.revision=tsc(U"1.0"),.date=tsc(U"2008-01-15")},.monetary={.int_curr_symbol=tsc(U"CAD "),.currency_symbol=tsc(U"$"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Sxe"),tsc(U"Spe"),tsc(U"Sel"),tsc(U"Ske"),tsc(U"Sme"),tsc(U"Sts"),tsc(U"Stq")},.day={tsc(U"Sxetspesq̓t"),tsc(U"Spetkesq̓t"),tsc(U"Selesq̓t"),tsc(U"Skellesq̓t"),tsc(U"Smesesq̓t"),tsc(U"Stselkstesq̓t"),tsc(U"Stqmekstesq̓t")},.abmon={tsc(U"Kwe"),tsc(U"Tsi"),tsc(U"Sqe"),tsc(U"Éwt"),tsc(U"Ell"),tsc(U"Tsp"),tsc(U"Tqw"),tsc(U"Ct̓é"),tsc(U"Qel"),tsc(U"Wél"),tsc(U"U7l"),tsc(U"Tet")},.mon={tsc(U"Pellkwet̓min"),tsc(U"Pelctsipwen̓ten"),tsc(U"Pellsqépts"),tsc(U"Peslléwten"),tsc(U"Pell7ell7é7llqten"),tsc(U"Pelltspéntsk"),tsc(U"Pelltqwelq̓wél̓t"),tsc(U"Pellct̓éxel̓cten"),tsc(U"Pesqelqlélten"),tsc(U"Pesllwélsten"),tsc(U"Pellc7ell7é7llcwten̓"),tsc(U"Pelltetétq̓em")},.d_t_fmt=tsc(U"%a %d %b %Y %r"),.d_fmt=tsc(U"%d//%m//%y"),.t_fmt=tsc(U"%r"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %d %b %Y %r %Z"),.am_pm={tsc(U"AM"),tsc(U"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yYoO]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"yes"),.nostr=tsc(U"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"011"),.int_prefix=tsc(U"1")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(U"CA"),.country_ab3=tsc(U"CAN"),.country_num=124,.country_car=tsc(U"CDN"),.lang_name=tsc(U"Secwepemctsín"),.lang_term=tsc(U"shs"),.lang_lib=tsc(U"shs")},.measurement={.measurement=1}};


}
}

#include"../main.h"