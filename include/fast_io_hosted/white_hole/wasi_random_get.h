#pragma once

namespace fast_io
{
namespace details
{
inline void wasi_random_get_impl(void* ptr,std::size_t sz)
{
	using u8ptr_may_alias
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= std::uint8_t*;
	if constexpr(sizeof(__wasi_size_t)!=sizeof(std::size_t))
	{
		throw_posix_error(EINVAL);
	}
	else
	{
		auto ret{::fast_io::noexcept_call(__wasi_random_get,reinterpret_cast<u8ptr_may_alias>(ptr),static_cast<__wasi_size_t>(sz))};
		if(ret)
			throw_posix_error(ret);
	}
}
}

template<std::integral ch_type>
class basic_wasi_random_get
{
public:
	using char_type = ch_type;
	constexpr void close() noexcept{}
};

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter read(basic_wasi_random_get<char_type>,Iter bg,Iter ed)
{
	::fast_io::details::wasi_random_get_impl(::fast_io::freestanding::to_address(bg),static_cast<std::size_t>(ed-bg)*sizeof(*bg));
	return ed;
}

template<std::integral char_type>
inline constexpr basic_wasi_random_get<char_type> io_value_handle(basic_wasi_random_get<char_type> brg) noexcept
{
	return brg;	
}

using wasi_random_get = basic_wasi_random_get<char>;

template<std::integral char_type>
inline constexpr void require_secure_clear(basic_wasi_random_get<char_type>){}

}