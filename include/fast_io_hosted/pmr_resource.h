#pragma once

namespace fast_io
{

namespace details
{

template<buffer_output_stream output>
inline constexpr std::pair<void*,std::size_t> convert_to_pr_output(output&& outp)
{
	auto bptr{obuffer_begin(outp)};
	auto cptr{obuffer_curr(outp)};
	auto eptr{obuffer_end(outp)};
	if(bptr==eptr)[[unlikely]]
	{
		if constexpr(requires()
		{
			obuffer_initialize(outp);
		})
		{
			obuffer_initialize(outp);
			cptr=obuffer_curr(outp);
			eptr=obuffer_end(outp);
		}
		else
			return {};
	}
	else if constexpr(flush_output_stream<output>)
	{
		std::size_t total_size(eptr-bptr);
		std::size_t remain_size(eptr-cptr);
		if(remain_size*4<total_size)
		{
			flush(outp);
			cptr=obuffer_curr(outp);
			eptr=obuffer_end(outp);
		}
	}
	return {cptr,static_cast<std::size_t>(eptr-cptr)*sizeof(*cptr)};
}

template<capacity_available_buffer_input_stream input>
inline constexpr std::pair<void*,std::size_t> convert_to_pr_input(input&& in)
{
	auto bptr{ibuffer_begin(in)};
	auto cptr{ibuffer_curr(in)};
	auto eptr{ibuffer_end(in)};
	auto cap_ptr(ibuffer_cap(in));
	if(cptr-bptr<cap_ptr-eptr)[[likely]]
		return {eptr,(cap_ptr-eptr)*sizeof(*cptr)};
	else
		return {bptr,static_cast<std::size_t>(cptr-bptr)*sizeof(*cptr)};
}

template<stream input>
inline constexpr std::pair<void*,std::size_t> convert_to_pr_io(input&& inp)
{
	if constexpr(capacity_available_buffer_input_stream<input>)
		return convert_to_pr_input(std::forward<input>(inp));
	else if constexpr(buffer_output_stream<input>)
		return convert_to_pr_output(std::forward<input>(inp));
	else
		return {};
}

}

class monotonic_output_buffer_resource:public std::pmr::monotonic_buffer_resource
{
	monotonic_output_buffer_resource(std::pair<void*,std::size_t> pr,std::pmr::memory_resource* upst):std::pmr::monotonic_buffer_resource(pr.first,pr.second,upst){}
public:
	explicit monotonic_output_buffer_resource(std::pmr::memory_resource* upst):std::pmr::monotonic_buffer_resource(upst){}
	monotonic_output_buffer_resource() noexcept=default;
	template<buffer_output_stream output>
	requires (std::is_lvalue_reference_v<output>||(std::is_rvalue_reference_v<output>&&std::is_trivially_copyable_v<std::remove_cvref_t<output>>))
	explicit monotonic_output_buffer_resource(output&& outp,std::pmr::memory_resource* upst=std::pmr::get_default_resource()):
		monotonic_output_buffer_resource(details::convert_to_pr_output(std::forward<output>(outp)),upst){}
};

class monotonic_input_buffer_resource:public std::pmr::monotonic_buffer_resource
{
	monotonic_input_buffer_resource(std::pair<void*,std::size_t> pr,std::pmr::memory_resource* upst):std::pmr::monotonic_buffer_resource(pr.first,pr.second,upst){}
public:
	explicit monotonic_input_buffer_resource(std::pmr::memory_resource* upst):std::pmr::monotonic_buffer_resource(upst){}
	monotonic_input_buffer_resource() noexcept=default;
	template<capacity_available_buffer_input_stream input>
	requires (std::is_lvalue_reference_v<input>||(std::is_rvalue_reference_v<input>&&std::is_trivially_copyable_v<std::remove_cvref_t<input>>))
	explicit monotonic_input_buffer_resource(input&& outp,std::pmr::memory_resource* upst=std::pmr::get_default_resource()):
		monotonic_input_buffer_resource(details::convert_to_pr_input(std::forward<input>(outp)),upst){}
};

class monotonic_io_buffer_resource:public std::pmr::monotonic_buffer_resource
{
	monotonic_io_buffer_resource(std::pair<void*,std::size_t> pr,std::pmr::memory_resource* upst):std::pmr::monotonic_buffer_resource(pr.first,pr.second,upst){}
public:
	explicit monotonic_io_buffer_resource(std::pmr::memory_resource* upst):std::pmr::monotonic_buffer_resource(upst){}
	monotonic_io_buffer_resource() noexcept=default;
	template<stream stm>
	requires (((capacity_available_buffer_input_stream<stm>||buffer_output_stream<stm>)&&!(capacity_available_buffer_input_stream<stm>&&buffer_output_stream<stm>))&&
	(std::is_lvalue_reference_v<stm>||(std::is_rvalue_reference_v<stm>&&std::is_trivially_copyable_v<std::remove_cvref_t<stm>>)))
	explicit monotonic_io_buffer_resource(stm&& outp,std::pmr::memory_resource* upst=std::pmr::get_default_resource()):
		monotonic_io_buffer_resource(details::convert_to_pr_io(std::forward<stm>(outp)),upst){}
};

}