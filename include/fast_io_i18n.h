#pragma once
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if __cplusplus<202002L && !defined(_MSC_VER)
#error "fast_io requires at least C++20 standard compiler."
#else
#include"fast_io_hosted.h"
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
#include"fast_io_i18n/locale.h"
#include"fast_io_i18n/imbuer.h"
#include"fast_io_i18n/lc_print_status.h"
#include"fast_io_i18n/lc_unsafe_fprint_status.h"
#include"fast_io_i18n/lc_numbers/impl.h"
#endif

#endif