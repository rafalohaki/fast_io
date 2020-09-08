#pragma once
struct bio_method_st
{
    int type;
    char const *name;
    int (*bwrite) (BIO *, const char *, size_t, size_t *);
    int (*bwrite_old) (BIO *, const char *, int);
    int (*bread) (BIO *, char *, size_t, size_t *);
    int (*bread_old) (BIO *, char *, int);
    int (*bputs) (BIO *, const char *);
    int (*bgets) (BIO *, char *, int);
    long (*ctrl) (BIO *, int, long, void *);
    int (*create) (BIO *);
    int (*destroy) (BIO *);
    long (*callback_ctrl) (BIO *, int, BIO_info_cb *);
};

namespace fast_io::openssl
{

namespace details
{
template<bool detach>
inline constexpr int calculate_bio_new_fp_flags(open_mode om)
{
	int flag{BIO_CLOSE};
	if constexpr(!detach)
		flag=BIO_NOCLOSE;
	if((om&open_mode::binary)==open_mode::none)
		flag|=BIO_FP_TEXT;
	return flag;
}

template<open_mode om,bool detach>
struct bio_new_fp_flags
{
	inline static constexpr int value=calculate_bio_new_fp_flags<detach>(om);
};
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
		functions.name=typeid(stm).name();
		constexpr int value(BIO_TYPE_DESCRIPTOR-BIO_TYPE_START);
		static_assert(0<value);
		functions.type=static_cast<int>(typeid(stm).hash_code()%value+BIO_TYPE_START);
	}
};

template<typename stm>
bio_io_cookie_functions_t<stm> const bio_io_cookie_functions{};

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
		std::FILE* fp{};
		BIO_get_fp(bio,std::addressof(fp));
		return {fp};
	}
	explicit operator basic_posix_io_observer<char_type>() const noexcept
	{
		auto c_iob{static_cast<basic_c_io_observer<char_type>>(*this)};
		if(c_iob)
			return static_cast<basic_posix_io_observer<char_type>>(c_iob);
		int fd{-1};
		BIO_get_fd(bio,std::addressof(fd));
		return {fd};
	}
#if defined(_WIN32)
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return basic_win32_io_observer<char_type>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	explicit operator basic_nt_io_observer<char_type>() const noexcept
	{
		return basic_nt_io_observer<char_type>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
};
template<std::integral ch_type>
class basic_bio_file:public basic_bio_io_observer<ch_type>
{
	void detect_open_failure()
	{
		if(this->native_handle()==nullptr)[[unlikely]]
			throw_openssl_error();
	}
public:
	using native_handle_type = BIO*;
	using char_type = ch_type;
	constexpr basic_bio_file()=default;
	constexpr basic_bio_file(native_handle_type bio):basic_bio_io_observer<char_type>(bio){}
	template<stream stm,typename ...Args>
	requires std::constructible_from<stm,Args...>
	basic_bio_file(io_cookie_t,std::in_place_type_t<stm>,Args&& ...args):basic_bio_io_observer<char_type>(BIO_new(std::addressof(bio_io_cookie_functions<stm>.functions)))
	{
		detect_open_failure();
		basic_bio_file<char_type> self(this->native_handle());
		BIO_set_data(this->native_handle(),bit_cast<void*>(new stm(std::forward<Args>(args)...)));
		self.release();
	}
	template<stream stm>
	basic_bio_file(io_cookie_t,stm& sm):basic_bio_io_observer<char_type>(BIO_new(std::addressof(bio_io_cookie_functions<stm&>.functions)))
	{
		detect_open_failure();
		basic_bio_file<char_type> self(this->native_handle());
		BIO_set_data(this->native_handle(),bit_cast<void*>(std::addressof(sm)));
		self.release();
	}

	template<stream stm>
	basic_bio_file(io_cookie_t,stm&& sm):basic_bio_file<char_type>(io_cookie,std::in_place_type<stm>,std::move(sm)){}


	template<fast_io::open_mode om>
	basic_bio_file(basic_c_io_handle<char_type>&& bmv,open_interface_t<om>):
		basic_bio_io_observer<char_type>(BIO_new_fp(bmv.native_handle(),details::bio_new_fp_flags<om,true>::value))
	{
		detect_open_failure();
		bmv.release();
	}

	basic_bio_file(basic_c_io_handle<char_type>&& bmv,fast_io::open_mode om):
		basic_bio_io_observer<char_type>(BIO_new_fp(bmv.native_handle(),details::calculate_bio_new_fp_flags<true>(om)))
	{
		detect_open_failure();
		bmv.release();
	}

