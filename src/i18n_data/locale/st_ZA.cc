#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Sotho locale for South Africa"),.source=tsc("Zuza Software Foundation (Translate.org.za)\t\t;\t\tfast_io"),.address=tsc("PO Box 28364, Sunnyside, 0132, South Africa\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("Dwayne Bailey\t\t;\t\tfast_io"),.email=tsc("dwayne@translate.org.za;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Southern Sotho"),.territory=tsc("South Africa"),.revision=tsc("0.3"),.date=tsc("2005-10-13")},.monetary={.int_curr_symbol=tsc("ZAR "),.currency_symbol=tsc("R"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Son"),tsc("Mma"),tsc("Bed"),tsc("Rar"),tsc("Ne"),tsc("Hla"),tsc("Moq")},.day={tsc("Sontaha"),tsc("Mantaha"),tsc("Labobedi"),tsc("Laboraro"),tsc("Labone"),tsc("Labohlano"),tsc("Moqebelo")},.abmon={tsc("Phe"),tsc("Hla"),tsc("TlH"),tsc("Mme"),tsc("Mot"),tsc("Jan"),tsc("Upu"),tsc("Pha"),tsc("Leo"),tsc("Mph"),tsc("Pud"),tsc("Tsh")},.mon={tsc("Pherekgong"),tsc("Hlakola"),tsc("Tlhakubele"),tsc("Mmese"),tsc("Motsheanong"),tsc("Phupjane"),tsc("Phupu"),tsc("Phato"),tsc("Leotse"),tsc("Mphalane"),tsc("Pudungwana"),tsc("Tshitwe")},.d_t_fmt=tsc("%a %-e %b %Y %T %Z"),.d_fmt=tsc("%d//%m//%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%a %b %-e %H:%M:%S %Z %Y"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yY]"),.noexpr=tsc("^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("(%A) %l"),.int_select=tsc("00"),.int_prefix=tsc("27")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Afrika Borwa"),.country_post=tsc("ZA"),.country_ab2=tsc("ZA"),.country_ab3=tsc("ZAF"),.country_num=710,.country_car=tsc("ZA"),.lang_name=tsc("Sesotho"),.lang_ab=tsc("st"),.lang_term=tsc("sot"),.lang_lib=tsc("sot")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Sotho locale for South Africa"),.source=tsc(L"Zuza Software Foundation (Translate.org.za)\t\t;\t\tfast_io"),.address=tsc(L"PO Box 28364, Sunnyside, 0132, South Africa\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"Dwayne Bailey\t\t;\t\tfast_io"),.email=tsc(L"dwayne@translate.org.za;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Southern Sotho"),.territory=tsc(L"South Africa"),.revision=tsc(L"0.3"),.date=tsc(L"2005-10-13")},.monetary={.int_curr_symbol=tsc(L"ZAR "),.currency_symbol=tsc(L"R"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Son"),tsc(L"Mma"),tsc(L"Bed"),tsc(L"Rar"),tsc(L"Ne"),tsc(L"Hla"),tsc(L"Moq")},.day={tsc(L"Sontaha"),tsc(L"Mantaha"),tsc(L"Labobedi"),tsc(L"Laboraro"),tsc(L"Labone"),tsc(L"Labohlano"),tsc(L"Moqebelo")},.abmon={tsc(L"Phe"),tsc(L"Hla"),tsc(L"TlH"),tsc(L"Mme"),tsc(L"Mot"),tsc(L"Jan"),tsc(L"Upu"),tsc(L"Pha"),tsc(L"Leo"),tsc(L"Mph"),tsc(L"Pud"),tsc(L"Tsh")},.mon={tsc(L"Pherekgong"),tsc(L"Hlakola"),tsc(L"Tlhakubele"),tsc(L"Mmese"),tsc(L"Motsheanong"),tsc(L"Phupjane"),tsc(L"Phupu"),tsc(L"Phato"),tsc(L"Leotse"),tsc(L"Mphalane"),tsc(L"Pudungwana"),tsc(L"Tshitwe")},.d_t_fmt=tsc(L"%a %-e %b %Y %T %Z"),.d_fmt=tsc(L"%d//%m//%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%a %b %-e %H:%M:%S %Z %Y"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yY]"),.noexpr=tsc(L"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"(%A) %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"27")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Afrika Borwa"),.country_post=tsc(L"ZA"),.country_ab2=tsc(L"ZA"),.country_ab3=tsc(L"ZAF"),.country_num=710,.country_car=tsc(L"ZA"),.lang_name=tsc(L"Sesotho"),.lang_ab=tsc(L"st"),.lang_term=tsc(L"sot"),.lang_lib=tsc(L"sot")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Sotho locale for South Africa"),.source=tsc(u8"Zuza Software Foundation (Translate.org.za)\t\t;\t\tfast_io"),.address=tsc(u8"PO Box 28364, Sunnyside, 0132, South Africa\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"Dwayne Bailey\t\t;\t\tfast_io"),.email=tsc(u8"dwayne@translate.org.za;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Southern Sotho"),.territory=tsc(u8"South Africa"),.revision=tsc(u8"0.3"),.date=tsc(u8"2005-10-13")},.monetary={.int_curr_symbol=tsc(u8"ZAR "),.currency_symbol=tsc(u8"R"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Son"),tsc(u8"Mma"),tsc(u8"Bed"),tsc(u8"Rar"),tsc(u8"Ne"),tsc(u8"Hla"),tsc(u8"Moq")},.day={tsc(u8"Sontaha"),tsc(u8"Mantaha"),tsc(u8"Labobedi"),tsc(u8"Laboraro"),tsc(u8"Labone"),tsc(u8"Labohlano"),tsc(u8"Moqebelo")},.abmon={tsc(u8"Phe"),tsc(u8"Hla"),tsc(u8"TlH"),tsc(u8"Mme"),tsc(u8"Mot"),tsc(u8"Jan"),tsc(u8"Upu"),tsc(u8"Pha"),tsc(u8"Leo"),tsc(u8"Mph"),tsc(u8"Pud"),tsc(u8"Tsh")},.mon={tsc(u8"Pherekgong"),tsc(u8"Hlakola"),tsc(u8"Tlhakubele"),tsc(u8"Mmese"),tsc(u8"Motsheanong"),tsc(u8"Phupjane"),tsc(u8"Phupu"),tsc(u8"Phato"),tsc(u8"Leotse"),tsc(u8"Mphalane"),tsc(u8"Pudungwana"),tsc(u8"Tshitwe")},.d_t_fmt=tsc(u8"%a %-e %b %Y %T %Z"),.d_fmt=tsc(u8"%d//%m//%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%a %b %-e %H:%M:%S %Z %Y"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yY]"),.noexpr=tsc(u8"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"(%A) %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"27")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Afrika Borwa"),.country_post=tsc(u8"ZA"),.country_ab2=tsc(u8"ZA"),.country_ab3=tsc(u8"ZAF"),.country_num=710,.country_car=tsc(u8"ZA"),.lang_name=tsc(u8"Sesotho"),.lang_ab=tsc(u8"st"),.lang_term=tsc(u8"sot"),.lang_lib=tsc(u8"sot")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Sotho locale for South Africa"),.source=tsc(u"Zuza Software Foundation (Translate.org.za)\t\t;\t\tfast_io"),.address=tsc(u"PO Box 28364, Sunnyside, 0132, South Africa\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"Dwayne Bailey\t\t;\t\tfast_io"),.email=tsc(u"dwayne@translate.org.za;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Southern Sotho"),.territory=tsc(u"South Africa"),.revision=tsc(u"0.3"),.date=tsc(u"2005-10-13")},.monetary={.int_curr_symbol=tsc(u"ZAR "),.currency_symbol=tsc(u"R"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Son"),tsc(u"Mma"),tsc(u"Bed"),tsc(u"Rar"),tsc(u"Ne"),tsc(u"Hla"),tsc(u"Moq")},.day={tsc(u"Sontaha"),tsc(u"Mantaha"),tsc(u"Labobedi"),tsc(u"Laboraro"),tsc(u"Labone"),tsc(u"Labohlano"),tsc(u"Moqebelo")},.abmon={tsc(u"Phe"),tsc(u"Hla"),tsc(u"TlH"),tsc(u"Mme"),tsc(u"Mot"),tsc(u"Jan"),tsc(u"Upu"),tsc(u"Pha"),tsc(u"Leo"),tsc(u"Mph"),tsc(u"Pud"),tsc(u"Tsh")},.mon={tsc(u"Pherekgong"),tsc(u"Hlakola"),tsc(u"Tlhakubele"),tsc(u"Mmese"),tsc(u"Motsheanong"),tsc(u"Phupjane"),tsc(u"Phupu"),tsc(u"Phato"),tsc(u"Leotse"),tsc(u"Mphalane"),tsc(u"Pudungwana"),tsc(u"Tshitwe")},.d_t_fmt=tsc(u"%a %-e %b %Y %T %Z"),.d_fmt=tsc(u"%d//%m//%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%a %b %-e %H:%M:%S %Z %Y"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yY]"),.noexpr=tsc(u"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"(%A) %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"27")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Afrika Borwa"),.country_post=tsc(u"ZA"),.country_ab2=tsc(u"ZA"),.country_ab3=tsc(u"ZAF"),.country_num=710,.country_car=tsc(u"ZA"),.lang_name=tsc(u"Sesotho"),.lang_ab=tsc(u"st"),.lang_term=tsc(u"sot"),.lang_lib=tsc(u"sot")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Sotho locale for South Africa"),.source=tsc(U"Zuza Software Foundation (Translate.org.za)\t\t;\t\tfast_io"),.address=tsc(U"PO Box 28364, Sunnyside, 0132, South Africa\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"Dwayne Bailey\t\t;\t\tfast_io"),.email=tsc(U"dwayne@translate.org.za;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Southern Sotho"),.territory=tsc(U"South Africa"),.revision=tsc(U"0.3"),.date=tsc(U"2005-10-13")},.monetary={.int_curr_symbol=tsc(U"ZAR "),.currency_symbol=tsc(U"R"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Son"),tsc(U"Mma"),tsc(U"Bed"),tsc(U"Rar"),tsc(U"Ne"),tsc(U"Hla"),tsc(U"Moq")},.day={tsc(U"Sontaha"),tsc(U"Mantaha"),tsc(U"Labobedi"),tsc(U"Laboraro"),tsc(U"Labone"),tsc(U"Labohlano"),tsc(U"Moqebelo")},.abmon={tsc(U"Phe"),tsc(U"Hla"),tsc(U"TlH"),tsc(U"Mme"),tsc(U"Mot"),tsc(U"Jan"),tsc(U"Upu"),tsc(U"Pha"),tsc(U"Leo"),tsc(U"Mph"),tsc(U"Pud"),tsc(U"Tsh")},.mon={tsc(U"Pherekgong"),tsc(U"Hlakola"),tsc(U"Tlhakubele"),tsc(U"Mmese"),tsc(U"Motsheanong"),tsc(U"Phupjane"),tsc(U"Phupu"),tsc(U"Phato"),tsc(U"Leotse"),tsc(U"Mphalane"),tsc(U"Pudungwana"),tsc(U"Tshitwe")},.d_t_fmt=tsc(U"%a %-e %b %Y %T %Z"),.d_fmt=tsc(U"%d//%m//%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%a %b %-e %H:%M:%S %Z %Y"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yY]"),.noexpr=tsc(U"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"(%A) %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"27")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Afrika Borwa"),.country_post=tsc(U"ZA"),.country_ab2=tsc(U"ZA"),.country_ab3=tsc(U"ZAF"),.country_num=710,.country_car=tsc(U"ZA"),.lang_name=tsc(U"Sesotho"),.lang_ab=tsc(U"st"),.lang_term=tsc(U"sot"),.lang_lib=tsc(U"sot")},.measurement={.measurement=1}};


}
}

#include"../main.h"