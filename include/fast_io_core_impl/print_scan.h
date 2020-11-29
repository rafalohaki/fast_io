#pragma once

namespace fast_io
{

namespace details
{

template<bool space,buffer_output_stream output,character_input_stream input,typename T>
inline constexpr bool scan_with_space_temporary_buffer_impl(output& buffer,input& in,T&& t)
{
	using no_cvref = std::remove_cvref_t<T>;
	if(!scan_reserve_transmit(io_reserve_type<typename std::remove_cvref_t<input>::char_type,no_cvref>,buffer,in))
		return false;
	if constexpr(space)
	{
		space_scan_reserve_define(io_reserve_type<typename std::remove_cvref_t<input>::char_type,no_cvref>,obuffer_begin(buffer),obuffer_curr(buffer),std::forward<T>(t));
		return true;
	}
	else
	{
		scan_reserve_define(io_reserve_type<typename std::remove_cvref_t<input>::char_type,no_cvref>,obuffer_begin(buffer),obuffer_curr(buffer),std::forward<T>(t));
		return true;
	}
}

template<bool space=true,character_input_stream input,typename T>
inline constexpr bool scan_with_space_temporary_buffer(input& in,T&& t)
{
	using no_cvref_input = std::remove_cvref_t<input>;
	using no_cvref = std::remove_cvref_t<T>;
	if constexpr(space&&space_scanable<no_cvref_input,T>)
	{
		space_scan_define(in,std::forward<T>(t));
		return true;
	}
	else if constexpr(!space&&!scanable<no_cvref_input,T>)
	{
		return scan_define(in,std::forward<T>(t));
	}
	else if constexpr(reserve_size_scanable<typename no_cvref_input::char_type,no_cvref>)
	{
		using char_type = typename std::remove_cvref_t<input>::char_type;
		constexpr std::size_t reserve_size{scan_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<T>>)};
		std::array<char_type,reserve_size> array;
		fast_io::ospan<char_type,reserve_size,true> osp(array);
		return scan_with_space_temporary_buffer_impl<space>(osp,in,std::forward<T>(t));
	}
	else
	{
		internal_temporary_buffer<typename std::remove_cvref_t<input>::char_type> buffer;
		return scan_with_space_temporary_buffer_impl<space>(buffer,in,std::forward<T>(t));
	}

}

template<character_input_stream input,typename T>
requires (general_scanable<input,T>||general_reserve_scanable<T,internal_temporary_buffer<typename std::remove_cvref_t<input>::char_type>,input>)
inline constexpr auto scan_with_space(input &in,T&& t)
{
	using no_cvref = std::remove_cvref_t<T>;
	constexpr bool not_contiguous{!contiguous_input_stream<input>};
	if constexpr(space_scanable<input,T>||reserve_space_scanable<T,internal_temporary_buffer<typename std::remove_cvref_t<input>::char_type>,input>)
	{
		if(!skip_space(in))
			return false;
		if constexpr(reserve_space_scanable<T,internal_temporary_buffer<typename std::remove_cvref_t<input>::char_type>,input>)
		{
			if constexpr(buffer_input_stream<input>)
			{
				if constexpr(reserve_scan_avoidance<input>&&space_scanable<input,T>)
					space_scan_define(in,std::forward<T>(t));
				else
				{
					auto curr{ibuffer_curr(in)};
					auto ed{ibuffer_end(in)};
					auto res{space_scan_reserve_define(io_reserve_type<typename std::remove_cvref_t<input>::char_type,no_cvref,not_contiguous>,curr,ed,std::forward<T>(t))};
					if constexpr(not_contiguous)
					{
						if(res==ed)[[unlikely]]
							return scan_with_space_temporary_buffer(in,std::forward<T>(t));
					}
					ibuffer_set_curr(in,res);
				}
			}
			else
				return scan_with_space_temporary_buffer(in,std::forward<T>(t));
		}
		else
		{
			space_scan_define(in,std::forward<T>(t));
		}
		return true;
	}
	else
	{
		if constexpr(reserve_scanable<T,internal_temporary_buffer<typename std::remove_cvref_t<input>::char_type>,input>)
		{
			using no_cvref = std::remove_cvref_t<T>;
			if constexpr(buffer_input_stream<input>)
			{
				if constexpr(reserve_scan_avoidance<input>&&scanable<input,T>)
					space_scan_define(in,std::forward<T>(t));
				else
				{
					auto curr{ibuffer_curr(in)};
					auto ed{ibuffer_end(in)};
					auto res{scan_reserve_define(io_reserve_type<typename std::remove_cvref_t<input>::char_type,no_cvref,!not_contiguous>,curr,ed,std::forward<T>(t))};
					if(!res.first)
						return false;
					if constexpr(not_contiguous)
					{
						if(res==ed)[[unlikely]]
							return scan_with_space_temporary_buffer<false>(in,std::forward<T>(t));
					}
					ibuffer_set_curr(in,res);
					return true;
				}
			}
			else
				return scan_with_space_temporary_buffer<false>(in,std::forward<T>(t));
		}
		else
		{
			return scan_define(in,std::forward<T>(t));
		}
	}
}

template<character_input_stream input,typename T>
inline constexpr void scan_with_ex(input &in,T&& t)
{
	if constexpr(std::same_as<decltype(scan_with_space(in,std::forward<T>(t))),void>)
		scan_with_space(in,std::forward<T>(t));
	else
	{
		if(!scan_with_space(in,t))[[unlikely]]
#ifdef __cpp_exceptions
			throw eof();
#else
			fast_terminate();
#endif
	}
}

template<bool report_eof,character_input_stream input,typename ...Args>
requires((general_scanable<input,Args>||
	general_reserve_scanable<Args,internal_temporary_buffer<typename input::char_type>,input>)&&...)
inline constexpr auto normal_scan(input &ip,Args&& ...args)
{
	if constexpr(report_eof)
		return (static_cast<std::size_t>(scan_with_space(ip,std::forward<Args>(args)))+...);
	else
		(scan_with_ex(ip,std::forward<Args>(args)),...);
}

}

