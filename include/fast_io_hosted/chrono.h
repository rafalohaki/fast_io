#pragma once
#include<chrono>

namespace fast_io
{
#ifdef __MSDOS__
struct timespec
{
std::time_t tv_sec;
long tv_nsec;
};
#endif
template<std::integral char_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,struct timespec>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::time_t>)+1+print_reserve_size(io_reserve_type<char_type,long>);
}

template<std::integral char_type,std::random_access_iterator Iter>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,struct timespec>,Iter it,struct timespec spc) noexcept
{
	it=print_reserve_define(io_reserve_type<char_type,std::time_t>,it,spc.tv_sec);
	unsigned long nsec{static_cast<unsigned long>(spc.tv_nsec)};
	if(nsec==0UL||999999999UL<nsec)
		return it;
	*it=u8'.';
	++it;
	std::size_t sz{};
	for(;nsec%10==0;++sz)
		nsec/=10;
	sz=9-sz;
	auto res{it+sz};
	it=res;
	--it;
	for(std::size_t i{};i!=sz;++i)
	{
		unsigned long const temp(nsec/10);
		*it=static_cast<char8_t>(static_cast<char8_t>(nsec%10)+u8'0');
		--it;
		nsec=temp;
	}
	return res;
}

//We use seconds since seconds is the standard unit of SI
//Use my own tweaked ryu algorithm for counting seconds
template<std::integral char_type,typename Rep,typename Period>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,double>)+1;
}

template<std::integral char_type,std::random_access_iterator Iter,typename Rep,typename Period>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>,Iter it,std::chrono::duration<Rep,Period> duration) noexcept
{
	*(it=print_reserve_define(io_reserve_type<char_type,double>,it,std::chrono::duration_cast<std::chrono::duration<double>>(duration).count()))=u8's';
	return ++it;
}



#if __cpp_lib_chrono >= 201907L || __GNUC__>= 11


#endif

}