﻿#pragma once

namespace fast_io
{

template<typename T>
struct basic_io_scatter_t
{
	T const* base;
	std::size_t len;
	inline operator basic_io_scatter_t<void>() const noexcept requires(!std::same_as<T,void>)
	{
		return {base,len*sizeof(T)};
	}
};

//should be binary compatible with POSIX's iovec

using io_scatter_t = basic_io_scatter_t<void>;
using io_scatters_t = basic_io_scatter_t<io_scatter_t>;

struct io_scatter_status_t
{
	std::size_t total_size;
	std::size_t position;
	std::size_t position_in_scatter;
};

template<typename T>
struct basic_message_hdr
{
	T const        *name;       /* Optional address */
	std::size_t     namelen;    /* Size of address */
	basic_io_scatter_t<T> const* iov;        /* Scatter/gather array */
	std::size_t   iovlen;     /* # elements in msg_iov */
	T const       *control;    /* Ancillary data, see below */
	std::size_t   controllen; /* Ancillary data buffer len */
	int           flags;      /* Flags (unused) */
	inline operator basic_message_hdr<void>() const noexcept requires(!std::same_as<T,void>)
	{
		return {name,namelen*sizeof(T),iov,iovlen*sizeof(T),control,controllen,flags};
	}
};

using message_hdr = basic_message_hdr<void>;
//should be binary compatible with POSIX's msghdr

template<typename T>
struct io_type_t
{
using type = T;
};
template<typename T>
inline constexpr io_type_t<T> io_type{};
#if 0
template<typename T>
struct io_message_t
{
	using type = typename std::remove_cvref_t<decltype(io_message_type(*static_cast<T*>(nullptr)))>::type;
};
#endif

enum class io_reserve_flags
{
none=0,
secure=1,
normal=1<<1,
serial=1<<2,
scatter=1<<3,
generator=1<<4,
};

constexpr io_reserve_flags operator&(io_reserve_flags x, io_reserve_flags y) noexcept
{
using utype = typename std::underlying_type<io_reserve_flags>::type;
return static_cast<io_reserve_flags>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr io_reserve_flags operator|(io_reserve_flags x, io_reserve_flags y) noexcept
{
using utype = typename std::underlying_type<io_reserve_flags>::type;
return static_cast<io_reserve_flags>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr io_reserve_flags operator^(io_reserve_flags x, io_reserve_flags y) noexcept
{
using utype = typename std::underlying_type<io_reserve_flags>::type;
return static_cast<io_reserve_flags>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr io_reserve_flags operator~(io_reserve_flags x) noexcept
{
using utype = typename std::underlying_type<io_reserve_flags>::type;
return static_cast<io_reserve_flags>(~static_cast<utype>(x));
}

inline constexpr io_reserve_flags& operator&=(io_reserve_flags& x, io_reserve_flags y) noexcept{return x=x&y;}

inline constexpr io_reserve_flags& operator|=(io_reserve_flags& x, io_reserve_flags y) noexcept{return x=x|y;}

inline constexpr io_reserve_flags& operator^=(io_reserve_flags& x, io_reserve_flags y) noexcept{return x=x^y;}

template<std::integral char_type,typename T,io_reserve_flags flags=io_reserve_flags::normal>
struct io_reserve_type_t
{
explicit constexpr io_reserve_type_t() noexcept =default;
};
template<std::integral char_type,typename T>
inline constexpr io_reserve_type_t<char_type,T> io_reserve_type{};

template<std::integral char_type,typename T>
using io_serial_type_t=io_reserve_type_t<char_type,T,io_reserve_flags::serial>;
template<std::integral char_type,typename T>
inline constexpr io_serial_type_t<char_type,T> io_serial_type{};

template<std::integral char_type,typename T>
using io_secure_reserve_type_t=io_reserve_type_t<char_type,T,io_reserve_flags::secure|io_reserve_flags::normal>;

template<std::integral char_type,typename T>
inline constexpr io_secure_reserve_type_t<char_type,T> io_secure_reserve_type{};

template<std::integral char_type,typename T>
using io_secure_serial_type_t=io_reserve_type_t<char_type,T,io_reserve_flags::secure|io_reserve_flags::serial>;
template<std::integral char_type,typename T>
inline constexpr io_secure_serial_type_t<char_type,T> io_secure_serial_type{};

template<std::integral char_type,typename T>
using io_generator_type_t=io_reserve_type_t<char_type,T,io_reserve_flags::generator>;
template<std::integral char_type,typename T>
inline constexpr io_generator_type_t<char_type,T> io_generator_type{};

struct io_alias_t
{
explicit constexpr io_alias_t() noexcept =default;
};

inline constexpr io_alias_t io_alias{};

template<std::integral char_type>
struct io_alias_type_t
{
explicit constexpr io_alias_type_t() noexcept = default;
};

template<std::integral char_type>
inline constexpr io_alias_type_t<char_type> io_alias_type{};

template<bool contiguous_only = false>
struct scan_context_t
{
	explicit inline constexpr scan_context_t() noexcept = default;
};

template<bool contiguous_only = false>
inline constexpr scan_context_t<contiguous_only> scan_context;

template<typename T>
struct scan_skip_type_t
{
	explicit inline constexpr scan_skip_type_t() noexcept = default;
};
template<typename T>
inline constexpr scan_skip_type_t<T> scan_skip_type{};

template<std::integral char_type>
struct try_get_result
{
	char_type ch;
	bool eof;
};

}
