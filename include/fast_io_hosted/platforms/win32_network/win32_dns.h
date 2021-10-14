#pragma once

namespace fast_io
{

template<win32_family fam>
requires (fam==win32_family::ansi_9x||fam==win32_family::wide_nt)
class win32_family_dns_io_observer
{
public:
	using char_type = std::conditional_t<fam==win32_family::wide_nt,wchar_t,char>;
	using native_handle_type = ::fast_io::win32::win32_family_addrinfo<fam>*;
	native_handle_type res{};
	inline constexpr native_handle_type native_handle() const noexcept
	{
		return res;
	}
	explicit constexpr operator bool() noexcept
	{
		return res;
	}
	inline native_handle_type release() noexcept
	{
		native_handle_type temp{res};
		res=nullptr;
		return temp;
	}
};

template<win32_family fam>
requires (fam==win32_family::ansi_9x||fam==win32_family::wide_nt)
struct win32_family_dns_iterator
{
	using native_handle_type = ::fast_io::win32::win32_family_addrinfo<fam>*;
	native_handle_type res{};
};

template<win32_family fam>
inline constexpr bool operator==(win32_family_dns_iterator<fam> a, win32_family_dns_iterator<fam> b) noexcept
{
	return a.res == b.res;
}

template<win32_family fam>
inline constexpr bool operator!=(win32_family_dns_iterator<fam> a, win32_family_dns_iterator<fam> b) noexcept
{
	return !(a==b);
}

template<win32_family fam>
inline constexpr win32_family_dns_io_observer<fam> operator*(win32_family_dns_iterator<fam> d) noexcept
{
	return {d.res};
}

template<win32_family fam>
inline constexpr win32_family_dns_iterator<fam>& operator++(win32_family_dns_iterator<fam>& d) noexcept
{
	d.res = d.res->ai_next;
	return d;
}

template<win32_family fam>
inline constexpr win32_family_dns_iterator<fam> operator++(win32_family_dns_iterator<fam>& d, int) noexcept
{
	auto temp{d};
	++d;
	return temp;
}

template<win32_family fam>
inline constexpr ::fast_io::ip to_ip(win32_family_dns_io_observer<fam> d,std::uint16_t port)
{
	return to_ip_with_ai_addr(d.res->ai_family,d.res->ai_addr,port);
}

namespace details
{

template<win32_family fam>
inline ::fast_io::win32::win32_family_addrinfo<fam>* win32_getaddrinfo_impl(std::conditional_t<fam==win32_family::wide_nt,wchar_t,char> const* node,std::conditional_t<fam==win32_family::wide_nt,wchar_t,char> const* service,::fast_io::win32::win32_family_addrinfo<fam> const* hints)
{
	::fast_io::win32::win32_family_addrinfo<fam>* res{};
	if constexpr(win32_family::ansi_9x==fam)
	{
		int ret{::fast_io::win32::getaddrinfo(node,service,hints,__builtin_addressof(res))};
		if(ret)
			throw_win32_error(static_cast<std::uint32_t>(ret));
	}
	else
	{
		int ret{::fast_io::win32::GetAddrInfoW(node,service,hints,__builtin_addressof(res))};
		if(ret)
			throw_win32_error(static_cast<std::uint32_t>(ret));
	}
	return res;
}

template<::fast_io::win32_family fam>
inline void win32_family_freeaddrinfo_impl(::fast_io::win32::win32_family_addrinfo<fam>* paddrinfo)
{
	if constexpr(fam==::fast_io::win32_family::ansi_9x)
	{
		::fast_io::win32::freeaddrinfo(paddrinfo);
	}
	else
	{
		::fast_io::win32::FreeAddrInfoW(paddrinfo);
	}
}


template<win32_family fam,typename T>
inline constexpr auto win32_family_dns_open_alien_char_type_impl(T const* t,std::size_t n)
{
	win32_family_api_encoding_converter<fam> converter(t,n);
	constexpr ::fast_io::win32::win32_family_addrinfo<fam> info{.ai_family=0};
	return win32_getaddrinfo_impl<fam>(converter.native_c_str(),nullptr,__builtin_addressof(info));
}

template<win32_family fam,typename T>
requires ::fast_io::constructible_to_os_c_str<T>
inline constexpr auto win32_family_dns_open_impl(T const& t)
{
	using char_type=std::conditional_t<fam==::fast_io::win32_family::ansi_9x,char,wchar_t>;
	if constexpr(::std::is_array_v<T>)
	{
		using cstr_char_type = std::remove_extent_t<T>;
		static_assert(::std::integral<cstr_char_type>);
		auto p{t};
		if constexpr(sizeof(cstr_char_type)==sizeof(char_type))
		{
			constexpr ::fast_io::win32::win32_family_addrinfo<fam> info{.ai_family=0};
			using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
			[[gnu::may_alias]]
#endif
			= char_type const*;
			return win32_getaddrinfo_impl<fam>(reinterpret_cast<char_type_may_alias_ptr>(p),nullptr,__builtin_addressof(info));
		}
		else
		{
			return win32_family_dns_open_alien_char_type_impl<fam>(p,::fast_io::details::cal_array_size(t));
		}
	}
	else
	{
		using cstr_char_type = std::remove_pointer_t<decltype(t.c_str())>;
		if constexpr(sizeof(cstr_char_type)==sizeof(char_type))
		{
			constexpr ::fast_io::win32::win32_family_addrinfo<fam> info{.ai_family=0};
			using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
			[[gnu::may_alias]]
#endif
			= char_type const*;
			return win32_getaddrinfo_impl<fam>(reinterpret_cast<char_type_may_alias_ptr>(t.c_str()),nullptr,__builtin_addressof(info));
		}
		else
		{
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1) && __cpp_lib_ranges >= 201911L
			if constexpr(::std::ranges::contiguous_range<std::remove_cvref_t<T>>)
			{
				return win32_family_dns_open_alien_char_type_impl<fam>(::std::ranges::data(t),::std::ranges::size(t));
			}
			else
#endif
			{
				auto ptr{t.c_str()};
				return win32_family_dns_open_alien_char_type_impl<fam>(ptr,::fast_io::cstr_len(ptr));
			}
		}
	}
}

}

