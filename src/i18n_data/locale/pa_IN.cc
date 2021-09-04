#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t monetary_mon_grouping_storage[]{3,2};

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Punjabi language locale for Indian Punjabi(Gurmukhi)"),.source=tsc("IndLinux.org\t\t;\t\tfast_io"),.address=tsc("https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("fast_io"),.email=tsc("bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Punjabi"),.territory=tsc("India"),.revision=tsc("0.2"),.date=tsc("2004-09-30")},.monetary={.int_curr_symbol=tsc("INR "),.currency_symbol=tsc("₹"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("ਐਤ"),tsc("ਸੋਮ"),tsc("ਮੰਗਲ"),tsc("ਬੁੱਧ"),tsc("ਵੀਰ"),tsc("ਸ਼ੁੱਕਰ"),tsc("ਸ਼ਨਿੱਚਰ")},.day={tsc("ਐਤਵਾਰ"),tsc("ਸੋਮਵਾਰ"),tsc("ਮੰਗਲਵਾਰ"),tsc("ਬੁੱਧਵਾਰ"),tsc("ਵੀਰਵਾਰ"),tsc("ਸ਼ੁੱਕਰਵਾਰ"),tsc("ਸ਼ਨਿੱਚਰਵਾਰ")},.abmon={tsc("ਜਨ"),tsc("ਫ਼ਰ"),tsc("ਮਾਰਚ"),tsc("ਅਪ੍ਰੈ"),tsc("ਮਈ"),tsc("ਜੂਨ"),tsc("ਜੁਲਾ"),tsc("ਅਗ"),tsc("ਸਤੰ"),tsc("ਅਕਤੂ"),tsc("ਨਵੰ"),tsc("ਦਸੰ")},.mon={tsc("ਜਨਵਰੀ"),tsc("ਫ਼ਰਵਰੀ"),tsc("ਮਾਰਚ"),tsc("ਅਪ੍ਰੈਲ"),tsc("ਮਈ"),tsc("ਜੂਨ"),tsc("ਜੁਲਾਈ"),tsc("ਅਗਸਤ"),tsc("ਸਤੰਬਰ"),tsc("ਅਕਤੂਬਰ"),tsc("ਨਵੰਬਰ"),tsc("ਦਸੰਬਰ")},.d_t_fmt=tsc("%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc("%-d//%-m//%y"),.t_fmt=tsc("%I:%M:%S %p %Z"),.t_fmt_ampm=tsc("%I:%M:%S %p %Z"),.date_fmt=tsc("%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc("ਸਵੇਰੇ"),tsc("ਸ਼ਾਮ")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^[+1yYਹ]"),.noexpr=tsc("^[-0nNਨ]"),.yesstr=tsc("ਹਾਂ"),.nostr=tsc("ਨਹੀਂ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc("+%c ;%a ;%l"),.int_select=tsc("00"),.int_prefix=tsc("91")},.name={.name_fmt=tsc("%p%t%f%t%g"),.name_gen=tsc(""),.name_miss=tsc("Miss."),.name_mr=tsc("Mr."),.name_mrs=tsc("Mrs."),.name_ms=tsc("Ms.")},.address={.postal_fmt=tsc("%z%c%T%s%b%e%r"),.country_name=tsc("ਭਾਰਤ"),.country_ab2=tsc("IN"),.country_ab3=tsc("IND"),.country_num=356,.country_car=tsc("IND"),.lang_name=tsc("ਪੰਜਾਬੀ"),.lang_ab=tsc("pa"),.lang_term=tsc("pan"),.lang_lib=tsc("pan")},.measurement={.measurement=1}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Punjabi language locale for Indian Punjabi(Gurmukhi)"),.source=tsc(L"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(L"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"fast_io"),.email=tsc(L"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Punjabi"),.territory=tsc(L"India"),.revision=tsc(L"0.2"),.date=tsc(L"2004-09-30")},.monetary={.int_curr_symbol=tsc(L"INR "),.currency_symbol=tsc(L"₹"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"ਐਤ"),tsc(L"ਸੋਮ"),tsc(L"ਮੰਗਲ"),tsc(L"ਬੁੱਧ"),tsc(L"ਵੀਰ"),tsc(L"ਸ਼ੁੱਕਰ"),tsc(L"ਸ਼ਨਿੱਚਰ")},.day={tsc(L"ਐਤਵਾਰ"),tsc(L"ਸੋਮਵਾਰ"),tsc(L"ਮੰਗਲਵਾਰ"),tsc(L"ਬੁੱਧਵਾਰ"),tsc(L"ਵੀਰਵਾਰ"),tsc(L"ਸ਼ੁੱਕਰਵਾਰ"),tsc(L"ਸ਼ਨਿੱਚਰਵਾਰ")},.abmon={tsc(L"ਜਨ"),tsc(L"ਫ਼ਰ"),tsc(L"ਮਾਰਚ"),tsc(L"ਅਪ੍ਰੈ"),tsc(L"ਮਈ"),tsc(L"ਜੂਨ"),tsc(L"ਜੁਲਾ"),tsc(L"ਅਗ"),tsc(L"ਸਤੰ"),tsc(L"ਅਕਤੂ"),tsc(L"ਨਵੰ"),tsc(L"ਦਸੰ")},.mon={tsc(L"ਜਨਵਰੀ"),tsc(L"ਫ਼ਰਵਰੀ"),tsc(L"ਮਾਰਚ"),tsc(L"ਅਪ੍ਰੈਲ"),tsc(L"ਮਈ"),tsc(L"ਜੂਨ"),tsc(L"ਜੁਲਾਈ"),tsc(L"ਅਗਸਤ"),tsc(L"ਸਤੰਬਰ"),tsc(L"ਅਕਤੂਬਰ"),tsc(L"ਨਵੰਬਰ"),tsc(L"ਦਸੰਬਰ")},.d_t_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(L"%-d//%-m//%y"),.t_fmt=tsc(L"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(L"%I:%M:%S %p %Z"),.date_fmt=tsc(L"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(L"ਸਵੇਰੇ"),tsc(L"ਸ਼ਾਮ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^[+1yYਹ]"),.noexpr=tsc(L"^[-0nNਨ]"),.yesstr=tsc(L"ਹਾਂ"),.nostr=tsc(L"ਨਹੀਂ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(L"+%c ;%a ;%l"),.int_select=tsc(L"00"),.int_prefix=tsc(L"91")},.name={.name_fmt=tsc(L"%p%t%f%t%g"),.name_gen=tsc(L""),.name_miss=tsc(L"Miss."),.name_mr=tsc(L"Mr."),.name_mrs=tsc(L"Mrs."),.name_ms=tsc(L"Ms.")},.address={.postal_fmt=tsc(L"%z%c%T%s%b%e%r"),.country_name=tsc(L"ਭਾਰਤ"),.country_ab2=tsc(L"IN"),.country_ab3=tsc(L"IND"),.country_num=356,.country_car=tsc(L"IND"),.lang_name=tsc(L"ਪੰਜਾਬੀ"),.lang_ab=tsc(L"pa"),.lang_term=tsc(L"pan"),.lang_lib=tsc(L"pan")},.measurement={.measurement=1}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Punjabi language locale for Indian Punjabi(Gurmukhi)"),.source=tsc(u8"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(u8"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"fast_io"),.email=tsc(u8"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Punjabi"),.territory=tsc(u8"India"),.revision=tsc(u8"0.2"),.date=tsc(u8"2004-09-30")},.monetary={.int_curr_symbol=tsc(u8"INR "),.currency_symbol=tsc(u8"₹"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"ਐਤ"),tsc(u8"ਸੋਮ"),tsc(u8"ਮੰਗਲ"),tsc(u8"ਬੁੱਧ"),tsc(u8"ਵੀਰ"),tsc(u8"ਸ਼ੁੱਕਰ"),tsc(u8"ਸ਼ਨਿੱਚਰ")},.day={tsc(u8"ਐਤਵਾਰ"),tsc(u8"ਸੋਮਵਾਰ"),tsc(u8"ਮੰਗਲਵਾਰ"),tsc(u8"ਬੁੱਧਵਾਰ"),tsc(u8"ਵੀਰਵਾਰ"),tsc(u8"ਸ਼ੁੱਕਰਵਾਰ"),tsc(u8"ਸ਼ਨਿੱਚਰਵਾਰ")},.abmon={tsc(u8"ਜਨ"),tsc(u8"ਫ਼ਰ"),tsc(u8"ਮਾਰਚ"),tsc(u8"ਅਪ੍ਰੈ"),tsc(u8"ਮਈ"),tsc(u8"ਜੂਨ"),tsc(u8"ਜੁਲਾ"),tsc(u8"ਅਗ"),tsc(u8"ਸਤੰ"),tsc(u8"ਅਕਤੂ"),tsc(u8"ਨਵੰ"),tsc(u8"ਦਸੰ")},.mon={tsc(u8"ਜਨਵਰੀ"),tsc(u8"ਫ਼ਰਵਰੀ"),tsc(u8"ਮਾਰਚ"),tsc(u8"ਅਪ੍ਰੈਲ"),tsc(u8"ਮਈ"),tsc(u8"ਜੂਨ"),tsc(u8"ਜੁਲਾਈ"),tsc(u8"ਅਗਸਤ"),tsc(u8"ਸਤੰਬਰ"),tsc(u8"ਅਕਤੂਬਰ"),tsc(u8"ਨਵੰਬਰ"),tsc(u8"ਦਸੰਬਰ")},.d_t_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u8"%-d//%-m//%y"),.t_fmt=tsc(u8"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p %Z"),.date_fmt=tsc(u8"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u8"ਸਵੇਰੇ"),tsc(u8"ਸ਼ਾਮ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^[+1yYਹ]"),.noexpr=tsc(u8"^[-0nNਨ]"),.yesstr=tsc(u8"ਹਾਂ"),.nostr=tsc(u8"ਨਹੀਂ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u8"+%c ;%a ;%l"),.int_select=tsc(u8"00"),.int_prefix=tsc(u8"91")},.name={.name_fmt=tsc(u8"%p%t%f%t%g"),.name_gen=tsc(u8""),.name_miss=tsc(u8"Miss."),.name_mr=tsc(u8"Mr."),.name_mrs=tsc(u8"Mrs."),.name_ms=tsc(u8"Ms.")},.address={.postal_fmt=tsc(u8"%z%c%T%s%b%e%r"),.country_name=tsc(u8"ਭਾਰਤ"),.country_ab2=tsc(u8"IN"),.country_ab3=tsc(u8"IND"),.country_num=356,.country_car=tsc(u8"IND"),.lang_name=tsc(u8"ਪੰਜਾਬੀ"),.lang_ab=tsc(u8"pa"),.lang_term=tsc(u8"pan"),.lang_lib=tsc(u8"pan")},.measurement={.measurement=1}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Punjabi language locale for Indian Punjabi(Gurmukhi)"),.source=tsc(u"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(u"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"fast_io"),.email=tsc(u"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Punjabi"),.territory=tsc(u"India"),.revision=tsc(u"0.2"),.date=tsc(u"2004-09-30")},.monetary={.int_curr_symbol=tsc(u"INR "),.currency_symbol=tsc(u"₹"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"ਐਤ"),tsc(u"ਸੋਮ"),tsc(u"ਮੰਗਲ"),tsc(u"ਬੁੱਧ"),tsc(u"ਵੀਰ"),tsc(u"ਸ਼ੁੱਕਰ"),tsc(u"ਸ਼ਨਿੱਚਰ")},.day={tsc(u"ਐਤਵਾਰ"),tsc(u"ਸੋਮਵਾਰ"),tsc(u"ਮੰਗਲਵਾਰ"),tsc(u"ਬੁੱਧਵਾਰ"),tsc(u"ਵੀਰਵਾਰ"),tsc(u"ਸ਼ੁੱਕਰਵਾਰ"),tsc(u"ਸ਼ਨਿੱਚਰਵਾਰ")},.abmon={tsc(u"ਜਨ"),tsc(u"ਫ਼ਰ"),tsc(u"ਮਾਰਚ"),tsc(u"ਅਪ੍ਰੈ"),tsc(u"ਮਈ"),tsc(u"ਜੂਨ"),tsc(u"ਜੁਲਾ"),tsc(u"ਅਗ"),tsc(u"ਸਤੰ"),tsc(u"ਅਕਤੂ"),tsc(u"ਨਵੰ"),tsc(u"ਦਸੰ")},.mon={tsc(u"ਜਨਵਰੀ"),tsc(u"ਫ਼ਰਵਰੀ"),tsc(u"ਮਾਰਚ"),tsc(u"ਅਪ੍ਰੈਲ"),tsc(u"ਮਈ"),tsc(u"ਜੂਨ"),tsc(u"ਜੁਲਾਈ"),tsc(u"ਅਗਸਤ"),tsc(u"ਸਤੰਬਰ"),tsc(u"ਅਕਤੂਬਰ"),tsc(u"ਨਵੰਬਰ"),tsc(u"ਦਸੰਬਰ")},.d_t_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(u"%-d//%-m//%y"),.t_fmt=tsc(u"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(u"%I:%M:%S %p %Z"),.date_fmt=tsc(u"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(u"ਸਵੇਰੇ"),tsc(u"ਸ਼ਾਮ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^[+1yYਹ]"),.noexpr=tsc(u"^[-0nNਨ]"),.yesstr=tsc(u"ਹਾਂ"),.nostr=tsc(u"ਨਹੀਂ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(u"+%c ;%a ;%l"),.int_select=tsc(u"00"),.int_prefix=tsc(u"91")},.name={.name_fmt=tsc(u"%p%t%f%t%g"),.name_gen=tsc(u""),.name_miss=tsc(u"Miss."),.name_mr=tsc(u"Mr."),.name_mrs=tsc(u"Mrs."),.name_ms=tsc(u"Ms.")},.address={.postal_fmt=tsc(u"%z%c%T%s%b%e%r"),.country_name=tsc(u"ਭਾਰਤ"),.country_ab2=tsc(u"IN"),.country_ab3=tsc(u"IND"),.country_num=356,.country_car=tsc(u"IND"),.lang_name=tsc(u"ਪੰਜਾਬੀ"),.lang_ab=tsc(u"pa"),.lang_term=tsc(u"pan"),.lang_lib=tsc(u"pan")},.measurement={.measurement=1}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Punjabi language locale for Indian Punjabi(Gurmukhi)"),.source=tsc(U"IndLinux.org\t\t;\t\tfast_io"),.address=tsc(U"https://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"fast_io"),.email=tsc(U"bug-glibc-locales@gnu.org;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Punjabi"),.territory=tsc(U"India"),.revision=tsc(U"0.2"),.date=tsc(U"2004-09-30")},.monetary={.int_curr_symbol=tsc(U"INR "),.currency_symbol=tsc(U"₹"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={monetary_mon_grouping_storage,2},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"ਐਤ"),tsc(U"ਸੋਮ"),tsc(U"ਮੰਗਲ"),tsc(U"ਬੁੱਧ"),tsc(U"ਵੀਰ"),tsc(U"ਸ਼ੁੱਕਰ"),tsc(U"ਸ਼ਨਿੱਚਰ")},.day={tsc(U"ਐਤਵਾਰ"),tsc(U"ਸੋਮਵਾਰ"),tsc(U"ਮੰਗਲਵਾਰ"),tsc(U"ਬੁੱਧਵਾਰ"),tsc(U"ਵੀਰਵਾਰ"),tsc(U"ਸ਼ੁੱਕਰਵਾਰ"),tsc(U"ਸ਼ਨਿੱਚਰਵਾਰ")},.abmon={tsc(U"ਜਨ"),tsc(U"ਫ਼ਰ"),tsc(U"ਮਾਰਚ"),tsc(U"ਅਪ੍ਰੈ"),tsc(U"ਮਈ"),tsc(U"ਜੂਨ"),tsc(U"ਜੁਲਾ"),tsc(U"ਅਗ"),tsc(U"ਸਤੰ"),tsc(U"ਅਕਤੂ"),tsc(U"ਨਵੰ"),tsc(U"ਦਸੰ")},.mon={tsc(U"ਜਨਵਰੀ"),tsc(U"ਫ਼ਰਵਰੀ"),tsc(U"ਮਾਰਚ"),tsc(U"ਅਪ੍ਰੈਲ"),tsc(U"ਮਈ"),tsc(U"ਜੂਨ"),tsc(U"ਜੁਲਾਈ"),tsc(U"ਅਗਸਤ"),tsc(U"ਸਤੰਬਰ"),tsc(U"ਅਕਤੂਬਰ"),tsc(U"ਨਵੰਬਰ"),tsc(U"ਦਸੰਬਰ")},.d_t_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p"),.d_fmt=tsc(U"%-d//%-m//%y"),.t_fmt=tsc(U"%I:%M:%S %p %Z"),.t_fmt_ampm=tsc(U"%I:%M:%S %p %Z"),.date_fmt=tsc(U"%A %d %b %Y %I:%M:%S %p %Z"),.am_pm={tsc(U"ਸਵੇਰੇ"),tsc(U"ਸ਼ਾਮ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^[+1yYਹ]"),.noexpr=tsc(U"^[-0nNਨ]"),.yesstr=tsc(U"ਹਾਂ"),.nostr=tsc(U"ਨਹੀਂ")},.paper={.width=210,.height=297},.telephone={.tel_int_fmt=tsc(U"+%c ;%a ;%l"),.int_select=tsc(U"00"),.int_prefix=tsc(U"91")},.name={.name_fmt=tsc(U"%p%t%f%t%g"),.name_gen=tsc(U""),.name_miss=tsc(U"Miss."),.name_mr=tsc(U"Mr."),.name_mrs=tsc(U"Mrs."),.name_ms=tsc(U"Ms.")},.address={.postal_fmt=tsc(U"%z%c%T%s%b%e%r"),.country_name=tsc(U"ਭਾਰਤ"),.country_ab2=tsc(U"IN"),.country_ab3=tsc(U"IND"),.country_num=356,.country_car=tsc(U"IND"),.lang_name=tsc(U"ਪੰਜਾਬੀ"),.lang_ab=tsc(U"pa"),.lang_term=tsc(U"pan"),.lang_lib=tsc(U"pan")},.measurement={.measurement=1}};


}
}

#include"../main.h"