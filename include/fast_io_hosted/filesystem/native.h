#pragma once

#if _BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
#include"posix.h"
#include"posix_at.h"
#include"common.h"
#elif _WIN32
#include"nt.h"
#include"common.h"
#endif