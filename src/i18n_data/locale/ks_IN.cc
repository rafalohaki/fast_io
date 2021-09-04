#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t monetary_mon_grouping_storage[]{3,2};

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Kashmiri language locale for India"),.source=tsc("Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc("Marisfot III, Marigold Premises, East-Wing, Kalyaninagar, Pune, India-411014\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Kashmiri"),.territory=tsc("India"),.revision=tsc("1.0"),.date=tsc("2009,April,06")},.monetary={.int_curr_symbol=tsc("INR "),.currency_symbol=tsc("₹"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("آتهوار"),tsc("ژءنتروار"),tsc("بوءںوار"),tsc("بودهوار"),tsc("برىسوار"),tsc("جمع"),tsc("بٹوار")},.day={tsc("آتهوار"),tsc("ژءندروار"),tsc("بوءںوار"),tsc("بودهوار"),tsc("برىسوار"),tsc("جمع"),tsc("بٹوار")},.abmon={tsc("جنؤری"),tsc("فرؤری"),tsc("مارٕچ"),tsc("اپریل"),tsc("میٔ"),tsc("جوٗن"),tsc("جوٗلایی"),tsc("اگست"),tsc("ستمبر"),tsc("اکتوٗبر"),tsc("نومبر"),tsc("دسمبر")},.mon={tsc("جنؤری"),tsc("فرؤری"),tsc("مارٕچ"),tsc("اپریل"),tsc("میٔ"),tsc("جوٗن"),tsc("جوٗلایی"),tsc("اگست"),tsc("ستمبر"),tsc("اکتوٗبر"),tsc("نومبر"),tsc("دسمبر")},.d_t_fmt=tsc("%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc("%-m//%-d//%y"),.t_fmt=tsc("%I:%M:%S %p %Z"),.t_fmt_ampm=tsc("%I:%M:%S %p %Z"),.date_fmt=tsc("%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc("دوپھربرونھ"),tsc("دوپھرپتھ")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yYآ]"),.noexpr=tsc("^[-0nNن]"),.yesstr=tsc("آ"),.nostr=tsc("نع")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("91")},.name={.name_fmt=tsc("%p%t%f%t%g"),.name_gen=tsc(""),.name_miss=tsc("مسمات"),.name_mr=tsc("جناب"),.name_mrs=tsc("مسمات"),.name_ms=tsc("محترمه")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("ہِندوستان"),.country_ab2=tsc("IN"),.country_ab3=tsc("IND"),.country_num=356,.country_car=tsc("IND"),.lang_name=tsc("کٲشُر"),.lang_ab=tsc("ks"),.lang_term=tsc("kas"),.lang_lib=tsc("kas")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Kashmiri language locale for India"),.source=tsc(L"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(L"Marisfot III, Marigold Premises, East-Wing, Kalyaninagar, Pune, India-411014\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Kashmiri"),.territory=tsc(L"India"),.revision=tsc(L"1.0"),.date=tsc(L"2009,April,06")},.monetary={.int_curr_symbol=tsc(L"INR "),.currency_symbol=tsc(L"₹"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"آتهوار"),tsc(L"ژءنتروار"),tsc(L"بوءںوار"),tsc(L"بودهوار"),tsc(L"برىسوار"),tsc(L"جمع"),tsc(L"بٹوار")},.day={tsc(L"آتهوار"),tsc(L"ژءندروار"),tsc(L"بوءںوار"),tsc(L"بودهوار"),tsc(L"برىسوار"),tsc(L"جمع"),tsc(L"بٹوار")},.abmon={tsc(L"جنؤری"),tsc(L"فرؤری"),tsc(L"مارٕچ"),tsc(L"اپریل"),tsc(L"میٔ"),tsc(L"جوٗن"),tsc(L"جوٗلایی"),tsc(L"اگست"),tsc(L"ستمبر"),tsc(L"اکتوٗبر"),tsc(L"نومبر"),tsc(L"دسمبر")},.mon={tsc(L"جنؤری"),tsc(L"فرؤری"),tsc(L"مارٕچ"),tsc(L"اپریل"),tsc(L"میٔ"),tsc(L"جوٗن"),tsc(L"جوٗلایی"),tsc(L"اگست"),tsc(L"ستمبر"),tsc(L"اکتوٗبر"),tsc(L"نومبر"),tsc(L"دسمبر")},.d_t_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(L"%-m//%-d//%y"),.t_fmt=tsc(L"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(L"%I:%M:%S %p %Z"),.date_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(L"دوپھربرونھ"),tsc(L"دوپھرپتھ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yYآ]"),.noexpr=tsc(L"^[-0nNن]"),.yesstr=tsc(L"آ"),.nostr=tsc(L"نع")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"91")},.name={.name_fmt=tsc(L"%p%t%f%t%g"),.name_gen=tsc(L""),.name_miss=tsc(L"مسمات"),.name_mr=tsc(L"جناب"),.name_mrs=tsc(L"مسمات"),.name_ms=tsc(L"محترمه")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"ہِندوستان"),.country_ab2=tsc(L"IN"),.country_ab3=tsc(L"IND"),.country_num=356,.country_car=tsc(L"IND"),.lang_name=tsc(L"کٲشُر"),.lang_ab=tsc(L"ks"),.lang_term=tsc(L"kas"),.lang_lib=tsc(L"kas")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Kashmiri language locale for India"),.source=tsc(u8"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(u8"Marisfot III, Marigold Premises, East-Wing, Kalyaninagar, Pune, India-411014\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Kashmiri"),.territory=tsc(u8"India"),.revision=tsc(u8"1.0"),.date=tsc(u8"2009,April,06")},.monetary={.int_curr_symbol=tsc(u8"INR "),.currency_symbol=tsc(u8"₹"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"آتهوار"),tsc(u8"ژءنتروار"),tsc(u8"بوءںوار"),tsc(u8"بودهوار"),tsc(u8"برىسوار"),tsc(u8"جمع"),tsc(u8"بٹوار")},.day={tsc(u8"آتهوار"),tsc(u8"ژءندروار"),tsc(u8"بوءںوار"),tsc(u8"بودهوار"),tsc(u8"برىسوار"),tsc(u8"جمع"),tsc(u8"بٹوار")},.abmon={tsc(u8"جنؤری"),tsc(u8"فرؤری"),tsc(u8"مارٕچ"),tsc(u8"اپریل"),tsc(u8"میٔ"),tsc(u8"جوٗن"),tsc(u8"جوٗلایی"),tsc(u8"اگست"),tsc(u8"ستمبر"),tsc(u8"اکتوٗبر"),tsc(u8"نومبر"),tsc(u8"دسمبر")},.mon={tsc(u8"جنؤری"),tsc(u8"فرؤری"),tsc(u8"مارٕچ"),tsc(u8"اپریل"),tsc(u8"میٔ"),tsc(u8"جوٗن"),tsc(u8"جوٗلایی"),tsc(u8"اگست"),tsc(u8"ستمبر"),tsc(u8"اکتوٗبر"),tsc(u8"نومبر"),tsc(u8"دسمبر")},.d_t_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u8"%-m//%-d//%y"),.t_fmt=tsc(u8"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p %Z"),.date_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u8"دوپھربرونھ"),tsc(u8"دوپھرپتھ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yYآ]"),.noexpr=tsc(u8"^[-0nNن]"),.yesstr=tsc(u8"آ"),.nostr=tsc(u8"نع")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"91")},.name={.name_fmt=tsc(u8"%p%t%f%t%g"),.name_gen=tsc(u8""),.name_miss=tsc(u8"مسمات"),.name_mr=tsc(u8"جناب"),.name_mrs=tsc(u8"مسمات"),.name_ms=tsc(u8"محترمه")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"ہِندوستان"),.country_ab2=tsc(u8"IN"),.country_ab3=tsc(u8"IND"),.country_num=356,.country_car=tsc(u8"IND"),.lang_name=tsc(u8"کٲشُر"),.lang_ab=tsc(u8"ks"),.lang_term=tsc(u8"kas"),.lang_lib=tsc(u8"kas")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Kashmiri language locale for India"),.source=tsc(u"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(u"Marisfot III, Marigold Premises, East-Wing, Kalyaninagar, Pune, India-411014\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Kashmiri"),.territory=tsc(u"India"),.revision=tsc(u"1.0"),.date=tsc(u"2009,April,06")},.monetary={.int_curr_symbol=tsc(u"INR "),.currency_symbol=tsc(u"₹"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"آتهوار"),tsc(u"ژءنتروار"),tsc(u"بوءںوار"),tsc(u"بودهوار"),tsc(u"برىسوار"),tsc(u"جمع"),tsc(u"بٹوار")},.day={tsc(u"آتهوار"),tsc(u"ژءندروار"),tsc(u"بوءںوار"),tsc(u"بودهوار"),tsc(u"برىسوار"),tsc(u"جمع"),tsc(u"بٹوار")},.abmon={tsc(u"جنؤری"),tsc(u"فرؤری"),tsc(u"مارٕچ"),tsc(u"اپریل"),tsc(u"میٔ"),tsc(u"جوٗن"),tsc(u"جوٗلایی"),tsc(u"اگست"),tsc(u"ستمبر"),tsc(u"اکتوٗبر"),tsc(u"نومبر"),tsc(u"دسمبر")},.mon={tsc(u"جنؤری"),tsc(u"فرؤری"),tsc(u"مارٕچ"),tsc(u"اپریل"),tsc(u"میٔ"),tsc(u"جوٗن"),tsc(u"جوٗلایی"),tsc(u"اگست"),tsc(u"ستمبر"),tsc(u"اکتوٗبر"),tsc(u"نومبر"),tsc(u"دسمبر")},.d_t_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u"%-m//%-d//%y"),.t_fmt=tsc(u"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u"%I:%M:%S %p %Z"),.date_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u"دوپھربرونھ"),tsc(u"دوپھرپتھ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yYآ]"),.noexpr=tsc(u"^[-0nNن]"),.yesstr=tsc(u"آ"),.nostr=tsc(u"نع")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"91")},.name={.name_fmt=tsc(u"%p%t%f%t%g"),.name_gen=tsc(u""),.name_miss=tsc(u"مسمات"),.name_mr=tsc(u"جناب"),.name_mrs=tsc(u"مسمات"),.name_ms=tsc(u"محترمه")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"ہِندوستان"),.country_ab2=tsc(u"IN"),.country_ab3=tsc(u"IND"),.country_num=356,.country_car=tsc(u"IND"),.lang_name=tsc(u"کٲشُر"),.lang_ab=tsc(u"ks"),.lang_term=tsc(u"kas"),.lang_lib=tsc(u"kas")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Kashmiri language locale for India"),.source=tsc(U"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(U"Marisfot III, Marigold Premises, East-Wing, Kalyaninagar, Pune, India-411014\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Kashmiri"),.territory=tsc(U"India"),.revision=tsc(U"1.0"),.date=tsc(U"2009,April,06")},.monetary={.int_curr_symbol=tsc(U"INR "),.currency_symbol=tsc(U"₹"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"آتهوار"),tsc(U"ژءنتروار"),tsc(U"بوءںوار"),tsc(U"بودهوار"),tsc(U"برىسوار"),tsc(U"جمع"),tsc(U"بٹوار")},.day={tsc(U"آتهوار"),tsc(U"ژءندروار"),tsc(U"بوءںوار"),tsc(U"بودهوار"),tsc(U"برىسوار"),tsc(U"جمع"),tsc(U"بٹوار")},.abmon={tsc(U"جنؤری"),tsc(U"فرؤری"),tsc(U"مارٕچ"),tsc(U"اپریل"),tsc(U"میٔ"),tsc(U"جوٗن"),tsc(U"جوٗلایی"),tsc(U"اگست"),tsc(U"ستمبر"),tsc(U"اکتوٗبر"),tsc(U"نومبر"),tsc(U"دسمبر")},.mon={tsc(U"جنؤری"),tsc(U"فرؤری"),tsc(U"مارٕچ"),tsc(U"اپریل"),tsc(U"میٔ"),tsc(U"جوٗن"),tsc(U"جوٗلایی"),tsc(U"اگست"),tsc(U"ستمبر"),tsc(U"اکتوٗبر"),tsc(U"نومبر"),tsc(U"دسمبر")},.d_t_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(U"%-m//%-d//%y"),.t_fmt=tsc(U"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(U"%I:%M:%S %p %Z"),.date_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(U"دوپھربرونھ"),tsc(U"دوپھرپتھ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yYآ]"),.noexpr=tsc(U"^[-0nNن]"),.yesstr=tsc(U"آ"),.nostr=tsc(U"نع")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"91")},.name={.name_fmt=tsc(U"%p%t%f%t%g"),.name_gen=tsc(U""),.name_miss=tsc(U"مسمات"),.name_mr=tsc(U"جناب"),.name_mrs=tsc(U"مسمات"),.name_ms=tsc(U"محترمه")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"ہِندوستان"),.country_ab2=tsc(U"IN"),.country_ab3=tsc(U"IND"),.country_num=356,.country_car=tsc(U"IND"),.lang_name=tsc(U"کٲشُر"),.lang_ab=tsc(U"ks"),.lang_term=tsc(U"kas"),.lang_lib=tsc(U"kas")},.measurement={.measurement=1}};


}
}

#include"../main.h"