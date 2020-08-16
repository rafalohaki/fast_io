#pragma once

namespace fast_io
{

template<std::integral ch_type>
class basic_omemory_map
{
public:
	using char_type = ch_type;
	char_type *begin_ptr{},*curr_ptr{},*end_ptr{};
	constexpr basic_omemory_map() = default;
	basic_omemory_map(native_memory_map_io_observer iob,std::size_t offset=0):begin_ptr(reinterpret_cast<char_type*>(iob.address_begin+offset)),curr_ptr(this->begin_ptr),end_ptr(this->begin_ptr+iob.bytes()/sizeof(char_type)){}

	constexpr std::size_t written_bytes() const noexcept
	{
		return static_cast<std::size_t>(curr_ptr-begin_ptr)*sizeof(char_type);
	}
};

namespace details
{

template<std::integral char_type>
inline constexpr void write(basic_omemory_map<char_type>& bomp,char_type const* begin,char_type const* end) noexcept
{
	std::size_t const to_write(end-begin);
	non_overlapped_copy_n(begin,to_write,bomp.curr_ptr);
	bomp.curr_ptr+=to_write;
}
}

template<std::integral char_type,std::contiguous_iterator Iter>
constexpr void write(basic_omemory_map<char_type>& bomp,Iter begin,Iter end) noexcept
{
	details::write(bomp,std::to_address(begin),std::to_address(end));
}

template<std::integral char_type>
constexpr char_type* obuffer_begin(basic_omemory_map<char_type>& bomp) noexcept
{
	return bomp.begin_ptr;
}
template<std::integral char_type>
constexpr char_type* obuffer_curr(basic_omemory_map<char_type>& bomp) noexcept
{
	return bomp.curr_ptr;
}
template<std::integral char_type>
constexpr char_type* obuffer_end(basic_omemory_map<char_type>& bomp) noexcept
{
	return bomp.end_ptr;
}

template<std::integral char_type>
constexpr void overflow(basic_omemory_map<char_type>& bomp,char_type) noexcept
{
//ub for overflow
}

template<std::integral char_type>
constexpr void obuffer_set_curr(basic_omemory_map<char_type>& bomp,char_type* ptr) noexcept
{
	bomp.curr_ptr=ptr;
}

using omemory_map = basic_omemory_map<char>;


}
