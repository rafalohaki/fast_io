#pragma once

namespace fast_io
{

template<typename output>
class ocrtp{};

template<typename output>
requires requires(output& outp)
{
	outp.ocrtp_handle();
}
constexpr decltype(auto) ocrtp_handle(ocrtp<output>& outp)
{
	return static_cast<output&>(outp).ocrtp_handle();
}

template<typename output>
requires requires(ocrtp<output>& outp)
{
	ocrtp_handle(outp);
}
using ocrtp_handle_type=std::remove_cvref_t<decltype(ocrtp_handle(*reinterpret_cast<output*>(0)))>;

template<typename output,typename... Args>
requires output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) write(ocrtp<output>& out,Args&& ...args)
{
	return write(ocrtp_handle(out),std::forward<Args>(args)...);
}

template<typename output,typename... Args>
requires character_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) put(ocrtp<output>& out,Args&& ...args)
{
	return put(ocrtp_handle(out),std::forward<Args>(args)...);
}

template<typename output,typename... Args>
requires buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) overflow(ocrtp<output>& out,Args&& ...args)
{
	return overflow(ocrtp_handle(out),std::forward<Args>(args)...);
}

template<typename output>
requires buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) obuffer_begin(ocrtp<output>& out)
{
	return obuffer_begin(ocrtp_handle(out));
}

template<typename output>
requires buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) obuffer_curr(ocrtp<output>& out)
{
	return obuffer_curr(ocrtp_handle(out));
}

template<typename output>
requires buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) obuffer_end(ocrtp<output>& out)
{
	return obuffer_end(ocrtp_handle(out));
}

template<typename output,typename... Args>
requires buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) obuffer_set_curr(icrtp<output>& out,Args&& ...args)
{
	return obuffer_set_curr(ocrtp_handle(out),std::forward<Args>(args)...);
}

template<typename output>
requires zero_copy_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) zero_copy_out_handle(ocrtp<output>& out)
{
	return zero_copy_out_handle(ocrtp_handle(out));
}

template<typename output>
requires memory_map_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) memory_map_out_handle(ocrtp<output>& out)
{
	return memory_map_out_handle(ocrtp_handle(out));
}

template<typename output,typename... Args>
requires dynamic_buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) ogrow(ocrtp<output>& out,Args&& ...args)
{
	return ogrow(ocrtp_handle(out),std::forward<Args>(args)...);
}
template<typename output,typename... Args>
requires dynamic_buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) otakeover(ocrtp<output>& out,Args&& ...args)
{
	return otakeover(ocrtp_handle(out),std::forward<Args>(args)...);
}
template<typename output,typename... Args>
requires dynamic_buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) oallocator(ocrtp<output>& out,Args&& ...args)
{
	return oallocator(ocrtp_handle(out),std::forward<Args>(args)...);
}

template<typename output,typename... Args>
requires dynamic_buffer_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) ocan_takeover(ocrtp<output>& out,Args&& ...args)
{
	return ocan_takeover(ocrtp_handle(out),std::forward<Args>(args)...);
}

template<typename output,typename... Args>
requires scatter_output_stream<ocrtp_handle_type<output>>
constexpr decltype(auto) scatter_write(ocrtp<output>& out,Args&& ...args)
{
	return scatter_write(ocrtp_handle(out),std::forward<Args>(args)...);
}

template<typename output>
requires requires(output outp)
{
	obuffer_initialize(outp);
}
constexpr void obuffer_initialize(ocrtp<output>& out)
{
	obuffer_initialize(out);
}

}