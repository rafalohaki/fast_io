#pragma once


#if defined(__arm__) || defined(__aarch32__) || defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM)
# if defined(__ARM_NEON) || defined(_MSC_VER)
#  include <arm_neon.h>
# endif
# if defined(__GNUC__) && !defined(__apple_build_version__)
#  if defined(__ARM_ACLE) || defined(__ARM_FEATURE_CRYPTO)
#   include <arm_acle.h>
#  endif
# endif
#endif

#include"sha1.h"
#include"sha256.h"
#include"sha512.h"
namespace fast_io
{
template<typename T,bool endian_reverse=true>
class sha
{
public:
	using function_type = T;
	using digest_type = typename T::digest_type;
	digest_type digest_block = T::digest_initial_value;
	static inline constexpr std::size_t block_size = T::block_size;
	T function;
	std::uint64_t transform_counter{};

	void operator()(std::byte const* process_blocks,std::size_t process_block_bytes) noexcept//This is multiple blocks
	{
		function(digest_block.data(),process_blocks,process_block_bytes);
		transform_counter+=process_block_bytes/block_size;
	}

	void digest(std::byte const* final_block,std::size_t final_block_bytes) noexcept//contracts: final_block.size()<block_size
	{
		std::uint64_t total_bits(static_cast<std::uint64_t>(transform_counter*block_size+final_block_bytes)*8);
		::fast_io::freestanding::array<std::byte,block_size> blocks{};
		::fast_io::details::my_memcpy(blocks.data(),final_block,final_block_bytes);
		blocks[final_block_bytes]=std::byte{0x80};
		auto start{blocks.data()+blocks.size()-8};
		if(block_size<=final_block_bytes+8)
		{
			function(digest_block.data(),blocks.data(),block_size);
			::fast_io::details::my_memset(blocks.data(),0,sizeof(block_size));
		}
		if constexpr(endian_reverse)
		{
			total_bits=details::byte_swap(total_bits);
			std::uint32_t bu3(static_cast<std::uint32_t>(total_bits));
			::fast_io::details::my_memcpy(start,std::addressof(bu3),4);
			std::uint32_t bu4(static_cast<std::uint32_t>(total_bits>>32));
			::fast_io::details::my_memcpy(start+4,std::addressof(bu4),4);
		}
		else
			::fast_io::details::my_memcpy(start,std::addressof(total_bits),8);
		function(digest_block.data(),blocks.data(),block_size);
	}
};

using sha1
[[deprecated("SHA1 is no longer a secure algorithm. See wikipedia https://en.wikipedia.org/wiki/SHA-1")]]
= sha<sha1_function>;

using sha256 = sha<sha256_function>;

using sha512 = sha<sha512_function>;

template<std::integral char_type,typename T,bool endian_reverse>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,sha<T,endian_reverse>>) noexcept
{
	return sizeof(typename T::digest_type)*8;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator caiter,typename T,bool endian_reverse>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,sha<T,endian_reverse>>,caiter iter,auto& i) noexcept
{
	constexpr std::size_t offset{sizeof(typename T::digest_type::value_type)*2};
	for(auto e : i.digest_block)
	{
		if constexpr(!endian_reverse)
			e=details::byte_swap(e);
		fast_io::details::optimize_size::output_unsigned_dummy<offset,16>(iter,e);
		iter+=offset;
	}
	return iter;
}
}