#pragma once

namespace fast_io
{
enum class scan_context_status_code :char8_t
{
	success = 0, need_more_data = 1, lexical_error = 2, syntax_error = 3
};

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
	{ scan_context_define(scan_context<char_type, contiguous_only>, begin, end, t).iter }->std::same_as<char_type const*>;
	{ scan_context_define(scan_context<char_type, contiguous_only>, begin, end, t).code }->std::convertible_to<scan_context_status_code>;
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

[[noreturn]] inline void throw_posix_error(int);
namespace details
{

template<buffer_input_stream input,typename T>
[[nodiscard]] inline constexpr bool scan_single_status_impl(input in,T& state_machine)
{
	for(;state_machine.code==scan_context_status_code::need_more_data;)
	{
		underflow(in);
		auto curr{ibuffer_curr(in)};
		auto end{ibuffer_end(in)};
		state_machine(curr,end);
		if(state_machine.code==scan_context_status_code::success)[[likely]]
		{
			ibuffer_set_curr(in, state_machine.iter);
			return true;
		}
	}
	throw_posix_error(EINVAL);
}

template<buffer_input_stream input,typename T>
[[nodiscard]] inline constexpr bool scan_single_impl(input in,T arg)
{
	using char_type = typename input::char_type;
	auto curr{ibuffer_curr(in)};
	auto end{ibuffer_end(in)};
#if 0
	if constexpr(contiguous_input_stream<input>)
	{

	}
	else
	{
#endif
		if constexpr(scanable_skipping<T>)
		{
			for(;(curr=scan_skip_define(scan_skip_type<T>,curr,end))==end;)
			{
				underflow(in);
				curr=ibuffer_curr(in);
				end=ibuffer_end(in);
			}
		}
		auto state_machine{scan_context_define(scan_context<false>,curr,end,arg)};
		if(state_machine.code != scan_context_status_code::success)[[likely]]
			return scan_single_status_impl(in,state_machine);
		ibuffer_set_curr(in, state_machine.iter);
		return true;
#if 0
	}
#endif
}

}

template<input_stream input,typename... Args>
[[nodiscard]] inline constexpr bool scan_freestanding_decay(input in,Args... args)
{
	if constexpr(mutex_stream<input>)
	{
		details::lock_guard lg{in};
		return scan_freestanding_decay(in.unlocked_handle(),args...);
	}
	else if constexpr(status_input_stream<input>)
		return scan_status_define(in,args...);
	else
		return (details::scan_single_impl(in,args)||...);
}

template<input_stream input,typename ...Args>
[[nodiscard]] inline constexpr bool scan_freestanding(input&& in,Args&& ...args)
{
	return scan_freestanding_decay(io_ref(in),io_scan_alias<typename std::remove_cvref_t<input>::char_type>(args)...);
}

}