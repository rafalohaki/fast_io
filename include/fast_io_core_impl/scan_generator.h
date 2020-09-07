#pragma once
namespace fast_io
{

template<typename function_type,typename char_type>
concept scan_generatable = std::integral<char_type>&&requires(function_type func,char_type const* it)
{
	{func.search(it,it)}->std::convertible_to<char_type const*>;
	{func.set_data(it,it)};
	{func.get_data()};
};

namespace details
{

struct empty{};

}

template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
struct scan_iterator
{
	using char_type = typename input::char_type;
	input *ptr;
	[[no_unique_address]] std::conditional_t<contiguous_input_stream<input>,details::empty,internal_temporary_buffer<typename input::char_type>*> pbuffer;
	[[no_unique_address]] Func function;
};

template<buffer_input_stream input,scan_generatable<typename input::char_type> Fun>
struct scan_generator
{
	input *ptr;
	[[no_unique_address]] std::conditional_t<contiguous_input_stream<input>,details::empty,internal_temporary_buffer<typename input::char_type>> buffer;
	constexpr scan_generator(input* pt):ptr(pt){}
	scan_generator(scan_generator const&)=delete;
	scan_generator& operator=(scan_generator const&)=delete;
};
namespace details
{

template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr scan_iterator<input,Func>& scan_iterator_transmit_impl
	(scan_iterator<input,Func>& line_it,typename input::char_type const* bg,typename input::char_type const* ed)
{
	auto& buffer{*line_it.pbuffer};
	obuffer_set_curr(buffer,obuffer_begin(buffer));
	write(buffer,bg,ed);
	while(underflow(*line_it.ptr))
	{
		auto bg{ibuffer_begin(*line_it.ptr)};
		auto ed{ibuffer_end(*line_it.ptr)};
		auto it{line_it.function.search(bg,ed)};
		write(*line_it.pbuffer,bg,it);
		if(it!=ed)
		{
			ibuffer_set_curr(*line_it.ptr,it+1);
			line_it.function.set_data(obuffer_begin(buffer),obuffer_curr(buffer));
			return line_it;
		}
	}
	auto ob_bg{obuffer_begin(buffer)};
	auto ob_cr{obuffer_curr(buffer)};
	if(ob_bg==ob_cr)
		line_it.ptr=nullptr;
	line_it.function.set_data(ob_bg,ob_cr);
	return line_it;
}
}


template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr scan_iterator<input,Func>& operator++(scan_iterator<input,Func>& line_it)
{
	auto bg{ibuffer_curr(*line_it.ptr)};
	auto ed{ibuffer_end(*line_it.ptr)};
	auto it{line_it.function.search(bg,ed)};
	if constexpr(!contiguous_input_stream<input>)
	{
		if(it==ed)[[unlikely]]
			return details::scan_iterator_transmit_impl(line_it,bg,ed);
	}
	line_it.function.set_data(bg,it);
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

template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr decltype(auto) operator*(scan_iterator<input,Func>& a)
{
	return a.function.get_data();
}


template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr bool operator!=(scan_iterator<input,Func> const& a, std::default_sentinel_t)
{
	return a.ptr;
}

template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr bool operator==(scan_iterator<input,Func> const& a, std::default_sentinel_t)
{
	return a.ptr==nullptr;
}

template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr bool operator!=(std::default_sentinel_t,scan_iterator<input,Func> const& a)
{
	return a.ptr;
}

template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr bool operator==(std::default_sentinel_t,scan_iterator<input,Func> const& a)
{
	return a.ptr==nullptr;
}


template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr scan_iterator<input,Func> begin(scan_generator<input,Func>& a)
{
	if constexpr(contiguous_input_stream<input>)
	{
		scan_iterator<input,Func> in{a.ptr};
		++in;
		return in;
	}
	else
	{
		scan_iterator<input,Func> in{a.ptr,std::addressof(a.buffer)};
		++in;
		return in;
	}
}

template<buffer_input_stream input,scan_generatable<typename input::char_type> Func>
inline constexpr std::default_sentinel_t end(scan_generator<input,Func>&)
{
	return {};
}
template<std::integral char_type,char_type seperator=u8'\n'>
struct scan_line_seperator
{
	std::basic_string_view<char_type> data;
	template<std::contiguous_iterator Iter>
	requires std::same_as<std::iter_value_t<Iter>,char_type>
	static constexpr Iter search(Iter bg,Iter ed) noexcept
	{
		return std::find(bg,ed,seperator);
	}
	template<std::contiguous_iterator Iter>
	requires std::same_as<std::iter_value_t<Iter>,char_type>	
	constexpr void set_data(Iter bg,Iter ed) noexcept
	{
		data=std::basic_string_view<char_type>(bg,ed);
	}
	constexpr std::basic_string_view<char_type> const& get_data() const noexcept
	{
		return data;
	}
};

template<buffer_input_stream input>
inline constexpr scan_generator<std::remove_cvref_t<input>,scan_line_seperator<typename input::char_type,u8'\n'>> line_generator(input&& in)
{
	return {std::forward<input>(in)};
}


}
