#pragma once

namespace fast_io
{

inline constexpr int dll_mode_to_posix_rtld_mode(dll_mode mode) noexcept
{
	int m{};
#if defined(RTLD_LAZY)
	if((mode&dll_mode::posix_rtld_lazy)==dll_mode::posix_rtld_lazy)
		m|=RTLD_LAZY;
#endif
#if defined(RTLD_NOW)
	if((mode&dll_mode::posix_rtld_now)==dll_mode::posix_rtld_now)
		m|=RTLD_NOW;
#endif
#if defined(RTLD_BINDING_MASK)
	if((mode&dll_mode::posix_rtld_binding_mask)==dll_mode::posix_rtld_binding_mask)
		m|=RTLD_BINDING_MASK;
#endif
#if defined(RTLD_GLOBAL)
	if((mode&dll_mode::posix_rtld_global)==dll_mode::posix_rtld_global)
		m|=RTLD_GLOBAL;
#endif
#if defined(RTLD_LOCAL)
	if((mode&dll_mode::posix_rtld_local)==dll_mode::posix_rtld_local)
		m|=RTLD_LOCAL;
#endif
#if defined(RTLD_NODELETE)
	if((mode&dll_mode::posix_rtld_nodelete)==dll_mode::posix_rtld_nodelete)
		m|=RTLD_NODELETE;
#endif
#if defined(RTLD_NOLOAD)
	if((mode&dll_mode::posix_rtld_noload)==dll_mode::posix_rtld_noload)
		m|=RTLD_NOLOAD;
#endif
#if defined(RTLD_DEEPBIND)
	if((mode&dll_mode::posix_rtld_deepbind)==dll_mode::posix_rtld_deepbind)
		m|=RTLD_DEEPBIND;
#endif
	return m;
}

class posix_dll_io_observer
{
public:
	using native_handle_type = void*;
	native_handle_type rtld_handle{};
	inline constexpr native_handle_type& native_handle() noexcept
	{
		return rtld_handle;
	}
	inline constexpr native_handle_type const& native_handle() const noexcept
	{
		return rtld_handle;
	}
	explicit constexpr operator bool() const noexcept
	{
		return rtld_handle;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{rtld_handle};
		rtld_handle=nullptr;
		return temp;
	}
};

struct posix_rtld_error{};

[[noreturn]] inline void throw_posix_rtld_error()
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw posix_rtld_error();
#endif
#else
	fast_terminate();
#endif
}

inline constexpr basic_io_scatter_t<char> print_alias_define(io_alias_t,posix_rtld_error) noexcept
{
	char const* message{noexcept_call(dlerror)};
	if(message==nullptr)
		return {};
	return {message,cstr_len(message)};
}

namespace details
{

inline void* create_posix_rtld(char const* filename,dll_mode mode)
{
	auto hd{noexcept_call(dlopen,filename,dll_mode_to_posix_rtld_mode(mode))};
	if(hd==nullptr)
		throw_posix_rtld_error();
	return hd;
}

template<std::integral char_type>
inline void* create_posix_rtld_api_converter(char_type const* filename,std::size_t size,dll_mode mode)
{
	posix_api_encoding_converter converter(filename,size);
	return create_posix_rtld(reinterpret_cast<char const*>(converter.c_str()),mode);
}

}

class
#if __has_cpp_attribute(gnu::trivial_abi)
[[gnu::trivial_abi]]
#endif
posix_dll_file:public posix_dll_io_observer
{
public:
	using native_handle_type = void*;
	constexpr posix_dll_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr posix_dll_file(native_hd handle) noexcept:posix_dll_io_observer{handle}{}
	explicit posix_dll_file(cstring_view filename,dll_mode mode):posix_dll_io_observer{details::create_posix_rtld(filename.c_str(),mode)}{}
	explicit posix_dll_file(u8cstring_view filename,dll_mode mode):posix_dll_io_observer{details::create_posix_rtld(reinterpret_cast<char const*>(filename.c_str()),mode)}{}
	explicit posix_dll_file(wcstring_view filename,dll_mode mode):posix_dll_io_observer{details::create_posix_rtld_api_converter(filename.c_str(),filename.size(),mode)}{}
	explicit posix_dll_file(u16cstring_view filename,dll_mode mode):posix_dll_io_observer{details::create_posix_rtld_api_converter(filename.c_str(),filename.size(),mode)}{}
	explicit posix_dll_file(u32cstring_view filename,dll_mode mode):posix_dll_io_observer{details::create_posix_rtld_api_converter(filename.c_str(),filename.size(),mode)}{}
	posix_dll_file(posix_dll_file const&)=delete;
	posix_dll_file& operator=(posix_dll_file const&)=delete;
	constexpr posix_dll_file(posix_dll_file&& other) noexcept:posix_dll_io_observer{other.rtld_handle}
	{
		other.rtld_handle=nullptr;
	}
	posix_dll_file& operator=(posix_dll_file&& other) noexcept
	{
		if(__builtin_addressof(other)==this)[[unlikely]]
			return *this;
		if(this->rtld_handle)[[likely]]
			noexcept_call(dlclose,this->rtld_handle);
		this->rtld_handle=other.rtld_handle;
		other.rtld_handle=nullptr;
		return *this;
	}
	void close()
	{
		if(this->rtld_handle)[[likely]]
		{
			auto ret{noexcept_call(dlclose,this->rtld_handle)};
			this->rtld_handle=nullptr;
			if(ret)
				throw_posix_rtld_error();
		}
	}
	~posix_dll_file()
	{
		if(this->rtld_handle)[[likely]]
			noexcept_call(dlclose,this->rtld_handle);
	}
};

inline void* dll_load_symbol(posix_dll_io_observer pdliob,char const* symbol)
{
	auto ptr{noexcept_call(dlsym,pdliob.rtld_handle,symbol)};
	if(ptr==nullptr)[[unlikely]]
		throw_posix_error(EINVAL);
	return ptr;
}

inline void* dll_load_symbol(posix_dll_io_observer pdliob,char8_t const* symbol)
{
	return dll_load_symbol(pdliob,reinterpret_cast<char const*>(symbol));
}

using native_dll_io_observer = posix_dll_io_observer;
using native_dll_file = posix_dll_file;

}
