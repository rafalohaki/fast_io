﻿#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t monetary_mon_grouping_storage[]{3,2};

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Gujarati Language Locale For India"),.source=tsc("IndLinux.org\t\t;\t\tfast_io"),.address=tsc("https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Gujarati"),.territory=tsc("India"),.revision=tsc("0.2"),.date=tsc("2004-09-14")},.monetary={.int_curr_symbol=tsc("INR "),.currency_symbol=tsc("₹"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("રવિ"),tsc("સોમ"),tsc("મંગળ"),tsc("બુધ"),tsc("ગુરુ"),tsc("શુક્ર"),tsc("શનિ")},.day={tsc("રવિવાર"),tsc("સોમવાર"),tsc("મંગળવાર"),tsc("બુધવાર"),tsc("ગુરુવાર"),tsc("શુક્રવાર"),tsc("શનિવાર")},.abmon={tsc("જાન્યુ"),tsc("ફેબ્રુ"),tsc("માર્ચ"),tsc("એપ્રિલ"),tsc("મે"),tsc("જૂન"),tsc("જુલાઈ"),tsc("ઑગસ્ટ"),tsc("સપ્ટે"),tsc("ઑક્ટો"),tsc("નવે"),tsc("ડિસે")},.mon={tsc("જાન્યુઆરી"),tsc("ફેબ્રુઆરી"),tsc("માર્ચ"),tsc("એપ્રિલ"),tsc("મે"),tsc("જૂન"),tsc("જુલાઈ"),tsc("ઑગસ્ટ"),tsc("સપ્ટેમ્બર"),tsc("ઑક્ટોબર"),tsc("નવેમ્બર"),tsc("ડિસેમ્બર")},.d_t_fmt=tsc("%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc("%-d//%-m//%y"),.t_fmt=tsc("%I:%M:%S %p %Z"),.t_fmt_ampm=tsc("%I:%M:%S %p %Z"),.date_fmt=tsc("%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc("સવારે"),tsc("સાંજે")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yYહ]"),.noexpr=tsc("^[-0nNન]"),.yesstr=tsc("હા"),.nostr=tsc("નહીં")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("91")},.name={.name_fmt=tsc("%p%t%f%t%g"),.name_gen=tsc("જાતિ"),.name_miss=tsc("કુમારી"),.name_mr=tsc("શ્રીમાન"),.name_mrs=tsc("શ્રીમતિ"),.name_ms=tsc("સશ્રી")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("ભારત"),.country_ab2=tsc("IN"),.country_ab3=tsc("IND"),.country_num=356,.country_car=tsc("IND"),.lang_name=tsc("ગુજરાતી"),.lang_ab=tsc("gu"),.lang_term=tsc("guj"),.lang_lib=tsc("guj")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Gujarati Language Locale For India"),.source=tsc(L"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(L"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Gujarati"),.territory=tsc(L"India"),.revision=tsc(L"0.2"),.date=tsc(L"2004-09-14")},.monetary={.int_curr_symbol=tsc(L"INR "),.currency_symbol=tsc(L"₹"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"રવિ"),tsc(L"સોમ"),tsc(L"મંગળ"),tsc(L"બુધ"),tsc(L"ગુરુ"),tsc(L"શુક્ર"),tsc(L"શનિ")},.day={tsc(L"રવિવાર"),tsc(L"સોમવાર"),tsc(L"મંગળવાર"),tsc(L"બુધવાર"),tsc(L"ગુરુવાર"),tsc(L"શુક્રવાર"),tsc(L"શનિવાર")},.abmon={tsc(L"જાન્યુ"),tsc(L"ફેબ્રુ"),tsc(L"માર્ચ"),tsc(L"એપ્રિલ"),tsc(L"મે"),tsc(L"જૂન"),tsc(L"જુલાઈ"),tsc(L"ઑગસ્ટ"),tsc(L"સપ્ટે"),tsc(L"ઑક્ટો"),tsc(L"નવે"),tsc(L"ડિસે")},.mon={tsc(L"જાન્યુઆરી"),tsc(L"ફેબ્રુઆરી"),tsc(L"માર્ચ"),tsc(L"એપ્રિલ"),tsc(L"મે"),tsc(L"જૂન"),tsc(L"જુલાઈ"),tsc(L"ઑગસ્ટ"),tsc(L"સપ્ટેમ્બર"),tsc(L"ઑક્ટોબર"),tsc(L"નવેમ્બર"),tsc(L"ડિસેમ્બર")},.d_t_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(L"%-d//%-m//%y"),.t_fmt=tsc(L"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(L"%I:%M:%S %p %Z"),.date_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(L"સવારે"),tsc(L"સાંજે")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yYહ]"),.noexpr=tsc(L"^[-0nNન]"),.yesstr=tsc(L"હા"),.nostr=tsc(L"નહીં")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"91")},.name={.name_fmt=tsc(L"%p%t%f%t%g"),.name_gen=tsc(L"જાતિ"),.name_miss=tsc(L"કુમારી"),.name_mr=tsc(L"શ્રીમાન"),.name_mrs=tsc(L"શ્રીમતિ"),.name_ms=tsc(L"સશ્રી")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"ભારત"),.country_ab2=tsc(L"IN"),.country_ab3=tsc(L"IND"),.country_num=356,.country_car=tsc(L"IND"),.lang_name=tsc(L"ગુજરાતી"),.lang_ab=tsc(L"gu"),.lang_term=tsc(L"guj"),.lang_lib=tsc(L"guj")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Gujarati Language Locale For India"),.source=tsc(u8"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(u8"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Gujarati"),.territory=tsc(u8"India"),.revision=tsc(u8"0.2"),.date=tsc(u8"2004-09-14")},.monetary={.int_curr_symbol=tsc(u8"INR "),.currency_symbol=tsc(u8"₹"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"રવિ"),tsc(u8"સોમ"),tsc(u8"મંગળ"),tsc(u8"બુધ"),tsc(u8"ગુરુ"),tsc(u8"શુક્ર"),tsc(u8"શનિ")},.day={tsc(u8"રવિવાર"),tsc(u8"સોમવાર"),tsc(u8"મંગળવાર"),tsc(u8"બુધવાર"),tsc(u8"ગુરુવાર"),tsc(u8"શુક્રવાર"),tsc(u8"શનિવાર")},.abmon={tsc(u8"જાન્યુ"),tsc(u8"ફેબ્રુ"),tsc(u8"માર્ચ"),tsc(u8"એપ્રિલ"),tsc(u8"મે"),tsc(u8"જૂન"),tsc(u8"જુલાઈ"),tsc(u8"ઑગસ્ટ"),tsc(u8"સપ્ટે"),tsc(u8"ઑક્ટો"),tsc(u8"નવે"),tsc(u8"ડિસે")},.mon={tsc(u8"જાન્યુઆરી"),tsc(u8"ફેબ્રુઆરી"),tsc(u8"માર્ચ"),tsc(u8"એપ્રિલ"),tsc(u8"મે"),tsc(u8"જૂન"),tsc(u8"જુલાઈ"),tsc(u8"ઑગસ્ટ"),tsc(u8"સપ્ટેમ્બર"),tsc(u8"ઑક્ટોબર"),tsc(u8"નવેમ્બર"),tsc(u8"ડિસેમ્બર")},.d_t_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u8"%-d//%-m//%y"),.t_fmt=tsc(u8"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p %Z"),.date_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u8"સવારે"),tsc(u8"સાંજે")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yYહ]"),.noexpr=tsc(u8"^[-0nNન]"),.yesstr=tsc(u8"હા"),.nostr=tsc(u8"નહીં")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"91")},.name={.name_fmt=tsc(u8"%p%t%f%t%g"),.name_gen=tsc(u8"જાતિ"),.name_miss=tsc(u8"કુમારી"),.name_mr=tsc(u8"શ્રીમાન"),.name_mrs=tsc(u8"શ્રીમતિ"),.name_ms=tsc(u8"સશ્રી")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"ભારત"),.country_ab2=tsc(u8"IN"),.country_ab3=tsc(u8"IND"),.country_num=356,.country_car=tsc(u8"IND"),.lang_name=tsc(u8"ગુજરાતી"),.lang_ab=tsc(u8"gu"),.lang_term=tsc(u8"guj"),.lang_lib=tsc(u8"guj")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Gujarati Language Locale For India"),.source=tsc(u"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(u"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Gujarati"),.territory=tsc(u"India"),.revision=tsc(u"0.2"),.date=tsc(u"2004-09-14")},.monetary={.int_curr_symbol=tsc(u"INR "),.currency_symbol=tsc(u"₹"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"રવિ"),tsc(u"સોમ"),tsc(u"મંગળ"),tsc(u"બુધ"),tsc(u"ગુરુ"),tsc(u"શુક્ર"),tsc(u"શનિ")},.day={tsc(u"રવિવાર"),tsc(u"સોમવાર"),tsc(u"મંગળવાર"),tsc(u"બુધવાર"),tsc(u"ગુરુવાર"),tsc(u"શુક્રવાર"),tsc(u"શનિવાર")},.abmon={tsc(u"જાન્યુ"),tsc(u"ફેબ્રુ"),tsc(u"માર્ચ"),tsc(u"એપ્રિલ"),tsc(u"મે"),tsc(u"જૂન"),tsc(u"જુલાઈ"),tsc(u"ઑગસ્ટ"),tsc(u"સપ્ટે"),tsc(u"ઑક્ટો"),tsc(u"નવે"),tsc(u"ડિસે")},.mon={tsc(u"જાન્યુઆરી"),tsc(u"ફેબ્રુઆરી"),tsc(u"માર્ચ"),tsc(u"એપ્રિલ"),tsc(u"મે"),tsc(u"જૂન"),tsc(u"જુલાઈ"),tsc(u"ઑગસ્ટ"),tsc(u"સપ્ટેમ્બર"),tsc(u"ઑક્ટોબર"),tsc(u"નવેમ્બર"),tsc(u"ડિસેમ્બર")},.d_t_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u"%-d//%-m//%y"),.t_fmt=tsc(u"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u"%I:%M:%S %p %Z"),.date_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u"સવારે"),tsc(u"સાંજે")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yYહ]"),.noexpr=tsc(u"^[-0nNન]"),.yesstr=tsc(u"હા"),.nostr=tsc(u"નહીં")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"91")},.name={.name_fmt=tsc(u"%p%t%f%t%g"),.name_gen=tsc(u"જાતિ"),.name_miss=tsc(u"કુમારી"),.name_mr=tsc(u"શ્રીમાન"),.name_mrs=tsc(u"શ્રીમતિ"),.name_ms=tsc(u"સશ્રી")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"ભારત"),.country_ab2=tsc(u"IN"),.country_ab3=tsc(u"IND"),.country_num=356,.country_car=tsc(u"IND"),.lang_name=tsc(u"ગુજરાતી"),.lang_ab=tsc(u"gu"),.lang_term=tsc(u"guj"),.lang_lib=tsc(u"guj")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Gujarati Language Locale For India"),.source=tsc(U"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(U"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Gujarati"),.territory=tsc(U"India"),.revision=tsc(U"0.2"),.date=tsc(U"2004-09-14")},.monetary={.int_curr_symbol=tsc(U"INR "),.currency_symbol=tsc(U"₹"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"રવિ"),tsc(U"સોમ"),tsc(U"મંગળ"),tsc(U"બુધ"),tsc(U"ગુરુ"),tsc(U"શુક્ર"),tsc(U"શનિ")},.day={tsc(U"રવિવાર"),tsc(U"સોમવાર"),tsc(U"મંગળવાર"),tsc(U"બુધવાર"),tsc(U"ગુરુવાર"),tsc(U"શુક્રવાર"),tsc(U"શનિવાર")},.abmon={tsc(U"જાન્યુ"),tsc(U"ફેબ્રુ"),tsc(U"માર્ચ"),tsc(U"એપ્રિલ"),tsc(U"મે"),tsc(U"જૂન"),tsc(U"જુલાઈ"),tsc(U"ઑગસ્ટ"),tsc(U"સપ્ટે"),tsc(U"ઑક્ટો"),tsc(U"નવે"),tsc(U"ડિસે")},.mon={tsc(U"જાન્યુઆરી"),tsc(U"ફેબ્રુઆરી"),tsc(U"માર્ચ"),tsc(U"એપ્રિલ"),tsc(U"મે"),tsc(U"જૂન"),tsc(U"જુલાઈ"),tsc(U"ઑગસ્ટ"),tsc(U"સપ્ટેમ્બર"),tsc(U"ઑક્ટોબર"),tsc(U"નવેમ્બર"),tsc(U"ડિસેમ્બર")},.d_t_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(U"%-d//%-m//%y"),.t_fmt=tsc(U"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(U"%I:%M:%S %p %Z"),.date_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(U"સવારે"),tsc(U"સાંજે")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yYહ]"),.noexpr=tsc(U"^[-0nNન]"),.yesstr=tsc(U"હા"),.nostr=tsc(U"નહીં")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"91")},.name={.name_fmt=tsc(U"%p%t%f%t%g"),.name_gen=tsc(U"જાતિ"),.name_miss=tsc(U"કુમારી"),.name_mr=tsc(U"શ્રીમાન"),.name_mrs=tsc(U"શ્રીમતિ"),.name_ms=tsc(U"સશ્રી")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"ભારત"),.country_ab2=tsc(U"IN"),.country_ab3=tsc(U"IND"),.country_num=356,.country_car=tsc(U"IND"),.lang_name=tsc(U"ગુજરાતી"),.lang_ab=tsc(U"gu"),.lang_term=tsc(U"guj"),.lang_lib=tsc(U"guj")},.measurement={.measurement=1}};


}
}

#include"../main.h"