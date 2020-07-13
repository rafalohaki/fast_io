#pragma once

//fast_io_concept.h allows you define your device and type without compilation time penalty

#include<concepts>
#include<cstddef>
#include<type_traits>
#include<memory>
#include<span>			//for span
#include<iterator>		//for iterator concepts

namespace fast_io
{

template<typename T>
struct basic_io_scatter_t
{
	T const* base;
	std::size_t len;
	inline operator basic_io_scatter_t<void>() const noexcept
	{
		return {base,len*sizeof(T)};
	}
};

//should be binary compatible with POSIX's iovec

using io_scatter_t = basic_io_scatter_t<void>;

namespace details
{

template<typename T>
concept stream_char_type_requirement = requires(T&&)
{
	typename std::remove_cvref_t<T>::char_type;
};

template<typename T>
concept input_stream_impl = stream_char_type_requirement<T>&&requires(T&& in,typename std::remove_cvref_t<T>::char_type* b)
{
	read(in,b,b);
};

template<typename T>
concept output_stream_impl = stream_char_type_requirement<T>&&requires(T&& out,typename std::remove_cvref_t<T>::char_type const* b)
{
	{write(out,b,b)};
};

template<typename T>
concept mutex_stream_impl = requires(T&& t)
{
	typename std::remove_cvref_t<T>::lock_guard_type;
	mutex(t);
	unlocked_handle(t);
};

template<typename T>
concept character_input_stream_impl = requires(T&& in)
{
	igenerator(in);
};

template<typename T>
concept character_output_stream_impl = requires(T&& out,typename std::remove_cvref_t<T>::char_type ch)
{
	put(out,ch);
};

template<typename T>
concept random_access_stream_impl = requires(T& t)
{
	seek(t,5);
};

template<typename T>
concept buffer_input_stream_impl = requires(T&& in)
{
	{ibuffer_begin(in)}->std::convertible_to<typename std::remove_cvref_t<T>::char_type const*>;
	ibuffer_set_curr(in,ibuffer_curr(in));
	{ibuffer_end(in)}->std::convertible_to<typename std::remove_cvref_t<T>::char_type const*>;
	{underflow(in)}->std::convertible_to<bool>;
};

template<typename T>
concept contiguous_buffer_input_stream_impl = requires(T&& in)
{
	underflow_forever_false(in);
};

template<typename T>
concept refill_buffer_input_stream_impl = requires(T&& in)
{
	{irefill(in)}->std::convertible_to<bool>;
};

template<typename T>
concept reserve_output_stream_impl = requires(T& out,std::size_t n)
{
	orelease(out,oreserve(out,n));
};

template<typename T>
concept buffer_output_stream_impl = requires(T&& out,typename std::remove_cvref_t<T>::char_type ch)
{
	{obuffer_begin(out)}->std::convertible_to<typename std::remove_cvref_t<T>::char_type*>;
	{obuffer_end(out)}->std::convertible_to<typename std::remove_cvref_t<T>::char_type*>;
	obuffer_set_curr(out,obuffer_curr(out));
	overflow(out,ch);
};
template<typename T>
concept maybe_buffer_output_stream_impl = requires(T& out)
{
	{obuffer_is_active(out)}->std::convertible_to<bool>;
};
template<typename T>
concept flush_output_stream_impl = requires(T& out)
{
	flush(out);
};

template<typename T>
concept fill_nc_output_stream_impl = requires(T&& out,std::size_t n,typename std::remove_cvref_t<T>::char_type ch)
{
	fill_nc_define(out,n,ch);
};

template<typename T>
concept dynamic_buffer_output_stream_impl = requires(T&& out,std::size_t size,typename std::remove_cvref_t<T>::char_type* ptr)
{
	oallocator(out);
	ogrow(out,size);
	otakeover(out,ptr,ptr,ptr);
	{ocan_takeover(out)}->std::convertible_to<bool>;
};

template<typename T>
concept zero_copy_input_stream_impl = requires(T& in)
{
	zero_copy_in_handle(in);
};

template<typename T>
concept zero_copy_output_stream_impl = requires(T& out)
{
	zero_copy_out_handle(out);
};

template<typename T>
concept redirect_stream_impl = requires(T& h)
{
	redirect_handle(h);
};

template<typename T>
concept memory_map_input_stream_impl = requires(T& in)
{
	memory_map_in_handle(in);
};

template<typename T>
concept memory_map_output_stream_impl = requires(T& out)
{
	memory_map_out_handle(out);
};

template<typename T>
concept status_stream_impl = requires(T&& stm)
{
	typename std::remove_cvref_t<T>::char_type::status_type;
};


template<typename T>
concept scatter_input_stream_impl = requires(T& in,std::span<io_scatter_t const> sp)
{
	{scatter_read(in,sp)}->std::same_as<std::size_t>;
};

template<typename T>
concept scatter_output_stream_impl = requires(T& out,std::span<io_scatter_t const> sp)
{
	scatter_write(out,sp);
};

}


template<typename T>
concept stream = details::input_stream_impl<T>||details::output_stream_impl<T>;

template<typename T>
concept redirect_stream = stream<T>&&details::redirect_stream_impl<T>;

template<typename T>
concept input_stream = stream<T>&&details::input_stream_impl<T>;

template<typename T>
concept output_stream = stream<T>&&details::output_stream_impl<T>;

template<typename T>
concept mutex_stream = stream<T>&&details::mutex_stream_impl<T>;

template<typename T>
concept mutex_input_stream = mutex_stream<T>&&input_stream<T>;

template<typename T>
concept mutex_output_stream = mutex_stream<T>&&output_stream<T>;

template<typename T>
concept random_access_stream = stream<T>&&details::random_access_stream_impl<T>;

template<typename T>
concept io_stream = input_stream<T>&&output_stream<T>;

template<typename T>
concept character_input_stream = input_stream<T>&&details::character_input_stream_impl<T>;

template<typename T>
concept character_output_stream = output_stream<T>&&details::character_output_stream_impl<T>;

template<typename T>
concept character_io_stream = character_input_stream<T>&&character_output_stream<T>;

template<typename T>
concept flush_output_stream = output_stream<T>&&details::flush_output_stream_impl<T>;

template<typename T>
concept mutex_io_stream = mutex_input_stream<T>&&mutex_output_stream<T>;

template<typename T>
concept reserve_output_stream = output_stream<T>&&details::reserve_output_stream_impl<T>;

template<typename T>
concept buffer_input_stream = input_stream<T>&&details::buffer_input_stream_impl<T>;

template<typename T>
concept refill_buffer_input_stream = buffer_input_stream<T>&&details::refill_buffer_input_stream_impl<T>;

template<typename T>
concept contiguous_buffer_input_stream = buffer_input_stream<T>&&details::contiguous_buffer_input_stream_impl<T>;

template<typename T>
concept buffer_output_stream = output_stream<T>&&details::buffer_output_stream_impl<T>;

template<typename T>
concept fill_nc_output_stream = output_stream<T>&&details::fill_nc_output_stream_impl<T>;

//Unfortunately, FILE* is a mess here. We have to support this to prevent the operating system not buffering anything
template<typename T>
concept maybe_buffer_output_stream = buffer_output_stream<T>&&details::maybe_buffer_output_stream_impl<T>;

template<typename T>
concept buffer_io_stream = buffer_input_stream<T>&&buffer_output_stream<T>&&io_stream<T>;

template<typename T>
concept dynamic_buffer_output_stream = buffer_output_stream<T>&&details::dynamic_buffer_output_stream_impl<T>;

template<typename T>
concept fixed_buffer_output_stream = buffer_output_stream<T>&&!dynamic_buffer_output_stream<T>;

template<typename T>
concept zero_copy_input_stream = input_stream<T>&&details::zero_copy_input_stream_impl<T>;

template<typename T>
concept zero_copy_output_stream = output_stream<T>&&details::zero_copy_output_stream_impl<T>;

template<typename T>
concept zero_copy_io_stream = zero_copy_input_stream<T>&&zero_copy_output_stream<T>;

template<typename T>
concept memory_map_input_stream = input_stream<T>&&details::memory_map_input_stream_impl<T>;

template<typename T>
concept memory_map_output_stream = output_stream<T>&&details::memory_map_output_stream_impl<T>;

template<typename T>
concept memory_map_io_stream = memory_map_input_stream<T>&&memory_map_output_stream<T>;

template<typename T>
concept scatter_input_stream = input_stream<T>&&details::scatter_input_stream_impl<T>;

template<typename T>
concept scatter_output_stream = output_stream<T>&&details::scatter_output_stream_impl<T>;

template<typename T>
concept secure_clear_requirement_stream = stream<T>&&requires(T stm)
{
	require_secure_clear(stm);
};

template<typename T>
concept status_output_stream = output_stream<T>&&details::status_stream_impl<T>&&requires(T out)
{
	print_status_define(out);
	println_status_define(out);
};

template<typename T>
concept status_input_stream = input_stream<T>&&details::status_stream_impl<T>&&requires(T in)
{
	scan_status_define<false>(in);
};

template<typename input,typename T>
concept space_scanable=input_stream<input>&&requires(input& in,T&& t)
{
	space_scan_define(in,std::forward<T>(t));
};

template<typename input,typename T>
concept scanable=input_stream<input>&&requires(input& in,T&& t)
{
	scan_define(in,std::forward<T>(t));
};

template<typename input,typename T>
concept receiveable=input_stream<input>&&requires(input& in,T&& t)
{
	receive_define(in,std::forward<T>(t));
};

template<typename output,typename T>
concept sendable=output_stream<output>&&requires(output& out,T&& t)
{
	send_define(out,std::forward<T>(t));
};

template<typename T>
struct print_reserve_type_t
{
	explicit constexpr print_reserve_type_t() = default;
};
template<typename T>
inline constexpr print_reserve_type_t<T> print_reserve_type{};

template<typename T>
struct print_scatter_type_t
{
	explicit constexpr print_scatter_type_t() = default;
};
template<typename T>
inline constexpr print_scatter_type_t<T> print_scatter_type{};

template<typename T>
concept reserve_printable=requires(T&& t,char8_t* ptr)
{
	{print_reserve_size(print_reserve_type<std::remove_cvref_t<T>>)}->std::convertible_to<std::size_t>;
	{print_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,ptr,t)}->std::same_as<char8_t*>;
};

