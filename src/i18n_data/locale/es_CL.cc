#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Spanish locale for Chile"),.source=tsc("RAP\t\t;\t\tfast_io"),.address=tsc("Sankt Jørgens Alle 8, DK-1615 København V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Spanish"),.territory=tsc("Chile"),.revision=tsc("1.0"),.date=tsc("2000-06-29")},.monetary={.int_curr_symbol=tsc("CLP "),.currency_symbol=tsc("$"),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc("."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc("."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("dom"),tsc("lun"),tsc("mar"),tsc("mié"),tsc("jue"),tsc("vie"),tsc("sáb")},.day={tsc("domingo"),tsc("lunes"),tsc("martes"),tsc("miércoles"),tsc("jueves"),tsc("viernes"),tsc("sábado")},.abmon={tsc("ene"),tsc("feb"),tsc("mar"),tsc("abr"),tsc("may"),tsc("jun"),tsc("jul"),tsc("ago"),tsc("sep"),tsc("oct"),tsc("nov"),tsc("dic")},.mon={tsc("enero"),tsc("febrero"),tsc("marzo"),tsc("abril"),tsc("mayo"),tsc("junio"),tsc("julio"),tsc("agosto"),tsc("septiembre"),tsc("octubre"),tsc("noviembre"),tsc("diciembre")},.d_t_fmt=tsc("%a %d %b %Y %T"),.d_fmt=tsc("%d//%m//%y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%a %d %b %Y %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1sSyY]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("sí"),.nostr=tsc("no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00"),.int_prefix=tsc("56")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Chile"),.country_ab2=tsc("CL"),.country_ab3=tsc("CHL"),.country_num=152,.country_car=tsc("RCH"),.lang_name=tsc("español"),.lang_ab=tsc("es"),.lang_term=tsc("spa"),.lang_lib=tsc("spa")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Spanish locale for Chile"),.source=tsc(L"RAP\t\t;\t\tfast_io"),.address=tsc(L"Sankt Jørgens Alle 8, DK-1615 København V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Spanish"),.territory=tsc(L"Chile"),.revision=tsc(L"1.0"),.date=tsc(L"2000-06-29")},.monetary={.int_curr_symbol=tsc(L"CLP "),.currency_symbol=tsc(L"$"),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"dom"),tsc(L"lun"),tsc(L"mar"),tsc(L"mié"),tsc(L"jue"),tsc(L"vie"),tsc(L"sáb")},.day={tsc(L"domingo"),tsc(L"lunes"),tsc(L"martes"),tsc(L"miércoles"),tsc(L"jueves"),tsc(L"viernes"),tsc(L"sábado")},.abmon={tsc(L"ene"),tsc(L"feb"),tsc(L"mar"),tsc(L"abr"),tsc(L"may"),tsc(L"jun"),tsc(L"jul"),tsc(L"ago"),tsc(L"sep"),tsc(L"oct"),tsc(L"nov"),tsc(L"dic")},.mon={tsc(L"enero"),tsc(L"febrero"),tsc(L"marzo"),tsc(L"abril"),tsc(L"mayo"),tsc(L"junio"),tsc(L"julio"),tsc(L"agosto"),tsc(L"septiembre"),tsc(L"octubre"),tsc(L"noviembre"),tsc(L"diciembre")},.d_t_fmt=tsc(L"%a %d %b %Y %T"),.d_fmt=tsc(L"%d//%m//%y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%a %d %b %Y %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1sSyY]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"sí"),.nostr=tsc(L"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"56")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Chile"),.country_ab2=tsc(L"CL"),.country_ab3=tsc(L"CHL"),.country_num=152,.country_car=tsc(L"RCH"),.lang_name=tsc(L"español"),.lang_ab=tsc(L"es"),.lang_term=tsc(L"spa"),.lang_lib=tsc(L"spa")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Spanish locale for Chile"),.source=tsc(u8"RAP\t\t;\t\tfast_io"),.address=tsc(u8"Sankt Jørgens Alle 8, DK-1615 København V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Spanish"),.territory=tsc(u8"Chile"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-06-29")},.monetary={.int_curr_symbol=tsc(u8"CLP "),.currency_symbol=tsc(u8"$"),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"dom"),tsc(u8"lun"),tsc(u8"mar"),tsc(u8"mié"),tsc(u8"jue"),tsc(u8"vie"),tsc(u8"sáb")},.day={tsc(u8"domingo"),tsc(u8"lunes"),tsc(u8"martes"),tsc(u8"miércoles"),tsc(u8"jueves"),tsc(u8"viernes"),tsc(u8"sábado")},.abmon={tsc(u8"ene"),tsc(u8"feb"),tsc(u8"mar"),tsc(u8"abr"),tsc(u8"may"),tsc(u8"jun"),tsc(u8"jul"),tsc(u8"ago"),tsc(u8"sep"),tsc(u8"oct"),tsc(u8"nov"),tsc(u8"dic")},.mon={tsc(u8"enero"),tsc(u8"febrero"),tsc(u8"marzo"),tsc(u8"abril"),tsc(u8"mayo"),tsc(u8"junio"),tsc(u8"julio"),tsc(u8"agosto"),tsc(u8"septiembre"),tsc(u8"octubre"),tsc(u8"noviembre"),tsc(u8"diciembre")},.d_t_fmt=tsc(u8"%a %d %b %Y %T"),.d_fmt=tsc(u8"%d//%m//%y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%a %d %b %Y %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1sSyY]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"sí"),.nostr=tsc(u8"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"56")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Chile"),.country_ab2=tsc(u8"CL"),.country_ab3=tsc(u8"CHL"),.country_num=152,.country_car=tsc(u8"RCH"),.lang_name=tsc(u8"español"),.lang_ab=tsc(u8"es"),.lang_term=tsc(u8"spa"),.lang_lib=tsc(u8"spa")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Spanish locale for Chile"),.source=tsc(u"RAP\t\t;\t\tfast_io"),.address=tsc(u"Sankt Jørgens Alle 8, DK-1615 København V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Spanish"),.territory=tsc(u"Chile"),.revision=tsc(u"1.0"),.date=tsc(u"2000-06-29")},.monetary={.int_curr_symbol=tsc(u"CLP "),.currency_symbol=tsc(u"$"),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"dom"),tsc(u"lun"),tsc(u"mar"),tsc(u"mié"),tsc(u"jue"),tsc(u"vie"),tsc(u"sáb")},.day={tsc(u"domingo"),tsc(u"lunes"),tsc(u"martes"),tsc(u"miércoles"),tsc(u"jueves"),tsc(u"viernes"),tsc(u"sábado")},.abmon={tsc(u"ene"),tsc(u"feb"),tsc(u"mar"),tsc(u"abr"),tsc(u"may"),tsc(u"jun"),tsc(u"jul"),tsc(u"ago"),tsc(u"sep"),tsc(u"oct"),tsc(u"nov"),tsc(u"dic")},.mon={tsc(u"enero"),tsc(u"febrero"),tsc(u"marzo"),tsc(u"abril"),tsc(u"mayo"),tsc(u"junio"),tsc(u"julio"),tsc(u"agosto"),tsc(u"septiembre"),tsc(u"octubre"),tsc(u"noviembre"),tsc(u"diciembre")},.d_t_fmt=tsc(u"%a %d %b %Y %T"),.d_fmt=tsc(u"%d//%m//%y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%a %d %b %Y %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1sSyY]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"sí"),.nostr=tsc(u"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"56")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Chile"),.country_ab2=tsc(u"CL"),.country_ab3=tsc(u"CHL"),.country_num=152,.country_car=tsc(u"RCH"),.lang_name=tsc(u"español"),.lang_ab=tsc(u"es"),.lang_term=tsc(u"spa"),.lang_lib=tsc(u"spa")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Spanish locale for Chile"),.source=tsc(U"RAP\t\t;\t\tfast_io"),.address=tsc(U"Sankt Jørgens Alle 8, DK-1615 København V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Spanish"),.territory=tsc(U"Chile"),.revision=tsc(U"1.0"),.date=tsc(U"2000-06-29")},.monetary={.int_curr_symbol=tsc(U"CLP "),.currency_symbol=tsc(U"$"),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"dom"),tsc(U"lun"),tsc(U"mar"),tsc(U"mié"),tsc(U"jue"),tsc(U"vie"),tsc(U"sáb")},.day={tsc(U"domingo"),tsc(U"lunes"),tsc(U"martes"),tsc(U"miércoles"),tsc(U"jueves"),tsc(U"viernes"),tsc(U"sábado")},.abmon={tsc(U"ene"),tsc(U"feb"),tsc(U"mar"),tsc(U"abr"),tsc(U"may"),tsc(U"jun"),tsc(U"jul"),tsc(U"ago"),tsc(U"sep"),tsc(U"oct"),tsc(U"nov"),tsc(U"dic")},.mon={tsc(U"enero"),tsc(U"febrero"),tsc(U"marzo"),tsc(U"abril"),tsc(U"mayo"),tsc(U"junio"),tsc(U"julio"),tsc(U"agosto"),tsc(U"septiembre"),tsc(U"octubre"),tsc(U"noviembre"),tsc(U"diciembre")},.d_t_fmt=tsc(U"%a %d %b %Y %T"),.d_fmt=tsc(U"%d//%m//%y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%a %d %b %Y %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1sSyY]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"sí"),.nostr=tsc(U"no")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"56")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Chile"),.country_ab2=tsc(U"CL"),.country_ab3=tsc(U"CHL"),.country_num=152,.country_car=tsc(U"RCH"),.lang_name=tsc(U"español"),.lang_ab=tsc(U"es"),.lang_term=tsc(U"spa"),.lang_lib=tsc(U"spa")},.measurement={.measurement=1}};


}
}

#include"../main.h"