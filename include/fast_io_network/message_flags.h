#pragma once

namespace fast_io
{

enum class message_flags:std::uint32_t
{
confirm=MSG_CONFIRM,
dontroute=MSG_DONTROUTE,
eor=MSG_EOR,
more=MSG_MORE,
nosignal=,
confirm=,
oob=,
};

}