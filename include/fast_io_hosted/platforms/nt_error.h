#pragma once

namespace fast_io
{

class nt_error
{
public:
	std::uint32_t ntstatus{};
	explicit nt_error(std::uint32_t status):ntstatus(status){}
	inline constexpr std::uint32_t code() const noexcept
	{
		return ntstatus;
	}
#if 0
	void report(error_reporter& report) const override
	{
		details::report_win32_error(report,win32::nt::rtl_nt_status_to_dos_error(ntstatus));
	}
#endif
};

[[noreturn]] inline void throw_nt_error([[maybe_unused]] std::uint32_t err)
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw nt_error(err);
#endif
#else
	fast_terminate();
#endif
}

namespace details
{

template<bool enable=true,::fast_io::freestanding::contiguous_iterator Iter>
requires (enable)
inline constexpr Iter print_reserve_nt_error_impl(Iter iter,std::uint32_t ntstatus) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
		iter=copy_string_literal("[nt:0x",iter);
	else if constexpr(std::same_as<char_type,wchar_t>)
		iter=copy_string_literal(L"[nt:0x",iter);
	else if constexpr(std::same_as<char_type,char16_t>)
		iter=copy_string_literal(u"[nt:0x",iter);
	else if constexpr(std::same_as<char_type,char32_t>)
		iter=copy_string_literal(U"[nt:0x",iter);
	else
		iter=copy_string_literal(u8"[nt:0x",iter);
	iter=print_reserve_define(io_reserve_type<char_type,::fast_io::manipulators::base_full_t<16,true,std::uint32_t>>,iter,{ntstatus});

	if constexpr(std::same_as<char_type,char>)
		*iter=']';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L']';
	else
		*iter=u8']';
	++iter;
	return print_reserve_define_win32_error_impl(iter,win32::nt::rtl_nt_status_to_dos_error(ntstatus));
}

}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,nt_error>) noexcept
{
	constexpr std::size_t full_size(print_reserve_size(io_reserve_type<char_type,::fast_io::manipulators::base_full_t<16,true,std::uint32_t>>)+print_reserve_size(io_reserve_type<char_type,win32_error>));
	if constexpr(std::same_as<char_type,char>)
	{
		return full_size+details::string_literal_size("[nt:0x]");
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		return full_size+details::string_literal_size(L"[nt:0x]");
	}
	else
		return full_size+details::string_literal_size(u8"[nt:0x]");
}

template<::fast_io::freestanding::contiguous_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<::fast_io::freestanding::iter_value_t<Iter>,nt_error>,Iter iter,nt_error const& e) noexcept
{
	return details::print_reserve_nt_error_impl(iter,e.code());
}

}