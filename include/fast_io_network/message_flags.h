#pragma once

namespace fast_io
{

enum class message_flag: std::uint32_t
{
oob = MSG_OOB,
peek = MSG_PEEK,
dontroute = MSG_DONTROUTE
};

constexpr message_flag operator&(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr message_flag operator|(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr message_flag operator^(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr message_flag operator~(message_flag x) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(~static_cast<utype>(x));
}

inline constexpr message_flag& operator&=(message_flag& x, message_flag y) noexcept{return x=x&y;}

inline constexpr message_flag& operator|=(message_flag& x, message_flag y) noexcept{return x=x|y;}

inline constexpr message_flag& operator^=(message_flag& x, message_flag y) noexcept{return x=x^y;}


}