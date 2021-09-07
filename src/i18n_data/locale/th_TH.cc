#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr basic_lc_time_era<char> time_era_storage[]{{.direction=true,.offset=1,.start_date_year=-543,.start_date_month=1,.start_date_day=1,.end_date_special=1,.era_name=tsc("พ.ศ."),.era_format=tsc("%EC %Ey")}};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Thai locale for Thailand"),.source=tsc("fast_io"),.address=tsc("https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Thai"),.territory=tsc("Thailand"),.revision=tsc("1.0"),.date=tsc("2000-06-29")},.monetary={.int_curr_symbol=tsc("THB "),.currency_symbol=tsc("฿"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("อา."),tsc("จ."),tsc("อ."),tsc("พ."),tsc("พฤ."),tsc("ศ."),tsc("ส.")},.day={tsc("อาทิตย์"),tsc("จันทร์"),tsc("อังคาร"),tsc("พุธ"),tsc("พฤหัสบดี"),tsc("ศุกร์"),tsc("เสาร์")},.abmon={tsc("ม.ค."),tsc("ก.พ."),tsc("มี.ค."),tsc("เม.ย."),tsc("พ.ค."),tsc("มิ.ย."),tsc("ก.ค."),tsc("ส.ค."),tsc("ก.ย."),tsc("ต.ค."),tsc("พ.ย."),tsc("ธ.ค.")},.mon={tsc("มกราคม"),tsc("กุมภาพันธ์"),tsc("มีนาคม"),tsc("เมษายน"),tsc("พฤษภาคม"),tsc("มิถุนายน"),tsc("กรกฎาคม"),tsc("สิงหาคม"),tsc("กันยายน"),tsc("ตุลาคม"),tsc("พฤศจิกายน"),tsc("ธันวาคม")},.d_t_fmt=tsc("%a %e %b %Ey, %H:%M:%S"),.d_fmt=tsc("%d//%m//%Ey"),.t_fmt=tsc("%H:%M:%S"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %e %b %Ey %H:%M:%S %Z"),.am_pm={tsc("AM"),tsc("PM")},.era={time_era_storage,1},.era_d_fmt=tsc("%e %b %Ey"),.era_d_t_fmt=tsc("วัน%Aที่ %e %B %EC %Ey, %H.%M.%S น."),.era_t_fmt=tsc("%H.%M.%S น."),.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yYช]"),.noexpr=tsc("^[-0nNม]"),.yesstr=tsc("ใช่"),.nostr=tsc("ไม่ใช่")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("0-%a%l"),.int_select=tsc("001"),.int_prefix=tsc("66")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f"),.name_gen=tsc("คุณ"),.name_miss=tsc("นางสาว"),.name_mr=tsc("นาย"),.name_mrs=tsc("นาง")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%r%t%e%t%b%N%h%t%s%N%T%N%S%N%z%c%N"),.country_name=tsc("ไทย"),.country_ab2=tsc("TH"),.country_ab3=tsc("THA"),.country_num=764,.country_car=tsc("T"),.lang_name=tsc("ไทย"),.lang_ab=tsc("th"),.lang_term=tsc("tha"),.lang_lib=tsc("tha")},.measurement={.measurement=1}};

