#include"../localedef.h"

namespace fast_io_i18n
{
namespace
{

inline constexpr std::size_t numeric_grouping_storage[]{3};

inline constexpr lc_all lc_all_global{.identification={.title=tsc("Cherokee language locale for United States"),.source=tsc("Cherokee Nation\t\t;\t\tfast_io"),.address=tsc("http://cldr.unicode.org/index/process\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc("Joseph Erb\t\t;\t\tfast_io"),.email=tsc("josepherb7@gmail.com;euloanty@live.com"),.tel=tsc(""),.fax=tsc(""),.language=tsc("Cherokee"),.territory=tsc("United States"),.revision=tsc("1.0"),.date=tsc("2010-10-20")},.monetary={.int_curr_symbol=tsc("USD "),.currency_symbol=tsc("$"),.mon_decimal_point=tsc("."),.mon_thousands_sep=tsc(","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(""),.negative_sign=tsc("-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc("."),.thousands_sep=tsc(","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc("ᏆᏍᎬ"),tsc("ᏉᏅᎯ"),tsc("ᏔᎵᏁ"),tsc("ᏦᎢᏁ"),tsc("ᏅᎩᏁ"),tsc("ᏧᎾᎩ"),tsc("ᏈᏕᎾ")},.day={tsc("ᎤᎾᏙᏓᏆᏍᎬ"),tsc("ᎤᎾᏙᏓᏉᏅᎯ"),tsc("ᏔᎵᏁᎢᎦ"),tsc("ᏦᎢᏁᎢᎦ"),tsc("ᏅᎩᏁᎢᎦ"),tsc("ᏧᎾᎩᎶᏍᏗ"),tsc("ᎤᎾᏙᏓᏈᏕᎾ")},.abmon={tsc("ᎤᏃ"),tsc("ᎧᎦ"),tsc("ᎠᏅ"),tsc("ᎧᏬ"),tsc("ᎠᏂ"),tsc("ᏕᎭ"),tsc("ᎫᏰ"),tsc("ᎦᎶ"),tsc("ᏚᎵ"),tsc("ᏚᏂ"),tsc("ᏅᏓ"),tsc("ᎥᏍ")},.mon={tsc("ᎤᏃᎸᏔᏅ"),tsc("ᎧᎦᎵ"),tsc("ᎠᏅᏱ"),tsc("ᎧᏬᏂ"),tsc("ᎠᏂᏍᎬᏘ"),tsc("ᏕᎭᎷᏱ"),tsc("ᎫᏰᏉᏂ"),tsc("ᎦᎶᏂ"),tsc("ᏚᎵᏍᏗ"),tsc("ᏚᏂᏅᏗ"),tsc("ᏅᏓᏕᏆ"),tsc("ᎥᏍᎩᏱ")},.d_t_fmt=tsc("%a %d %b %Y %r"),.d_fmt=tsc("%m//%d//%Y"),.t_fmt=tsc("%r"),.t_fmt_ampm=tsc("%I:%M:%S %p"),.date_fmt=tsc("%a %d %b %Y %r %Z"),.am_pm={tsc("ᏌᎾᎴ"),tsc("ᏒᎯᏱᎢᏗᏢ")},.week={7,19971130,1}},.messages={.yesexpr=tsc("^([+1yY]|ᎥᎥ)"),.noexpr=tsc("^([-0nN]|ᎥᏝ)"),.yesstr=tsc("ᎥᎥ"),.nostr=tsc("ᎥᏝ")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc("+%c (%a) %l"),.tel_dom_fmt=tsc("(%a) %l"),.int_select=tsc("11"),.int_prefix=tsc("1")},.name={.name_fmt=tsc("%d%t%g%t%m%t%f"),.name_miss=tsc("Miss."),.name_mr=tsc("Mr."),.name_mrs=tsc("Mrs."),.name_ms=tsc("Ms.")},.address={.postal_fmt=tsc("%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc("ᏌᏊ ᎢᏳᎾᎵᏍᏔᏅ ᏍᎦᏚᎩ"),.country_post=tsc("USA"),.country_ab2=tsc("US"),.country_ab3=tsc("USA"),.country_num=840,.country_car=tsc("USA"),.country_isbn=tsc("0"),.lang_name=tsc("ᏣᎳᎩ"),.lang_ab=tsc(""),.lang_term=tsc("chr"),.lang_lib=tsc("chr")},.measurement={.measurement=2}};

inline constexpr wlc_all wlc_all_global{.identification={.title=tsc(L"Cherokee language locale for United States"),.source=tsc(L"Cherokee Nation\t\t;\t\tfast_io"),.address=tsc(L"http://cldr.unicode.org/index/process\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(L"Joseph Erb\t\t;\t\tfast_io"),.email=tsc(L"josepherb7@gmail.com;euloanty@live.com"),.tel=tsc(L""),.fax=tsc(L""),.language=tsc(L"Cherokee"),.territory=tsc(L"United States"),.revision=tsc(L"1.0"),.date=tsc(L"2010-10-20")},.monetary={.int_curr_symbol=tsc(L"USD "),.currency_symbol=tsc(L"$"),.mon_decimal_point=tsc(L"."),.mon_thousands_sep=tsc(L","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(L""),.negative_sign=tsc(L"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(L"."),.thousands_sep=tsc(L","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(L"ᏆᏍᎬ"),tsc(L"ᏉᏅᎯ"),tsc(L"ᏔᎵᏁ"),tsc(L"ᏦᎢᏁ"),tsc(L"ᏅᎩᏁ"),tsc(L"ᏧᎾᎩ"),tsc(L"ᏈᏕᎾ")},.day={tsc(L"ᎤᎾᏙᏓᏆᏍᎬ"),tsc(L"ᎤᎾᏙᏓᏉᏅᎯ"),tsc(L"ᏔᎵᏁᎢᎦ"),tsc(L"ᏦᎢᏁᎢᎦ"),tsc(L"ᏅᎩᏁᎢᎦ"),tsc(L"ᏧᎾᎩᎶᏍᏗ"),tsc(L"ᎤᎾᏙᏓᏈᏕᎾ")},.abmon={tsc(L"ᎤᏃ"),tsc(L"ᎧᎦ"),tsc(L"ᎠᏅ"),tsc(L"ᎧᏬ"),tsc(L"ᎠᏂ"),tsc(L"ᏕᎭ"),tsc(L"ᎫᏰ"),tsc(L"ᎦᎶ"),tsc(L"ᏚᎵ"),tsc(L"ᏚᏂ"),tsc(L"ᏅᏓ"),tsc(L"ᎥᏍ")},.mon={tsc(L"ᎤᏃᎸᏔᏅ"),tsc(L"ᎧᎦᎵ"),tsc(L"ᎠᏅᏱ"),tsc(L"ᎧᏬᏂ"),tsc(L"ᎠᏂᏍᎬᏘ"),tsc(L"ᏕᎭᎷᏱ"),tsc(L"ᎫᏰᏉᏂ"),tsc(L"ᎦᎶᏂ"),tsc(L"ᏚᎵᏍᏗ"),tsc(L"ᏚᏂᏅᏗ"),tsc(L"ᏅᏓᏕᏆ"),tsc(L"ᎥᏍᎩᏱ")},.d_t_fmt=tsc(L"%a %d %b %Y %r"),.d_fmt=tsc(L"%m//%d//%Y"),.t_fmt=tsc(L"%r"),.t_fmt_ampm=tsc(L"%I:%M:%S %p"),.date_fmt=tsc(L"%a %d %b %Y %r %Z"),.am_pm={tsc(L"ᏌᎾᎴ"),tsc(L"ᏒᎯᏱᎢᏗᏢ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(L"^([+1yY]|ᎥᎥ)"),.noexpr=tsc(L"^([-0nN]|ᎥᏝ)"),.yesstr=tsc(L"ᎥᎥ"),.nostr=tsc(L"ᎥᏝ")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(L"+%c (%a) %l"),.tel_dom_fmt=tsc(L"(%a) %l"),.int_select=tsc(L"11"),.int_prefix=tsc(L"1")},.name={.name_fmt=tsc(L"%d%t%g%t%m%t%f"),.name_miss=tsc(L"Miss."),.name_mr=tsc(L"Mr."),.name_mrs=tsc(L"Mrs."),.name_ms=tsc(L"Ms.")},.address={.postal_fmt=tsc(L"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(L"ᏌᏊ ᎢᏳᎾᎵᏍᏔᏅ ᏍᎦᏚᎩ"),.country_post=tsc(L"USA"),.country_ab2=tsc(L"US"),.country_ab3=tsc(L"USA"),.country_num=840,.country_car=tsc(L"USA"),.country_isbn=tsc(L"0"),.lang_name=tsc(L"ᏣᎳᎩ"),.lang_ab=tsc(L""),.lang_term=tsc(L"chr"),.lang_lib=tsc(L"chr")},.measurement={.measurement=2}};

inline constexpr u8lc_all u8lc_all_global{.identification={.title=tsc(u8"Cherokee language locale for United States"),.source=tsc(u8"Cherokee Nation\t\t;\t\tfast_io"),.address=tsc(u8"http://cldr.unicode.org/index/process\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u8"Joseph Erb\t\t;\t\tfast_io"),.email=tsc(u8"josepherb7@gmail.com;euloanty@live.com"),.tel=tsc(u8""),.fax=tsc(u8""),.language=tsc(u8"Cherokee"),.territory=tsc(u8"United States"),.revision=tsc(u8"1.0"),.date=tsc(u8"2010-10-20")},.monetary={.int_curr_symbol=tsc(u8"USD "),.currency_symbol=tsc(u8"$"),.mon_decimal_point=tsc(u8"."),.mon_thousands_sep=tsc(u8","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u8""),.negative_sign=tsc(u8"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u8"."),.thousands_sep=tsc(u8","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u8"ᏆᏍᎬ"),tsc(u8"ᏉᏅᎯ"),tsc(u8"ᏔᎵᏁ"),tsc(u8"ᏦᎢᏁ"),tsc(u8"ᏅᎩᏁ"),tsc(u8"ᏧᎾᎩ"),tsc(u8"ᏈᏕᎾ")},.day={tsc(u8"ᎤᎾᏙᏓᏆᏍᎬ"),tsc(u8"ᎤᎾᏙᏓᏉᏅᎯ"),tsc(u8"ᏔᎵᏁᎢᎦ"),tsc(u8"ᏦᎢᏁᎢᎦ"),tsc(u8"ᏅᎩᏁᎢᎦ"),tsc(u8"ᏧᎾᎩᎶᏍᏗ"),tsc(u8"ᎤᎾᏙᏓᏈᏕᎾ")},.abmon={tsc(u8"ᎤᏃ"),tsc(u8"ᎧᎦ"),tsc(u8"ᎠᏅ"),tsc(u8"ᎧᏬ"),tsc(u8"ᎠᏂ"),tsc(u8"ᏕᎭ"),tsc(u8"ᎫᏰ"),tsc(u8"ᎦᎶ"),tsc(u8"ᏚᎵ"),tsc(u8"ᏚᏂ"),tsc(u8"ᏅᏓ"),tsc(u8"ᎥᏍ")},.mon={tsc(u8"ᎤᏃᎸᏔᏅ"),tsc(u8"ᎧᎦᎵ"),tsc(u8"ᎠᏅᏱ"),tsc(u8"ᎧᏬᏂ"),tsc(u8"ᎠᏂᏍᎬᏘ"),tsc(u8"ᏕᎭᎷᏱ"),tsc(u8"ᎫᏰᏉᏂ"),tsc(u8"ᎦᎶᏂ"),tsc(u8"ᏚᎵᏍᏗ"),tsc(u8"ᏚᏂᏅᏗ"),tsc(u8"ᏅᏓᏕᏆ"),tsc(u8"ᎥᏍᎩᏱ")},.d_t_fmt=tsc(u8"%a %d %b %Y %r"),.d_fmt=tsc(u8"%m//%d//%Y"),.t_fmt=tsc(u8"%r"),.t_fmt_ampm=tsc(u8"%I:%M:%S %p"),.date_fmt=tsc(u8"%a %d %b %Y %r %Z"),.am_pm={tsc(u8"ᏌᎾᎴ"),tsc(u8"ᏒᎯᏱᎢᏗᏢ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u8"^([+1yY]|ᎥᎥ)"),.noexpr=tsc(u8"^([-0nN]|ᎥᏝ)"),.yesstr=tsc(u8"ᎥᎥ"),.nostr=tsc(u8"ᎥᏝ")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u8"+%c (%a) %l"),.tel_dom_fmt=tsc(u8"(%a) %l"),.int_select=tsc(u8"11"),.int_prefix=tsc(u8"1")},.name={.name_fmt=tsc(u8"%d%t%g%t%m%t%f"),.name_miss=tsc(u8"Miss."),.name_mr=tsc(u8"Mr."),.name_mrs=tsc(u8"Mrs."),.name_ms=tsc(u8"Ms.")},.address={.postal_fmt=tsc(u8"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(u8"ᏌᏊ ᎢᏳᎾᎵᏍᏔᏅ ᏍᎦᏚᎩ"),.country_post=tsc(u8"USA"),.country_ab2=tsc(u8"US"),.country_ab3=tsc(u8"USA"),.country_num=840,.country_car=tsc(u8"USA"),.country_isbn=tsc(u8"0"),.lang_name=tsc(u8"ᏣᎳᎩ"),.lang_ab=tsc(u8""),.lang_term=tsc(u8"chr"),.lang_lib=tsc(u8"chr")},.measurement={.measurement=2}};

inline constexpr u16lc_all u16lc_all_global{.identification={.title=tsc(u"Cherokee language locale for United States"),.source=tsc(u"Cherokee Nation\t\t;\t\tfast_io"),.address=tsc(u"http://cldr.unicode.org/index/process\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(u"Joseph Erb\t\t;\t\tfast_io"),.email=tsc(u"josepherb7@gmail.com;euloanty@live.com"),.tel=tsc(u""),.fax=tsc(u""),.language=tsc(u"Cherokee"),.territory=tsc(u"United States"),.revision=tsc(u"1.0"),.date=tsc(u"2010-10-20")},.monetary={.int_curr_symbol=tsc(u"USD "),.currency_symbol=tsc(u"$"),.mon_decimal_point=tsc(u"."),.mon_thousands_sep=tsc(u","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(u""),.negative_sign=tsc(u"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(u"."),.thousands_sep=tsc(u","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(u"ᏆᏍᎬ"),tsc(u"ᏉᏅᎯ"),tsc(u"ᏔᎵᏁ"),tsc(u"ᏦᎢᏁ"),tsc(u"ᏅᎩᏁ"),tsc(u"ᏧᎾᎩ"),tsc(u"ᏈᏕᎾ")},.day={tsc(u"ᎤᎾᏙᏓᏆᏍᎬ"),tsc(u"ᎤᎾᏙᏓᏉᏅᎯ"),tsc(u"ᏔᎵᏁᎢᎦ"),tsc(u"ᏦᎢᏁᎢᎦ"),tsc(u"ᏅᎩᏁᎢᎦ"),tsc(u"ᏧᎾᎩᎶᏍᏗ"),tsc(u"ᎤᎾᏙᏓᏈᏕᎾ")},.abmon={tsc(u"ᎤᏃ"),tsc(u"ᎧᎦ"),tsc(u"ᎠᏅ"),tsc(u"ᎧᏬ"),tsc(u"ᎠᏂ"),tsc(u"ᏕᎭ"),tsc(u"ᎫᏰ"),tsc(u"ᎦᎶ"),tsc(u"ᏚᎵ"),tsc(u"ᏚᏂ"),tsc(u"ᏅᏓ"),tsc(u"ᎥᏍ")},.mon={tsc(u"ᎤᏃᎸᏔᏅ"),tsc(u"ᎧᎦᎵ"),tsc(u"ᎠᏅᏱ"),tsc(u"ᎧᏬᏂ"),tsc(u"ᎠᏂᏍᎬᏘ"),tsc(u"ᏕᎭᎷᏱ"),tsc(u"ᎫᏰᏉᏂ"),tsc(u"ᎦᎶᏂ"),tsc(u"ᏚᎵᏍᏗ"),tsc(u"ᏚᏂᏅᏗ"),tsc(u"ᏅᏓᏕᏆ"),tsc(u"ᎥᏍᎩᏱ")},.d_t_fmt=tsc(u"%a %d %b %Y %r"),.d_fmt=tsc(u"%m//%d//%Y"),.t_fmt=tsc(u"%r"),.t_fmt_ampm=tsc(u"%I:%M:%S %p"),.date_fmt=tsc(u"%a %d %b %Y %r %Z"),.am_pm={tsc(u"ᏌᎾᎴ"),tsc(u"ᏒᎯᏱᎢᏗᏢ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(u"^([+1yY]|ᎥᎥ)"),.noexpr=tsc(u"^([-0nN]|ᎥᏝ)"),.yesstr=tsc(u"ᎥᎥ"),.nostr=tsc(u"ᎥᏝ")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(u"+%c (%a) %l"),.tel_dom_fmt=tsc(u"(%a) %l"),.int_select=tsc(u"11"),.int_prefix=tsc(u"1")},.name={.name_fmt=tsc(u"%d%t%g%t%m%t%f"),.name_miss=tsc(u"Miss."),.name_mr=tsc(u"Mr."),.name_mrs=tsc(u"Mrs."),.name_ms=tsc(u"Ms.")},.address={.postal_fmt=tsc(u"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(u"ᏌᏊ ᎢᏳᎾᎵᏍᏔᏅ ᏍᎦᏚᎩ"),.country_post=tsc(u"USA"),.country_ab2=tsc(u"US"),.country_ab3=tsc(u"USA"),.country_num=840,.country_car=tsc(u"USA"),.country_isbn=tsc(u"0"),.lang_name=tsc(u"ᏣᎳᎩ"),.lang_ab=tsc(u""),.lang_term=tsc(u"chr"),.lang_lib=tsc(u"chr")},.measurement={.measurement=2}};

inline constexpr u32lc_all u32lc_all_global{.identification={.title=tsc(U"Cherokee language locale for United States"),.source=tsc(U"Cherokee Nation\t\t;\t\tfast_io"),.address=tsc(U"http://cldr.unicode.org/index/process\t\t;\t\thttps://gitee.com/qabeowjbtkwb/fast_io"),.contact=tsc(U"Joseph Erb\t\t;\t\tfast_io"),.email=tsc(U"josepherb7@gmail.com;euloanty@live.com"),.tel=tsc(U""),.fax=tsc(U""),.language=tsc(U"Cherokee"),.territory=tsc(U"United States"),.revision=tsc(U"1.0"),.date=tsc(U"2010-10-20")},.monetary={.int_curr_symbol=tsc(U"USD "),.currency_symbol=tsc(U"$"),.mon_decimal_point=tsc(U"."),.mon_thousands_sep=tsc(U","),.mon_grouping={numeric_grouping_storage,1},.positive_sign=tsc(U""),.negative_sign=tsc(U"-"),.int_frac_digits=2,.frac_digits=2,.p_cs_precedes=1,.p_sep_by_space=0,.n_cs_precedes=1,.n_sep_by_space=0,.int_p_sep_by_space=1,.int_n_sep_by_space=1,.p_sign_posn=1,.n_sign_posn=1},.numeric={.decimal_point=tsc(U"."),.thousands_sep=tsc(U","),.grouping={numeric_grouping_storage,1}},.time={.abday={tsc(U"ᏆᏍᎬ"),tsc(U"ᏉᏅᎯ"),tsc(U"ᏔᎵᏁ"),tsc(U"ᏦᎢᏁ"),tsc(U"ᏅᎩᏁ"),tsc(U"ᏧᎾᎩ"),tsc(U"ᏈᏕᎾ")},.day={tsc(U"ᎤᎾᏙᏓᏆᏍᎬ"),tsc(U"ᎤᎾᏙᏓᏉᏅᎯ"),tsc(U"ᏔᎵᏁᎢᎦ"),tsc(U"ᏦᎢᏁᎢᎦ"),tsc(U"ᏅᎩᏁᎢᎦ"),tsc(U"ᏧᎾᎩᎶᏍᏗ"),tsc(U"ᎤᎾᏙᏓᏈᏕᎾ")},.abmon={tsc(U"ᎤᏃ"),tsc(U"ᎧᎦ"),tsc(U"ᎠᏅ"),tsc(U"ᎧᏬ"),tsc(U"ᎠᏂ"),tsc(U"ᏕᎭ"),tsc(U"ᎫᏰ"),tsc(U"ᎦᎶ"),tsc(U"ᏚᎵ"),tsc(U"ᏚᏂ"),tsc(U"ᏅᏓ"),tsc(U"ᎥᏍ")},.mon={tsc(U"ᎤᏃᎸᏔᏅ"),tsc(U"ᎧᎦᎵ"),tsc(U"ᎠᏅᏱ"),tsc(U"ᎧᏬᏂ"),tsc(U"ᎠᏂᏍᎬᏘ"),tsc(U"ᏕᎭᎷᏱ"),tsc(U"ᎫᏰᏉᏂ"),tsc(U"ᎦᎶᏂ"),tsc(U"ᏚᎵᏍᏗ"),tsc(U"ᏚᏂᏅᏗ"),tsc(U"ᏅᏓᏕᏆ"),tsc(U"ᎥᏍᎩᏱ")},.d_t_fmt=tsc(U"%a %d %b %Y %r"),.d_fmt=tsc(U"%m//%d//%Y"),.t_fmt=tsc(U"%r"),.t_fmt_ampm=tsc(U"%I:%M:%S %p"),.date_fmt=tsc(U"%a %d %b %Y %r %Z"),.am_pm={tsc(U"ᏌᎾᎴ"),tsc(U"ᏒᎯᏱᎢᏗᏢ")},.week={7,19971130,1}},.messages={.yesexpr=tsc(U"^([+1yY]|ᎥᎥ)"),.noexpr=tsc(U"^([-0nN]|ᎥᏝ)"),.yesstr=tsc(U"ᎥᎥ"),.nostr=tsc(U"ᎥᏝ")},.paper={.width=216,.height=279},.telephone={.tel_int_fmt=tsc(U"+%c (%a) %l"),.tel_dom_fmt=tsc(U"(%a) %l"),.int_select=tsc(U"11"),.int_prefix=tsc(U"1")},.name={.name_fmt=tsc(U"%d%t%g%t%m%t%f"),.name_miss=tsc(U"Miss."),.name_mr=tsc(U"Mr."),.name_mrs=tsc(U"Mrs."),.name_ms=tsc(U"Ms.")},.address={.postal_fmt=tsc(U"%a%N%f%N%d%N%b%N%h %s %e %r%N%T, %S %z%N%c%N"),.country_name=tsc(U"ᏌᏊ ᎢᏳᎾᎵᏍᏔᏅ ᏍᎦᏚᎩ"),.country_post=tsc(U"USA"),.country_ab2=tsc(U"US"),.country_ab3=tsc(U"USA"),.country_num=840,.country_car=tsc(U"USA"),.country_isbn=tsc(U"0"),.lang_name=tsc(U"ᏣᎳᎩ"),.lang_ab=tsc(U""),.lang_term=tsc(U"chr"),.lang_lib=tsc(U"chr")},.measurement={.measurement=2}};


}
}

#include"../main.h"