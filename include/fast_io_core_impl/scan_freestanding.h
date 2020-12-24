#pragma once

namespace fast_io
{

template<bool contiguous_only = false>
struct scan_context_t
{
	explicit inline constexpr scan_context_t() noexcept = default;
};

template<bool contiguous_only = false>
inline constexpr scan_context_t<contiguous_only> scan_context;

template<typename char_type, typename T, bool contiguous_only = false>
concept context_scanable = requires(char_type const* begin, char_type const* end, T t)
{
	{ scan_context_define(scan_context<contiguous_only>, begin, end, t).iter }->std::convertible_to<char_type const*>;
	{ scan_context_define(scan_context<contiguous_only>, begin, end, t).code }->std::convertible_to<std::errc>;
};

template<typename T>
struct scan_skip_type_t
{
	explicit inline constexpr scan_skip_type_t() noexcept = default;
};
template<typename T>
inline constexpr scan_skip_type_t<T> scan_skip_type{};

template<typename T>
concept scanable_skipping = requires(scan_skip_type_t<T> t, char8_t const* begin, char8_t const* end)
{
	{ scan_skip_define(scan_skip_type<T>, begin, end) }->std::convertible_to<char8_t const*>;
};


template<std::integral char_type,typename T>
[[nodiscard]] inline constexpr auto io_scan_alias(T&& t)
{
	if constexpr(alias_type_scanable<char_type,std::remove_reference_t<T>>)
		return scan_alias_define(io_alias_type<char_type>,std::forward<T>(t));
	else if constexpr(alias_scanable<std::remove_reference_t<T>>)
		return scan_alias_define(io_alias,std::forward<T>(t));
	else if constexpr(manipulator<T>)
		return t;
	else
		return parameter<std::remove_reference_t<T>&>{t};
}

[[noreturn]] inline void throw_scan_error(std::errc);
namespace details
{

template<typename T>
struct unget_temp_buffer
{
	using char_type = typename T::char_type;
	[[no_unique_address]] T input;
	typename T::char_type buffer;
	char8_t pos{};
	char8_t pos_end{};
	constexpr unget_temp_buffer(T in):input(in){}
	unget_temp_buffer(unget_temp_buffer const&)=delete;
	unget_temp_buffer& operator=(unget_temp_buffer const&)=delete;
	constexpr ~unget_temp_buffer() requires requires(typename T::char_type ch)
	{
		try_unget(input,ch);
	}
	{
		if(pos!=pos_end)
			try_unget(input,buffer);
	}
	constexpr ~unget_temp_buffer() = default;
};

template<input_stream T,std::input_or_output_iterator Iter>
inline constexpr Iter read(unget_temp_buffer<T>& in,Iter begin, Iter end)
{
	if(begin==end)
		return begin;
	if(in.pos!=in.pos_end)
	{
		*begin=in.buffer;
		++begin;
	}
	if(begin!=end)
		begin=read(in.input,begin,end);	
	in.pos=in.pos_end={};
	return begin;
}

template<typename T>
inline constexpr auto ibuffer_begin(unget_temp_buffer<T>& in) noexcept
{
	return std::addressof(in.buffer);
}

template<typename T>
inline constexpr auto ibuffer_curr(unget_temp_buffer<T>& in) noexcept
{
	return std::addressof(in.buffer)+in.pos;
}

template<typename T>
inline constexpr auto ibuffer_end(unget_temp_buffer<T>& in) noexcept
{
	return std::addressof(in.buffer)+in.pos_end;
}

template<typename T>
inline constexpr auto ibuffer_set_curr(unget_temp_buffer<T>& in,typename T::char_type* ptr) noexcept
{
	in.pos=ptr-std::addressof(in.buffer);
}

template<typename T>
inline constexpr bool underflow(unget_temp_buffer<T>& in)
{
	if constexpr(requires()
	{
		{try_get(in.input)}->std::convertible_to<std::pair<typename T::char_type,bool>>;
	})
	{
		auto ret{try_get(in.input)};
		in.buffer=ret.first;
		in.pos={};
		in.pos_end=ret.second;
		return ret.second;
	}
	else
	{
		bool not_eof{read(in.input,std::addressof(in.buffer),std::addressof(in.buffer)+1)!=std::addressof(in.buffer)};
		in.pos={};
		in.pos_end=not_eof;
		return not_eof;
	}
}

template<buffer_input_stream input,typename T,typename P>
inline constexpr bool scan_single_status_impl(input in,T& state_machine,P arg)
{
	for(;state_machine.code==std::errc::resource_unavailable_try_again;)
	{
		if(!underflow(in))
			return state_machine.test_eof(arg);
		auto curr{ibuffer_curr(in)};
		auto end{ibuffer_end(in)};
		state_machine(curr,end,arg);
		ibuffer_set_curr(in, state_machine.iter);
		if(state_machine.code==std::errc{})[[likely]]
			return true;
	}
	throw_scan_error(state_machine.code);
}
template<input_stream input,typename T>
requires (scanable<input,T>||context_scanable<typename input::char_type,T,false>
||(contiguous_input_stream<input>&&context_scanable<typename input::char_type,T,true>))
[[nodiscard]] inline constexpr bool scan_single_impl(input in,T arg)
{
	using char_type = typename input::char_type;
	if constexpr(buffer_input_stream<input>)
	{
		auto curr{ibuffer_curr(in)};
		auto end{ibuffer_end(in)};
		if constexpr(contiguous_input_stream<input>)
		{
			if constexpr(scanable_skipping<T>)
				curr=scan_skip_define(scan_skip_type<T>,curr,end);
			if constexpr(context_scanable<char_type,T,true>||context_scanable<char_type,T,false>)
			{
				auto state_machine{scan_context_define(scan_context<context_scanable<char_type,T,true>>,curr,end,arg)};
				ibuffer_set_curr(in, state_machine.iter);
				if(state_machine!=std::errc{})
				{
					if(state_machine.code==std::errc::resource_unavailable_try_again)
						return false;
					throw_scan_error(state_machine.code);
				}
				return true;
			}
			else if constexpr(scanable<input,T>)
				return scan_define(in,arg);
		}
		else
		{
			if constexpr(scanable_skipping<T>)
			{
				for(;(curr=scan_skip_define(scan_skip_type<T>,curr,end))==end;)
				{
					underflow(in);
					curr=ibuffer_curr(in);
					end=ibuffer_end(in);
				}
			}
			if constexpr(context_scanable<char_type,T,false>)
			{
				auto state_machine{scan_context_define(scan_context<false>,curr,end,arg)};
				ibuffer_set_curr(in, state_machine.iter);
				if(state_machine.code!=std::errc{})[[likely]]
					return scan_single_status_impl(in,state_machine,arg);
				return true;
			}
			else if constexpr(scanable<input,T>)
				return scan_define(in,arg);
		}
	}
	else
	{
		if constexpr(context_scanable<char_type,T,false>)
		{
			unget_temp_buffer in_buffer(io_ref(in));
			return scan_single_impl(io_ref(in_buffer),arg);
		}
		else if constexpr(scanable<input,T>)
			return scan_define(in,arg);
	}
}
}

template<typename input,typename... Args>
requires (status_input_stream<input>||input_stream<input>)
[[nodiscard]] inline constexpr bool scan_freestanding_decay(input in,Args... args)
{
	if constexpr(status_input_stream<input>)
		return scan_status_define(in,args...);
	else if constexpr(mutex_stream<input>)
	{
		details::lock_guard lg{in};
		return scan_freestanding_decay(in.unlocked_handle(),args...);
	}
	else if constexpr(buffer_input_stream<input>)
		return (details::scan_single_impl(in,args)&&...);
	else
	{
		details::unget_temp_buffer in_buffer(io_ref(in));
		return scan_freestanding_decay(io_ref(in_buffer),args...);
	}
}

template<typename input,typename ...Args>
requires (status_input_stream<input>||input_stream<input>)
[[nodiscard("scan does not require checking return value but scan_freestanding requires")]] inline constexpr bool scan_freestanding(input&& in,Args&& ...args)
{
	return scan_freestanding_decay(io_ref(in),io_scan_alias<typename std::remove_cvref_t<input>::char_type>(args)...);
}

}