#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Inuktitut language locale for Nunavut, Canada"),.source=tsc("fast_io"),.address=tsc("https://github.com/expnkx/fast_io"),.contact=tsc("Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc("pablo@mandriva.com;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Inuktitut"),.territory=tsc("Canada"),.revision=tsc("0.1"),.date=tsc("2001-05-04")},.monetary={.int_curr_symbol=tsc("CAD "),.currency_symbol=tsc("$"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("ᓈ"),tsc("ᓇ"),tsc("ᓕ"),tsc("ᐱ"),tsc("ᕿ"),tsc("ᐅ"),tsc("ᓯ")},.day={tsc("ᓈᑦᑎᖑᔭᕐᕕᒃ"),tsc("ᓇᒡᒐᔾᔭᐅ"),tsc("ᓇᒡᒐᔾᔭᐅᓕᖅᑭᑦ"),tsc("ᐱᖓᓲᓕᖅᓯᐅᑦ"),tsc("ᕿᑎᖅᑰᑦ"),tsc("ᐅᓪᓗᕈᓘᑐᐃᓇᖅ"),tsc("ᓯᕙᑖᕕᒃ")},.abmon={tsc("ᔮᓄ"),tsc("ᕕᕗ"),tsc("ᒪᔅ"),tsc("ᐃᐳ"),tsc("ᒪᐃ"),tsc("ᔪᓂ"),tsc("ᔪᓚ"),tsc("ᐊᒋ"),tsc("ᓯᑎ"),tsc("ᐊᑦ"),tsc("ᓄᕕ"),tsc("ᑎᓯ")},.mon={tsc("ᔮᓄᐊᓕ"),tsc("ᕕᕗᐊᓕ"),tsc("ᒪᔅᓯ"),tsc("ᐃᐳᓗ"),tsc("ᒪᐃ"),tsc("ᔪᓂ"),tsc("ᔪᓚᐃ"),tsc("ᐊᒋᓯ"),tsc("ᓯᑎᕙ"),tsc("ᐊᑦᑐᕙ"),tsc("ᓄᕕᕙ"),tsc("ᑎᓯᕝᕙ")},.d_t_fmt=tsc("%a %d %b %Y %r"),.d_fmt=tsc("%m//%d//%y"),.t_fmt=tsc("%r"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %d %b %Y %r %Z"),.am_pm={tsc("AM"),tsc("PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yYsS]"),.noexpr=tsc("^[-0nN]")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("011"),.int_prefix=tsc("1")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc("ᑲᓇᑕ"),.country_post=tsc("CA"),.country_ab2=tsc("CA"),.country_ab3=tsc("CAN"),.country_num=124,.country_car=tsc("CDN"),.lang_name=tsc("ᐃᓄᒃᑎᑐᑦ"),.lang_ab=tsc("iu"),.lang_term=tsc("iku"),.lang_lib=tsc("iku")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Inuktitut language locale for Nunavut, Canada"),.source=tsc(L"fast_io"),.address=tsc(L"https://github.com/expnkx/fast_io"),.contact=tsc(L"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(L"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Inuktitut"),.territory=tsc(L"Canada"),.revision=tsc(L"0.1"),.date=tsc(L"2001-05-04")},.monetary={.int_curr_symbol=tsc(L"CAD "),.currency_symbol=tsc(L"$"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"ᓈ"),tsc(L"ᓇ"),tsc(L"ᓕ"),tsc(L"ᐱ"),tsc(L"ᕿ"),tsc(L"ᐅ"),tsc(L"ᓯ")},.day={tsc(L"ᓈᑦᑎᖑᔭᕐᕕᒃ"),tsc(L"ᓇᒡᒐᔾᔭᐅ"),tsc(L"ᓇᒡᒐᔾᔭᐅᓕᖅᑭᑦ"),tsc(L"ᐱᖓᓲᓕᖅᓯᐅᑦ"),tsc(L"ᕿᑎᖅᑰᑦ"),tsc(L"ᐅᓪᓗᕈᓘᑐᐃᓇᖅ"),tsc(L"ᓯᕙᑖᕕᒃ")},.abmon={tsc(L"ᔮᓄ"),tsc(L"ᕕᕗ"),tsc(L"ᒪᔅ"),tsc(L"ᐃᐳ"),tsc(L"ᒪᐃ"),tsc(L"ᔪᓂ"),tsc(L"ᔪᓚ"),tsc(L"ᐊᒋ"),tsc(L"ᓯᑎ"),tsc(L"ᐊᑦ"),tsc(L"ᓄᕕ"),tsc(L"ᑎᓯ")},.mon={tsc(L"ᔮᓄᐊᓕ"),tsc(L"ᕕᕗᐊᓕ"),tsc(L"ᒪᔅᓯ"),tsc(L"ᐃᐳᓗ"),tsc(L"ᒪᐃ"),tsc(L"ᔪᓂ"),tsc(L"ᔪᓚᐃ"),tsc(L"ᐊᒋᓯ"),tsc(L"ᓯᑎᕙ"),tsc(L"ᐊᑦᑐᕙ"),tsc(L"ᓄᕕᕙ"),tsc(L"ᑎᓯᕝᕙ")},.d_t_fmt=tsc(L"%a %d %b %Y %r"),.d_fmt=tsc(L"%m//%d//%y"),.t_fmt=tsc(L"%r"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %d %b %Y %r %Z"),.am_pm={tsc(L"AM"),tsc(L"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yYsS]"),.noexpr=tsc(L"^[-0nN]")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"011"),.int_prefix=tsc(L"1")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(L"ᑲᓇᑕ"),.country_post=tsc(L"CA"),.country_ab2=tsc(L"CA"),.country_ab3=tsc(L"CAN"),.country_num=124,.country_car=tsc(L"CDN"),.lang_name=tsc(L"ᐃᓄᒃᑎᑐᑦ"),.lang_ab=tsc(L"iu"),.lang_term=tsc(L"iku"),.lang_lib=tsc(L"iku")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Inuktitut language locale for Nunavut, Canada"),.source=tsc(u8"fast_io"),.address=tsc(u8"https://github.com/expnkx/fast_io"),.contact=tsc(u8"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(u8"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Inuktitut"),.territory=tsc(u8"Canada"),.revision=tsc(u8"0.1"),.date=tsc(u8"2001-05-04")},.monetary={.int_curr_symbol=tsc(u8"CAD "),.currency_symbol=tsc(u8"$"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"ᓈ"),tsc(u8"ᓇ"),tsc(u8"ᓕ"),tsc(u8"ᐱ"),tsc(u8"ᕿ"),tsc(u8"ᐅ"),tsc(u8"ᓯ")},.day={tsc(u8"ᓈᑦᑎᖑᔭᕐᕕᒃ"),tsc(u8"ᓇᒡᒐᔾᔭᐅ"),tsc(u8"ᓇᒡᒐᔾᔭᐅᓕᖅᑭᑦ"),tsc(u8"ᐱᖓᓲᓕᖅᓯᐅᑦ"),tsc(u8"ᕿᑎᖅᑰᑦ"),tsc(u8"ᐅᓪᓗᕈᓘᑐᐃᓇᖅ"),tsc(u8"ᓯᕙᑖᕕᒃ")},.abmon={tsc(u8"ᔮᓄ"),tsc(u8"ᕕᕗ"),tsc(u8"ᒪᔅ"),tsc(u8"ᐃᐳ"),tsc(u8"ᒪᐃ"),tsc(u8"ᔪᓂ"),tsc(u8"ᔪᓚ"),tsc(u8"ᐊᒋ"),tsc(u8"ᓯᑎ"),tsc(u8"ᐊᑦ"),tsc(u8"ᓄᕕ"),tsc(u8"ᑎᓯ")},.mon={tsc(u8"ᔮᓄᐊᓕ"),tsc(u8"ᕕᕗᐊᓕ"),tsc(u8"ᒪᔅᓯ"),tsc(u8"ᐃᐳᓗ"),tsc(u8"ᒪᐃ"),tsc(u8"ᔪᓂ"),tsc(u8"ᔪᓚᐃ"),tsc(u8"ᐊᒋᓯ"),tsc(u8"ᓯᑎᕙ"),tsc(u8"ᐊᑦᑐᕙ"),tsc(u8"ᓄᕕᕙ"),tsc(u8"ᑎᓯᕝᕙ")},.d_t_fmt=tsc(u8"%a %d %b %Y %r"),.d_fmt=tsc(u8"%m//%d//%y"),.t_fmt=tsc(u8"%r"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %d %b %Y %r %Z"),.am_pm={tsc(u8"AM"),tsc(u8"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yYsS]"),.noexpr=tsc(u8"^[-0nN]")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"011"),.int_prefix=tsc(u8"1")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(u8"ᑲᓇᑕ"),.country_post=tsc(u8"CA"),.country_ab2=tsc(u8"CA"),.country_ab3=tsc(u8"CAN"),.country_num=124,.country_car=tsc(u8"CDN"),.lang_name=tsc(u8"ᐃᓄᒃᑎᑐᑦ"),.lang_ab=tsc(u8"iu"),.lang_term=tsc(u8"iku"),.lang_lib=tsc(u8"iku")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Inuktitut language locale for Nunavut, Canada"),.source=tsc(u"fast_io"),.address=tsc(u"https://github.com/expnkx/fast_io"),.contact=tsc(u"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(u"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Inuktitut"),.territory=tsc(u"Canada"),.revision=tsc(u"0.1"),.date=tsc(u"2001-05-04")},.monetary={.int_curr_symbol=tsc(u"CAD "),.currency_symbol=tsc(u"$"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"ᓈ"),tsc(u"ᓇ"),tsc(u"ᓕ"),tsc(u"ᐱ"),tsc(u"ᕿ"),tsc(u"ᐅ"),tsc(u"ᓯ")},.day={tsc(u"ᓈᑦᑎᖑᔭᕐᕕᒃ"),tsc(u"ᓇᒡᒐᔾᔭᐅ"),tsc(u"ᓇᒡᒐᔾᔭᐅᓕᖅᑭᑦ"),tsc(u"ᐱᖓᓲᓕᖅᓯᐅᑦ"),tsc(u"ᕿᑎᖅᑰᑦ"),tsc(u"ᐅᓪᓗᕈᓘᑐᐃᓇᖅ"),tsc(u"ᓯᕙᑖᕕᒃ")},.abmon={tsc(u"ᔮᓄ"),tsc(u"ᕕᕗ"),tsc(u"ᒪᔅ"),tsc(u"ᐃᐳ"),tsc(u"ᒪᐃ"),tsc(u"ᔪᓂ"),tsc(u"ᔪᓚ"),tsc(u"ᐊᒋ"),tsc(u"ᓯᑎ"),tsc(u"ᐊᑦ"),tsc(u"ᓄᕕ"),tsc(u"ᑎᓯ")},.mon={tsc(u"ᔮᓄᐊᓕ"),tsc(u"ᕕᕗᐊᓕ"),tsc(u"ᒪᔅᓯ"),tsc(u"ᐃᐳᓗ"),tsc(u"ᒪᐃ"),tsc(u"ᔪᓂ"),tsc(u"ᔪᓚᐃ"),tsc(u"ᐊᒋᓯ"),tsc(u"ᓯᑎᕙ"),tsc(u"ᐊᑦᑐᕙ"),tsc(u"ᓄᕕᕙ"),tsc(u"ᑎᓯᕝᕙ")},.d_t_fmt=tsc(u"%a %d %b %Y %r"),.d_fmt=tsc(u"%m//%d//%y"),.t_fmt=tsc(u"%r"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %d %b %Y %r %Z"),.am_pm={tsc(u"AM"),tsc(u"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yYsS]"),.noexpr=tsc(u"^[-0nN]")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"011"),.int_prefix=tsc(u"1")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(u"ᑲᓇᑕ"),.country_post=tsc(u"CA"),.country_ab2=tsc(u"CA"),.country_ab3=tsc(u"CAN"),.country_num=124,.country_car=tsc(u"CDN"),.lang_name=tsc(u"ᐃᓄᒃᑎᑐᑦ"),.lang_ab=tsc(u"iu"),.lang_term=tsc(u"iku"),.lang_lib=tsc(u"iku")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Inuktitut language locale for Nunavut, Canada"),.source=tsc(U"fast_io"),.address=tsc(U"https://github.com/expnkx/fast_io"),.contact=tsc(U"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(U"pablo@mandriva.com;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Inuktitut"),.territory=tsc(U"Canada"),.revision=tsc(U"0.1"),.date=tsc(U"2001-05-04")},.monetary={.int_curr_symbol=tsc(U"CAD "),.currency_symbol=tsc(U"$"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"ᓈ"),tsc(U"ᓇ"),tsc(U"ᓕ"),tsc(U"ᐱ"),tsc(U"ᕿ"),tsc(U"ᐅ"),tsc(U"ᓯ")},.day={tsc(U"ᓈᑦᑎᖑᔭᕐᕕᒃ"),tsc(U"ᓇᒡᒐᔾᔭᐅ"),tsc(U"ᓇᒡᒐᔾᔭᐅᓕᖅᑭᑦ"),tsc(U"ᐱᖓᓲᓕᖅᓯᐅᑦ"),tsc(U"ᕿᑎᖅᑰᑦ"),tsc(U"ᐅᓪᓗᕈᓘᑐᐃᓇᖅ"),tsc(U"ᓯᕙᑖᕕᒃ")},.abmon={tsc(U"ᔮᓄ"),tsc(U"ᕕᕗ"),tsc(U"ᒪᔅ"),tsc(U"ᐃᐳ"),tsc(U"ᒪᐃ"),tsc(U"ᔪᓂ"),tsc(U"ᔪᓚ"),tsc(U"ᐊᒋ"),tsc(U"ᓯᑎ"),tsc(U"ᐊᑦ"),tsc(U"ᓄᕕ"),tsc(U"ᑎᓯ")},.mon={tsc(U"ᔮᓄᐊᓕ"),tsc(U"ᕕᕗᐊᓕ"),tsc(U"ᒪᔅᓯ"),tsc(U"ᐃᐳᓗ"),tsc(U"ᒪᐃ"),tsc(U"ᔪᓂ"),tsc(U"ᔪᓚᐃ"),tsc(U"ᐊᒋᓯ"),tsc(U"ᓯᑎᕙ"),tsc(U"ᐊᑦᑐᕙ"),tsc(U"ᓄᕕᕙ"),tsc(U"ᑎᓯᕝᕙ")},.d_t_fmt=tsc(U"%a %d %b %Y %r"),.d_fmt=tsc(U"%m//%d//%y"),.t_fmt=tsc(U"%r"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %d %b %Y %r %Z"),.am_pm={tsc(U"AM"),tsc(U"PM")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yYsS]"),.noexpr=tsc(U"^[-0nN]")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"011"),.int_prefix=tsc(U"1")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%a%N%f%N%d%N%b%N%h %s %e %r%N%T %z%N%c%N"),.country_name=tsc(U"ᑲᓇᑕ"),.country_post=tsc(U"CA"),.country_ab2=tsc(U"CA"),.country_ab3=tsc(U"CAN"),.country_num=124,.country_car=tsc(U"CDN"),.lang_name=tsc(U"ᐃᓄᒃᑎᑐᑦ"),.lang_ab=tsc(U"iu"),.lang_term=tsc(U"iku"),.lang_lib=tsc(U"iku")},.measurement={.measurement=1}};


}
}

#include"../main.h"