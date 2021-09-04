#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Filipino language locale for Philippines"),.source=tsc("Rene Torres\t\t;\t\tfast_io"),.address=tsc("https://github.com/expnkx/fast_io"),.contact=tsc("Rene Torres, Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc("rgtorre@rocketmail.com, pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Filipino"),.territory=tsc("Philippines"),.revision=tsc("0.5"),.date=tsc("2005-02-02")},.monetary={.int_curr_symbol=tsc("PHP "),.currency_symbol=tsc("₱"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Lin"),tsc("Lun"),tsc("Mar"),tsc("Miy"),tsc("Huw"),tsc("Biy"),tsc("Sab")},.day={tsc("Linggo"),tsc("Lunes"),tsc("Martes"),tsc("Miyerkoles"),tsc("Huwebes"),tsc("Biyernes"),tsc("Sabado")},.abmon={tsc("Ene"),tsc("Peb"),tsc("Mar"),tsc("Abr"),tsc("May"),tsc("Hun"),tsc("Hul"),tsc("Ago"),tsc("Set"),tsc("Okt"),tsc("Nob"),tsc("Dis")},.mon={tsc("Enero"),tsc("Pebrero"),tsc("Marso"),tsc("Abril"),tsc("Mayo"),tsc("Hunyo"),tsc("Hulyo"),tsc("Agosto"),tsc("Setyembre"),tsc("Oktubre"),tsc("Nobyembre"),tsc("Disyembre")},.d_t_fmt=tsc("%a %d %b %Y %r"),.d_fmt=tsc("%m//%d//%y"),.t_fmt=tsc("%r"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %d %b %Y %r %Z"),.am_pm={tsc("N.U."),tsc("N.H.")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1oOyY]"),.noexpr=tsc("^[-0hHnN]"),.yesstr=tsc("oo"),.nostr=tsc("hindi")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("63")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f"),.name_gen=tsc(""),.name_miss=tsc("Bb."),.name_mr=tsc("G."),.name_mrs=tsc("Gng."),.name_ms=tsc("Bb.")},.address={.postal_fmt=tsc("%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc("Pilipinas"),.country_ab2=tsc("PH"),.country_ab3=tsc("PHL"),.country_num=608,.country_car=tsc("RP"),.lang_name=tsc("Filipino"),.lang_term=tsc("fil")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Filipino language locale for Philippines"),.source=tsc(L"Rene Torres\t\t;\t\tfast_io"),.address=tsc(L"https://github.com/expnkx/fast_io"),.contact=tsc(L"Rene Torres, Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(L"rgtorre@rocketmail.com, pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Filipino"),.territory=tsc(L"Philippines"),.revision=tsc(L"0.5"),.date=tsc(L"2005-02-02")},.monetary={.int_curr_symbol=tsc(L"PHP "),.currency_symbol=tsc(L"₱"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Lin"),tsc(L"Lun"),tsc(L"Mar"),tsc(L"Miy"),tsc(L"Huw"),tsc(L"Biy"),tsc(L"Sab")},.day={tsc(L"Linggo"),tsc(L"Lunes"),tsc(L"Martes"),tsc(L"Miyerkoles"),tsc(L"Huwebes"),tsc(L"Biyernes"),tsc(L"Sabado")},.abmon={tsc(L"Ene"),tsc(L"Peb"),tsc(L"Mar"),tsc(L"Abr"),tsc(L"May"),tsc(L"Hun"),tsc(L"Hul"),tsc(L"Ago"),tsc(L"Set"),tsc(L"Okt"),tsc(L"Nob"),tsc(L"Dis")},.mon={tsc(L"Enero"),tsc(L"Pebrero"),tsc(L"Marso"),tsc(L"Abril"),tsc(L"Mayo"),tsc(L"Hunyo"),tsc(L"Hulyo"),tsc(L"Agosto"),tsc(L"Setyembre"),tsc(L"Oktubre"),tsc(L"Nobyembre"),tsc(L"Disyembre")},.d_t_fmt=tsc(L"%a %d %b %Y %r"),.d_fmt=tsc(L"%m//%d//%y"),.t_fmt=tsc(L"%r"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %d %b %Y %r %Z"),.am_pm={tsc(L"N.U."),tsc(L"N.H.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1oOyY]"),.noexpr=tsc(L"^[-0hHnN]"),.yesstr=tsc(L"oo"),.nostr=tsc(L"hindi")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"63")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f"),.name_gen=tsc(L""),.name_miss=tsc(L"Bb."),.name_mr=tsc(L"G."),.name_mrs=tsc(L"Gng."),.name_ms=tsc(L"Bb.")},.address={.postal_fmt=tsc(L"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(L"Pilipinas"),.country_ab2=tsc(L"PH"),.country_ab3=tsc(L"PHL"),.country_num=608,.country_car=tsc(L"RP"),.lang_name=tsc(L"Filipino"),.lang_term=tsc(L"fil")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Filipino language locale for Philippines"),.source=tsc(u8"Rene Torres\t\t;\t\tfast_io"),.address=tsc(u8"https://github.com/expnkx/fast_io"),.contact=tsc(u8"Rene Torres, Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(u8"rgtorre@rocketmail.com, pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Filipino"),.territory=tsc(u8"Philippines"),.revision=tsc(u8"0.5"),.date=tsc(u8"2005-02-02")},.monetary={.int_curr_symbol=tsc(u8"PHP "),.currency_symbol=tsc(u8"₱"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Lin"),tsc(u8"Lun"),tsc(u8"Mar"),tsc(u8"Miy"),tsc(u8"Huw"),tsc(u8"Biy"),tsc(u8"Sab")},.day={tsc(u8"Linggo"),tsc(u8"Lunes"),tsc(u8"Martes"),tsc(u8"Miyerkoles"),tsc(u8"Huwebes"),tsc(u8"Biyernes"),tsc(u8"Sabado")},.abmon={tsc(u8"Ene"),tsc(u8"Peb"),tsc(u8"Mar"),tsc(u8"Abr"),tsc(u8"May"),tsc(u8"Hun"),tsc(u8"Hul"),tsc(u8"Ago"),tsc(u8"Set"),tsc(u8"Okt"),tsc(u8"Nob"),tsc(u8"Dis")},.mon={tsc(u8"Enero"),tsc(u8"Pebrero"),tsc(u8"Marso"),tsc(u8"Abril"),tsc(u8"Mayo"),tsc(u8"Hunyo"),tsc(u8"Hulyo"),tsc(u8"Agosto"),tsc(u8"Setyembre"),tsc(u8"Oktubre"),tsc(u8"Nobyembre"),tsc(u8"Disyembre")},.d_t_fmt=tsc(u8"%a %d %b %Y %r"),.d_fmt=tsc(u8"%m//%d//%y"),.t_fmt=tsc(u8"%r"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %d %b %Y %r %Z"),.am_pm={tsc(u8"N.U."),tsc(u8"N.H.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1oOyY]"),.noexpr=tsc(u8"^[-0hHnN]"),.yesstr=tsc(u8"oo"),.nostr=tsc(u8"hindi")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"63")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f"),.name_gen=tsc(u8""),.name_miss=tsc(u8"Bb."),.name_mr=tsc(u8"G."),.name_mrs=tsc(u8"Gng."),.name_ms=tsc(u8"Bb.")},.address={.postal_fmt=tsc(u8"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(u8"Pilipinas"),.country_ab2=tsc(u8"PH"),.country_ab3=tsc(u8"PHL"),.country_num=608,.country_car=tsc(u8"RP"),.lang_name=tsc(u8"Filipino"),.lang_term=tsc(u8"fil")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Filipino language locale for Philippines"),.source=tsc(u"Rene Torres\t\t;\t\tfast_io"),.address=tsc(u"https://github.com/expnkx/fast_io"),.contact=tsc(u"Rene Torres, Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(u"rgtorre@rocketmail.com, pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Filipino"),.territory=tsc(u"Philippines"),.revision=tsc(u"0.5"),.date=tsc(u"2005-02-02")},.monetary={.int_curr_symbol=tsc(u"PHP "),.currency_symbol=tsc(u"₱"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Lin"),tsc(u"Lun"),tsc(u"Mar"),tsc(u"Miy"),tsc(u"Huw"),tsc(u"Biy"),tsc(u"Sab")},.day={tsc(u"Linggo"),tsc(u"Lunes"),tsc(u"Martes"),tsc(u"Miyerkoles"),tsc(u"Huwebes"),tsc(u"Biyernes"),tsc(u"Sabado")},.abmon={tsc(u"Ene"),tsc(u"Peb"),tsc(u"Mar"),tsc(u"Abr"),tsc(u"May"),tsc(u"Hun"),tsc(u"Hul"),tsc(u"Ago"),tsc(u"Set"),tsc(u"Okt"),tsc(u"Nob"),tsc(u"Dis")},.mon={tsc(u"Enero"),tsc(u"Pebrero"),tsc(u"Marso"),tsc(u"Abril"),tsc(u"Mayo"),tsc(u"Hunyo"),tsc(u"Hulyo"),tsc(u"Agosto"),tsc(u"Setyembre"),tsc(u"Oktubre"),tsc(u"Nobyembre"),tsc(u"Disyembre")},.d_t_fmt=tsc(u"%a %d %b %Y %r"),.d_fmt=tsc(u"%m//%d//%y"),.t_fmt=tsc(u"%r"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %d %b %Y %r %Z"),.am_pm={tsc(u"N.U."),tsc(u"N.H.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1oOyY]"),.noexpr=tsc(u"^[-0hHnN]"),.yesstr=tsc(u"oo"),.nostr=tsc(u"hindi")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"63")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f"),.name_gen=tsc(u""),.name_miss=tsc(u"Bb."),.name_mr=tsc(u"G."),.name_mrs=tsc(u"Gng."),.name_ms=tsc(u"Bb.")},.address={.postal_fmt=tsc(u"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(u"Pilipinas"),.country_ab2=tsc(u"PH"),.country_ab3=tsc(u"PHL"),.country_num=608,.country_car=tsc(u"RP"),.lang_name=tsc(u"Filipino"),.lang_term=tsc(u"fil")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Filipino language locale for Philippines"),.source=tsc(U"Rene Torres\t\t;\t\tfast_io"),.address=tsc(U"https://github.com/expnkx/fast_io"),.contact=tsc(U"Rene Torres, Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(U"rgtorre@rocketmail.com, pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Filipino"),.territory=tsc(U"Philippines"),.revision=tsc(U"0.5"),.date=tsc(U"2005-02-02")},.monetary={.int_curr_symbol=tsc(U"PHP "),.currency_symbol=tsc(U"₱"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Lin"),tsc(U"Lun"),tsc(U"Mar"),tsc(U"Miy"),tsc(U"Huw"),tsc(U"Biy"),tsc(U"Sab")},.day={tsc(U"Linggo"),tsc(U"Lunes"),tsc(U"Martes"),tsc(U"Miyerkoles"),tsc(U"Huwebes"),tsc(U"Biyernes"),tsc(U"Sabado")},.abmon={tsc(U"Ene"),tsc(U"Peb"),tsc(U"Mar"),tsc(U"Abr"),tsc(U"May"),tsc(U"Hun"),tsc(U"Hul"),tsc(U"Ago"),tsc(U"Set"),tsc(U"Okt"),tsc(U"Nob"),tsc(U"Dis")},.mon={tsc(U"Enero"),tsc(U"Pebrero"),tsc(U"Marso"),tsc(U"Abril"),tsc(U"Mayo"),tsc(U"Hunyo"),tsc(U"Hulyo"),tsc(U"Agosto"),tsc(U"Setyembre"),tsc(U"Oktubre"),tsc(U"Nobyembre"),tsc(U"Disyembre")},.d_t_fmt=tsc(U"%a %d %b %Y %r"),.d_fmt=tsc(U"%m//%d//%y"),.t_fmt=tsc(U"%r"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %d %b %Y %r %Z"),.am_pm={tsc(U"N.U."),tsc(U"N.H.")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1oOyY]"),.noexpr=tsc(U"^[-0hHnN]"),.yesstr=tsc(U"oo"),.nostr=tsc(U"hindi")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"63")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f"),.name_gen=tsc(U""),.name_miss=tsc(U"Bb."),.name_mr=tsc(U"G."),.name_mrs=tsc(U"Gng."),.name_ms=tsc(U"Bb.")},.address={.postal_fmt=tsc(U"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(U"Pilipinas"),.country_ab2=tsc(U"PH"),.country_ab3=tsc(U"PHL"),.country_num=608,.country_car=tsc(U"RP"),.lang_name=tsc(U"Filipino"),.lang_term=tsc(U"fil")},.measurement={.measurement=1}};


}
}

#include"../main.h"