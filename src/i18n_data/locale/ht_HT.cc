#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Kreyol locale for Haiti"),.source=tsc("Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc("https://github.com/expnkx/fast_io"),.contact=tsc("sugarlabs.org\t\t;\t\tfast_io"),.email=tsc("libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Haitian Creole"),.territory=tsc("Haiti"),.revision=tsc("1.1"),.date=tsc("2013-08-09")},.monetary={.int_curr_symbol=tsc("HTG "),.currency_symbol=tsc("g"),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc(" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc(" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("dim"),tsc("len"),tsc("mad"),tsc("mèk"),tsc("jed"),tsc("van"),tsc("sam")},.day={tsc("dimanch"),tsc("lendi"),tsc("madi"),tsc("mèkredi"),tsc("jedi"),tsc("vandredi"),tsc("samdi")},.abmon={tsc("jan"),tsc("fev"),tsc("mas"),tsc("avr"),tsc("me"),tsc("jen"),tsc("jiy"),tsc("out"),tsc("sep"),tsc("okt"),tsc("nov"),tsc("des")},.mon={tsc("janvye"),tsc("fevriye"),tsc("mas"),tsc("avril"),tsc("me"),tsc("jen"),tsc("jiyè"),tsc("out"),tsc("septanm"),tsc("oktòb"),tsc("novanm"),tsc("desanm")},.d_t_fmt=tsc("%a %d %b %Y %T"),.d_fmt=tsc("%d//%m//%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%a %d %b %Y %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1wWoOyY]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("Wi"),.nostr=tsc("Non")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("%a %l"),.int_select=tsc("00"),.int_prefix=tsc("509")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %R%N%%Z %t%N%c%N"),.country_name=tsc("Ayiti"),.country_post=tsc("HT"),.country_ab2=tsc("HT"),.country_ab3=tsc("HTI"),.country_num=332,.country_car=tsc("RH"),.country_isbn=tsc("99935"),.lang_name=tsc("Kreyòl ayisyen"),.lang_ab=tsc("ht"),.lang_term=tsc("hat"),.lang_lib=tsc("hat")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Kreyol locale for Haiti"),.source=tsc(L"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(L"https://github.com/expnkx/fast_io"),.contact=tsc(L"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(L"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Haitian Creole"),.territory=tsc(L"Haiti"),.revision=tsc(L"1.1"),.date=tsc(L"2013-08-09")},.monetary={.int_curr_symbol=tsc(L"HTG "),.currency_symbol=tsc(L"g"),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"dim"),tsc(L"len"),tsc(L"mad"),tsc(L"mèk"),tsc(L"jed"),tsc(L"van"),tsc(L"sam")},.day={tsc(L"dimanch"),tsc(L"lendi"),tsc(L"madi"),tsc(L"mèkredi"),tsc(L"jedi"),tsc(L"vandredi"),tsc(L"samdi")},.abmon={tsc(L"jan"),tsc(L"fev"),tsc(L"mas"),tsc(L"avr"),tsc(L"me"),tsc(L"jen"),tsc(L"jiy"),tsc(L"out"),tsc(L"sep"),tsc(L"okt"),tsc(L"nov"),tsc(L"des")},.mon={tsc(L"janvye"),tsc(L"fevriye"),tsc(L"mas"),tsc(L"avril"),tsc(L"me"),tsc(L"jen"),tsc(L"jiyè"),tsc(L"out"),tsc(L"septanm"),tsc(L"oktòb"),tsc(L"novanm"),tsc(L"desanm")},.d_t_fmt=tsc(L"%a %d %b %Y %T"),.d_fmt=tsc(L"%d//%m//%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%a %d %b %Y %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1wWoOyY]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"Wi"),.nostr=tsc(L"Non")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"%a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"509")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %R%N%%Z %t%N%c%N"),.country_name=tsc(L"Ayiti"),.country_post=tsc(L"HT"),.country_ab2=tsc(L"HT"),.country_ab3=tsc(L"HTI"),.country_num=332,.country_car=tsc(L"RH"),.country_isbn=tsc(L"99935"),.lang_name=tsc(L"Kreyòl ayisyen"),.lang_ab=tsc(L"ht"),.lang_term=tsc(L"hat"),.lang_lib=tsc(L"hat")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Kreyol locale for Haiti"),.source=tsc(u8"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(u8"https://github.com/expnkx/fast_io"),.contact=tsc(u8"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(u8"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Haitian Creole"),.territory=tsc(u8"Haiti"),.revision=tsc(u8"1.1"),.date=tsc(u8"2013-08-09")},.monetary={.int_curr_symbol=tsc(u8"HTG "),.currency_symbol=tsc(u8"g"),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"dim"),tsc(u8"len"),tsc(u8"mad"),tsc(u8"mèk"),tsc(u8"jed"),tsc(u8"van"),tsc(u8"sam")},.day={tsc(u8"dimanch"),tsc(u8"lendi"),tsc(u8"madi"),tsc(u8"mèkredi"),tsc(u8"jedi"),tsc(u8"vandredi"),tsc(u8"samdi")},.abmon={tsc(u8"jan"),tsc(u8"fev"),tsc(u8"mas"),tsc(u8"avr"),tsc(u8"me"),tsc(u8"jen"),tsc(u8"jiy"),tsc(u8"out"),tsc(u8"sep"),tsc(u8"okt"),tsc(u8"nov"),tsc(u8"des")},.mon={tsc(u8"janvye"),tsc(u8"fevriye"),tsc(u8"mas"),tsc(u8"avril"),tsc(u8"me"),tsc(u8"jen"),tsc(u8"jiyè"),tsc(u8"out"),tsc(u8"septanm"),tsc(u8"oktòb"),tsc(u8"novanm"),tsc(u8"desanm")},.d_t_fmt=tsc(u8"%a %d %b %Y %T"),.d_fmt=tsc(u8"%d//%m//%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%a %d %b %Y %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1wWoOyY]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"Wi"),.nostr=tsc(u8"Non")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"%a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"509")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %R%N%%Z %t%N%c%N"),.country_name=tsc(u8"Ayiti"),.country_post=tsc(u8"HT"),.country_ab2=tsc(u8"HT"),.country_ab3=tsc(u8"HTI"),.country_num=332,.country_car=tsc(u8"RH"),.country_isbn=tsc(u8"99935"),.lang_name=tsc(u8"Kreyòl ayisyen"),.lang_ab=tsc(u8"ht"),.lang_term=tsc(u8"hat"),.lang_lib=tsc(u8"hat")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Kreyol locale for Haiti"),.source=tsc(u"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(u"https://github.com/expnkx/fast_io"),.contact=tsc(u"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(u"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Haitian Creole"),.territory=tsc(u"Haiti"),.revision=tsc(u"1.1"),.date=tsc(u"2013-08-09")},.monetary={.int_curr_symbol=tsc(u"HTG "),.currency_symbol=tsc(u"g"),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"dim"),tsc(u"len"),tsc(u"mad"),tsc(u"mèk"),tsc(u"jed"),tsc(u"van"),tsc(u"sam")},.day={tsc(u"dimanch"),tsc(u"lendi"),tsc(u"madi"),tsc(u"mèkredi"),tsc(u"jedi"),tsc(u"vandredi"),tsc(u"samdi")},.abmon={tsc(u"jan"),tsc(u"fev"),tsc(u"mas"),tsc(u"avr"),tsc(u"me"),tsc(u"jen"),tsc(u"jiy"),tsc(u"out"),tsc(u"sep"),tsc(u"okt"),tsc(u"nov"),tsc(u"des")},.mon={tsc(u"janvye"),tsc(u"fevriye"),tsc(u"mas"),tsc(u"avril"),tsc(u"me"),tsc(u"jen"),tsc(u"jiyè"),tsc(u"out"),tsc(u"septanm"),tsc(u"oktòb"),tsc(u"novanm"),tsc(u"desanm")},.d_t_fmt=tsc(u"%a %d %b %Y %T"),.d_fmt=tsc(u"%d//%m//%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%a %d %b %Y %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1wWoOyY]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"Wi"),.nostr=tsc(u"Non")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"%a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"509")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %R%N%%Z %t%N%c%N"),.country_name=tsc(u"Ayiti"),.country_post=tsc(u"HT"),.country_ab2=tsc(u"HT"),.country_ab3=tsc(u"HTI"),.country_num=332,.country_car=tsc(u"RH"),.country_isbn=tsc(u"99935"),.lang_name=tsc(u"Kreyòl ayisyen"),.lang_ab=tsc(u"ht"),.lang_term=tsc(u"hat"),.lang_lib=tsc(u"hat")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Kreyol locale for Haiti"),.source=tsc(U"Sugar Labs // OLPC\t\t;\t\tfast_io"),.address=tsc(U"https://github.com/expnkx/fast_io"),.contact=tsc(U"sugarlabs.org\t\t;\t\tfast_io"),.email=tsc(U"libc-alpha@sourceware.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Haitian Creole"),.territory=tsc(U"Haiti"),.revision=tsc(U"1.1"),.date=tsc(U"2013-08-09")},.monetary={.int_curr_symbol=tsc(U"HTG "),.currency_symbol=tsc(U"g"),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"dim"),tsc(U"len"),tsc(U"mad"),tsc(U"mèk"),tsc(U"jed"),tsc(U"van"),tsc(U"sam")},.day={tsc(U"dimanch"),tsc(U"lendi"),tsc(U"madi"),tsc(U"mèkredi"),tsc(U"jedi"),tsc(U"vandredi"),tsc(U"samdi")},.abmon={tsc(U"jan"),tsc(U"fev"),tsc(U"mas"),tsc(U"avr"),tsc(U"me"),tsc(U"jen"),tsc(U"jiy"),tsc(U"out"),tsc(U"sep"),tsc(U"okt"),tsc(U"nov"),tsc(U"des")},.mon={tsc(U"janvye"),tsc(U"fevriye"),tsc(U"mas"),tsc(U"avril"),tsc(U"me"),tsc(U"jen"),tsc(U"jiyè"),tsc(U"out"),tsc(U"septanm"),tsc(U"oktòb"),tsc(U"novanm"),tsc(U"desanm")},.d_t_fmt=tsc(U"%a %d %b %Y %T"),.d_fmt=tsc(U"%d//%m//%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%a %d %b %Y %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1wWoOyY]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"Wi"),.nostr=tsc(U"Non")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"%a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"509")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %R%N%%Z %t%N%c%N"),.country_name=tsc(U"Ayiti"),.country_post=tsc(U"HT"),.country_ab2=tsc(U"HT"),.country_ab3=tsc(U"HTI"),.country_num=332,.country_car=tsc(U"RH"),.country_isbn=tsc(U"99935"),.lang_name=tsc(U"Kreyòl ayisyen"),.lang_ab=tsc(U"ht"),.lang_term=tsc(U"hat"),.lang_lib=tsc(U"hat")},.measurement={.measurement=1}};


}
}

#include"../main.h"