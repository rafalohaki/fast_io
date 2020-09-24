#pragma once

namespace fast_io::win32
{

struct __crt_lc_time_data
{
    char*    wday_abbr [ 7];
    char*    wday      [ 7];
    char*    month_abbr[12];
    char*    month     [12];
    char*    ampm      [ 2];
    char*    ww_sdatefmt;
    char*    ww_ldatefmt;
    char*    ww_timefmt;
    int      ww_caltype;
    long     refcount;
    wchar_t* _W_wday_abbr [ 7];
    wchar_t* _W_wday      [ 7];
    wchar_t* _W_month_abbr[12];
    wchar_t* _W_month     [12];
    wchar_t* _W_ampm      [ 2];
    wchar_t* _W_ww_sdatefmt;
    wchar_t* _W_ww_ldatefmt;
    wchar_t* _W_ww_timefmt;
    wchar_t* _W_ww_locale_name;
};
struct __crt_locale_refcount
{
    char*    locale;
    wchar_t* wlocale;
    long*    refcount;
    long*    wrefcount;
};

struct __crt_locale_data
{
#if defined(_MSC_VER) || defined(_UCRT)
    __crt_locale_data_public  _public;
#endif
    long                      refcount;
    unsigned int              lc_collate_cp;
    unsigned int              lc_time_cp;
    int                       lc_clike;
    __crt_locale_refcount     lc_category[6];
    long*                     lconv_intl_refcount;
    long*                     lconv_num_refcount;
    long*                     lconv_mon_refcount;
    struct lconv*             lconv;
    long*                     ctype1_refcount;
    unsigned short*           ctype1;
    unsigned char const*      pclmap;
    unsigned char const*      pcumap;
    __crt_lc_time_data const* lc_time_curr;
    wchar_t*                  locale_name[6];
};

/*

mingw-w64 corecrt???
https://github.com/mingw-w64/mingw-w64/blob/b50da295398cd3777ed4326b9262f095a471b0a2/mingw-w64-headers/crt/corecrt.h

typedef struct threadlocaleinfostruct {
#ifdef _UCRT
  const unsigned short *_locale_pctype;
  int _locale_mb_cur_max;
  unsigned int _locale_lc_codepage;
#else
  int refcount;
  unsigned int lc_codepage;
  unsigned int lc_collate_cp;
  unsigned long lc_handle[6];
  LC_ID lc_id[6];
  struct {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
  } lc_category[6];
  int lc_clike;
  int mb_cur_max;
  int *lconv_intl_refcount;
  int *lconv_num_refcount;
  int *lconv_mon_refcount;
  struct lconv *lconv;
  int *ctype1_refcount;
  unsigned short *ctype1;
  const unsigned short *pctype;
  const unsigned char *pclmap;
  const unsigned char *pcumap;
  struct __lc_time_data *lc_time_curr;
#endif
} threadlocinfo;

*/



}