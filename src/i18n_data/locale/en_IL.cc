#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("English locale for Israel"),.source=tsc("Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc("https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("English"),.territory=tsc("Israel"),.revision=tsc("1.1"),.date=tsc("2016-04-19")},.monetary={.int_curr_symbol=tsc("ILS "),.currency_symbol=tsc("₪"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=2,.n_sign_posn=2},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Sun"),tsc("Mon"),tsc("Tue"),tsc("Wed"),tsc("Thu"),tsc("Fri"),tsc("Sat")},.day={tsc("Sunday"),tsc("Monday"),tsc("Tuesday"),tsc("Wednesday"),tsc("Thursday"),tsc("Friday"),tsc("Saturday")},.abmon={tsc("Jan"),tsc("Feb"),tsc("Mar"),tsc("Apr"),tsc("May"),tsc("Jun"),tsc("Jul"),tsc("Aug"),tsc("Sep"),tsc("Oct"),tsc("Nov"),tsc("Dec")},.mon={tsc("January"),tsc("February"),tsc("March"),tsc("April"),tsc("May"),tsc("June"),tsc("July"),tsc("August"),tsc("September"),tsc("October"),tsc("November"),tsc("December")},.d_t_fmt=tsc("%a %d %b %Y %T"),.d_fmt=tsc("%d//%m//%y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%a %d %b %Y %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc("^[+1yY]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("yes"),.nostr=tsc("no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00,012,013,014,018"),.int_prefix=tsc("972")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f"),.name_miss=tsc("Miss."),.name_mr=tsc("Mr."),.name_mrs=tsc("Mrs."),.name_ms=tsc("Ms.")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Israel"),.country_post=tsc("ISR"),.country_ab2=tsc("IL"),.country_ab3=tsc("ISR"),.country_num=376,.country_car=tsc("IL"),.lang_name=tsc("English"),.lang_ab=tsc("en"),.lang_term=tsc("eng"),.lang_lib=tsc("eng")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"English locale for Israel"),.source=tsc(L"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(L"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"English"),.territory=tsc(L"Israel"),.revision=tsc(L"1.1"),.date=tsc(L"2016-04-19")},.monetary={.int_curr_symbol=tsc(L"ILS "),.currency_symbol=tsc(L"₪"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=2,.n_sign_posn=2},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Sun"),tsc(L"Mon"),tsc(L"Tue"),tsc(L"Wed"),tsc(L"Thu"),tsc(L"Fri"),tsc(L"Sat")},.day={tsc(L"Sunday"),tsc(L"Monday"),tsc(L"Tuesday"),tsc(L"Wednesday"),tsc(L"Thursday"),tsc(L"Friday"),tsc(L"Saturday")},.abmon={tsc(L"Jan"),tsc(L"Feb"),tsc(L"Mar"),tsc(L"Apr"),tsc(L"May"),tsc(L"Jun"),tsc(L"Jul"),tsc(L"Aug"),tsc(L"Sep"),tsc(L"Oct"),tsc(L"Nov"),tsc(L"Dec")},.mon={tsc(L"January"),tsc(L"February"),tsc(L"March"),tsc(L"April"),tsc(L"May"),tsc(L"June"),tsc(L"July"),tsc(L"August"),tsc(L"September"),tsc(L"October"),tsc(L"November"),tsc(L"December")},.d_t_fmt=tsc(L"%a %d %b %Y %T"),.d_fmt=tsc(L"%d//%m//%y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%a %d %b %Y %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(L"^[+1yY]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"yes"),.nostr=tsc(L"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00,012,013,014,018"),.int_prefix=tsc(L"972")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f"),.name_miss=tsc(L"Miss."),.name_mr=tsc(L"Mr."),.name_mrs=tsc(L"Mrs."),.name_ms=tsc(L"Ms.")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Israel"),.country_post=tsc(L"ISR"),.country_ab2=tsc(L"IL"),.country_ab3=tsc(L"ISR"),.country_num=376,.country_car=tsc(L"IL"),.lang_name=tsc(L"English"),.lang_ab=tsc(L"en"),.lang_term=tsc(L"eng"),.lang_lib=tsc(L"eng")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"English locale for Israel"),.source=tsc(u8"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u8"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"English"),.territory=tsc(u8"Israel"),.revision=tsc(u8"1.1"),.date=tsc(u8"2016-04-19")},.monetary={.int_curr_symbol=tsc(u8"ILS "),.currency_symbol=tsc(u8"₪"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=2,.n_sign_posn=2},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Sun"),tsc(u8"Mon"),tsc(u8"Tue"),tsc(u8"Wed"),tsc(u8"Thu"),tsc(u8"Fri"),tsc(u8"Sat")},.day={tsc(u8"Sunday"),tsc(u8"Monday"),tsc(u8"Tuesday"),tsc(u8"Wednesday"),tsc(u8"Thursday"),tsc(u8"Friday"),tsc(u8"Saturday")},.abmon={tsc(u8"Jan"),tsc(u8"Feb"),tsc(u8"Mar"),tsc(u8"Apr"),tsc(u8"May"),tsc(u8"Jun"),tsc(u8"Jul"),tsc(u8"Aug"),tsc(u8"Sep"),tsc(u8"Oct"),tsc(u8"Nov"),tsc(u8"Dec")},.mon={tsc(u8"January"),tsc(u8"February"),tsc(u8"March"),tsc(u8"April"),tsc(u8"May"),tsc(u8"June"),tsc(u8"July"),tsc(u8"August"),tsc(u8"September"),tsc(u8"October"),tsc(u8"November"),tsc(u8"December")},.d_t_fmt=tsc(u8"%a %d %b %Y %T"),.d_fmt=tsc(u8"%d//%m//%y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%a %d %b %Y %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(u8"^[+1yY]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"yes"),.nostr=tsc(u8"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00,012,013,014,018"),.int_prefix=tsc(u8"972")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f"),.name_miss=tsc(u8"Miss."),.name_mr=tsc(u8"Mr."),.name_mrs=tsc(u8"Mrs."),.name_ms=tsc(u8"Ms.")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Israel"),.country_post=tsc(u8"ISR"),.country_ab2=tsc(u8"IL"),.country_ab3=tsc(u8"ISR"),.country_num=376,.country_car=tsc(u8"IL"),.lang_name=tsc(u8"English"),.lang_ab=tsc(u8"en"),.lang_term=tsc(u8"eng"),.lang_lib=tsc(u8"eng")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"English locale for Israel"),.source=tsc(u"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"English"),.territory=tsc(u"Israel"),.revision=tsc(u"1.1"),.date=tsc(u"2016-04-19")},.monetary={.int_curr_symbol=tsc(u"ILS "),.currency_symbol=tsc(u"₪"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=2,.n_sign_posn=2},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Sun"),tsc(u"Mon"),tsc(u"Tue"),tsc(u"Wed"),tsc(u"Thu"),tsc(u"Fri"),tsc(u"Sat")},.day={tsc(u"Sunday"),tsc(u"Monday"),tsc(u"Tuesday"),tsc(u"Wednesday"),tsc(u"Thursday"),tsc(u"Friday"),tsc(u"Saturday")},.abmon={tsc(u"Jan"),tsc(u"Feb"),tsc(u"Mar"),tsc(u"Apr"),tsc(u"May"),tsc(u"Jun"),tsc(u"Jul"),tsc(u"Aug"),tsc(u"Sep"),tsc(u"Oct"),tsc(u"Nov"),tsc(u"Dec")},.mon={tsc(u"January"),tsc(u"February"),tsc(u"March"),tsc(u"April"),tsc(u"May"),tsc(u"June"),tsc(u"July"),tsc(u"August"),tsc(u"September"),tsc(u"October"),tsc(u"November"),tsc(u"December")},.d_t_fmt=tsc(u"%a %d %b %Y %T"),.d_fmt=tsc(u"%d//%m//%y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%a %d %b %Y %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(u"^[+1yY]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"yes"),.nostr=tsc(u"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00,012,013,014,018"),.int_prefix=tsc(u"972")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f"),.name_miss=tsc(u"Miss."),.name_mr=tsc(u"Mr."),.name_mrs=tsc(u"Mrs."),.name_ms=tsc(u"Ms.")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Israel"),.country_post=tsc(u"ISR"),.country_ab2=tsc(u"IL"),.country_ab3=tsc(u"ISR"),.country_num=376,.country_car=tsc(u"IL"),.lang_name=tsc(u"English"),.lang_ab=tsc(u"en"),.lang_term=tsc(u"eng"),.lang_lib=tsc(u"eng")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"English locale for Israel"),.source=tsc(U"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(U"https://www.gnu.org/software/libc/\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"English"),.territory=tsc(U"Israel"),.revision=tsc(U"1.1"),.date=tsc(U"2016-04-19")},.monetary={.int_curr_symbol=tsc(U"ILS "),.currency_symbol=tsc(U"₪"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=2,.n_sign_posn=2},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Sun"),tsc(U"Mon"),tsc(U"Tue"),tsc(U"Wed"),tsc(U"Thu"),tsc(U"Fri"),tsc(U"Sat")},.day={tsc(U"Sunday"),tsc(U"Monday"),tsc(U"Tuesday"),tsc(U"Wednesday"),tsc(U"Thursday"),tsc(U"Friday"),tsc(U"Saturday")},.abmon={tsc(U"Jan"),tsc(U"Feb"),tsc(U"Mar"),tsc(U"Apr"),tsc(U"May"),tsc(U"Jun"),tsc(U"Jul"),tsc(U"Aug"),tsc(U"Sep"),tsc(U"Oct"),tsc(U"Nov"),tsc(U"Dec")},.mon={tsc(U"January"),tsc(U"February"),tsc(U"March"),tsc(U"April"),tsc(U"May"),tsc(U"June"),tsc(U"July"),tsc(U"August"),tsc(U"September"),tsc(U"October"),tsc(U"November"),tsc(U"December")},.d_t_fmt=tsc(U"%a %d %b %Y %T"),.d_fmt=tsc(U"%d//%m//%y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%a %d %b %Y %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(U"^[+1yY]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"yes"),.nostr=tsc(U"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00,012,013,014,018"),.int_prefix=tsc(U"972")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f"),.name_miss=tsc(U"Miss."),.name_mr=tsc(U"Mr."),.name_mrs=tsc(U"Mrs."),.name_ms=tsc(U"Ms.")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Israel"),.country_post=tsc(U"ISR"),.country_ab2=tsc(U"IL"),.country_ab3=tsc(U"ISR"),.country_num=376,.country_car=tsc(U"IL"),.lang_name=tsc(U"English"),.lang_ab=tsc(U"en"),.lang_term=tsc(U"eng"),.lang_lib=tsc(U"eng")},.measurement={.measurement=1}};


}
}

#include"../main.h"