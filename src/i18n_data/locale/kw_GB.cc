#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Cornish locale for Britain"),.source=tsc("Alastair McKinstry\t\t;\t\tfast_io"),.address=tsc("Croí Lár, Ballinahalla, Maigh Cuilinn,, Co. Gaillimh, Ireland\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Cornish"),.territory=tsc("United Kingdom"),.revision=tsc("1.0"),.date=tsc("2000-06-29")},.monetary={.int_curr_symbol=tsc("GBP "),.currency_symbol=tsc("£"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("Sul"),tsc("Lun"),tsc("Mth"),tsc("Mhr"),tsc("Yow"),tsc("Gwe"),tsc("Sad")},.day={tsc("De Sul"),tsc("De Lun"),tsc("De Merth"),tsc("De Merher"),tsc("De Yow"),tsc("De Gwener"),tsc("De Sadorn")},.abmon={tsc("Gen"),tsc("Hwe"),tsc("Meu"),tsc("Ebr"),tsc("Me"),tsc("Met"),tsc("Gor"),tsc("Est"),tsc("Gwn"),tsc("Hed"),tsc("Du"),tsc("Kev")},.mon={tsc("mis Genver"),tsc("mis Hwevrer"),tsc("mis Meurth"),tsc("mis Ebrel"),tsc("mis Me"),tsc("mis Metheven"),tsc("mis Gortheren"),tsc("mis Est"),tsc("mis Gwynngala"),tsc("mis Hedra"),tsc("mis Du"),tsc("mis Kevardhu")},.d_t_fmt=tsc("%a %d %b %Y %T"),.d_fmt=tsc("%d//%m//%y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%a %d %b %Y %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc("^[+1eEyY]"),.noexpr=tsc("^[-0nN]"),.yesstr=tsc("ea"),.nostr=tsc("na")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00"),.int_prefix=tsc("44")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Rywvaneth Unys"),.country_ab2=tsc("GB"),.country_ab3=tsc("GBR"),.country_num=826,.country_car=tsc("GB"),.lang_name=tsc("kernewek"),.lang_ab=tsc("kw"),.lang_term=tsc("cor"),.lang_lib=tsc("cor")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Cornish locale for Britain"),.source=tsc(L"Alastair McKinstry\t\t;\t\tfast_io"),.address=tsc(L"Croí Lár, Ballinahalla, Maigh Cuilinn,, Co. Gaillimh, Ireland\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Cornish"),.territory=tsc(L"United Kingdom"),.revision=tsc(L"1.0"),.date=tsc(L"2000-06-29")},.monetary={.int_curr_symbol=tsc(L"GBP "),.currency_symbol=tsc(L"£"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"Sul"),tsc(L"Lun"),tsc(L"Mth"),tsc(L"Mhr"),tsc(L"Yow"),tsc(L"Gwe"),tsc(L"Sad")},.day={tsc(L"De Sul"),tsc(L"De Lun"),tsc(L"De Merth"),tsc(L"De Merher"),tsc(L"De Yow"),tsc(L"De Gwener"),tsc(L"De Sadorn")},.abmon={tsc(L"Gen"),tsc(L"Hwe"),tsc(L"Meu"),tsc(L"Ebr"),tsc(L"Me"),tsc(L"Met"),tsc(L"Gor"),tsc(L"Est"),tsc(L"Gwn"),tsc(L"Hed"),tsc(L"Du"),tsc(L"Kev")},.mon={tsc(L"mis Genver"),tsc(L"mis Hwevrer"),tsc(L"mis Meurth"),tsc(L"mis Ebrel"),tsc(L"mis Me"),tsc(L"mis Metheven"),tsc(L"mis Gortheren"),tsc(L"mis Est"),tsc(L"mis Gwynngala"),tsc(L"mis Hedra"),tsc(L"mis Du"),tsc(L"mis Kevardhu")},.d_t_fmt=tsc(L"%a %d %b %Y %T"),.d_fmt=tsc(L"%d//%m//%y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%a %d %b %Y %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1eEyY]"),.noexpr=tsc(L"^[-0nN]"),.yesstr=tsc(L"ea"),.nostr=tsc(L"na")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"44")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Rywvaneth Unys"),.country_ab2=tsc(L"GB"),.country_ab3=tsc(L"GBR"),.country_num=826,.country_car=tsc(L"GB"),.lang_name=tsc(L"kernewek"),.lang_ab=tsc(L"kw"),.lang_term=tsc(L"cor"),.lang_lib=tsc(L"cor")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Cornish locale for Britain"),.source=tsc(u8"Alastair McKinstry\t\t;\t\tfast_io"),.address=tsc(u8"Croí Lár, Ballinahalla, Maigh Cuilinn,, Co. Gaillimh, Ireland\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Cornish"),.territory=tsc(u8"United Kingdom"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-06-29")},.monetary={.int_curr_symbol=tsc(u8"GBP "),.currency_symbol=tsc(u8"£"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"Sul"),tsc(u8"Lun"),tsc(u8"Mth"),tsc(u8"Mhr"),tsc(u8"Yow"),tsc(u8"Gwe"),tsc(u8"Sad")},.day={tsc(u8"De Sul"),tsc(u8"De Lun"),tsc(u8"De Merth"),tsc(u8"De Merher"),tsc(u8"De Yow"),tsc(u8"De Gwener"),tsc(u8"De Sadorn")},.abmon={tsc(u8"Gen"),tsc(u8"Hwe"),tsc(u8"Meu"),tsc(u8"Ebr"),tsc(u8"Me"),tsc(u8"Met"),tsc(u8"Gor"),tsc(u8"Est"),tsc(u8"Gwn"),tsc(u8"Hed"),tsc(u8"Du"),tsc(u8"Kev")},.mon={tsc(u8"mis Genver"),tsc(u8"mis Hwevrer"),tsc(u8"mis Meurth"),tsc(u8"mis Ebrel"),tsc(u8"mis Me"),tsc(u8"mis Metheven"),tsc(u8"mis Gortheren"),tsc(u8"mis Est"),tsc(u8"mis Gwynngala"),tsc(u8"mis Hedra"),tsc(u8"mis Du"),tsc(u8"mis Kevardhu")},.d_t_fmt=tsc(u8"%a %d %b %Y %T"),.d_fmt=tsc(u8"%d//%m//%y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%a %d %b %Y %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1eEyY]"),.noexpr=tsc(u8"^[-0nN]"),.yesstr=tsc(u8"ea"),.nostr=tsc(u8"na")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"44")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Rywvaneth Unys"),.country_ab2=tsc(u8"GB"),.country_ab3=tsc(u8"GBR"),.country_num=826,.country_car=tsc(u8"GB"),.lang_name=tsc(u8"kernewek"),.lang_ab=tsc(u8"kw"),.lang_term=tsc(u8"cor"),.lang_lib=tsc(u8"cor")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Cornish locale for Britain"),.source=tsc(u"Alastair McKinstry\t\t;\t\tfast_io"),.address=tsc(u"Croí Lár, Ballinahalla, Maigh Cuilinn,, Co. Gaillimh, Ireland\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Cornish"),.territory=tsc(u"United Kingdom"),.revision=tsc(u"1.0"),.date=tsc(u"2000-06-29")},.monetary={.int_curr_symbol=tsc(u"GBP "),.currency_symbol=tsc(u"£"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"Sul"),tsc(u"Lun"),tsc(u"Mth"),tsc(u"Mhr"),tsc(u"Yow"),tsc(u"Gwe"),tsc(u"Sad")},.day={tsc(u"De Sul"),tsc(u"De Lun"),tsc(u"De Merth"),tsc(u"De Merher"),tsc(u"De Yow"),tsc(u"De Gwener"),tsc(u"De Sadorn")},.abmon={tsc(u"Gen"),tsc(u"Hwe"),tsc(u"Meu"),tsc(u"Ebr"),tsc(u"Me"),tsc(u"Met"),tsc(u"Gor"),tsc(u"Est"),tsc(u"Gwn"),tsc(u"Hed"),tsc(u"Du"),tsc(u"Kev")},.mon={tsc(u"mis Genver"),tsc(u"mis Hwevrer"),tsc(u"mis Meurth"),tsc(u"mis Ebrel"),tsc(u"mis Me"),tsc(u"mis Metheven"),tsc(u"mis Gortheren"),tsc(u"mis Est"),tsc(u"mis Gwynngala"),tsc(u"mis Hedra"),tsc(u"mis Du"),tsc(u"mis Kevardhu")},.d_t_fmt=tsc(u"%a %d %b %Y %T"),.d_fmt=tsc(u"%d//%m//%y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%a %d %b %Y %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1eEyY]"),.noexpr=tsc(u"^[-0nN]"),.yesstr=tsc(u"ea"),.nostr=tsc(u"na")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"44")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Rywvaneth Unys"),.country_ab2=tsc(u"GB"),.country_ab3=tsc(u"GBR"),.country_num=826,.country_car=tsc(u"GB"),.lang_name=tsc(u"kernewek"),.lang_ab=tsc(u"kw"),.lang_term=tsc(u"cor"),.lang_lib=tsc(u"cor")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Cornish locale for Britain"),.source=tsc(U"Alastair McKinstry\t\t;\t\tfast_io"),.address=tsc(U"Croí Lár, Ballinahalla, Maigh Cuilinn,, Co. Gaillimh, Ireland\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Cornish"),.territory=tsc(U"United Kingdom"),.revision=tsc(U"1.0"),.date=tsc(U"2000-06-29")},.monetary={.int_curr_symbol=tsc(U"GBP "),.currency_symbol=tsc(U"£"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"Sul"),tsc(U"Lun"),tsc(U"Mth"),tsc(U"Mhr"),tsc(U"Yow"),tsc(U"Gwe"),tsc(U"Sad")},.day={tsc(U"De Sul"),tsc(U"De Lun"),tsc(U"De Merth"),tsc(U"De Merher"),tsc(U"De Yow"),tsc(U"De Gwener"),tsc(U"De Sadorn")},.abmon={tsc(U"Gen"),tsc(U"Hwe"),tsc(U"Meu"),tsc(U"Ebr"),tsc(U"Me"),tsc(U"Met"),tsc(U"Gor"),tsc(U"Est"),tsc(U"Gwn"),tsc(U"Hed"),tsc(U"Du"),tsc(U"Kev")},.mon={tsc(U"mis Genver"),tsc(U"mis Hwevrer"),tsc(U"mis Meurth"),tsc(U"mis Ebrel"),tsc(U"mis Me"),tsc(U"mis Metheven"),tsc(U"mis Gortheren"),tsc(U"mis Est"),tsc(U"mis Gwynngala"),tsc(U"mis Hedra"),tsc(U"mis Du"),tsc(U"mis Kevardhu")},.d_t_fmt=tsc(U"%a %d %b %Y %T"),.d_fmt=tsc(U"%d//%m//%y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%a %d %b %Y %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1eEyY]"),.noexpr=tsc(U"^[-0nN]"),.yesstr=tsc(U"ea"),.nostr=tsc(U"na")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"44")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Rywvaneth Unys"),.country_ab2=tsc(U"GB"),.country_ab3=tsc(U"GBR"),.country_num=826,.country_car=tsc(U"GB"),.lang_name=tsc(U"kernewek"),.lang_ab=tsc(U"kw"),.lang_term=tsc(U"cor"),.lang_lib=tsc(U"cor")},.measurement={.measurement=1}};


}
}

#include"../main.h"