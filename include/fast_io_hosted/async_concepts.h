#pragma once

namespace fast_io
{

namespace details
{

template<typename T>
concept async_input_stream_impl = stream_char_type_requirement<T>&&requires(T& in,typename T::char_type* b)
{
	async_read(in,b,b);
};

template<typename T>
concept async_output_stream_impl = stream_char_type_requirement<T>&&requires(T& out,typename T::char_type const* b)
{
	async_write(out,b,b);
};


template<typename T>
concept async_scatter_input_stream_impl = stream_char_type_requirement<T>&&requires(std::remove_cvref_t<T>::async_scheduler_type&& sched,T&& in,std::span<io_scatter_t const> sp)
{
	async_scatter_read(sched,in,sp);
};

template<typename T>
concept async_scatter_output_stream_impl = stream_char_type_requirement<T>&&requires(std::remove_cvref_t<T>::async_scheduler_type&& sched,T& out,std::span<io_scatter_t const> sp)
{
	async_scatter_write(sched,out,sp);
};

}

template<typename T>
concept async_input_stream = stream<T>&&details::async_input_stream_impl<T>;

template<typename T>
concept async_output_stream = stream<T>&&details::async_output_stream_impl<T>;

template<typename T>
concept async_scatter_input_stream = stream<T>&&details::async_input_stream_impl<T>;

template<typename T>
concept async_scatter_output_stream = stream<T>&&details::async_output_stream_impl<T>;

}
