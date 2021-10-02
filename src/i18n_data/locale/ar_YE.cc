﻿#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Arabic language locale for Yemen"),.source=tsc("IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc("1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Arabic"),.territory=tsc("Yemen"),.revision=tsc("1.0"),.date=tsc("2000-07-20")},.monetary={.int_curr_symbol=tsc("YER "),.currency_symbol=tsc("ر.ي."),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("ح"),tsc("ن"),tsc("ث"),tsc("ر"),tsc("خ"),tsc("ج"),tsc("س")},.day={tsc("الأحد"),tsc("الاثنين"),tsc("الثلاثاء"),tsc("الأربعاء"),tsc("الخميس"),tsc("الجمعة"),tsc("السبت")},.abmon={tsc("ينا"),tsc("فبر"),tsc("مار"),tsc("أبر"),tsc("ماي"),tsc("يون"),tsc("يول"),tsc("أغس"),tsc("سبت"),tsc("أكت"),tsc("نوف"),tsc("ديس")},.mon={tsc("يناير"),tsc("فبراير"),tsc("مارس"),tsc("أبريل"),tsc("مايو"),tsc("يونيو"),tsc("يوليو"),tsc("أغسطس"),tsc("سبتمبر"),tsc("أكتوبر"),tsc("نوفمبر"),tsc("ديسمبر")},.d_t_fmt=tsc("%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc("%d %b, %Y"),.t_fmt=tsc("%Z %I:%M:%S %p"),.t_fmt_ampm=tsc("%Z %I:%M:%S %p"),.date_fmt=tsc("%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc("ص"),tsc("م")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc("^[+1نyY]"),.noexpr=tsc("^[-0لnN]"),.yesstr=tsc("نعم"),.nostr=tsc("لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("967")},.name={.name_fmt=tsc("%p%t%f%t%g"),.name_gen=tsc("-san"),.name_miss=tsc("Miss."),.name_mr=tsc("Mr."),.name_mrs=tsc("Mrs."),.name_ms=tsc("Ms.")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("اليمن"),.country_ab2=tsc("YE"),.country_ab3=tsc("YEM"),.country_num=887,.country_car=tsc("YAR"),.lang_name=tsc("العربية"),.lang_ab=tsc("ar"),.lang_term=tsc("ara"),.lang_lib=tsc("ara")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Arabic language locale for Yemen"),.source=tsc(L"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(L"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Arabic"),.territory=tsc(L"Yemen"),.revision=tsc(L"1.0"),.date=tsc(L"2000-07-20")},.monetary={.int_curr_symbol=tsc(L"YER "),.currency_symbol=tsc(L"ر.ي."),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"ح"),tsc(L"ن"),tsc(L"ث"),tsc(L"ر"),tsc(L"خ"),tsc(L"ج"),tsc(L"س")},.day={tsc(L"الأحد"),tsc(L"الاثنين"),tsc(L"الثلاثاء"),tsc(L"الأربعاء"),tsc(L"الخميس"),tsc(L"الجمعة"),tsc(L"السبت")},.abmon={tsc(L"ينا"),tsc(L"فبر"),tsc(L"مار"),tsc(L"أبر"),tsc(L"ماي"),tsc(L"يون"),tsc(L"يول"),tsc(L"أغس"),tsc(L"سبت"),tsc(L"أكت"),tsc(L"نوف"),tsc(L"ديس")},.mon={tsc(L"يناير"),tsc(L"فبراير"),tsc(L"مارس"),tsc(L"أبريل"),tsc(L"مايو"),tsc(L"يونيو"),tsc(L"يوليو"),tsc(L"أغسطس"),tsc(L"سبتمبر"),tsc(L"أكتوبر"),tsc(L"نوفمبر"),tsc(L"ديسمبر")},.d_t_fmt=tsc(L"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(L"%d %b, %Y"),.t_fmt=tsc(L"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(L"%Z %I:%M:%S %p"),.date_fmt=tsc(L"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(L"ص"),tsc(L"م")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(L"^[+1نyY]"),.noexpr=tsc(L"^[-0لnN]"),.yesstr=tsc(L"نعم"),.nostr=tsc(L"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"967")},.name={.name_fmt=tsc(L"%p%t%f%t%g"),.name_gen=tsc(L"-san"),.name_miss=tsc(L"Miss."),.name_mr=tsc(L"Mr."),.name_mrs=tsc(L"Mrs."),.name_ms=tsc(L"Ms.")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"اليمن"),.country_ab2=tsc(L"YE"),.country_ab3=tsc(L"YEM"),.country_num=887,.country_car=tsc(L"YAR"),.lang_name=tsc(L"العربية"),.lang_ab=tsc(L"ar"),.lang_term=tsc(L"ara"),.lang_lib=tsc(L"ara")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Arabic language locale for Yemen"),.source=tsc(u8"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(u8"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Arabic"),.territory=tsc(u8"Yemen"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-07-20")},.monetary={.int_curr_symbol=tsc(u8"YER "),.currency_symbol=tsc(u8"ر.ي."),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"ح"),tsc(u8"ن"),tsc(u8"ث"),tsc(u8"ر"),tsc(u8"خ"),tsc(u8"ج"),tsc(u8"س")},.day={tsc(u8"الأحد"),tsc(u8"الاثنين"),tsc(u8"الثلاثاء"),tsc(u8"الأربعاء"),tsc(u8"الخميس"),tsc(u8"الجمعة"),tsc(u8"السبت")},.abmon={tsc(u8"ينا"),tsc(u8"فبر"),tsc(u8"مار"),tsc(u8"أبر"),tsc(u8"ماي"),tsc(u8"يون"),tsc(u8"يول"),tsc(u8"أغس"),tsc(u8"سبت"),tsc(u8"أكت"),tsc(u8"نوف"),tsc(u8"ديس")},.mon={tsc(u8"يناير"),tsc(u8"فبراير"),tsc(u8"مارس"),tsc(u8"أبريل"),tsc(u8"مايو"),tsc(u8"يونيو"),tsc(u8"يوليو"),tsc(u8"أغسطس"),tsc(u8"سبتمبر"),tsc(u8"أكتوبر"),tsc(u8"نوفمبر"),tsc(u8"ديسمبر")},.d_t_fmt=tsc(u8"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(u8"%d %b, %Y"),.t_fmt=tsc(u8"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(u8"%Z %I:%M:%S %p"),.date_fmt=tsc(u8"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(u8"ص"),tsc(u8"م")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(u8"^[+1نyY]"),.noexpr=tsc(u8"^[-0لnN]"),.yesstr=tsc(u8"نعم"),.nostr=tsc(u8"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"967")},.name={.name_fmt=tsc(u8"%p%t%f%t%g"),.name_gen=tsc(u8"-san"),.name_miss=tsc(u8"Miss."),.name_mr=tsc(u8"Mr."),.name_mrs=tsc(u8"Mrs."),.name_ms=tsc(u8"Ms.")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"اليمن"),.country_ab2=tsc(u8"YE"),.country_ab3=tsc(u8"YEM"),.country_num=887,.country_car=tsc(u8"YAR"),.lang_name=tsc(u8"العربية"),.lang_ab=tsc(u8"ar"),.lang_term=tsc(u8"ara"),.lang_lib=tsc(u8"ara")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Arabic language locale for Yemen"),.source=tsc(u"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(u"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Arabic"),.territory=tsc(u"Yemen"),.revision=tsc(u"1.0"),.date=tsc(u"2000-07-20")},.monetary={.int_curr_symbol=tsc(u"YER "),.currency_symbol=tsc(u"ر.ي."),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"ح"),tsc(u"ن"),tsc(u"ث"),tsc(u"ر"),tsc(u"خ"),tsc(u"ج"),tsc(u"س")},.day={tsc(u"الأحد"),tsc(u"الاثنين"),tsc(u"الثلاثاء"),tsc(u"الأربعاء"),tsc(u"الخميس"),tsc(u"الجمعة"),tsc(u"السبت")},.abmon={tsc(u"ينا"),tsc(u"فبر"),tsc(u"مار"),tsc(u"أبر"),tsc(u"ماي"),tsc(u"يون"),tsc(u"يول"),tsc(u"أغس"),tsc(u"سبت"),tsc(u"أكت"),tsc(u"نوف"),tsc(u"ديس")},.mon={tsc(u"يناير"),tsc(u"فبراير"),tsc(u"مارس"),tsc(u"أبريل"),tsc(u"مايو"),tsc(u"يونيو"),tsc(u"يوليو"),tsc(u"أغسطس"),tsc(u"سبتمبر"),tsc(u"أكتوبر"),tsc(u"نوفمبر"),tsc(u"ديسمبر")},.d_t_fmt=tsc(u"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(u"%d %b, %Y"),.t_fmt=tsc(u"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(u"%Z %I:%M:%S %p"),.date_fmt=tsc(u"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(u"ص"),tsc(u"م")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(u"^[+1نyY]"),.noexpr=tsc(u"^[-0لnN]"),.yesstr=tsc(u"نعم"),.nostr=tsc(u"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"967")},.name={.name_fmt=tsc(u"%p%t%f%t%g"),.name_gen=tsc(u"-san"),.name_miss=tsc(u"Miss."),.name_mr=tsc(u"Mr."),.name_mrs=tsc(u"Mrs."),.name_ms=tsc(u"Ms.")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"اليمن"),.country_ab2=tsc(u"YE"),.country_ab3=tsc(u"YEM"),.country_num=887,.country_car=tsc(u"YAR"),.lang_name=tsc(u"العربية"),.lang_ab=tsc(u"ar"),.lang_term=tsc(u"ara"),.lang_lib=tsc(u"ara")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Arabic language locale for Yemen"),.source=tsc(U"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(U"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Arabic"),.territory=tsc(U"Yemen"),.revision=tsc(U"1.0"),.date=tsc(U"2000-07-20")},.monetary={.int_curr_symbol=tsc(U"YER "),.currency_symbol=tsc(U"ر.ي."),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"ح"),tsc(U"ن"),tsc(U"ث"),tsc(U"ر"),tsc(U"خ"),tsc(U"ج"),tsc(U"س")},.day={tsc(U"الأحد"),tsc(U"الاثنين"),tsc(U"الثلاثاء"),tsc(U"الأربعاء"),tsc(U"الخميس"),tsc(U"الجمعة"),tsc(U"السبت")},.abmon={tsc(U"ينا"),tsc(U"فبر"),tsc(U"مار"),tsc(U"أبر"),tsc(U"ماي"),tsc(U"يون"),tsc(U"يول"),tsc(U"أغس"),tsc(U"سبت"),tsc(U"أكت"),tsc(U"نوف"),tsc(U"ديس")},.mon={tsc(U"يناير"),tsc(U"فبراير"),tsc(U"مارس"),tsc(U"أبريل"),tsc(U"مايو"),tsc(U"يونيو"),tsc(U"يوليو"),tsc(U"أغسطس"),tsc(U"سبتمبر"),tsc(U"أكتوبر"),tsc(U"نوفمبر"),tsc(U"ديسمبر")},.d_t_fmt=tsc(U"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(U"%d %b, %Y"),.t_fmt=tsc(U"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(U"%Z %I:%M:%S %p"),.date_fmt=tsc(U"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(U"ص"),tsc(U"م")},.week={7,19971130,1},.first_workday=1},.messages={.yesexpr=tsc(U"^[+1نyY]"),.noexpr=tsc(U"^[-0لnN]"),.yesstr=tsc(U"نعم"),.nostr=tsc(U"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"967")},.name={.name_fmt=tsc(U"%p%t%f%t%g"),.name_gen=tsc(U"-san"),.name_miss=tsc(U"Miss."),.name_mr=tsc(U"Mr."),.name_mrs=tsc(U"Mrs."),.name_ms=tsc(U"Ms.")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"اليمن"),.country_ab2=tsc(U"YE"),.country_ab3=tsc(U"YEM"),.country_num=887,.country_car=tsc(U"YAR"),.lang_name=tsc(U"العربية"),.lang_ab=tsc(U"ar"),.lang_term=tsc(U"ara"),.lang_lib=tsc(U"ara")},.measurement={.measurement=1}};


}
}

#include"../main.h"