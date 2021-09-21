#pragma once

namespace fast_io::details
{

template<typename output>
requires (output_stream<output>)
struct temporary_buffer
{
	using char_type = typename output::char_type;
	static inline constexpr std::size_t buffer_size{512};
	std::size_t pos{};
	char_type buffer[buffer_size];
	output out;
};

template<typename output>
inline constexpr typename output::char_type* obuffer_begin(temporary_buffer<output>& out) noexcept
{
	return out.buffer;
}

template<typename output>
inline constexpr typename output::char_type* obuffer_curr(temporary_buffer<output>& out) noexcept
{
	return out.buffer+out.pos;
}

template<typename output>
inline constexpr typename output::char_type* obuffer_end(temporary_buffer<output>& out) noexcept
{
	return out.buffer+temporary_buffer<output>::buffer_size;
}

template<typename output>
inline constexpr void obuffer_set_curr(temporary_buffer<output>& out,typename output::char_type* ptr) noexcept
{
	out.pos=static_cast<std::size_t>(ptr-out.buffer);
}

template<typename output>
inline constexpr void flush(temporary_buffer<output>& out)
{
	auto start{out.buffer};
	write(out.out,start,start+temporary_buffer<output>::buffer_size);
}

template<typename output>
#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline constexpr void obuffer_overflow(temporary_buffer<output>& out,typename output::char_type ch)
{
	auto start{out.buffer};
	write(out.out,start,start+temporary_buffer<output>::buffer_size);
	*start=ch;
	out.pos=1;
}

template<typename output>
#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline constexpr void temporary_buffer_write_cold_path(temporary_buffer<output>& out,
	typename output::char_type const* first,
	typename output::char_type const* last)
{
	auto start{out.buffer};
	std::size_t const pos{out.pos};
	std::size_t const remain_space{static_cast<std::size_t>(temporary_buffer<output>::buffer_size-pos)};
	non_overlapped_copy_n(first,remain_space,start+pos);
	first+=remain_space;
	write(out.out,start,start+temporary_buffer<output>::buffer_size);
	out.pos=0;
	std::size_t const new_diff{static_cast<std::size_t>(last-first)};
	if(new_diff<temporary_buffer<output>::buffer_size)
	{
		non_overlapped_copy_n(first,new_diff,start);
		out.pos=new_diff;
	}
	else
	{
		write(out.out,first,last);	//write remaining characters
	}
}

template<typename output>
inline constexpr void write(temporary_buffer<output>& out,
	typename output::char_type const* first,
	typename output::char_type const* last)
{
	std::size_t const pos{out.pos};
	std::size_t const remain_space{temporary_buffer<output>::buffer_size-pos};
	std::size_t const ptr_diff{static_cast<std::size_t>(last-first)};
	if(remain_space<=ptr_diff)
#if __has_cpp_attribute(unlikely)
[[unlikely]]
#endif
		return temporary_buffer_write_cold_path(out,first,last);
	auto start{out.buffer};
	non_overlapped_copy_n(start,ptr_diff,start+pos);
	out.pos+=ptr_diff;
}

template<std::integral ch_type>
struct basic_virtual_output_device
{
	using char_type = ch_type;
	void* device{};
	void (*write_ptr)(void* device,ch_type const* first,ch_type const* last)={};
};

template<std::integral ch_type>
constexpr void write(basic_virtual_output_device<ch_type> device,ch_type const* first,ch_type const* last)
{
	(device.write_ptr)(device.device,first,last);
}

template<std::integral ch_type>
using basic_virtual_temporary_buffer = temporary_buffer<basic_virtual_output_device<ch_type>>;

template<typename output>
inline basic_virtual_output_device<typename output::char_type> construct_virtual_device_from_output_stream(output out)
{
	using char_type = typename output::char_type;
	constexpr bool without_indirection{(sizeof(output)<=sizeof(void*)) && std::is_trivially_copyable_v<output>};
	void* out_ptr;
	if constexpr(without_indirection)
		my_memcpy(__builtin_addressof(out_ptr),__builtin_addressof(out),sizeof(output));
	else
		out_ptr=__builtin_addressof(out);
	return {
	.device=out_ptr,
	.write_ptr=[](void* dev,char_type const* first,char_type const* last)
	{
		if constexpr(without_indirection)
		{
			output pout;
			my_memcpy(__builtin_addressof(pout),__builtin_addressof(dev),sizeof(output));
			write(pout,first,last);
		}
		else
		{
			write(*reinterpret_cast<output*>(dev),first,last);
		}
	}};
}

}
