#pragma once

namespace fast_io
{

namespace details
{

class compress_current_position
{};
}

template<std::integral ch_type,typename Func>
class basic_hash_processor
{
public:
	using char_type = ch_type;
	using function_type = Func;
	function_type& function;
	inline static constexpr std::size_t block_size = function_type::block_size;
	[[no_unique_address]] std::array<std::byte,block_size> temporary_buffer{};
	[[no_unique_address]] std::conditional_t<block_size==0,details::compress_current_position,std::size_t> current_position{};
	constexpr basic_hash_processor(function_type& func) noexcept:function(func)
	{
		if constexpr(requires(Func& func)
		{
			func.block_init(temporary_buffer);
		})
			current_position+=func.block_init(temporary_buffer);
	}
	constexpr void do_final() noexcept
	{
		if constexpr(block_size!=0)
			function.digest(std::as_bytes(std::span<std::byte const>{temporary_buffer.data(),current_position}));
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

template<std::integral ch_type,typename Func,std::contiguous_iterator Iter>
requires (std::same_as<std::iter_value_t<Iter>,ch_type>||std::same_as<ch_type,char>)
inline constexpr void write_cold_path(basic_hash_processor<ch_type,Func>& out,Iter begin,Iter end)
{
	if(out.current_position)
	{
		std::size_t to_copy{Func::block_size-out.current_position};
		memcpy(out.temporary_buffer.data()+out.current_position,std::to_address(begin),to_copy);
		out.function(std::span<std::byte const,Func::block_size>{out.temporary_buffer});
		begin+=to_copy;
		out.current_position={};
	}
	std::size_t const total_bytes((end-begin)*sizeof(*begin));
	std::size_t const blocks(total_bytes/Func::block_size);
	std::size_t const blocks_bytes(blocks*Func::block_size);
	out.function(std::span<std::byte const>{reinterpret_cast<std::byte const*>(std::to_address(begin)),blocks_bytes});	
	std::size_t const to_copy(total_bytes-blocks_bytes);
	memcpy(out.temporary_buffer.data(),reinterpret_cast<std::byte const*>(std::to_address(end))-to_copy,to_copy);
	out.current_position=to_copy;
}

}

template<std::integral ch_type,typename Func,std::contiguous_iterator Iter>
requires (std::same_as<std::iter_value_t<Iter>,ch_type>||std::same_as<ch_type,char>)
inline void write(basic_hash_processor<ch_type,Func>& out,Iter begin,Iter end)
{
	if(std::same_as<std::iter_value_t<Iter>,char>)
	{
		if constexpr(Func::block_size==0)
		{
			out.function(std::as_bytes(std::span{std::to_address(begin),std::to_address(end)}));
		}
		else
		{
		std::size_t const bytes(end-begin);
		std::size_t to_copy{Func::block_size-out.current_position};
		if(bytes<to_copy)[[likely]]
		{
			memcpy(out.temporary_buffer.data()+out.current_position,std::to_address(begin),bytes);
			out.current_position+=bytes;
			return;
		}
		details::hash_processor::write_cold_path(out,begin,end);
		}
	}
	else
		write(out,reinterpret_cast<char const*>(std::to_address(begin)),reinterpret_cast<char const*>(std::to_address(end)));
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