#pragma once

/*

Provide a general emulation of FILE*'s internal implementation with fgetc_unlocked and ungetc_unlocked.

It is probably slow. However, it is a general case when we have not supported hacks of libcs on your platform.

BTW, musl libc and BSD libc does not provide general buffering for wide characters. This should cover that gap.
*/

namespace fast_io
{

namespace details
{
template<std::integral char_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,wchar_t>)
class basic_c_io_observer_unlocked_igenerator_iterator
{
public:
	FILE* fp{};
	char_type char_temp{};
	bool eof{};
};

template<std::integral char_type>
inline auto fgetc_unlocked_impl(std::FILE* fp)
{
	if constexpr(std::same_as<char_type,char>)
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		return _fgetc_nolock(fp);
#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE || _BSD_SOURCE || _SVID_SOURCE
		return fgetc_unlocked(fp);
#else
		return fgetc(fp);
#endif
	}
	else
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		return _fgetwc_nolock(fp);
#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE || _BSD_SOURCE || _SVID_SOURCE
		return fgetwc_unlocked(fp);
#else
		return fgetwc(fp);
#endif
	}
}

template<std::integral char_type,std::integral int_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,wchar_t>)
inline constexpr bool equals_to_eof_macro(int_type inv)
{
	if constexpr(std::same_as<char_type,char>)
		return inv==EOF;
	else
		return inv==WEOF;
}

template<std::integral char_type>
inline auto ungetc_unlocked_impl(std::conditional_t<std::same_as<char_type,char>,int,wint_t> ch,std::FILE* fp)
{
	if constexpr(std::same_as<char_type,char>)
	{
#if defined(_MSC_VER)
		return _ungetc_nolock(ch,fp);
#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE || _BSD_SOURCE || _SVID_SOURCE
		return ungetc_unlocked(ch,fp);
#else
		return ungetc(ch,fp);
#endif
	}
	else
	{
#if defined(_MSC_VER)
		return _ungetwc_nolock(ch,fp);
#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE || _BSD_SOURCE || _SVID_SOURCE
		return ungetwc_unlocked(ch,fp);
#else
		return ungetwc(ch,fp);
#endif
	}
}

inline void ferror_throw_ex_impl(FILE* fp)
{
	if(
#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE || _BSD_SOURCE || _SVID_SOURCE
	ferror_unlocked(fp)
#else
	ferror(fp)
#endif
	)
		throw_posix_error();
}


template<std::integral char_type>
struct basic_c_io_observer_unlocked_igenerator
{
	FILE* fp{};
};
template<std::integral char_type>
inline basic_c_io_observer_unlocked_igenerator_iterator<char_type> begin(basic_c_io_observer_unlocked_igenerator<char_type> gen)
{
	auto ch{fgetc_unlocked_impl<char_type>(gen.fp)};
	bool eof{equals_to_eof_macro<char_type>(ch)};
	if(eof)[[unlikely]]
		ferror_throw_ex_impl(gen.fp);
	else
		ungetc_unlocked_impl<char_type>(ch,gen.fp);
	return {gen.fp,static_cast<char_type>(static_cast<std::make_unsigned_t<char_type>>(ch)),eof};
}

template<std::integral char_type>
inline std::default_sentinel_t end(basic_c_io_observer_unlocked_igenerator<char_type>)
{
	return {};
}

template<std::integral char_type>
inline constexpr auto operator*(basic_c_io_observer_unlocked_igenerator_iterator<char_type> gen)
{
	return gen.char_temp;
}

template<std::integral char_type>
inline basic_c_io_observer_unlocked_igenerator_iterator<char_type>& operator++(basic_c_io_observer_unlocked_igenerator_iterator<char_type>& gen)
{
	{
		auto ch{fgetc_unlocked_impl<char_type>(gen.fp)};
		bool eof{equals_to_eof_macro<char_type>(ch)};
		if(eof)[[unlikely]]
		{
			ferror_throw_ex_impl(gen.fp);
			gen.eof=true;
			return gen;
		}
	}
	auto ch{fgetc_unlocked_impl<char_type>(gen.fp)};
	bool eof{equals_to_eof_macro<char_type>(ch)};
	if(eof)[[unlikely]]
		ferror_throw_ex_impl(gen.fp);
	else
		ungetc_unlocked_impl<char_type>(ch,gen.fp);
	gen.char_temp=ch;
	gen.eof=eof;
	return gen;
}

template<std::integral char_type>
inline void operator++(basic_c_io_observer_unlocked_igenerator_iterator<char_type>& gen,int)
{
	static_cast<void>(operator++(gen));
}

template<std::integral char_type>
inline constexpr bool operator!=(basic_c_io_observer_unlocked_igenerator_iterator<char_type> a, std::default_sentinel_t)
{
	return !a.eof;
}

template<std::integral char_type>
inline constexpr bool operator==(basic_c_io_observer_unlocked_igenerator_iterator<char_type> a, std::default_sentinel_t)
{
	return a.eof;
}

template<std::integral char_type>
inline constexpr bool operator!=(std::default_sentinel_t,basic_c_io_observer_unlocked_igenerator_iterator<char_type> a)
{
	return !a.eof;
}

template<std::integral char_type>
inline constexpr bool operator==(std::default_sentinel_t,basic_c_io_observer_unlocked_igenerator_iterator<char_type> a)
{
	return a.eof;
}

}

template<std::integral char_type>
requires ((!buffer_input_stream<basic_c_io_observer_unlocked<char_type>>)&&(std::same_as<char_type,char>||std::same_as<char_type,wchar_t>))
inline constexpr details::basic_c_io_observer_unlocked_igenerator<char_type> igenerator(basic_c_io_observer_unlocked<char_type> in)
{
	return {in.fp};
}

}