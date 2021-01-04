#pragma once

namespace fast_io
{

/*
out of range is contract violation. which is undefined behavior.
*/

struct iso6709_coordinate
{
	std::int16_t degrees{};
	std::uint8_t minutes{};
	std::uint8_t seconds{};
	uintiso_t subseconds{};
};

struct iso6709_coordinates
{
	iso6709_coordinate latitude{};
	iso6709_coordinate longtitude{};
};


namespace details
{

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_iso6709_coordinate_impl(Iter iter,iso6709_coordinate dms) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	std::uint16_t absolute{static_cast<std::uint16_t>(dms.degrees)};
	bool const negative(dms.degrees<0);
	if(negative)
	{
		absolute = 0 - absolute;
		if constexpr(std::same_as<char_type,char>)
			*iter='-';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L'-';
		else
			*iter=u8'-';
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			*iter='+';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L'+';
		else
			*iter=u8'+';
	}
	++iter;
#ifdef FAST_IO_OPTIMIZE_SIZE
	optimize_size::with_length::output_unsigned(iter,absolute,3);
	iter+=3;
	if(dms.subseconds||dms.seconds||dms.minutes)[[likely]]
	{
		optimize_size::with_length::output_unsigned(iter,dms.minutes,2);
		iter+=2;
	}
	if(dms.subseconds||dms.seconds)[[likely]]
	{
		optimize_size::with_length::output_unsigned(iter,dms.seconds,2);
		iter+=2;
	}

#elif defined(FAST_IO_OPTIMIZE_TIME)
	iter=non_overlapped_copy_n(jiaendu::static_tables<char_type>::table3[absolute].data(),3,iter);
	if(dms.minutes||dms.seconds||dms.subseconds)[[likely]]
		iter=non_overlapped_copy_n(jiaendu::static_tables<char_type>::table2[dms.minutes].data(),2,iter);
	if(dms.seconds||dms.subseconds)[[likely]]
		iter=non_overlapped_copy_n(jiaendu::static_tables<char_type>::table2[dms.seconds].data(),2,iter);
#else
	if(absolute>=100)
	{
		if constexpr(std::same_as<char_type,char>)
			*iter='1';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L'1';
		else
			*iter=u8'1';
		absolute-=100;
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			*iter='0';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L'0';
		else
			*iter=u8'0';
	}
	++iter;
	iter=non_overlapped_copy_n(shared_static_base_table<char_type,10,false,false>::table[absolute].data(),2,iter);
	if(dms.subseconds||dms.seconds||dms.minutes)[[likely]]
		iter=non_overlapped_copy_n(shared_static_base_table<char_type,10,false,false>::table[dms.minutes].data(),2,iter);
	if(dms.subseconds||dms.seconds)[[likely]]
		iter=non_overlapped_copy_n(shared_static_base_table<char_type,10,false,false>::table[dms.seconds].data(),2,iter);
#endif
	if(dms.subseconds)
		iter=output_iso8601_subseconds(iter,dms.subseconds);
	return iter;
}
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,iso6709_coordinate>) noexcept
{
	return 8+print_reserve_size(io_reserve_type<char_type,uintiso_t>);
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso6709_coordinate>,Iter iter,iso6709_coordinate dms) noexcept
{
	return details::print_reserve_define_iso6709_coordinate_impl(iter,dms);
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,iso6709_coordinates>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,iso6709_coordinate>)*2;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso6709_coordinates>,Iter iter,iso6709_coordinates dms) noexcept
{
	return details::print_reserve_define_iso6709_coordinate_impl(details::print_reserve_define_iso6709_coordinate_impl(iter,dms.latitude),dms.longtitude);
}

template<std::random_access_iterator Iter>
inline constexpr Iter scan_skip_define(scan_skip_type_t<parameter<iso6709_coordinates&>>, Iter beg, Iter ed) noexcept
{
	return scan_skip_space(beg, ed);
}
#if 0
namespace details::ctx_scan_iso6709
{

template<bool contiguous_only, std::random_access_iterator Iter>
struct voldmort
{
	Iter iter;
	std::errc code{};
	std::conditional_t<>
	inline constexpr voldmort(Iter begin, Iter end, iso6709_coordinates& t) noexcept
	{

	}
};

}

template<bool contiguous_only, std::random_access_iterator Iter>
inline constexpr auto scan_context_define(scan_context_t<contiguous_only>, Iter begin, Iter end, parameter<iso6709_coordinates&> t) noexcept
{
	return details::ctx_scan_iso6709::voldmort<contiguous_only,Iter,iso6709_coordinates>(begin,end,t);
}
#endif
}