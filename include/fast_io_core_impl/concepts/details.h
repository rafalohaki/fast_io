#pragma once

namespace fast_io::details
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


//async stream concepts

template<typename T>
concept async_input_stream_impl = stream_char_type_requirement<T>&&requires(T in,std::span<io_scatter_t const,1> sp)
{
	requires requires(typename std::remove_cvref_t<decltype(async_scheduler_type(in))>::type sch,
	typename std::remove_cvref_t<decltype(async_overlapped_type(in))>::type overlapped,std::ptrdiff_t offset)
	{
		async_scatter_read_callback(sch,in,sp,overlapped,offset);
	};
};

template<typename T>
concept async_output_stream_impl = stream_char_type_requirement<T>&&requires(T out,std::span<io_scatter_t const,1> sp)
{
	requires requires(typename std::remove_cvref_t<decltype(async_scheduler_type(out))>::type sch,
	typename std::remove_cvref_t<decltype(async_overlapped_type(out))>::type overlapped,std::ptrdiff_t offset)
	{
		async_scatter_write_callback(sch,out,sp,overlapped,offset);
	};
};

template<typename T>
concept async_scatter_input_stream_impl = requires(T in,std::span<io_scatter_t const> sp)
{
	requires requires(typename std::remove_cvref_t<decltype(async_scheduler_type(in))>::type sch,
	typename std::remove_cvref_t<decltype(async_overlapped_type(in))>::type overlapped,std::ptrdiff_t offset)
	{
		async_scatter_read_callback(sch,in,sp,overlapped,offset);
	};
};

template<typename T>
concept async_scatter_output_stream_impl = requires(T out,std::span<io_scatter_t const> sp)
{
	requires requires(typename std::remove_cvref_t<decltype(async_scheduler_type(out))>::type sch,
	typename std::remove_cvref_t<decltype(async_overlapped_type(out))>::type overlapped,std::ptrdiff_t offset)
	{
		async_scatter_write_callback(sch,out,sp,overlapped,offset);
	};
};

}
