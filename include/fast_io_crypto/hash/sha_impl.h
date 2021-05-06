#pragma once

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
#if __has_cpp_attribute(no_unique_address)
	[[no_unique_address]]
#endif
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


template<std::integral char_type,typename T,bool endian_reverse>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,sha<T,endian_reverse>>) noexcept
{
	return sizeof(typename T::digest_type)*8;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator caiter,typename T,bool endian_reverse>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,sha<T,endian_reverse>>,caiter iter,sha<T,endian_reverse> const& i) noexcept
{
	return details::crypto_hash_main_reserve_define_common_impl<false,endian_reverse>(i.digest_block.data(),i.digest_block.data()+i.digest_block.size(),iter);
}
template<std::integral char_type,typename T,bool endian_reverse>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::base_full_t<16,true,sha<T,endian_reverse> const&>>) noexcept
{
	return sizeof(typename T::digest_type)*8;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator caiter,typename T,bool endian_reverse>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,manipulators::base_full_t<16,true,sha<T,endian_reverse> const&>>,caiter iter,manipulators::base_full_t<16,true,sha<T,endian_reverse> const&> i) noexcept
{
	return details::crypto_hash_main_reserve_define_common_impl<true,endian_reverse>(i.reference.digest_block.data(),i.reference.digest_block.data()+i.reference.digest_block.size(),iter);
}

using sha256 = sha<sha256_function>;


namespace manipulators
{

template<typename T,bool endian_reverse>
inline constexpr base_full_t<16,true,::fast_io::sha<T,endian_reverse> const&> upper(::fast_io::sha<T,endian_reverse> const& res) noexcept
{
	return {res};
}

template<typename T,bool endian_reverse>
inline constexpr parameter<::fast_io::sha<T,endian_reverse> const&> lower(::fast_io::sha<T,endian_reverse> const& res) noexcept
{
	return {res};
}

}

}
