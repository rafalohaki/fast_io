#pragma once
#include<openssl/bio.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<openssl/sha.h>
#include<openssl/crypto.h>
#include<typeinfo>

#include"../fast_io_hosted.h"
#include"../fast_io_legacy_impl/c/impl.h"
#include"openssl_driver/init.h"
#include"openssl_driver/observer.h"
#include"openssl_driver/error.h"
#include"openssl_driver/bio.h"
#include"openssl_driver/ossl_lib_context.h"
#include"openssl_driver/context.h"
#include"openssl_driver/ssl.h"
#include"openssl_driver/sha.h"
#include"openssl_driver/evp.h"