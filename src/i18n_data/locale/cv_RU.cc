#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Chuvash locale for Russia"),.source=tsc("RAP\t\t;\t\tfast_io"),.address=tsc("Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Chuvash"),.territory=tsc("Russia"),.revision=tsc("1.0"),.date=tsc("2000-06-29")},.monetary={.int_curr_symbol=tsc("RUB "),.currency_symbol=tsc("₽"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc(" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("vr"),tsc("tn"),tsc("yt"),tsc("jn"),tsc("kş"),tsc("er"),tsc("šm")},.day={tsc("vyrsarnikun"),tsc("tuntikun"),tsc("ytlarikun"),tsc("junkun"),tsc("kĕşnernikun"),tsc("ernekun"),tsc("šămatkun")},.abmon={tsc("KĂR"),tsc("NAR"),tsc("PUŠ"),tsc("AKA"),tsc("ŞU"),tsc("ŞĔR"),tsc("UTĂ"),tsc("ŞUR"),tsc("AVĂ"),tsc("JUP"),tsc("CÜK"),tsc("RAŠ")},.mon={tsc("kărlac"),tsc("narăs"),tsc("puš"),tsc("aka"),tsc("şu"),tsc("şĕrtme"),tsc("ută"),tsc("şurla"),tsc("avăn"),tsc("jupa"),tsc("cük"),tsc("raštav")},.d_t_fmt=tsc("%a %d %b %Y %T"),.d_fmt=tsc("%d.%m.%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%a %d %b %Y %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1yY]"),.noexpr=tsc("^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("0~10"),.int_prefix=tsc("7")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc("RU"),.country_ab3=tsc("RUS"),.country_num=643,.country_car=tsc("RUS"),.lang_name=tsc("Чӑвашла"),.lang_ab=tsc("cv"),.lang_term=tsc("chv"),.lang_lib=tsc("chv")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Chuvash locale for Russia"),.source=tsc(L"RAP\t\t;\t\tfast_io"),.address=tsc(L"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Chuvash"),.territory=tsc(L"Russia"),.revision=tsc(L"1.0"),.date=tsc(L"2000-06-29")},.monetary={.int_curr_symbol=tsc(L"RUB "),.currency_symbol=tsc(L"₽"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"vr"),tsc(L"tn"),tsc(L"yt"),tsc(L"jn"),tsc(L"kş"),tsc(L"er"),tsc(L"šm")},.day={tsc(L"vyrsarnikun"),tsc(L"tuntikun"),tsc(L"ytlarikun"),tsc(L"junkun"),tsc(L"kĕşnernikun"),tsc(L"ernekun"),tsc(L"šămatkun")},.abmon={tsc(L"KĂR"),tsc(L"NAR"),tsc(L"PUŠ"),tsc(L"AKA"),tsc(L"ŞU"),tsc(L"ŞĔR"),tsc(L"UTĂ"),tsc(L"ŞUR"),tsc(L"AVĂ"),tsc(L"JUP"),tsc(L"CÜK"),tsc(L"RAŠ")},.mon={tsc(L"kărlac"),tsc(L"narăs"),tsc(L"puš"),tsc(L"aka"),tsc(L"şu"),tsc(L"şĕrtme"),tsc(L"ută"),tsc(L"şurla"),tsc(L"avăn"),tsc(L"jupa"),tsc(L"cük"),tsc(L"raštav")},.d_t_fmt=tsc(L"%a %d %b %Y %T"),.d_fmt=tsc(L"%d.%m.%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%a %d %b %Y %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1yY]"),.noexpr=tsc(L"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"0~10"),.int_prefix=tsc(L"7")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(L"RU"),.country_ab3=tsc(L"RUS"),.country_num=643,.country_car=tsc(L"RUS"),.lang_name=tsc(L"Чӑвашла"),.lang_ab=tsc(L"cv"),.lang_term=tsc(L"chv"),.lang_lib=tsc(L"chv")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Chuvash locale for Russia"),.source=tsc(u8"RAP\t\t;\t\tfast_io"),.address=tsc(u8"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Chuvash"),.territory=tsc(u8"Russia"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-06-29")},.monetary={.int_curr_symbol=tsc(u8"RUB "),.currency_symbol=tsc(u8"₽"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"vr"),tsc(u8"tn"),tsc(u8"yt"),tsc(u8"jn"),tsc(u8"kş"),tsc(u8"er"),tsc(u8"šm")},.day={tsc(u8"vyrsarnikun"),tsc(u8"tuntikun"),tsc(u8"ytlarikun"),tsc(u8"junkun"),tsc(u8"kĕşnernikun"),tsc(u8"ernekun"),tsc(u8"šămatkun")},.abmon={tsc(u8"KĂR"),tsc(u8"NAR"),tsc(u8"PUŠ"),tsc(u8"AKA"),tsc(u8"ŞU"),tsc(u8"ŞĔR"),tsc(u8"UTĂ"),tsc(u8"ŞUR"),tsc(u8"AVĂ"),tsc(u8"JUP"),tsc(u8"CÜK"),tsc(u8"RAŠ")},.mon={tsc(u8"kărlac"),tsc(u8"narăs"),tsc(u8"puš"),tsc(u8"aka"),tsc(u8"şu"),tsc(u8"şĕrtme"),tsc(u8"ută"),tsc(u8"şurla"),tsc(u8"avăn"),tsc(u8"jupa"),tsc(u8"cük"),tsc(u8"raštav")},.d_t_fmt=tsc(u8"%a %d %b %Y %T"),.d_fmt=tsc(u8"%d.%m.%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%a %d %b %Y %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1yY]"),.noexpr=tsc(u8"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"0~10"),.int_prefix=tsc(u8"7")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(u8"RU"),.country_ab3=tsc(u8"RUS"),.country_num=643,.country_car=tsc(u8"RUS"),.lang_name=tsc(u8"Чӑвашла"),.lang_ab=tsc(u8"cv"),.lang_term=tsc(u8"chv"),.lang_lib=tsc(u8"chv")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Chuvash locale for Russia"),.source=tsc(u"RAP\t\t;\t\tfast_io"),.address=tsc(u"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Chuvash"),.territory=tsc(u"Russia"),.revision=tsc(u"1.0"),.date=tsc(u"2000-06-29")},.monetary={.int_curr_symbol=tsc(u"RUB "),.currency_symbol=tsc(u"₽"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"vr"),tsc(u"tn"),tsc(u"yt"),tsc(u"jn"),tsc(u"kş"),tsc(u"er"),tsc(u"šm")},.day={tsc(u"vyrsarnikun"),tsc(u"tuntikun"),tsc(u"ytlarikun"),tsc(u"junkun"),tsc(u"kĕşnernikun"),tsc(u"ernekun"),tsc(u"šămatkun")},.abmon={tsc(u"KĂR"),tsc(u"NAR"),tsc(u"PUŠ"),tsc(u"AKA"),tsc(u"ŞU"),tsc(u"ŞĔR"),tsc(u"UTĂ"),tsc(u"ŞUR"),tsc(u"AVĂ"),tsc(u"JUP"),tsc(u"CÜK"),tsc(u"RAŠ")},.mon={tsc(u"kărlac"),tsc(u"narăs"),tsc(u"puš"),tsc(u"aka"),tsc(u"şu"),tsc(u"şĕrtme"),tsc(u"ută"),tsc(u"şurla"),tsc(u"avăn"),tsc(u"jupa"),tsc(u"cük"),tsc(u"raštav")},.d_t_fmt=tsc(u"%a %d %b %Y %T"),.d_fmt=tsc(u"%d.%m.%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%a %d %b %Y %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1yY]"),.noexpr=tsc(u"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"0~10"),.int_prefix=tsc(u"7")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(u"RU"),.country_ab3=tsc(u"RUS"),.country_num=643,.country_car=tsc(u"RUS"),.lang_name=tsc(u"Чӑвашла"),.lang_ab=tsc(u"cv"),.lang_term=tsc(u"chv"),.lang_lib=tsc(u"chv")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Chuvash locale for Russia"),.source=tsc(U"RAP\t\t;\t\tfast_io"),.address=tsc(U"Sankt Jorgens Alle 8, DK-1615 Kobenhavn V, Danmark\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Chuvash"),.territory=tsc(U"Russia"),.revision=tsc(U"1.0"),.date=tsc(U"2000-06-29")},.monetary={.int_curr_symbol=tsc(U"RUB "),.currency_symbol=tsc(U"₽"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"vr"),tsc(U"tn"),tsc(U"yt"),tsc(U"jn"),tsc(U"kş"),tsc(U"er"),tsc(U"šm")},.day={tsc(U"vyrsarnikun"),tsc(U"tuntikun"),tsc(U"ytlarikun"),tsc(U"junkun"),tsc(U"kĕşnernikun"),tsc(U"ernekun"),tsc(U"šămatkun")},.abmon={tsc(U"KĂR"),tsc(U"NAR"),tsc(U"PUŠ"),tsc(U"AKA"),tsc(U"ŞU"),tsc(U"ŞĔR"),tsc(U"UTĂ"),tsc(U"ŞUR"),tsc(U"AVĂ"),tsc(U"JUP"),tsc(U"CÜK"),tsc(U"RAŠ")},.mon={tsc(U"kărlac"),tsc(U"narăs"),tsc(U"puš"),tsc(U"aka"),tsc(U"şu"),tsc(U"şĕrtme"),tsc(U"ută"),tsc(U"şurla"),tsc(U"avăn"),tsc(U"jupa"),tsc(U"cük"),tsc(U"raštav")},.d_t_fmt=tsc(U"%a %d %b %Y %T"),.d_fmt=tsc(U"%d.%m.%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%a %d %b %Y %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1yY]"),.noexpr=tsc(U"^[-0nN]")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"0~10"),.int_prefix=tsc(U"7")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_ab2=tsc(U"RU"),.country_ab3=tsc(U"RUS"),.country_num=643,.country_car=tsc(U"RUS"),.lang_name=tsc(U"Чӑвашла"),.lang_ab=tsc(U"cv"),.lang_term=tsc(U"chv"),.lang_lib=tsc(U"chv")},.measurement={.measurement=1}};


}
}

#include"../main.h"