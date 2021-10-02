﻿#pragma once

namespace fast_io
{

template<std::integral T>
class basic_mfc_io_observer
{
public:
	using char_type = T;
	using native_handle_type = CFile*;
	native_handle_type phandle{};
	explicit constexpr operator bool() const noexcept
	{
		return phandle;
	}
	template<win32_family family>
	explicit operator basic_win32_family_io_observer<family,char_type>() const
	{
		return {static_cast<void*>(handle)};
	}
	constexpr auto& native_handle() const noexcept
	{
		return phandle;
	}
	constexpr auto& native_handle() noexcept
	{
		return phandle;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{phandle};
		phandle=nullptr;
		return temp;
	}
};


template<std::integral T>
class basic_mfc_io_handle:public basic_mfc_io_observer<T>
{
public:
	using char_type = T;
	using native_handle_type = CFile*;
	basic_mfc_io_handle()=default;
	basic_mfc_io_handle(native_handle_type hd):basic_mfc_io_observer<T>{hd}{}
	basic_mfc_io_handle(basic_mfc_io_handle const& mcf):basic_mfc_io_observer<T>{mcf.native_handle()->Duplicate()}{}
	basic_mfc_io_handle& operator=(basic_mfc_io_handle const& mcf)
	{
		auto temp{mcf.native_handle()->Duplicate()};
		delete this->native_handle();
		this->native_handle()=temp;
		return *this;
	}
	basic_mfc_io_handle(basic_mfc_io_handle&& mcf) noexcept:basic_mfc_io_observer<T>{mcf.native_handle()}
	{
		mcf.native_handle()=nullptr;
	}
	basic_mfc_io_handle& operator=(basic_mfc_io_handle&& mcf) noexcept
	{
		if(mcf.native_handle()!=handle)
		{
			delete handle;
			this->native_handle()=mcf.native_handle();
			mcf.native_handle()=nullptr;
		}
		return *this;
	}

	inline constexpr void reset(native_handle_type newhandle=nullptr) noexcept
	{
		delete this->native_handle();
		this->native_handle()=newhandle;
	}
};

template<std::integral T,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter write(basic_mfc_io_observer<T> hd,Iter begin,Iter end)
{
	std::size_t const to_write{(end-begin)*sizeof(*begin)};
	if constexpr(sizeof(std::size_t)>4)
	{
		if (static_cast<std::size_t>(UINT32_MAX)<to_write)
		{
			hd.native_handle()->Write(::fast_io::freestanding::to_address(begin),UINT32_MAX);
			return begin+(UINT32_MAX/sizeof(*begin));
		}
	}
	hd.native_handle()->Write(::fast_io::freestanding::to_address(begin),static_cast<std::uint32_t>(to_write));
	return end;
}

template<std::integral T>
inline void flush(basic_mfc_io_observer<T>& hd)
{
	hd.native_handle()->Flush();
}

template<std::integral T,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter read(basic_mfc_io_observer<T& hd,Iter begin,Iter end)
{
	std::size_t to_read{(end-begin)*sizeof(*begin)};
	if constexpr(sizeof(std::size_t)>4)
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	return begin+(hd.native_handle()->Read(::fast_io::freestanding::to_address(begin),static_cast<std::uint32_t>(to_read))/sizeof(*begin));

}

template<std::integral T>
class basic_mfc_file:public basic_mfc_io_handle<T>
{
public:
	using char_type = T;
	using native_handle_type = CFile*;
	basic_mfc_file()=default;
	basic_mfc_file(native_handle_type hd):basic_mfc_io_handle<T>(hd){}
	basic_mfc_file(native_interface_t,native_handle_type val):basic_mfc_io_handle<T>(val){}
	basic_mfc_file(basic_win32_io_handle<char_type>&& hd,open_mode om):
		basic_mfc_io_handle<T>(new CFile(hd.native_handle()))
	{
		hd.release();
	}
	basic_mfc_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_mfc_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_mfc_file(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_mfc_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}
	basic_mfc_file(basic_mfc_file const&)=default;
	basic_mfc_file& operator=(basic_mfc_file const&)=default;
	basic_mfc_file(basic_mfc_file&&) noexcept=default;
	basic_mfc_file& operator=(basic_mfc_file&&) noexcept=default;
	~basic_mfc_file()
	{
		delete this->native_handle();
	}
};

template<std::integral ch_type>
requires (redirect_stream<basic_win32_io_observer<ch_type>>)
inline decltype(auto) redirect_handle(basic_mfc_io_observer<ch_type>& hd)
{
	return redirect_handle(static_cast<basic_win32_io_observer<ch_type>>(hd));
}

template<std::integral ch_type>
requires (zero_copy_input_stream<basic_win32_io_observer<ch_type>>)
inline decltype(auto) zero_copy_in_handle(basic_mfc_io_observer<ch_type>& hd)
{
	return zero_copy_in_handle(static_cast<basic_win32_io_observer<ch_type>>(hd));
}

template<std::integral ch_type>
requires (zero_copy_output_stream<basic_win32_io_observer<ch_type>>)
inline decltype(auto) zero_copy_out_handle(basic_mfc_io_observer<ch_type>& hd)
{
	return zero_copy_out_handle(static_cast<basic_win32_io_observer<ch_type>>(hd));
}
using mfc_io_observer=basic_mfc_io_observer<char>;
using mfc_io_handle=basic_mfc_io_handle<char>;
using mfc_file=basic_mfc_file<char>;
using u8mfc_io_observer=basic_mfc_io_observer<char8_t>;
using u8mfc_io_handle=basic_mfc_io_handle<char8_t>;
using u8mfc_file=basic_mfc_file<char8_t>;


}