#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Breton locale for France with Euro"),.source=tsc("Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc("https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Breton"),.territory=tsc("France"),.revision=tsc("1.0"),.date=tsc("2002-02-28")},.monetary={.int_curr_symbol=tsc("EUR "),.currency_symbol=tsc("€"),.mon_decimal_point=tsc(","),.mon_thousands_sep=tsc(" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(","),.thousands_sep=tsc(" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("sul"),tsc("lun"),tsc("meu"),tsc("mer"),tsc("yao"),tsc("gwe"),tsc("sad")},.day={tsc("sul"),tsc("lun"),tsc("meurzh"),tsc("mercʼher"),tsc("yaou"),tsc("gwener"),tsc("sadorn")},.abmon={tsc("Gen "),tsc("Cʼhw"),tsc("Meu "),tsc("Ebr "),tsc("Mae "),tsc("Mezh"),tsc("Gou "),tsc("Eos "),tsc("Gwe "),tsc("Her "),tsc("Du  "),tsc("Ker ")},.mon={tsc("Genver"),tsc("Cʼhwevrer"),tsc("Meurzh"),tsc("Ebrel"),tsc("Mae"),tsc("Mezheven"),tsc("Gouere"),tsc("Eost"),tsc("Gwengolo"),tsc("Here"),tsc("Du"),tsc("Kerzu")},.d_t_fmt=tsc("Dʼar %A %d a viz %B %Y %T"),.d_fmt=tsc("%d.%m.%Y"),.t_fmt=tsc("%T"),.t_fmt_ampm=tsc("%Ie%M:%S %p"),.date_fmt=tsc("Dʼar %A %d a viz %B %Y %T %Z"),.am_pm={tsc(" "),tsc(" ")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc("^[+1oOyY]"),.noexpr=tsc("^[-0kKnN]"),.yesstr=tsc("ya"),.nostr=tsc("ket")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("%a %l"),.int_select=tsc("00"),.int_prefix=tsc("33")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc("%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc("Frañs"),.country_ab2=tsc("FR"),.country_ab3=tsc("FRA"),.country_num=250,.country_car=tsc("F"),.country_isbn=tsc("979-10"),.lang_name=tsc("brezhoneg"),.lang_ab=tsc("br"),.lang_term=tsc("bre"),.lang_lib=tsc("bre")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Breton locale for France with Euro"),.source=tsc(L"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(L"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Breton"),.territory=tsc(L"France"),.revision=tsc(L"1.0"),.date=tsc(L"2002-02-28")},.monetary={.int_curr_symbol=tsc(L"EUR "),.currency_symbol=tsc(L"€"),.mon_decimal_point=tsc(L","),.mon_thousands_sep=tsc(L" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L","),.thousands_sep=tsc(L" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"sul"),tsc(L"lun"),tsc(L"meu"),tsc(L"mer"),tsc(L"yao"),tsc(L"gwe"),tsc(L"sad")},.day={tsc(L"sul"),tsc(L"lun"),tsc(L"meurzh"),tsc(L"mercʼher"),tsc(L"yaou"),tsc(L"gwener"),tsc(L"sadorn")},.abmon={tsc(L"Gen "),tsc(L"Cʼhw"),tsc(L"Meu "),tsc(L"Ebr "),tsc(L"Mae "),tsc(L"Mezh"),tsc(L"Gou "),tsc(L"Eos "),tsc(L"Gwe "),tsc(L"Her "),tsc(L"Du  "),tsc(L"Ker ")},.mon={tsc(L"Genver"),tsc(L"Cʼhwevrer"),tsc(L"Meurzh"),tsc(L"Ebrel"),tsc(L"Mae"),tsc(L"Mezheven"),tsc(L"Gouere"),tsc(L"Eost"),tsc(L"Gwengolo"),tsc(L"Here"),tsc(L"Du"),tsc(L"Kerzu")},.d_t_fmt=tsc(L"Dʼar %A %d a viz %B %Y %T"),.d_fmt=tsc(L"%d.%m.%Y"),.t_fmt=tsc(L"%T"),.t_fmt_ampm=tsc(L"%Ie%M:%S %p"),.date_fmt=tsc(L"Dʼar %A %d a viz %B %Y %T %Z"),.am_pm={tsc(L" "),tsc(L" ")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1oOyY]"),.noexpr=tsc(L"^[-0kKnN]"),.yesstr=tsc(L"ya"),.nostr=tsc(L"ket")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"%a %l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"33")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(L"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(L"Frañs"),.country_ab2=tsc(L"FR"),.country_ab3=tsc(L"FRA"),.country_num=250,.country_car=tsc(L"F"),.country_isbn=tsc(L"979-10"),.lang_name=tsc(L"brezhoneg"),.lang_ab=tsc(L"br"),.lang_term=tsc(L"bre"),.lang_lib=tsc(L"bre")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Breton locale for France with Euro"),.source=tsc(u8"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u8"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Breton"),.territory=tsc(u8"France"),.revision=tsc(u8"1.0"),.date=tsc(u8"2002-02-28")},.monetary={.int_curr_symbol=tsc(u8"EUR "),.currency_symbol=tsc(u8"€"),.mon_decimal_point=tsc(u8","),.mon_thousands_sep=tsc(u8" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8","),.thousands_sep=tsc(u8" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"sul"),tsc(u8"lun"),tsc(u8"meu"),tsc(u8"mer"),tsc(u8"yao"),tsc(u8"gwe"),tsc(u8"sad")},.day={tsc(u8"sul"),tsc(u8"lun"),tsc(u8"meurzh"),tsc(u8"mercʼher"),tsc(u8"yaou"),tsc(u8"gwener"),tsc(u8"sadorn")},.abmon={tsc(u8"Gen "),tsc(u8"Cʼhw"),tsc(u8"Meu "),tsc(u8"Ebr "),tsc(u8"Mae "),tsc(u8"Mezh"),tsc(u8"Gou "),tsc(u8"Eos "),tsc(u8"Gwe "),tsc(u8"Her "),tsc(u8"Du  "),tsc(u8"Ker ")},.mon={tsc(u8"Genver"),tsc(u8"Cʼhwevrer"),tsc(u8"Meurzh"),tsc(u8"Ebrel"),tsc(u8"Mae"),tsc(u8"Mezheven"),tsc(u8"Gouere"),tsc(u8"Eost"),tsc(u8"Gwengolo"),tsc(u8"Here"),tsc(u8"Du"),tsc(u8"Kerzu")},.d_t_fmt=tsc(u8"Dʼar %A %d a viz %B %Y %T"),.d_fmt=tsc(u8"%d.%m.%Y"),.t_fmt=tsc(u8"%T"),.t_fmt_ampm=tsc(u8"%Ie%M:%S %p"),.date_fmt=tsc(u8"Dʼar %A %d a viz %B %Y %T %Z"),.am_pm={tsc(u8" "),tsc(u8" ")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1oOyY]"),.noexpr=tsc(u8"^[-0kKnN]"),.yesstr=tsc(u8"ya"),.nostr=tsc(u8"ket")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"%a %l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"33")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u8"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u8"Frañs"),.country_ab2=tsc(u8"FR"),.country_ab3=tsc(u8"FRA"),.country_num=250,.country_car=tsc(u8"F"),.country_isbn=tsc(u8"979-10"),.lang_name=tsc(u8"brezhoneg"),.lang_ab=tsc(u8"br"),.lang_term=tsc(u8"bre"),.lang_lib=tsc(u8"bre")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Breton locale for France with Euro"),.source=tsc(u"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(u"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Breton"),.territory=tsc(u"France"),.revision=tsc(u"1.0"),.date=tsc(u"2002-02-28")},.monetary={.int_curr_symbol=tsc(u"EUR "),.currency_symbol=tsc(u"€"),.mon_decimal_point=tsc(u","),.mon_thousands_sep=tsc(u" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u","),.thousands_sep=tsc(u" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"sul"),tsc(u"lun"),tsc(u"meu"),tsc(u"mer"),tsc(u"yao"),tsc(u"gwe"),tsc(u"sad")},.day={tsc(u"sul"),tsc(u"lun"),tsc(u"meurzh"),tsc(u"mercʼher"),tsc(u"yaou"),tsc(u"gwener"),tsc(u"sadorn")},.abmon={tsc(u"Gen "),tsc(u"Cʼhw"),tsc(u"Meu "),tsc(u"Ebr "),tsc(u"Mae "),tsc(u"Mezh"),tsc(u"Gou "),tsc(u"Eos "),tsc(u"Gwe "),tsc(u"Her "),tsc(u"Du  "),tsc(u"Ker ")},.mon={tsc(u"Genver"),tsc(u"Cʼhwevrer"),tsc(u"Meurzh"),tsc(u"Ebrel"),tsc(u"Mae"),tsc(u"Mezheven"),tsc(u"Gouere"),tsc(u"Eost"),tsc(u"Gwengolo"),tsc(u"Here"),tsc(u"Du"),tsc(u"Kerzu")},.d_t_fmt=tsc(u"Dʼar %A %d a viz %B %Y %T"),.d_fmt=tsc(u"%d.%m.%Y"),.t_fmt=tsc(u"%T"),.t_fmt_ampm=tsc(u"%Ie%M:%S %p"),.date_fmt=tsc(u"Dʼar %A %d a viz %B %Y %T %Z"),.am_pm={tsc(u" "),tsc(u" ")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1oOyY]"),.noexpr=tsc(u"^[-0kKnN]"),.yesstr=tsc(u"ya"),.nostr=tsc(u"ket")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"%a %l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"33")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(u"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(u"Frañs"),.country_ab2=tsc(u"FR"),.country_ab3=tsc(u"FRA"),.country_num=250,.country_car=tsc(u"F"),.country_isbn=tsc(u"979-10"),.lang_name=tsc(u"brezhoneg"),.lang_ab=tsc(u"br"),.lang_term=tsc(u"bre"),.lang_lib=tsc(u"bre")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Breton locale for France with Euro"),.source=tsc(U"Free Software Foundation, Inc.\t\t;\t\tfast_io"),.address=tsc(U"https://www.gnu.org/software/libc/\t\t;\t\thttps://github.com/expnkx/fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Breton"),.territory=tsc(U"France"),.revision=tsc(U"1.0"),.date=tsc(U"2002-02-28")},.monetary={.int_curr_symbol=tsc(U"EUR "),.currency_symbol=tsc(U"€"),.mon_decimal_point=tsc(U","),.mon_thousands_sep=tsc(U" "),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=0,.p_sep_by_space=1,.n_cs_precedes=0,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U","),.thousands_sep=tsc(U" "),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"sul"),tsc(U"lun"),tsc(U"meu"),tsc(U"mer"),tsc(U"yao"),tsc(U"gwe"),tsc(U"sad")},.day={tsc(U"sul"),tsc(U"lun"),tsc(U"meurzh"),tsc(U"mercʼher"),tsc(U"yaou"),tsc(U"gwener"),tsc(U"sadorn")},.abmon={tsc(U"Gen "),tsc(U"Cʼhw"),tsc(U"Meu "),tsc(U"Ebr "),tsc(U"Mae "),tsc(U"Mezh"),tsc(U"Gou "),tsc(U"Eos "),tsc(U"Gwe "),tsc(U"Her "),tsc(U"Du  "),tsc(U"Ker ")},.mon={tsc(U"Genver"),tsc(U"Cʼhwevrer"),tsc(U"Meurzh"),tsc(U"Ebrel"),tsc(U"Mae"),tsc(U"Mezheven"),tsc(U"Gouere"),tsc(U"Eost"),tsc(U"Gwengolo"),tsc(U"Here"),tsc(U"Du"),tsc(U"Kerzu")},.d_t_fmt=tsc(U"Dʼar %A %d a viz %B %Y %T"),.d_fmt=tsc(U"%d.%m.%Y"),.t_fmt=tsc(U"%T"),.t_fmt_ampm=tsc(U"%Ie%M:%S %p"),.date_fmt=tsc(U"Dʼar %A %d a viz %B %Y %T %Z"),.am_pm={tsc(U" "),tsc(U" ")},.week={7,19971130,4},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1oOyY]"),.noexpr=tsc(U"^[-0kKnN]"),.yesstr=tsc(U"ya"),.nostr=tsc(U"ket")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"%a %l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"33")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f")},.address={.postal_fmt=tsc(U"%f%N%a%N%d%N%b%N%s %h %e %r%N%z %T%N%c%N"),.country_name=tsc(U"Frañs"),.country_ab2=tsc(U"FR"),.country_ab3=tsc(U"FRA"),.country_num=250,.country_car=tsc(U"F"),.country_isbn=tsc(U"979-10"),.lang_name=tsc(U"brezhoneg"),.lang_ab=tsc(U"br"),.lang_term=tsc(U"bre"),.lang_lib=tsc(U"bre")},.measurement={.measurement=1}};


}
}

#include"../main.h"