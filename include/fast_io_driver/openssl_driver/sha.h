#pragma once

namespace fast_io::ossl
{

enum class hash_flag
{
md2,md4,md5,sha1,sha224,sha256,sha384,sha512
};

template<std::size_t N>
class hash_final_result
{
public:
	using digest_type = ::fast_io::freestanding::array<unsigned char,N>;
#if __has_cpp_attribute(no_unique_address)
	[[no_unique_address]]
#endif
	digest_type digest_block;
	constexpr hash_final_result() noexcept=default;
	constexpr hash_final_result(hash_final_result const&) noexcept=default;
	constexpr hash_final_result& operator=(hash_final_result const&) noexcept=default;
	constexpr ~hash_final_result()
	{
		secure_clear(digest_block.data(),N);
	}
};

template<std::integral char_type,std::size_t N>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,hash_final_result<N>>) noexcept
{
	return N*2;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator caiter,std::size_t N>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,hash_final_result<N>>,caiter iter,hash_final_result<N> const& i) noexcept
{
	return ::fast_io::details::crypto_hash_print_reserve_define_common_impl<false,false>(i.digest_block.data(),i.digest_block.data()+i.digest_block.size(),iter);
}
#if 0
template<std::integral char_type,std::size_t N>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,::fast_io::manipulators::base_full_t<16,true,hash_final_result<N> const&>>) noexcept
{
	return N*2;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator caiter,std::size_t N>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,::fast_io::manipulators::base_full_t<16,true,hash_final_result<N> const&>>,caiter iter,::fast_io::manipulators::base_full_t<16,true,hash_final_result<N> const&> i) noexcept
{
	return ::fast_io::details::crypto_hash_print_reserve_define_common_impl<true,false>(i.reference.digest_block.data(),i.reference.digest_block.data()+i.reference.digest_block.size(),iter);
}
#endif
namespace details
{

template<typename T>
struct hash_native_handle_type_carrier
{
	using type=T;
};

template<hash_flag ctx_type>
inline constexpr auto hash_type_cal() noexcept
{
	using enum hash_flag;
	if constexpr(ctx_type==md2)
	{
#if __has_include(<openssl/md2.h>) && defined(MD2_CTX)
		return hash_native_handle_type_carrier<MD2_CTX>{};
#else
		static_assert(ctx_type==md2,"MD2 is dangerous and removed from OpenSSL");
#endif
	}
	else if constexpr(ctx_type==md4)
		return hash_native_handle_type_carrier<MD4_CTX>{};
	else if constexpr(ctx_type==md5)
		return hash_native_handle_type_carrier<MD5_CTX>{};
	else if constexpr(ctx_type==sha1)
		return hash_native_handle_type_carrier<SHA_CTX>{};
	else if constexpr(ctx_type==sha224||ctx_type==sha256)
		return hash_native_handle_type_carrier<SHA256_CTX>{};
	else if constexpr(ctx_type==sha384||ctx_type==sha512)
		return hash_native_handle_type_carrier<SHA512_CTX>{};
}

template<hash_flag flag>
using real_hash_type = typename decltype(hash_type_cal<flag>())::type;

template<hash_flag ctx_type,typename T>
inline constexpr auto hash_type_initialize(T* context)
{
	using enum hash_flag;
#if __has_include(<openssl/md2.h>) && defined(MD2_CTX)
	if constexpr(ctx_type==md2)
	{
		if(!MD2_Init(context))
			throw_openssl_error();
	}
	else
#endif
	if constexpr(ctx_type==md4)
	{
		if(!MD4_Init(context))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==md5)
	{
		if(!MD5_Init(context))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha1)
	{
		if(!SHA1_Init(context))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha224)
	{
		if(!SHA224_Init(context))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha256)
	{
		if(!SHA256_Init(context))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha384)
	{
		if(!SHA384_Init(context))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha512)
	{
		if(!SHA512_Init(context))
			throw_openssl_error();
	}
}

template<hash_flag ctx_type,typename T>
inline constexpr auto hash_do_final(T* ctx)
{
	using enum hash_flag;
#if __has_include(<openssl/md2.h>) && defined(MD2_CTX)
	if constexpr(ctx_type==md2)
	{
		hash_final_result<MD2_DIGEST_LENGTH> ret; 
		if(!MD2_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else
#endif
	if constexpr(ctx_type==md4)
	{
		hash_final_result<MD4_DIGEST_LENGTH> ret; 
		if(!MD4_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else if constexpr(ctx_type==md5)
	{
		hash_final_result<MD5_DIGEST_LENGTH> ret; 
		if(!MD5_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else if constexpr(ctx_type==sha1)
	{
		hash_final_result<SHA_DIGEST_LENGTH> ret; 
		if(!SHA1_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else if constexpr(ctx_type==sha224)
	{
		hash_final_result<SHA224_DIGEST_LENGTH> ret; 
		if(!SHA224_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else if constexpr(ctx_type==sha256)
	{
		hash_final_result<SHA256_DIGEST_LENGTH> ret; 
		if(!SHA256_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else if constexpr(ctx_type==sha384)
	{
		hash_final_result<SHA384_DIGEST_LENGTH> ret; 
		if(!SHA384_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else if constexpr(ctx_type==sha512)
	{
		hash_final_result<SHA512_DIGEST_LENGTH> ret; 
		if(!SHA512_Final(ret.digest_block.data(),ctx))
			throw_openssl_error();
		return ret;
	}
	else
	{
		static_assert(ctx_type==sha512,"unknown crypto flag");
	}
}


template<hash_flag ctx_type,typename T>
inline void write_hash_impl(T* context,char unsigned const* first,char unsigned const* last)
{
	using enum hash_flag;
#ifdef FAST_IO_OPENSSL_ENABLE_MD2
	if constexpr(ctx_type==md2)
	{
		if(!MD2_Update(context,first,last-first))
			throw_openssl_error();
	}
	else
#endif
	if constexpr(ctx_type==md4)
	{
		if(!MD4_Update(context,first,last-first))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==md5)
	{
		if(!MD5_Update(context,first,last-first))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha1)
	{
		if(!SHA1_Update(context,first,last-first))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha224)
	{
		if(!SHA224_Update(context,first,last-first))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha256)
	{
		if(!SHA256_Update(context,first,last-first))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha384)
	{
		if(!SHA384_Update(context,first,last-first))
			throw_openssl_error();
	}
	else if constexpr(ctx_type==sha512)
	{
		if(!SHA512_Update(context,first,last-first))
			throw_openssl_error();
	}
}

template<hash_flag ctx_type,typename T>
inline void write_hash_scatter_impl(T* context,io_scatter_t* scatters_base,std::size_t scatters_len)
{
	for(std::size_t i{};i!=scatters_len;++i)
	{
		auto e{scatters_base[i]};
		write_hash_impl<ctx_type>(context,reinterpret_cast<char unsigned const*>(e.base),
			reinterpret_cast<char unsigned const*>(e.base)+e.len);
	}
}

}


template<std::integral ch_type,hash_flag flg>
requires (static_cast<std::size_t>(flg)<8)
class basic_hash_context
{
public:
	static inline constexpr hash_flag flag = flg;
	using char_type = ch_type;
	using native_handle_type = details::real_hash_type<flg>;
#if __has_cpp_attribute(no_unique_address)
	[[no_unique_address]]
#endif
	native_handle_type ctx;
	basic_hash_context()
	{
		details::hash_type_initialize<flg>(__builtin_addressof(ctx));
	}
	[[nodiscard("What's the point of calculating this without getting your result??")]] auto do_final()
	{
		return details::hash_do_final<flg>(__builtin_addressof(ctx));
	}
	constexpr basic_hash_context(basic_hash_context const&) noexcept=default;
	constexpr basic_hash_context& operator=(basic_hash_context&) noexcept=default;
	~basic_hash_context()
	{
		secure_clear(__builtin_addressof(ctx),sizeof(ctx));
	}
};

template<std::integral char_type,hash_flag ctx_type,::fast_io::freestanding::contiguous_iterator Iter>
inline void write(basic_hash_context<char_type,ctx_type>& ctx,Iter begin,Iter end)
{
	details::write_hash_impl<ctx_type>(__builtin_addressof(ctx.ctx),
		reinterpret_cast<char unsigned const*>(::fast_io::freestanding::to_address(begin)),
		reinterpret_cast<char unsigned const*>(::fast_io::freestanding::to_address(end)));
}

template<std::integral char_type,hash_flag ctx_type>
inline void scatter_write(basic_hash_context<char_type,ctx_type>& ctx,io_scatters_t sp)
{
	details::write_hash_scatter_impl<ctx_type>(__builtin_addressof(ctx.ctx),sp.base,sp.len);
}
#ifdef FAST_IO_OPENSSL_ENABLE_MD2
using md2
[[deprecated("The security of MD2 has been severely compromised. In 2009, security updates were issued disabling MD2 in OpenSSL, GnuTLS, and Network Security Services. See wikipedia https://en.wikipedia.org/wiki/MD2")]]
=basic_hash_context<char,hash_flag::md2>;
#endif
using md4
[[deprecated("The security of MD4 has been severely compromised. The first full collision attack against MD4 was published in 1995 and several newer attacks have been published since then. As of 2007, an attack can generate collisions in less than 2 MD4 hash operations. A theoretical preimage attack also exists. See wikipedia https://en.wikipedia.org/wiki/MD4")]]
=basic_hash_context<char,hash_flag::md4>;
using md5
[[deprecated("The weaknesses of MD5 have been exploited in the field, most infamously by the Flame malware in 2012. See wikipedia https://en.wikipedia.org/wiki/MD5")]]
=basic_hash_context<char,hash_flag::md5>;
using sha1
[[deprecated("SHA1 is no longer a secure algorithm. See wikipedia https://en.wikipedia.org/wiki/SHA-1")]]
=basic_hash_context<char,hash_flag::sha1>;
using sha224=basic_hash_context<char,hash_flag::sha224>;
using sha256=basic_hash_context<char,hash_flag::sha256>;
using sha384=basic_hash_context<char,hash_flag::sha384>;
using sha512=basic_hash_context<char,hash_flag::sha512>;

}
#if 0
namespace fast_io::manipulators
{

template<std::size_t N>
inline constexpr base_full_t<16,true,::fast_io::ossl::hash_final_result<N> const&> upper(::fast_io::ossl::hash_final_result<N> const& res) noexcept
{
	return {res};
}

template<std::size_t N>
inline constexpr parameter<::fast_io::ossl::hash_final_result<N> const&> lower(::fast_io::ossl::hash_final_result<N> const& res) noexcept
{
	return {res};
}

}
#endif