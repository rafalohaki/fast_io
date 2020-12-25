#pragma once

namespace fast_io
{

namespace details
{

template<std::random_access_iterator Iter>
inline constexpr Iter skip_zero(Iter begin, Iter end) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	if constexpr(std::same_as<char_type,char>)
		for (; begin != end && *begin=='0'; ++begin);
	else if constexpr(std::same_as<char_type,wchar_t>)
		for (; begin != end && *begin==L'0'; ++begin);
	else
		for (; begin != end && *begin==u8'0'; ++begin);
	return begin;
}

template<char8_t base,std::integral char_type>
requires (2<=base&&base<=36)
inline constexpr bool char_digit_to_literal(std::make_unsigned_t<char_type>& ch) noexcept
{
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	constexpr bool ebcdic{exec_charset_is_ebcdic<char_type>()};
	constexpr char_type base_char_type(base);
	if constexpr(base<=10)
	{
		if constexpr(ebcdic)
			ch-=static_cast<unsigned_char_type>(240);
		else
			ch-=static_cast<unsigned_char_type>(u8'0');
		return base_char_type<=ch;
	}
	else
	{
		if constexpr(ebcdic)
		{

			if constexpr(base<=19)
			{
				constexpr char_type mns{base-10};
				char_type ch2(ch);
				ch2-=0xC1;
				char_type ch3(ch);
				ch3-=0x81;
				ch-=0xF0;
				if(ch2<mns)
					ch=ch2+static_cast<unsigned_char_type>(10);
				else if(ch3<mns)
					ch=ch3+static_cast<unsigned_char_type>(10);
				else if(10<=ch)
					return true;
				return false;
			}
			else if constexpr(base<=28)
			{
				constexpr char_type mns{base-19};
				char_type ch2(ch);
				ch2-=0xC1;
				char_type ch3(ch);
				ch3-=0x81;
				char_type ch4(ch);
				ch2-=0xD1;
				char_type ch5(ch);
				ch3-=0x91;
				ch-=0xF0;
				if(ch4<mns)
					ch=ch4+static_cast<unsigned_char_type>(19);
				else if(ch5<mns)
					ch=ch5+static_cast<unsigned_char_type>(19);
				else if(ch2<9)
					ch=ch2+static_cast<unsigned_char_type>(10);
				else if(ch3<9)
					ch=ch3+static_cast<unsigned_char_type>(10);
				else if(10<=ch)
					return true;
				return false;
			}
			else
			{
				constexpr char_type mns{base-27};
				char_type ch2(ch);
				ch2-=0xC1;
				char_type ch3(ch);
				ch3-=0x81;
				char_type ch4(ch);
				ch4-=0xD1;
				char_type ch5(ch);
				ch5-=0x91;
				char_type ch6(ch);
				ch6-=0xE2;
				char_type ch7(ch);
				ch7-=0xA2;
				ch-=0xF0;
				if(ch6<mns)
					ch=ch6+static_cast<unsigned_char_type>(27);
				else if(ch7<mns)
					ch=ch7+static_cast<unsigned_char_type>(27);
				else if(ch4<9)
					ch=ch4+static_cast<unsigned_char_type>(19);
				else if(ch5<9)
					ch=ch5+static_cast<unsigned_char_type>(19);
				else if(ch2<9)
					ch=ch2+static_cast<unsigned_char_type>(10);
				else if(ch3<9)
					ch=ch3+static_cast<unsigned_char_type>(10);
				else if(10<=ch)
					return true;
				return false;
			}
		}
		else
		{
			constexpr char_type mns{base-10};
			char_type ch2(ch);
			ch2-=u8'A';
			char_type ch3(ch);
			ch3-=u8'a';
			ch-=u8'0';
			if(ch2<mns)
				ch=ch2+static_cast<unsigned_char_type>(10);
			else if(ch3<mns)
				ch=ch3+static_cast<unsigned_char_type>(10);
			else if(10<=ch)
				return true;
			return false;
		}
	}
}

template<char8_t base,std::random_access_iterator Iter,my_unsigned_integral muint>
inline constexpr void from_chars_main(Iter& b,Iter e,muint& res) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	constexpr unsigned_char_type base_char_type(base);
	for(;b!=e;++b)
	{
		unsigned_char_type result(*b);
		if(char_digit_to_literal<base,char_type>(result))[[unlikely]]
			break;
		res*=base_char_type;
		res+=result;
	}
}

template<char8_t base,std::random_access_iterator Iter>
inline constexpr Iter skip_digits(Iter b,Iter e) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	for(;b!=e;++b)
	{
		unsigned_char_type result(*b);
		if(char_digit_to_literal<base,char_type>(result))
			break;
	}
	return b;
}

