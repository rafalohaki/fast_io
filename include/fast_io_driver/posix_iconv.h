#pragma once
#include"../fast_io_freestanding.h"
#include<iconv.h>

namespace fast_io
{

class posix_iconv_io_observer
{
public:
	using native_handle_type = iconv_t;
	iconv_t cd{std::bit_cast<iconv_t>(static_cast<uintptr_t>(-1))};

	constexpr auto& native_handle() noexcept
	{
		return cd;
	}
	constexpr auto& native_handle() const noexcept
	{
		return cd;
	}
	constexpr iconv_t release() noexcept
	{
		auto temp{cd};
		cd = std::bit_cast<iconv_t>(static_cast<uintptr_t>(-1));
		return temp;
	}
};

class posix_iconv_file:public posix_iconv_io_observer
{
public:
	using native_handle_type = iconv_t;
	constexpr posix_iconv_file() noexcept = default;
	constexpr posix_iconv_file(iconv_t icv) noexcept : posix_iconv_io_observer{icv}{}

	posix_iconv_file(char const* tocode,char const* fromcode):posix_iconv_io_observer{iconv_open(tocode,fromcode)}
	{
		if(this->cd==std::bit_cast<iconv_t>(static_cast<uintptr_t>(-1)))
			throw_posix_error();
	}
	posix_iconv_file(posix_iconv_file const&)=delete;
	posix_iconv_file& operator=(posix_iconv_file const&)=delete;
	constexpr posix_iconv_file(posix_iconv_file&& other) noexcept:posix_iconv_io_observer{other.release()}{}	
	posix_iconv_file& operator=(posix_iconv_file&& other) noexcept
	{
		if(std::addressof(other)==this)[[unlikely]]
			return *this;
		iconv_close(this->cd);
		this->cd=other.release();
		return *this;
	}
	void close()
	{
		if(iconv_close(this->cd)==-1)
			throw_posix_error();
	}
	~posix_iconv_file()
	{
		iconv_close(this->cd);
	}
};

namespace details
{
inline std::size_t do_iconv_impl(iconv_t cd,
	char** __restrict__ inbuf,std::size_t* __restrict__  inbytesleft,
	char** __restrict__ outbuf,std::size_t* __restrict__ outbytesleft)
{
	std::size_t res{iconv(cd,inbuf,inbytesleft,
		outbuf,outbytesleft)};
	if(res==static_cast<std::size_t>(-1))
		throw_posix_error();
	return res;
}
}

inline std::size_t do_iconv(posix_iconv_io_observer piciob,
	char** __restrict__ inbuf,std::size_t* __restrict__  inbytesleft,
	char** __restrict__ outbuf,std::size_t* __restrict__ outbytesleft)
{
	return details::do_iconv_impl(piciob.cd,inbuf,inbytesleft,outbuf,outbytesleft);
}

inline void reset_state(posix_iconv_io_observer piciob)
{
	if(iconv(piciob.cd,nullptr,0,nullptr,nullptr)==static_cast<std::size_t>(-1))
		throw_posix_error();
}

namespace details
{

inline std::size_t iconv_print_reserve_define_impl(iconv_t cd,char const* inbyteptr,std::size_t inbytesize,char* outbyteptr)
{
	char* inbyteptr_no_const{const_cast<char*>(inbyteptr)};
	std::size_t const output_buffer_size{inbytesize*8};
	std::size_t unconverted_char_counts{output_buffer_size};
	do_iconv_impl(cd,std::addressof(inbyteptr_no_const),std::addressof(inbytesize),std::addressof(outbyteptr),std::addressof(unconverted_char_counts));
	return output_buffer_size-unconverted_char_counts;
}

}

namespace manipulators
{

template<typename T>
struct iconv_code_cvt_t
{
	using manip_tag = manip_tag_t;
	iconv_t cd;
	T reference;
};

template<std::integral char_type>
constexpr iconv_code_cvt_t<io_scatter_t> iconv_code_cvt(posix_iconv_io_observer piiob,basic_io_scatter_t<char_type> scatter) noexcept
{
	return {piiob.cd,scatter};
}

template<typename rg>
requires (std::ranges::contiguous_range<std::remove_cvref_t<rg>>&&std::integral<std::ranges::range_value_t<std::remove_cvref_t<rg>>>)
constexpr iconv_code_cvt_t<io_scatter_t> iconv_code_cvt(posix_iconv_io_observer piiob,rg&& t)
{
	if constexpr(std::is_array_v<std::remove_cvref_t<rg>>)
		return {piiob.cd,{std::ranges::data(t),sizeof(std::ranges::range_value_t<std::remove_cvref_t<rg>>)*(std::ranges::size(t)-1)}};
	else
		return {piiob.cd,{std::ranges::data(t),sizeof(std::ranges::range_value_t<std::remove_cvref_t<rg>>)*std::ranges::size(t)}};
}

template<std::integral char_type>
constexpr iconv_code_cvt_t<io_scatter_t> iconv_code_cvt(posix_iconv_io_observer piiob,chvw_t<char_type const*> t) noexcept
{
	std::basic_string_view<char_type> view(t.reference);
	return {piiob.cd,{view.data(),sizeof(char_type)*view.size()}};
}
template<std::integral char_type>
inline std::size_t print_reserve_size(
	io_reserve_type_t<char_type,
	iconv_code_cvt_t<io_scatter_t>>,iconv_code_cvt_t<io_scatter_t> v) noexcept
{
	static_assert(sizeof(char_type)<=8);
	constexpr std::size_t maximum_size_sentinel{std::numeric_limits<std::size_t>::max()/8*sizeof(char_type)};
	if(v.reference.len>maximum_size_sentinel)[[unlikely]]
		fast_terminate();
	return v.reference.len*8/sizeof(char_type);
}

template<std::contiguous_iterator Iter>
inline Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,iconv_code_cvt_t<io_scatter_t>>,
	Iter iter,iconv_code_cvt_t<io_scatter_t> v)
{
	using char_type = std::iter_value_t<Iter>;
	std::size_t const sz{::fast_io::details::iconv_print_reserve_define_impl(v.cd,
	reinterpret_cast<char const*>(std::to_address(v.reference.base)),
	v.reference.len,reinterpret_cast<char*>(std::to_address(iter)))};
	return iter+sz/sizeof(char_type);
}
}

}