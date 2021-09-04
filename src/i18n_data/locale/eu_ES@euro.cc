#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Basque language locale for Spain with Euro"),.source=tsc("Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc("https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Basque"),.territory=tsc("Spain"),.revision=tsc("1.0"),.date=tsc("2000-08-21")},.monetary={.int_curr_symbol=tsc("EUR "),.currency_symbol=tsc("€"),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc("."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc("."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("ig."),tsc("al."),tsc("ar."),tsc("az."),tsc("og."),tsc("or."),tsc("lr.")},.day={tsc("igandea"),tsc("astelehena"),tsc("asteartea"),tsc("asteazkena"),tsc("osteguna"),tsc("ostirala"),tsc("larunbata")},.abmon={tsc("urt"),tsc("ots"),tsc("mar"),tsc("api"),tsc("mai"),tsc("eka"),tsc("uzt"),tsc("abu"),tsc("ira"),tsc("urr"),tsc("aza"),tsc("abe")},.mon={tsc("urtarrila"),tsc("otsaila"),tsc("martxoa"),tsc("apirila"),tsc("maiatza"),tsc("ekaina"),tsc("uztaila"),tsc("abuztua"),tsc("iraila"),tsc("urria"),tsc("azaroa"),tsc("abendua")},.d_t_fmt=tsc("%y-%m-%d %T"),.d_fmt=tsc("%a, %Y.eko %bren %da"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%y-%m-%d %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc("^[+1bBsSyY]"),.noexpr=tsc("^[-0eEnN]"),.yesstr=tsc("bai"),.nostr=tsc("ez")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.int_select=tsc("00"),.int_prefix=tsc("34")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Espainia"),.country_ab2=tsc("ES"),.country_ab3=tsc("ESP"),.country_num=724,.country_car=tsc("E"),.lang_name=tsc("euskara"),.lang_ab=tsc("eu"),.lang_term=tsc("eus"),.lang_lib=tsc("baq")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Basque language locale for Spain with Euro"),.source=tsc(L"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(L"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Basque"),.territory=tsc(L"Spain"),.revision=tsc(L"1.0"),.date=tsc(L"2000-08-21")},.monetary={.int_curr_symbol=tsc(L"EUR "),.currency_symbol=tsc(L"€"),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"ig."),tsc(L"al."),tsc(L"ar."),tsc(L"az."),tsc(L"og."),tsc(L"or."),tsc(L"lr.")},.day={tsc(L"igandea"),tsc(L"astelehena"),tsc(L"asteartea"),tsc(L"asteazkena"),tsc(L"osteguna"),tsc(L"ostirala"),tsc(L"larunbata")},.abmon={tsc(L"urt"),tsc(L"ots"),tsc(L"mar"),tsc(L"api"),tsc(L"mai"),tsc(L"eka"),tsc(L"uzt"),tsc(L"abu"),tsc(L"ira"),tsc(L"urr"),tsc(L"aza"),tsc(L"abe")},.mon={tsc(L"urtarrila"),tsc(L"otsaila"),tsc(L"martxoa"),tsc(L"apirila"),tsc(L"maiatza"),tsc(L"ekaina"),tsc(L"uztaila"),tsc(L"abuztua"),tsc(L"iraila"),tsc(L"urria"),tsc(L"azaroa"),tsc(L"abendua")},.d_t_fmt=tsc(L"%y-%m-%d %T"),.d_fmt=tsc(L"%a, %Y.eko %bren %da"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%y-%m-%d %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1bBsSyY]"),.noexpr=tsc(L"^[-0eEnN]"),.yesstr=tsc(L"bai"),.nostr=tsc(L"ez")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"34")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Espainia"),.country_ab2=tsc(L"ES"),.country_ab3=tsc(L"ESP"),.country_num=724,.country_car=tsc(L"E"),.lang_name=tsc(L"euskara"),.lang_ab=tsc(L"eu"),.lang_term=tsc(L"eus"),.lang_lib=tsc(L"baq")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Basque language locale for Spain with Euro"),.source=tsc(u8"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u8"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Basque"),.territory=tsc(u8"Spain"),.revision=tsc(u8"1.0"),.date=tsc(u8"2000-08-21")},.monetary={.int_curr_symbol=tsc(u8"EUR "),.currency_symbol=tsc(u8"€"),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"ig."),tsc(u8"al."),tsc(u8"ar."),tsc(u8"az."),tsc(u8"og."),tsc(u8"or."),tsc(u8"lr.")},.day={tsc(u8"igandea"),tsc(u8"astelehena"),tsc(u8"asteartea"),tsc(u8"asteazkena"),tsc(u8"osteguna"),tsc(u8"ostirala"),tsc(u8"larunbata")},.abmon={tsc(u8"urt"),tsc(u8"ots"),tsc(u8"mar"),tsc(u8"api"),tsc(u8"mai"),tsc(u8"eka"),tsc(u8"uzt"),tsc(u8"abu"),tsc(u8"ira"),tsc(u8"urr"),tsc(u8"aza"),tsc(u8"abe")},.mon={tsc(u8"urtarrila"),tsc(u8"otsaila"),tsc(u8"martxoa"),tsc(u8"apirila"),tsc(u8"maiatza"),tsc(u8"ekaina"),tsc(u8"uztaila"),tsc(u8"abuztua"),tsc(u8"iraila"),tsc(u8"urria"),tsc(u8"azaroa"),tsc(u8"abendua")},.d_t_fmt=tsc(u8"%y-%m-%d %T"),.d_fmt=tsc(u8"%a, %Y.eko %bren %da"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%y-%m-%d %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1bBsSyY]"),.noexpr=tsc(u8"^[-0eEnN]"),.yesstr=tsc(u8"bai"),.nostr=tsc(u8"ez")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"34")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Espainia"),.country_ab2=tsc(u8"ES"),.country_ab3=tsc(u8"ESP"),.country_num=724,.country_car=tsc(u8"E"),.lang_name=tsc(u8"euskara"),.lang_ab=tsc(u8"eu"),.lang_term=tsc(u8"eus"),.lang_lib=tsc(u8"baq")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Basque language locale for Spain with Euro"),.source=tsc(u"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Basque"),.territory=tsc(u"Spain"),.revision=tsc(u"1.0"),.date=tsc(u"2000-08-21")},.monetary={.int_curr_symbol=tsc(u"EUR "),.currency_symbol=tsc(u"€"),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"ig."),tsc(u"al."),tsc(u"ar."),tsc(u"az."),tsc(u"og."),tsc(u"or."),tsc(u"lr.")},.day={tsc(u"igandea"),tsc(u"astelehena"),tsc(u"asteartea"),tsc(u"asteazkena"),tsc(u"osteguna"),tsc(u"ostirala"),tsc(u"larunbata")},.abmon={tsc(u"urt"),tsc(u"ots"),tsc(u"mar"),tsc(u"api"),tsc(u"mai"),tsc(u"eka"),tsc(u"uzt"),tsc(u"abu"),tsc(u"ira"),tsc(u"urr"),tsc(u"aza"),tsc(u"abe")},.mon={tsc(u"urtarrila"),tsc(u"otsaila"),tsc(u"martxoa"),tsc(u"apirila"),tsc(u"maiatza"),tsc(u"ekaina"),tsc(u"uztaila"),tsc(u"abuztua"),tsc(u"iraila"),tsc(u"urria"),tsc(u"azaroa"),tsc(u"abendua")},.d_t_fmt=tsc(u"%y-%m-%d %T"),.d_fmt=tsc(u"%a, %Y.eko %bren %da"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%y-%m-%d %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1bBsSyY]"),.noexpr=tsc(u"^[-0eEnN]"),.yesstr=tsc(u"bai"),.nostr=tsc(u"ez")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"34")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Espainia"),.country_ab2=tsc(u"ES"),.country_ab3=tsc(u"ESP"),.country_num=724,.country_car=tsc(u"E"),.lang_name=tsc(u"euskara"),.lang_ab=tsc(u"eu"),.lang_term=tsc(u"eus"),.lang_lib=tsc(u"baq")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Basque language locale for Spain with Euro"),.source=tsc(U"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(U"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Basque"),.territory=tsc(U"Spain"),.revision=tsc(U"1.0"),.date=tsc(U"2000-08-21")},.monetary={.int_curr_symbol=tsc(U"EUR "),.currency_symbol=tsc(U"€"),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U"."),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U"."),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"ig."),tsc(U"al."),tsc(U"ar."),tsc(U"az."),tsc(U"og."),tsc(U"or."),tsc(U"lr.")},.day={tsc(U"igandea"),tsc(U"astelehena"),tsc(U"asteartea"),tsc(U"asteazkena"),tsc(U"osteguna"),tsc(U"ostirala"),tsc(U"larunbata")},.abmon={tsc(U"urt"),tsc(U"ots"),tsc(U"mar"),tsc(U"api"),tsc(U"mai"),tsc(U"eka"),tsc(U"uzt"),tsc(U"abu"),tsc(U"ira"),tsc(U"urr"),tsc(U"aza"),tsc(U"abe")},.mon={tsc(U"urtarrila"),tsc(U"otsaila"),tsc(U"martxoa"),tsc(U"apirila"),tsc(U"maiatza"),tsc(U"ekaina"),tsc(U"uztaila"),tsc(U"abuztua"),tsc(U"iraila"),tsc(U"urria"),tsc(U"azaroa"),tsc(U"abendua")},.d_t_fmt=tsc(U"%y-%m-%d %T"),.d_fmt=tsc(U"%a, %Y.eko %bren %da"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%y-%m-%d %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1bBsSyY]"),.noexpr=tsc(U"^[-0eEnN]"),.yesstr=tsc(U"bai"),.nostr=tsc(U"ez")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"34")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Espainia"),.country_ab2=tsc(U"ES"),.country_ab3=tsc(U"ESP"),.country_num=724,.country_car=tsc(U"E"),.lang_name=tsc(U"euskara"),.lang_ab=tsc(U"eu"),.lang_term=tsc(U"eus"),.lang_lib=tsc(U"baq")},.measurement={.measurement=1}};


}
}

#include"../main.h"