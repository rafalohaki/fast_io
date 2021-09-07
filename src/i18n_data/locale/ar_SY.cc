#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Arabic language locale for Syrian Arab Republic"),.source=tsc("IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc("1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Arabic"),.territory=tsc("Syria"),.revision=tsc("1.0"),.date=tsc("2000-07-20")},.monetary={.int_curr_symbol=tsc("SYP "),.currency_symbol=tsc("ل.س."),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("ح"),tsc("ن"),tsc("ث"),tsc("ر"),tsc("خ"),tsc("ج"),tsc("س")},.day={tsc("الأحد"),tsc("الاثنين"),tsc("الثلاثاء"),tsc("الأربعاء"),tsc("الخميس"),tsc("الجمعة"),tsc("السبت")},.abmon={tsc("كانون الثاني"),tsc("شباط"),tsc("آذار"),tsc("نيسان"),tsc("أيار"),tsc("حزيران"),tsc("تموز"),tsc("آب"),tsc("أيلول"),tsc("تشرين الأول"),tsc("تشرين الثاني"),tsc("كانون الأول")},.mon={tsc("كانون الثاني"),tsc("شباط"),tsc("آذار"),tsc("نيسان"),tsc("أيار"),tsc("حزيران"),tsc("تموز"),tsc("آب"),tsc("أيلول"),tsc("تشرين الأول"),tsc("تشرين الثاني"),tsc("كانون الأول")},.d_t_fmt=tsc("%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc("%d %b, %Y"),.t_fmt=tsc("%Z %I:%M:%S %p"),.t_fmt_ampm=tsc("%Z %I:%M:%S %p"),.date_fmt=tsc("%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc("ص"),tsc("م")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc("^[+1نyY]"),.noexpr=tsc("^[-0لnN]"),.yesstr=tsc("نعم"),.nostr=tsc("لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("963")},.name={.name_fmt=tsc("%p%t%f%t%g"),.name_gen=tsc("-san"),.name_miss=tsc("Miss."),.name_mr=tsc("Mr."),.name_mrs=tsc("Mrs."),.name_ms=tsc("Ms.")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("سوريا"),.country_ab2=tsc("SY"),.country_ab3=tsc("SYR"),.country_num=760,.country_car=tsc("SYR"),.lang_name=tsc("العربية"),.lang_ab=tsc("ar"),.lang_term=tsc("ara"),.lang_lib=tsc("ara")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Arabic language locale for Syrian Arab Republic"),.source=tsc(L"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(L"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Arabic"),.territory=tsc(L"Syria"),.revision=tsc(L"1.0"),.date=tsc(L"2000-07-20")},.monetary={.int_curr_symbol=tsc(L"SYP "),.currency_symbol=tsc(L"ل.س."),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"ح"),tsc(L"ن"),tsc(L"ث"),tsc(L"ر"),tsc(L"خ"),tsc(L"ج"),tsc(L"س")},.day={tsc(L"الأحد"),tsc(L"الاثنين"),tsc(L"الثلاثاء"),tsc(L"الأربعاء"),tsc(L"الخميس"),tsc(L"الجمعة"),tsc(L"السبت")},.abmon={tsc(L"كانون الثاني"),tsc(L"شباط"),tsc(L"آذار"),tsc(L"نيسان"),tsc(L"أيار"),tsc(L"حزيران"),tsc(L"تموز"),tsc(L"آب"),tsc(L"أيلول"),tsc(L"تشرين الأول"),tsc(L"تشرين الثاني"),tsc(L"كانون الأول")},.mon={tsc(L"كانون الثاني"),tsc(L"شباط"),tsc(L"آذار"),tsc(L"نيسان"),tsc(L"أيار"),tsc(L"حزيران"),tsc(L"تموز"),tsc(L"آب"),tsc(L"أيلول"),tsc(L"تشرين الأول"),tsc(L"تشرين الثاني"),tsc(L"كانون الأول")},.d_t_fmt=tsc(L"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(L"%d %b, %Y"),.t_fmt=tsc(L"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(L"%Z %I:%M:%S %p"),.date_fmt=tsc(L"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(L"ص"),tsc(L"م")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(L"^[+1نyY]"),.noexpr=tsc(L"^[-0لnN]"),.yesstr=tsc(L"نعم"),.nostr=tsc(L"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"963")},.name={.name_fmt=tsc(L"%p%t%f%t%g"),.name_gen=tsc(L"-san"),.name_miss=tsc(L"Miss."),.name_mr=tsc(L"Mr."),.name_mrs=tsc(L"Mrs."),.name_ms=tsc(L"Ms.")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"سوريا"),.country_ab2=tsc(L"SY"),.country_ab3=tsc(L"SYR"),.country_num=760,.country_car=tsc(L"SYR"),.lang_name=tsc(L"العربية"),.lang_ab=tsc(L"ar"),.lang_term=tsc(L"ara"),.lang_lib=tsc(L"ara")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Arabic language locale for Syrian Arab Republic"),.source=tsc(u8"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(u8"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Arabic"),.territory=tsc(u8"Syria"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-07-20")},.monetary={.int_curr_symbol=tsc(u8"SYP "),.currency_symbol=tsc(u8"ل.س."),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"ح"),tsc(u8"ن"),tsc(u8"ث"),tsc(u8"ر"),tsc(u8"خ"),tsc(u8"ج"),tsc(u8"س")},.day={tsc(u8"الأحد"),tsc(u8"الاثنين"),tsc(u8"الثلاثاء"),tsc(u8"الأربعاء"),tsc(u8"الخميس"),tsc(u8"الجمعة"),tsc(u8"السبت")},.abmon={tsc(u8"كانون الثاني"),tsc(u8"شباط"),tsc(u8"آذار"),tsc(u8"نيسان"),tsc(u8"أيار"),tsc(u8"حزيران"),tsc(u8"تموز"),tsc(u8"آب"),tsc(u8"أيلول"),tsc(u8"تشرين الأول"),tsc(u8"تشرين الثاني"),tsc(u8"كانون الأول")},.mon={tsc(u8"كانون الثاني"),tsc(u8"شباط"),tsc(u8"آذار"),tsc(u8"نيسان"),tsc(u8"أيار"),tsc(u8"حزيران"),tsc(u8"تموز"),tsc(u8"آب"),tsc(u8"أيلول"),tsc(u8"تشرين الأول"),tsc(u8"تشرين الثاني"),tsc(u8"كانون الأول")},.d_t_fmt=tsc(u8"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(u8"%d %b, %Y"),.t_fmt=tsc(u8"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(u8"%Z %I:%M:%S %p"),.date_fmt=tsc(u8"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(u8"ص"),tsc(u8"م")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(u8"^[+1نyY]"),.noexpr=tsc(u8"^[-0لnN]"),.yesstr=tsc(u8"نعم"),.nostr=tsc(u8"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"963")},.name={.name_fmt=tsc(u8"%p%t%f%t%g"),.name_gen=tsc(u8"-san"),.name_miss=tsc(u8"Miss."),.name_mr=tsc(u8"Mr."),.name_mrs=tsc(u8"Mrs."),.name_ms=tsc(u8"Ms.")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"سوريا"),.country_ab2=tsc(u8"SY"),.country_ab3=tsc(u8"SYR"),.country_num=760,.country_car=tsc(u8"SYR"),.lang_name=tsc(u8"العربية"),.lang_ab=tsc(u8"ar"),.lang_term=tsc(u8"ara"),.lang_lib=tsc(u8"ara")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Arabic language locale for Syrian Arab Republic"),.source=tsc(u"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(u"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Arabic"),.territory=tsc(u"Syria"),.revision=tsc(u"1.0"),.date=tsc(u"2000-07-20")},.monetary={.int_curr_symbol=tsc(u"SYP "),.currency_symbol=tsc(u"ل.س."),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"ح"),tsc(u"ن"),tsc(u"ث"),tsc(u"ر"),tsc(u"خ"),tsc(u"ج"),tsc(u"س")},.day={tsc(u"الأحد"),tsc(u"الاثنين"),tsc(u"الثلاثاء"),tsc(u"الأربعاء"),tsc(u"الخميس"),tsc(u"الجمعة"),tsc(u"السبت")},.abmon={tsc(u"كانون الثاني"),tsc(u"شباط"),tsc(u"آذار"),tsc(u"نيسان"),tsc(u"أيار"),tsc(u"حزيران"),tsc(u"تموز"),tsc(u"آب"),tsc(u"أيلول"),tsc(u"تشرين الأول"),tsc(u"تشرين الثاني"),tsc(u"كانون الأول")},.mon={tsc(u"كانون الثاني"),tsc(u"شباط"),tsc(u"آذار"),tsc(u"نيسان"),tsc(u"أيار"),tsc(u"حزيران"),tsc(u"تموز"),tsc(u"آب"),tsc(u"أيلول"),tsc(u"تشرين الأول"),tsc(u"تشرين الثاني"),tsc(u"كانون الأول")},.d_t_fmt=tsc(u"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(u"%d %b, %Y"),.t_fmt=tsc(u"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(u"%Z %I:%M:%S %p"),.date_fmt=tsc(u"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(u"ص"),tsc(u"م")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(u"^[+1نyY]"),.noexpr=tsc(u"^[-0لnN]"),.yesstr=tsc(u"نعم"),.nostr=tsc(u"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"963")},.name={.name_fmt=tsc(u"%p%t%f%t%g"),.name_gen=tsc(u"-san"),.name_miss=tsc(u"Miss."),.name_mr=tsc(u"Mr."),.name_mrs=tsc(u"Mrs."),.name_ms=tsc(u"Ms.")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"سوريا"),.country_ab2=tsc(u"SY"),.country_ab3=tsc(u"SYR"),.country_num=760,.country_car=tsc(u"SYR"),.lang_name=tsc(u"العربية"),.lang_ab=tsc(u"ar"),.lang_term=tsc(u"ara"),.lang_lib=tsc(u"ara")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Arabic language locale for Syrian Arab Republic"),.source=tsc(U"IBM Globalization Center of Competency, Yamato Software Laboratory\t\t;\t\tfast_io"),.address=tsc(U"1623-14, Shimotsuruma, Yamato-shi, Kanagawa-ken, 242-8502, Japan\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Arabic"),.territory=tsc(U"Syria"),.revision=tsc(U"1.0"),.date=tsc(U"2000-07-20")},.monetary={.int_curr_symbol=tsc(U"SYP "),.currency_symbol=tsc(U"ل.س."),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"ح"),tsc(U"ن"),tsc(U"ث"),tsc(U"ر"),tsc(U"خ"),tsc(U"ج"),tsc(U"س")},.day={tsc(U"الأحد"),tsc(U"الاثنين"),tsc(U"الثلاثاء"),tsc(U"الأربعاء"),tsc(U"الخميس"),tsc(U"الجمعة"),tsc(U"السبت")},.abmon={tsc(U"كانون الثاني"),tsc(U"شباط"),tsc(U"آذار"),tsc(U"نيسان"),tsc(U"أيار"),tsc(U"حزيران"),tsc(U"تموز"),tsc(U"آب"),tsc(U"أيلول"),tsc(U"تشرين الأول"),tsc(U"تشرين الثاني"),tsc(U"كانون الأول")},.mon={tsc(U"كانون الثاني"),tsc(U"شباط"),tsc(U"آذار"),tsc(U"نيسان"),tsc(U"أيار"),tsc(U"حزيران"),tsc(U"تموز"),tsc(U"آب"),tsc(U"أيلول"),tsc(U"تشرين الأول"),tsc(U"تشرين الثاني"),tsc(U"كانون الأول")},.d_t_fmt=tsc(U"%d %b, %Y %I:%M:%S %p"),.d_fmt=tsc(U"%d %b, %Y"),.t_fmt=tsc(U"%Z %I:%M:%S %p"),.t_fmt_ampm=tsc(U"%Z %I:%M:%S %p"),.date_fmt=tsc(U"%d %b, %Y %Z %I:%M:%S %p"),.am_pm={tsc(U"ص"),tsc(U"م")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(U"^[+1نyY]"),.noexpr=tsc(U"^[-0لnN]"),.yesstr=tsc(U"نعم"),.nostr=tsc(U"لا")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"963")},.name={.name_fmt=tsc(U"%p%t%f%t%g"),.name_gen=tsc(U"-san"),.name_miss=tsc(U"Miss."),.name_mr=tsc(U"Mr."),.name_mrs=tsc(U"Mrs."),.name_ms=tsc(U"Ms.")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"سوريا"),.country_ab2=tsc(U"SY"),.country_ab3=tsc(U"SYR"),.country_num=760,.country_car=tsc(U"SYR"),.lang_name=tsc(U"العربية"),.lang_ab=tsc(U"ar"),.lang_term=tsc(U"ara"),.lang_lib=tsc(U"ara")},.measurement={.measurement=1}};


}
}

#include"../main.h"