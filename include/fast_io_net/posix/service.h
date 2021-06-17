#pragma once

namespace fast_io
{

struct
#if __has_cpp_attribute(maybe_unused)
[[maybe_unused]]
#endif
posix_empty_network_service{};
//a dummy service to make serivce work with windows at a cross platform level

using native_network_service = posix_empty_network_service;
}
