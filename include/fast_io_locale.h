#pragma once
#include"fast_io_freestanding.h"
#include<locale.h>
#include"fast_io_locale/c_locale.h"
#ifdef _WIN32
#include"fast_io_locale/ucrt.h"
#endif
#include"fast_io_locale/c_locale_thread_locale_guard.h"
#include"fast_io_locale/lconv_storage.h"
#include"fast_io_locale/c_lconv_storage.h"
#include"fast_io_locale/ctype_storage.h"
#include"fast_io_locale/c_ctype_storage.h"
#include"fast_io_locale/lcv.h"
#include"fast_io_locale/c_locale_wrapper.h"