template<std::integral char_type,typename T>
requires reserve_printable<char_type,T>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manip::line<T>>)
{
	constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<T>>)+1};
	return sz;
}

template<std::integral char_type,std::random_access_iterator raiter,typename T,typename U>
requires reserve_printable<char_type,T>
inline constexpr raiter print_reserve_define(io_reserve_type_t<char_type,manip::line<T>>,raiter start,U a)
{
	auto it{print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,start,a.reference)};
	*it=u8'\n';
	return ++it;
}
namespace details
{
template<bool report_eof,typename T,typename... Args>
concept test_normal_scan = requires(T t,Args&& ...args)
{
	details::normal_scan<report_eof>(t,std::forward<Args>(args)...);
};
}

template<bool report_eof=false,input_stream input_t,typename ...Args>
inline constexpr auto scan(input_t &&in,Args&& ...args)
{
	using input = std::remove_cvref_t<input_t>;
	if constexpr(mutex_stream<input>)
	{
		details::lock_guard lg{in};
		return scan<report_eof>(in.unlocked_handle(),std::forward<Args>(args)...);
	}
#if 0
	else if constexpr(status_input_stream<input>)
		return scan_status_define<report_eof>(in,std::forward<Args>(args)...);
#endif
	else if constexpr(!character_input_stream<input>)
	{
		single_character_input_buffer<std::remove_cvref_t<input>> scib{in};
		return scan<report_eof>(scib,std::forward<Args>(args)...);
	}
	else if constexpr(!details::test_normal_scan<report_eof,std::remove_cvref_t<input>,Args&&...>)
	{
		static_assert(!character_input_stream<input>,
		"\n\n\tThe type is not defined for scanning. Please consider defining as with scan_define or space_scan_define.\n");
	}
	else
		return details::normal_scan<report_eof>(in,std::forward<Args>(args)...);

}

}
