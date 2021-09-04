#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Valencian (southern Catalan) locale for Spain with Euro"),.source=tsc("fast_io"),.address=tsc("https://github.com/expnkx/fast_io"),.contact=tsc("Jordi Mallach\t\t;\t\tfast_io"),.email=tsc("jordi@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Catalan"),.territory=tsc("Spain"),.revision=tsc("1.0"),.date=tsc("2006-04-06")},.monetary={.int_curr_symbol=tsc("EUR "),.currency_symbol=tsc("€"),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc("."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc("."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("dg."),tsc("dl."),tsc("dt."),tsc("dc."),tsc("dj."),tsc("dv."),tsc("ds.")},.day={tsc("diumenge"),tsc("dilluns"),tsc("dimarts"),tsc("dimecres"),tsc("dijous"),tsc("divendres"),tsc("dissabte")},.abmon={tsc("de gen."),tsc("de febr."),tsc("de març"),tsc("d’abr."),tsc("de maig"),tsc("de juny"),tsc("de jul."),tsc("d’ag."),tsc("de set."),tsc("d’oct."),tsc("de nov."),tsc("de des.")},.ab_alt_mon={tsc("gen."),tsc("febr."),tsc("març"),tsc("abr."),tsc("maig"),tsc("juny"),tsc("jul."),tsc("ag."),tsc("set."),tsc("oct."),tsc("nov."),tsc("des.")},.mon={tsc("de gener"),tsc("de febrer"),tsc("de març"),tsc("d’abril"),tsc("de maig"),tsc("de juny"),tsc("de juliol"),tsc("d’agost"),tsc("de setembre"),tsc("d’octubre"),tsc("de novembre"),tsc("de desembre")},.d_t_fmt=tsc("%A, %-d %B de %Y, %T"),.d_fmt=tsc("%-d//%-m//%y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%A, %-d %B de %Y, %T %Z"),.am_pm={tsc("a. m."),tsc("p. m.")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc("^[+1sSyY]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("sí"),.nostr=tsc("no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00"),.int_prefix=tsc("34")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Espanya"),.country_ab2=tsc("ES"),.country_ab3=tsc("ESP"),.country_num=724,.country_car=tsc("E"),.lang_name=tsc("català"),.lang_ab=tsc("ca"),.lang_term=tsc("cat"),.lang_lib=tsc("cat")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Valencian (southern Catalan) locale for Spain with Euro"),.source=tsc(L"fast_io"),.address=tsc(L"https://github.com/expnkx/fast_io"),.contact=tsc(L"Jordi Mallach\t\t;\t\tfast_io"),.email=tsc(L"jordi@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Catalan"),.territory=tsc(L"Spain"),.revision=tsc(L"1.0"),.date=tsc(L"2006-04-06")},.monetary={.int_curr_symbol=tsc(L"EUR "),.currency_symbol=tsc(L"€"),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"dg."),tsc(L"dl."),tsc(L"dt."),tsc(L"dc."),tsc(L"dj."),tsc(L"dv."),tsc(L"ds.")},.day={tsc(L"diumenge"),tsc(L"dilluns"),tsc(L"dimarts"),tsc(L"dimecres"),tsc(L"dijous"),tsc(L"divendres"),tsc(L"dissabte")},.abmon={tsc(L"de gen."),tsc(L"de febr."),tsc(L"de març"),tsc(L"d’abr."),tsc(L"de maig"),tsc(L"de juny"),tsc(L"de jul."),tsc(L"d’ag."),tsc(L"de set."),tsc(L"d’oct."),tsc(L"de nov."),tsc(L"de des.")},.ab_alt_mon={tsc(L"gen."),tsc(L"febr."),tsc(L"març"),tsc(L"abr."),tsc(L"maig"),tsc(L"juny"),tsc(L"jul."),tsc(L"ag."),tsc(L"set."),tsc(L"oct."),tsc(L"nov."),tsc(L"des.")},.mon={tsc(L"de gener"),tsc(L"de febrer"),tsc(L"de març"),tsc(L"d’abril"),tsc(L"de maig"),tsc(L"de juny"),tsc(L"de juliol"),tsc(L"d’agost"),tsc(L"de setembre"),tsc(L"d’octubre"),tsc(L"de novembre"),tsc(L"de desembre")},.d_t_fmt=tsc(L"%A, %-d %B de %Y, %T"),.d_fmt=tsc(L"%-d//%-m//%y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%A, %-d %B de %Y, %T %Z"),.am_pm={tsc(L"a. m."),tsc(L"p. m.")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1sSyY]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"sí"),.nostr=tsc(L"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"34")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Espanya"),.country_ab2=tsc(L"ES"),.country_ab3=tsc(L"ESP"),.country_num=724,.country_car=tsc(L"E"),.lang_name=tsc(L"català"),.lang_ab=tsc(L"ca"),.lang_term=tsc(L"cat"),.lang_lib=tsc(L"cat")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Valencian (southern Catalan) locale for Spain with Euro"),.source=tsc(u8"fast_io"),.address=tsc(u8"https://github.com/expnkx/fast_io"),.contact=tsc(u8"Jordi Mallach\t\t;\t\tfast_io"),.email=tsc(u8"jordi@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Catalan"),.territory=tsc(u8"Spain"),.revision=tsc(u8"1.0"),.date=tsc(u8"2006-04-06")},.monetary={.int_curr_symbol=tsc(u8"EUR "),.currency_symbol=tsc(u8"€"),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"dg."),tsc(u8"dl."),tsc(u8"dt."),tsc(u8"dc."),tsc(u8"dj."),tsc(u8"dv."),tsc(u8"ds.")},.day={tsc(u8"diumenge"),tsc(u8"dilluns"),tsc(u8"dimarts"),tsc(u8"dimecres"),tsc(u8"dijous"),tsc(u8"divendres"),tsc(u8"dissabte")},.abmon={tsc(u8"de gen."),tsc(u8"de febr."),tsc(u8"de març"),tsc(u8"d’abr."),tsc(u8"de maig"),tsc(u8"de juny"),tsc(u8"de jul."),tsc(u8"d’ag."),tsc(u8"de set."),tsc(u8"d’oct."),tsc(u8"de nov."),tsc(u8"de des.")},.ab_alt_mon={tsc(u8"gen."),tsc(u8"febr."),tsc(u8"març"),tsc(u8"abr."),tsc(u8"maig"),tsc(u8"juny"),tsc(u8"jul."),tsc(u8"ag."),tsc(u8"set."),tsc(u8"oct."),tsc(u8"nov."),tsc(u8"des.")},.mon={tsc(u8"de gener"),tsc(u8"de febrer"),tsc(u8"de març"),tsc(u8"d’abril"),tsc(u8"de maig"),tsc(u8"de juny"),tsc(u8"de juliol"),tsc(u8"d’agost"),tsc(u8"de setembre"),tsc(u8"d’octubre"),tsc(u8"de novembre"),tsc(u8"de desembre")},.d_t_fmt=tsc(u8"%A, %-d %B de %Y, %T"),.d_fmt=tsc(u8"%-d//%-m//%y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%A, %-d %B de %Y, %T %Z"),.am_pm={tsc(u8"a. m."),tsc(u8"p. m.")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1sSyY]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"sí"),.nostr=tsc(u8"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"34")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Espanya"),.country_ab2=tsc(u8"ES"),.country_ab3=tsc(u8"ESP"),.country_num=724,.country_car=tsc(u8"E"),.lang_name=tsc(u8"català"),.lang_ab=tsc(u8"ca"),.lang_term=tsc(u8"cat"),.lang_lib=tsc(u8"cat")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Valencian (southern Catalan) locale for Spain with Euro"),.source=tsc(u"fast_io"),.address=tsc(u"https://github.com/expnkx/fast_io"),.contact=tsc(u"Jordi Mallach\t\t;\t\tfast_io"),.email=tsc(u"jordi@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Catalan"),.territory=tsc(u"Spain"),.revision=tsc(u"1.0"),.date=tsc(u"2006-04-06")},.monetary={.int_curr_symbol=tsc(u"EUR "),.currency_symbol=tsc(u"€"),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"dg."),tsc(u"dl."),tsc(u"dt."),tsc(u"dc."),tsc(u"dj."),tsc(u"dv."),tsc(u"ds.")},.day={tsc(u"diumenge"),tsc(u"dilluns"),tsc(u"dimarts"),tsc(u"dimecres"),tsc(u"dijous"),tsc(u"divendres"),tsc(u"dissabte")},.abmon={tsc(u"de gen."),tsc(u"de febr."),tsc(u"de març"),tsc(u"d’abr."),tsc(u"de maig"),tsc(u"de juny"),tsc(u"de jul."),tsc(u"d’ag."),tsc(u"de set."),tsc(u"d’oct."),tsc(u"de nov."),tsc(u"de des.")},.ab_alt_mon={tsc(u"gen."),tsc(u"febr."),tsc(u"març"),tsc(u"abr."),tsc(u"maig"),tsc(u"juny"),tsc(u"jul."),tsc(u"ag."),tsc(u"set."),tsc(u"oct."),tsc(u"nov."),tsc(u"des.")},.mon={tsc(u"de gener"),tsc(u"de febrer"),tsc(u"de març"),tsc(u"d’abril"),tsc(u"de maig"),tsc(u"de juny"),tsc(u"de juliol"),tsc(u"d’agost"),tsc(u"de setembre"),tsc(u"d’octubre"),tsc(u"de novembre"),tsc(u"de desembre")},.d_t_fmt=tsc(u"%A, %-d %B de %Y, %T"),.d_fmt=tsc(u"%-d//%-m//%y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%A, %-d %B de %Y, %T %Z"),.am_pm={tsc(u"a. m."),tsc(u"p. m.")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1sSyY]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"sí"),.nostr=tsc(u"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"34")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Espanya"),.country_ab2=tsc(u"ES"),.country_ab3=tsc(u"ESP"),.country_num=724,.country_car=tsc(u"E"),.lang_name=tsc(u"català"),.lang_ab=tsc(u"ca"),.lang_term=tsc(u"cat"),.lang_lib=tsc(u"cat")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Valencian (southern Catalan) locale for Spain with Euro"),.source=tsc(U"fast_io"),.address=tsc(U"https://github.com/expnkx/fast_io"),.contact=tsc(U"Jordi Mallach\t\t;\t\tfast_io"),.email=tsc(U"jordi@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Catalan"),.territory=tsc(U"Spain"),.revision=tsc(U"1.0"),.date=tsc(U"2006-04-06")},.monetary={.int_curr_symbol=tsc(U"EUR "),.currency_symbol=tsc(U"€"),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"dg."),tsc(U"dl."),tsc(U"dt."),tsc(U"dc."),tsc(U"dj."),tsc(U"dv."),tsc(U"ds.")},.day={tsc(U"diumenge"),tsc(U"dilluns"),tsc(U"dimarts"),tsc(U"dimecres"),tsc(U"dijous"),tsc(U"divendres"),tsc(U"dissabte")},.abmon={tsc(U"de gen."),tsc(U"de febr."),tsc(U"de març"),tsc(U"d’abr."),tsc(U"de maig"),tsc(U"de juny"),tsc(U"de jul."),tsc(U"d’ag."),tsc(U"de set."),tsc(U"d’oct."),tsc(U"de nov."),tsc(U"de des.")},.ab_alt_mon={tsc(U"gen."),tsc(U"febr."),tsc(U"març"),tsc(U"abr."),tsc(U"maig"),tsc(U"juny"),tsc(U"jul."),tsc(U"ag."),tsc(U"set."),tsc(U"oct."),tsc(U"nov."),tsc(U"des.")},.mon={tsc(U"de gener"),tsc(U"de febrer"),tsc(U"de març"),tsc(U"d’abril"),tsc(U"de maig"),tsc(U"de juny"),tsc(U"de juliol"),tsc(U"d’agost"),tsc(U"de setembre"),tsc(U"d’octubre"),tsc(U"de novembre"),tsc(U"de desembre")},.d_t_fmt=tsc(U"%A, %-d %B de %Y, %T"),.d_fmt=tsc(U"%-d//%-m//%y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%A, %-d %B de %Y, %T %Z"),.am_pm={tsc(U"a. m."),tsc(U"p. m.")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1sSyY]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"sí"),.nostr=tsc(U"no")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"34")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Espanya"),.country_ab2=tsc(U"ES"),.country_ab3=tsc(U"ESP"),.country_num=724,.country_car=tsc(U"E"),.lang_name=tsc(U"català"),.lang_ab=tsc(U"ca"),.lang_term=tsc(U"cat"),.lang_lib=tsc(U"cat")},.measurement={.measurement=1}};


}
}

#include"../main.h"