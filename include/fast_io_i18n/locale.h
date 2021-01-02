#pragma once

#include"lc.h"
#include"lc_print.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif

namespace fast_io
{


#ifndef _WIN32

class posix_dl_error:public fast_io_error
{
public:
	virtual	void report(error_reporter& err) const override
	{
		print(err,fast_io::chvw(dlerror()));
	}
};

[[noreturn]] inline void throw_posix_dl_error()
{
#ifdef __cpp_exceptions
	throw posix_dl_error();
#else
	fast_terminate();
#endif
}

#endif
namespace details
{

struct close_dll
{
	void operator()(void* handle) const noexcept
	{
		if(handle)[[likely]]
#ifdef _WIN32
			fast_io::win32::FreeLibrary(handle);
#else
			dlclose(handle);	
#endif
	}
};

inline void load_lc_locale(void* dll_handle,lc_locale& loc)
{
#ifdef _WIN32
	auto func(bit_cast<void __stdcall(*)(lc_locale*) noexcept >(fast_io::win32::GetProcAddress(dll_handle,"export_locale_data")));
#else
	auto func(bit_cast<void (*)(lc_locale*) noexcept >(dlsym(dll_handle,"export_locale_data")));
#endif

	if(func==nullptr)
#ifdef _WIN32
		throw_win32_error();
#else
		throw_posix_dl_error();
#endif
	func(std::addressof(loc));
}

template<std::integral T>
requires (std::same_as<T,char>
#ifdef _WIN32
||std::same_as<T,wchar_t>
#endif
)
inline void* load_library_general(T const* loc_file_path) noexcept
{
#ifdef _WIN32
	if constexpr(std::same_as<T,wchar_t>)
		return fast_io::win32::LoadLibraryW(loc_file_path);
	else
		return fast_io::win32::LoadLibraryA(loc_file_path);
#else
	return dlopen(loc_file_path,RTLD_GLOBAL | RTLD_NOW);
#endif
}


template<typename T>
requires (std::same_as<T,char>
#ifdef _WIN32
||std::same_as<T,wchar_t>
#endif
)
inline void* load_dll(T const* loc_file_path,lc_locale& loc)
{
	std::unique_ptr<void,close_dll> ptr{load_library_general(loc_file_path)};
	if(!ptr)
#ifdef _WIN32
		throw_win32_error();
#else
		throw_posix_dl_error();
#endif
	load_lc_locale(ptr.get(),loc);
	return ptr.release();
}

}

struct system_locale_t
{
explicit constexpr system_locale_t()=default;
};

inline constexpr system_locale_t system_locale{};

struct locale_fullname_t
{
explicit constexpr locale_fullname_t()=default;
};

inline constexpr locale_fullname_t locale_fullname{};

namespace details
{
template<typename char_type1,std::size_t n1,typename char_type2,std::size_t n2>
inline constexpr bool compile_time_compare(char_type1 const (&a)[n1],char_type2 const (&b)[n2]) noexcept
{
	if constexpr((sizeof(char_type1)!=sizeof(char_type2))||(n1!=n2))
		return false;
	for(std::size_t i{};i!=n1;++i)
	{
		if(static_cast<std::make_unsigned_t<char_type1>>(a[i])!=static_cast<std::make_unsigned_t<char_type2>>(b[i]))
			return false;
	}
	return true;
}

template<std::integral char_type>
requires (std::same_as<char_type,char>||(std::same_as<char_type,wchar_t>&&(sizeof(wchar_t)==sizeof(char16_t))))
inline constexpr auto exec_encoding_dll_array() noexcept
{
	if constexpr(std::same_as<wchar_t,char_type>)
	{
		if constexpr('A'!=u8'A')
			return std::to_array(u".IBM12712.so");
		else if constexpr(details::compile_time_compare("我",u8"我"))
			return std::to_array(u".UTF-8.so");
		else
			return std::to_array(u".GB18030.so");
	}
	else
	{
	if constexpr('A'!=u8'A')
		return std::to_array(u8".IBM12712.so");
	else if constexpr(details::compile_time_compare("我",u8"我"))
		return std::to_array(u8".UTF-8.so");
	else
		return std::to_array(u8".GB18030.so");
	}
}

template<std::integral char_type>
requires (std::same_as<char_type,char>||(std::same_as<char_type,wchar_t>&&(sizeof(wchar_t)==sizeof(char16_t))))
inline constexpr auto exec_dll_array() noexcept
{
	if constexpr(std::same_as<wchar_t,char_type>)
		return std::to_array(u".so");
	else
		return std::to_array(u8".so");
}

template<std::integral char_type>
requires (std::same_as<char_type,char>||(std::same_as<char_type,wchar_t>&&(sizeof(wchar_t)==sizeof(char16_t))))
inline constexpr auto l10n_path_prefix_dll_array() noexcept
{
#ifdef _WIN32
	if constexpr(std::same_as<wchar_t,char_type>)
		return std::to_array(u"fast_io_i18n_data\\locale\\");
	else
		return std::to_array(u8"fast_io_i18n_data\\locale\\");
#else
	if constexpr(std::same_as<wchar_t,char_type>)
		return std::to_array(u"/usr/local/lib/fast_io_i18n_data/locale/");
	else
		return std::to_array(u8"/usr/local/lib/fast_io_i18n_data/locale/");
#endif
}

template<std::integral char_type>
inline void* load_l10n_with_real_name_impl(lc_locale& loc,std::basic_string_view<char_type> locale_name)
{
	constexpr auto prefix(details::l10n_path_prefix_dll_array<char_type>());
	constexpr auto encoding{details::exec_encoding_dll_array<char_type>()};
	constexpr auto prefix_no_0_size{prefix.size()-1};
	std::unique_ptr<char_type[]> arrptr(new char_type[prefix_no_0_size+locale_name.size()+encoding.size()]);
	memcpy(arrptr.get(),prefix.data(),prefix_no_0_size*sizeof(char_type));
	memcpy(arrptr.get()+prefix_no_0_size,locale_name.data(),locale_name.size()*sizeof(char_type));
	memcpy(arrptr.get()+prefix_no_0_size+locale_name.size(),encoding.data(),encoding.size()*sizeof(char_type));
	return details::load_dll(arrptr.get(),loc);
}

template<std::integral char_type>
inline void* load_l10n_with_real_name_impl(lc_locale& loc,std::basic_string_view<char_type> locale_name,std::basic_string_view<char_type> encoding)
{
	constexpr auto prefix(details::l10n_path_prefix_dll_array<char_type>());
	constexpr auto prefix_no_0_size{prefix.size()-1};
	constexpr auto dll_postfix{exec_dll_array<char_type>()};
	std::unique_ptr<char_type[]> arrptr(new char_type[prefix_no_0_size+locale_name.size()+1+encoding.size()+dll_postfix.size()]);
	memcpy(arrptr.get(),prefix.data(),prefix_no_0_size*sizeof(char_type));
	memcpy(arrptr.get()+prefix_no_0_size,locale_name.data(),locale_name.size()*sizeof(char_type));
	arrptr[prefix_no_0_size+locale_name.size()]=u8'.';
	if(encoding.size())
		memcpy(arrptr.get()+prefix_no_0_size+locale_name.size()+1,encoding.data(),encoding.size()*sizeof(char_type));
	memcpy(arrptr.get()+prefix_no_0_size+locale_name.size()+1+encoding.size(),dll_postfix.data(),dll_postfix.size()*sizeof(char_type));
	return details::load_dll(arrptr.get(),loc);
}

template<std::integral char_type>
inline void* load_l10n_with_full_name_impl(lc_locale& loc,std::basic_string_view<char_type> locale_fullname)
{
	constexpr auto prefix(details::l10n_path_prefix_dll_array<char_type>());
	constexpr auto prefix_no_0_size{prefix.size()-1};
	constexpr auto dll_postfix{exec_dll_array<char_type>()};
	std::unique_ptr<char_type[]> arrptr(new char_type[prefix_no_0_size+locale_fullname.size()+dll_postfix.size()]);
	memcpy(arrptr.get(),prefix.data(),prefix_no_0_size*sizeof(char_type));
	memcpy(arrptr.get()+prefix_no_0_size,locale_fullname.data(),locale_fullname.size()*sizeof(char_type));
	memcpy(arrptr.get()+prefix_no_0_size+locale_fullname.size(),dll_postfix.data(),dll_postfix.size()*sizeof(char_type));
	return details::load_dll(arrptr.get(),loc);
}

#ifdef _WIN32

inline std::string_view get_win32_lang_env(std::array<char,64>& buffer) noexcept
{
	std::array<wchar_t,64> wbuffer;
	std::size_t size(win32::GetUserDefaultLocaleName(wbuffer.data(),wbuffer.size()));
	if(!size)
		return reinterpret_cast<char const*>(u8"C");
	--size;
	for(std::size_t i{};i!=size;++i)
		if(127<static_cast<std::make_unsigned_t<wchar_t>>(wbuffer[i]))
			return reinterpret_cast<char const*>(u8"C");
		else if(wbuffer[i]==u'-')
			buffer[i]=u8'_';
		else
			buffer[i]=static_cast<char>(wbuffer[i]);
	return std::string_view(buffer.data(),size);
}

extern "C" std::uint32_t __stdcall GetACP() noexcept;

inline constexpr std::u8string_view get_code_page_encoding(std::uint32_t codepage) noexcept
{
//we only deal with GB2312/GB18030 and UTF-8
	switch(codepage)
	{
	case 936:case 54936:return u8"GB18030";
	default:
		return u8"UTF-8";
	}
}

/*
Since MSYS2 console respects POSIX locales. We should support it too.
*/

extern "C" errno_t __stdcall getenv_s(std::size_t *,char* buffer,std::size_t ,char const *) noexcept;

inline std::string_view acp_encoding_name() noexcept
{
	auto strvw{get_code_page_encoding(GetACP())};
	return {reinterpret_cast<char const*>(strvw.data()),strvw.size()};
}

inline std::size_t get_lc_all_or_lang(std::array<char,64>& buffer) noexcept
{
	std::size_t size_buffer{};
	if(!getenv_s(std::addressof(size_buffer),buffer.data(),buffer.size(),reinterpret_cast<char const*>(u8"LC_ALL")))
		return size_buffer;
	if(!getenv_s(std::addressof(size_buffer),buffer.data(),buffer.size(),reinterpret_cast<char const*>(u8"LANG")))
		return size_buffer;
	return 0;
}

struct language_encoding_views
{
	std::string_view language,encoding;
};

inline language_encoding_views win32_get_language_encoding_based_on_lc_all_or_lang(std::array<char,64>& buffer) noexcept
{
	std::size_t size{get_lc_all_or_lang(buffer)};
	if(size)
		--size;
	std::string_view buffer_strvw(buffer.data(),size);
	std::size_t dot_pos{buffer_strvw.find(u8'.')};
	std::string_view language;
	std::string_view encoding;
	if(dot_pos==std::string_view::npos)
		language=buffer_strvw;
	else
	{
		language=buffer_strvw.substr(0,dot_pos);
		encoding=buffer_strvw.substr(dot_pos+1);
	}
	return {language,encoding};
}

inline std::string_view win32_only_get_language_not_encoding(std::array<char,64>& buffer) noexcept
{
	auto language{win32_get_language_encoding_based_on_lc_all_or_lang(buffer).language};
	if(language.empty())
		return get_win32_lang_env(buffer);
	return language;
}

inline void* load_l10n_with_extracting_local_name_impl(lc_locale& loc)
{
	std::array<char,64> buffer;
	return load_l10n_with_real_name_impl(loc,win32_only_get_language_not_encoding(buffer));
}
inline void* load_l10n_with_extracting_local_name_impl(lc_locale& loc,std::string_view encoding)
{
	std::array<char,64> buffer;
	return load_l10n_with_real_name_impl(loc,win32_only_get_language_not_encoding(buffer),encoding);
}

inline void* deal_with_local_locale_name_local_encoding(lc_locale& loc)
{
	std::array<char,64> buffer;
	auto [language,encoding]=win32_get_language_encoding_based_on_lc_all_or_lang(buffer);
	std::array<char,64> lang_buffer;
	if(language.empty())
		language=get_win32_lang_env(lang_buffer);
	if(encoding.empty())
		encoding=acp_encoding_name();
	return load_l10n_with_real_name_impl(loc,language,encoding);
}

inline void* deal_with_local_locale_name_encoding(lc_locale& loc,std::string_view encoding)
{
	std::array<char,64> buffer;
	return load_l10n_with_real_name_impl(loc,win32_only_get_language_not_encoding(buffer),encoding);
}

inline void* deal_with_locale_name_local_encoding(lc_locale& loc,std::string_view locale_name)
{
	std::array<char,64> buffer;
	auto encoding{win32_get_language_encoding_based_on_lc_all_or_lang(buffer).encoding};
	if(encoding.empty())
		encoding=acp_encoding_name();
	return load_l10n_with_real_name_impl(loc,locale_name,encoding);
}

#else

inline char const* my_getenv(char8_t const* env) noexcept
{
	return
#if defined(_GNU_SOURCE)
	secure_getenv(reinterpret_cast<char const*>(env));
#else
	std::getenv(reinterpret_cast<char const*>(env));
#endif
}

inline std::string_view get_lc_all_or_lang() noexcept
{
	auto lc_all_ptr = my_getenv(u8"LC_ALL");
	if(lc_all_ptr)
		return std::string_view(lc_all_ptr);
	lc_all_ptr = my_getenv(u8"LANG");
	if(lc_all_ptr)
		return std::string_view(lc_all_ptr);
	return std::string_view(reinterpret_cast<char const*>(u8"C.UTF-8"),sizeof(u8"C.UTF-8")-1);
}

inline std::string_view get_posix_lang_env() noexcept
{
	std::string_view lang_env{get_lc_all_or_lang()};
	auto dot{lang_env.find(u8'.')};
	if(dot!=std::string_view::npos)
		lang_env=lang_env.substr(0,dot);
	return lang_env;
}

inline void* load_l10n_with_extracting_local_name_impl(lc_locale& loc)
{
	return load_l10n_with_real_name_impl(loc,get_posix_lang_env());
}
inline void* load_l10n_with_extracting_local_name_impl(lc_locale& loc,std::string_view encoding)
{
	return load_l10n_with_real_name_impl(loc,get_posix_lang_env(),encoding);
}

inline std::string_view get_lc_all_or_lang_non_empty() noexcept
{
	std::string_view lang_env=get_lc_all_or_lang();
	if(lang_env.empty())
		lang_env=reinterpret_cast<char const*>(u8"C");
	return lang_env;
}

inline constexpr std::u8string_view exec_encoding_u8strvw() noexcept
{
	if constexpr('A'!=u8'A')
		return u8"IBM12712";
	else if constexpr(details::compile_time_compare("我",u8"我"))
		return u8"UTF-8";
	else
		return u8"GB18030";
}

inline std::string_view exec_charset_encoding_strvw() noexcept
{
	constexpr auto u8sv{exec_encoding_u8strvw()};
	return {reinterpret_cast<char const*>(u8sv.data()),u8sv.size()};
}


inline void* deal_with_local_locale_name_local_encoding(lc_locale& loc)
{
	auto strvw{get_lc_all_or_lang_non_empty()};
	auto pos{strvw.find(u8'.')};
	std::string_view name,coding;
	if(pos==std::string_view::npos)
	{
		name=strvw;
		coding=exec_charset_encoding_strvw();
	}
	else
	{
		name=strvw.substr(0,pos);
		coding=strvw.substr(pos+1);
	}
	return load_l10n_with_real_name_impl(loc,name,coding);
}

inline void* deal_with_local_locale_name_encoding(lc_locale& loc,std::string_view encoding)
{
	auto name{get_lc_all_or_lang_non_empty()};
	auto pos{name.find(u8'.')};
	if(pos!=std::string_view::npos)
		name=name.substr(0,pos);
	return load_l10n_with_real_name_impl(loc,name,encoding);
}

inline void* deal_with_locale_name_local_encoding(lc_locale& loc,std::string_view locale_name)
{
	auto coding{get_lc_all_or_lang_non_empty()};
	auto pos{coding.find(u8'.')};
	if(pos==std::string_view::npos)
		coding=exec_charset_encoding_strvw();
	else
		coding=coding.substr(pos+1);
	return load_l10n_with_real_name_impl(loc,locale_name,coding);
}

#endif

inline void* deal_with_local_locale_name_init_encoding(lc_locale& loc,std::string_view encoding)
{
	if(encoding.empty())
		return deal_with_local_locale_name_local_encoding(loc);
	else
		return deal_with_local_locale_name_encoding(loc,encoding);
}

inline void* deal_with_locale_name_init_encoding(lc_locale& loc,std::string_view locale_name,std::string_view encoding)
{
	if(encoding.empty())
		return deal_with_locale_name_local_encoding(loc,locale_name);
	else
		return load_l10n_with_real_name_impl(loc,locale_name,encoding);
}

inline void* deal_with_locale_fullname(lc_locale& loc,std::string_view fullname)
{
	if(fullname.empty())
		return deal_with_local_locale_name_local_encoding(loc);
	else
	{
		auto pos{fullname.find(u8'.')};
		if(pos==0)
			return deal_with_local_locale_name_init_encoding(loc,fullname.substr(1));
		else if(pos==std::string_view::npos)
			return load_l10n_with_real_name_impl(loc,fullname);
		else
			return load_l10n_with_full_name_impl(loc,fullname);
	}
}

}

struct l10n_fullname_t
{
explicit constexpr l10n_fullname_t() noexcept = default;
};

inline constexpr l10n_fullname_t l10n_fullname{};

class l10n
{
public:
	lc_locale loc{};
	void* dll_handle{};
	constexpr l10n() noexcept=default;
	explicit l10n(std::string_view locale_name)
	{
		if(locale_name.empty())
			dll_handle=details::load_l10n_with_extracting_local_name_impl(loc);
		else
			dll_handle=details::load_l10n_with_real_name_impl(loc,locale_name);
	}
	explicit l10n(std::string_view locale_name,std::string_view encoding)
	{
		if(locale_name.empty())
			dll_handle=details::deal_with_local_locale_name_init_encoding(loc,encoding);
		else
			dll_handle=details::deal_with_locale_name_init_encoding(loc,locale_name,encoding);
	}
	explicit l10n(l10n_fullname_t,std::string_view fullname)
	{
		dll_handle=details::deal_with_locale_fullname(loc,fullname);
	}
	constexpr l10n(l10n const&)=delete;
	constexpr l10n(l10n&& other) noexcept : loc(std::move(other.loc)),dll_handle(other.dll_handle)
	{
		dll_handle=nullptr;
	}
	explicit constexpr operator bool() noexcept
	{
		return dll_handle;
	}
	void close() noexcept
	{
		if(dll_handle)[[likely]]
#ifdef _WIN32
			fast_io::win32::FreeLibrary(dll_handle);
#else
			dlclose(dll_handle);
#endif
	}
	l10n& operator=(l10n const&)=delete;
	l10n& operator=(l10n&& other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		close();
		loc=std::move(other.loc);
		dll_handle=other.dll_handle;
		other.dll_handle=nullptr;
		return *this;
	}
	~l10n()
	{
		close();
	}
};

}