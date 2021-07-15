#pragma once
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if defined(__GNUC__) && __GNUC__>=11 && __cplusplus<202002L
#error "fast_io requires at least C++20 standard compiler."
#else
#include"fast_io_hosted.h"
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
#include"fast_io_i18n/locale.h"
#include"fast_io_i18n/imbuer.h"
#include"fast_io_i18n/lc_print_status.h"
#if 0
#include"fast_io_i18n/lc_unsafe_fprint_status.h"
#endif
#include"fast_io_i18n/lc_numbers/impl.h"
#endif

#endif