inline constexpr basic_lc_time_era<wchar_t> wtime_era_storage[]{{.direction=true,.offset=1,.start_date_year=-543,.start_date_month=1,.start_date_day=1,.end_date_special=1,.era_name=tsc(L"พ.ศ."),.era_format=tsc(L"%EC %Ey")}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Thai locale for Thailand"),.source=tsc(L"fast_io"),.address=tsc(L"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Thai"),.territory=tsc(L"Thailand"),.revision=tsc(L"1.0"),.date=tsc(L"2000-06-29")},.monetary={.int_curr_symbol=tsc(L"THB "),.currency_symbol=tsc(L"฿"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"อา."),tsc(L"จ."),tsc(L"อ."),tsc(L"พ."),tsc(L"พฤ."),tsc(L"ศ."),tsc(L"ส.")},.day={tsc(L"อาทิตย์"),tsc(L"จันทร์"),tsc(L"อังคาร"),tsc(L"พุธ"),tsc(L"พฤหัสบดี"),tsc(L"ศุกร์"),tsc(L"เสาร์")},.abmon={tsc(L"ม.ค."),tsc(L"ก.พ."),tsc(L"มี.ค."),tsc(L"เม.ย."),tsc(L"พ.ค."),tsc(L"มิ.ย."),tsc(L"ก.ค."),tsc(L"ส.ค."),tsc(L"ก.ย."),tsc(L"ต.ค."),tsc(L"พ.ย."),tsc(L"ธ.ค.")},.mon={tsc(L"มกราคม"),tsc(L"กุมภาพันธ์"),tsc(L"มีนาคม"),tsc(L"เมษายน"),tsc(L"พฤษภาคม"),tsc(L"มิถุนายน"),tsc(L"กรกฎาคม"),tsc(L"สิงหาคม"),tsc(L"กันยายน"),tsc(L"ตุลาคม"),tsc(L"พฤศจิกายน"),tsc(L"ธันวาคม")},.d_t_fmt=tsc(L"%a %e %b %Ey, %H:%M:%S"),.d_fmt=tsc(L"%d//%m//%Ey"),.t_fmt=tsc(L"%H:%M:%S"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %e %b %Ey %H:%M:%S %Z"),.am_pm={tsc(L"AM"),tsc(L"PM")},.era={wtime_era_storage,1},.era_d_fmt=tsc(L"%e %b %Ey"),.era_d_t_fmt=tsc(L"วัน%Aที่ %e %B %EC %Ey, %H.%M.%S น."),.era_t_fmt=tsc(L"%H.%M.%S น."),.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yYช]"),.noexpr=tsc(L"^[-0nNม]"),.yesstr=tsc(L"ใช่"),.nostr=tsc(L"ไม่ใช่")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"0-%a%l"),.int_select=tsc(L"001"),.int_prefix=tsc(L"66")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f"),.name_gen=tsc(L"คุณ"),.name_miss=tsc(L"นางสาว"),.name_mr=tsc(L"นาย"),.name_mrs=tsc(L"นาง")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%r%t%e%t%b%N%h%t%s%N%T%N%S%N%z%c%N"),.country_name=tsc(L"ไทย"),.country_ab2=tsc(L"TH"),.country_ab3=tsc(L"THA"),.country_num=764,.country_car=tsc(L"T"),.lang_name=tsc(L"ไทย"),.lang_ab=tsc(L"th"),.lang_term=tsc(L"tha"),.lang_lib=tsc(L"tha")},.measurement={.measurement=1}};

inline constexpr basic_lc_time_era<char8_t> u8time_era_storage[]{{.direction=true,.offset=1,.start_date_year=-543,.start_date_month=1,.start_date_day=1,.end_date_special=1,.era_name=tsc(u8"พ.ศ."),.era_format=tsc(u8"%EC %Ey")}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Thai locale for Thailand"),.source=tsc(u8"fast_io"),.address=tsc(u8"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Thai"),.territory=tsc(u8"Thailand"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-06-29")},.monetary={.int_curr_symbol=tsc(u8"THB "),.currency_symbol=tsc(u8"฿"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"อา."),tsc(u8"จ."),tsc(u8"อ."),tsc(u8"พ."),tsc(u8"พฤ."),tsc(u8"ศ."),tsc(u8"ส.")},.day={tsc(u8"อาทิตย์"),tsc(u8"จันทร์"),tsc(u8"อังคาร"),tsc(u8"พุธ"),tsc(u8"พฤหัสบดี"),tsc(u8"ศุกร์"),tsc(u8"เสาร์")},.abmon={tsc(u8"ม.ค."),tsc(u8"ก.พ."),tsc(u8"มี.ค."),tsc(u8"เม.ย."),tsc(u8"พ.ค."),tsc(u8"มิ.ย."),tsc(u8"ก.ค."),tsc(u8"ส.ค."),tsc(u8"ก.ย."),tsc(u8"ต.ค."),tsc(u8"พ.ย."),tsc(u8"ธ.ค.")},.mon={tsc(u8"มกราคม"),tsc(u8"กุมภาพันธ์"),tsc(u8"มีนาคม"),tsc(u8"เมษายน"),tsc(u8"พฤษภาคม"),tsc(u8"มิถุนายน"),tsc(u8"กรกฎาคม"),tsc(u8"สิงหาคม"),tsc(u8"กันยายน"),tsc(u8"ตุลาคม"),tsc(u8"พฤศจิกายน"),tsc(u8"ธันวาคม")},.d_t_fmt=tsc(u8"%a %e %b %Ey, %H:%M:%S"),.d_fmt=tsc(u8"%d//%m//%Ey"),.t_fmt=tsc(u8"%H:%M:%S"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %e %b %Ey %H:%M:%S %Z"),.am_pm={tsc(u8"AM"),tsc(u8"PM")},.era={u8time_era_storage,1},.era_d_fmt=tsc(u8"%e %b %Ey"),.era_d_t_fmt=tsc(u8"วัน%Aที่ %e %B %EC %Ey, %H.%M.%S น."),.era_t_fmt=tsc(u8"%H.%M.%S น."),.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yYช]"),.noexpr=tsc(u8"^[-0nNม]"),.yesstr=tsc(u8"ใช่"),.nostr=tsc(u8"ไม่ใช่")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"0-%a%l"),.int_select=tsc(u8"001"),.int_prefix=tsc(u8"66")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f"),.name_gen=tsc(u8"คุณ"),.name_miss=tsc(u8"นางสาว"),.name_mr=tsc(u8"นาย"),.name_mrs=tsc(u8"นาง")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%r%t%e%t%b%N%h%t%s%N%T%N%S%N%z%c%N"),.country_name=tsc(u8"ไทย"),.country_ab2=tsc(u8"TH"),.country_ab3=tsc(u8"THA"),.country_num=764,.country_car=tsc(u8"T"),.lang_name=tsc(u8"ไทย"),.lang_ab=tsc(u8"th"),.lang_term=tsc(u8"tha"),.lang_lib=tsc(u8"tha")},.measurement={.measurement=1}};

