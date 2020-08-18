#pragma once

#include"hash_processor.h"
#include"jenkins.h"
#include"crc.h"
#include"md5.h"
#include"sha.h"
namespace fast_io
{

using md5
[[deprecated("The weaknesses of MD5 have been exploited in the field, most infamously by the Flame malware in 2012. See wikipedia https://en.wikipedia.org/wiki/MD5")]]
= sha<md5_function,false>;
}
#include"hmac.h"
#if defined(__GNUC__)
#include"poly1305.h"
#endif