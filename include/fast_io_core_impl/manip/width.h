#pragma once

namespace fast_io
{

namespace manip
{

template<typename T,std::integral char_type>
struct width
{
	using manip_tag = manip_tag_t;
	T reference;
	std::size_t width;
	bool left;
	char_type indent_character;
};

}

template<typename T,std::integral ch_type=char8_t>
requires (!manipulator<T>)
inline constexpr manip::width<T const&,ch_type> width(T const& t,std::size_t wd,bool left=false,ch_type indent_character=u8' ')
{
	return {t,wd,left,indent_character};
}

template<manipulator T,std::integral ch_type=char8_t>
inline constexpr manip::width<T,ch_type> width(T t,std::size_t wd,bool left=false,ch_type indent_character=u8' ')
{
	return {t,wd,left,indent_character};
}

template<typename T,std::integral ch_type=char8_t>
requires (!manipulator<T>)
inline constexpr manip::width<T const&,ch_type> left_width(T const& t,std::size_t wd,ch_type indent_character=u8' ')
{
	return {t,wd,true,indent_character};
}

template<manipulator T,std::integral ch_type=char8_t>
inline constexpr manip::width<T,ch_type> left_width(T t,std::size_t wd,ch_type indent_character=u8' ')
{
	return {t,wd,true,indent_character};
}

template<typename T,std::integral ch_type=char8_t>
requires (!manipulator<T>)
inline constexpr manip::width<T const&,ch_type> right_width(T const& t,std::size_t wd,ch_type indent_character=u8' ')
{
	return {t,wd,false,indent_character};
}

template<manipulator T,std::integral ch_type=char8_t>
inline constexpr manip::width<T,ch_type> right_width(T t,std::size_t wd,ch_type indent_character=u8' ')
{
	return {t,wd,false,indent_character};
}


namespace details
{
template<fast_io::output_stream output,typename T,std::integral ch_type>
inline constexpr void width_unhappy_case(output& out,manip::width<T,ch_type> wdt)
{
	internal_temporary_buffer<typename output::char_type> buffer;
	print_freestanding(buffer,wdt.reference);
	std::size_t const real_width(buffer.end_ptr-buffer.beg_ptr);
	if(real_width<wdt.width)
	{
		if(wdt.left)
		{
			if constexpr(buffer_output_stream<output>)
			{
				write(out,buffer.beg_ptr,buffer.end_ptr);
				print_freestanding(out,fill_nc(wdt.width-real_width,wdt.indent_character));
			}
			else
			{
				print_freestanding(buffer,fill_nc(wdt.width-real_width,wdt.indent_character));
				write(out,buffer.beg_ptr,buffer.end_ptr);
			}
		}
		else
		{
			if constexpr(buffer_output_stream<output>)
			{
				print_freestanding(out,fill_nc(wdt.width-real_width,wdt.indent_character));
				write(out,buffer.beg_ptr,buffer.end_ptr);
			}
			else
			{
				internal_temporary_buffer<typename output::char_type> buffer2;
				print_freestanding(buffer2,fill_nc(wdt.width-real_width,wdt.indent_character));
				write(buffer2,buffer.beg_ptr,buffer.end_ptr);
				write(out,buffer2.beg_ptr,buffer2.end_ptr);
			}
		}
	}
	else
		write(out,buffer.beg_ptr,buffer.end_ptr);
}
}
#if 0
template<fast_io::output_stream output,typename T,std::integral ch_type>
inline constexpr void print_define(output out,manip::width<T,ch_type> wdt)
{
	if constexpr(reserve_output_stream<output>&&reserve_printable<T>)
	{
		using no_cvref = std::remove_cvref_t<T>;
		constexpr std::size_t rsize{print_reserve_size(io_reserve_type<no_cvref>)};
		std::size_t size(wdt.width);
		if(size<rsize)
			size=rsize;
		if constexpr(reserve_output_stream<output>)
		{
			auto ptr{oreserve(out,size)};
			if constexpr(std::is_pointer_v<std::remove_cvref_t<decltype(oreserve(out,size))>>)
			{
				if(ptr==nullptr)[[unlikely]]
				{
					details::width_unhappy_case(out,wdt);
					return;
				}
			}
			auto printed(print_reserve_define(io_reserve_type<no_cvref>,ptr,wdt.reference));
			std::size_t printed_chars_count(printed-ptr);
			if(printed_chars_count<wdt.width)[[likely]]
			{
				if(wdt.left)
				{
					details::my_fill_n(printed,wdt.width-printed_chars_count,wdt.indent_character);
				}
				else
				{
					std::copy_backward(ptr,printed,ptr+wdt.width);
					details::my_fill_n(ptr,wdt.width-printed_chars_count,wdt.indent_character);
				}
				orelease(out,ptr+wdt.width);
			}
			else
				orelease(out,printed);
		}
	}
	else
		details::width_unhappy_case(out,wdt);
}
#endif
}