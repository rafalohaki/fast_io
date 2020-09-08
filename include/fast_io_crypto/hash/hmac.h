#pragma once

namespace fast_io
{
template<typename func,bool endian_reverse>
struct hmac
{
	using function_type = func;
	using key_type = std::array<std::byte,func::block_size>;
	function_type function;
	key_type inner_key;
	key_type outer_key{};
	inline static constexpr std::size_t block_size = function_type::block_size;
	hmac(std::span<std::byte const> init_key)
	{
		if(block_size<init_key.size())
		{
			hash_processor processor(function);
			write(processor,reinterpret_cast<char const*>(init_key.data()),
				reinterpret_cast<char const*>(init_key.data()+init_key.size()));
			processor.do_final();
			if constexpr(endian_reverse)
				for(auto & e : function.digest_block)
					e=details::byte_swap(e);
			memcpy(outer_key.data(),function.digest_block.data(),function.digest_block.size());
			function={};
		}
		else
			memcpy(outer_key.data(),init_key.data(),init_key.size());
		for(std::size_t i{};i!=inner_key.size();++i)
			inner_key[i]=outer_key[i]^std::byte{0x36};
	}
	hmac(cstring_view key):hmac(std::as_bytes(std::span{key.data(),key.size()})){}
	std::size_t block_init(std::span<std::byte,block_size> sp)
	{
		memcpy(sp.data(),inner_key.data(),sizeof(key_type));
		return sizeof(key_type);
	}
	void operator()(std::span<std::byte const,block_size> process_block)
	{
		function(process_block);
	}
	void operator()(std::span<std::byte const> process_blocks)
	{
		function(process_blocks);
	}
	void digest(std::span<std::byte const> final_block)
	{
		function.digest(final_block);
		for(auto & e : outer_key)
			e^=std::byte{0x5c};
		auto digest_block{function.digest_block};
		if constexpr(endian_reverse)
			for(auto& e : digest_block)
				e=details::byte_swap(e);
		function={};
		hash_processor processor(function);
		write(processor,reinterpret_cast<char const*>(outer_key.data()),
			reinterpret_cast<char const*>(outer_key.data()+outer_key.size()));
		write(processor,reinterpret_cast<char const*>(digest_block.data()),
			reinterpret_cast<char const*>(digest_block.data()+digest_block.size()));
		processor.do_final();
	}
};


template<reserve_printable T,bool endian_reverse>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<hmac<T,endian_reverse>>)
{
	return print_reserve_size(io_reserve_type<T>);
}

template<reserve_printable T,bool endian_reverse,std::random_access_iterator caiter>
inline constexpr caiter print_reserve_define(io_reserve_type_t<hmac<T,endian_reverse>>,caiter iter,auto& i)
{
	return print_reserve_define(io_reserve_type<T>,iter,i.function);
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