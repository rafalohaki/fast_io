#pragma once

namespace fast_io::details
{
template<char8_t start=u8'0',std::random_access_iterator Iter>
inline constexpr void append_nine_digits_dummy(Iter str,std::uint32_t value)
{
	using char_type = std::iter_value_t<Iter>;
	str+=9;
	for(std::size_t i{};i!=9;++i)
	{
		std::uint32_t const temp(value/10);
		char_type const res(value%10);
		*--str=start+res;
		value = temp;
	}
}

template<char8_t start=u8'0',std::random_access_iterator Iter>
inline constexpr void append_nine_digits(Iter str,std::uint32_t value)
{
#ifndef FAST_IO_OPTIMIZE_SIZE
	if(std::is_constant_evaluated())
#endif
		append_nine_digits_dummy<start>(str,value);
#ifndef FAST_IO_OPTIMIZE_SIZE

	else
	{
		using char_type = std::iter_value_t<Iter>;
#ifdef FAST_IO_OPTIMIZE_TIME

	#if (_WIN64 || __x86_64__ || __ppc64__)
		std::uint64_t remains0{(static_cast<std::uint64_t>(value) *
		static_cast<std::uint64_t>(3518437209)) >> 45};
		std::uint64_t remains1{static_cast<std::uint64_t>(value) * 
		static_cast<std::uint64_t>((2882303762)) >> 58};
	#else
		std::uint32_t remains0{value/10000};
		std::uint32_t remains1{value/100000000};
	#endif
		auto v2(remains1);
		remains1 = remains0 - remains1*10000;
		remains0 = value - remains0*10000;
		*str = static_cast<char8_t>(v2)+start;
		my_copy_n(jiaendu::static_tables<char_type,start>::table4[remains1].data(),4,++str);
		my_copy_n(jiaendu::static_tables<char_type,start>::table4[remains0].data(),4,str += 4);
#else
		constexpr auto &table(get_shared_inline_constexpr_base_table<char_type,10,false,start==0>());
		constexpr std::uint32_t pw(static_cast<std::uint32_t>(table.size()));
		constexpr std::size_t chars(table.front().size());
		str+=9;
		for(std::uint32_t i{};i!=4;++i)
		{
			auto const rem(value%pw);
			value/=pw;
			my_copy_n(table[rem].data(),chars,str-=chars);
		}
		*--str=value+start;
#endif
	}
#endif
}
}