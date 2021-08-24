#pragma once

namespace fast_io
{

inline constexpr std::uint32_t dll_mode_to_win32_ex_flags(dll_mode mode) noexcept
{
	std::uint32_t flags{};
	if((mode&dll_mode::win32_dont_resolve_dll_references)==dll_mode::win32_dont_resolve_dll_references)
		flags|=0x00000001;
	if((mode&dll_mode::win32_load_ignore_code_authz_level)==dll_mode::win32_load_ignore_code_authz_level)
		flags|=0x00000010;
	if((mode&dll_mode::win32_load_library_as_datafile)==dll_mode::win32_load_library_as_datafile)
		flags|=0x00000002;
	if((mode&dll_mode::win32_load_library_as_datafile_exclusive)==dll_mode::win32_load_library_as_datafile_exclusive)
		flags|=0x00000040;
	if((mode&dll_mode::win32_load_library_as_image_resource)==dll_mode::win32_load_library_as_image_resource)
		flags|=0x00000020;
	if((mode&dll_mode::win32_load_library_search_application_dir)==dll_mode::win32_load_library_search_application_dir)
		flags|=0x00000200;
	if((mode&dll_mode::win32_load_library_search_default_dirs)==dll_mode::win32_load_library_search_default_dirs)
		flags|=0x00001000;
	if((mode&dll_mode::win32_load_library_search_dll_load_dir)==dll_mode::win32_load_library_search_dll_load_dir)
		flags|=0x00000100;
	if((mode&dll_mode::win32_load_library_search_system32)==dll_mode::win32_load_library_search_system32)
		flags|=0x00000800;
	if((mode&dll_mode::win32_load_library_search_user_dirs)==dll_mode::win32_load_library_search_user_dirs)
		flags|=0x00000400;
	if((mode&dll_mode::win32_load_with_altered_search_path)==dll_mode::win32_load_with_altered_search_path)
		flags|=0x00000008;
	if((mode&dll_mode::win32_load_library_require_signed_target)==dll_mode::win32_load_library_require_signed_target)
		flags|=0x00000080;
	if((mode&dll_mode::win32_load_library_safe_current_dirs)==dll_mode::win32_load_library_safe_current_dirs)
		flags|=0x00002000;
	return flags;
}

template<win32_family family>
class win32_family_dll_io_observer
{
public:
	using native_handle_type = void*;
	native_handle_type hmodule{};
	inline constexpr native_handle_type& native_handle() noexcept
	{
		return hmodule;
	}
	inline constexpr native_handle_type const& native_handle() const noexcept
	{
		return hmodule;
	}
	explicit constexpr operator bool() const noexcept
	{
		return hmodule;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{hmodule};
		hmodule=nullptr;
		return temp;
	}
};

namespace details
{

inline void* create_win32_dll_9xa(char const* filename)	//9x kernel does not support Ex apis
{
	auto hmodule{::fast_io::win32::LoadLibraryA(filename)};
	if(hmodule==nullptr)
		throw_win32_error();
	return hmodule;
}
inline void* create_win32_dll_ntw(wchar_t const* filename,dll_mode mode)
{
	auto hmodule{
#if defined(_WIN32_WINNT) && _WIN32_WINNT <= 0x0500
//Windows 2000 does not support Ex apis
::fast_io::win32::LoadLibraryW(filename)
#else
::fast_io::win32::LoadLibraryExW(filename,nullptr,dll_mode_to_win32_ex_flags(mode))
#endif
};
	if(hmodule==nullptr)
		throw_win32_error();
	return hmodule;
}

template<std::integral char_type>
inline void* create_win32_dll_9x_main_impl(basic_cstring_view<char_type> view)
{
	posix_api_encoding_converter converter(view.data(),view.size());
	return create_win32_dll_9xa(reinterpret_cast<char const*>(converter.c_str()));
}

template<win32_family family,std::integral char_type>
requires (family==win32_family::ansi_9x||family==win32_family::wide_nt)
inline void* create_win32_dll_main_impl(basic_cstring_view<char_type> view,dll_mode mode)
{
	if constexpr(family==win32_family::ansi_9x)
	{
		if constexpr(sizeof(char_type)==1)
			return create_win32_dll_9xa(view.c_str());
		else
			return create_win32_dll_9x_main_impl(view);
	}
	else
	{
		if constexpr(std::same_as<char_type,wchar_t>)
		{
			return create_win32_dll_ntw(view.c_str(),mode);
		}
		else if constexpr(sizeof(char_type)==2)
		{
			using general_wchar_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
			[[gnu::may_alias]]
#endif
			= wchar_t const*;
			return create_win32_dll_ntw(reinterpret_cast<general_wchar_may_alias_ptr>(view.c_str()),mode);
		}
		else
		{
			nt_api_encoding_converter converter(view.data(),view.size());
			using general_wchar_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
			[[gnu::may_alias]]
#endif
			= wchar_t const*;
			return create_win32_dll_ntw(reinterpret_cast<general_wchar_may_alias_ptr>(converter.c_str()),mode);
		}
	}
}

}

template<win32_family family>
class
#if __has_cpp_attribute(gnu::trivial_abi)
[[gnu::trivial_abi]]
#endif
win32_family_dll_file:public win32_family_dll_io_observer<family>
{
public:
	using native_handle_type = void*;
	constexpr win32_family_dll_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr win32_family_dll_file(native_hd handle) noexcept:win32_family_dll_io_observer<family>{handle}{}
	explicit win32_family_dll_file(cstring_view filename,dll_mode mode):win32_family_dll_io_observer<family>{details::create_win32_dll_main_impl<family>(filename,mode)}{}
	explicit win32_family_dll_file(u8cstring_view filename,dll_mode mode):win32_family_dll_io_observer<family>{details::create_win32_dll_main_impl<family>(filename,mode)}{}
	explicit win32_family_dll_file(wcstring_view filename,dll_mode mode):win32_family_dll_io_observer<family>{details::create_win32_dll_main_impl<family>(filename,mode)}{}
	explicit win32_family_dll_file(u16cstring_view filename,dll_mode mode):win32_family_dll_io_observer<family>{details::create_win32_dll_main_impl<family>(filename,mode)}{}
	explicit win32_family_dll_file(u32cstring_view filename,dll_mode mode):win32_family_dll_io_observer<family>{details::create_win32_dll_main_impl<family>(filename,mode)}{}
	win32_family_dll_file(win32_family_dll_file const&)=delete;
	win32_family_dll_file& operator=(win32_family_dll_file const&)=delete;
	constexpr win32_family_dll_file(win32_family_dll_file&& other) noexcept:win32_family_dll_io_observer<family>{other.rtld_handle}
	{
		other.rtld_handle=nullptr;
	}
	win32_family_dll_file& operator=(win32_family_dll_file&& other) noexcept
	{
		if(__builtin_addressof(other)==this)[[unlikely]]
			return *this;
		if(this->hmodule)[[likely]]
			win32::FreeLibrary(this->hmodule);
		this->hmodule=other.hmodule;
		other.hmodule=nullptr;
		return *this;
	}
	void close()
	{
		if(this->hmodule)[[likely]]
		{
			auto ret{win32::FreeLibrary(this->hmodule)};
			this->hmodule=nullptr;
			if(!ret)
				throw_win32_error();
		}
	}
	~win32_family_dll_file()
	{
		if(this->hmodule)[[likely]]
			win32::FreeLibrary(this->hmodule);
	}
};

namespace details
{
inline void* win32_dll_load_symbol_impl(void* hmodule,char const* symbol)
{
	auto ptr{::fast_io::win32::GetProcAddress(hmodule,symbol)};
	if(ptr==nullptr)[[unlikely]]
		throw_win32_error();
	return reinterpret_cast<void*>(ptr);
}

}

template<win32_family family>
inline void* dll_load_symbol(win32_family_dll_io_observer<family> pdliob,char const* symbol)
{
	return details::win32_dll_load_symbol_impl(pdliob.hmodule,symbol);
}

template<win32_family family>
inline void* dll_load_symbol(win32_family_dll_io_observer<family> pdliob,char8_t const* symbol)
{
	return details::win32_dll_load_symbol_impl(pdliob.hmodule,reinterpret_cast<char const*>(symbol));
}

using win32_dll_io_observer_9xa = win32_family_dll_io_observer<win32_family::ansi_9x>;
using win32_dll_file_9xa = win32_family_dll_file<win32_family::ansi_9x>;

using win32_dll_io_observer_ntw = win32_family_dll_io_observer<win32_family::wide_nt>;
using win32_dll_file_ntw = win32_family_dll_file<win32_family::wide_nt>;

using win32_dll_io_observer = win32_family_dll_io_observer<win32_family::native>;
using win32_dll_file = win32_family_dll_file<win32_family::native>;

#if !defined(__CYGWIN__) && !defined(__WINE__)
using native_dll_io_observer = win32_dll_io_observer;
using native_dll_file = win32_dll_file;
#endif

}
