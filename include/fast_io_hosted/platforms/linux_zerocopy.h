#pragma once
// To verify __NR_copy_file_range
namespace fast_io
{

template<std::integral ch_type>
struct basic_linux_zero_copy_entry
{
	int fd{-1};
};

template<std::integral ch_type>
inline basic_linux_zero_copy_entry<ch_type> zero_copy_in_handle(basic_posix_io_observer<ch_type> h)
{
	return {h.fd};
}
template<std::integral ch_type>
inline basic_linux_zero_copy_entry<ch_type> zero_copy_out_handle(basic_posix_io_observer<ch_type> h)
{
	return {h.fd};
}

template<std::integral ch_type>
inline basic_linux_zero_copy_entry<ch_type> zero_copy_in_handle(basic_posix_pipe<ch_type>& h)
{
	return {h.in().fd};
}
template<std::integral ch_type>
inline basic_linux_zero_copy_entry<ch_type> zero_copy_out_handle(basic_posix_pipe<ch_type>& h)
{
	return {h.out().fd};
}

namespace details
{

inline std::uintmax_t linux_send_file_transmit_raw_impl(int out_fd,int in_fd)
{
	return raw_transmit_decay(posix_io_observer{out_fd},posix_io_observer{in_fd});
}

inline std::uintmax_t zero_copy_transmit_define_impl(int out_fd,int in_fd)
{
	using posix_ssize_t = std::make_signed_t<std::size_t>;
	constexpr std::size_t transmit_a_round{static_cast<std::size_t>(std::numeric_limits<posix_ssize_t>::max())};
	std::uintmax_t total_bytes{};
	for(;;)
	{
		posix_ssize_t diff{
#if defined(__NR_sendfile64)
		system_call<__NR_sendfile64,posix_ssize_t>
#else
		system_call<__NR_sendfile,posix_ssize_t>
#endif
		(out_fd,in_fd,nullptr,transmit_a_round)};
		if(linux_system_call_fails(diff))[[unlikely]]
			return total_bytes+linux_send_file_transmit_raw_impl(out_fd,in_fd);
		else if(diff==0)[[unlikely]]
			break;
		total_bytes+=static_cast<std::size_t>(diff);
	}
	return total_bytes;

}

}

template<std::integral ch_type1,std::integral ch_type2>
inline std::uintmax_t zero_copy_transmit_define(io_alias_t,basic_linux_zero_copy_entry<ch_type1> outs,basic_linux_zero_copy_entry<ch_type2> ins)
{
	return details::zero_copy_transmit_define_impl(outs.fd,ins.fd)/sizeof(ch_type2);
}

}
