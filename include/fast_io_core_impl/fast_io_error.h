#pragma once

namespace fast_io
{

class error_reporter
{
public:
	using char_type = char;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char const* write_impl(char const*,char const*)=0;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char_type* oreserve_impl(std::size_t)=0;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void orelease_impl(char_type*)=0;
};
namespace details
{
template<typename sm>
requires (reserve_output_stream<std::remove_cvref_t<sm>>)
class error_reporter_derv:public error_reporter
{
public:
	using char_type = char;
	sm reff;
	template<typename... Args>
	requires std::constructible_from<sm,Args...>
	constexpr error_reporter_derv(std::in_place_t,Args&& ...args):reff(std::forward<Args>(args)...){}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char const* write_impl(char_type const* b,char_type const* e) override
	{
		if constexpr(std::same_as<decltype(write(reff,b,e)),void>)
		{
			write(reff,b,e);
			return e;
		}
		else
			return write(reff,b,e);
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char_type* oreserve_impl(std::size_t sz) override
	{
		return oreserve(reff,sz);
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void orelease_impl(char_type* ptr) override
	{
		orelease(reff,ptr);
	}
};
}
template<std::contiguous_iterator Iter>
#if __cpp_constexpr >= 201907L
	constexpr
#endif
Iter write(error_reporter& dev,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,char>)
	{
		return dev.write_impl(std::to_address(begin),std::to_address(end))-std::to_address(begin)+begin;
	}
	else
	{
		auto b(reinterpret_cast<char const*>(std::to_address(begin)));
		return begin+(dev.write_impl(b,
			b+(end-begin)*sizeof(*begin))-b)/sizeof(*begin);
	}
}

constexpr inline void flush(error_reporter&){}

#if __cpp_constexpr >= 201907L
	constexpr
#endif
inline auto oreserve(error_reporter& dev,std::size_t size)
{
	return dev.oreserve_impl(size);
}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
inline void orelease(error_reporter& dev,char* ptr)
{
	dev.orelease_impl(ptr);
}

class fast_io_error
#ifndef FAST_IO_ERROR_DISABLE_STD_EXCEPTION
:public std::exception
#endif
{
#ifndef FAST_IO_ERROR_DISABLE_STD_EXCEPTION
	details::temp_unique_arr_ptr<char> mutable uptr;
#endif
public:
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void report(error_reporter& err) const=0;
//.what() in std::exception was a horrible mistake
#ifndef FAST_IO_ERROR_DISABLE_STD_EXCEPTION
	char const* what() const noexcept override
	{
#ifdef __cpp_exceptions
		try
		{
#endif
			internal_temporary_buffer<char> buffer;
			details::error_reporter_derv<internal_temporary_buffer<char>&> err(std::in_place,buffer);
			report(err);
			put(buffer,0);
			if(buffer.beg_ptr==buffer.static_buffer.data())
			{
				uptr.reset(new char[buffer.end_ptr-buffer.beg_ptr]);
				std::copy(buffer.beg_ptr,buffer.end_ptr,uptr.get());
			}
			else
			{
				uptr.reset(buffer.beg_ptr);
				buffer.beg_ptr=buffer.static_buffer.data();
			}
			return uptr.get();
#ifdef __cpp_exceptions
		}
/*
comment it to avoid dead loop???
		catch(std::exception const& e)
		{
			auto str{e.what()};
			std::size_t len{strlen(str)};
			uptr.reset(new char[len+1]);
			*std::copy(str,str+len,uptr.get())=0;
			return uptr.get();
		}
*/
		catch(...)
		{
			return "unknown fast_io_error";
		}
#endif
	}
#endif
};

namespace details
{
template<fast_io::output_stream output>
requires std::same_as<typename output::char_type,char>
constexpr void print_define_to_fast_io_error(output& out,fast_io_error const& err)
{
	internal_temporary_buffer<char> buffer;
	details::error_reporter_derv<internal_temporary_buffer<char>&> dev(std::in_place,buffer);
	err.report(dev);
	write(out,buffer.beg_ptr,buffer.end_ptr);
}
}

template<fast_io::output_stream output>
requires std::same_as<typename output::char_type,char>
constexpr void print_define(output& out,fast_io_error const& err)
{
	if constexpr(fast_io::reserve_output_stream<output>)
	{
		if constexpr(std::same_as<decltype(oreserve(out,0)),char*>)
		{
			details::error_reporter_derv<output&> dev(std::in_place,out);
			err.report(dev);
			return;
		}
		else
			details::print_define_to_fast_io_error(out,err);
	}
	else
		details::print_define_to_fast_io_error(out,err);
}

class fast_io_text_error:public fast_io_error
{
public:
	std::string_view text;
	explicit fast_io_text_error(std::string_view txt):text(txt){}
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void report(error_reporter& err) const override
	{
		write(err,text.data(),text.data()+text.size());
	}
};


inline void throw_malformed_input();

inline void throw_input_overflow_error();

}