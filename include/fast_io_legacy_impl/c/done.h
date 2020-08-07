#pragma once

namespace fast_io
{

template<std::integral T,std::contiguous_iterator Iter>
inline void write(basic_c_io_observer_unlocked<T> cfhd,Iter begin,Iter end)
{
	std::size_t const count(end-begin);
	if constexpr((std::same_as<T,char>&&buffer_io_stream<c_io_observer_unlocked>)||(std::same_as<T,wchar_t>&&buffer_io_stream<wc_io_observer_unlocked>))
	{
		std::size_t const total_bytes_to_write(count*sizeof(*begin));
		std::size_t const total_count(total_bytes_to_write/sizeof(T));
		auto curr{obuffer_curr(cfhd)};
		auto ed{obuffer_end(cfhd)};
		if(curr+total_count<ed)[[likely]]
		{
			memcpy(curr,std::to_address(begin),total_bytes_to_write);
			obuffer_set_curr(cfhd,curr+total_count);
			return;
		}

#if defined(__WINNT__) || defined(_MSC_VER)
		if constexpr(maybe_buffer_output_stream<basic_c_io_observer_unlocked<T>>)
		{
			if(!obuffer_is_active(cfhd))[[unlikely]]
			{
				basic_posix_io_observer<T> piob(cfhd);
				if(_isatty(piob.fd))
				{
					write(piob,begin,end);
					return;
				}
			}
		}
#endif
	}
	if(
#if defined(_MSC_VER)||defined(_UCRT)
	_fwrite_nolock
#elif defined(_POSIX_SOURCE)
	fwrite_unlocked
#else
	fwrite
#endif
	(std::to_address(begin),sizeof(*begin),count,cfhd.native_handle())<count)
		throw_posix_error();
}

/*
static_assert(buffer_io_stream<c_io_observer_unlocked>);
static_assert(buffer_io_stream<wc_io_observer_unlocked>);
static_assert(!buffer_io_stream<basic_c_io_observer_unlocked<char8_t>>);
*/
}