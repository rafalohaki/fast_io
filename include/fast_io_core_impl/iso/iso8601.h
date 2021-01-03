#pragma once

namespace fast_io
{
//I just realize C++20 date time is purely nonsense and trash

struct iso8601_timestamp
{
	intiso_t year{};
	std::uint8_t month{};
	std::uint8_t day{};
	std::uint8_t hours{};
	std::uint8_t minutes{};
	std::uint8_t seconds{};
	uintiso_t subseconds{};
};

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,iso8601_timestamp>) noexcept
{
//ISO 8601 timestamp example : 2021-01-03T10:29:56Z
	return print_reserve_size(io_reserve_type<char_type,intiso_t>)+
	16+print_reserve_size(io_reserve_type<char_type,uintiso_t>);
}

namespace details
{
template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_iso8601_timestamp_impl(Iter iter,iso8601_timestamp timestamp) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=chrono_year_impl(iter,timestamp.year);
	if constexpr(std::same_as<char_type,char>)
		*iter='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'-';
	else
		*iter=u8'-';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.month);
	if constexpr(std::same_as<char_type,char>)
		*iter='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'-';
	else
		*iter=u8'-';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.day);
	if constexpr(std::same_as<char_type,char>)
		*iter='T';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'T';
	else
		*iter=u8'T';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.hours);
	if constexpr(std::same_as<char_type,char>)
		*iter=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L':';
	else
		*iter=u8':';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.minutes);
	if constexpr(std::same_as<char_type,char>)
		*iter=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L':';
	else
		*iter=u8':';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.seconds);
	if(timestamp.subseconds)
		iter=output_iso8601_subseconds(iter,timestamp.subseconds);
	if constexpr(std::same_as<char_type,char>)
		*iter='Z';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'Z';
	else
		*iter=u8'Z';
	++iter;
	return iter;
}
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso8601_timestamp>,Iter iter,iso8601_timestamp timestamp) noexcept
{
	return details::print_reserve_iso8601_timestamp_impl(iter,timestamp);
}

}