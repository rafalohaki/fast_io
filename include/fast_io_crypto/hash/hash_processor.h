#pragma once

namespace fast_io
{

namespace details
{

class compress_current_position
{};

template<typename Func>
concept hash_require_block_init=requires(Func& func,std::byte const* ptr)
{
	func.block_init(ptr);
};

}

template<std::integral ch_type,typename Func>
class basic_hash_processor
{
public:
	using char_type = ch_type;
	using function_type = Func;
	function_type& function;
	inline static constexpr std::size_t block_size = function_type::block_size;
#if __has_cpp_attribute(no_unique_address)
	[[no_unique_address]]
#endif
	::fast_io::freestanding::array<std::byte,block_size> temporary_buffer{};
#if __has_cpp_attribute(no_unique_address)
	[[no_unique_address]]
#endif
	std::conditional_t<block_size==0,details::compress_current_position,std::size_t> current_position{};
	constexpr basic_hash_processor(function_type& func) noexcept:function(func)
	{
		if constexpr(details::hash_require_block_init<function_type>)
			current_position+=func.block_init(temporary_buffer.data());
	}
	constexpr void do_final() noexcept
	{
		if constexpr(block_size!=0)
			function.digest(temporary_buffer.data(),current_position);
		else
			function.digest();
	}
	constexpr basic_hash_processor(basic_hash_processor const&) noexcept=default;
	constexpr basic_hash_processor& operator=(basic_hash_processor const&) noexcept=default;
	constexpr ~basic_hash_processor()
	{
		secure_clear(temporary_buffer.data(),block_size);
		secure_clear(std::addressof(current_position),sizeof(current_position));
	}
};

namespace details::hash_processor
{

template<std::integral ch_type,typename Func>
#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline constexpr void write_cold_path_impl(basic_hash_processor<ch_type,Func>& out,ch_type const* begin,ch_type const* end) noexcept
{
	if(out.current_position)
	{
		std::size_t to_copy{Func::block_size-out.current_position};
		my_memcpy(out.temporary_buffer.data()+out.current_position,begin,to_copy);
		out.function(out.temporary_buffer.data(),out.temporary_buffer.size());
		begin+=to_copy;
		out.current_position={};
	}
	std::size_t const total_bytes((end-begin)*sizeof(*begin));
	std::size_t const blocks(total_bytes/Func::block_size);
	std::size_t const blocks_bytes(blocks*Func::block_size);
	out.function(reinterpret_cast<std::byte const*>(begin),blocks_bytes);
	std::size_t const to_copy(total_bytes-blocks_bytes);
	if(to_copy)
		my_memcpy(out.temporary_buffer.data(),reinterpret_cast<std::byte const*>(end)-to_copy,to_copy);
	out.current_position=to_copy;
}

template<std::integral ch_type,typename Func,::fast_io::freestanding::contiguous_iterator Iter>
requires (std::same_as<::fast_io::freestanding::iter_value_t<Iter>,ch_type>||std::same_as<ch_type,char>)
inline constexpr void write_cold_path(basic_hash_processor<ch_type,Func>& out,Iter begin,Iter end)
{
	if constexpr(std::same_as<::fast_io::freestanding::iter_value_t<Iter>,ch_type>)
		write_cold_path_impl(out,::fast_io::freestanding::to_address(begin),::fast_io::freestanding::to_address(end));
	else
	{
		write_cold_path_impl(out,
			reinterpret_cast<char const*>(::fast_io::freestanding::to_address(begin)),
			reinterpret_cast<char const*>(::fast_io::freestanding::to_address(end)));
	}
}

}

template<std::integral ch_type,typename Func,::fast_io::freestanding::contiguous_iterator Iter>
requires (std::same_as<::fast_io::freestanding::iter_value_t<Iter>,ch_type>||std::same_as<ch_type,char>)
inline void write(basic_hash_processor<ch_type,Func>& out,Iter begin,Iter end)
{
	if(std::same_as<::fast_io::freestanding::iter_value_t<Iter>,char>)
	{
		if constexpr(Func::block_size==0)
		{
			out.function(::fast_io::freestanding::to_address(begin),::fast_io::freestanding::to_address(end)-::fast_io::freestanding::to_address(begin));
		}
		else
		{
			std::size_t const bytes(end-begin);
			std::size_t to_copy{Func::block_size-out.current_position};
			if(bytes<to_copy)[[likely]]
			{
				if(bytes)
					::fast_io::details::my_memcpy(out.temporary_buffer.data()+out.current_position,::fast_io::freestanding::to_address(begin),bytes);
				out.current_position+=bytes;
				return;
			}
			details::hash_processor::write_cold_path(out,begin,end);
		}
	}
	else
		write(out,reinterpret_cast<char const*>(::fast_io::freestanding::to_address(begin)),reinterpret_cast<char const*>(::fast_io::freestanding::to_address(end)));
}

template<std::integral ch_type,typename Func>
inline void scatter_write(basic_hash_processor<ch_type,Func>& out,io_scatters_t sp)
{
	for(std::size_t i{};i!=sp.len;++i)
		write(out,reinterpret_cast<char const*>(sp.base[i].base),reinterpret_cast<char const*>(sp.base[i].base)+sp.base[i].len);
}

template<typename Func>
class hash_processor:public basic_hash_processor<char,Func>
{
public:
	using typename basic_hash_processor<char,Func>::char_type;
	using typename basic_hash_processor<char,Func>::function_type;
	using basic_hash_processor<char,Func>::block_size;
	constexpr hash_processor(Func& func) noexcept:basic_hash_processor<char,Func>(func){}
};

template<typename Func>
hash_processor(Func& func)->hash_processor<Func>;

}
