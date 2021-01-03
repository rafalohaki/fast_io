#pragma once

namespace fast_io
{
/*
Allow us to change this in the future without breaking ABI if uintmax_t becomes uint128_t in the future.
*/
using uintiso_t = std::uintmax_t;
using intiso_t = std::intmax_t;
}

#include"iso3166.h"
#include"iso6709.h"
#include"iso8601.h"