template<char8_t base,std::random_access_iterator Iter,my_unsigned_integral muint>
inline constexpr void probe_overflow(Iter& b,Iter e,muint& res,std::size_t& sz) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	if(b==e)
		return;
	unsigned_char_type result(*b);
	if(char_digit_to_literal<base,char_type>(result))[[unlikely]]
		return;
	constexpr muint risky_value{std::numeric_limits<muint>::max()/base};
	constexpr unsigned_char_type risky_digit(std::numeric_limits<muint>::max()%base);
	constexpr std::size_t npos(-1);
	if((risky_value<res)||(risky_value==res&&risky_digit<result))
	{
		sz=npos;
		return;
	}
	res*=base;
	res+=result;
	++b;
	if(b==e)
		return;
	{
		unsigned_char_type result(*b);
		if(char_digit_to_literal<base,char_type>(result))
			return;
		sz=npos;
		++b;
		b=skip_digits<base>(b,e);
	}
}

template<char8_t base,bool ignore=false,std::random_access_iterator Iter,my_unsigned_integral muint>
inline constexpr void from_chars_u64(Iter& b,Iter e,muint& res,std::size_t& sz) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	constexpr std::size_t max_size{cal_max_int_size<muint,base>()-1};
	if constexpr(ignore)
	{
		b=skip_zero(b,e);
		std::size_t iter_diff(e-b);
		if(max_size<iter_diff)[[likely]]
		{
			iter_diff=max_size;
			auto from_chars_ed{b+max_size};
			auto start{b};
			from_chars_main<base>(b,from_chars_ed,res);
			sz=b-start;
			if(b==from_chars_ed)[[unlikely]]
			{
				probe_overflow<base>(b,e,res,sz);
				return;
			}
		}
		else
		{
			auto start{b};
			from_chars_main<base>(b,e,res);
			sz=b-start;
		}
	}
	else
	{
		constexpr std::size_t npos(-1);
		if(sz==npos)
		{
			b=skip_digits<base>(b,e);
			return;
		}
		if(!res)
			b=skip_zero(b,e);
		std::size_t iter_diff(e-b);
		std::size_t max_size_msz(max_size-sz);
		if(max_size_msz<iter_diff)[[likely]]
		{
			iter_diff=max_size_msz;
			auto from_chars_ed{b+max_size_msz};
			auto start{b};
			from_chars_main<base>(b,from_chars_ed,res);
			sz+=b-start;
			if(b==from_chars_ed)[[unlikely]]
			{
				probe_overflow<base>(b,e,res,sz);
				return;
			}
		}
		else
		{
			auto start{b};
			from_chars_main<base>(b,e,res);
			sz+=b-start;
		}
	}
}

}

template<details::my_integral T, std::random_access_iterator Iter>
inline constexpr Iter scan_skip_define(scan_skip_type_t<parameter<T&>>, Iter beg, Iter ed) noexcept
{
	return scan_skip_space(beg, ed);
}

