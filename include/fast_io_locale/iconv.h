#pragma once

#include"../fast_io_locale_device.h"
#include<iconv.h>

namespace fast_io
{

class posix_iconv_observer
{
public:
	using native_handle_type = iconv_t;
	std::uintptr_t cd=-1;
	constexpr decltype(auto) native_handle() const noexcept
	{
		return (cd);
	}
	constexpr decltype(auto) native_handle() noexcept
	{
		return (cd);
	}
	iconv_t get() noexcept
	{
		return bit_cast<iconv_t>(cd);
	}
	iconv_t get() const noexcept
	{
		return bit_cast<iconv_t>(cd);		
	}
	constexpr auto release() noexcept
	{
		auto temp{cd};
		cd = -1;
		return temp;
	}
	inline constexpr void swap(posix_iconv_observer& other) noexcept
	{
		std::swap(cd, other.cd);
	}
};

class posix_iconv:public posix_iconv_observer
{
public:
	using native_handle_type = iconv_t;
	constexpr posix_iconv()=default;
	constexpr posix_iconv(std::uintptr_t cd):posix_iconv_observer{cd}{}
	posix_iconv(std::string_view to_code,std::string_view from_code):posix_iconv(bit_cast<std::uintptr_t>(iconv_open(to_code.data(),from_code.data())))
	{
		if(this->native_handle()==std::uintptr_t(-1))
			throw_posix_error();
	}
	posix_iconv(posix_iconv const&)=delete;
	posix_iconv& operator=(posix_iconv const&)=delete;
	constexpr posix_iconv(posix_iconv&& bmv) noexcept:posix_iconv_observer{bmv.native_handle()}
	{
		bmv.native_handle()=-1;
	}
	posix_iconv& operator=(posix_iconv&& bmv) noexcept
	{
		if(std::addressof(bmv)==this)
			return *this;
		if(this->native_handle()!=-1)[[likely]]
			iconv_close(this->get());
		this->native_handle()=bmv.native_handle();
		bmv.native_handle()=-1;
		return *this;
	}
	~posix_iconv()
	{
		if(this->native_handle()!=-1)[[likely]]
			iconv_close(this->get());
	}
	inline void close()
	{
		if(this->native_handle()!=-1)[[likely]]
		{
			if(iconv_close(this->get())==-1)[[unlikely]]
				throw_posix_error();
			this->native_handle()=-1;
		}
	}
	inline void reset(std::uintptr_t newcd=-1) noexcept
	{
		if(this->native_handle()!=-1)[[likely]]
			iconv_close(this->get());
		this->native_handle()=newcd;
	}
};

class posix_iconv_function
{
public:
	posix_iconv_observer iob{};
	template<output_stream output,std::contiguous_iterator Iter>
	Iter operator()(output& out,Iter begin,Iter end)
	{
		//we do not know how much it would require. Let's just put it as 8 times. It should be enough.
		auto begin_ptr{reinterpret_cast<char*>(std::to_address(begin))};
		std::size_t const buffer_size((end-begin)*8);
		reserve_write(out,buffer_size,[&](auto ptr)
		{
			std::size_t insize(end-begin),avail(buffer_size);
			auto ptr_ptr{reinterpret_cast<char*>(std::to_address(ptr))};
			auto nconv{iconv(iob.get(),std::addressof(begin_ptr),std::addressof(insize),std::addressof(ptr_ptr),std::addressof(avail))};
			if(nconv==std::numeric_limits<std::size_t>::max()&&errno!=EINVAL)
			{
				throw_posix_error();
			}
			return ptr+(ptr_ptr-reinterpret_cast<char*>(std::to_address(ptr)))/sizeof(*ptr);
		});
		return begin+(begin_ptr-reinterpret_cast<char*>(std::to_address(begin)))/sizeof(*begin);
	}
};

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire>
class basic_iconv:public basic_indirect_ibuffer<src,indire,posix_iconv_function>
{
public:
	using source_buffer_type = src;
	using indirect_buffer_type = indire;
	using function_type = posix_iconv_function;
	using char_type = typename indirect_buffer_type::char_type;
	template<typename... Args>
	requires std::constructible_from<source_buffer_type,Args...>
	constexpr basic_iconv(posix_iconv_observer iob,Args&& ...args):basic_indirect_ibuffer<src,indire,function_type>{.source={std::forward<Args>(args)...},.function={iob}}
	{}
};

template<std::integral new_code_type,input_stream input>
using basic_ibuf_posix_iconv = basic_iconv<basic_ibuf<input>,vector_buffer<new_code_type>>;

template<std::integral new_code_type,std::integral source_code_type>
using basic_ibuf_posix_iconv_file = basic_ibuf_posix_iconv<new_code_type,basic_inative_file<source_code_type>>;

}