#pragma once

namespace fast_io
{

namespace details
{

template<char8_t base,bool uppercase,::fast_io::freestanding::contiguous_iterator Iter>
inline constexpr void rep_single_unit(char8_t b,Iter ptr)
{
	if constexpr(base==16)
	{
		constexpr char8_t offset=(uppercase?u8'A':u8'a')-static_cast<char8_t>(10);
		char8_t high=b>>static_cast<char8_t>(4);
		char8_t low=b&static_cast<char8_t>(15);
		if(9<high)
		{
			if constexpr(uppercase)
			high+=offset;
		}
		else
			high+=u8'0';
		if(9<low)
			low+=offset;
		else
			low+=u8'0';
		*ptr=high;
		ptr[1]=low;
	}
	else
	{
		ptr+=7;
		for(std::size_t i{};i!=8;++i)
		{
			*ptr=static_cast<char8_t>(b&1)+u8'0';
			b>>=1;
			--ptr;
		}
	}
}

}

template<std::integral char_type,typename T,char8_t base,bool uppercase=false,bool space=true>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manip::representation<T,base,uppercase,space>>)
{
	if constexpr(sizeof(T)==0)
		return 0;
	constexpr std::size_t result=static_cast<std::size_t>(base==2?8:2)*sizeof(T) + static_cast<std::size_t>(space)*(sizeof(T)-1);
	return result;
}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator caiter,typename T,char8_t base,bool uppercase,bool space,typename U>
inline caiter print_reserve_define(io_reserve_type_t<char_type,manip::representation<T,base,uppercase,space>>,caiter iter,U i)
{
	if constexpr(sizeof(T)!=0)
	{
		auto pointer{reinterpret_cast<std::byte*>(std::addressof(i.reference))};
		auto pointer_end{pointer+sizeof(T)};
		if constexpr(space)
		{
			details::rep_single_unit<base,uppercase>(std::to_integer<char8_t>(*pointer),iter);
			++pointer;
			if constexpr(base==2)
				iter+=8;
			else
				iter+=2;
		}
		for(;pointer!=pointer_end;++pointer)
		{
			if constexpr(space)
			{
				*iter=u8' ';
				++iter;
			}
			details::rep_single_unit<base,uppercase>(std::to_integer<char8_t>(*pointer),iter);
			if constexpr(base==2)
				iter+=8;
			else
				iter+=2;
		}
	}
	return iter;
}

}