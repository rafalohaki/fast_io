#pragma once

namespace fast_io
{
template<typename func,bool endian_reverse>
struct hmac
{
	using function_type = func;
	using key_type = ::fast_io::freestanding::array<std::byte,func::block_size>;
	function_type function;
	key_type inner_key;
	key_type outer_key{};
	inline static constexpr std::size_t block_size = function_type::block_size;
	hmac(std::byte const* init_key_data, std::size_t init_key_size) noexcept
	{
		if(block_size<init_key_size)
		{
			hash_processor processor(function);
			write(processor,reinterpret_cast<char const*>(init_key_data),
				reinterpret_cast<char const*>(init_key_data+init_key_size));
			processor.do_final();
			if constexpr(endian_reverse)
				for(auto & e : function.digest_block)
					e=details::byte_swap(e);
			::fast_io::details::my_memcpy(outer_key.data(),function.digest_block.data(),function.digest_block.size());
			function={};
		}
		else
			::fast_io::details::my_memcpy(outer_key.data(),init_key_data,init_key_size);
		for(std::size_t i{};i!=inner_key.size();++i)
			inner_key[i]=outer_key[i]^std::byte{0x36};
	}
	hmac(::fast_io::freestanding::string_view key) noexcept:hmac(reinterpret_cast<std::byte const*>(key.data()),key.size()){}
	std::size_t block_init(std::byte const* sp) noexcept
	{
		::fast_io::details::my_memcpy(inner_key.data(),sp,sizeof(key_type));
		return sizeof(key_type);
	}
	void operator()(std::byte const* process_blocks,std::size_t block_bytes) noexcept
	{
		function(process_blocks,block_bytes);
	}
	void digest(std::byte const* final_process_blocks,std::size_t final_block_bytes) noexcept
	{
		function.digest(final_process_blocks,final_block_bytes);
		for(auto & e : outer_key)
			e^=std::byte{0x5c};
		auto digest_block{function.digest_block};
		if constexpr(endian_reverse)
			for(auto& e : digest_block)
				e=details::byte_swap(e);
		function={};
		hash_processor processor(function);
		write(processor,outer_key.data(),outer_key.data()+outer_key.size());
		write(processor,digest_block.data(),digest_block.data()+digest_block.size());
		processor.do_final();
	}
};


template<std::integral char_type,typename T,bool endian_reverse>
requires reserve_printable<char_type,T>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,hmac<T,endian_reverse>>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,T>);
}

template<std::integral char_type,typename T,bool endian_reverse,::fast_io::freestanding::random_access_iterator caiter>
requires reserve_printable<char_type,T>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,hmac<T,endian_reverse>>,caiter iter,auto& i) noexcept
{
	return print_reserve_define(io_reserve_type<char_type,T>,iter,i.function);
}

using hmac_sha1
[[deprecated("SHA1 is no longer a secure algorithm. See wikipedia https://en.wikipedia.org/wiki/SHA-1")]]
=hmac<sha<sha1_function>,true>;

using hmac_sha256=hmac<sha256,true>;

using hmac_sha512=hmac<sha512,true>;


using hmac_md5 
[[deprecated("The weaknesses of MD5 have been exploited in the field, most infamously by the Flame malware in 2012. See wikipedia https://en.wikipedia.org/wiki/MD5")]]
=hmac<sha<md5_function,false>,false>;


}