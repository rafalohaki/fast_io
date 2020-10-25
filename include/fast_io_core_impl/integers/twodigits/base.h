#pragma once

namespace fast_io
{

namespace details
{

template<char8_t base,bool uppercase,bool point=false,char32_t dec=u8'.',bool transparent=false,std::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr auto output_base_number_impl(Iter iter,U a)
{
//number: 0:48 9:57
//upper: 65 :A 70: F
//lower: 97 :a 102 :f
	constexpr auto &table(details::shared_static_base_table<std::iter_value_t<Iter>,base,uppercase,transparent>::table);
	constexpr std::uint32_t pw(static_cast<std::uint32_t>(table.size()));
	constexpr std::size_t chars(table.front().size());
	for(;pw<=a;)
	{
		auto const rem(a%pw);
		a/=pw;
		non_overlapped_copy_n(table[rem].data(),chars,iter-=chars);
	}
	if constexpr(chars==2)
	{
		if(base<=a)
		{
			auto const& tm(table[a]);
			
			if constexpr(point)
			{
				*--iter=tm[1];
				*--iter=dec;
				*--iter=tm.front();
			}
			else
			{
				non_overlapped_copy_n(tm.data(),chars,iter-=chars);
			}
		}
		else
		{
			if constexpr(point)
				*--iter=dec;
			if constexpr(transparent)
				*--iter=a;
			else
			{
				if constexpr(10 < base)
				{
					if(a<10)
						*--iter = a+0x30;
					else
					{
						if constexpr (uppercase)
							*--iter = a+55;	
						else
							*--iter = a+87;
					}
				}
				else
					*--iter=a+0x30;
			}
		}
	}
	else
	{
		if(base<=a)
		{
			auto const& tm(table[a]);
			auto i(tm.data());
			for(;*i==0x30;++i);
			auto const ed(tm.data()+chars);
			if constexpr(point)
			{
				non_overlapped_copy_n(i+1,ed,iter-=ed-(i+1));
				*--iter=dec;
				*--iter=*i;
			}
			else
				non_overlapped_copy_n(i,ed,iter-=ed-i);
		}
		else
		{
			if constexpr(point)
				*--iter=dec;
			if constexpr(transparent)
				*--iter=a;
			else
			{
				if constexpr(10 < base)
				{
					if(a<10)
						*--iter = a+0x30;
					else
					{
						if constexpr (uppercase)
							*--iter = a+55;	
						else
							*--iter = a+87;
					}
				}
				else
					*--iter=a+0x30;
			}
		}
	}
	return iter;
}

template<bool sign,std::uint8_t base>
requires (0x2<base&&base<=0x36)
struct is_numerical
{
template<std::integral T>
inline constexpr bool operator()(T ch)
{
	std::make_unsigned_t<T> e(ch);
	if constexpr(sign)
	{
		if constexpr(base<=0xA)
			return (e==0x2d)||static_cast<std::uint8_t>(e-0x30)<base;
		else
		{
			constexpr std::uint8_t basem10(base-0xa);
			return (e==0x2d)||static_cast<std::uint8_t>((e-0x30)<0xA)||
				(static_cast<std::uint8_t>(e-0x41)<basem10)||
				(static_cast<std::uint8_t>(e-0x61)<basem10);
		}
	}
	else
	{
		if constexpr(base<=0xA)
			return static_cast<std::uint8_t>(e-0x30)<base;
		else
		{
			constexpr std::uint8_t basem10(base-0xa);
			return (static_cast<std::uint8_t>(e-0x30)<0xA)||
				(static_cast<std::uint8_t>(e-0x41)<basem10)||
				(static_cast<std::uint8_t>(e-0x61)<basem10);
		}
	}
}
};
template<std::integral T>
inline constexpr bool is_space(T const u)
{
	if constexpr(std::unsigned_integral<std::remove_cvref_t<T>>)
	{
		return (u==0x20)|((u-0x9)<0x4);
	}
	else
	{
		std::make_unsigned_t<T> const e(u);
		return (e==0x20)|((e-0x9)<0x4);
	}
}


namespace twodigits
{

template<char8_t base=10,bool uppercase=false,std::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr std::size_t output_unsigned(Iter str,U value)
{
	std::size_t const len{chars_len<base>(value)};
	output_base_number_impl<base,uppercase>(str+len,value);
	return len;
}
namespace fp
{
template<char8_t start=u8'0',std::random_access_iterator Iter,my_unsigned_integral U>
requires (start==0||start==u8'0')
inline constexpr std::size_t output_unsigned(Iter str,U value)
{
	std::size_t const len{chars_len<10,true>(value)};
	output_base_number_impl<10,false,false,u8'.',start==0>(str+len,value);
	return len;
}
}

namespace with_length
{
template<char8_t start=u8'0',std::random_access_iterator Iter,my_unsigned_integral U>
requires (start==0||start==u8'0')
inline constexpr void output_unsigned(Iter str,U value,std::size_t const len)
{
	output_base_number_impl<10,false,false,u8'.',start==0>(str+len,value);
}
}



}
}

}