	template<fast_io::open_mode om>
	basic_bio_file(basic_posix_io_handle<char_type>&& bmv,open_interface_t<om>):
		basic_bio_io_observer<char_type>(BIO_new_fd(bmv.native_handle(),details::bio_new_fp_flags<om,true>::value))
	{
		detect_open_failure();
		bmv.release();
	}
	basic_bio_file(basic_posix_io_handle<char_type>&& bmv,fast_io::open_mode om):
		basic_bio_io_observer<char_type>(BIO_new_fd(bmv.native_handle(),details::calculate_bio_new_fp_flags<true>(om)))
	{
		detect_open_failure();
		bmv.release();
	}



#if defined(_WIN32)
	template<typename... Args>
	basic_bio_file(basic_win32_io_handle<char_type>&& bmv,Args&& ...args):
		basic_bio_file(basic_posix_file(std::move(bmv),std::forward<Args>(args)...),std::forward<Args>(args)...)
	{
	}
#endif
	template<open_mode om,typename... Args>
	basic_bio_file(cstring_view file,open_interface_t<om>,Args&& ...args):
		basic_bio_file(basic_c_file<char_type>(file,open_interface<om>,std::forward<Args>(args)...),open_interface<om>)
	{}
	template<typename... Args>
	basic_bio_file(cstring_view file,open_mode om,Args&& ...args):
		basic_bio_file(basic_c_file<char_type>(file,om,std::forward<Args>(args)...),om)
	{}

	template<open_mode om,typename... Args>
	basic_bio_file(io_at_t,native_io_observer niob,cstring_view file,open_interface_t<om>,Args&& ...args):
		basic_bio_file(basic_c_file<char_type>(at,niob,file,open_interface<om>,std::forward<Args>(args)...),open_interface<om>)
	{}
	template<typename... Args>
	basic_bio_file(io_at_t,native_io_observer niob,cstring_view file,open_mode om,Args&& ...args):
		basic_bio_file(basic_c_file<char_type>(at,niob,file,om,std::forward<Args>(args)...),om)
	{}


	inline constexpr void reset(native_handle_type newhandle=nullptr) noexcept
	{
		if(this->native_handle())[[likely]]
			BIO_free(this->native_handle());
		this->native_handle()=newhandle;
	}


	basic_bio_file(basic_bio_file const&)=delete;
	basic_bio_file& operator=(basic_bio_file const&)=delete;
	constexpr basic_bio_file(basic_bio_file&& bf) noexcept:basic_bio_io_observer<char_type>(bf.native_handle())
	{
		bf.native_handle()=nullptr;
	}
	basic_bio_file& operator=(basic_bio_file&& bf) noexcept
	{
		if(bf.native_handle()==this->native_handle())
			return *this;
		if(this->native_handle())[[likely]]
			BIO_free(this->native_handle());
		this->native_handle()=bf.native_handle();
		bf.native_handle()=nullptr;
		return *this;
	}
	~basic_bio_file()
	{
		if(this->native_handle())[[likely]]
			BIO_free(this->native_handle());
	}
};

using bio_io_observer =  basic_bio_io_observer<char>;
using bio_file =  basic_bio_file<char>;

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_bio_io_observer<ch_type> iob,Iter begin,Iter end)
{
	std::size_t read_bytes{};
	if(BIO_read_ex(iob.native_handle(),std::to_address(begin),
		sizeof(*begin)*(std::to_address(end)-std::to_address(begin)),std::addressof(read_bytes))==-1)
		throw_openssl_error();
	return begin+read_bytes/sizeof(*begin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_bio_io_observer<ch_type> iob,Iter begin,Iter end)
{
	std::size_t written_bytes{};
	if(BIO_write_ex(iob.native_handle(),std::to_address(begin),
		sizeof(*begin)*(std::to_address(end)-std::to_address(begin)),std::addressof(written_bytes))==-1)
		throw_openssl_error();
	return begin+written_bytes/sizeof(*begin);
}

template<std::integral ch_type>
inline constexpr basic_bio_io_observer<ch_type> io_value_handle(basic_bio_io_observer<ch_type> other) noexcept
{
	return other;
}

static_assert(input_stream<bio_file>);
static_assert(output_stream<bio_file>);

template<output_stream output,std::integral ch_type>
constexpr void print_define(output& out,basic_bio_io_observer<ch_type> bio)
{
	print(out,bio.native_handle());
}

template<output_stream output,std::integral ch_type>
inline void print_define(output& out,openssl_error const& err)
{
	bio_file bf(io_cookie,out);
	ERR_print_errors(bf.native_handle());
}

inline void openssl_error::report(error_reporter& err) const
{
	bio_file bf(io_cookie,err);
	ERR_print_errors(bf.native_handle());
}
}
