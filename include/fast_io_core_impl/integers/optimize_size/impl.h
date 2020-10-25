#pragma once

namespace fast_io::details::optimize_size
{

namespace with_length
{
template<char8_t base=10,bool uppercase=false,std::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr void output_unsigned(Iter str,U value,std::size_t const len)
{
	using char_type=std::iter_value_t<Iter>;
	str+=len-1;
	for(std::size_t i{};i!=len;++i)
	{
		U const temp(value/base);
		char_type const res(value%base);
		if constexpr(base<=10)
		{
			if constexpr(exec_charset_is_ebcdic<char_type>())
				*str=0xF0+res;
			else
				*str=u8'0'+res;
		}
		else
		{
			if constexpr(exec_charset_is_ebcdic<char_type>())
			{
				if(res<10)
					*str=0xF0+res;
				else
				{
					if constexpr(base<=19)
					{
						if constexpr(uppercase)
							*str=(0xC1-10)+res;
						else
							*str=(0x81-10)+res;
					}
					else if constexpr(base<=28)
					{
						if(res<19)
						{
							if constexpr(uppercase)
								*str=(0xC1-10)+res;
							else
								*str=(0x81-10)+res;
						}
						else
						{
							if constexpr(uppercase)
								*str=(0xD1-19)+res;
							else
								*str=(0x91-19)+res;
						}
					}
					else
					{
						if(res<19)
						{
							if constexpr(uppercase)
								*str=(0xC1-10)+res;
							else
								*str=(0x81-10)+res;
						}
						else if(res<28)
						{
							if constexpr(uppercase)
								*str=(0xD1-19)+res;
							else
								*str=(0x91-19)+res;
						}
						else
						{
							if constexpr(uppercase)
								*str=(0xE2-28)+res;
							else
								*str=(0xA2-28)+res;
						}
					}
				}
			}
			else
			{
				if(res<10)
					*str=u8'0'+res;
				else
				{
					if constexpr(uppercase)
						*str=(u8'A'-10)+res;
					else
						*str=(u8'a'-10)+res;
				}
			}
		}
		--str;
		value = temp;
	}
}
}



template<std::size_t len,char8_t base=10,bool uppercase=false,std::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr void output_unsigned_dummy(Iter str,U value)
{
	with_length::output_unsigned<base,uppercase>(str,value,len);
}

template<char8_t base=10,bool uppercase=false,std::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr std::size_t output_unsigned(Iter str,U value)
{
	std::size_t const len{chars_len<base>(value)};
	with_length::output_unsigned(str,value,len);
	return len;
}

}