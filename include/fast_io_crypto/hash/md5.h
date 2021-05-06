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

inline void md5_main(std::uint32_t * __restrict state,std::byte const* __restrict block_start,std::size_t block_bytes) noexcept
{
	std::uint32_t at{*state},bt{state[1]},ct{state[2]},dt{state[3]};
	std::uint32_t x[16];
	constexpr std::size_t block_size{64};
	for(auto block(block_start),ed(block_start+block_bytes);block!=ed;block+=block_size)
	{
		std::uint32_t a{at},b{bt},c{ct},d{dt};
		my_memcpy(x,block,block_size);
		uu<operation::F>(a, b, c, d, x[ 0], 7, 0xd76aa478);
		uu<operation::F>(d, a, b, c, x[ 1], 12, 0xe8c7b756);
		uu<operation::F>(c, d, a, b, x[ 2], 17, 0x242070db);
		uu<operation::F>(b, c, d, a, x[ 3], 22, 0xc1bdceee);
		uu<operation::F>(a, b, c, d, x[ 4], 7, 0xf57c0faf);
		uu<operation::F>(d, a, b, c, x[ 5], 12, 0x4787c62a);
		uu<operation::F>(c, d, a, b, x[ 6], 17, 0xa8304613);
		uu<operation::F>(b, c, d, a, x[ 7], 22, 0xfd469501);
		uu<operation::F>(a, b, c, d, x[ 8], 7, 0x698098d8);
		uu<operation::F>(d, a, b, c, x[ 9], 12, 0x8b44f7af);
		uu<operation::F>(c, d, a, b, x[10], 17, 0xffff5bb1);
		uu<operation::F>(b, c, d, a, x[11], 22, 0x895cd7be);
		uu<operation::F>(a, b, c, d, x[12], 7, 0x6b901122);
		uu<operation::F>(d, a, b, c, x[13], 12, 0xfd987193);
		uu<operation::F>(c, d, a, b, x[14], 17, 0xa679438e);
		uu<operation::F>(b, c, d, a, x[15], 22, 0x49b40821);

		/* Round 2 */
		uu<operation::G>(a, b, c, d, x[ 1], 5, 0xf61e2562);
		uu<operation::G>(d, a, b, c, x[ 6], 9, 0xc040b340);
		uu<operation::G>(c, d, a, b, x[11], 14, 0x265e5a51);
		uu<operation::G>(b, c, d, a, x[ 0], 20, 0xe9b6c7aa);
		uu<operation::G>(a, b, c, d, x[ 5], 5, 0xd62f105d);
		uu<operation::G>(d, a, b, c, x[10], 9,  0x2441453);
		uu<operation::G>(c, d, a, b, x[15], 14, 0xd8a1e681);
		uu<operation::G>(b, c, d, a, x[ 4], 20, 0xe7d3fbc8);
		uu<operation::G>(a, b, c, d, x[ 9], 5, 0x21e1cde6);
		uu<operation::G>(d, a, b, c, x[14], 9, 0xc33707d6);
		uu<operation::G>(c, d, a, b, x[ 3], 14, 0xf4d50d87);
		uu<operation::G>(b, c, d, a, x[ 8], 20, 0x455a14ed);
		uu<operation::G>(a, b, c, d, x[13], 5, 0xa9e3e905);
		uu<operation::G>(d, a, b, c, x[ 2], 9, 0xfcefa3f8);
		uu<operation::G>(c, d, a, b, x[ 7], 14, 0x676f02d9);
		uu<operation::G>(b, c, d, a, x[12], 20, 0x8d2a4c8a);

		/* Round 3 */
		uu<operation::H>(a, b, c, d, x[ 5], 4, 0xfffa3942);
		uu<operation::H>(d, a, b, c, x[ 8], 11, 0x8771f681);
		uu<operation::H>(c, d, a, b, x[11], 16, 0x6d9d6122);
		uu<operation::H>(b, c, d, a, x[14], 23, 0xfde5380c);
		uu<operation::H>(a, b, c, d, x[ 1], 4, 0xa4beea44);
		uu<operation::H>(d, a, b, c, x[ 4], 11, 0x4bdecfa9);
		uu<operation::H>(c, d, a, b, x[ 7], 16, 0xf6bb4b60);
		uu<operation::H>(b, c, d, a, x[10], 23, 0xbebfbc70);
		uu<operation::H>(a, b, c, d, x[13], 4, 0x289b7ec6);
		uu<operation::H>(d, a, b, c, x[ 0], 11, 0xeaa127fa);
		uu<operation::H>(c, d, a, b, x[ 3], 16, 0xd4ef3085);
		uu<operation::H>(b, c, d, a, x[ 6], 23,  0x4881d05);
		uu<operation::H>(a, b, c, d, x[ 9], 4, 0xd9d4d039);
		uu<operation::H>(d, a, b, c, x[12], 11, 0xe6db99e5);
		uu<operation::H>(c, d, a, b, x[15], 16, 0x1fa27cf8);
		uu<operation::H>(b, c, d, a, x[ 2], 23, 0xc4ac5665);

		/* Round 4 */
		uu<operation::I>(a, b, c, d, x[ 0], 6, 0xf4292244);
		uu<operation::I>(d, a, b, c, x[ 7], 10, 0x432aff97);
		uu<operation::I>(c, d, a, b, x[14], 15, 0xab9423a7);
		uu<operation::I>(b, c, d, a, x[ 5], 21, 0xfc93a039);
		uu<operation::I>(a, b, c, d, x[12], 6, 0x655b59c3);
		uu<operation::I>(d, a, b, c, x[ 3], 10, 0x8f0ccc92);
		uu<operation::I>(c, d, a, b, x[10], 15, 0xffeff47d);
		uu<operation::I>(b, c, d, a, x[ 1], 21, 0x85845dd1);
		uu<operation::I>(a, b, c, d, x[ 8], 6, 0x6fa87e4f);
		uu<operation::I>(d, a, b, c, x[15], 10, 0xfe2ce6e0);
		uu<operation::I>(c, d, a, b, x[ 6], 15, 0xa3014314);
		uu<operation::I>(b, c, d, a, x[13], 21, 0x4e0811a1);
		uu<operation::I>(a, b, c, d, x[ 4], 6, 0xf7537e82);
		uu<operation::I>(d, a, b, c, x[11], 10, 0xbd3af235);
		uu<operation::I>(c, d, a, b, x[ 2], 15, 0x2ad7d2bb);
		uu<operation::I>(b, c, d, a, x[ 9], 21, 0xeb86d391);

		at+=a;
		bt+=b;
		ct+=c;
		dt+=d;
	}
	*state=at;
	state[1]=bt;
	state[2]=ct;
	state[3]=dt;
}


}

class md5_function
{
public:
	using digest_type = ::fast_io::freestanding::array<std::uint32_t,4>;
	static inline constexpr digest_type digest_initial_value{0x67452301,0xefcdab89,0x98badcfe,0x10325476};
	static inline constexpr std::size_t block_size{64};
	void operator()(std::uint32_t *state,std::byte const* block_start,std::size_t block_bytes) noexcept
	{
		details::md5::md5_main(state,block_start,block_bytes);
	}
};

}