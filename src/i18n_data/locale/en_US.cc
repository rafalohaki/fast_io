﻿#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("English locale for the USA"),.source=tsc("Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc("https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("American English"),.territory=tsc("United States"),.revision=tsc("1.0"),.date=tsc("2000-06-24")},.monetary={.int_curr_symbol=tsc("USD "),.currency_symbol=tsc("$"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Sun"),tsc("Mon"),tsc("Tue"),tsc("Wed"),tsc("Thu"),tsc("Fri"),tsc("Sat")},.day={tsc("Sunday"),tsc("Monday"),tsc("Tuesday"),tsc("Wednesday"),tsc("Thursday"),tsc("Friday"),tsc("Saturday")},.abmon={tsc("Jan"),tsc("Feb"),tsc("Mar"),tsc("Apr"),tsc("May"),tsc("Jun"),tsc("Jul"),tsc("Aug"),tsc("Sep"),tsc("Oct"),tsc("Nov"),tsc("Dec")},.mon={tsc("January"),tsc("February"),tsc("March"),tsc("April"),tsc("May"),tsc("June"),tsc("July"),tsc("August"),tsc("September"),tsc("October"),tsc("November"),tsc("December")},.d_t_fmt=tsc("%a %d %b %Y %r %Z"),.d_fmt=tsc("%m//%d//%Y"),.t_fmt=tsc("%r"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %b %e %r %Z %Y"),.am_pm={tsc("AM"),tsc("PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yY]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("yes"),.nostr=tsc("no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc("+%c (%a) %l"),.tel_dom_fmt=tsc("(%a) %l"),.int_select=tsc("11"),.int_prefix=tsc("1")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f"),.name_miss=tsc("Miss."),.name_mr=tsc("Mr."),.name_mrs=tsc("Mrs."),.name_ms=tsc("Ms.")},.address={.postal_fmt=tsc("%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc("United States"),.country_post=tsc("USA"),.country_ab2=tsc("US"),.country_ab3=tsc("USA"),.country_num=840,.country_car=tsc("USA"),.country_isbn=tsc("0"),.lang_name=tsc("English"),.lang_ab=tsc("en"),.lang_term=tsc("eng"),.lang_lib=tsc("eng")},.measurement={.measurement=2}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"English locale for the USA"),.source=tsc(L"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(L"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"American English"),.territory=tsc(L"United States"),.revision=tsc(L"1.0"),.date=tsc(L"2000-06-24")},.monetary={.int_curr_symbol=tsc(L"USD "),.currency_symbol=tsc(L"$"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Sun"),tsc(L"Mon"),tsc(L"Tue"),tsc(L"Wed"),tsc(L"Thu"),tsc(L"Fri"),tsc(L"Sat")},.day={tsc(L"Sunday"),tsc(L"Monday"),tsc(L"Tuesday"),tsc(L"Wednesday"),tsc(L"Thursday"),tsc(L"Friday"),tsc(L"Saturday")},.abmon={tsc(L"Jan"),tsc(L"Feb"),tsc(L"Mar"),tsc(L"Apr"),tsc(L"May"),tsc(L"Jun"),tsc(L"Jul"),tsc(L"Aug"),tsc(L"Sep"),tsc(L"Oct"),tsc(L"Nov"),tsc(L"Dec")},.mon={tsc(L"January"),tsc(L"February"),tsc(L"March"),tsc(L"April"),tsc(L"May"),tsc(L"June"),tsc(L"July"),tsc(L"August"),tsc(L"September"),tsc(L"October"),tsc(L"November"),tsc(L"December")},.d_t_fmt=tsc(L"%a %d %b %Y %r %Z"),.d_fmt=tsc(L"%m//%d//%Y"),.t_fmt=tsc(L"%r"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %b %e %r %Z %Y"),.am_pm={tsc(L"AM"),tsc(L"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yY]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"yes"),.nostr=tsc(L"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(L"+%c (%a) %l"),.tel_dom_fmt=tsc(L"(%a) %l"),.int_select=tsc(L"11"),.int_prefix=tsc(L"1")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f"),.name_miss=tsc(L"Miss."),.name_mr=tsc(L"Mr."),.name_mrs=tsc(L"Mrs."),.name_ms=tsc(L"Ms.")},.address={.postal_fmt=tsc(L"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(L"United States"),.country_post=tsc(L"USA"),.country_ab2=tsc(L"US"),.country_ab3=tsc(L"USA"),.country_num=840,.country_car=tsc(L"USA"),.country_isbn=tsc(L"0"),.lang_name=tsc(L"English"),.lang_ab=tsc(L"en"),.lang_term=tsc(L"eng"),.lang_lib=tsc(L"eng")},.measurement={.measurement=2}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"English locale for the USA"),.source=tsc(u8"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u8"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"American English"),.territory=tsc(u8"United States"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-06-24")},.monetary={.int_curr_symbol=tsc(u8"USD "),.currency_symbol=tsc(u8"$"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Sun"),tsc(u8"Mon"),tsc(u8"Tue"),tsc(u8"Wed"),tsc(u8"Thu"),tsc(u8"Fri"),tsc(u8"Sat")},.day={tsc(u8"Sunday"),tsc(u8"Monday"),tsc(u8"Tuesday"),tsc(u8"Wednesday"),tsc(u8"Thursday"),tsc(u8"Friday"),tsc(u8"Saturday")},.abmon={tsc(u8"Jan"),tsc(u8"Feb"),tsc(u8"Mar"),tsc(u8"Apr"),tsc(u8"May"),tsc(u8"Jun"),tsc(u8"Jul"),tsc(u8"Aug"),tsc(u8"Sep"),tsc(u8"Oct"),tsc(u8"Nov"),tsc(u8"Dec")},.mon={tsc(u8"January"),tsc(u8"February"),tsc(u8"March"),tsc(u8"April"),tsc(u8"May"),tsc(u8"June"),tsc(u8"July"),tsc(u8"August"),tsc(u8"September"),tsc(u8"October"),tsc(u8"November"),tsc(u8"December")},.d_t_fmt=tsc(u8"%a %d %b %Y %r %Z"),.d_fmt=tsc(u8"%m//%d//%Y"),.t_fmt=tsc(u8"%r"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %b %e %r %Z %Y"),.am_pm={tsc(u8"AM"),tsc(u8"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yY]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"yes"),.nostr=tsc(u8"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u8"+%c (%a) %l"),.tel_dom_fmt=tsc(u8"(%a) %l"),.int_select=tsc(u8"11"),.int_prefix=tsc(u8"1")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f"),.name_miss=tsc(u8"Miss."),.name_mr=tsc(u8"Mr."),.name_mrs=tsc(u8"Mrs."),.name_ms=tsc(u8"Ms.")},.address={.postal_fmt=tsc(u8"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(u8"United States"),.country_post=tsc(u8"USA"),.country_ab2=tsc(u8"US"),.country_ab3=tsc(u8"USA"),.country_num=840,.country_car=tsc(u8"USA"),.country_isbn=tsc(u8"0"),.lang_name=tsc(u8"English"),.lang_ab=tsc(u8"en"),.lang_term=tsc(u8"eng"),.lang_lib=tsc(u8"eng")},.measurement={.measurement=2}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"English locale for the USA"),.source=tsc(u"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"American English"),.territory=tsc(u"United States"),.revision=tsc(u"1.0"),.date=tsc(u"2000-06-24")},.monetary={.int_curr_symbol=tsc(u"USD "),.currency_symbol=tsc(u"$"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Sun"),tsc(u"Mon"),tsc(u"Tue"),tsc(u"Wed"),tsc(u"Thu"),tsc(u"Fri"),tsc(u"Sat")},.day={tsc(u"Sunday"),tsc(u"Monday"),tsc(u"Tuesday"),tsc(u"Wednesday"),tsc(u"Thursday"),tsc(u"Friday"),tsc(u"Saturday")},.abmon={tsc(u"Jan"),tsc(u"Feb"),tsc(u"Mar"),tsc(u"Apr"),tsc(u"May"),tsc(u"Jun"),tsc(u"Jul"),tsc(u"Aug"),tsc(u"Sep"),tsc(u"Oct"),tsc(u"Nov"),tsc(u"Dec")},.mon={tsc(u"January"),tsc(u"February"),tsc(u"March"),tsc(u"April"),tsc(u"May"),tsc(u"June"),tsc(u"July"),tsc(u"August"),tsc(u"September"),tsc(u"October"),tsc(u"November"),tsc(u"December")},.d_t_fmt=tsc(u"%a %d %b %Y %r %Z"),.d_fmt=tsc(u"%m//%d//%Y"),.t_fmt=tsc(u"%r"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %b %e %r %Z %Y"),.am_pm={tsc(u"AM"),tsc(u"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yY]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"yes"),.nostr=tsc(u"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u"+%c (%a) %l"),.tel_dom_fmt=tsc(u"(%a) %l"),.int_select=tsc(u"11"),.int_prefix=tsc(u"1")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f"),.name_miss=tsc(u"Miss."),.name_mr=tsc(u"Mr."),.name_mrs=tsc(u"Mrs."),.name_ms=tsc(u"Ms.")},.address={.postal_fmt=tsc(u"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(u"United States"),.country_post=tsc(u"USA"),.country_ab2=tsc(u"US"),.country_ab3=tsc(u"USA"),.country_num=840,.country_car=tsc(u"USA"),.country_isbn=tsc(u"0"),.lang_name=tsc(u"English"),.lang_ab=tsc(u"en"),.lang_term=tsc(u"eng"),.lang_lib=tsc(u"eng")},.measurement={.measurement=2}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"English locale for the USA"),.source=tsc(U"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(U"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"American English"),.territory=tsc(U"United States"),.revision=tsc(U"1.0"),.date=tsc(U"2000-06-24")},.monetary={.int_curr_symbol=tsc(U"USD "),.currency_symbol=tsc(U"$"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Sun"),tsc(U"Mon"),tsc(U"Tue"),tsc(U"Wed"),tsc(U"Thu"),tsc(U"Fri"),tsc(U"Sat")},.day={tsc(U"Sunday"),tsc(U"Monday"),tsc(U"Tuesday"),tsc(U"Wednesday"),tsc(U"Thursday"),tsc(U"Friday"),tsc(U"Saturday")},.abmon={tsc(U"Jan"),tsc(U"Feb"),tsc(U"Mar"),tsc(U"Apr"),tsc(U"May"),tsc(U"Jun"),tsc(U"Jul"),tsc(U"Aug"),tsc(U"Sep"),tsc(U"Oct"),tsc(U"Nov"),tsc(U"Dec")},.mon={tsc(U"January"),tsc(U"February"),tsc(U"March"),tsc(U"April"),tsc(U"May"),tsc(U"June"),tsc(U"July"),tsc(U"August"),tsc(U"September"),tsc(U"October"),tsc(U"November"),tsc(U"December")},.d_t_fmt=tsc(U"%a %d %b %Y %r %Z"),.d_fmt=tsc(U"%m//%d//%Y"),.t_fmt=tsc(U"%r"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %b %e %r %Z %Y"),.am_pm={tsc(U"AM"),tsc(U"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yY]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"yes"),.nostr=tsc(U"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(U"+%c (%a) %l"),.tel_dom_fmt=tsc(U"(%a) %l"),.int_select=tsc(U"11"),.int_prefix=tsc(U"1")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f"),.name_miss=tsc(U"Miss."),.name_mr=tsc(U"Mr."),.name_mrs=tsc(U"Mrs."),.name_ms=tsc(U"Ms.")},.address={.postal_fmt=tsc(U"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(U"United States"),.country_post=tsc(U"USA"),.country_ab2=tsc(U"US"),.country_ab3=tsc(U"USA"),.country_num=840,.country_car=tsc(U"USA"),.country_isbn=tsc(U"0"),.lang_name=tsc(U"English"),.lang_ab=tsc(U"en"),.lang_term=tsc(U"eng"),.lang_lib=tsc(U"eng")},.measurement={.measurement=2}};


}
}

#include"../main.h"