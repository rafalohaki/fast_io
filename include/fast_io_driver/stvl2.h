#pragma once
#include"../fast_io_concept.h"
#include"bootloader/stvl2.h"

namespace fast_io
{

namespace details
{
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
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_header_tag>) noexcept
{
	return 32;
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,stvl2::stvl2_header>) noexcept
{
	constexpr std::size_t tag_size{print_reserve_size(io_reserve_type<char_type,stvl2::stvl2_header_tag>)
	+print_resrve_size(io_reserve_type<char_type,void const*>)*2
	+print_resrve_size(io_reserve_type<char_type,std::uint64_t>)};
	if constexpr(std::same_as<char_type,char>)
		return tag_size+details::string_literal_size("entry_point: \nstack: \nflags: \ntags: ");
	else if constexpr(std::same_as<char_type,wchar_t>)
		return tag_size+details::string_literal_size(L"entry_point: \nstack: \nflags: \ntags: ");
	else
		return tag_size+details::string_literal_size(u8"entry_point: \nstack: \nflags: \ntags: ");
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

}