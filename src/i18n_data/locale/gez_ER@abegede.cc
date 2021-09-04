#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t monetary_mon_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Ge'ez language locale for Eritrea With Abegede Collation."),.source=tsc("Ge'ez Frontier Foundation\t\t;\t\tfast_io"),.address=tsc("7802 Solomon Seal Dr., Springfield, VA 22152, USA\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("locales@geez.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Geez"),.territory=tsc("Eritrea"),.revision=tsc("0.20"),.date=tsc("2003-07-05")},.monetary={.int_curr_symbol=tsc("ERN "),.currency_symbol=tsc("Nfk"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={monetary_mon_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc("")},.time={.abday={tsc("እኁድ"),tsc("ሰኑይ"),tsc("ሠሉስ"),tsc("ራብዕ"),tsc("ሐሙስ"),tsc("ዓርበ"),tsc("ቀዳሚ")},.day={tsc("እኁድ"),tsc("ሰኑይ"),tsc("ሠሉስ"),tsc("ራብዕ"),tsc("ሐሙስ"),tsc("ዓርበ"),tsc("ቀዳሚት")},.abmon={tsc("ጠሐረ"),tsc("ከተተ"),tsc("መገበ"),tsc("አኀዘ"),tsc("ግንባ"),tsc("ሠንየ"),tsc("ሐመለ"),tsc("ነሐሰ"),tsc("ከረመ"),tsc("ጠቀመ"),tsc("ኀደረ"),tsc("ኀሠሠ")},.mon={tsc("ጠሐረ"),tsc("ከተተ"),tsc("መገበ"),tsc("አኀዘ"),tsc("ግንባት"),tsc("ሠንየ"),tsc("ሐመለ"),tsc("ነሐሰ"),tsc("ከረመ"),tsc("ጠቀመ"),tsc("ኀደረ"),tsc("ኀሠሠ")},.d_t_fmt=tsc("%A፥%B፡%e፡መዓልት፡%Y፡%r፡%Z"),.d_fmt=tsc("%d//%m//%Y"),.t_fmt=tsc("%l:%M:%S፡%p"),.t_fmt_ampm=tsc("%l:%M:%S፡%p"),.date_fmt=tsc("%A፥%B፡%e፡መዓልት፡%r፡%Z፡%Y፡ዓ//ም"),.am_pm={tsc("ጽባሕ"),tsc("ምሴት")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc("^[+1yYእ]"),.noexpr=tsc("^[-0nNነ]"),.yesstr=tsc("እወ"),.nostr=tsc("ነኖእ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c %a %l"),.tel_dom_fmt=tsc("%a-%l"),.int_select=tsc("00"),.int_prefix=tsc("291")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f"),.name_gen=tsc(""),.name_miss=tsc("ወ//ሪት"),.name_mr={tsc("<U12A3><U1276>"),tsc("<U12A0><U12ED><U1270>")},.name_mrs=tsc("ወ//ሮ"),.name_ms=tsc("ወ//ሪት")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("ኤርትራ"),.country_post=tsc("ERI"),.country_ab2=tsc("ER"),.country_ab3=tsc("ERI"),.country_num=232,.country_car=tsc("ER"),.lang_name=tsc("ግዕዝ"),.lang_term=tsc("gez")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Ge'ez language locale for Eritrea With Abegede Collation."),.source=tsc(L"Ge'ez Frontier Foundation\t\t;\t\tfast_io"),.address=tsc(L"7802 Solomon Seal Dr., Springfield, VA 22152, USA\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"locales@geez.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Geez"),.territory=tsc(L"Eritrea"),.revision=tsc(L"0.20"),.date=tsc(L"2003-07-05")},.monetary={.int_curr_symbol=tsc(L"ERN "),.currency_symbol=tsc(L"Nfk"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={monetary_mon_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L"")},.time={.abday={tsc(L"እኁድ"),tsc(L"ሰኑይ"),tsc(L"ሠሉስ"),tsc(L"ራብዕ"),tsc(L"ሐሙስ"),tsc(L"ዓርበ"),tsc(L"ቀዳሚ")},.day={tsc(L"እኁድ"),tsc(L"ሰኑይ"),tsc(L"ሠሉስ"),tsc(L"ራብዕ"),tsc(L"ሐሙስ"),tsc(L"ዓርበ"),tsc(L"ቀዳሚት")},.abmon={tsc(L"ጠሐረ"),tsc(L"ከተተ"),tsc(L"መገበ"),tsc(L"አኀዘ"),tsc(L"ግንባ"),tsc(L"ሠንየ"),tsc(L"ሐመለ"),tsc(L"ነሐሰ"),tsc(L"ከረመ"),tsc(L"ጠቀመ"),tsc(L"ኀደረ"),tsc(L"ኀሠሠ")},.mon={tsc(L"ጠሐረ"),tsc(L"ከተተ"),tsc(L"መገበ"),tsc(L"አኀዘ"),tsc(L"ግንባት"),tsc(L"ሠንየ"),tsc(L"ሐመለ"),tsc(L"ነሐሰ"),tsc(L"ከረመ"),tsc(L"ጠቀመ"),tsc(L"ኀደረ"),tsc(L"ኀሠሠ")},.d_t_fmt=tsc(L"%A፥%B፡%e፡መዓልት፡%Y፡%r፡%Z"),.d_fmt=tsc(L"%d//%m//%Y"),.t_fmt=tsc(L"%l:%M:%S፡%p"),.t_fmt_ampm=tsc(L"%l:%M:%S፡%p"),.date_fmt=tsc(L"%A፥%B፡%e፡መዓልት፡%r፡%Z፡%Y፡ዓ//ም"),.am_pm={tsc(L"ጽባሕ"),tsc(L"ምሴት")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(L"^[+1yYእ]"),.noexpr=tsc(L"^[-0nNነ]"),.yesstr=tsc(L"እወ"),.nostr=tsc(L"ነኖእ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c %a %l"),.tel_dom_fmt=tsc(L"%a-%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"291")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f"),.name_gen=tsc(L""),.name_miss=tsc(L"ወ//ሪት"),.name_mr={tsc(L"<U12A3><U1276>"),tsc(L"<U12A0><U12ED><U1270>")},.name_mrs=tsc(L"ወ//ሮ"),.name_ms=tsc(L"ወ//ሪት")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"ኤርትራ"),.country_post=tsc(L"ERI"),.country_ab2=tsc(L"ER"),.country_ab3=tsc(L"ERI"),.country_num=232,.country_car=tsc(L"ER"),.lang_name=tsc(L"ግዕዝ"),.lang_term=tsc(L"gez")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Ge'ez language locale for Eritrea With Abegede Collation."),.source=tsc(u8"Ge'ez Frontier Foundation\t\t;\t\tfast_io"),.address=tsc(u8"7802 Solomon Seal Dr., Springfield, VA 22152, USA\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"locales@geez.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Geez"),.territory=tsc(u8"Eritrea"),.revision=tsc(u8"0.20"),.date=tsc(u8"2003-07-05")},.monetary={.int_curr_symbol=tsc(u8"ERN "),.currency_symbol=tsc(u8"Nfk"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={monetary_mon_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8"")},.time={.abday={tsc(u8"እኁድ"),tsc(u8"ሰኑይ"),tsc(u8"ሠሉስ"),tsc(u8"ራብዕ"),tsc(u8"ሐሙስ"),tsc(u8"ዓርበ"),tsc(u8"ቀዳሚ")},.day={tsc(u8"እኁድ"),tsc(u8"ሰኑይ"),tsc(u8"ሠሉስ"),tsc(u8"ራብዕ"),tsc(u8"ሐሙስ"),tsc(u8"ዓርበ"),tsc(u8"ቀዳሚት")},.abmon={tsc(u8"ጠሐረ"),tsc(u8"ከተተ"),tsc(u8"መገበ"),tsc(u8"አኀዘ"),tsc(u8"ግንባ"),tsc(u8"ሠንየ"),tsc(u8"ሐመለ"),tsc(u8"ነሐሰ"),tsc(u8"ከረመ"),tsc(u8"ጠቀመ"),tsc(u8"ኀደረ"),tsc(u8"ኀሠሠ")},.mon={tsc(u8"ጠሐረ"),tsc(u8"ከተተ"),tsc(u8"መገበ"),tsc(u8"አኀዘ"),tsc(u8"ግንባት"),tsc(u8"ሠንየ"),tsc(u8"ሐመለ"),tsc(u8"ነሐሰ"),tsc(u8"ከረመ"),tsc(u8"ጠቀመ"),tsc(u8"ኀደረ"),tsc(u8"ኀሠሠ")},.d_t_fmt=tsc(u8"%A፥%B፡%e፡መዓልት፡%Y፡%r፡%Z"),.d_fmt=tsc(u8"%d//%m//%Y"),.t_fmt=tsc(u8"%l:%M:%S፡%p"),.t_fmt_ampm=tsc(u8"%l:%M:%S፡%p"),.date_fmt=tsc(u8"%A፥%B፡%e፡መዓልት፡%r፡%Z፡%Y፡ዓ//ም"),.am_pm={tsc(u8"ጽባሕ"),tsc(u8"ምሴት")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u8"^[+1yYእ]"),.noexpr=tsc(u8"^[-0nNነ]"),.yesstr=tsc(u8"እወ"),.nostr=tsc(u8"ነኖእ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c %a %l"),.tel_dom_fmt=tsc(u8"%a-%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"291")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f"),.name_gen=tsc(u8""),.name_miss=tsc(u8"ወ//ሪት"),.name_mr={tsc(u8"<U12A3><U1276>"),tsc(u8"<U12A0><U12ED><U1270>")},.name_mrs=tsc(u8"ወ//ሮ"),.name_ms=tsc(u8"ወ//ሪት")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"ኤርትራ"),.country_post=tsc(u8"ERI"),.country_ab2=tsc(u8"ER"),.country_ab3=tsc(u8"ERI"),.country_num=232,.country_car=tsc(u8"ER"),.lang_name=tsc(u8"ግዕዝ"),.lang_term=tsc(u8"gez")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Ge'ez language locale for Eritrea With Abegede Collation."),.source=tsc(u"Ge'ez Frontier Foundation\t\t;\t\tfast_io"),.address=tsc(u"7802 Solomon Seal Dr., Springfield, VA 22152, USA\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"locales@geez.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Geez"),.territory=tsc(u"Eritrea"),.revision=tsc(u"0.20"),.date=tsc(u"2003-07-05")},.monetary={.int_curr_symbol=tsc(u"ERN "),.currency_symbol=tsc(u"Nfk"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={monetary_mon_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u"")},.time={.abday={tsc(u"እኁድ"),tsc(u"ሰኑይ"),tsc(u"ሠሉስ"),tsc(u"ራብዕ"),tsc(u"ሐሙስ"),tsc(u"ዓርበ"),tsc(u"ቀዳሚ")},.day={tsc(u"እኁድ"),tsc(u"ሰኑይ"),tsc(u"ሠሉስ"),tsc(u"ራብዕ"),tsc(u"ሐሙስ"),tsc(u"ዓርበ"),tsc(u"ቀዳሚት")},.abmon={tsc(u"ጠሐረ"),tsc(u"ከተተ"),tsc(u"መገበ"),tsc(u"አኀዘ"),tsc(u"ግንባ"),tsc(u"ሠንየ"),tsc(u"ሐመለ"),tsc(u"ነሐሰ"),tsc(u"ከረመ"),tsc(u"ጠቀመ"),tsc(u"ኀደረ"),tsc(u"ኀሠሠ")},.mon={tsc(u"ጠሐረ"),tsc(u"ከተተ"),tsc(u"መገበ"),tsc(u"አኀዘ"),tsc(u"ግንባት"),tsc(u"ሠንየ"),tsc(u"ሐመለ"),tsc(u"ነሐሰ"),tsc(u"ከረመ"),tsc(u"ጠቀመ"),tsc(u"ኀደረ"),tsc(u"ኀሠሠ")},.d_t_fmt=tsc(u"%A፥%B፡%e፡መዓልት፡%Y፡%r፡%Z"),.d_fmt=tsc(u"%d//%m//%Y"),.t_fmt=tsc(u"%l:%M:%S፡%p"),.t_fmt_ampm=tsc(u"%l:%M:%S፡%p"),.date_fmt=tsc(u"%A፥%B፡%e፡መዓልት፡%r፡%Z፡%Y፡ዓ//ም"),.am_pm={tsc(u"ጽባሕ"),tsc(u"ምሴት")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(u"^[+1yYእ]"),.noexpr=tsc(u"^[-0nNነ]"),.yesstr=tsc(u"እወ"),.nostr=tsc(u"ነኖእ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c %a %l"),.tel_dom_fmt=tsc(u"%a-%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"291")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f"),.name_gen=tsc(u""),.name_miss=tsc(u"ወ//ሪት"),.name_mr={tsc(u"<U12A3><U1276>"),tsc(u"<U12A0><U12ED><U1270>")},.name_mrs=tsc(u"ወ//ሮ"),.name_ms=tsc(u"ወ//ሪት")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"ኤርትራ"),.country_post=tsc(u"ERI"),.country_ab2=tsc(u"ER"),.country_ab3=tsc(u"ERI"),.country_num=232,.country_car=tsc(u"ER"),.lang_name=tsc(u"ግዕዝ"),.lang_term=tsc(u"gez")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Ge'ez language locale for Eritrea With Abegede Collation."),.source=tsc(U"Ge'ez Frontier Foundation\t\t;\t\tfast_io"),.address=tsc(U"7802 Solomon Seal Dr., Springfield, VA 22152, USA\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"locales@geez.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Geez"),.territory=tsc(U"Eritrea"),.revision=tsc(U"0.20"),.date=tsc(U"2003-07-05")},.monetary={.int_curr_symbol=tsc(U"ERN "),.currency_symbol=tsc(U"Nfk"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={monetary_mon_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=0,.frac_digits=0,.p_cs_precedes=1,.p_sep_by_space=1,.n_cs_precedes=1,.n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U"")},.time={.abday={tsc(U"እኁድ"),tsc(U"ሰኑይ"),tsc(U"ሠሉስ"),tsc(U"ራብዕ"),tsc(U"ሐሙስ"),tsc(U"ዓርበ"),tsc(U"ቀዳሚ")},.day={tsc(U"እኁድ"),tsc(U"ሰኑይ"),tsc(U"ሠሉስ"),tsc(U"ራብዕ"),tsc(U"ሐሙስ"),tsc(U"ዓርበ"),tsc(U"ቀዳሚት")},.abmon={tsc(U"ጠሐረ"),tsc(U"ከተተ"),tsc(U"መገበ"),tsc(U"አኀዘ"),tsc(U"ግንባ"),tsc(U"ሠንየ"),tsc(U"ሐመለ"),tsc(U"ነሐሰ"),tsc(U"ከረመ"),tsc(U"ጠቀመ"),tsc(U"ኀደረ"),tsc(U"ኀሠሠ")},.mon={tsc(U"ጠሐረ"),tsc(U"ከተተ"),tsc(U"መገበ"),tsc(U"አኀዘ"),tsc(U"ግንባት"),tsc(U"ሠንየ"),tsc(U"ሐመለ"),tsc(U"ነሐሰ"),tsc(U"ከረመ"),tsc(U"ጠቀመ"),tsc(U"ኀደረ"),tsc(U"ኀሠሠ")},.d_t_fmt=tsc(U"%A፥%B፡%e፡መዓልት፡%Y፡%r፡%Z"),.d_fmt=tsc(U"%d//%m//%Y"),.t_fmt=tsc(U"%l:%M:%S፡%p"),.t_fmt_ampm=tsc(U"%l:%M:%S፡%p"),.date_fmt=tsc(U"%A፥%B፡%e፡መዓልት፡%r፡%Z፡%Y፡ዓ//ም"),.am_pm={tsc(U"ጽባሕ"),tsc(U"ምሴት")},.week={7,19971130,1},.first_weekday=2},.messages={.yesexpr=tsc(U"^[+1yYእ]"),.noexpr=tsc(U"^[-0nNነ]"),.yesstr=tsc(U"እወ"),.nostr=tsc(U"ነኖእ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c %a %l"),.tel_dom_fmt=tsc(U"%a-%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"291")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f"),.name_gen=tsc(U""),.name_miss=tsc(U"ወ//ሪት"),.name_mr={tsc(U"<U12A3><U1276>"),tsc(U"<U12A0><U12ED><U1270>")},.name_mrs=tsc(U"ወ//ሮ"),.name_ms=tsc(U"ወ//ሪት")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"ኤርትራ"),.country_post=tsc(U"ERI"),.country_ab2=tsc(U"ER"),.country_ab3=tsc(U"ERI"),.country_num=232,.country_car=tsc(U"ER"),.lang_name=tsc(U"ግዕዝ"),.lang_term=tsc(U"gez")},.measurement={.measurement=1}};


}
}

#include"../main.h"