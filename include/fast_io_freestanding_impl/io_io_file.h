#pragma once

namespace fast_io
{

namespace details::io_io
{
template<std::integral char_type>
class base
{
public:
	virtual 
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char_type* read_impl(char_type*,char_type*) = 0;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char_type const* write_impl(char_type const*,char_type const*) = 0;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void flush_impl() = 0;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	std::uintmax_t seek_impl(std::intmax_t,seekdir) = 0;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	base* clone() = 0;
	virtual
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	~base() = default;
};
template<std::integral char_type,typename stm>
class derv:public base<char_type>
{
public:
	using value_type = std::remove_reference_t<stm>;
	stm io;
	template<typename... Args>
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	derv(std::in_place_type_t<stm>,Args&& ...args):io(std::forward<Args>(args)...){}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char_type* read_impl(char_type* b,char_type* e) override
	{
		if constexpr(input_stream<value_type>)
			return read(io,b,e);
		else
			throw_posix_error(EOPNOTSUPP);
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	char_type const* write_impl(char_type const* b,char_type const* e) override
	{
		if constexpr(output_stream<value_type>)
		{
			if constexpr(std::same_as<decltype(write(io,b,e)),void>)
			{
				write(io,b,e);
				return e;
			}
			else
			{
				return write(io,b,e);
			}			
		}
		else
			throw_posix_error(EOPNOTSUPP);
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void flush_impl() override
	{
		if constexpr(output_stream<value_type>)
		{
			if constexpr(flush_output_stream<value_type>)
				flush(io);
		}
		else
			throw_posix_error(EOPNOTSUPP);
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	std::uintmax_t seek_impl(std::intmax_t off,seekdir dir) override
	{
		if constexpr(random_access_stream<value_type>)
			return seek(io,seek_type<char_type>,off,dir);
		else
			throw_posix_error(EOPNOTSUPP);
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	base<char_type>* clone() override
	{
		if constexpr(std::is_reference_v<stm>||std::copyable<stm>)
			return new derv(std::in_place_type<stm>,this->io);
		else
			throw_posix_error(EOPNOTSUPP);
	}
};

}

template<std::integral ch_type>
class basic_io_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = details::io_io::base<char_type>*;
	native_handle_type io_ptr{};
	constexpr auto release() noexcept
	{
		auto temp{io_ptr};
		io_ptr=nullptr;
		return temp;
	}
	constexpr auto& native_handle() noexcept
	{
		return io_ptr;
	}
	constexpr auto& native_handle() const noexcept
	{
		return io_ptr;
	}
	inline constexpr void reset(native_handle_type newptr=nullptr) noexcept
	{
		io_ptr=newptr;
	}
	inline constexpr void swap(basic_io_io_observer& other) noexcept
	{
		std::swap(io_ptr, other.io_ptr);
	}
};
#ifdef __cpp_rtti
template<typename T,std::integral char_type>
constexpr auto& io_io_cast(basic_io_io_observer<char_type> iob)
{
	return dynamic_cast<details::io_io::derv<char_type,T>>(iob.io);
}
#endif

template<std::integral ch_type>
class basic_io_io_handle:public basic_io_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = typename basic_io_io_observer<ch_type>::native_handle_type;
	constexpr basic_io_io_handle() = default;
	constexpr basic_io_io_handle(native_handle_type io_ptr):basic_io_io_observer<ch_type>{io_ptr}{}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_io_handle(basic_io_io_handle const& other):basic_io_io_observer<ch_type>(other.io_ptr->clone()){}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_io_handle& operator=(basic_io_io_handle const& other)
	{
		auto temp{other.io_ptr->clone()};
		delete this->io_ptr;
		this->io_ptr=temp;
		return *this;
	}
	constexpr basic_io_io_handle(basic_io_io_handle&& other) noexcept:basic_io_io_observer<ch_type>(other.io_ptr)
	{
		other.io_ptr=nullptr;
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_io_handle& operator=(basic_io_io_handle&& other) noexcept
	{
		if(other.io_ptr==this->io_ptr)
			return *this;
		delete this->io_ptr;
		this->io_ptr=other.io_ptr;
		other.io_ptr=nullptr;
		return *this;
	}
};

template<std::integral ch_type>
class basic_io_file:public basic_io_io_handle<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = typename basic_io_io_observer<ch_type>::native_handle_type;
	constexpr basic_io_file()=default;
	constexpr basic_io_file(native_handle_type ptr):basic_io_io_handle<ch_type>(ptr){}
	template<stream smt,typename... Args>
	requires std::constructible_from<smt,Args...>
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_file(io_cookie_t,std::in_place_type_t<smt>,Args&& ...args)
		:basic_io_io_handle<ch_type>(new details::io_io::derv<char_type,smt>(std::in_place_type<smt>,std::forward<Args>(args)...)){}
	template<stream smt>
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_file(io_cookie_t,smt& sm):
		basic_io_io_handle<ch_type>(new details::io_io::derv<char_type,smt&>(std::in_place_type<smt&>,sm))
	{}
	template<stream smt>
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_file(io_cookie_t,smt&& sm):
		basic_io_io_handle<ch_type>(new details::io_io::derv<char_type,smt>(std::in_place_type<smt>,std::move(sm)))
	{}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_file(basic_io_file const&)=default;
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_file& operator=(basic_io_file const&)=default;
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_file(basic_io_file&&) noexcept=default;
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	basic_io_file& operator=(basic_io_file&&) noexcept=default;
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	~basic_io_file()
	{
		delete this->io_ptr;
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
requires (std::same_as<std::iter_value_t<Iter>,ch_type>||std::same_as<ch_type,char>)
constexpr Iter read(basic_io_io_observer<ch_type> iob,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,ch_type>)
	{
		return iob.io_ptr->read_impl(std::to_address(begin),std::to_address(end))-std::to_address(begin)+begin;
	}
	else
	{
		auto b(reinterpret_cast<ch_type*>(std::to_address(begin)));
		return begin+(iob.io_ptr->read_impl(b,
			b+(end-begin)*sizeof(*begin))-b)/sizeof(*begin);
	}
}

template<std::integral ch_type,std::contiguous_iterator Iter>
requires (std::same_as<std::iter_value_t<Iter>,ch_type>||std::same_as<ch_type,char>)
constexpr Iter write(basic_io_io_observer<ch_type> iob,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,ch_type>)
	{
		return iob.io_ptr->write_impl(std::to_address(begin),std::to_address(end))-std::to_address(begin)+begin;
	}
	else
	{
		auto b(reinterpret_cast<ch_type const*>(std::to_address(begin)));
		return begin+(iob.io_ptr->write_impl(b,
			b+(end-begin)*sizeof(*begin))-b)/sizeof(*begin);
	}
}
template<std::integral ch_type>
constexpr void flush(basic_io_io_observer<ch_type> iob)
{
	iob.io_ptr->flush_impl();
}
template<std::integral ch_type,typename T,std::integral U>
constexpr std::uintmax_t seek(basic_io_io_observer<ch_type> iob,seek_type_t<T>,U i=0,seekdir s=seekdir::cur)
{
	return iob.io_ptr->seek_impl(seek_precondition<std::intmax_t,T,ch_type>(i),s);
}

template<std::integral ch_type,std::integral U>
constexpr auto seek(basic_io_io_observer<ch_type> iob,U i=0,seekdir s=seekdir::cur)
{
	return seek(iob,seek_type<ch_type>,i,s);
}
using io_io_observer = basic_io_io_observer<char>;
using io_io_handle = basic_io_io_handle<char>;
using io_file = basic_io_file<char>;

using u8io_io_observer = basic_io_io_observer<char8_t>;
using u8io_io_handle = basic_io_io_handle<char8_t>;
using u8io_file = basic_io_file<char8_t>;

using wio_io_observer = basic_io_io_observer<wchar_t>;
using wio_io_handle = basic_io_io_handle<wchar_t>;
using wio_file = basic_io_file<wchar_t>;

}