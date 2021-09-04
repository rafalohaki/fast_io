#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Berber language locale for Morocco (tifinagh)"),.source=tsc("fast_io"),.address=tsc("https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc("pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Berber"),.territory=tsc("Morocco"),.revision=tsc("0.1"),.date=tsc("2002-06-26")},.monetary={.int_curr_symbol=tsc("MAD "),.currency_symbol=tsc("ⴷ.ⵎ."),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("baz"),tsc("bir"),tsc("iki"),tsc("üçü"),tsc("dör"),tsc("beş"),tsc("alt")},.day={tsc("bazar günü"),tsc("birinci gün"),tsc("ikinci gün"),tsc("üçüncü gün"),tsc("dördüncü gün"),tsc("beşinci gün"),tsc("altıncı gün")},.abmon={tsc("Yan"),tsc("Fev"),tsc("Mar"),tsc("Apr"),tsc("May"),tsc("İyn"),tsc("İyl"),tsc("Avq"),tsc("Sen"),tsc("Okt"),tsc("Noy"),tsc("Dek")},.mon={tsc("yanvar"),tsc("fevral"),tsc("mart"),tsc("aprel"),tsc("may"),tsc("iyun"),tsc("iyul"),tsc("avqust"),tsc("sentyabr"),tsc("oktyabr"),tsc("noyabr"),tsc("dekabr")},.d_t_fmt=tsc("%A, %d %B %Y %T"),.d_fmt=tsc("%d.%m.%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc(""),.date_fmt=tsc("%A, %d %B %Y %T %Z"),.am_pm={tsc(""),tsc("")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc("^[+1Bb]"),.noexpr=tsc("^[-0YyNn]"),.yesstr=tsc("bəli"),.nostr=tsc("yox")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("212")},.name={.name_fmt=tsc("%p%t%f%t%g"),.name_gen=tsc("-san"),.name_miss=tsc("Miss."),.name_mr=tsc("Mr."),.name_mrs=tsc("Mrs."),.name_ms=tsc("Ms.")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("ⵜⴰⴳⵍⴷⵉⵜ ⵏ ⵍⵎⵖⵔⵉⴱ"),.country_ab2=tsc("MA"),.country_ab3=tsc("MAR"),.country_num=504,.country_car=tsc("MA"),.lang_name=tsc("ⵜⴰⵎⴰⵣⵉⵖⵜ"),.lang_term=tsc("ber"),.lang_lib=tsc("ber")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Berber language locale for Morocco (tifinagh)"),.source=tsc(L"fast_io"),.address=tsc(L"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(L"pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Berber"),.territory=tsc(L"Morocco"),.revision=tsc(L"0.1"),.date=tsc(L"2002-06-26")},.monetary={.int_curr_symbol=tsc(L"MAD "),.currency_symbol=tsc(L"ⴷ.ⵎ."),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"baz"),tsc(L"bir"),tsc(L"iki"),tsc(L"üçü"),tsc(L"dör"),tsc(L"beş"),tsc(L"alt")},.day={tsc(L"bazar günü"),tsc(L"birinci gün"),tsc(L"ikinci gün"),tsc(L"üçüncü gün"),tsc(L"dördüncü gün"),tsc(L"beşinci gün"),tsc(L"altıncı gün")},.abmon={tsc(L"Yan"),tsc(L"Fev"),tsc(L"Mar"),tsc(L"Apr"),tsc(L"May"),tsc(L"İyn"),tsc(L"İyl"),tsc(L"Avq"),tsc(L"Sen"),tsc(L"Okt"),tsc(L"Noy"),tsc(L"Dek")},.mon={tsc(L"yanvar"),tsc(L"fevral"),tsc(L"mart"),tsc(L"aprel"),tsc(L"may"),tsc(L"iyun"),tsc(L"iyul"),tsc(L"avqust"),tsc(L"sentyabr"),tsc(L"oktyabr"),tsc(L"noyabr"),tsc(L"dekabr")},.d_t_fmt=tsc(L"%A, %d %B %Y %T"),.d_fmt=tsc(L"%d.%m.%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L""),.date_fmt=tsc(L"%A, %d %B %Y %T %Z"),.am_pm={tsc(L""),tsc(L"")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(L"^[+1Bb]"),.noexpr=tsc(L"^[-0YyNn]"),.yesstr=tsc(L"bəli"),.nostr=tsc(L"yox")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"212")},.name={.name_fmt=tsc(L"%p%t%f%t%g"),.name_gen=tsc(L"-san"),.name_miss=tsc(L"Miss."),.name_mr=tsc(L"Mr."),.name_mrs=tsc(L"Mrs."),.name_ms=tsc(L"Ms.")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"ⵜⴰⴳⵍⴷⵉⵜ ⵏ ⵍⵎⵖⵔⵉⴱ"),.country_ab2=tsc(L"MA"),.country_ab3=tsc(L"MAR"),.country_num=504,.country_car=tsc(L"MA"),.lang_name=tsc(L"ⵜⴰⵎⴰⵣⵉⵖⵜ"),.lang_term=tsc(L"ber"),.lang_lib=tsc(L"ber")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Berber language locale for Morocco (tifinagh)"),.source=tsc(u8"fast_io"),.address=tsc(u8"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(u8"pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Berber"),.territory=tsc(u8"Morocco"),.revision=tsc(u8"0.1"),.date=tsc(u8"2002-06-26")},.monetary={.int_curr_symbol=tsc(u8"MAD "),.currency_symbol=tsc(u8"ⴷ.ⵎ."),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"baz"),tsc(u8"bir"),tsc(u8"iki"),tsc(u8"üçü"),tsc(u8"dör"),tsc(u8"beş"),tsc(u8"alt")},.day={tsc(u8"bazar günü"),tsc(u8"birinci gün"),tsc(u8"ikinci gün"),tsc(u8"üçüncü gün"),tsc(u8"dördüncü gün"),tsc(u8"beşinci gün"),tsc(u8"altıncı gün")},.abmon={tsc(u8"Yan"),tsc(u8"Fev"),tsc(u8"Mar"),tsc(u8"Apr"),tsc(u8"May"),tsc(u8"İyn"),tsc(u8"İyl"),tsc(u8"Avq"),tsc(u8"Sen"),tsc(u8"Okt"),tsc(u8"Noy"),tsc(u8"Dek")},.mon={tsc(u8"yanvar"),tsc(u8"fevral"),tsc(u8"mart"),tsc(u8"aprel"),tsc(u8"may"),tsc(u8"iyun"),tsc(u8"iyul"),tsc(u8"avqust"),tsc(u8"sentyabr"),tsc(u8"oktyabr"),tsc(u8"noyabr"),tsc(u8"dekabr")},.d_t_fmt=tsc(u8"%A, %d %B %Y %T"),.d_fmt=tsc(u8"%d.%m.%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8""),.date_fmt=tsc(u8"%A, %d %B %Y %T %Z"),.am_pm={tsc(u8""),tsc(u8"")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(u8"^[+1Bb]"),.noexpr=tsc(u8"^[-0YyNn]"),.yesstr=tsc(u8"bəli"),.nostr=tsc(u8"yox")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"212")},.name={.name_fmt=tsc(u8"%p%t%f%t%g"),.name_gen=tsc(u8"-san"),.name_miss=tsc(u8"Miss."),.name_mr=tsc(u8"Mr."),.name_mrs=tsc(u8"Mrs."),.name_ms=tsc(u8"Ms.")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"ⵜⴰⴳⵍⴷⵉⵜ ⵏ ⵍⵎⵖⵔⵉⴱ"),.country_ab2=tsc(u8"MA"),.country_ab3=tsc(u8"MAR"),.country_num=504,.country_car=tsc(u8"MA"),.lang_name=tsc(u8"ⵜⴰⵎⴰⵣⵉⵖⵜ"),.lang_term=tsc(u8"ber"),.lang_lib=tsc(u8"ber")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Berber language locale for Morocco (tifinagh)"),.source=tsc(u"fast_io"),.address=tsc(u"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(u"pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Berber"),.territory=tsc(u"Morocco"),.revision=tsc(u"0.1"),.date=tsc(u"2002-06-26")},.monetary={.int_curr_symbol=tsc(u"MAD "),.currency_symbol=tsc(u"ⴷ.ⵎ."),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"baz"),tsc(u"bir"),tsc(u"iki"),tsc(u"üçü"),tsc(u"dör"),tsc(u"beş"),tsc(u"alt")},.day={tsc(u"bazar günü"),tsc(u"birinci gün"),tsc(u"ikinci gün"),tsc(u"üçüncü gün"),tsc(u"dördüncü gün"),tsc(u"beşinci gün"),tsc(u"altıncı gün")},.abmon={tsc(u"Yan"),tsc(u"Fev"),tsc(u"Mar"),tsc(u"Apr"),tsc(u"May"),tsc(u"İyn"),tsc(u"İyl"),tsc(u"Avq"),tsc(u"Sen"),tsc(u"Okt"),tsc(u"Noy"),tsc(u"Dek")},.mon={tsc(u"yanvar"),tsc(u"fevral"),tsc(u"mart"),tsc(u"aprel"),tsc(u"may"),tsc(u"iyun"),tsc(u"iyul"),tsc(u"avqust"),tsc(u"sentyabr"),tsc(u"oktyabr"),tsc(u"noyabr"),tsc(u"dekabr")},.d_t_fmt=tsc(u"%A, %d %B %Y %T"),.d_fmt=tsc(u"%d.%m.%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u""),.date_fmt=tsc(u"%A, %d %B %Y %T %Z"),.am_pm={tsc(u""),tsc(u"")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(u"^[+1Bb]"),.noexpr=tsc(u"^[-0YyNn]"),.yesstr=tsc(u"bəli"),.nostr=tsc(u"yox")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"212")},.name={.name_fmt=tsc(u"%p%t%f%t%g"),.name_gen=tsc(u"-san"),.name_miss=tsc(u"Miss."),.name_mr=tsc(u"Mr."),.name_mrs=tsc(u"Mrs."),.name_ms=tsc(u"Ms.")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"ⵜⴰⴳⵍⴷⵉⵜ ⵏ ⵍⵎⵖⵔⵉⴱ"),.country_ab2=tsc(u"MA"),.country_ab3=tsc(u"MAR"),.country_num=504,.country_car=tsc(u"MA"),.lang_name=tsc(u"ⵜⴰⵎⴰⵣⵉⵖⵜ"),.lang_term=tsc(u"ber"),.lang_lib=tsc(u"ber")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Berber language locale for Morocco (tifinagh)"),.source=tsc(U"fast_io"),.address=tsc(U"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"Pablo Saratxaga\t\t;\t\tfast_io"),.email=tsc(U"pablo@mandrakesoft.com;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Berber"),.territory=tsc(U"Morocco"),.revision=tsc(U"0.1"),.date=tsc(U"2002-06-26")},.monetary={.int_curr_symbol=tsc(U"MAD "),.currency_symbol=tsc(U"ⴷ.ⵎ."),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=3,.frac_digits=3,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=2},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"baz"),tsc(U"bir"),tsc(U"iki"),tsc(U"üçü"),tsc(U"dör"),tsc(U"beş"),tsc(U"alt")},.day={tsc(U"bazar günü"),tsc(U"birinci gün"),tsc(U"ikinci gün"),tsc(U"üçüncü gün"),tsc(U"dördüncü gün"),tsc(U"beşinci gün"),tsc(U"altıncı gün")},.abmon={tsc(U"Yan"),tsc(U"Fev"),tsc(U"Mar"),tsc(U"Apr"),tsc(U"May"),tsc(U"İyn"),tsc(U"İyl"),tsc(U"Avq"),tsc(U"Sen"),tsc(U"Okt"),tsc(U"Noy"),tsc(U"Dek")},.mon={tsc(U"yanvar"),tsc(U"fevral"),tsc(U"mart"),tsc(U"aprel"),tsc(U"may"),tsc(U"iyun"),tsc(U"iyul"),tsc(U"avqust"),tsc(U"sentyabr"),tsc(U"oktyabr"),tsc(U"noyabr"),tsc(U"dekabr")},.d_t_fmt=tsc(U"%A, %d %B %Y %T"),.d_fmt=tsc(U"%d.%m.%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U""),.date_fmt=tsc(U"%A, %d %B %Y %T %Z"),.am_pm={tsc(U""),tsc(U"")},.week={7,19971130,1},.first_weekday=7,.first_workday=1},.messages={.yesexpr=tsc(U"^[+1Bb]"),.noexpr=tsc(U"^[-0YyNn]"),.yesstr=tsc(U"bəli"),.nostr=tsc(U"yox")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"212")},.name={.name_fmt=tsc(U"%p%t%f%t%g"),.name_gen=tsc(U"-san"),.name_miss=tsc(U"Miss."),.name_mr=tsc(U"Mr."),.name_mrs=tsc(U"Mrs."),.name_ms=tsc(U"Ms.")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"ⵜⴰⴳⵍⴷⵉⵜ ⵏ ⵍⵎⵖⵔⵉⴱ"),.country_ab2=tsc(U"MA"),.country_ab3=tsc(U"MAR"),.country_num=504,.country_car=tsc(U"MA"),.lang_name=tsc(U"ⵜⴰⵎⴰⵣⵉⵖⵜ"),.lang_term=tsc(U"ber"),.lang_lib=tsc(U"ber")},.measurement={.measurement=1}};


}
}

#include"../main.h"