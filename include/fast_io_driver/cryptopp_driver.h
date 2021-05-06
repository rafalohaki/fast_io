#pragma once
#include<CryptoPP/sha.h>
namespace fast_io::cryptopp
{

template<typename T>
class iterated_hash_ref
{
public:
	using value_type = T;
	using char_type = char;
	T& reference;
	constexpr iterated_hash_ref(value_type& ref):reference(ref){}
};

namespace details
{

template<typename T>
inline void cryptopp_hash_write_impl(T& ihb,std::byte const* first,std::byte const* last)
{
	ihb.Update(reinterpret_cast<CryptoPP::byte const*>(first),static_cast<std::size_t>(last-first));
}

template<typename T>
inline void cryptopp_scatter_write_impl(T& ihb,io_scatter_t const* scatter_first,std::size_t scatters_len)
{
	for(std::size_t i{};i!=scatters_len;++i)
	{
		auto e{scatter_first[i]};
		cryptopp_hash_write_impl(ihb,e.base,reinterpret_cast<char unsigned const*>(e.base),
			reinterpret_cast<char unsigned const*>(e.base)+e.len);
	}
}

}

template<typename T, ::fast_io::freestanding::contiguous_iterator Iter>
inline void write(iterated_hash_ref<T> ihb,Iter begin,Iter end)
{
	details::cryptopp_hash_write_impl(ihb.reference,
		reinterpret_cast<std::byte const*>(::fast_io::freestanding::to_address(begin)),
		reinterpret_cast<std::byte const*>(::fast_io::freestanding::to_address(end)));
}

template<typename T>
inline void scatter_write(iterated_hash_ref<T> ihb,io_scatters_t sp)
{
	details::cryptopp_scatter_write_impl(ihb.reference,sp.base,sp.len);
}

template<std::size_t N>
class digest_result
{
public:
	using digest_type = ::fast_io::freestanding::array<CryptoPP::byte,N>;
	digest_type digest_block;
	constexpr digest_result()=default;
	constexpr digest_result(digest_result const&) noexcept=default;
	constexpr digest_result& operator=(digest_result const&) noexcept=default;
	~digest_result()
	{
		secure_clear(digest_block.data(),sizeof(digest_type));
	}
};

template<std::integral char_type,std::size_t N>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,digest_result<N>>)
{
	return N*2;
}

template<std::integral char_type,::fast_io::freestanding::forward_iterator caiter,std::size_t N>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,digest_result<N>>,caiter iter,digest_result<N> const& i)
{
	return ::fast_io::details::crypto_hash_print_reserve_define_common_impl<false,false>(i.digest_block.data(),i.digest_block.data()+i.digest_block.size(),iter);
}

template<std::integral char_type,std::size_t N>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,::fast_io::manipulators::base_full_t<16,true,digest_result<N> const&>>) noexcept
{
	return N*2;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator caiter,std::size_t N>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,::fast_io::manipulators::base_full_t<16,true,digest_result<N> const&>>,caiter iter,::fast_io::manipulators::base_full_t<16,true,digest_result<N> const&> i) noexcept
{
	return ::fast_io::details::crypto_hash_print_reserve_define_common_impl<true,false>(i.reference.digest_block.data(),i.reference.digest_block.data()+i.reference.digest_block.size(),iter);
}

template<typename T,std::size_t N>
[[nodiscard]] inline digest_result<N> do_final_with_size(iterated_hash_ref<T>& ihb)
{
	digest_result<N> result;
	ihb.reference.TruncatedFinal(result.digest_block.data(),N);
	return result;
}

template<typename T>
requires requires(T t)
{
	T::DIGESTSIZE;
}
[[nodiscard]] inline auto do_final(iterated_hash_ref<T>& ihb)
{
	digest_result<T::DIGESTSIZE> result;
	ihb.reference.TruncatedFinal(result.digest_block.data(),T::DIGESTSIZE);
	return result;
}

template<typename T>
iterated_hash_ref(T& func)->iterated_hash_ref<T>;

}

namespace fast_io::manipulators
{
template<std::size_t N>
inline constexpr base_full_t<16,true,::fast_io::cryptopp::digest_result<N> const&> upper(::fast_io::cryptopp::digest_result<N> const& res) noexcept
{
	return {res};
}

template<std::size_t N>
inline constexpr parameter<::fast_io::cryptopp::digest_result<N> const&> lower(::fast_io::cryptopp::digest_result<N> const& res) noexcept
{
	return {res};
}
}