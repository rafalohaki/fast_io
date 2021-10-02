#pragma once

namespace fast_io
{

namespace details::md5
{

enum class operation
{
F,G,H,I
};

template<operation op>
inline constexpr auto unit(auto const x,auto const y,auto const z) noexcept
{
	if constexpr(op==operation::F)
		return (x&y)|(~x&z);
	else if constexpr(op==operation::G)
		return (x&z)|(y&~z);
	else if constexpr(op==operation::H)
		return x^y^z;
	else
		return y^(x|(~z));
}

template<operation op>
inline constexpr auto uu_impl(auto a,auto b,auto c,auto d,auto x,auto s,auto ac) noexcept
{
	return std::rotl(a+unit<op>(b,c,d)+x+ac,s)+b;
}

template<operation op>
inline constexpr void uu(auto& a,auto b,auto c,auto d,auto x,auto s,auto ac) noexcept
{
	a=uu_impl<op>(a,b,c,d,x,s,ac);
}

inline
#if __cpp_lib_is_constant_evaluated >= 201811L
constexpr
#endif
void md5_main(std::uint32_t * __restrict state,std::byte const* __restrict block_start,std::size_t block_bytes) noexcept
{
	std::uint32_t x[16];
	std::uint32_t a{*state},b{state[1]},c{state[2]},d{state[3]};
	constexpr std::size_t block_size{64};
	for(auto block(block_start),ed(block_start+block_bytes);block!=ed;block+=block_size)
	{
#if __cpp_lib_is_constant_evaluated >= 201811L
		if(std::is_constant_evaluated())
		{
			for(std::size_t j{};j!=16;++j)
			{
				auto dj{block+j*4};
				x[j]=(std::to_integer<std::uint32_t>(*dj))|(std::to_integer<std::uint32_t>(dj[1])<<8)|
					(std::to_integer<std::uint32_t>(dj[2])<<16)|std::to_integer<std::uint32_t>(dj[3]<<24);
			}
		}
		else
#endif
		{
			my_memcpy(x,block,block_size);
		}
		uu<operation::F>(a, b, c, d, x[ 0], 7u, 0xd76aa478);
		uu<operation::F>(d, a, b, c, x[ 1], 12u, 0xe8c7b756);
		uu<operation::F>(c, d, a, b, x[ 2], 17u, 0x242070db);
		uu<operation::F>(b, c, d, a, x[ 3], 22u, 0xc1bdceee);
		uu<operation::F>(a, b, c, d, x[ 4], 7u, 0xf57c0faf);
		uu<operation::F>(d, a, b, c, x[ 5], 12u, 0x4787c62a);
		uu<operation::F>(c, d, a, b, x[ 6], 17u, 0xa8304613);
		uu<operation::F>(b, c, d, a, x[ 7], 22u, 0xfd469501);
		uu<operation::F>(a, b, c, d, x[ 8], 7u, 0x698098d8);
		uu<operation::F>(d, a, b, c, x[ 9], 12u, 0x8b44f7af);
		uu<operation::F>(c, d, a, b, x[10], 17u, 0xffff5bb1);
		uu<operation::F>(b, c, d, a, x[11], 22u, 0x895cd7be);
		uu<operation::F>(a, b, c, d, x[12], 7u, 0x6b901122);
		uu<operation::F>(d, a, b, c, x[13], 12u, 0xfd987193);
		uu<operation::F>(c, d, a, b, x[14], 17u, 0xa679438e);
		uu<operation::F>(b, c, d, a, x[15], 22u, 0x49b40821);

		/* Round 2 */
		uu<operation::G>(a, b, c, d, x[ 1], 5u, 0xf61e2562);
		uu<operation::G>(d, a, b, c, x[ 6], 9u, 0xc040b340);
		uu<operation::G>(c, d, a, b, x[11], 14u, 0x265e5a51);
		uu<operation::G>(b, c, d, a, x[ 0], 20u, 0xe9b6c7aa);
		uu<operation::G>(a, b, c, d, x[ 5], 5u, 0xd62f105d);
		uu<operation::G>(d, a, b, c, x[10], 9u,  0x2441453);
		uu<operation::G>(c, d, a, b, x[15], 14u, 0xd8a1e681);
		uu<operation::G>(b, c, d, a, x[ 4], 20u, 0xe7d3fbc8);
		uu<operation::G>(a, b, c, d, x[ 9], 5u, 0x21e1cde6);
		uu<operation::G>(d, a, b, c, x[14], 9u, 0xc33707d6);
		uu<operation::G>(c, d, a, b, x[ 3], 14u, 0xf4d50d87);
		uu<operation::G>(b, c, d, a, x[ 8], 20u, 0x455a14ed);
		uu<operation::G>(a, b, c, d, x[13], 5u, 0xa9e3e905);
		uu<operation::G>(d, a, b, c, x[ 2], 9u, 0xfcefa3f8);
		uu<operation::G>(c, d, a, b, x[ 7], 14u, 0x676f02d9);
		uu<operation::G>(b, c, d, a, x[12], 20u, 0x8d2a4c8a);

		/* Round 3 */
		uu<operation::H>(a, b, c, d, x[ 5], 4u, 0xfffa3942);
		uu<operation::H>(d, a, b, c, x[ 8], 11u, 0x8771f681);
		uu<operation::H>(c, d, a, b, x[11], 16u, 0x6d9d6122);
		uu<operation::H>(b, c, d, a, x[14], 23u, 0xfde5380c);
		uu<operation::H>(a, b, c, d, x[ 1], 4u, 0xa4beea44);
		uu<operation::H>(d, a, b, c, x[ 4], 11u, 0x4bdecfa9);
		uu<operation::H>(c, d, a, b, x[ 7], 16u, 0xf6bb4b60);
		uu<operation::H>(b, c, d, a, x[10], 23u, 0xbebfbc70);
		uu<operation::H>(a, b, c, d, x[13], 4u, 0x289b7ec6);
		uu<operation::H>(d, a, b, c, x[ 0], 11u, 0xeaa127fa);
		uu<operation::H>(c, d, a, b, x[ 3], 16u, 0xd4ef3085);
		uu<operation::H>(b, c, d, a, x[ 6], 23u,  0x4881d05);
		uu<operation::H>(a, b, c, d, x[ 9], 4u, 0xd9d4d039);
		uu<operation::H>(d, a, b, c, x[12], 11u, 0xe6db99e5);
		uu<operation::H>(c, d, a, b, x[15], 16u, 0x1fa27cf8);
		uu<operation::H>(b, c, d, a, x[ 2], 23u, 0xc4ac5665);

		/* Round 4 */
		uu<operation::I>(a, b, c, d, x[ 0], 6u, 0xf4292244);
		uu<operation::I>(d, a, b, c, x[ 7], 10u, 0x432aff97);
		uu<operation::I>(c, d, a, b, x[14], 15u, 0xab9423a7);
		uu<operation::I>(b, c, d, a, x[ 5], 21u, 0xfc93a039);
		uu<operation::I>(a, b, c, d, x[12], 6u, 0x655b59c3);
		uu<operation::I>(d, a, b, c, x[ 3], 10u, 0x8f0ccc92);
		uu<operation::I>(c, d, a, b, x[10], 15u, 0xffeff47d);
		uu<operation::I>(b, c, d, a, x[ 1], 21u, 0x85845dd1);
		uu<operation::I>(a, b, c, d, x[ 8], 6u, 0x6fa87e4f);
		uu<operation::I>(d, a, b, c, x[15], 10u, 0xfe2ce6e0);
		uu<operation::I>(c, d, a, b, x[ 6], 15u, 0xa3014314);
		uu<operation::I>(b, c, d, a, x[13], 21u, 0x4e0811a1);
		uu<operation::I>(a, b, c, d, x[ 4], 6u, 0xf7537e82);
		uu<operation::I>(d, a, b, c, x[11], 10u, 0xbd3af235);
		uu<operation::I>(c, d, a, b, x[ 2], 15u, 0x2ad7d2bb);
		uu<operation::I>(b, c, d, a, x[ 9], 21u, 0xeb86d391);

		a=(*state+=a);
		b=(state[1]+=b);
		c=(state[2]+=c);
		d=(state[3]+=d);
	}
}

}

class md5_function
{
public:
	using digest_type = ::fast_io::freestanding::array<std::uint32_t,4>;
	static inline constexpr digest_type digest_initial_value{0x67452301,0xefcdab89,0x98badcfe,0x10325476};
	static inline constexpr std::size_t block_size{64};
#if __cpp_lib_is_constant_evaluated >= 201811L
	constexpr
#endif
	void operator()(std::uint32_t *state,std::byte const* block_start,std::size_t block_bytes) noexcept
	{
		details::md5::md5_main(state,block_start,block_bytes);
	}
};

}