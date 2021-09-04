#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Ukrainian Language Locale for Ukraine"),.source=tsc("fast_io"),.address=tsc("https://github.com/expnkx/fast_io"),.contact=tsc("GNU libc maintainers\t\t;\t\tfast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Ukrainian"),.territory=tsc("Ukraine"),.revision=tsc("2.1.12"),.date=tsc("2006-05-20")},.monetary={.int_curr_symbol=tsc("UAH "),.currency_symbol=tsc("грн."),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc(" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=2,.n_cs_precedes=0,.n_sep_by_space=1,.int_p_cs_precedes=1,.int_p_sep_by_space=2,.int_n_cs_precedes=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1,.int_p_sign_posn=4,.int_n_sign_posn=4},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc(" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("нд"),tsc("пн"),tsc("вт"),tsc("ср"),tsc("чт"),tsc("пт"),tsc("сб")},.day={tsc("неділя"),tsc("понеділок"),tsc("вівторок"),tsc("середа"),tsc("четвер"),tsc("п'ятниця"),tsc("субота")},.abmon={tsc("січ"),tsc("лют"),tsc("бер"),tsc("кві"),tsc("тра"),tsc("чер"),tsc("лип"),tsc("сер"),tsc("вер"),tsc("жов"),tsc("лис"),tsc("гру")},.mon={tsc("січня"),tsc("лютого"),tsc("березня"),tsc("квітня"),tsc("травня"),tsc("червня"),tsc("липня"),tsc("серпня"),tsc("вересня"),tsc("жовтня"),tsc("листопада"),tsc("грудня")},.d_t_fmt=tsc("%a, %d-%b-%Y %X %z"),.d_fmt=tsc("%d.%m.%y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%A, %-d %B %Y %X %z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=2,.cal_direction=2},.messages={.yesexpr=tsc("^([+1Yy]|[Тт][Аа][Кк]?)$"),.noexpr=tsc("^([-0Nn]|[Нн][Іі])$"),.yesstr=tsc("так"),.nostr=tsc("ні")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("(%A) %l"),.int_select=tsc("8~10"),.int_prefix=tsc("380")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f"),.name_gen=tsc("шановний(-на)"),.name_miss=tsc("панна"),.name_mr=tsc("пан"),.name_mrs=tsc("пані"),.name_ms=tsc("пані")},.address={.postal_fmt=tsc("%a%t%f%t%d%N%s%t%h%t, %b%t%e%t%r%N%T%t%S%N%z%N%c"),.country_name=tsc("Україна"),.country_post=tsc("UA"),.country_ab2=tsc("UA"),.country_ab3=tsc("UKR"),.country_num=804,.country_car=tsc("UA"),.country_isbn=tsc("966"),.lang_name=tsc("українська"),.lang_ab=tsc("uk"),.lang_term=tsc("ukr"),.lang_lib=tsc("ukr")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Ukrainian Language Locale for Ukraine"),.source=tsc(L"fast_io"),.address=tsc(L"https://github.com/expnkx/fast_io"),.contact=tsc(L"GNU libc maintainers\t\t;\t\tfast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Ukrainian"),.territory=tsc(L"Ukraine"),.revision=tsc(L"2.1.12"),.date=tsc(L"2006-05-20")},.monetary={.int_curr_symbol=tsc(L"UAH "),.currency_symbol=tsc(L"грн."),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=2,.n_cs_precedes=0,.n_sep_by_space=1,.int_p_cs_precedes=1,.int_p_sep_by_space=2,.int_n_cs_precedes=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1,.int_p_sign_posn=4,.int_n_sign_posn=4},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"нд"),tsc(L"пн"),tsc(L"вт"),tsc(L"ср"),tsc(L"чт"),tsc(L"пт"),tsc(L"сб")},.day={tsc(L"неділя"),tsc(L"понеділок"),tsc(L"вівторок"),tsc(L"середа"),tsc(L"четвер"),tsc(L"п'ятниця"),tsc(L"субота")},.abmon={tsc(L"січ"),tsc(L"лют"),tsc(L"бер"),tsc(L"кві"),tsc(L"тра"),tsc(L"чер"),tsc(L"лип"),tsc(L"сер"),tsc(L"вер"),tsc(L"жов"),tsc(L"лис"),tsc(L"гру")},.mon={tsc(L"січня"),tsc(L"лютого"),tsc(L"березня"),tsc(L"квітня"),tsc(L"травня"),tsc(L"червня"),tsc(L"липня"),tsc(L"серпня"),tsc(L"вересня"),tsc(L"жовтня"),tsc(L"листопада"),tsc(L"грудня")},.d_t_fmt=tsc(L"%a, %d-%b-%Y %X %z"),.d_fmt=tsc(L"%d.%m.%y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%A, %-d %B %Y %X %z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=2,.cal_direction=2},.messages={.yesexpr=tsc(L"^([+1Yy]|[Тт][Аа][Кк]?)$"),.noexpr=tsc(L"^([-0Nn]|[Нн][Іі])$"),.yesstr=tsc(L"так"),.nostr=tsc(L"ні")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"(%A) %l"),.int_select=tsc(L"8~10"),.int_prefix=tsc(L"380")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f"),.name_gen=tsc(L"шановний(-на)"),.name_miss=tsc(L"панна"),.name_mr=tsc(L"пан"),.name_mrs=tsc(L"пані"),.name_ms=tsc(L"пані")},.address={.postal_fmt=tsc(L"%a%t%f%t%d%N%s%t%h%t, %b%t%e%t%r%N%T%t%S%N%z%N%c"),.country_name=tsc(L"Україна"),.country_post=tsc(L"UA"),.country_ab2=tsc(L"UA"),.country_ab3=tsc(L"UKR"),.country_num=804,.country_car=tsc(L"UA"),.country_isbn=tsc(L"966"),.lang_name=tsc(L"українська"),.lang_ab=tsc(L"uk"),.lang_term=tsc(L"ukr"),.lang_lib=tsc(L"ukr")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Ukrainian Language Locale for Ukraine"),.source=tsc(u8"fast_io"),.address=tsc(u8"https://github.com/expnkx/fast_io"),.contact=tsc(u8"GNU libc maintainers\t\t;\t\tfast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Ukrainian"),.territory=tsc(u8"Ukraine"),.revision=tsc(u8"2.1.12"),.date=tsc(u8"2006-05-20")},.monetary={.int_curr_symbol=tsc(u8"UAH "),.currency_symbol=tsc(u8"грн."),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=2,.n_cs_precedes=0,.n_sep_by_space=1,.int_p_cs_precedes=1,.int_p_sep_by_space=2,.int_n_cs_precedes=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1,.int_p_sign_posn=4,.int_n_sign_posn=4},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"нд"),tsc(u8"пн"),tsc(u8"вт"),tsc(u8"ср"),tsc(u8"чт"),tsc(u8"пт"),tsc(u8"сб")},.day={tsc(u8"неділя"),tsc(u8"понеділок"),tsc(u8"вівторок"),tsc(u8"середа"),tsc(u8"четвер"),tsc(u8"п'ятниця"),tsc(u8"субота")},.abmon={tsc(u8"січ"),tsc(u8"лют"),tsc(u8"бер"),tsc(u8"кві"),tsc(u8"тра"),tsc(u8"чер"),tsc(u8"лип"),tsc(u8"сер"),tsc(u8"вер"),tsc(u8"жов"),tsc(u8"лис"),tsc(u8"гру")},.mon={tsc(u8"січня"),tsc(u8"лютого"),tsc(u8"березня"),tsc(u8"квітня"),tsc(u8"травня"),tsc(u8"червня"),tsc(u8"липня"),tsc(u8"серпня"),tsc(u8"вересня"),tsc(u8"жовтня"),tsc(u8"листопада"),tsc(u8"грудня")},.d_t_fmt=tsc(u8"%a, %d-%b-%Y %X %z"),.d_fmt=tsc(u8"%d.%m.%y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%A, %-d %B %Y %X %z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=2,.cal_direction=2},.messages={.yesexpr=tsc(u8"^([+1Yy]|[Тт][Аа][Кк]?)$"),.noexpr=tsc(u8"^([-0Nn]|[Нн][Іі])$"),.yesstr=tsc(u8"так"),.nostr=tsc(u8"ні")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"(%A) %l"),.int_select=tsc(u8"8~10"),.int_prefix=tsc(u8"380")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f"),.name_gen=tsc(u8"шановний(-на)"),.name_miss=tsc(u8"панна"),.name_mr=tsc(u8"пан"),.name_mrs=tsc(u8"пані"),.name_ms=tsc(u8"пані")},.address={.postal_fmt=tsc(u8"%a%t%f%t%d%N%s%t%h%t, %b%t%e%t%r%N%T%t%S%N%z%N%c"),.country_name=tsc(u8"Україна"),.country_post=tsc(u8"UA"),.country_ab2=tsc(u8"UA"),.country_ab3=tsc(u8"UKR"),.country_num=804,.country_car=tsc(u8"UA"),.country_isbn=tsc(u8"966"),.lang_name=tsc(u8"українська"),.lang_ab=tsc(u8"uk"),.lang_term=tsc(u8"ukr"),.lang_lib=tsc(u8"ukr")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Ukrainian Language Locale for Ukraine"),.source=tsc(u"fast_io"),.address=tsc(u"https://github.com/expnkx/fast_io"),.contact=tsc(u"GNU libc maintainers\t\t;\t\tfast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Ukrainian"),.territory=tsc(u"Ukraine"),.revision=tsc(u"2.1.12"),.date=tsc(u"2006-05-20")},.monetary={.int_curr_symbol=tsc(u"UAH "),.currency_symbol=tsc(u"грн."),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=2,.n_cs_precedes=0,.n_sep_by_space=1,.int_p_cs_precedes=1,.int_p_sep_by_space=2,.int_n_cs_precedes=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1,.int_p_sign_posn=4,.int_n_sign_posn=4},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"нд"),tsc(u"пн"),tsc(u"вт"),tsc(u"ср"),tsc(u"чт"),tsc(u"пт"),tsc(u"сб")},.day={tsc(u"неділя"),tsc(u"понеділок"),tsc(u"вівторок"),tsc(u"середа"),tsc(u"четвер"),tsc(u"п'ятниця"),tsc(u"субота")},.abmon={tsc(u"січ"),tsc(u"лют"),tsc(u"бер"),tsc(u"кві"),tsc(u"тра"),tsc(u"чер"),tsc(u"лип"),tsc(u"сер"),tsc(u"вер"),tsc(u"жов"),tsc(u"лис"),tsc(u"гру")},.mon={tsc(u"січня"),tsc(u"лютого"),tsc(u"березня"),tsc(u"квітня"),tsc(u"травня"),tsc(u"червня"),tsc(u"липня"),tsc(u"серпня"),tsc(u"вересня"),tsc(u"жовтня"),tsc(u"листопада"),tsc(u"грудня")},.d_t_fmt=tsc(u"%a, %d-%b-%Y %X %z"),.d_fmt=tsc(u"%d.%m.%y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%A, %-d %B %Y %X %z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=2,.cal_direction=2},.messages={.yesexpr=tsc(u"^([+1Yy]|[Тт][Аа][Кк]?)$"),.noexpr=tsc(u"^([-0Nn]|[Нн][Іі])$"),.yesstr=tsc(u"так"),.nostr=tsc(u"ні")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"(%A) %l"),.int_select=tsc(u"8~10"),.int_prefix=tsc(u"380")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f"),.name_gen=tsc(u"шановний(-на)"),.name_miss=tsc(u"панна"),.name_mr=tsc(u"пан"),.name_mrs=tsc(u"пані"),.name_ms=tsc(u"пані")},.address={.postal_fmt=tsc(u"%a%t%f%t%d%N%s%t%h%t, %b%t%e%t%r%N%T%t%S%N%z%N%c"),.country_name=tsc(u"Україна"),.country_post=tsc(u"UA"),.country_ab2=tsc(u"UA"),.country_ab3=tsc(u"UKR"),.country_num=804,.country_car=tsc(u"UA"),.country_isbn=tsc(u"966"),.lang_name=tsc(u"українська"),.lang_ab=tsc(u"uk"),.lang_term=tsc(u"ukr"),.lang_lib=tsc(u"ukr")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Ukrainian Language Locale for Ukraine"),.source=tsc(U"fast_io"),.address=tsc(U"https://github.com/expnkx/fast_io"),.contact=tsc(U"GNU libc maintainers\t\t;\t\tfast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Ukrainian"),.territory=tsc(U"Ukraine"),.revision=tsc(U"2.1.12"),.date=tsc(U"2006-05-20")},.monetary={.int_curr_symbol=tsc(U"UAH "),.currency_symbol=tsc(U"грн."),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=2,.n_cs_precedes=0,.n_sep_by_space=1,.int_p_cs_precedes=1,.int_p_sep_by_space=2,.int_n_cs_precedes=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1,.int_p_sign_posn=4,.int_n_sign_posn=4},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"нд"),tsc(U"пн"),tsc(U"вт"),tsc(U"ср"),tsc(U"чт"),tsc(U"пт"),tsc(U"сб")},.day={tsc(U"неділя"),tsc(U"понеділок"),tsc(U"вівторок"),tsc(U"середа"),tsc(U"четвер"),tsc(U"п'ятниця"),tsc(U"субота")},.abmon={tsc(U"січ"),tsc(U"лют"),tsc(U"бер"),tsc(U"кві"),tsc(U"тра"),tsc(U"чер"),tsc(U"лип"),tsc(U"сер"),tsc(U"вер"),tsc(U"жов"),tsc(U"лис"),tsc(U"гру")},.mon={tsc(U"січня"),tsc(U"лютого"),tsc(U"березня"),tsc(U"квітня"),tsc(U"травня"),tsc(U"червня"),tsc(U"липня"),tsc(U"серпня"),tsc(U"вересня"),tsc(U"жовтня"),tsc(U"листопада"),tsc(U"грудня")},.d_t_fmt=tsc(U"%a, %d-%b-%Y %X %z"),.d_fmt=tsc(U"%d.%m.%y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%A, %-d %B %Y %X %z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=2,.cal_direction=2},.messages={.yesexpr=tsc(U"^([+1Yy]|[Тт][Аа][Кк]?)$"),.noexpr=tsc(U"^([-0Nn]|[Нн][Іі])$"),.yesstr=tsc(U"так"),.nostr=tsc(U"ні")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"(%A) %l"),.int_select=tsc(U"8~10"),.int_prefix=tsc(U"380")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f"),.name_gen=tsc(U"шановний(-на)"),.name_miss=tsc(U"панна"),.name_mr=tsc(U"пан"),.name_mrs=tsc(U"пані"),.name_ms=tsc(U"пані")},.address={.postal_fmt=tsc(U"%a%t%f%t%d%N%s%t%h%t, %b%t%e%t%r%N%T%t%S%N%z%N%c"),.country_name=tsc(U"Україна"),.country_post=tsc(U"UA"),.country_ab2=tsc(U"UA"),.country_ab3=tsc(U"UKR"),.country_num=804,.country_car=tsc(U"UA"),.country_isbn=tsc(U"966"),.lang_name=tsc(U"українська"),.lang_ab=tsc(U"uk"),.lang_term=tsc(U"ukr"),.lang_lib=tsc(U"ukr")},.measurement={.measurement=1}};


}
}

#include"../main.h"