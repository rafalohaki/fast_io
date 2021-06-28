#pragma once

namespace fast_io
{

namespace details
{
template<char8_t sign_ch,std::integral char_type>
inline constexpr char_type calculate_minus_sign_character() noexcept
{
	using char_type_nocvref = std::remove_cvref_t<char_type>;
	if constexpr(sign_ch==u8'-')
	{
		if constexpr(std::same_as<char_type_nocvref,char>)
			return '-';
		else if constexpr(std::same_as<char_type_nocvref,wchar_t>)
			return L'-';
		else
			return u8'-';
	}
	else if constexpr(sign_ch==u8'+')
	{
		if constexpr(std::same_as<char_type_nocvref,char>)
			return '+';
		else if constexpr(std::same_as<char_type_nocvref,wchar_t>)
			return L'+';
		else
			return u8'+';
	}
	else if constexpr(sign_ch==u8'0')
	{
		if constexpr(std::same_as<char_type_nocvref,char>)
			return '0';
		else if constexpr(std::same_as<char_type_nocvref,wchar_t>)
			return L'0';
		else
			return u8'0';
	}
	else if constexpr(sign_ch==u8'1')
	{
		if constexpr(std::same_as<char_type_nocvref,char>)
			return '1';
		else if constexpr(std::same_as<char_type_nocvref,wchar_t>)
			return L'1';
		else
			return u8'1';
	}
	else if constexpr(sign_ch==u8'E')
	{
		if constexpr(std::same_as<char_type_nocvref,char>)
			return 'E';
		else if constexpr(std::same_as<char_type_nocvref,wchar_t>)
			return L'E';
		else
			return u8'E';
	}
	else if constexpr(sign_ch==u8'e')
	{
		if constexpr(std::same_as<char_type_nocvref,char>)
			return 'e';
		else if constexpr(std::same_as<char_type_nocvref,wchar_t>)
			return L'e';
		else
			return u8'e';
	}
}

template<char8_t ch,std::integral char_type>
inline constexpr char_type sign_ch{calculate_minus_sign_character<ch,char_type>()};

}

namespace manipulators
{

enum class scalar_placement:char8_t
{
none,left,middle,right,internal
};

enum class floating_format:char8_t
{
general,fixed,scientific,hexafloat,precise
};

struct scalar_flags
{
	std::size_t base{10};
	bool boolalpha{};
	bool showbase{};
	bool showpos{};
	bool skipws{};
	bool uppercase_showbase{};
	bool uppercase{};
	bool uppercase_e{};
	bool showpos_e{true};
	bool comma{};
	bool localeparse{};
	bool full{};
	scalar_placement placement{scalar_placement::none};
	floating_format floating{floating_format::general};
};

template<scalar_flags flags,typename T>
struct scalar_manip_t
{
	using reference_type = T;
	reference_type reference;
};

template<std::size_t bs,bool upper=false,bool shbase=false,bool full=false,::fast_io::details::my_integral int_type>
requires (2<=bs&&bs<=36)
inline constexpr scalar_manip_t<scalar_flags{.base=bs,.showbase=shbase,.uppercase=((bs<=10)?false:upper),.full=false,.floating=floating_format::fixed},int_type> base(int_type t) noexcept
{
	return {t};
}

template<scalar_flags flags,::fast_io::details::my_integral int_type>
requires (2<=flags.base&&flags.base<=36)
inline constexpr scalar_manip_t<flags,int_type> scalar(int_type t) noexcept
{
	return {t};
}

template<bool upper=false>
inline constexpr scalar_manip_t<scalar_flags{.boolalpha=true,.uppercase=upper},bool> blvw(bool b) noexcept
{
	return {b};
}

}

namespace details
{
template<std::integral char_type,std::size_t base>
inline constexpr auto generate_base_prefix_array() noexcept
{
	static_assert(2<=base&&base<=36);
	if constexpr(base<10)
	{
	//0[9]0000
		if constexpr(std::same_as<char_type,char>&&('A'!=u8'A'))
			return ::fast_io::freestanding::array<char_type,4>{'0','[',
			static_cast<char>(static_cast<char8_t>('0')+static_cast<char8_t>(base)),']'};
		else if constexpr(std::same_as<char_type,wchar_t>&&(L'A'!=u8'A'))
		{
			using wchar_t_unsigned = std::make_unsigned_t<wchar_t>;
			return ::fast_io::freestanding::array<char_type,4>{L'0',L'[',
			static_cast<wchar_t_unsigned>(static_cast<wchar_t_unsigned>(L'0')+static_cast<wchar_t_unsigned>(base)),L']'};
		}
		else
		{
			using char_type_unsigned = std::make_unsigned_t<char_type>;
			return ::fast_io::freestanding::array<char_type,4>{u8'0',u8'[',
			static_cast<char_type>(static_cast<char_type_unsigned>(u8'0'+static_cast<char8_t>(base))),u8']'};
		}
	}
	else
	{
		constexpr char8_t decade{static_cast<char8_t>(static_cast<char8_t>(base)/static_cast<char8_t>(10u))},
			unit{static_cast<char8_t>(static_cast<char8_t>(base)%static_cast<char8_t>(10u))};
		if constexpr(std::same_as<char_type,char>&&('A'!=u8'A'))
			return ::fast_io::freestanding::array<char_type,5>{'0','[',
			static_cast<char>(static_cast<char8_t>('0')+static_cast<char8_t>(decade)),
			static_cast<char>(static_cast<char8_t>('0')+static_cast<char8_t>(unit)),']'};
		else if constexpr(std::same_as<char_type,wchar_t>&&(L'A'!=u8'A'))
		{
			using wchar_t_unsigned = std::make_unsigned_t<wchar_t>;
			return ::fast_io::freestanding::array<char_type,5>{L'0',L'[',
			static_cast<wchar_t_unsigned>(static_cast<wchar_t_unsigned>(L'0')+static_cast<wchar_t_unsigned>(decade)),
			static_cast<wchar_t_unsigned>(static_cast<wchar_t_unsigned>(L'0')+static_cast<wchar_t_unsigned>(unit)),
			L']'};
		}
		else
		{
			using char_type_unsigned = std::make_unsigned_t<char_type>;
			return ::fast_io::freestanding::array<char_type,5>{u8'0',u8'[',
			static_cast<char_type>(static_cast<char_type_unsigned>(u8'0'+static_cast<char8_t>(decade))),
			static_cast<char_type>(static_cast<char_type_unsigned>(u8'0'+static_cast<char8_t>(unit))),
			u8']'};
		}
	}
}

template<std::integral char_type,std::size_t base>
inline constexpr auto base_prefix_array{generate_base_prefix_array<char_type,base>()};

template<std::size_t base,bool uppercase_showbase,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_show_base_impl(Iter iter)
{
	static_assert(2<=base&&base<=36);
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(base==2)
	{
		if constexpr(uppercase_showbase)
		{
			if constexpr(std::same_as<char_type,char>)
				iter=copy_string_literal("0B",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				iter=copy_string_literal(L"0B",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				iter=copy_string_literal(u"0B",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				iter=copy_string_literal(U"0B",iter);
			else
				iter=copy_string_literal(u8"0B",iter);
		}
		else
		{
			if constexpr(std::same_as<char_type,char>)
				iter=copy_string_literal("0b",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				iter=copy_string_literal(L"0b",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				iter=copy_string_literal(u"0b",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				iter=copy_string_literal(U"0b",iter);
			else
				iter=copy_string_literal(u8"0b",iter);
		}
	}
	if constexpr(base==3)
	{
		if constexpr(uppercase_showbase)
		{
			if constexpr(std::same_as<char_type,char>)
				iter=copy_string_literal("0T",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				iter=copy_string_literal(L"0T",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				iter=copy_string_literal(u"0T",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				iter=copy_string_literal(U"0T",iter);
			else
				iter=copy_string_literal(u8"0T",iter);
		}
		else
		{
			if constexpr(std::same_as<char_type,char>)
				iter=copy_string_literal("0t",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				iter=copy_string_literal(L"0t",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				iter=copy_string_literal(u"0t",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				iter=copy_string_literal(U"0t",iter);
			else
				iter=copy_string_literal(u8"0t",iter);
		}
	}
	else if constexpr(base==8)
	{
		*iter=sign_ch<u8'0',char_type>;
		++iter;
	}
	else if constexpr(base==16)
	{
		if constexpr(uppercase_showbase)
		{
			if constexpr(std::same_as<char_type,char>)
				iter=copy_string_literal("0X",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				iter=copy_string_literal(L"0X",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				iter=copy_string_literal(u"0X",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				iter=copy_string_literal(U"0X",iter);
			else
				iter=copy_string_literal(u8"0X",iter);
		}
		else
		{
			if constexpr(std::same_as<char_type,char>)
				iter=copy_string_literal("0x",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				iter=copy_string_literal(L"0x",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				iter=copy_string_literal(u"0x",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				iter=copy_string_literal(U"0x",iter);
			else
				iter=copy_string_literal(u8"0x",iter);
		}
	}
	else if constexpr(base!=10)
	{
		constexpr auto arr{base_prefix_array<char_type,base>};
		constexpr std::size_t sz{arr.size()};
		iter=non_overlapped_copy_n(arr.data(),sz,iter);
	}
	return iter;
}

template<std::size_t base,bool uppercase,::fast_io::freestanding::random_access_iterator Iter,typename T>
inline constexpr void print_reserve_integral_main_impl(Iter iter,T t,std::size_t len)
{
	if constexpr(base<=10&&uppercase)
	{
		return print_reserve_integral_main_impl<base,false>(iter,t,len);//prevent duplications
	}
	else
	{
		using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
		constexpr auto tb{::fast_io::details::get_shared_inline_constexpr_base_table<char8_t,base,uppercase>().element};
		constexpr T pw{static_cast<T>(base*base)};
		std::size_t len2{len>>1};
		for(std::size_t i{};i!=len2;++i)
		{
			auto const rem{t%pw};
			t/=pw;
			non_overlapped_copy_n(tb[rem].element,2,iter-=2);
		}
		if((len&1))
		{
			if constexpr(base<=10)
				*--iter=static_cast<char_type>(t+sign_ch<u8'0',char_type>);
			else
				*--iter=tb[t].element[1];
		}
	}
}

template<std::size_t base=10,
	bool showbase=false,
	bool uppercase_showbase=false,
	bool showpos=false,
	bool uppercase=false,
	bool full=false,
	typename int_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_integral_define(Iter first,int_type t)
{
	if constexpr(base<=10&&uppercase)
	{
		return print_reserve_integral_define<base,showbase,uppercase_showbase,showpos,false>(first,t);//prevent duplications
	}
	else
	{
		static_assert((2<=base)&&(base<=36));
		using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
		using unsigned_type = ::fast_io::details::my_make_unsigned_t<int_type>;
		unsigned_type u{static_cast<unsigned_type>(t)};
		if constexpr(showpos)
		{
			if constexpr(::fast_io::details::my_unsigned_integral<int_type>)
			{
				*first=sign_ch<u8'+',char_type>;
			}
			else
			{
				if(t<0)
				{
					*first=sign_ch<u8'-',char_type>;
					constexpr unsigned_type zero{};
					u=zero-u;
				}
				else
				{
					*first=sign_ch<u8'+',char_type>;
				}
			}
			++first;
		}
		else
		{
			if constexpr(::fast_io::details::my_signed_integral<int_type>)
			{
				if(t<0)
				{
					*first=sign_ch<u8'-',char_type>;
					++first;
					constexpr unsigned_type zero{};
					u=zero-u;
				}
			}
		}
		if constexpr(showbase)
			first=print_reserve_show_base_impl<base,uppercase_showbase>(first);
		if constexpr(std::same_as<int_type,bool>)
		{
			*first=t?sign_ch<u8'1',char_type>:sign_ch<u8'0',char_type>;
			++first;
		}
		else if constexpr(full)
		{
			constexpr std::size_t sz{::fast_io::details::cal_max_int_size<::fast_io::details::my_make_unsigned_t<int_type>,base>()};
			print_reserve_integral_main_impl<base,uppercase>(first+=sz,u,sz);
		}
		else
		{
			std::size_t sz{chars_len<base,false>(u)};
			print_reserve_integral_main_impl<base,uppercase>(first+=sz,u,sz);
		}
		return first;
	}
}


template<
std::size_t base=10,
bool showbase=false,
bool showpos=false,
my_integral T>
inline constexpr std::size_t print_reserve_scalar_size_impl()
{
	std::size_t total_sum{};
	if constexpr(showpos)
		++total_sum;
	if constexpr(showbase)
	{
		if constexpr(base==2||base==3||base==16)
			total_sum+=2;	//0b 0t 0x
		else if constexpr(base==8)
			++total_sum;	//0
		else if constexpr(base<10)
			total_sum+=4;	//0[9]
		else if constexpr(10<base)
			total_sum+=5;	//0[36]
		//base==10 does not have showbase
	}
	if constexpr(std::same_as<std::remove_cvref_t<T>,bool>)
		++total_sum;
	else
	{
		if constexpr(my_signed_integral<T>)
			++total_sum;
		total_sum+=::fast_io::details::cal_max_int_size<::fast_io::details::my_make_unsigned_t<T>,base>();
	}
	return total_sum;
}

template<std::size_t base,
bool showbase,
bool showpos,
std::integral T>
inline constexpr std::size_t print_integer_reserved_size_cache{print_reserve_scalar_size_impl<base,showbase,showpos,T>()};

template<bool uppercase,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_boolalpha_impl(Iter iter,bool b)
{
	using char_type = std::remove_cvref_t<::fast_io::freestanding::iter_value_t<Iter>>;
	if(b)
	{
		if constexpr(std::same_as<char_type,char>)
		{
			if constexpr(uppercase)
				return copy_string_literal("TRUE",iter);
			else
				return copy_string_literal("true",iter);
		}
		else if constexpr(std::same_as<char_type,wchar_t>)
		{
			if constexpr(uppercase)
				return copy_string_literal(L"TRUE",iter);
			else
				return copy_string_literal(L"true",iter);
		}
		else if constexpr(std::same_as<char_type,char16_t>)
		{
			if constexpr(uppercase)
				return copy_string_literal(u"TRUE",iter);
			else
				return copy_string_literal(u"true",iter);
		}
		else if constexpr(std::same_as<char_type,char32_t>)
		{
			if constexpr(uppercase)
				return copy_string_literal(U"TRUE",iter);
			else
				return copy_string_literal(U"true",iter);
		}
		else
		{
			if constexpr(uppercase)
				return copy_string_literal(u8"TRUE",iter);
			else
				return copy_string_literal(u8"true",iter);
		}
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
		{
			if constexpr(uppercase)
				return copy_string_literal("FALSE",iter);
			else
				return copy_string_literal("false",iter);
		}
		else if constexpr(std::same_as<char_type,wchar_t>)
		{
			if constexpr(uppercase)
				return copy_string_literal(L"FALSE",iter);
			else
				return copy_string_literal(L"false",iter);
		}
		else if constexpr(std::same_as<char_type,char16_t>)
		{
			if constexpr(uppercase)
				return copy_string_literal(u"FALSE",iter);
			else
				return copy_string_literal(u"false",iter);
		}
		else if constexpr(std::same_as<char_type,char32_t>)
		{
			if constexpr(uppercase)
				return copy_string_literal(U"FALSE",iter);
			else
				return copy_string_literal(U"false",iter);
		}
		else
		{
			if constexpr(uppercase)
				return copy_string_literal(u8"FALSE",iter);
			else
				return copy_string_literal(u8"false",iter);
		}
	}
}

}

template<std::integral char_type,typename T>
requires (details::my_integral<T>||std::same_as<std::remove_cv_t<T>,std::byte>)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,T>) noexcept
{
	if constexpr(std::same_as<std::remove_cv_t<T>,std::byte>)
		return details::print_integer_reserved_size_cache<10,false,false,char8_t>;
	else
		return details::print_integer_reserved_size_cache<10,false,false,T>;
}

template<freestanding::random_access_iterator Iter,typename T>
requires (details::my_integral<T>||std::same_as<std::remove_cv_t<T>,std::byte>)
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]//always inline to reduce inline depth in GCC and LLVM clang
#endif
inline constexpr Iter print_reserve_define(io_reserve_type_t<freestanding::iter_value_t<Iter>,T>,Iter iter,T t) noexcept
{
	if constexpr(std::same_as<std::remove_cv_t<T>,std::byte>)
		return details::print_reserve_integral_define(iter,static_cast<char8_t>(t));
	else
		return details::print_reserve_integral_define(iter,t);
}


template<std::integral char_type,manipulators::scalar_flags flags,typename T>
requires (details::my_integral<T>||std::same_as<std::remove_cv_t<T>,std::byte>)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::scalar_manip_t<flags,T>>) noexcept
{
	if constexpr(flags.boolalpha)
		return 5;
	else if constexpr(std::same_as<std::remove_cv_t<T>,std::byte>)
		return details::print_integer_reserved_size_cache<flags.base,flags.showbase,flags.showpos,char8_t>;
	else
		return details::print_integer_reserved_size_cache<flags.base,flags.showbase,flags.showpos,T>;
}

template<freestanding::random_access_iterator Iter,manipulators::scalar_flags flags,typename T>
requires (details::my_integral<T>||std::same_as<std::remove_cv_t<T>,std::byte>)
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]//always inline to reduce inline depth in GCC and LLVM clang
#endif
inline constexpr Iter print_reserve_define(io_reserve_type_t<freestanding::iter_value_t<Iter>,manipulators::scalar_manip_t<flags,T>>,Iter iter,manipulators::scalar_manip_t<flags,T> t) noexcept
{
	if constexpr(flags.boolalpha)
		return details::print_reserve_boolalpha_impl<flags.uppercase>(iter,t.reference);
	else if constexpr(std::same_as<std::remove_cv_t<T>,std::byte>)
		return details::print_reserve_integral_define<flags.base,flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase>(iter,static_cast<char8_t>(t.reference));
	else
		return details::print_reserve_integral_define<flags.base,flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase>(iter,t.reference);
}

}
