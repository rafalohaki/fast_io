#pragma once
#if (defined(_WIN32)&&!defined(__WINE__)) || defined(__CYGWIN__)
#include"rtl_gen_random.h"
#endif
#include"posix_dev_urandom.h"
#if (defined(__linux__) && defined(__NR_getrandom)) || __has_include(<sys/random.h>)
#include"linux_getrandom.h"
#endif

namespace fast_io
{

template<std::integral char_type>
using basic_native_white_hole =
#if defined(_WIN32)&&!defined(__WINE__)
basic_rtl_gen_random<char_type>;
#elif (defined(__linux__) && defined(__NR_getrandom)) || __has_include(<sys/random.h>)
basic_linux_getrandom<char_type>;
#else
posix_dev_urandom<basic_native_file<char_type>>;
#endif

template<std::integral char_type>
using basic_ibuf_white_hole = basic_io_buffer<basic_native_white_hole<char_type>,buffer_mode::in|buffer_mode::secure_clear>;

using native_white_hole = basic_native_white_hole<char>;
using ibuf_white_hole = basic_ibuf_white_hole<char>;

using wnative_white_hole = basic_native_white_hole<wchar_t>;
using wibuf_white_hole = basic_ibuf_white_hole<wchar_t>;

using u8native_white_hole = basic_native_white_hole<char8_t>;
using u8ibuf_white_hole = basic_ibuf_white_hole<char8_t>;

using u16native_white_hole = basic_native_white_hole<char16_t>;
using u16ibuf_white_hole = basic_ibuf_white_hole<char16_t>;

using u32native_white_hole = basic_native_white_hole<char32_t>;
using u32ibuf_white_hole = basic_ibuf_white_hole<char32_t>;

namespace details
{

template<typename T>
concept has_entroy_method_impl = requires(T handle)
{
	{handle.entropy()}->std::same_as<double>;
};

}

template<input_stream handletype>
requires std::same_as<std::remove_cvref_t<typename handletype::char_type>,char>
struct basic_white_hole_engine
{
	using handle_type = handletype;
	using result_type = std::size_t;
	handle_type handle;
	static inline constexpr result_type min() noexcept
	{
		return 0;
	}
	static inline constexpr result_type max() noexcept
	{
		return SIZE_MAX;
	}
	inline constexpr double entropy() const noexcept
	{
		if constexpr(::fast_io::details::has_entroy_method_impl<handletype>)
			return handle.entropy();
		else
			return static_cast<double>(sizeof(std::size_t));
	}
	inline constexpr result_type operator()()
	{
		result_type type;
		if(read(handle,__builtin_addressof(type),__builtin_addressof(type)+1)!=__builtin_addressof(type)+1)
			throw_posix_error(EINVAL);
		return type;
	}
};

using native_white_hole_engine = basic_white_hole_engine<native_white_hole>;
using ibuf_white_hole_engine = basic_white_hole_engine<ibuf_white_hole>;

}