template<typename T>
concept reverse_reserve_printable=reserve_printable<T>&&requires(T&& t,char8_t* ptr)
{
	{print_reverse_reserve_define(print_reserve_type<std::remove_cvref_t<T>>,ptr,t)}->std::same_as<char8_t*>;
};

template<typename T>
concept reserve_print_testable=requires(T&& t)
{
	{print_reserve_test<static_cast<std::size_t>(0)>(print_reserve_type<std::remove_cvref_t<T>>,std::forward<T>(t))}->std::convertible_to<bool>;
}&&reserve_printable<T>;

template<typename output,typename T>
concept printable=output_stream<output>&&requires(output& out,T&& t)
{
	print_define(out,std::forward<T>(t));
};

template<typename char_type,typename T>
concept scatter_printable=requires(char_type ch,T&& t)
{
	{print_scatter_define(print_scatter_type<char_type>,std::forward<T>(t))}->std::convertible_to<io_scatter_t>;
};

template<typename char_type,typename T>
concept scatter_type_printable=scatter_printable<char_type,T>&&requires(char_type ch,T&& t)
{
	{print_scatter_define(print_scatter_type<char_type>,std::forward<T>(t))}->std::convertible_to<basic_io_scatter_t<char_type>>;
};

template<typename input,typename T>
concept general_scanable=space_scanable<input,T>||scanable<input,T>;

template<typename output,typename T>
concept general_printable=reserve_printable<T>||printable<output,T>;

template<typename io_device,typename... Args>
concept io_controllable=requires(io_device& device,Args&& ...args)
{
	io_control(device,std::forward<Args>(args)...);
};

struct manip_tag_t{};

template<typename T>
concept manipulator = std::same_as<typename T::manip_tag,manip_tag_t>&&requires(T t)
{
	{t.reference};
};

}
