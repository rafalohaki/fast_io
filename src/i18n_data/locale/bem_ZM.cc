﻿#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Bemba locale for Zambia"),.source=tsc("ANLoc\t\t;\t\tfast_io"),.contact=tsc("Martin Benjamin\t\t;\t\tfast_io"),.email=tsc("locales@africanlocalization.net;euloanty@live.com"),.language=tsc("Bemba"),.territory=tsc("Zambia"),.revision=tsc("1.0"),.date=tsc("2011-04-18")},.monetary={.int_curr_symbol=tsc("ZMW "),.currency_symbol=tsc("K"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Sun"),tsc("Mon"),tsc("Tue"),tsc("Wed"),tsc("Thu"),tsc("Fri"),tsc("Sat")},.day={tsc("Pa Mulungu"),tsc("Palichimo"),tsc("Palichibuli"),tsc("Palichitatu"),tsc("Palichine"),tsc("Palichisano"),tsc("Pachibelushi")},.abmon={tsc("Jan"),tsc("Feb"),tsc("Mac"),tsc("Epr"),tsc("Mei"),tsc("Jun"),tsc("Jul"),tsc("Oga"),tsc("Sep"),tsc("Okt"),tsc("Nov"),tsc("Dis")},.mon={tsc("Januari"),tsc("Februari"),tsc("Machi"),tsc("Epreo"),tsc("Mei"),tsc("Juni"),tsc("Julai"),tsc("Ogasti"),tsc("Septemba"),tsc("Oktoba"),tsc("Novemba"),tsc("Disemba")},.d_t_fmt=tsc("%a %d %b %Y %R"),.d_fmt=tsc("%m//%d//%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %d %b %Y %R %Z"),.am_pm={tsc("uluchelo"),tsc("akasuba")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1yYeE]"),.noexpr=tsc("^[-0nNaA]"),.yesstr=tsc("Ee"),.nostr=tsc("Awe")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %A %l"),.tel_dom_fmt=tsc("%A %l"),.int_select=tsc("00"),.int_prefix=tsc("260")},.name={.name_fmt=tsc("%p%t%d1%g%t%M%f"),.name_gen=tsc("Ba"),.name_miss=tsc("Ba"),.name_mr=tsc("Ba"),.name_mrs=tsc("Bana"),.name_ms=tsc("Ba")},.address={.postal_fmt=tsc("%a%N%f%N%e %h%N%b %s%N%z%N%T%N%c%N"),.country_name=tsc("Zambia"),.country_post=tsc("ZM"),.country_ab2=tsc("ZM"),.country_ab3=tsc("ZMB"),.country_num=894,.country_car=tsc("Z"),.lang_name=tsc("Ichibemba"),.lang_term=tsc("bem"),.lang_lib=tsc("bem")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Bemba locale for Zambia"),.source=tsc(L"ANLoc\t\t;\t\tfast_io"),.contact=tsc(L"Martin Benjamin\t\t;\t\tfast_io"),.email=tsc(L"locales@africanlocalization.net;euloanty@live.com"),.language=tsc(L"Bemba"),.territory=tsc(L"Zambia"),.revision=tsc(L"1.0"),.date=tsc(L"2011-04-18")},.monetary={.int_curr_symbol=tsc(L"ZMW "),.currency_symbol=tsc(L"K"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Sun"),tsc(L"Mon"),tsc(L"Tue"),tsc(L"Wed"),tsc(L"Thu"),tsc(L"Fri"),tsc(L"Sat")},.day={tsc(L"Pa Mulungu"),tsc(L"Palichimo"),tsc(L"Palichibuli"),tsc(L"Palichitatu"),tsc(L"Palichine"),tsc(L"Palichisano"),tsc(L"Pachibelushi")},.abmon={tsc(L"Jan"),tsc(L"Feb"),tsc(L"Mac"),tsc(L"Epr"),tsc(L"Mei"),tsc(L"Jun"),tsc(L"Jul"),tsc(L"Oga"),tsc(L"Sep"),tsc(L"Okt"),tsc(L"Nov"),tsc(L"Dis")},.mon={tsc(L"Januari"),tsc(L"Februari"),tsc(L"Machi"),tsc(L"Epreo"),tsc(L"Mei"),tsc(L"Juni"),tsc(L"Julai"),tsc(L"Ogasti"),tsc(L"Septemba"),tsc(L"Oktoba"),tsc(L"Novemba"),tsc(L"Disemba")},.d_t_fmt=tsc(L"%a %d %b %Y %R"),.d_fmt=tsc(L"%m//%d//%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %d %b %Y %R %Z"),.am_pm={tsc(L"uluchelo"),tsc(L"akasuba")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1yYeE]"),.noexpr=tsc(L"^[-0nNaA]"),.yesstr=tsc(L"Ee"),.nostr=tsc(L"Awe")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %A %l"),.tel_dom_fmt=tsc(L"%A %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"260")},.name={.name_fmt=tsc(L"%p%t%d1%g%t%M%f"),.name_gen=tsc(L"Ba"),.name_miss=tsc(L"Ba"),.name_mr=tsc(L"Ba"),.name_mrs=tsc(L"Bana"),.name_ms=tsc(L"Ba")},.address={.postal_fmt=tsc(L"%a%N%f%N%e %h%N%b %s%N%z%N%T%N%c%N"),.country_name=tsc(L"Zambia"),.country_post=tsc(L"ZM"),.country_ab2=tsc(L"ZM"),.country_ab3=tsc(L"ZMB"),.country_num=894,.country_car=tsc(L"Z"),.lang_name=tsc(L"Ichibemba"),.lang_term=tsc(L"bem"),.lang_lib=tsc(L"bem")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Bemba locale for Zambia"),.source=tsc(u8"ANLoc\t\t;\t\tfast_io"),.contact=tsc(u8"Martin Benjamin\t\t;\t\tfast_io"),.email=tsc(u8"locales@africanlocalization.net;euloanty@live.com"),.language=tsc(u8"Bemba"),.territory=tsc(u8"Zambia"),.revision=tsc(u8"1.0"),.date=tsc(u8"2011-04-18")},.monetary={.int_curr_symbol=tsc(u8"ZMW "),.currency_symbol=tsc(u8"K"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Sun"),tsc(u8"Mon"),tsc(u8"Tue"),tsc(u8"Wed"),tsc(u8"Thu"),tsc(u8"Fri"),tsc(u8"Sat")},.day={tsc(u8"Pa Mulungu"),tsc(u8"Palichimo"),tsc(u8"Palichibuli"),tsc(u8"Palichitatu"),tsc(u8"Palichine"),tsc(u8"Palichisano"),tsc(u8"Pachibelushi")},.abmon={tsc(u8"Jan"),tsc(u8"Feb"),tsc(u8"Mac"),tsc(u8"Epr"),tsc(u8"Mei"),tsc(u8"Jun"),tsc(u8"Jul"),tsc(u8"Oga"),tsc(u8"Sep"),tsc(u8"Okt"),tsc(u8"Nov"),tsc(u8"Dis")},.mon={tsc(u8"Januari"),tsc(u8"Februari"),tsc(u8"Machi"),tsc(u8"Epreo"),tsc(u8"Mei"),tsc(u8"Juni"),tsc(u8"Julai"),tsc(u8"Ogasti"),tsc(u8"Septemba"),tsc(u8"Oktoba"),tsc(u8"Novemba"),tsc(u8"Disemba")},.d_t_fmt=tsc(u8"%a %d %b %Y %R"),.d_fmt=tsc(u8"%m//%d//%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %d %b %Y %R %Z"),.am_pm={tsc(u8"uluchelo"),tsc(u8"akasuba")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1yYeE]"),.noexpr=tsc(u8"^[-0nNaA]"),.yesstr=tsc(u8"Ee"),.nostr=tsc(u8"Awe")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %A %l"),.tel_dom_fmt=tsc(u8"%A %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"260")},.name={.name_fmt=tsc(u8"%p%t%d1%g%t%M%f"),.name_gen=tsc(u8"Ba"),.name_miss=tsc(u8"Ba"),.name_mr=tsc(u8"Ba"),.name_mrs=tsc(u8"Bana"),.name_ms=tsc(u8"Ba")},.address={.postal_fmt=tsc(u8"%a%N%f%N%e %h%N%b %s%N%z%N%T%N%c%N"),.country_name=tsc(u8"Zambia"),.country_post=tsc(u8"ZM"),.country_ab2=tsc(u8"ZM"),.country_ab3=tsc(u8"ZMB"),.country_num=894,.country_car=tsc(u8"Z"),.lang_name=tsc(u8"Ichibemba"),.lang_term=tsc(u8"bem"),.lang_lib=tsc(u8"bem")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Bemba locale for Zambia"),.source=tsc(u"ANLoc\t\t;\t\tfast_io"),.contact=tsc(u"Martin Benjamin\t\t;\t\tfast_io"),.email=tsc(u"locales@africanlocalization.net;euloanty@live.com"),.language=tsc(u"Bemba"),.territory=tsc(u"Zambia"),.revision=tsc(u"1.0"),.date=tsc(u"2011-04-18")},.monetary={.int_curr_symbol=tsc(u"ZMW "),.currency_symbol=tsc(u"K"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Sun"),tsc(u"Mon"),tsc(u"Tue"),tsc(u"Wed"),tsc(u"Thu"),tsc(u"Fri"),tsc(u"Sat")},.day={tsc(u"Pa Mulungu"),tsc(u"Palichimo"),tsc(u"Palichibuli"),tsc(u"Palichitatu"),tsc(u"Palichine"),tsc(u"Palichisano"),tsc(u"Pachibelushi")},.abmon={tsc(u"Jan"),tsc(u"Feb"),tsc(u"Mac"),tsc(u"Epr"),tsc(u"Mei"),tsc(u"Jun"),tsc(u"Jul"),tsc(u"Oga"),tsc(u"Sep"),tsc(u"Okt"),tsc(u"Nov"),tsc(u"Dis")},.mon={tsc(u"Januari"),tsc(u"Februari"),tsc(u"Machi"),tsc(u"Epreo"),tsc(u"Mei"),tsc(u"Juni"),tsc(u"Julai"),tsc(u"Ogasti"),tsc(u"Septemba"),tsc(u"Oktoba"),tsc(u"Novemba"),tsc(u"Disemba")},.d_t_fmt=tsc(u"%a %d %b %Y %R"),.d_fmt=tsc(u"%m//%d//%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %d %b %Y %R %Z"),.am_pm={tsc(u"uluchelo"),tsc(u"akasuba")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1yYeE]"),.noexpr=tsc(u"^[-0nNaA]"),.yesstr=tsc(u"Ee"),.nostr=tsc(u"Awe")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %A %l"),.tel_dom_fmt=tsc(u"%A %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"260")},.name={.name_fmt=tsc(u"%p%t%d1%g%t%M%f"),.name_gen=tsc(u"Ba"),.name_miss=tsc(u"Ba"),.name_mr=tsc(u"Ba"),.name_mrs=tsc(u"Bana"),.name_ms=tsc(u"Ba")},.address={.postal_fmt=tsc(u"%a%N%f%N%e %h%N%b %s%N%z%N%T%N%c%N"),.country_name=tsc(u"Zambia"),.country_post=tsc(u"ZM"),.country_ab2=tsc(u"ZM"),.country_ab3=tsc(u"ZMB"),.country_num=894,.country_car=tsc(u"Z"),.lang_name=tsc(u"Ichibemba"),.lang_term=tsc(u"bem"),.lang_lib=tsc(u"bem")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Bemba locale for Zambia"),.source=tsc(U"ANLoc\t\t;\t\tfast_io"),.contact=tsc(U"Martin Benjamin\t\t;\t\tfast_io"),.email=tsc(U"locales@africanlocalization.net;euloanty@live.com"),.language=tsc(U"Bemba"),.territory=tsc(U"Zambia"),.revision=tsc(U"1.0"),.date=tsc(U"2011-04-18")},.monetary={.int_curr_symbol=tsc(U"ZMW "),.currency_symbol=tsc(U"K"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Sun"),tsc(U"Mon"),tsc(U"Tue"),tsc(U"Wed"),tsc(U"Thu"),tsc(U"Fri"),tsc(U"Sat")},.day={tsc(U"Pa Mulungu"),tsc(U"Palichimo"),tsc(U"Palichibuli"),tsc(U"Palichitatu"),tsc(U"Palichine"),tsc(U"Palichisano"),tsc(U"Pachibelushi")},.abmon={tsc(U"Jan"),tsc(U"Feb"),tsc(U"Mac"),tsc(U"Epr"),tsc(U"Mei"),tsc(U"Jun"),tsc(U"Jul"),tsc(U"Oga"),tsc(U"Sep"),tsc(U"Okt"),tsc(U"Nov"),tsc(U"Dis")},.mon={tsc(U"Januari"),tsc(U"Februari"),tsc(U"Machi"),tsc(U"Epreo"),tsc(U"Mei"),tsc(U"Juni"),tsc(U"Julai"),tsc(U"Ogasti"),tsc(U"Septemba"),tsc(U"Oktoba"),tsc(U"Novemba"),tsc(U"Disemba")},.d_t_fmt=tsc(U"%a %d %b %Y %R"),.d_fmt=tsc(U"%m//%d//%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %d %b %Y %R %Z"),.am_pm={tsc(U"uluchelo"),tsc(U"akasuba")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1yYeE]"),.noexpr=tsc(U"^[-0nNaA]"),.yesstr=tsc(U"Ee"),.nostr=tsc(U"Awe")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %A %l"),.tel_dom_fmt=tsc(U"%A %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"260")},.name={.name_fmt=tsc(U"%p%t%d1%g%t%M%f"),.name_gen=tsc(U"Ba"),.name_miss=tsc(U"Ba"),.name_mr=tsc(U"Ba"),.name_mrs=tsc(U"Bana"),.name_ms=tsc(U"Ba")},.address={.postal_fmt=tsc(U"%a%N%f%N%e %h%N%b %s%N%z%N%T%N%c%N"),.country_name=tsc(U"Zambia"),.country_post=tsc(U"ZM"),.country_ab2=tsc(U"ZM"),.country_ab3=tsc(U"ZMB"),.country_num=894,.country_car=tsc(U"Z"),.lang_name=tsc(U"Ichibemba"),.lang_term=tsc(U"bem"),.lang_lib=tsc(U"bem")},.measurement={.measurement=1}};


}
}

#include"../main.h"