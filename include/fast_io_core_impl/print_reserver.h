#pragma once

namespace fast_io
{

//I wasn't thought to add this stupid stuff since it is useless. Unfortunately, fmt library author keeps using bullshit benchmarks to claim his library is superior to everyone else. While he is horribl wrong.
/*
use a stupid loop to ruin jiaendu algorithm for example. While all real world benchmarks show jiaendu is fastest algorithm we have.
*/


template<typename T,reserve_printable<T>  char_type=char>
class print_reserver
{
public:
	std::array<char_type,print_reserve_size(io_reserve_type<char_type,T>)+1> mutable buffer;
	std::size_t position;
	constexpr print_reserver(T const& t):position(print_reserve_define(io_reserve_type<char_type,T>,buffer.data(),t)-buffer.data()){}
	constexpr std::size_t size() const noexcept
	{
		return position;
	}
constexpr char_type* data() noexcept
{
    return buffer.data();
}
constexpr char_type const* data() const noexcept
{
    return buffer.data();
}
constexpr char_type const* c_str() const noexcept
{
    buffer[position]=0;
    return buffer.data();
}
inline static constexpr std::size_t reserve_size() noexcept
{
    constexpr std::size_t val{print_reserve_size(io_reserve_type<char_type,T>)+1};
    return val;
}
constexpr std::basic_string_view<char_type> strvw() const noexcept
{
    return {buffer.data(),position};
}
};

template<typename T,reserve_printable<T> char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,print_reserver<T,char_type>>)
{
	return print_reserver<T,char_type>::reserve_size();
}

template<typename T,reserve_printable<T> char_type,std::contiguous_iterator Iter>
requires (std::same_as<char_type,std::iter_value_t<Iter>>||(std::same_as<std::iter_value_t<Iter>,char>&&std::same_as<char_type,char8_t>))
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,print_reserver<T,char_type>>,Iter beg,T&& ref)
{
#ifdef __cpp_lib_is_constant_evaluated
	if(std::is_constant_evaluated())
		my_copy_n(beg,ref.size(),ref.data());
	else
	{
#endif
		memcpy(std::to_address(beg),ref.data(),ref.size()*sizeof(char_type));
#ifdef __cpp_lib_is_constant_evaluated
	}
#endif
	return beg+ref.size();
}

template<typename T,reserve_printable<T>  char_type=char>
class reverse_print_reserver
{
public:
	std::array<char_type,print_reserve_size(io_reserve_type<T,char_type>)+1> mutable buffer;
	std::size_t position;
	constexpr reverse_print_reserver(T const& t):position(reverse_print_reserve_define(io_reserve_type<T,char_type>,buffer.size()-1+buffer.data(),t)-buffer.data()){}
	constexpr std::size_t size() const noexcept
	{
		return buffer.size()-1-position;
	}
	constexpr char_type* data() noexcept
	{
		return buffer.data()+position;
	}
	constexpr char_type const* data() const noexcept
	{
		return buffer.data()+position;
	}
	constexpr char_type const* c_str() const noexcept
	{
		buffer.back()=0;
		return buffer.data()+position;
	}
	inline static constexpr std::size_t reserve_size() noexcept
	{
		constexpr std::size_t val{print_reserve_size(io_reserve_type<T,char_type>)+1};
		return val;
	}
	constexpr std::basic_string_view<char_type> strvw() const noexcept
	{
		return {buffer.data()+position,size()};
	}
};


template<typename T,reserve_printable<T> char_type=char>
inline constexpr print_reserver<T,char_type> print_reserve(T const& t)
{
	return {t};
}
#if 0
template<std::integral char_type=char,reverse_reserve_printable type>
inline constexpr reverse_print_reserver<type,char_type> reverse_print_reserve(type const& t)
{
	return {t};
}


template<reserve_printable type,std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,reverse_print_reserver<type,char_type>>)
{
	return reverse_print_reserver<type,char_type>::reserve_size();
}

template<reserve_printable type,std::integral char_type,std::contiguous_iterator Iter,typename T>
requires (std::same_as<char_type,std::iter_value_t<Iter>>||(std::same_as<std::iter_value_t<Iter>,char>&&std::same_as<char_type,char8_t>))
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,reverse_print_reserver<type,char_type>>,Iter beg,T&& ref)
{
#ifdef __cpp_lib_is_constant_evaluated
	if(std::is_constant_evaluated())
		my_copy_n(beg,ref.size(),ref.data());
	else
	{
#endif
		memcpy(std::to_address(beg),ref.data(),ref.size()*sizeof(char_type));
#ifdef __cpp_lib_is_constant_evaluated
	}
#endif
	return beg+ref.size();
}
#endif
}