template<win32_family fam>
class win32_family_dns_file:public win32_family_dns_io_observer<fam>
{
public:
	using char_type = typename win32_family_dns_io_observer<fam>::char_type;
	using native_handle_type = typename win32_family_dns_io_observer<fam>::native_handle_type;
	constexpr win32_family_dns_file() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr win32_family_dns_file(native_hd res1) noexcept: win32_family_dns_io_observer<fam>{res1}{}
	explicit constexpr win32_family_dns_file(decltype(nullptr)) noexcept = delete;
	win32_family_dns_file(char_type const* node,char_type const* service,::fast_io::win32::win32_family_addrinfo<fam> const* hints):win32_family_dns_io_observer<fam>{::fast_io::details::win32_getaddrinfo_impl<fam>(node,service,hints)}{}
	template<typename T>
	requires ::fast_io::constructible_to_os_c_str<T>
	explicit win32_family_dns_file(T const& t):win32_family_dns_io_observer<fam>{::fast_io::details::win32_family_dns_open_impl<fam>(t)}{}
	win32_family_dns_file(win32_family_dns_file const&)=delete;
	win32_family_dns_file& operator=(win32_family_dns_file const&)=delete;
	constexpr win32_family_dns_file(win32_family_dns_file&& other) noexcept: win32_family_dns_io_observer<fam>{other.res}
	{
		other.res=nullptr;
	}
	win32_family_dns_file& operator=(win32_family_dns_file&& other) noexcept
	{
		if(__builtin_addressof(other)==this)
			return *this;
		if(this->res)[[likely]]
			::fast_io::details::win32_family_freeaddrinfo_impl<fam>(this->res);
		this->res=other.res;
		other.res=nullptr;
		return *this;
	}
	void close() noexcept
	{
		if(this->res)[[likely]]
		{
			::fast_io::details::win32_family_freeaddrinfo_impl<fam>(this->res);
			this->res=nullptr;
		}
	}
	~win32_family_dns_file()
	{
		if(this->res)[[likely]]
			::fast_io::details::win32_family_freeaddrinfo_impl<fam>(this->res);
	}
};

template<win32_family fam>
inline constexpr win32_family_dns_iterator<fam> begin(win32_family_dns_io_observer<fam> pdniob) noexcept
{
	return {pdniob.res};
}

template<win32_family fam>
inline constexpr win32_family_dns_iterator<fam> cbegin(win32_family_dns_io_observer<fam> pdniob) noexcept
{
	return {pdniob.res};
}

template<win32_family fam>
inline constexpr win32_family_dns_iterator<fam> end(win32_family_dns_io_observer<fam>) noexcept
{
	return {};
}

template<win32_family fam>
inline constexpr win32_family_dns_iterator<fam> cend(win32_family_dns_io_observer<fam>) noexcept
{
	return {};
}

using ::fast_io::win32::win32_family_addrinfo;
using ::fast_io::win32::win32_addrinfo_9xa;
using ::fast_io::win32::win32_addrinfo_ntw;
using win32_addrinfo = ::fast_io::win32::win32_family_addrinfo<::fast_io::win32_family::native>;

using win32_dns_io_observer_9xa = win32_family_dns_io_observer<win32_family::ansi_9x>;
using win32_dns_io_observer_ntw = win32_family_dns_io_observer<win32_family::wide_nt>;
using win32_dns_io_observer = win32_family_dns_io_observer<win32_family::native>;

using win32_dns_file_9xa = win32_family_dns_file<win32_family::ansi_9x>;
using win32_dns_file_ntw = win32_family_dns_file<win32_family::wide_nt>;
using win32_dns_file = win32_family_dns_file<win32_family::native>;

using win32_dns_iterator_9xa = win32_family_dns_iterator<win32_family::ansi_9x>;
using win32_dns_iterator_ntw = win32_family_dns_iterator<win32_family::wide_nt>;
using win32_dns_iterator = win32_family_dns_iterator<win32_family::native>;


inline win32_dns_file_9xa win32_addrinfo_serivce(char const* node,char const* service)
{
	constexpr win32_addrinfo_9xa info{.ai_family=0};
	return win32_dns_file_9xa(node,service,__builtin_addressof(info));
}

#if !defined(__WINE__) && !defined(__CYGWIN__)
inline win32_dns_file_9xa native_addrinfo_service(char const* node,char const* service)
{
	return win32_addrinfo_serivce(node,service);
}

using native_addrinfo = win32_addrinfo;
using native_dns_io_observer = win32_dns_io_observer;
using native_dns_file = win32_dns_file;
using native_dns_iterator = win32_dns_iterator;
#endif

template<win32_family fam>
inline win32_socket_factory addrinfo_service_connect(win32_family_dns_io_observer<fam> dniob,open_mode mode={})
{
	auto& info{*dniob.res};
	basic_win32_socket_file<char> pf(::fast_io::win32::details::open_win32_socket_raw_om_custom_only_impl<win32_family::native>(info.ai_family,info.ai_socktype,info.ai_protocol,mode));
	posix_connect(pf,info.ai_addr,info.ai_addrlen);
	return win32_socket_factory(pf.release());
}

}
