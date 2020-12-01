#pragma once

namespace fast_io
{

enum class buffer_mode
{
in=1<<0,
out=1<<1,
tie=1<<2,
io=in|out|tie,
secure_clear=1<<3
};

constexpr buffer_mode operator&(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr buffer_mode operator|(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr buffer_mode operator^(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr buffer_mode operator~(buffer_mode x) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(~static_cast<utype>(x));
}

inline constexpr buffer_mode& operator&=(buffer_mode& x, buffer_mode y) noexcept{return x=x&y;}

inline constexpr buffer_mode& operator|=(buffer_mode& x, buffer_mode y) noexcept{return x=x|y;}

inline constexpr buffer_mode& operator^=(buffer_mode& x, buffer_mode y) noexcept{return x=x^y;}


template<typename char_type>
inline constexpr std::size_t iobuf_default_buffer_size = details::cal_buffer_size<char_type,true>();

template<typename char_type>
struct basic_buffer
{
	char_type *beg{},*curr{},*ed{};
};

struct empty_buffer
{};

template<typename handletype,
	buffer_mode mde,
	Allocator = io_aligned_allocator<CharT>,
	std::size_t bfs=iobuf_default_buffer_size<typename handletype::char_type>>
class basic_iobuf
{
public:
	using handle_type = handletype;
	using char_type = char_type;
	using allocator_type = Allocator;
	using allocator_traits = std::allocator_traits<allocator_type>;
	inline static constexpr buffer_mode mode = mde;
	inline static constexpr std::size_t buffer_size = bfs;
	[[no_unique_address]] handle_type handle;
	[[no_unique_address]] allocator_type allocator;
	[[no_unique_address]] std::conditional_t<(mode&buffer_mode::in)==buffer_mode::in,basic_buffer<char_type>,empty_buffer> ibuffer;
	[[no_unique_address]] std::conditional_t<(mode&buffer_mode::out)==buffer_mode::out,basic_buffer<char_type>,empty_buffer> obuffer;
};

}