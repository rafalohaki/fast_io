#pragma once
#include<openssl/bio.h>
#include<openssl/ssl.h>
#include<openssl/err.h>

#if __has_include(<openssl/md2.h>)
#include<openssl/md2.h>
#endif
#include<openssl/md4.h>
#include<openssl/md5.h>
#include<openssl/sha.h>
#include<openssl/crypto.h>
#if __cpp_rtti
#include<typeinfo>
#endif

#include"../fast_io_hosted.h"
#include"../fast_io_legacy_impl/c/impl.h"
#include"openssl_driver/init.h"
#include"openssl_driver/observer.h"
#include"openssl_driver/error.h"
#include"openssl_driver/bio.h"
#if 0
#include"openssl_driver/ossl_lib_context.h"
#include"openssl_driver/context.h"
#include"openssl_driver/ssl.h"
#endif
#include"openssl_driver/sha.h"
#if 0
#include"openssl_driver/evp.h"
#endif