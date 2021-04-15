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


template<typename T, ::fast_io::freestanding::contiguous_iterator Iter>
inline void write(iterated_hash_ref<T>& ihb,Iter begin,Iter end)
{
	ihb.reference.Update(reinterpret_cast<CryptoPP::byte*>(::fast_io::freestanding::to_address(begin)),
		(end-begin)*sizeof(*begin));
}

template<std::size_t N>
class digest_result
{
public:
	using digest_type = ::fast_io::freestanding::array<CryptoPP::byte,N>;
	digest_type digest_block;
};

template<std::integral char_type,std::size_t N>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,digest_result<N>>)
{
	return N*2;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator caiter,std::size_t N>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,digest_result<N>>,caiter iter,auto const& i)
{
	for(auto e : i.digest_block)
	{
		fast_io::details::optimize_size::output_unsigned_dummy<2,16>(iter,e);
		iter+=2;
	}
	return iter;
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