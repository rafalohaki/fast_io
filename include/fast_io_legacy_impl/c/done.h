#pragma once

namespace fast_io
{

namespace details
{

inline std::size_t c_fwrite_unlocked_impl(void const* __restrict begin,std::size_t type_size,std::size_t count,std::FILE* __restrict fp)
{
#if defined(_POSIX_C_SOURCE)
	clearerr_unlocked(fp);
#elif defined(__MINGW32__)
	fp->_flag&=~0x0020;
#else
	clearerr(fp);
#endif
	std::size_t written_count{
#if defined(_MSC_VER)||defined(_UCRT)
	_fwrite_nolock
#elif defined(_POSIX_SOURCE)
	fwrite_unlocked
#else
	fwrite
#endif
	(begin,type_size,count,fp)};
	auto errn{errno};
	if(
#if defined(_POSIX_C_SOURCE)
	ferror_unlocked(fp)
#elif defined(__MINGW32__)
	fp->_flag&0x0020
#else
	ferror(fp)
#endif
	)
		throw_posix_error(errn);
	return written_count;
}

inline std::size_t c_fread_unlocked_impl(void* __restrict begin,std::size_t type_size,std::size_t count,std::FILE* __restrict fp)
{
#if defined(_POSIX_C_SOURCE)
	clearerr_unlocked(fp);
#elif defined(__MINGW32__)
	fp->_flag&=~0x0020;
#else
	clearerr(fp);
#endif
	std::size_t read_count{
#if defined(_MSC_VER)||defined(_UCRT)
	_fread_nolock
#elif defined(_POSIX_SOURCE)
	fread_unlocked
#else
	fread
#endif
	(begin,type_size,count,fp)};
	auto errn{errno};
	if(
#if defined(_POSIX_C_SOURCE)
	ferror_unlocked(fp)
#elif defined(__MINGW32__)
	fp->_flag&0x0020
#else
	ferror(fp)
#endif
	)
		throw_posix_error(errn);
	return read_count;
}



template<std::integral char_type>
inline std::size_t c_io_write_impl(basic_c_io_observer_unlocked<char_type> cfhd,char_type const* begin,char_type const* end)
{
	std::size_t const count(end-begin);
	if constexpr((std::same_as<char_type,char>&&buffer_io_stream<c_io_observer_unlocked>)
	||(std::same_as<char_type,wchar_t>&&buffer_io_stream<wc_io_observer_unlocked>)
	||(std::same_as<char_type,char8_t>&&buffer_output_stream<u8c_io_observer_unlocked>)
	||(std::same_as<char_type,char16_t>&&buffer_output_stream<u16c_io_observer_unlocked>)
	||(std::same_as<char_type,char32_t>&&buffer_output_stream<u32c_io_observer_unlocked>)
)
	{
		auto curr{obuffer_curr(cfhd)};
		auto ed{obuffer_end(cfhd)};
		if(curr+count<ed)[[likely]]
		{
			non_overlapped_copy_n(begin,count,curr);
			obuffer_set_curr(cfhd,curr+count);
			return count;
		}
	}
	return c_fwrite_unlocked_impl(begin,sizeof(char_type),count,cfhd.fp);
}

template<std::integral char_type>
inline std::size_t c_io_read_impl(basic_c_io_observer_unlocked<char_type> cfhd,char_type* begin,char_type* end)
{
	std::size_t count(end-begin);
	if constexpr((std::same_as<char_type,char>&&buffer_io_stream<c_io_observer_unlocked>)
	||(std::same_as<char_type,wchar_t>&&buffer_io_stream<wc_io_observer_unlocked>)
	||(std::same_as<char_type,char8_t>&&buffer_io_stream<u8c_io_observer_unlocked>)
	||(std::same_as<char_type,char16_t>&&buffer_io_stream<u16c_io_observer_unlocked>)
	||(std::same_as<char_type,char32_t>&&buffer_io_stream<u32c_io_observer_unlocked>)
)
	{
		auto curr{ibuffer_curr(cfhd)};
		auto ed{ibuffer_end(cfhd)};
		if(curr+count<ed)[[likely]]
		{
			non_overlapped_copy_n(curr,count,begin);
			ibuffer_set_curr(cfhd,curr+count);
			return count;
		}
	}
	return c_fread_unlocked_impl(begin,sizeof(char_type),count,cfhd.fp);
}

}

template<std::integral T,std::contiguous_iterator Iter>
requires (std::same_as<T,std::iter_value_t<Iter>>||std::same_as<T,char>)
inline Iter write(basic_c_io_observer_unlocked<T> cfhd,Iter cbegin,Iter cend)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,T>)
		return cbegin+details::c_io_write_impl(cfhd,std::to_address(cbegin),std::to_address(cend));
	else
		return cbegin+details::c_io_write_impl(cfhd,reinterpret_cast<char const*>(std::to_address(cbegin)),reinterpret_cast<char const*>(std::to_address(cend)))/sizeof(*cbegin);
}

template<std::integral T,std::contiguous_iterator Iter>
requires (std::same_as<T,std::iter_value_t<Iter>>||std::same_as<T,char>)
[[nodiscard]] inline Iter read(basic_c_io_observer_unlocked<T> cfhd,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,T>)
		return begin+details::c_io_read_impl(cfhd,std::to_address(begin),std::to_address(end));
	else
		return begin+details::c_io_read_impl(cfhd,reinterpret_cast<char*>(std::to_address(begin)),reinterpret_cast<char*>(std::to_address(end)))/sizeof(*begin);
}


/*
static_assert(buffer_io_stream<c_io_observer_unlocked>);
static_assert(buffer_io_stream<wc_io_observer_unlocked>);
static_assert(!buffer_io_stream<basic_c_io_observer_unlocked<char8_t>>);
*/
}
