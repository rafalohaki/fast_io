#pragma once
struct bio_method_st
{
    int type;
    char const *name;
    int (*bwrite) (BIO *, const char *, size_t, size_t *) noexcept;
    int (*bwrite_old) (BIO *, const char *, int) noexcept;
    int (*bread) (BIO *, char *, size_t, size_t *) noexcept;
    int (*bread_old) (BIO *, char *, int) noexcept;
    int (*bputs) (BIO *, const char *) noexcept;
    int (*bgets) (BIO *, char *, int) noexcept;
    long (*ctrl) (BIO *, int, long, void *) noexcept;
    int (*create) (BIO *) noexcept;
    int (*destroy) (BIO *) noexcept;
    long (*callback_ctrl) (BIO *, int, BIO_info_cb *) noexcept;
};

namespace fast_io
{

namespace details
{
template<bool detach>
inline constexpr int calculate_bio_new_fp_flags(open_mode om)
{
	int flag{BIO_CLOSE};
	if constexpr(!detach)
		flag=BIO_NOCLOSE;
	if((om&open_mode::text)!=open_mode::none)
		flag|=BIO_FP_TEXT;
	return flag;
}

template<open_mode om,bool detach>
struct bio_new_fp_flags
{
	inline static constexpr int value=calculate_bio_new_fp_flags<detach>(om);
};


inline std::FILE* bio_to_fp(BIO* bio) noexcept
{
	std::FILE* fp{};
	BIO_get_fp(bio,__builtin_addressof(fp));
	return fp;
}

inline int bio_to_fd(BIO* bio) noexcept
{
	auto fp{bio_to_fp(bio)};
	if(fp==nullptr)
	{
		int fd{-1};
		BIO_get_fd(bio,__builtin_addressof(fd));
		return fd;
	}
	return ::fast_io::details::fp_to_fd(fp);
}

}

template<typename stm>
requires (stream<std::remove_reference_t<stm>>)
struct bio_io_cookie_functions_t
{
	using native_functions_type = bio_method_st;
	native_functions_type functions{};
	explicit bio_io_cookie_functions_t()
	{
		using value_type = std::remove_reference_t<stm>;
		if constexpr(input_stream<value_type>)
		{
			functions.bread=[](BIO* bbio,char* buf,std::size_t size,std::size_t* readd) noexcept->int
			{
#ifdef __cpp_exceptions
				try
				{
#endif
					*readd=read(*bit_cast<value_type*>(BIO_get_data(bbio)),buf,buf+size)-buf;
					return 1;
#ifdef __cpp_exceptions
				}
				catch(...)
				{
					return -1;
				}
#endif
			};
		}
		if constexpr(output_stream<value_type>)
		{
			functions.bwrite=[](BIO* bbio,char const* buf,std::size_t size,std::size_t* written) noexcept->int
			{
#ifdef __cpp_exceptions
				try
				{
#endif
					if constexpr(std::same_as<decltype(write(*bit_cast<value_type*>(BIO_get_data(bbio)),buf,buf+size)),void>)
					{
						write(*bit_cast<value_type*>(BIO_get_data(bbio)),buf,buf+size);
						*written=size;
					}
					else
						*written=write(*bit_cast<value_type*>(BIO_get_data(bbio)),buf,buf+size)-buf;
					return 1;
#ifdef __cpp_exceptions
				}
				catch(...)
				{
					return -1;
				}
#endif
			};
		}
		if constexpr(!std::is_reference_v<stm>)
			functions.destroy=[](BIO* bbio) noexcept -> int
			{
				delete bit_cast<stm*>(BIO_get_data(bbio));
				return 1;
			};
#if __cpp_rtti
		functions.name=typeid(stm).name();
		constexpr int value(BIO_TYPE_DESCRIPTOR-BIO_TYPE_START);
		static_assert(0<value);
		functions.type=static_cast<int>(typeid(stm).hash_code()%value+BIO_TYPE_START);
#else
		functions.name=reinterpret_cast<char const*>(u8"unknown");
		constexpr int value(BIO_TYPE_DESCRIPTOR-BIO_TYPE_START);
		static_assert(0<value);
		functions.type=static_cast<int>(BIO_TYPE_START);
#endif
	}
};

template<typename stm>
inline bio_io_cookie_functions_t<stm> const bio_io_cookie_functions{};

namespace details
{

inline BIO* bio_new_stream_type(bio_method_st const* methods)
{
	auto bio{BIO_new(methods)};
	if(bio==nullptr)
		throw_openssl_error();
	return bio;
}

template<stream stm>
inline BIO* construct_bio_by_t(stm* ptr)
{
	auto bp{bio_new_stream_type(__builtin_addressof(bio_io_cookie_functions<stm>.functions))};
	BIO_set_data(bp,ptr);
	return bp;
}
#if 0
template<stream stm,typename... Args>
inline BIO* construct_bio_by_args(Args... args)
{

	std::unique_ptr<stm> p{new stm(::fast_io::freestanding::forward<Args>(args)...)};
	BIO* fp{construct_bio_by_t(p.get())};
	p.release();
	return fp;
}
#endif
}

template<std::integral ch_type>
class basic_bio_io_observer
{
public:
	using native_handle_type = BIO*;
	using char_type = ch_type;
	native_handle_type bio{};
	constexpr operator bool() const noexcept
	{
		return bio;
	}
	constexpr auto& native_handle() const noexcept
	{
		return bio;
	}
	constexpr auto& native_handle() noexcept
	{
		return bio;
	}
	constexpr auto release() noexcept
	{
		auto temp{bio};
		bio=nullptr;
		return temp;
	}
	explicit operator basic_c_io_observer<char_type>() const noexcept
	{
		return {details::bio_to_fp(bio)};
	}
	explicit operator basic_posix_io_observer<char_type>() const noexcept
	{
		return {details::bio_to_fd(bio)};
	}
#ifdef _WIN32
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return basic_win32_io_observer<char_type>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	template<nt_family fam>
	explicit operator basic_nt_family_io_observer<fam,char_type>() const noexcept
	{
		return basic_nt_family_io_observer<fam,char_type>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
};
template<std::integral ch_type>
class basic_bio_file:public basic_bio_io_observer<ch_type>
{
	void detect_open_failure()
	{
		if(this->bio==nullptr)[[unlikely]]
			throw_openssl_error();
	}
public:
	using native_handle_type = BIO*;
	using char_type = ch_type;
	constexpr basic_bio_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_bio_file(native_hd bio):basic_bio_io_observer<char_type>{bio}{}

#if 0
	template<stream stm,typename ...Args>
	requires std::constructible_from<stm,Args...>
	basic_bio_file(std::in_place_type_t<stm>,Args&& ...args):
		basic_bio_io_observer<char_type>(details::construct_bio_by_args<stm>(::fast_io::freestanding::forward<Args>(args)...))
	{
	}
#endif
	basic_bio_file(basic_c_io_handle<char_type>&& bmv,fast_io::open_mode om):
		basic_bio_io_observer<char_type>(BIO_new_fp(bmv.fp,details::calculate_bio_new_fp_flags<true>(om)))
	{
		detect_open_failure();
		bmv.release();
	}
	basic_bio_file(basic_posix_io_handle<char_type>&& bmv,fast_io::open_mode om):
		basic_bio_io_observer<char_type>(BIO_new_fd(bmv.fd,details::calculate_bio_new_fp_flags<true>(om)))
	{
		detect_open_failure();
		bmv.release();
	}


#if defined(_WIN32) || defined(__CYGWIN__)
	template<win32_family family>
	basic_bio_file(basic_win32_family_io_handle<family,char_type>&& bmv,fast_io::open_mode om):
		basic_bio_file(basic_posix_file(::fast_io::freestanding::move(bmv),om),om)
	{
	}
	template<nt_family family>
	basic_bio_file(basic_nt_family_io_handle<family,char_type>&& nt_handle,open_mode om):
		basic_bio_file(basic_posix_file<char_type>(::fast_io::freestanding::move(nt_handle),om),to_native_c_mode(om))
	{
	}
#endif
	basic_bio_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(file,om,pm),om)
	{}
	basic_bio_file(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(nate,file,om,pm),om)
	{}
	basic_bio_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(file,om,pm),om)
	{}
	basic_bio_file(native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(nate,file,om,pm),om)
	{}
	basic_bio_file(u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(file,om,pm),om)
	{}
	basic_bio_file(native_at_entry nate,u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(nate,file,om,pm),om)
	{}
	basic_bio_file(u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(file,om,pm),om)
	{}
	basic_bio_file(native_at_entry nate,u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(nate,file,om,pm),om)
	{}
	basic_bio_file(u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(file,om,pm),om)
	{}
	basic_bio_file(native_at_entry nate,u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_bio_file(basic_c_file<char_type>(nate,file,om,pm),om)
	{}
	inline constexpr void reset(native_handle_type newhandle=nullptr) noexcept
	{
		if(this->bio)[[likely]]
			BIO_free(this->bio);
		this->bio=newhandle;
	}


	basic_bio_file(basic_bio_file const&)=delete;
	basic_bio_file& operator=(basic_bio_file const&)=delete;
	constexpr basic_bio_file(basic_bio_file&& bf) noexcept:basic_bio_io_observer<char_type>(bf.bio)
	{
		bf.bio=nullptr;
	}
	basic_bio_file& operator=(basic_bio_file&& bf) noexcept
	{
		if(__builtin_addressof(bf)==this)
			return *this;
		if(this->bio)[[likely]]
			BIO_free(this->bio);
		this->bio=bf.bio;
		bf.bio=nullptr;
		return *this;
	}
	void close()
	{
		if(this->bio)[[likely]]
		{
			int ret{BIO_free(this->bio)};
			this->bio=nullptr;
			if(!ret)[[unlikely]]
				throw_openssl_error();
		}
	}
	~basic_bio_file()
	{
		if(this->bio)[[likely]]
			BIO_free(this->bio);
	}
};

using bio_io_observer =  basic_bio_io_observer<char>;
using bio_file =  basic_bio_file<char>;
using wbio_io_observer =  basic_bio_io_observer<wchar_t>;
using wbio_file =  basic_bio_file<wchar_t>;
using u8bio_io_observer =  basic_bio_io_observer<char8_t>;
using u8bio_file =  basic_bio_file<char8_t>;
using u16bio_io_observer =  basic_bio_io_observer<char16_t>;
using u16bio_file =  basic_bio_file<char16_t>;
using u32bio_io_observer =  basic_bio_io_observer<char32_t>;
using u32bio_file =  basic_bio_file<char32_t>;

namespace details
{
inline std::size_t bio_read_impl(BIO* bio,void* address,std::size_t size)
{
	std::size_t read_bytes{};
	if(BIO_read_ex(bio,address,size,__builtin_addressof(read_bytes))==-1)
		throw_openssl_error();
	return read_bytes;
}

inline std::size_t bio_write_impl(BIO* bio,void const* address,std::size_t size)
{
	std::size_t written_bytes{};
	if(BIO_write_ex(bio,address,size,__builtin_addressof(written_bytes))==-1)
		throw_openssl_error();
	return written_bytes;
}
inline posix_file_status bio_status_impl(BIO* bio)
{
	return status(posix_io_observer{bio_to_fd(bio)});
}

}

template<std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter read(basic_bio_io_observer<ch_type> iob,Iter begin,Iter end)
{
	return begin+details::bio_read_impl(iob.bio,::fast_io::freestanding::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}

template<std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter write(basic_bio_io_observer<ch_type> iob,Iter begin,Iter end)
{
	return begin+details::bio_write_impl(iob.bio,::fast_io::freestanding::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}

#if __cpp_lib_three_way_comparison >= 201907L
template<std::integral ch_type>
inline constexpr bool operator==(basic_bio_io_observer<ch_type> a,basic_bio_io_observer<ch_type> b) noexcept
{
	return a.bio==b.bio;
}

template<std::integral ch_type>
inline constexpr auto operator<=>(basic_bio_io_observer<ch_type> a,basic_bio_io_observer<ch_type> b) noexcept
{
	return a.bio<=>b.bio;
}
#endif

template<std::integral ch_type>
inline constexpr basic_bio_io_observer<ch_type> io_value_handle(basic_bio_io_observer<ch_type> other) noexcept
{
	return other;
}

template<std::integral ch_type>
inline constexpr posix_at_entry at(basic_bio_io_observer<ch_type> bio) noexcept
{
	return posix_at_entry{details::bio_to_fd(bio.bio)};
}

template<std::integral ch_type>
inline constexpr posix_file_status status(basic_bio_io_observer<ch_type> bio)
{
	return details::bio_status_impl(bio.bio);
}

static_assert(input_stream<bio_file>);
static_assert(output_stream<bio_file>);

template<output_stream output,std::integral ch_type>
inline void print_define(output& out,openssl_error const& err)
{
	bio_file bf(io_cookie,out);
	ERR_print_errors(bf.bio);
}
#if 0
inline void openssl_error::report(error_reporter& err) const
{
	bio_file bf(io_cookie,err);
	ERR_print_errors(bf.bio);
}
#endif
}
