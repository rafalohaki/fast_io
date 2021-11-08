#pragma once

namespace fast_io
{

inline cross_code_cvt_t<::winrt::hstring::value_type> print_alias_define(io_alias_t,::winrt::hstring const& hstr) noexcept
{
	return {{hstr.c_str(),hstr.size()}};
}

}
