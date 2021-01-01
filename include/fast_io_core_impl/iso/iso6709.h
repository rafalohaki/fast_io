#pragma once

namespace fast_io
{
/*
out of range is contract violation. which is undefined behavior.
*/
struct iso6709_dddmmss
{
	std::int16_t degrees{};
	std::uint8_t minutes{};
	std::uint8_t seconds{};
};

template<typename T>
struct basic_iso6709_coordinates
{
	using coordinate_type = T;
	T latitude{};
	T longtitude{};
};

using iso6709_dddmmss_coordinates = basic_iso6709_coordinates<iso6709_dddmmss>;



namespace details
{
template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_iso6709_dddmmss_impl(Iter iter,iso6709_dddmmss dms) noexcept
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
	if(dms.minutes)[[likely]]
	{
		optimize_size::with_length::output_unsigned(iter,dms.minutes,2);
		iter+=2;
	}
	if(dms.seconds)[[likely]]
	{
		optimize_size::with_length::output_unsigned(iter,dms.seconds,2);
		iter+=2;
	}
#elif defined(FAST_IO_OPTIMIZE_TIME)
	iter=non_overlapped_copy_n(jiaendu::static_tables<char_type>::table3[absolute].data(),3,iter);
	if(dms.minutes)[[likely]]
		iter=non_overlapped_copy_n(jiaendu::static_tables<char_type>::table2[dms.minutes].data(),2,iter);
	if(dms.seconds)[[likely]]
		iter=non_overlapped_copy_n(jiaendu::static_tables<char_type>::table2[dms.seconds].data(),2,iter);
#else
	optimize_size::with_length::output_unsigned(iter,absolute,3);
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
	if(dms.minutes)[[likely]]
		iter=non_overlapped_copy_n(shared_static_base_table<char_type,10,false,false>::table[dms.minutes].data(),2,iter);
	if(dms.seconds)[[likely]]
		iter=non_overlapped_copy_n(shared_static_base_table<char_type,10,false,false>::table[dms.seconds].data(),2,iter);
#endif
	return iter;
}
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,iso6709_dddmmss>) noexcept
{
	return 8;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso6709_dddmmss>,Iter iter,iso6709_dddmmss dms) noexcept
{
	return details::print_reserve_define_iso6709_dddmmss_impl(iter,dms);
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,iso6709_dddmmss_coordinates>) noexcept
{
	return 16;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso6709_dddmmss_coordinates>,Iter iter,iso6709_dddmmss_coordinates dms) noexcept
{
	return details::print_reserve_define_iso6709_dddmmss_impl(details::print_reserve_define_iso6709_dddmmss_impl(iter,dms.latitude),dms.longtitude);
}

}