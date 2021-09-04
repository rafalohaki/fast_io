#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t monetary_mon_grouping_storage[]{3,2};

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Konkani language locale for India"),.source=tsc("Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc("Level-1, Tower 10, Cyber City, Magarpatta City, Hadapsar, Pune, Maharashtra, India-411013\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Konkani"),.territory=tsc("India"),.revision=tsc("1.0"),.date=tsc("2009,December,08")},.monetary={.int_curr_symbol=tsc("INR "),.currency_symbol=tsc("₹"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("आयतार"),tsc("सोमार"),tsc("मंगळवार"),tsc("बुधवार"),tsc("बेरेसतार"),tsc("शुकरार"),tsc("शेनवार")},.day={tsc("आयतार"),tsc("सोमार"),tsc("मंगळवार"),tsc("बुधवार"),tsc("बेरेसतार"),tsc("शुकरार"),tsc("शेनवार")},.abmon={tsc("जानेवारी"),tsc("फेब्रुवारी"),tsc("मार्च"),tsc("एप्रिल"),tsc("मे"),tsc("जून"),tsc("जुलै"),tsc("ओगस्ट"),tsc("सेप्टेंबर"),tsc("ओक्टोबर"),tsc("नोव्हेंबर"),tsc("डिसेंबर")},.mon={tsc("जानेवारी"),tsc("फेब्रुवारी"),tsc("मार्च"),tsc("एप्रिल"),tsc("मे"),tsc("जून"),tsc("जुलै"),tsc("ओगस्ट"),tsc("सेप्टेंबर"),tsc("ओक्टोबर"),tsc("नोव्हेंबर"),tsc("डिसेंबर")},.d_t_fmt=tsc("%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc("%-d-%-m-%y"),.t_fmt=tsc("%I:%M:%S %p %Z"),.t_fmt_ampm=tsc("%I:%M:%S %p %Z"),.date_fmt=tsc("%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc("म.पू."),tsc("म.नं.")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yYह]"),.noexpr=tsc("^[-0nNन]"),.yesstr=tsc("हय"),.nostr=tsc("न्ही")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("91")},.name={.name_fmt=tsc("%p%t%f%t%g"),.name_gen=tsc(""),.name_miss=tsc("कुमारी"),.name_mr=tsc("श्री"),.name_mrs=tsc("श्रीमती"),.name_ms=tsc("कुमार")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("भारत"),.country_ab2=tsc("IN"),.country_ab3=tsc("IND"),.country_num=356,.country_car=tsc("IND"),.lang_name=tsc("कोंकणी"),.lang_term=tsc("kok"),.lang_lib=tsc("kok")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Konkani language locale for India"),.source=tsc(L"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(L"Level-1, Tower 10, Cyber City, Magarpatta City, Hadapsar, Pune, Maharashtra, India-411013\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Konkani"),.territory=tsc(L"India"),.revision=tsc(L"1.0"),.date=tsc(L"2009,December,08")},.monetary={.int_curr_symbol=tsc(L"INR "),.currency_symbol=tsc(L"₹"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"आयतार"),tsc(L"सोमार"),tsc(L"मंगळवार"),tsc(L"बुधवार"),tsc(L"बेरेसतार"),tsc(L"शुकरार"),tsc(L"शेनवार")},.day={tsc(L"आयतार"),tsc(L"सोमार"),tsc(L"मंगळवार"),tsc(L"बुधवार"),tsc(L"बेरेसतार"),tsc(L"शुकरार"),tsc(L"शेनवार")},.abmon={tsc(L"जानेवारी"),tsc(L"फेब्रुवारी"),tsc(L"मार्च"),tsc(L"एप्रिल"),tsc(L"मे"),tsc(L"जून"),tsc(L"जुलै"),tsc(L"ओगस्ट"),tsc(L"सेप्टेंबर"),tsc(L"ओक्टोबर"),tsc(L"नोव्हेंबर"),tsc(L"डिसेंबर")},.mon={tsc(L"जानेवारी"),tsc(L"फेब्रुवारी"),tsc(L"मार्च"),tsc(L"एप्रिल"),tsc(L"मे"),tsc(L"जून"),tsc(L"जुलै"),tsc(L"ओगस्ट"),tsc(L"सेप्टेंबर"),tsc(L"ओक्टोबर"),tsc(L"नोव्हेंबर"),tsc(L"डिसेंबर")},.d_t_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(L"%-d-%-m-%y"),.t_fmt=tsc(L"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(L"%I:%M:%S %p %Z"),.date_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(L"म.पू."),tsc(L"म.नं.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yYह]"),.noexpr=tsc(L"^[-0nNन]"),.yesstr=tsc(L"हय"),.nostr=tsc(L"न्ही")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"91")},.name={.name_fmt=tsc(L"%p%t%f%t%g"),.name_gen=tsc(L""),.name_miss=tsc(L"कुमारी"),.name_mr=tsc(L"श्री"),.name_mrs=tsc(L"श्रीमती"),.name_ms=tsc(L"कुमार")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"भारत"),.country_ab2=tsc(L"IN"),.country_ab3=tsc(L"IND"),.country_num=356,.country_car=tsc(L"IND"),.lang_name=tsc(L"कोंकणी"),.lang_term=tsc(L"kok"),.lang_lib=tsc(L"kok")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Konkani language locale for India"),.source=tsc(u8"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(u8"Level-1, Tower 10, Cyber City, Magarpatta City, Hadapsar, Pune, Maharashtra, India-411013\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Konkani"),.territory=tsc(u8"India"),.revision=tsc(u8"1.0"),.date=tsc(u8"2009,December,08")},.monetary={.int_curr_symbol=tsc(u8"INR "),.currency_symbol=tsc(u8"₹"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"आयतार"),tsc(u8"सोमार"),tsc(u8"मंगळवार"),tsc(u8"बुधवार"),tsc(u8"बेरेसतार"),tsc(u8"शुकरार"),tsc(u8"शेनवार")},.day={tsc(u8"आयतार"),tsc(u8"सोमार"),tsc(u8"मंगळवार"),tsc(u8"बुधवार"),tsc(u8"बेरेसतार"),tsc(u8"शुकरार"),tsc(u8"शेनवार")},.abmon={tsc(u8"जानेवारी"),tsc(u8"फेब्रुवारी"),tsc(u8"मार्च"),tsc(u8"एप्रिल"),tsc(u8"मे"),tsc(u8"जून"),tsc(u8"जुलै"),tsc(u8"ओगस्ट"),tsc(u8"सेप्टेंबर"),tsc(u8"ओक्टोबर"),tsc(u8"नोव्हेंबर"),tsc(u8"डिसेंबर")},.mon={tsc(u8"जानेवारी"),tsc(u8"फेब्रुवारी"),tsc(u8"मार्च"),tsc(u8"एप्रिल"),tsc(u8"मे"),tsc(u8"जून"),tsc(u8"जुलै"),tsc(u8"ओगस्ट"),tsc(u8"सेप्टेंबर"),tsc(u8"ओक्टोबर"),tsc(u8"नोव्हेंबर"),tsc(u8"डिसेंबर")},.d_t_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u8"%-d-%-m-%y"),.t_fmt=tsc(u8"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p %Z"),.date_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u8"म.पू."),tsc(u8"म.नं.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yYह]"),.noexpr=tsc(u8"^[-0nNन]"),.yesstr=tsc(u8"हय"),.nostr=tsc(u8"न्ही")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"91")},.name={.name_fmt=tsc(u8"%p%t%f%t%g"),.name_gen=tsc(u8""),.name_miss=tsc(u8"कुमारी"),.name_mr=tsc(u8"श्री"),.name_mrs=tsc(u8"श्रीमती"),.name_ms=tsc(u8"कुमार")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"भारत"),.country_ab2=tsc(u8"IN"),.country_ab3=tsc(u8"IND"),.country_num=356,.country_car=tsc(u8"IND"),.lang_name=tsc(u8"कोंकणी"),.lang_term=tsc(u8"kok"),.lang_lib=tsc(u8"kok")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Konkani language locale for India"),.source=tsc(u"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(u"Level-1, Tower 10, Cyber City, Magarpatta City, Hadapsar, Pune, Maharashtra, India-411013\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Konkani"),.territory=tsc(u"India"),.revision=tsc(u"1.0"),.date=tsc(u"2009,December,08")},.monetary={.int_curr_symbol=tsc(u"INR "),.currency_symbol=tsc(u"₹"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"आयतार"),tsc(u"सोमार"),tsc(u"मंगळवार"),tsc(u"बुधवार"),tsc(u"बेरेसतार"),tsc(u"शुकरार"),tsc(u"शेनवार")},.day={tsc(u"आयतार"),tsc(u"सोमार"),tsc(u"मंगळवार"),tsc(u"बुधवार"),tsc(u"बेरेसतार"),tsc(u"शुकरार"),tsc(u"शेनवार")},.abmon={tsc(u"जानेवारी"),tsc(u"फेब्रुवारी"),tsc(u"मार्च"),tsc(u"एप्रिल"),tsc(u"मे"),tsc(u"जून"),tsc(u"जुलै"),tsc(u"ओगस्ट"),tsc(u"सेप्टेंबर"),tsc(u"ओक्टोबर"),tsc(u"नोव्हेंबर"),tsc(u"डिसेंबर")},.mon={tsc(u"जानेवारी"),tsc(u"फेब्रुवारी"),tsc(u"मार्च"),tsc(u"एप्रिल"),tsc(u"मे"),tsc(u"जून"),tsc(u"जुलै"),tsc(u"ओगस्ट"),tsc(u"सेप्टेंबर"),tsc(u"ओक्टोबर"),tsc(u"नोव्हेंबर"),tsc(u"डिसेंबर")},.d_t_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u"%-d-%-m-%y"),.t_fmt=tsc(u"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u"%I:%M:%S %p %Z"),.date_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u"म.पू."),tsc(u"म.नं.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yYह]"),.noexpr=tsc(u"^[-0nNन]"),.yesstr=tsc(u"हय"),.nostr=tsc(u"न्ही")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"91")},.name={.name_fmt=tsc(u"%p%t%f%t%g"),.name_gen=tsc(u""),.name_miss=tsc(u"कुमारी"),.name_mr=tsc(u"श्री"),.name_mrs=tsc(u"श्रीमती"),.name_ms=tsc(u"कुमार")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"भारत"),.country_ab2=tsc(u"IN"),.country_ab3=tsc(u"IND"),.country_num=356,.country_car=tsc(u"IND"),.lang_name=tsc(u"कोंकणी"),.lang_term=tsc(u"kok"),.lang_lib=tsc(u"kok")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Konkani language locale for India"),.source=tsc(U"Red Hat, Pune\t\t;\t\tfast_io"),.address=tsc(U"Level-1, Tower 10, Cyber City, Magarpatta City, Hadapsar, Pune, Maharashtra, India-411013\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Konkani"),.territory=tsc(U"India"),.revision=tsc(U"1.0"),.date=tsc(U"2009,December,08")},.monetary={.int_curr_symbol=tsc(U"INR "),.currency_symbol=tsc(U"₹"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"आयतार"),tsc(U"सोमार"),tsc(U"मंगळवार"),tsc(U"बुधवार"),tsc(U"बेरेसतार"),tsc(U"शुकरार"),tsc(U"शेनवार")},.day={tsc(U"आयतार"),tsc(U"सोमार"),tsc(U"मंगळवार"),tsc(U"बुधवार"),tsc(U"बेरेसतार"),tsc(U"शुकरार"),tsc(U"शेनवार")},.abmon={tsc(U"जानेवारी"),tsc(U"फेब्रुवारी"),tsc(U"मार्च"),tsc(U"एप्रिल"),tsc(U"मे"),tsc(U"जून"),tsc(U"जुलै"),tsc(U"ओगस्ट"),tsc(U"सेप्टेंबर"),tsc(U"ओक्टोबर"),tsc(U"नोव्हेंबर"),tsc(U"डिसेंबर")},.mon={tsc(U"जानेवारी"),tsc(U"फेब्रुवारी"),tsc(U"मार्च"),tsc(U"एप्रिल"),tsc(U"मे"),tsc(U"जून"),tsc(U"जुलै"),tsc(U"ओगस्ट"),tsc(U"सेप्टेंबर"),tsc(U"ओक्टोबर"),tsc(U"नोव्हेंबर"),tsc(U"डिसेंबर")},.d_t_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(U"%-d-%-m-%y"),.t_fmt=tsc(U"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(U"%I:%M:%S %p %Z"),.date_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(U"म.पू."),tsc(U"म.नं.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yYह]"),.noexpr=tsc(U"^[-0nNन]"),.yesstr=tsc(U"हय"),.nostr=tsc(U"न्ही")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"91")},.name={.name_fmt=tsc(U"%p%t%f%t%g"),.name_gen=tsc(U""),.name_miss=tsc(U"कुमारी"),.name_mr=tsc(U"श्री"),.name_mrs=tsc(U"श्रीमती"),.name_ms=tsc(U"कुमार")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"भारत"),.country_ab2=tsc(U"IN"),.country_ab3=tsc(U"IND"),.country_num=356,.country_car=tsc(U"IND"),.lang_name=tsc(U"कोंकणी"),.lang_term=tsc(U"kok"),.lang_lib=tsc(U"kok")},.measurement={.measurement=1}};


}
}

#include"../main.h"