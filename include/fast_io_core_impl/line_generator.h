#pragma once

namespace fast_io
{

namespace details
{

struct empty{};

template<buffer_input_stream input>
struct input_buffer_line_iterator
{
	using char_type = typename input::char_type;
	input *ptr;
	[[no_unique_address]] std::conditional_t<contiguous_input_stream<input>,empty,internal_temporary_buffer<typename input::char_type>*> pbuffer;
	std::basic_string_view<char_type> data;
};

template<buffer_input_stream input>
struct input_buffer_line_generator
{
	input *ptr;
	[[no_unique_address]] std::conditional_t<contiguous_input_stream<input>,empty,internal_temporary_buffer<typename input::char_type>> buffer;
	constexpr explicit input_buffer_line_generator(input* pt):ptr(pt){}
	input_buffer_line_generator(input_buffer_line_generator const&)=delete;
	input_buffer_line_generator& operator=(input_buffer_line_generator const&)=delete;
};

template<buffer_input_stream input>
inline constexpr input_buffer_line_iterator<input>& next_line_transmit
	(input_buffer_line_iterator<input>& line_it,typename input::char_type const* bg,typename input::char_type const* ed)
{
	constexpr typename input::char_type ch=u8'\n';
	auto& buffer{*line_it.pbuffer};
	obuffer_set_curr(buffer,obuffer_begin(buffer));
	write(buffer,bg,ed);
	while(underflow(*line_it.ptr))
	{
		auto bg{ibuffer_begin(*line_it.ptr)};
		auto ed{ibuffer_end(*line_it.ptr)};
		auto it{std::find(bg,ed,ch)};
		write(*line_it.pbuffer,bg,it);
		if(it!=ed)
		{
			ibuffer_set_curr(*line_it.ptr,it+1);
			line_it.data=std::basic_string_view<typename input::char_type>(obuffer_begin(buffer),obuffer_curr(buffer));
			return line_it;
		}
	}
	auto ob_bg{obuffer_begin(buffer)};
	auto ob_cr{obuffer_curr(buffer)};
	if(ob_bg==ob_cr)
		line_it.ptr=nullptr;
	line_it.data=std::basic_string_view<typename input::char_type>(ob_bg,ob_cr);
	return line_it;
}

template<buffer_input_stream input>
inline constexpr input_buffer_line_iterator<input>& operator++(input_buffer_line_iterator<input>& line_it)
{
	constexpr typename input::char_type ch=u8'\n';
	auto bg{ibuffer_curr(*line_it.ptr)};
	auto ed{ibuffer_end(*line_it.ptr)};
	auto it{std::find(bg,ed,ch)};
	if constexpr(!contiguous_input_stream<input>)
	{
		if(it==ed)[[unlikely]]
			return next_line_transmit(line_it,bg,ed);
	}
	line_it.data=std::basic_string_view<typename input::char_type>(bg,it);
	if constexpr(contiguous_input_stream<input>)
	{
		if(it==ed)
		{
			ibuffer_set_curr(*line_it.ptr,it);
			if(bg==it)
				line_it.ptr=nullptr;
		}
		else
			ibuffer_set_curr(*line_it.ptr,it+1);
	}
	else
	{
		ibuffer_set_curr(*line_it.ptr,it+1);
	}
	return line_it;
}

template<buffer_input_stream input>
inline constexpr std::basic_string_view<typename input::char_type> operator*(input_buffer_line_iterator<input>& a)
{
	return a.data;
}


template<buffer_input_stream input>
inline constexpr bool operator!=(input_buffer_line_iterator<input> const& a, std::default_sentinel_t)
{
	return a.ptr;
}

template<buffer_input_stream input>
inline constexpr bool operator==(input_buffer_line_iterator<input> const& a, std::default_sentinel_t)
{
	return a.ptr==nullptr;
}

template<buffer_input_stream input>
inline constexpr bool operator!=(std::default_sentinel_t,input_buffer_line_iterator<input> const& a)
{
	return a.ptr;
}

template<buffer_input_stream input>
inline constexpr bool operator==(std::default_sentinel_t,input_buffer_line_iterator<input> const& a)
{
	return a.ptr==nullptr;
}


template<buffer_input_stream input>
inline constexpr input_buffer_line_iterator<input> begin(input_buffer_line_generator<input>& a)
{
	if constexpr(contiguous_input_stream<input>)
	{
		input_buffer_line_iterator<input> in{a.ptr};
		++in;
		return in;
	}
	else
	{
		input_buffer_line_iterator<input> in{a.ptr,std::addressof(a.buffer)};
		++in;
		return in;
	}
}

template<buffer_input_stream input>
inline constexpr std::default_sentinel_t end(input_buffer_line_generator<input>&)
{
	return {};
}

}

template<buffer_input_stream input>
inline constexpr details::input_buffer_line_generator<input> line_generator(input& in)
{
	return details::input_buffer_line_generator<input>(std::addressof(in));
}

}