namespace details::ctx_scan_integer
{
template<char8_t base, bool contiguous_only, std::random_access_iterator Iter, details::my_integral T>
struct voldmort
{
	Iter iter;
	std::errc code{};
	[[no_unique_address]] std::conditional_t<contiguous_only, details::empty,std::uint64_t> value{};
	[[no_unique_address]] std::conditional_t<contiguous_only, details::empty,std::size_t> size{};
	[[no_unique_address]] std::conditional_t<!contiguous_only&&details::my_signed_integral<T>, bool, empty> minus{};
	inline constexpr bool test_eof(parameter<T&> t) noexcept requires(!contiguous_only)
	{
		constexpr std::size_t npos(-1);
		if(size==npos)
		{
			code=std::errc::result_out_of_range;
			return true;
		}
		code={};
		if constexpr(!(std::unsigned_integral<T>&&sizeof(T)==sizeof(std::uint64_t)))
		{
			if constexpr(std::unsigned_integral<T>)
			{
				if(value>static_cast<std::uint64_t>(std::numeric_limits<T>::max()))
				{
					code=std::errc::result_out_of_range;
					return true;
				}					
			}
			else
			{
				if(value>(static_cast<std::uint64_t>(std::numeric_limits<T>::max())+minus))
				{
					code=std::errc::result_out_of_range;
					return true;
				}
			}
		}
		if constexpr(my_signed_integral<T>)
		{
			if(minus)
				t.reference=static_cast<std::uint64_t>(0)-value;
			else
				t.reference=value;
		}
		else
			t.reference=value;
		return true;
	}
	inline constexpr void operator()(Iter begin, Iter end,parameter<T&> t) noexcept requires(!contiguous_only)
	{
		iter=begin;
		details::from_chars_u64<base>(iter,end,value,size);
		code = {};
		if(iter==end)
		{
			code = std::errc::resource_unavailable_try_again;
			return;
		}
		constexpr std::size_t npos(-1);
		if(size==npos)
		{
			code=std::errc::result_out_of_range;
			return;
		}
		if constexpr(!(std::unsigned_integral<T>&&sizeof(T)==sizeof(std::uint64_t)))
		{
			if constexpr(std::unsigned_integral<T>)
			{
				if(value>static_cast<std::uint64_t>(std::numeric_limits<T>::max()))
				{
					code=std::errc::result_out_of_range;
					return;
				}
			}
			else
			{
				if(value>static_cast<std::uint64_t>(std::numeric_limits<T>::max())+minus)
				{
					code=std::errc::result_out_of_range;
					return;
				}
			}
		}
		if constexpr(my_signed_integral<T>)
		{
			if(minus)
				t.reference=static_cast<std::uint64_t>(0)-value;
			else
				t.reference=value;
		}
		else
			t.reference=value;
	}
	inline constexpr voldmort(Iter begin, Iter end, T& t) noexcept
	{
		iter = begin;
		if constexpr (contiguous_only)
		{
			bool minus{};
			using char_type = std::iter_value_t<Iter>;
			/*Dealing with EBCDIC exec-charset */
			if constexpr(my_signed_integral<T>)
			{
				if constexpr(std::same_as<char_type,char>)
					minus=(*iter == '-');
				else if constexpr(std::same_as<char_type,wchar_t>)
					minus=(*iter == L'-');
				else
					minus=(*iter == u8'-');
				if constexpr(std::contiguous_iterator<Iter>)
					iter+=minus;
				else
				{
					if(minus)
						++iter;
				}
			}
			std::uint64_t temp{};
			if(details::from_chars_u64<base>(iter,end,temp))
			{
				code=std::errc::result_out_of_range;
				return;
			}
			if(begin==iter)
			{
				code=std::errc::invalid_argument;
				return;
			}
			constexpr std::size_t npos(-1);
			if(size==npos)
			{
				code=std::errc::result_out_of_range;
				return;
			}
			if constexpr(!(std::unsigned_integral<T>&&sizeof(T)==sizeof(std::uint64_t)))
			{
				if constexpr(std::unsigned_integral<T>)
				{
					if(temp>static_cast<std::uint64_t>(std::numeric_limits<T>::max()))
					{
						code=std::errc::result_out_of_range;
						return;
					}					
				}
				else
				{
					if(temp>(static_cast<std::uint64_t>(std::numeric_limits<T>::max())+minus))
					{
						code=std::errc::result_out_of_range;
						return;
					}
				}
			}
			if constexpr(my_signed_integral<T>)
			{
				if(minus)
					t=0-temp;
				else
					t=temp;
			}
			else
				t=temp;
		}
		else
		{
			using char_type = std::iter_value_t<Iter>;
			/*Dealing with EBCDIC exec-charset */
			if constexpr(my_signed_integral<T>)
			{
				if constexpr(std::same_as<char_type,char>)
					minus=(*iter == '-');
				else if constexpr(std::same_as<char_type,wchar_t>)
					minus=(*iter == L'-');
				else
					minus=(*iter == u8'-');
				if constexpr(std::contiguous_iterator<Iter>)
					iter+=minus;
				else
				{
					if(minus)
						++iter;
				}
			}
			details::from_chars_u64<base,true>(iter,end,value,size);
			if(begin==iter)
			{
				code=std::errc::invalid_argument;
				return;
			}
			if(iter==end)
			{
				code=std::errc::resource_unavailable_try_again;
				return;
			}
			constexpr std::size_t npos(-1);
			if(size==npos)
			{
				code=std::errc::result_out_of_range;
				return;
			}
			if constexpr(!(std::unsigned_integral<T>&&sizeof(T)==sizeof(std::uint64_t)))
			{
				if constexpr(std::unsigned_integral<T>)
				{
					if(value>static_cast<std::uint64_t>(std::numeric_limits<T>::max()))
					{
						code=std::errc::result_out_of_range;
						return;
					}	
				}
				else
				{
					if(value>static_cast<std::uint64_t>(std::numeric_limits<T>::max())+(minus))
					{
						code=std::errc::result_out_of_range;
						return;
					}
				}
			}
			if constexpr(my_signed_integral<T>)
			{
				if(minus)
					t=static_cast<std::uint64_t>(0)-value;
				else
					t=value;
			}
			else
				t=value;
		}
	}
};
}

template<bool contiguous_only, std::random_access_iterator Iter, details::my_integral T>
inline constexpr auto scan_context_define(scan_context_t<contiguous_only>, Iter begin, Iter end, parameter<T&> t) noexcept
{
	return details::ctx_scan_integer::voldmort<10, contiguous_only, Iter, T>(begin, end, t.reference);
}

}

