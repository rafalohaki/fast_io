#pragma once

namespace fast_io
{

template<typename T>
struct basic_io_scatter_t
{
	T const* base;
	std::size_t len;
	inline operator basic_io_scatter_t<void>() const noexcept
	{
		return {base,len*sizeof(T)};
	}
};

//should be binary compatible with POSIX's iovec

using io_scatter_t = basic_io_scatter_t<void>;

template<typename T>
class io_async_scheduler_t
{
public:
	using type = typename std::remove_cvref_t<decltype(async_scheduler_type(*static_cast<T*>(nullptr)))>::type;
};

template<typename T>
class io_async_overlapped_t
{
public:
	using type = typename std::remove_cvref_t<decltype(async_overlapped_type(*static_cast<T*>(nullptr)))>::type;
};

template<typename T,bool end_test=false>
struct io_reserve_type_t
{
	explicit constexpr io_reserve_type_t() = default;
};
template<typename T,bool end_test=false>
inline constexpr io_reserve_type_t<T,end_test> io_reserve_type{};

template<typename T>
struct print_scatter_type_t
{
	explicit constexpr print_scatter_type_t() = default;
};

template<typename T>
inline constexpr print_scatter_type_t<T> print_scatter_type{};

}