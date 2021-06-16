#pragma once

namespace fast_io
{

enum class dll_mode:std::uint32_t
{
none=0,
posix_rtld_lazy=1<<0,
posix_rtld_now=1<<1,
posix_rtld_binding_mask=1<<2,
posix_rtld_global=1<<3,
posix_rtld_local=1<<4,
posix_rtld_nodelete=1<<5,
posix_rtld_noload=1<<6,
posix_rtld_deepbind=1<<7,
win32_dont_resolve_dll_references=1<<10,
win32_load_ignore_code_authz_level=1<<11,
win32_load_library_as_datafile=1<<12,
win32_load_library_as_datafile_exclusive=1<<13,
win32_load_library_as_image_resource=1<<14,
win32_load_library_search_application_dir=1<<15,
win32_load_library_search_default_dirs=1<<16,
win32_load_library_search_dll_load_dir=1<<17,
win32_load_library_search_system32=1<<18,
win32_load_library_search_user_dirs=1<<19,
win32_load_with_altered_search_path=1<<20,
win32_load_library_require_signed_target=1<<21,
win32_load_library_safe_current_dirs=1<<22
};


constexpr dll_mode operator&(dll_mode x, dll_mode y) noexcept
{
using utype = typename std::underlying_type<dll_mode>::type;
return static_cast<dll_mode>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr dll_mode operator|(dll_mode x, dll_mode y) noexcept
{
using utype = typename std::underlying_type<dll_mode>::type;
return static_cast<dll_mode>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr dll_mode operator^(dll_mode x, dll_mode y) noexcept
{
using utype = typename std::underlying_type<dll_mode>::type;
return static_cast<dll_mode>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr dll_mode operator~(dll_mode x) noexcept
{
using utype = typename std::underlying_type<dll_mode>::type;
return static_cast<dll_mode>(~static_cast<utype>(x));
}

inline constexpr dll_mode& operator&=(dll_mode& x, dll_mode y) noexcept{return x=x&y;}

inline constexpr dll_mode& operator|=(dll_mode& x, dll_mode y) noexcept{return x=x|y;}

inline constexpr dll_mode& operator^=(dll_mode& x, dll_mode y) noexcept{return x=x^y;}
}