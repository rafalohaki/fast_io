#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Greenlandic locale for Greenland"),.source=tsc("Danish Standards Association\t\t;\t\tfast_io"),.address=tsc("Kollegievej 6, DK-2920 Charlottenlund, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Kalaallisut"),.territory=tsc("Greenland"),.revision=tsc("1.0"),.date=tsc("2000-06-29")},.monetary={.int_curr_symbol=tsc("DKK "),.currency_symbol=tsc("kr."),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc("."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc("."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("sap"),tsc("ata"),tsc("mar"),tsc("pin"),tsc("sis"),tsc("tal"),tsc("arf")},.day={tsc("sapaat"),tsc("ataasinngorneq"),tsc("marlunngorneq"),tsc("pingasunngorneq"),tsc("sisamanngorneq"),tsc("tallimanngorneq"),tsc("arfininngorneq")},.abmon={tsc("jan"),tsc("feb"),tsc("mar"),tsc("apr"),tsc("maj"),tsc("jun"),tsc("jul"),tsc("aug"),tsc("sep"),tsc("okt"),tsc("nov"),tsc("dec")},.mon={tsc("januaarip"),tsc("februaarip"),tsc("marsip"),tsc("apriilip"),tsc("maajip"),tsc("juunip"),tsc("juulip"),tsc("aggustip"),tsc("septembarip"),tsc("oktobarip"),tsc("novembarip"),tsc("decembarip")},.d_t_fmt=tsc("%a %b %d %Y %T"),.d_fmt=tsc("%b %d %Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%a %b %d %Y %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1JjYyAa]"),.noexpr=tsc("^[-0Nn]"),.yesstr=tsc("aap"),.nostr=tsc("naagga")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00"),.int_prefix=tsc("299")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Kalaallit Nunaat"),.country_ab2=tsc("GL"),.country_ab3=tsc("GRL"),.country_num=304,.country_car=tsc("KN"),.lang_name=tsc("kalaallisut"),.lang_ab=tsc("kl"),.lang_term=tsc("kal"),.lang_lib=tsc("kal")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Greenlandic locale for Greenland"),.source=tsc(L"Danish Standards Association\t\t;\t\tfast_io"),.address=tsc(L"Kollegievej 6, DK-2920 Charlottenlund, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Kalaallisut"),.territory=tsc(L"Greenland"),.revision=tsc(L"1.0"),.date=tsc(L"2000-06-29")},.monetary={.int_curr_symbol=tsc(L"DKK "),.currency_symbol=tsc(L"kr."),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"sap"),tsc(L"ata"),tsc(L"mar"),tsc(L"pin"),tsc(L"sis"),tsc(L"tal"),tsc(L"arf")},.day={tsc(L"sapaat"),tsc(L"ataasinngorneq"),tsc(L"marlunngorneq"),tsc(L"pingasunngorneq"),tsc(L"sisamanngorneq"),tsc(L"tallimanngorneq"),tsc(L"arfininngorneq")},.abmon={tsc(L"jan"),tsc(L"feb"),tsc(L"mar"),tsc(L"apr"),tsc(L"maj"),tsc(L"jun"),tsc(L"jul"),tsc(L"aug"),tsc(L"sep"),tsc(L"okt"),tsc(L"nov"),tsc(L"dec")},.mon={tsc(L"januaarip"),tsc(L"februaarip"),tsc(L"marsip"),tsc(L"apriilip"),tsc(L"maajip"),tsc(L"juunip"),tsc(L"juulip"),tsc(L"aggustip"),tsc(L"septembarip"),tsc(L"oktobarip"),tsc(L"novembarip"),tsc(L"decembarip")},.d_t_fmt=tsc(L"%a %b %d %Y %T"),.d_fmt=tsc(L"%b %d %Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%a %b %d %Y %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1JjYyAa]"),.noexpr=tsc(L"^[-0Nn]"),.yesstr=tsc(L"aap"),.nostr=tsc(L"naagga")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"299")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Kalaallit Nunaat"),.country_ab2=tsc(L"GL"),.country_ab3=tsc(L"GRL"),.country_num=304,.country_car=tsc(L"KN"),.lang_name=tsc(L"kalaallisut"),.lang_ab=tsc(L"kl"),.lang_term=tsc(L"kal"),.lang_lib=tsc(L"kal")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Greenlandic locale for Greenland"),.source=tsc(u8"Danish Standards Association\t\t;\t\tfast_io"),.address=tsc(u8"Kollegievej 6, DK-2920 Charlottenlund, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Kalaallisut"),.territory=tsc(u8"Greenland"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-06-29")},.monetary={.int_curr_symbol=tsc(u8"DKK "),.currency_symbol=tsc(u8"kr."),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"sap"),tsc(u8"ata"),tsc(u8"mar"),tsc(u8"pin"),tsc(u8"sis"),tsc(u8"tal"),tsc(u8"arf")},.day={tsc(u8"sapaat"),tsc(u8"ataasinngorneq"),tsc(u8"marlunngorneq"),tsc(u8"pingasunngorneq"),tsc(u8"sisamanngorneq"),tsc(u8"tallimanngorneq"),tsc(u8"arfininngorneq")},.abmon={tsc(u8"jan"),tsc(u8"feb"),tsc(u8"mar"),tsc(u8"apr"),tsc(u8"maj"),tsc(u8"jun"),tsc(u8"jul"),tsc(u8"aug"),tsc(u8"sep"),tsc(u8"okt"),tsc(u8"nov"),tsc(u8"dec")},.mon={tsc(u8"januaarip"),tsc(u8"februaarip"),tsc(u8"marsip"),tsc(u8"apriilip"),tsc(u8"maajip"),tsc(u8"juunip"),tsc(u8"juulip"),tsc(u8"aggustip"),tsc(u8"septembarip"),tsc(u8"oktobarip"),tsc(u8"novembarip"),tsc(u8"decembarip")},.d_t_fmt=tsc(u8"%a %b %d %Y %T"),.d_fmt=tsc(u8"%b %d %Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%a %b %d %Y %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1JjYyAa]"),.noexpr=tsc(u8"^[-0Nn]"),.yesstr=tsc(u8"aap"),.nostr=tsc(u8"naagga")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"299")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Kalaallit Nunaat"),.country_ab2=tsc(u8"GL"),.country_ab3=tsc(u8"GRL"),.country_num=304,.country_car=tsc(u8"KN"),.lang_name=tsc(u8"kalaallisut"),.lang_ab=tsc(u8"kl"),.lang_term=tsc(u8"kal"),.lang_lib=tsc(u8"kal")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Greenlandic locale for Greenland"),.source=tsc(u"Danish Standards Association\t\t;\t\tfast_io"),.address=tsc(u"Kollegievej 6, DK-2920 Charlottenlund, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Kalaallisut"),.territory=tsc(u"Greenland"),.revision=tsc(u"1.0"),.date=tsc(u"2000-06-29")},.monetary={.int_curr_symbol=tsc(u"DKK "),.currency_symbol=tsc(u"kr."),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"sap"),tsc(u"ata"),tsc(u"mar"),tsc(u"pin"),tsc(u"sis"),tsc(u"tal"),tsc(u"arf")},.day={tsc(u"sapaat"),tsc(u"ataasinngorneq"),tsc(u"marlunngorneq"),tsc(u"pingasunngorneq"),tsc(u"sisamanngorneq"),tsc(u"tallimanngorneq"),tsc(u"arfininngorneq")},.abmon={tsc(u"jan"),tsc(u"feb"),tsc(u"mar"),tsc(u"apr"),tsc(u"maj"),tsc(u"jun"),tsc(u"jul"),tsc(u"aug"),tsc(u"sep"),tsc(u"okt"),tsc(u"nov"),tsc(u"dec")},.mon={tsc(u"januaarip"),tsc(u"februaarip"),tsc(u"marsip"),tsc(u"apriilip"),tsc(u"maajip"),tsc(u"juunip"),tsc(u"juulip"),tsc(u"aggustip"),tsc(u"septembarip"),tsc(u"oktobarip"),tsc(u"novembarip"),tsc(u"decembarip")},.d_t_fmt=tsc(u"%a %b %d %Y %T"),.d_fmt=tsc(u"%b %d %Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%a %b %d %Y %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1JjYyAa]"),.noexpr=tsc(u"^[-0Nn]"),.yesstr=tsc(u"aap"),.nostr=tsc(u"naagga")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"299")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Kalaallit Nunaat"),.country_ab2=tsc(u"GL"),.country_ab3=tsc(u"GRL"),.country_num=304,.country_car=tsc(u"KN"),.lang_name=tsc(u"kalaallisut"),.lang_ab=tsc(u"kl"),.lang_term=tsc(u"kal"),.lang_lib=tsc(u"kal")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Greenlandic locale for Greenland"),.source=tsc(U"Danish Standards Association\t\t;\t\tfast_io"),.address=tsc(U"Kollegievej 6, DK-2920 Charlottenlund, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Kalaallisut"),.territory=tsc(U"Greenland"),.revision=tsc(U"1.0"),.date=tsc(U"2000-06-29")},.monetary={.int_curr_symbol=tsc(U"DKK "),.currency_symbol=tsc(U"kr."),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=2,.n_cs_precedes=1,.n_sep_by_space=2,.p_sign_posn=4,.n_sign_posn=4},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"sap"),tsc(U"ata"),tsc(U"mar"),tsc(U"pin"),tsc(U"sis"),tsc(U"tal"),tsc(U"arf")},.day={tsc(U"sapaat"),tsc(U"ataasinngorneq"),tsc(U"marlunngorneq"),tsc(U"pingasunngorneq"),tsc(U"sisamanngorneq"),tsc(U"tallimanngorneq"),tsc(U"arfininngorneq")},.abmon={tsc(U"jan"),tsc(U"feb"),tsc(U"mar"),tsc(U"apr"),tsc(U"maj"),tsc(U"jun"),tsc(U"jul"),tsc(U"aug"),tsc(U"sep"),tsc(U"okt"),tsc(U"nov"),tsc(U"dec")},.mon={tsc(U"januaarip"),tsc(U"februaarip"),tsc(U"marsip"),tsc(U"apriilip"),tsc(U"maajip"),tsc(U"juunip"),tsc(U"juulip"),tsc(U"aggustip"),tsc(U"septembarip"),tsc(U"oktobarip"),tsc(U"novembarip"),tsc(U"decembarip")},.d_t_fmt=tsc(U"%a %b %d %Y %T"),.d_fmt=tsc(U"%b %d %Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%a %b %d %Y %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1JjYyAa]"),.noexpr=tsc(U"^[-0Nn]"),.yesstr=tsc(U"aap"),.nostr=tsc(U"naagga")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"299")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Kalaallit Nunaat"),.country_ab2=tsc(U"GL"),.country_ab3=tsc(U"GRL"),.country_num=304,.country_car=tsc(U"KN"),.lang_name=tsc(U"kalaallisut"),.lang_ab=tsc(U"kl"),.lang_term=tsc(U"kal"),.lang_lib=tsc(U"kal")},.measurement={.measurement=1}};


}
}

#include"../main.h"