inline constexpr basic_lc_time_era<char16_t> u16time_era_storage[]{{.direction=true,.offset=1,.start_date_year=-543,.start_date_month=1,.start_date_day=1,.end_date_special=1,.era_name=tsc(u"พ.ศ."),.era_format=tsc(u"%EC %Ey")}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Thai locale for Thailand"),.source=tsc(u"fast_io"),.address=tsc(u"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Thai"),.territory=tsc(u"Thailand"),.revision=tsc(u"1.0"),.date=tsc(u"2000-06-29")},.monetary={.int_curr_symbol=tsc(u"THB "),.currency_symbol=tsc(u"฿"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"อา."),tsc(u"จ."),tsc(u"อ."),tsc(u"พ."),tsc(u"พฤ."),tsc(u"ศ."),tsc(u"ส.")},.day={tsc(u"อาทิตย์"),tsc(u"จันทร์"),tsc(u"อังคาร"),tsc(u"พุธ"),tsc(u"พฤหัสบดี"),tsc(u"ศุกร์"),tsc(u"เสาร์")},.abmon={tsc(u"ม.ค."),tsc(u"ก.พ."),tsc(u"มี.ค."),tsc(u"เม.ย."),tsc(u"พ.ค."),tsc(u"มิ.ย."),tsc(u"ก.ค."),tsc(u"ส.ค."),tsc(u"ก.ย."),tsc(u"ต.ค."),tsc(u"พ.ย."),tsc(u"ธ.ค.")},.mon={tsc(u"มกราคม"),tsc(u"กุมภาพันธ์"),tsc(u"มีนาคม"),tsc(u"เมษายน"),tsc(u"พฤษภาคม"),tsc(u"มิถุนายน"),tsc(u"กรกฎาคม"),tsc(u"สิงหาคม"),tsc(u"กันยายน"),tsc(u"ตุลาคม"),tsc(u"พฤศจิกายน"),tsc(u"ธันวาคม")},.d_t_fmt=tsc(u"%a %e %b %Ey, %H:%M:%S"),.d_fmt=tsc(u"%d//%m//%Ey"),.t_fmt=tsc(u"%H:%M:%S"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %e %b %Ey %H:%M:%S %Z"),.am_pm={tsc(u"AM"),tsc(u"PM")},.era={u16time_era_storage,1},.era_d_fmt=tsc(u"%e %b %Ey"),.era_d_t_fmt=tsc(u"วัน%Aที่ %e %B %EC %Ey, %H.%M.%S น."),.era_t_fmt=tsc(u"%H.%M.%S น."),.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yYช]"),.noexpr=tsc(u"^[-0nNม]"),.yesstr=tsc(u"ใช่"),.nostr=tsc(u"ไม่ใช่")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"0-%a%l"),.int_select=tsc(u"001"),.int_prefix=tsc(u"66")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f"),.name_gen=tsc(u"คุณ"),.name_miss=tsc(u"นางสาว"),.name_mr=tsc(u"นาย"),.name_mrs=tsc(u"นาง")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%r%t%e%t%b%N%h%t%s%N%T%N%S%N%z%c%N"),.country_name=tsc(u"ไทย"),.country_ab2=tsc(u"TH"),.country_ab3=tsc(u"THA"),.country_num=764,.country_car=tsc(u"T"),.lang_name=tsc(u"ไทย"),.lang_ab=tsc(u"th"),.lang_term=tsc(u"tha"),.lang_lib=tsc(u"tha")},.measurement={.measurement=1}};

inline constexpr basic_lc_time_era<char32_t> u32time_era_storage[]{{.direction=true,.offset=1,.start_date_year=-543,.start_date_month=1,.start_date_day=1,.end_date_special=1,.era_name=tsc(U"พ.ศ."),.era_format=tsc(U"%EC %Ey")}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Thai locale for Thailand"),.source=tsc(U"fast_io"),.address=tsc(U"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Thai"),.territory=tsc(U"Thailand"),.revision=tsc(U"1.0"),.date=tsc(U"2000-06-29")},.monetary={.int_curr_symbol=tsc(U"THB "),.currency_symbol=tsc(U"฿"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"อา."),tsc(U"จ."),tsc(U"อ."),tsc(U"พ."),tsc(U"พฤ."),tsc(U"ศ."),tsc(U"ส.")},.day={tsc(U"อาทิตย์"),tsc(U"จันทร์"),tsc(U"อังคาร"),tsc(U"พุธ"),tsc(U"พฤหัสบดี"),tsc(U"ศุกร์"),tsc(U"เสาร์")},.abmon={tsc(U"ม.ค."),tsc(U"ก.พ."),tsc(U"มี.ค."),tsc(U"เม.ย."),tsc(U"พ.ค."),tsc(U"มิ.ย."),tsc(U"ก.ค."),tsc(U"ส.ค."),tsc(U"ก.ย."),tsc(U"ต.ค."),tsc(U"พ.ย."),tsc(U"ธ.ค.")},.mon={tsc(U"มกราคม"),tsc(U"กุมภาพันธ์"),tsc(U"มีนาคม"),tsc(U"เมษายน"),tsc(U"พฤษภาคม"),tsc(U"มิถุนายน"),tsc(U"กรกฎาคม"),tsc(U"สิงหาคม"),tsc(U"กันยายน"),tsc(U"ตุลาคม"),tsc(U"พฤศจิกายน"),tsc(U"ธันวาคม")},.d_t_fmt=tsc(U"%a %e %b %Ey, %H:%M:%S"),.d_fmt=tsc(U"%d//%m//%Ey"),.t_fmt=tsc(U"%H:%M:%S"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %e %b %Ey %H:%M:%S %Z"),.am_pm={tsc(U"AM"),tsc(U"PM")},.era={u32time_era_storage,1},.era_d_fmt=tsc(U"%e %b %Ey"),.era_d_t_fmt=tsc(U"วัน%Aที่ %e %B %EC %Ey, %H.%M.%S น."),.era_t_fmt=tsc(U"%H.%M.%S น."),.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yYช]"),.noexpr=tsc(U"^[-0nNม]"),.yesstr=tsc(U"ใช่"),.nostr=tsc(U"ไม่ใช่")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"0-%a%l"),.int_select=tsc(U"001"),.int_prefix=tsc(U"66")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f"),.name_gen=tsc(U"คุณ"),.name_miss=tsc(U"นางสาว"),.name_mr=tsc(U"นาย"),.name_mrs=tsc(U"นาง")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%r%t%e%t%b%N%h%t%s%N%T%N%S%N%z%c%N"),.country_name=tsc(U"ไทย"),.country_ab2=tsc(U"TH"),.country_ab3=tsc(U"THA"),.country_num=764,.country_car=tsc(U"T"),.lang_name=tsc(U"ไทย"),.lang_ab=tsc(U"th"),.lang_term=tsc(U"tha"),.lang_lib=tsc(U"tha")},.measurement={.measurement=1}};


}
}

#include"../main.h"