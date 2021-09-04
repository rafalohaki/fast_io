#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Romanian locale for Romania"),.source=tsc("RAP\t\t;\t\tfast_io"),.address=tsc("Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Romanian"),.territory=tsc("Romania"),.revision=tsc("1.0"),.date=tsc("2000-06-29")},.monetary={.int_curr_symbol=tsc("RON "),.currency_symbol=tsc("Lei"),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc("."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc("."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Du"),tsc("Lu"),tsc("Ma"),tsc("Mi"),tsc("Jo"),tsc("Vi"),tsc("Sb")},.day={tsc("duminică"),tsc("luni"),tsc("marți"),tsc("miercuri"),tsc("joi"),tsc("vineri"),tsc("sâmbătă")},.abmon={tsc("ian"),tsc("feb"),tsc("mar"),tsc("apr"),tsc("mai"),tsc("iun"),tsc("iul"),tsc("aug"),tsc("sep"),tsc("oct"),tsc("nov"),tsc("dec")},.mon={tsc("ianuarie"),tsc("februarie"),tsc("martie"),tsc("aprilie"),tsc("mai"),tsc("iunie"),tsc("iulie"),tsc("august"),tsc("septembrie"),tsc("octombrie"),tsc("noiembrie"),tsc("decembrie")},.d_t_fmt=tsc("%a %d %b %Y %T %z"),.d_fmt=tsc("%d.%m.%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%A %-e %B %Y, %H:%M:%S %z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1DdYy]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("da"),.nostr=tsc("nu")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00"),.int_prefix=tsc("40")},.name={.name_fmt=tsc("%d%t%s%t%f%t%g%t%m"),.name_miss=tsc("D-ra."),.name_mr=tsc("Dl."),.name_mrs=tsc("D-na.")},.address={.postal_fmt=tsc("%f%N%a%d%N%s%t%h%N%b%t%e%t%r%N%z%t%T%N%S%t%c%N"),.country_name=tsc("România"),.country_post=tsc("RO"),.country_ab2=tsc("RO"),.country_ab3=tsc("ROU"),.country_num=642,.country_car=tsc("RO"),.country_isbn=tsc("973"),.lang_name=tsc("română"),.lang_ab=tsc("ro"),.lang_term=tsc("ron"),.lang_lib=tsc("rum")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Romanian locale for Romania"),.source=tsc(L"RAP\t\t;\t\tfast_io"),.address=tsc(L"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Romanian"),.territory=tsc(L"Romania"),.revision=tsc(L"1.0"),.date=tsc(L"2000-06-29")},.monetary={.int_curr_symbol=tsc(L"RON "),.currency_symbol=tsc(L"Lei"),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Du"),tsc(L"Lu"),tsc(L"Ma"),tsc(L"Mi"),tsc(L"Jo"),tsc(L"Vi"),tsc(L"Sb")},.day={tsc(L"duminică"),tsc(L"luni"),tsc(L"marți"),tsc(L"miercuri"),tsc(L"joi"),tsc(L"vineri"),tsc(L"sâmbătă")},.abmon={tsc(L"ian"),tsc(L"feb"),tsc(L"mar"),tsc(L"apr"),tsc(L"mai"),tsc(L"iun"),tsc(L"iul"),tsc(L"aug"),tsc(L"sep"),tsc(L"oct"),tsc(L"nov"),tsc(L"dec")},.mon={tsc(L"ianuarie"),tsc(L"februarie"),tsc(L"martie"),tsc(L"aprilie"),tsc(L"mai"),tsc(L"iunie"),tsc(L"iulie"),tsc(L"august"),tsc(L"septembrie"),tsc(L"octombrie"),tsc(L"noiembrie"),tsc(L"decembrie")},.d_t_fmt=tsc(L"%a %d %b %Y %T %z"),.d_fmt=tsc(L"%d.%m.%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%A %-e %B %Y, %H:%M:%S %z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1DdYy]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"da"),.nostr=tsc(L"nu")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"40")},.name={.name_fmt=tsc(L"%d%t%s%t%f%t%g%t%m"),.name_miss=tsc(L"D-ra."),.name_mr=tsc(L"Dl."),.name_mrs=tsc(L"D-na.")},.address={.postal_fmt=tsc(L"%f%N%a%d%N%s%t%h%N%b%t%e%t%r%N%z%t%T%N%S%t%c%N"),.country_name=tsc(L"România"),.country_post=tsc(L"RO"),.country_ab2=tsc(L"RO"),.country_ab3=tsc(L"ROU"),.country_num=642,.country_car=tsc(L"RO"),.country_isbn=tsc(L"973"),.lang_name=tsc(L"română"),.lang_ab=tsc(L"ro"),.lang_term=tsc(L"ron"),.lang_lib=tsc(L"rum")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Romanian locale for Romania"),.source=tsc(u8"RAP\t\t;\t\tfast_io"),.address=tsc(u8"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Romanian"),.territory=tsc(u8"Romania"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-06-29")},.monetary={.int_curr_symbol=tsc(u8"RON "),.currency_symbol=tsc(u8"Lei"),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Du"),tsc(u8"Lu"),tsc(u8"Ma"),tsc(u8"Mi"),tsc(u8"Jo"),tsc(u8"Vi"),tsc(u8"Sb")},.day={tsc(u8"duminică"),tsc(u8"luni"),tsc(u8"marți"),tsc(u8"miercuri"),tsc(u8"joi"),tsc(u8"vineri"),tsc(u8"sâmbătă")},.abmon={tsc(u8"ian"),tsc(u8"feb"),tsc(u8"mar"),tsc(u8"apr"),tsc(u8"mai"),tsc(u8"iun"),tsc(u8"iul"),tsc(u8"aug"),tsc(u8"sep"),tsc(u8"oct"),tsc(u8"nov"),tsc(u8"dec")},.mon={tsc(u8"ianuarie"),tsc(u8"februarie"),tsc(u8"martie"),tsc(u8"aprilie"),tsc(u8"mai"),tsc(u8"iunie"),tsc(u8"iulie"),tsc(u8"august"),tsc(u8"septembrie"),tsc(u8"octombrie"),tsc(u8"noiembrie"),tsc(u8"decembrie")},.d_t_fmt=tsc(u8"%a %d %b %Y %T %z"),.d_fmt=tsc(u8"%d.%m.%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%A %-e %B %Y, %H:%M:%S %z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1DdYy]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"da"),.nostr=tsc(u8"nu")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"40")},.name={.name_fmt=tsc(u8"%d%t%s%t%f%t%g%t%m"),.name_miss=tsc(u8"D-ra."),.name_mr=tsc(u8"Dl."),.name_mrs=tsc(u8"D-na.")},.address={.postal_fmt=tsc(u8"%f%N%a%d%N%s%t%h%N%b%t%e%t%r%N%z%t%T%N%S%t%c%N"),.country_name=tsc(u8"România"),.country_post=tsc(u8"RO"),.country_ab2=tsc(u8"RO"),.country_ab3=tsc(u8"ROU"),.country_num=642,.country_car=tsc(u8"RO"),.country_isbn=tsc(u8"973"),.lang_name=tsc(u8"română"),.lang_ab=tsc(u8"ro"),.lang_term=tsc(u8"ron"),.lang_lib=tsc(u8"rum")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Romanian locale for Romania"),.source=tsc(u"RAP\t\t;\t\tfast_io"),.address=tsc(u"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Romanian"),.territory=tsc(u"Romania"),.revision=tsc(u"1.0"),.date=tsc(u"2000-06-29")},.monetary={.int_curr_symbol=tsc(u"RON "),.currency_symbol=tsc(u"Lei"),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Du"),tsc(u"Lu"),tsc(u"Ma"),tsc(u"Mi"),tsc(u"Jo"),tsc(u"Vi"),tsc(u"Sb")},.day={tsc(u"duminică"),tsc(u"luni"),tsc(u"marți"),tsc(u"miercuri"),tsc(u"joi"),tsc(u"vineri"),tsc(u"sâmbătă")},.abmon={tsc(u"ian"),tsc(u"feb"),tsc(u"mar"),tsc(u"apr"),tsc(u"mai"),tsc(u"iun"),tsc(u"iul"),tsc(u"aug"),tsc(u"sep"),tsc(u"oct"),tsc(u"nov"),tsc(u"dec")},.mon={tsc(u"ianuarie"),tsc(u"februarie"),tsc(u"martie"),tsc(u"aprilie"),tsc(u"mai"),tsc(u"iunie"),tsc(u"iulie"),tsc(u"august"),tsc(u"septembrie"),tsc(u"octombrie"),tsc(u"noiembrie"),tsc(u"decembrie")},.d_t_fmt=tsc(u"%a %d %b %Y %T %z"),.d_fmt=tsc(u"%d.%m.%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%A %-e %B %Y, %H:%M:%S %z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1DdYy]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"da"),.nostr=tsc(u"nu")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"40")},.name={.name_fmt=tsc(u"%d%t%s%t%f%t%g%t%m"),.name_miss=tsc(u"D-ra."),.name_mr=tsc(u"Dl."),.name_mrs=tsc(u"D-na.")},.address={.postal_fmt=tsc(u"%f%N%a%d%N%s%t%h%N%b%t%e%t%r%N%z%t%T%N%S%t%c%N"),.country_name=tsc(u"România"),.country_post=tsc(u"RO"),.country_ab2=tsc(u"RO"),.country_ab3=tsc(u"ROU"),.country_num=642,.country_car=tsc(u"RO"),.country_isbn=tsc(u"973"),.lang_name=tsc(u"română"),.lang_ab=tsc(u"ro"),.lang_term=tsc(u"ron"),.lang_lib=tsc(u"rum")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Romanian locale for Romania"),.source=tsc(U"RAP\t\t;\t\tfast_io"),.address=tsc(U"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Romanian"),.territory=tsc(U"Romania"),.revision=tsc(U"1.0"),.date=tsc(U"2000-06-29")},.monetary={.int_curr_symbol=tsc(U"RON "),.currency_symbol=tsc(U"Lei"),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Du"),tsc(U"Lu"),tsc(U"Ma"),tsc(U"Mi"),tsc(U"Jo"),tsc(U"Vi"),tsc(U"Sb")},.day={tsc(U"duminică"),tsc(U"luni"),tsc(U"marți"),tsc(U"miercuri"),tsc(U"joi"),tsc(U"vineri"),tsc(U"sâmbătă")},.abmon={tsc(U"ian"),tsc(U"feb"),tsc(U"mar"),tsc(U"apr"),tsc(U"mai"),tsc(U"iun"),tsc(U"iul"),tsc(U"aug"),tsc(U"sep"),tsc(U"oct"),tsc(U"nov"),tsc(U"dec")},.mon={tsc(U"ianuarie"),tsc(U"februarie"),tsc(U"martie"),tsc(U"aprilie"),tsc(U"mai"),tsc(U"iunie"),tsc(U"iulie"),tsc(U"august"),tsc(U"septembrie"),tsc(U"octombrie"),tsc(U"noiembrie"),tsc(U"decembrie")},.d_t_fmt=tsc(U"%a %d %b %Y %T %z"),.d_fmt=tsc(U"%d.%m.%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%A %-e %B %Y, %H:%M:%S %z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1DdYy]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"da"),.nostr=tsc(U"nu")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"40")},.name={.name_fmt=tsc(U"%d%t%s%t%f%t%g%t%m"),.name_miss=tsc(U"D-ra."),.name_mr=tsc(U"Dl."),.name_mrs=tsc(U"D-na.")},.address={.postal_fmt=tsc(U"%f%N%a%d%N%s%t%h%N%b%t%e%t%r%N%z%t%T%N%S%t%c%N"),.country_name=tsc(U"România"),.country_post=tsc(U"RO"),.country_ab2=tsc(U"RO"),.country_ab3=tsc(U"ROU"),.country_num=642,.country_car=tsc(U"RO"),.country_isbn=tsc(U"973"),.lang_name=tsc(U"română"),.lang_ab=tsc(U"ro"),.lang_term=tsc(U"ron"),.lang_lib=tsc(U"rum")},.measurement={.measurement=1}};


}
}

#include"../main.h"