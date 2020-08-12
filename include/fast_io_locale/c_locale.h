#pragma once

namespace fast_io
{

enum class c_locale_category:int
{
all=
#ifdef LC_ALL
LC_ALL
#else
0
#endif
,

collate=
#ifdef LC_COLLATE
LC_COLLATE
#else
0
#endif
,
ctype=
#ifdef LC_CTYPE
LC_CTYPE
#else
0
#endif
,
monetary=
#ifdef LC_MONETARY
LC_MONETARY
#else
0
#endif
,
numeric=
#ifdef LC_NUMERIC
LC_NUMERIC
#else
0
#endif
,
time=
#ifdef LC_TIME
LC_TIME
#else
0
#endif
,

address=
#ifdef LC_ADDRESS
LC_ADDRESS
#else
0
#endif
,

identification=
#ifdef LC_IDENTIFICATION
LC_IDENTIFICATION
#else
0
#endif
,

measurement=
#ifdef LC_MEASUREMENT
LC_MEASUREMENT
#else
0
#endif
,

messages=
#ifdef LC_MESSAGES
LC_MESSAGES
#else
0
#endif
,
name=
#ifdef LC_NAME
LC_NAME
#else
0
#endif
,
paper=
#ifdef LC_PAPER
LC_PAPER
#else
0
#endif
,
telephone=
#ifdef LC_TELEPHONE
LC_TELEPHONE
#else
0
#endif
,
xliterate=
#ifdef LC_XLITERATE
LC_XLITERATE
#else
0
#endif
,
keyboard=
#ifdef LC_KEYBOARD
LC_KEYBOARD
#else
0
#endif
,

none=0
};

constexpr c_locale_category operator&(c_locale_category x, c_locale_category y) noexcept
{
using utype = typename std::underlying_type<c_locale_category>::type;
return static_cast<c_locale_category>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr c_locale_category operator|(c_locale_category x, c_locale_category y) noexcept
{
using utype = typename std::underlying_type<c_locale_category>::type;
return static_cast<c_locale_category>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr c_locale_category operator^(c_locale_category x, c_locale_category y) noexcept
{
using utype = typename std::underlying_type<c_locale_category>::type;
return static_cast<c_locale_category>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr c_locale_category operator~(c_locale_category x) noexcept
{
using utype = typename std::underlying_type<c_locale_category>::type;
return static_cast<c_locale_category>(~static_cast<utype>(x));
}

inline constexpr c_locale_category& operator&=(c_locale_category& x, c_locale_category y) noexcept{return x=x&y;}

inline constexpr c_locale_category& operator|=(c_locale_category& x, c_locale_category y) noexcept{return x=x|y;}

inline constexpr c_locale_category& operator^=(c_locale_category& x, c_locale_category y) noexcept{return x=x^y;}

class c_locale_observer
{
public:
	using native_handle_type = 
#if defined(__WINNT__) || defined(_MSC_VER)
_locale_t
#else
locale_t
#endif
	;
	native_handle_type loc{};
	constexpr auto& native_handle() const noexcept
	{
		return loc;
	}
	constexpr auto& native_handle() noexcept
	{
		return loc;
	}
	constexpr operator bool() const noexcept
	{
		return loc!=static_cast<native_handle_type>(0);
	}
	auto release() noexcept
	{
		auto d{loc};
		loc=static_cast<native_handle_type>(0);
		return d;
	}
	inline constexpr void reset(native_handle_type newloc={}) noexcept
	{
		loc=newloc;
	}
	inline constexpr void swap(c_locale_observer& other) noexcept
	{
		std::swap(loc, other.loc);
	}
};

class c_locale_handle:public c_locale_observer
{
public:
	using native_handle_type = c_locale_observer::native_handle_type;
	constexpr c_locale_handle()=default;
	constexpr c_locale_handle(native_handle_type hd):c_locale_observer(hd){}
	void close()
	{
	if(*this)[[likely]]
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		_free_locale(this->native_handle());
#else
		freelocale(this->native_handle());
#endif
		this->native_handle()=static_cast<native_handle_type>(0);
	}		
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	c_locale_handle(c_locale_handle const&)=delete;
	c_locale_handle& operator=(c_locale_handle const&)=delete;
#else
	c_locale_handle(c_locale_handle const& c):c_locale_observer(duplocale(c.native_handle()))
	{
		if(!*this)
			throw_posix_error();
	}
	c_locale_handle& operator=(c_locale_handle const& c)
	{
		auto cloned{duplocale(c.native_handle())};
		if(cloned==static_cast<native_handle_type>(0))
			throw_posix_error();
		if(*this)[[likely]]
#if defined(__WINNT__) || defined(_MSC_VER)
			_free_locale(this->native_handle());
#else
			freelocale(this->native_handle());
#endif
		this->native_handle()=cloned;
		return *this;
	}
#endif

	constexpr c_locale_handle(c_locale_handle&& bmv) noexcept:c_locale_observer(bmv.native_handle())
	{
		bmv.native_handle()=static_cast<native_handle_type>(0);
	}
	c_locale_handle& operator=(c_locale_handle&& bmv) noexcept
	{
		if(this->native_handle()==bmv.native_handle())
			return *this;
		
		if(*this)[[likely]]
#if defined(__WINNT__) || defined(_MSC_VER)
			_free_locale(this->native_handle());
#else
			freelocale(this->native_handle());
#endif
		this->native_handle()=bmv.native_handle();
		bmv.native_handle()=static_cast<native_handle_type>(0);
		return *this;
	}
};

class c_locale:public c_locale_handle
{
public:
	using native_handle_type = c_locale_handle::native_handle_type;
	constexpr c_locale()=default;
	constexpr c_locale(native_handle_type hd):c_locale_handle(hd){}
	c_locale(c_locale_category catg,std::string_view loc):
		c_locale_handle(
#if defined(__WINNT__) || defined(_MSC_VER)
	_create_locale(static_cast<int>(catg),loc=="POSIX"?"C":loc.data())
#else
	newlocale(static_cast<int>(catg),loc.data(),static_cast<locale_t>(0))
#endif
)
{
	if(!*this)
#ifdef __cpp_exceptions
		throw fast_io_text_error("unknown locale");
#else
		fast_terminate();
#endif
}
	c_locale(c_locale_category catg):
		c_locale_handle(
#if defined(__WINNT__) || defined(_MSC_VER)
	_create_locale(static_cast<int>(catg),"")
#else
	newlocale(static_cast<int>(catg),"",static_cast<locale_t>(0))
#endif
)
{
	if(!*this)
#ifdef __cpp_exceptions
		throw fast_io_text_error("unknown locale");
#else
		fast_terminate();
#endif
}
	~c_locale()
	{
	if(*this)[[likely]]
#if defined(__WINNT__) || defined(_MSC_VER)
		_free_locale(this->native_handle());
#else
		freelocale(this->native_handle());
#endif
	}
};

}