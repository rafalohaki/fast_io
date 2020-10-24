#pragma once

namespace fast_io
{

template<typename T>
inline constexpr auto io_forward(T const& t) noexcept
{
	if constexpr(std::is_trivially_copyable_v<T>&&sizeof(t)<=alignof(std::max_align_t))		//predict the cost of passing by value
		return std::remove_cvref_t<T>(t);
	else
		return parameter<std::remove_reference_t<T>>{t};
}

template<stream stm>
requires (!value_based_stream<stm>)
struct io_reference_wrapper
{
	using char_type = typename stm::char_type;
	stm* ptr{};
	inline constexpr void lock() requires(mutex_stream<stm>)
	{
		ptr->lock();
	}
	inline constexpr void unlock() requires(mutex_stream<stm>)
	{
		ptr->unlock();
	}
	inline constexpr decltype(auto) unlocked_handle() requires(mutex_stream<stm>)
	{
		return ptr->unlocked_handle();
	}
};
template<stream strm>
inline constexpr auto io_ref(strm& stm) noexcept
{
	if constexpr(value_based_stream<std::remove_cvref_t<strm>>)
		return io_value_handle(stm);
	else
		return io_reference_wrapper<strm>{std::addressof(stm)};
}

template<stream stm>
constexpr io_reference_wrapper<stm> io_value_handle(io_reference_wrapper<stm> wrap) noexcept
{
	return wrap;
}

template<output_stream output,typename... Args>
constexpr decltype(auto) write(io_reference_wrapper<output> out,Args&& ...args)
{
	return write(*out.ptr,std::forward<Args>(args)...);
}

template<character_output_stream output,typename... Args>
constexpr decltype(auto) overflow(io_reference_wrapper<output> out,Args&& ...args)
{
	return overflow(*out.ptr,std::forward<Args>(args)...);
}

template<buffer_output_stream output>
constexpr decltype(auto) obuffer_begin(io_reference_wrapper<output> out)
{
	return obuffer_begin(*out.ptr);
}

template<buffer_output_stream output>
constexpr decltype(auto) obuffer_curr(io_reference_wrapper<output> out)
{
	return obuffer_curr(*out.ptr);
}

template<buffer_output_stream output>
constexpr decltype(auto) obuffer_end(io_reference_wrapper<output> out)
{
	return obuffer_end(*out.ptr);
}

template<zero_copy_output_stream output>
constexpr decltype(auto) zero_copy_out_handle(io_reference_wrapper<output> out)
{
	return zero_copy_out_handle(*out.ptr);
}

template<buffer_output_stream output,typename... Args>
constexpr void obuffer_set_curr(io_reference_wrapper<output> out,Args&& ...args)
{
	obuffer_set_curr(*out.ptr,std::forward<Args>(args)...);
}

template<dynamic_buffer_output_stream output,typename... Args>
constexpr decltype(auto) ogrow(io_reference_wrapper<output> out,Args&& ...args)
{
	return ogrow(*out.ptr,std::forward<Args>(args)...);
}
template<dynamic_buffer_output_stream output,typename... Args>
constexpr decltype(auto) otakeover(io_reference_wrapper<output> out,Args&& ...args)
{
	return otakeover(*out.ptr,std::forward<Args>(args)...);
}

template<dynamic_buffer_output_stream output,typename... Args>
constexpr decltype(auto) oallocator(io_reference_wrapper<output> out,Args&& ...args)
{
	return oallocator(*out.ptr,std::forward<Args>(args)...);
}

template<dynamic_buffer_output_stream output,typename... Args>
constexpr decltype(auto) ocan_takeover(io_reference_wrapper<output> out,Args&& ...args)
{
	return ocan_takeover(*out.ptr,std::forward<Args>(args)...);
}

template<scatter_output_stream output,typename... Args>
constexpr decltype(auto) scatter_write(io_reference_wrapper<output> out,Args&& ...args)
{
	return scatter_write(*out.ptr,std::forward<Args>(args)...);
}

template<buffer_output_stream output>
requires requires(output outp)
{
	obuffer_initialize(outp);
}
constexpr void obuffer_initialize(io_reference_wrapper<output> out)
{
	obuffer_initialize(*out.ptr);
}

template<input_stream input,typename... Args>
constexpr decltype(auto) read(io_reference_wrapper<input> in,Args&& ...args)
{
	return read(*in.ptr,std::forward<Args>(args)...);
}

template<character_input_stream input>
constexpr decltype(auto) igenerator(io_reference_wrapper<input> in)
{
	return igenerator(*in.ptr);
}

template<buffer_input_stream input>
constexpr decltype(auto) underflow(io_reference_wrapper<input> in)
{
	return underflow(*in.ptr);
}

template<capacity_available_buffer_input_stream input>
constexpr decltype(auto) ibuffer_cap(io_reference_wrapper<input> in)
{
	return ibuffer_cap(*in.ptr);
}

template<buffer_input_stream input,typename... Args>
constexpr decltype(auto) ibuffer_set_curr(io_reference_wrapper<input> in,Args&& ...args)
{
	return ibuffer_set_curr(*in.ptr,std::forward<Args>(args)...);
}

template<refill_buffer_input_stream input>
constexpr decltype(auto) irefill(io_reference_wrapper<input> in)
{
	return irefill(*in.ptr);
}
template<memory_map_input_stream input>
constexpr decltype(auto) memory_map_in_handle(io_reference_wrapper<input> in)
{
	return memory_map_in_handle(*in.ptr);
}

template<memory_map_output_stream output>
constexpr decltype(auto) memory_map_out_handle(io_reference_wrapper<output> out)
{
	return memory_map_out_handle(*out.ptr);
}

template<zero_copy_input_stream input>
constexpr decltype(auto) zero_copy_in_handle(io_reference_wrapper<input> in)
{
	return zero_copy_in_handle(*in.ptr);
}

template<scatter_input_stream input,typename... Args>
constexpr decltype(auto) scatter_read(io_reference_wrapper<input> in,Args&& ...args)
{
	return scatter_read(*in.ptr,std::forward<Args>(args)...);
}

template<buffer_input_stream input>
requires requires(input inp)
{
	ibuffer_initialize(inp);
}
constexpr void ibuffer_initialize(io_reference_wrapper<input> in)
{
	ibuffer_initialize(*in.ptr);
}

template<random_access_stream racs,typename... Args>
constexpr decltype(auto) seek(io_reference_wrapper<racs> rac,Args&& ...args)
{
	return seek(*rac.ptr,std::forward<Args>(args)...);
}

template<secure_clear_requirement_stream scrs>
constexpr decltype(auto) require_secure_clear(io_reference_wrapper<scrs> sc)
{
	return require_secure_clear(*sc.ptr);
}

template<contiguous_input_stream cis>
constexpr decltype(auto) underflow_forever_false(io_reference_wrapper<cis> ci)
{
	return underflow_forever_false(*ci.ptr);
}

template<contiguous_output_stream cos>
constexpr decltype(auto) overflow_never(io_reference_wrapper<cos> co)
{
	return overflow_never(*co.ptr);
}

template<maybe_buffer_output_stream output>
constexpr decltype(auto) obuffer_is_active(io_reference_wrapper<output> co)
{
	return obuffer_is_active(*co.ptr);
}

template<flush_output_stream output>
constexpr void flush(io_reference_wrapper<output> co)
{
	flush(*co.ptr);
}

template<fill_nc_output_stream T>
constexpr void fill_nc_define(io_reference_wrapper<T> fnc,std::size_t n,typename std::remove_cvref_t<T>::char_type ch)
{
	fill_nc_define(*fnc.ptr,n,ch);
}

template<status_output_stream output,typename... Args>
inline constexpr void print_status_define(io_reference_wrapper<output> out,Args&& ...args)
{
	print_status_define(*out.ptr,std::forward<Args>(args)...);
}

template<status_output_stream output,typename... Args>
inline constexpr void println_status_define(io_reference_wrapper<output> out,Args&& ...args)
{
	println_status_define(*out.ptr,std::forward<Args>(args)...);
}

template<bool rac=false,status_input_stream input,typename... Args>
inline constexpr void scan_status_define(io_reference_wrapper<input> out,Args&& ...args)
{
	scan_status_define<rac>(*out.ptr,std::forward<Args>(args)...);
}

}