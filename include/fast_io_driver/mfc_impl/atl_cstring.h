#pragma once

namespace fast_io
{

template<std::integral ch_type,typename traits_type>
inline constexpr basic_io_scatter_t<ch_type> print_alias_define(io_alias_t,::ATL::CStringT<ch_type, traits_type> const& atl_mfccstr) noexcept
{
	return {static_cast<ch_type const*>(atl_mfccstr),static_cast<std::size_t>(atl_mfccstr.GetLength())};
}

}
