#pragma once
#include"../fast_io_concept.h"
#include"bootloader/stvl2.h"

namespace fast_io
{

namespace details
{

template<std::integral char_type>
inline constexpr std::size_t cal_stvl2_cstr_reserved_size(std::size_t sz) noexcept
{
	constexpr bool ebcdic{exec_charset_is_ebcdic<char_type>()};
	if constexpr(sizeof(char_type)==1&&!ebcdic)
		return sz;
	else
		return cal_full_reserve_size<1,sizeof(char_type)>(sz);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_tag(Iter iter,stvl2::stvl2_tag tg) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		iter=copy_string_literal("identifier: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.identifier);
		iter=copy_string_literal("\nnext: ",iter);
		return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.next);
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		iter=copy_string_literal(L"identifier: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.identifier);
		iter=copy_string_literal(L"\nnext: ",iter);
		return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.next);
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		iter=copy_string_literal(u"identifier: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.identifier);
		iter=copy_string_literal(u"\nnext: ",iter);
		return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.next);
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		iter=copy_string_literal(U"identifier: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.identifier);
		iter=copy_string_literal(U"\nnext: ",iter);
		return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.next);
	}
	else
	{
		iter=copy_string_literal(u8"identifier: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.identifier);
		iter=copy_string_literal(u8"\nnext: ",iter);
		return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.next);
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_header_tag(Iter iter,stvl2::stvl2_header_tag tg) noexcept
{
	using enum stvl2::stvl2_header_tag;
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		switch(tg)
		{
		case framebuffer_id:
			return copy_string_literal("framebuffer_id",iter);
		case fb_mtrr_id:
			return copy_string_literal("fb_mtrr_id",iter);
		case smp_id:
			return copy_string_literal("smp_id",iter);
		default:
			return copy_string_literal("unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		switch(tg)
		{
		case framebuffer_id:
			return copy_string_literal(L"framebuffer_id",iter);
		case fb_mtrr_id:
			return copy_string_literal(L"fb_mtrr_id",iter);
		case smp_id:
			return copy_string_literal(L"smp_id",iter);
		default:
			return copy_string_literal(L"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		switch(tg)
		{
		case framebuffer_id:
			return copy_string_literal(u"framebuffer_id",iter);
		case fb_mtrr_id:
			return copy_string_literal(u"fb_mtrr_id",iter);
		case smp_id:
			return copy_string_literal(u"smp_id",iter);
		default:
			return copy_string_literal(u"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		switch(tg)
		{
		case framebuffer_id:
			return copy_string_literal(U"framebuffer_id",iter);
		case fb_mtrr_id:
			return copy_string_literal(U"fb_mtrr_id",iter);
		case smp_id:
			return copy_string_literal(U"smp_id",iter);
		default:
			return copy_string_literal(U"unknown",iter);
		}
	}
	else
	{
		switch(tg)
		{
		case framebuffer_id:
			return copy_string_literal(u8"framebuffer_id",iter);
		case fb_mtrr_id:
			return copy_string_literal(u8"fb_mtrr_id",iter);
		case smp_id:
			return copy_string_literal(u8"smp_id",iter);
		default:
			return copy_string_literal(u8"unknown",iter);
		}
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_header(Iter iter,stvl2::stvl2_header tg) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		iter=copy_string_literal("entry_point: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.entry_point);
		iter=copy_string_literal("\nstack: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.stack);
		iter=copy_string_literal("\nflags: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.flags);
		iter=copy_string_literal("\ntags: ",iter);
		return print_reserve_define(io_reserve_type<char_type,stvl2::stvl2_header_tag>,iter,tg.tags);
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		iter=copy_string_literal(L"entry_point: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.entry_point);
		iter=copy_string_literal(L"\nstack: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.stack);
		iter=copy_string_literal(L"\nflags: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.flags);
		iter=copy_string_literal(L"\ntags: ",iter);
		return print_reserve_define(io_reserve_type<char_type,stvl2::stvl2_header_tag>,iter,tg.tags);
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		iter=copy_string_literal(u"entry_point: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.entry_point);
		iter=copy_string_literal(u"\nstack: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.stack);
		iter=copy_string_literal(u"\nflags: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.flags);
		iter=copy_string_literal(u"\ntags: ",iter);
		return print_reserve_define(io_reserve_type<char_type,stvl2::stvl2_header_tag>,iter,tg.tags);
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		iter=copy_string_literal(U"entry_point: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.entry_point);
		iter=copy_string_literal(U"\nstack: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.stack);
		iter=copy_string_literal(U"\nflags: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.flags);
		iter=copy_string_literal(U"\ntags: ",iter);
		return print_reserve_define(io_reserve_type<char_type,stvl2::stvl2_header_tag>,iter,tg.tags);
	}
	else
	{
		iter=copy_string_literal(u8"entry_point: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.entry_point);
		iter=copy_string_literal(u8"\nstack: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.stack);
		iter=copy_string_literal(u8"\nflags: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.flags);
		iter=copy_string_literal(u8"\ntags: ",iter);
		return print_reserve_define(io_reserve_type<char_type,stvl2::stvl2_header_tag>,iter,tg.tags);
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_header_tag_framebuffer(Iter iter,stvl2::stvl2_header_tag_framebuffer tg) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=print_reserve_define_impl_for_stvl2_tag(iter,tg);
	if constexpr(std::same_as<char_type,char>)
	{
		iter=copy_string_literal("\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal("\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal("\nframebuffer_bpp: ",iter);
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		iter=copy_string_literal(L"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(L"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(L"\nframebuffer_bpp: ",iter);
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		iter=copy_string_literal(u"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(u"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(u"\nframebuffer_bpp: ",iter);
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		iter=copy_string_literal(U"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(U"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(U"\nframebuffer_bpp: ",iter);
	}
	else
	{
		iter=copy_string_literal(u8"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(u8"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(u8"\nframebuffer_bpp: ",iter);
	}
	return print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_bpp);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_header_tag_smp(Iter iter,stvl2::stvl2_header_tag_smp tg) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=print_reserve_define_impl_for_stvl2_tag(iter,tg);
	if constexpr(std::same_as<char_type,char>)
		iter=copy_string_literal("\nflags: ",iter);
	else if constexpr(std::same_as<char_type,wchar_t>)
		iter=copy_string_literal(L"\nflags: ",iter);
	else if constexpr(std::same_as<char_type,char16_t>)
		iter=copy_string_literal(u"\nflags: ",iter);
	else if constexpr(std::same_as<char_type,char32_t>)
		iter=copy_string_literal(U"\nflags: ",iter);
	else
		iter=copy_string_literal(u8"\nflags: ",iter);
	return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,tg.flags);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_struct_tag(Iter iter,stvl2::stvl2_struct_tag tg) noexcept
{
	using enum stvl2::stvl2_struct_tag;
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		switch(tg)
		{
		case cmdline_id:return copy_string_literal("cmdline_id",iter);
		case memmap_id:return copy_string_literal("memmap_id",iter);
		case framebuffer_id:return copy_string_literal("framebuffer_id",iter);
		case fb_mtrr_id:return copy_string_literal("fb_mtrr_id",iter);
		case modules_id:return copy_string_literal("modules_id",iter);
		case rsdp_id:return copy_string_literal("rsdp_id",iter);
		case epoch_id:return copy_string_literal("epoch_id",iter);
		case firmware_id:return copy_string_literal("firmware_id",iter);
		case smp_id:return copy_string_literal("smp_id",iter);
		case pxe_server_info:return copy_string_literal("pxe_server_info",iter);
		case mmio32_uart:return copy_string_literal("mmio32_uart",iter);
		case dtb:return copy_string_literal("dtb",iter);
		case vmap:return copy_string_literal("vmap",iter);
		default:return copy_string_literal("unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		switch(tg)
		{
		case cmdline_id:return copy_string_literal(L"cmdline_id",iter);
		case memmap_id:return copy_string_literal(L"memmap_id",iter);
		case framebuffer_id:return copy_string_literal(L"framebuffer_id",iter);
		case fb_mtrr_id:return copy_string_literal(L"fb_mtrr_id",iter);
		case modules_id:return copy_string_literal(L"modules_id",iter);
		case rsdp_id:return copy_string_literal(L"rsdp_id",iter);
		case epoch_id:return copy_string_literal(L"epoch_id",iter);
		case firmware_id:return copy_string_literal(L"firmware_id",iter);
		case smp_id:return copy_string_literal(L"smp_id",iter);
		case pxe_server_info:return copy_string_literal(L"pxe_server_info",iter);
		case mmio32_uart:return copy_string_literal(L"mmio32_uart",iter);
		case dtb:return copy_string_literal(L"dtb",iter);
		case vmap:return copy_string_literal(L"vmap",iter);
		default:return copy_string_literal(L"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		switch(tg)
		{
		case cmdline_id:return copy_string_literal(u"cmdline_id",iter);
		case memmap_id:return copy_string_literal(u"memmap_id",iter);
		case framebuffer_id:return copy_string_literal(u"framebuffer_id",iter);
		case fb_mtrr_id:return copy_string_literal(u"fb_mtrr_id",iter);
		case modules_id:return copy_string_literal(u"modules_id",iter);
		case rsdp_id:return copy_string_literal(u"rsdp_id",iter);
		case epoch_id:return copy_string_literal(u"epoch_id",iter);
		case firmware_id:return copy_string_literal(u"firmware_id",iter);
		case smp_id:return copy_string_literal(u"smp_id",iter);
		case pxe_server_info:return copy_string_literal(u"pxe_server_info",iter);
		case mmio32_uart:return copy_string_literal(u"mmio32_uart",iter);
		case dtb:return copy_string_literal(u"dtb",iter);
		case vmap:return copy_string_literal(u"vmap",iter);
		default:return copy_string_literal(u"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		switch(tg)
		{
		case cmdline_id:return copy_string_literal(U"cmdline_id",iter);
		case memmap_id:return copy_string_literal(U"memmap_id",iter);
		case framebuffer_id:return copy_string_literal(U"framebuffer_id",iter);
		case fb_mtrr_id:return copy_string_literal(U"fb_mtrr_id",iter);
		case modules_id:return copy_string_literal(U"modules_id",iter);
		case rsdp_id:return copy_string_literal(U"rsdp_id",iter);
		case epoch_id:return copy_string_literal(U"epoch_id",iter);
		case firmware_id:return copy_string_literal(U"firmware_id",iter);
		case smp_id:return copy_string_literal(U"smp_id",iter);
		case pxe_server_info:return copy_string_literal(U"pxe_server_info",iter);
		case mmio32_uart:return copy_string_literal(U"mmio32_uart",iter);
		case dtb:return copy_string_literal(U"dtb",iter);
		case vmap:return copy_string_literal(U"vmap",iter);
		default:return copy_string_literal(U"unknown",iter);
		}
	}
	else
	{
		switch(tg)
		{
		case cmdline_id:return copy_string_literal(u8"cmdline_id",iter);
		case memmap_id:return copy_string_literal(u8"memmap_id",iter);
		case framebuffer_id:return copy_string_literal(u8"framebuffer_id",iter);
		case fb_mtrr_id:return copy_string_literal(u8"fb_mtrr_id",iter);
		case modules_id:return copy_string_literal(u8"modules_id",iter);
		case rsdp_id:return copy_string_literal(u8"rsdp_id",iter);
		case epoch_id:return copy_string_literal(u8"epoch_id",iter);
		case firmware_id:return copy_string_literal(u8"firmware_id",iter);
		case smp_id:return copy_string_literal(u8"smp_id",iter);
		case pxe_server_info:return copy_string_literal(u8"pxe_server_info",iter);
		case mmio32_uart:return copy_string_literal(u8"mmio32_uart",iter);
		case dtb:return copy_string_literal(u8"dtb",iter);
		case vmap:return copy_string_literal(u8"vmap",iter);
		default:return copy_string_literal(u8"unknown",iter);
		}
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_mmap_type(Iter iter,stvl2::stvl2_mmap_type tg) noexcept
{
	using enum stvl2::stvl2_mmap_type;
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		switch(tg)
		{
		case usable:return copy_string_literal("usable",iter);
		case reserved:return copy_string_literal("reserved",iter);
		case acpi_reclaimable:return copy_string_literal("acpi_reclaimable",iter);
		case acpi_nvs:return copy_string_literal("acpi_nvs",iter);
		case bad_memory:return copy_string_literal("bad_memory",iter);
		case bootloader_reclaimable:return copy_string_literal("bootloader_reclaimable",iter);
		case kernel_and_modules:return copy_string_literal("kernel_and_modules",iter);
		default:return copy_string_literal("unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		switch(tg)
		{
		case usable:return copy_string_literal(L"usable",iter);
		case reserved:return copy_string_literal(L"reserved",iter);
		case acpi_reclaimable:return copy_string_literal(L"acpi_reclaimable",iter);
		case acpi_nvs:return copy_string_literal(L"acpi_nvs",iter);
		case bad_memory:return copy_string_literal(L"bad_memory",iter);
		case bootloader_reclaimable:return copy_string_literal(L"bootloader_reclaimable",iter);
		case kernel_and_modules:return copy_string_literal(L"kernel_and_modules",iter);
		default:return copy_string_literal(L"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		switch(tg)
		{
		case usable:return copy_string_literal(u"usable",iter);
		case reserved:return copy_string_literal(u"reserved",iter);
		case acpi_reclaimable:return copy_string_literal(u"acpi_reclaimable",iter);
		case acpi_nvs:return copy_string_literal(u"acpi_nvs",iter);
		case bad_memory:return copy_string_literal(u"bad_memory",iter);
		case bootloader_reclaimable:return copy_string_literal(u"bootloader_reclaimable",iter);
		case kernel_and_modules:return copy_string_literal(u"kernel_and_modules",iter);
		default:return copy_string_literal(u"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		switch(tg)
		{
		case usable:return copy_string_literal(U"usable",iter);
		case reserved:return copy_string_literal(U"reserved",iter);
		case acpi_reclaimable:return copy_string_literal(U"acpi_reclaimable",iter);
		case acpi_nvs:return copy_string_literal(U"acpi_nvs",iter);
		case bad_memory:return copy_string_literal(U"bad_memory",iter);
		case bootloader_reclaimable:return copy_string_literal(U"bootloader_reclaimable",iter);
		case kernel_and_modules:return copy_string_literal(U"kernel_and_modules",iter);
		default:return copy_string_literal(U"unknown",iter);
		}
	}
	else
	{
		switch(tg)
		{
		case usable:return copy_string_literal(u8"usable",iter);
		case reserved:return copy_string_literal(u8"reserved",iter);
		case acpi_reclaimable:return copy_string_literal(u8"acpi_reclaimable",iter);
		case acpi_nvs:return copy_string_literal(u8"acpi_nvs",iter);
		case bad_memory:return copy_string_literal(u8"bad_memory",iter);
		case bootloader_reclaimable:return copy_string_literal(u8"bootloader_reclaimable",iter);
		case kernel_and_modules:return copy_string_literal(u8"kernel_and_modules",iter);
		default:return copy_string_literal(u8"unknown",iter);
		}
	}
}


template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_firmware_flags(Iter iter,stvl2::stvl2_firmware_flags tg) noexcept
{
	using enum stvl2::stvl2_firmware_flags;
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		switch(tg)
		{
		case bios:return copy_string_literal("bios",iter);
		default:return copy_string_literal("unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		switch(tg)
		{
		case bios:return copy_string_literal(L"bios",iter);
		default:return copy_string_literal(L"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		switch(tg)
		{
		case bios:return copy_string_literal(u"bios",iter);
		default:return copy_string_literal(u"unknown",iter);
		}
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		switch(tg)
		{
		case bios:return copy_string_literal(U"bios",iter);
		default:return copy_string_literal(U"unknown",iter);
		}
	}
	else
	{
		switch(tg)
		{
		case bios:return copy_string_literal(u8"bios",iter);
		default:return copy_string_literal(u8"unknown",iter);
		}
	}
}

inline constexpr std::size_t constexpr_stvl2_u8strlen(char8_t const* strlen) noexcept
{
	if(std::is_constant_evaluated())
	{
		std::size_t len{};
		for(;strlen[len];++len);
		return len;
	}
	else
	{
		return std::strlen(reinterpret_cast<char const*>(strlen));
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter deal_with_stvl2_cstr(Iter iter,char8_t const* u8cstr) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	constexpr bool ebcdic{exec_charset_is_ebcdic<char_type>()};
	if constexpr(sizeof(char_type)==1&&!ebcdic)
		return non_overlapped_copy_n(u8cstr,constexpr_stvl2_u8strlen(u8cstr),iter);
	else
	{
		std::size_t sz{constexpr_stvl2_u8strlen(u8cstr)};
		return codecvt::general_code_cvt_full(u8cstr,u8cstr+sz,iter);
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_struct(Iter iter,stvl2::stvl2_struct const& st) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		iter=copy_string_literal("bootloader_brand: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_brand);
		iter=copy_string_literal("\nbootloader_version: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_version);
		iter=copy_string_literal("\ntags: ",iter);
		return print_reserve_define_impl_for_stvl2_struct_tag(iter,st.tags);
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		iter=copy_string_literal(L"bootloader_brand: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_brand);
		iter=copy_string_literal(L"\nbootloader_version: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_version);
		iter=copy_string_literal(L"\ntags: ",iter);
		return print_reserve_define_impl_for_stvl2_struct_tag(iter,st.tags);
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		iter=copy_string_literal(u"bootloader_brand: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_brand);
		iter=copy_string_literal(u"\nbootloader_version: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_version);
		iter=copy_string_literal(u"\ntags: ",iter);
		return print_reserve_define_impl_for_stvl2_struct_tag(iter,st.tags);
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		iter=copy_string_literal(U"bootloader_brand: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_brand);
		iter=copy_string_literal(U"\nbootloader_version: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_version);
		iter=copy_string_literal(U"\ntags: ",iter);
		return print_reserve_define_impl_for_stvl2_struct_tag(iter,st.tags);
	}
	else
	{
		iter=copy_string_literal(u8"bootloader_brand: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_brand);
		iter=copy_string_literal(u8"\nbootloader_version: ",iter);
		iter=deal_with_stvl2_cstr(iter,st.bootloader_version);
		iter=copy_string_literal(u8"\ntags: ",iter);
		return print_reserve_define_impl_for_stvl2_struct_tag(iter,st.tags);
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_struct_tag_cmdline(Iter iter,stvl2::stvl2_struct_tag_cmdline st) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=print_reserve_define_impl_for_stvl2_tag(iter,st);
	if constexpr(std::same_as<char_type,char>)
		iter=copy_string_literal("\ncmdline: ",iter);
	else if constexpr(std::same_as<char_type,wchar_t>)
		iter=copy_string_literal(L"\ncmdline: ",iter);
	else if constexpr(std::same_as<char_type,char16_t>)
		iter=copy_string_literal(u"\ncmdline: ",iter);
	else if constexpr(std::same_as<char_type,char32_t>)
		iter=copy_string_literal(U"\ncmdline: ",iter);
	else
		iter=copy_string_literal(u8"\ncmdline: ",iter);
	return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,st.cmdline);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_struct_tag_memmap(Iter iter,stvl2::stvl2_struct_tag_memmap st) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=print_reserve_define_impl_for_stvl2_tag(iter,st);
	if constexpr(std::same_as<char_type,char>)
		iter=copy_string_literal("\nentries: ",iter);
	else if constexpr(std::same_as<char_type,wchar_t>)
		iter=copy_string_literal(L"\nentries: ",iter);
	else if constexpr(std::same_as<char_type,char16_t>)
		iter=copy_string_literal(u"\nentries: ",iter);
	else if constexpr(std::same_as<char_type,char32_t>)
		iter=copy_string_literal(U"\nentries: ",iter);
	else
		iter=copy_string_literal(u8"\nentries: ",iter);
	return print_reserve_define(io_reserve_type<char_type,std::uint64_t>,iter,st.entries);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_struct_tag_framebuffer(Iter iter,stvl2::stvl2_struct_tag_framebuffer tg) noexcept
{
	iter=print_reserve_define_impl_for_stvl2_tag(iter,tg);
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		iter=copy_string_literal("\nframebuffer_addr: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.framebuffer_addr);
		iter=copy_string_literal("\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal("\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal("\nframebuffer_pitch: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_pitch);
		iter=copy_string_literal("\nframebuffer_bpp: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_bpp);
		iter=copy_string_literal("\nmemory_model: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.memory_model);
		iter=copy_string_literal("\nred_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_size);
		iter=copy_string_literal("\nred_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_shift);
		iter=copy_string_literal("\ngreen_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_size);
		iter=copy_string_literal("\ngreen_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_shift);
		iter=copy_string_literal("\nblue_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.blue_mask_size);
		iter=copy_string_literal("\nblue_mask_shift: ",iter);
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		iter=copy_string_literal(L"\nframebuffer_addr: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.framebuffer_addr);
		iter=copy_string_literal(L"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(L"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(L"\nframebuffer_pitch: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_pitch);
		iter=copy_string_literal(L"\nframebuffer_bpp: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_bpp);
		iter=copy_string_literal(L"\nmemory_model: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.memory_model);
		iter=copy_string_literal(L"\nred_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_size);
		iter=copy_string_literal(L"\nred_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_shift);
		iter=copy_string_literal(L"\ngreen_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_size);
		iter=copy_string_literal(L"\ngreen_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_shift);
		iter=copy_string_literal(L"\nblue_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.blue_mask_size);
		iter=copy_string_literal(L"\nblue_mask_shift: ",iter);
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		iter=copy_string_literal(u"\nframebuffer_addr: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.framebuffer_addr);
		iter=copy_string_literal(u"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(u"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(u"\nframebuffer_pitch: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_pitch);
		iter=copy_string_literal(u"\nframebuffer_bpp: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_bpp);
		iter=copy_string_literal(u"\nmemory_model: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.memory_model);
		iter=copy_string_literal(u"\nred_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_size);
		iter=copy_string_literal(u"\nred_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_shift);
		iter=copy_string_literal(u"\ngreen_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_size);
		iter=copy_string_literal(u"\ngreen_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_shift);
		iter=copy_string_literal(u"\nblue_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.blue_mask_size);
		iter=copy_string_literal(u"\nblue_mask_shift: ",iter);
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		iter=copy_string_literal(U"\nframebuffer_addr: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.framebuffer_addr);
		iter=copy_string_literal(U"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(U"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(U"\nframebuffer_pitch: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_pitch);
		iter=copy_string_literal(U"\nframebuffer_bpp: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_bpp);
		iter=copy_string_literal(U"\nmemory_model: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.memory_model);
		iter=copy_string_literal(U"\nred_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_size);
		iter=copy_string_literal(U"\nred_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_shift);
		iter=copy_string_literal(U"\ngreen_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_size);
		iter=copy_string_literal(U"\ngreen_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_shift);
		iter=copy_string_literal(U"\nblue_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.blue_mask_size);
		iter=copy_string_literal(U"\nblue_mask_shift: ",iter);
	}
	else
	{
		iter=copy_string_literal(u8"\nframebuffer_addr: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,tg.framebuffer_addr);
		iter=copy_string_literal(u8"\nframebuffer_width: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_width);
		iter=copy_string_literal(u8"\nframebuffer_height: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_height);
		iter=copy_string_literal(u8"\nframebuffer_pitch: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_pitch);
		iter=copy_string_literal(u8"\nframebuffer_bpp: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint16_t>,iter,tg.framebuffer_bpp);
		iter=copy_string_literal(u8"\nmemory_model: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.memory_model);
		iter=copy_string_literal(u8"\nred_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_size);
		iter=copy_string_literal(u8"\nred_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.red_mask_shift);
		iter=copy_string_literal(u8"\ngreen_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_size);
		iter=copy_string_literal(u8"\ngreen_mask_shift: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.green_mask_shift);
		iter=copy_string_literal(u8"\nblue_mask_size: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.blue_mask_size);
		iter=copy_string_literal(u8"\nblue_mask_shift: ",iter);
	}
	return print_reserve_define(io_reserve_type<char_type,std::uint8_t>,iter,tg.blue_mask_shift);
}


template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_impl_for_stvl2_module(Iter iter,stvl2::stvl2_module st) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
	{
		iter=copy_string_literal("module_begin: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_begin);
		iter=copy_string_literal("\nmodule_end: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_end);
		iter=copy_string_literal("\nmodule_string: ",iter);
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		iter=copy_string_literal(L"module_begin: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_begin);
		iter=copy_string_literal(L"\nmodule_end: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_end);
		iter=copy_string_literal(L"\nmodule_string: ",iter);
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		iter=copy_string_literal(u"module_begin: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_begin);
		iter=copy_string_literal(u"\nmodule_end: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_end);
		iter=copy_string_literal(u"\nmodule_string: ",iter);
	}
	else if constexpr(std::same_as<char_type,char32_t>)
	{
		iter=copy_string_literal(U"module_begin: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_begin);
		iter=copy_string_literal(U"\nmodule_end: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_end);
		iter=copy_string_literal(U"\nmodule_string: ",iter);
	}
	else
	{
		iter=copy_string_literal(u8"module_begin: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_begin);
		iter=copy_string_literal(u8"\nmodule_end: ",iter);
		iter=print_reserve_define(io_reserve_type<char_type,void const*>,iter,st.module_end);
		iter=copy_string_literal(u8"\nmodule_string: ",iter);
	}
	return deal_with_stvl2_cstr(iter,st.module_string);
}

}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_tag>) noexcept
{
	constexpr std::size_t number_size
	{
		print_reserve_size(io_reserve_type<char_type,std::uint64_t>)*2
	};
	if constexpr(std::same_as<char_type,char>)
		return number_size+details::string_literal_size("identifier: \nnext: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return number_size+details::string_literal_size(L"identifier: \nnext: ");
	else
		return number_size+details::string_literal_size(u8"identifier: \nnext: ");
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_header>) noexcept
{
	constexpr std::size_t tag_size{print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_header_tag>)
	+print_reserve_size(io_reserve_type<char_type,void const*>)*2
	+print_reserve_size(io_reserve_type<char_type,std::uint64_t>)};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("entry_point: \nstack: \nflags: \ntags: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"entry_point: \nstack: \nflags: \ntags: ");
	else
		return tag_size+details::string_literal_size(u8"entry_point: \nstack: \nflags: \ntags: ");
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_header_tag_framebuffer>) noexcept
{
	constexpr std::size_t tag_size
	{
		print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_tag>)+
		print_reserve_size(io_reserve_type<char_type,std::uint16_t>)*3
	};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("\nframebuffer_width: \nframebuffer_height: \nframebuffer_bpp: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"\nframebuffer_width: \nframebuffer_height: \nframebuffer_bpp: ");
	else
		return tag_size+details::string_literal_size(u8"\nframebuffer_width: \nframebuffer_height: \nframebuffer_bpp: ");
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_header_tag_smp>) noexcept
{
	constexpr std::size_t tag_size
	{
		print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_tag>)+
		print_reserve_size(io_reserve_type<char_type,std::uint64_t>)
	};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("\nflags: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"\nflags: ");
	else
		return tag_size+details::string_literal_size(u8"\nflags: ");
}

template<std::integral char_type,typename T>
requires (std::same_as<T,stvl2::stvl2_mmap_type>||std::same_as<T,stvl2::stvl2_struct_tag>||
	std::same_as<T,stvl2::stvl2_header_tag>||std::same_as<T,stvl2::stvl2_firmware_flags>)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,T>) noexcept
{
	return 32;
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_struct>) noexcept
{
	constexpr std::size_t tag_size{
		print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_struct_tag>)+
		details::cal_stvl2_cstr_reserved_size<char_type>(stvl2::stvl2_struct::bootloader_brand_size+
		stvl2::stvl2_struct::bootloader_version_size)
		};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("bootloader_brand: \nbootloader_version: \ntags: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"bootloader_brand: \nbootloader_version: \ntags: ");
	else if constexpr(std::same_as<char_type,char16_t>)
		return tag_size+details::string_literal_size(u"bootloader_brand: \nbootloader_version: \ntags: ");
	else if constexpr(std::same_as<char_type,char32_t>)
		return tag_size+details::string_literal_size(U"bootloader_brand: \nbootloader_version: \ntags: ");
	else
		return tag_size+details::string_literal_size(u8"bootloader_brand: \nbootloader_version: \ntags: ");
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_struct_tag_cmdline>) noexcept
{
	constexpr std::size_t tag_size{print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_tag>)
	+print_reserve_size(io_reserve_type<char_type,std::uint64_t>)};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("\ncmdline: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"\ncmdline: ");
	else if constexpr(std::same_as<char_type,char16_t>)
		return tag_size+details::string_literal_size(u"\ncmdline: ");
	else if constexpr(std::same_as<char_type,char32_t>)
		return tag_size+details::string_literal_size(U"\ncmdline: ");
	else
		return tag_size+details::string_literal_size(u8"\ncmdline: ");
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_struct_tag_memmap>) noexcept
{
	constexpr std::size_t tag_size{print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_tag>)
	+print_reserve_size(io_reserve_type<char_type,std::uint64_t>)};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("\nentries: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"\nentries: ");
	else if constexpr(std::same_as<char_type,char16_t>)
		return tag_size+details::string_literal_size(u"\nentries: ");
	else if constexpr(std::same_as<char_type,char32_t>)
		return tag_size+details::string_literal_size(U"\nentries: ");
	else
		return tag_size+details::string_literal_size(u8"\nentries: ");
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_struct_tag_framebuffer>) noexcept
{
	constexpr std::size_t tag_size{
	print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_tag>)
	+print_reserve_size(io_reserve_type<char_type,std::uint8_t>)*7+
	+print_reserve_size(io_reserve_type<char_type,std::uint16_t>)*4
	+print_reserve_size(io_reserve_type<char_type,void const*>)};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("\nframebuffer_addr: \nframebuffer_width: \nframebuffer_height: \nframebuffer_pitch: \nframebuffer_bpp: \nmemory_model: \nred_mask_size: \nred_mask_shift: \ngreen_mask_size: \ngreen_mask_shift: \nblue_mask_size: \nblue_mask_shift: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"\nframebuffer_addr: \nframebuffer_width: \nframebuffer_height: \nframebuffer_pitch: \nframebuffer_bpp: \nmemory_model: \nred_mask_size: \nred_mask_shift: \ngreen_mask_size: \ngreen_mask_shift: \nblue_mask_size: \nblue_mask_shift: ");
	else if constexpr(std::same_as<char_type,char16_t>)
		return tag_size+details::string_literal_size(u"\nframebuffer_addr: \nframebuffer_width: \nframebuffer_height: \nframebuffer_pitch: \nframebuffer_bpp: \nmemory_model: \nred_mask_size: \nred_mask_shift: \ngreen_mask_size: \ngreen_mask_shift: \nblue_mask_size: \nblue_mask_shift: ");
	else if constexpr(std::same_as<char_type,char32_t>)
		return tag_size+details::string_literal_size(U"\nframebuffer_addr: \nframebuffer_width: \nframebuffer_height: \nframebuffer_pitch: \nframebuffer_bpp: \nmemory_model: \nred_mask_size: \nred_mask_shift: \ngreen_mask_size: \ngreen_mask_shift: \nblue_mask_size: \nblue_mask_shift: ");
	else
		return tag_size+details::string_literal_size(u8"\nframebuffer_addr: \nframebuffer_width: \nframebuffer_height: \nframebuffer_pitch: \nframebuffer_bpp: \nmemory_model: \nred_mask_size: \nred_mask_shift: \ngreen_mask_size: \ngreen_mask_shift: \nblue_mask_size: \nblue_mask_shift: ");
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_module>) noexcept
{
	constexpr std::size_t tag_size{
	details::cal_stvl2_cstr_reserved_size<char_type>(stvl2::stvl2_module::module_string_size)
	+print_reserve_size(io_reserve_type<char_type,void const*>)*2};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("module_begin: \nmodule_end: \nmodule_string: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"module_begin: \nmodule_end: \nmodule_string: ");
	else if constexpr(std::same_as<char_type,char16_t>)
		return tag_size+details::string_literal_size(u"module_begin: \nmodule_end: \nmodule_string: ");
	else if constexpr(std::same_as<char_type,char32_t>)
		return tag_size+details::string_literal_size(U"module_begin: \nmodule_end: \nmodule_string: ");
	else
		return tag_size+details::string_literal_size(u8"module_begin: \nmodule_end: \nmodule_string: ");
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_tag>,Iter iter,stvl2::stvl2_tag tg) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_tag(iter,tg);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_header_tag>,Iter iter,stvl2::stvl2_header_tag hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_header_tag(iter,hd);
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_header>,Iter iter,stvl2::stvl2_header hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_header(iter,hd);
}

static_assert(reserve_printable<char8_t,stvl2::stvl2_header>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_header_tag_framebuffer>,Iter iter,stvl2::stvl2_header_tag_framebuffer hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_header_tag_framebuffer(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_header_tag_framebuffer>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_header_tag_smp>,Iter iter,stvl2::stvl2_header_tag_smp hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_header_tag_smp(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_header_tag_smp>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_struct_tag>,Iter iter,stvl2::stvl2_struct_tag hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_struct_tag(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_struct_tag>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_struct>,Iter iter,stvl2::stvl2_struct const& hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_struct(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_struct>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_struct_tag_cmdline>,Iter iter,stvl2::stvl2_struct_tag_cmdline hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_struct_tag_cmdline(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_struct_tag_cmdline>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_struct_tag_memmap>,Iter iter,stvl2::stvl2_struct_tag_memmap hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_struct_tag_memmap(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_struct_tag_memmap>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_struct_tag_framebuffer>,Iter iter,stvl2::stvl2_struct_tag_framebuffer hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_struct_tag_framebuffer(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_struct_tag_framebuffer>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_mmap_type>,Iter iter,stvl2::stvl2_mmap_type hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_mmap_type(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_mmap_type>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_firmware_flags>,Iter iter,stvl2::stvl2_firmware_flags hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_firmware_flags(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_firmware_flags>);

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,stvl2::stvl2_module>,Iter iter,stvl2::stvl2_module hd) noexcept
{
	return details::print_reserve_define_impl_for_stvl2_module(iter,hd);
}
static_assert(reserve_printable<char8_t,stvl2::stvl2_module>);

}