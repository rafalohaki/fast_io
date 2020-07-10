#pragma once

namespace fast_io
{
namespace details
{
struct is_none_space
{
template<std::integral T>
inline constexpr bool operator()(T ch) const
{
	std::make_unsigned_t<T> e(ch);
	return (0x4<(e-0x9))&(e!=0x20);
}
};

}
template<character_input_stream input,typename UnaryPredicate>
[[nodiscard]] inline constexpr bool skip_while(input& in,UnaryPredicate&& pred)
{
	if constexpr(buffer_input_stream<input>)
	{
		for(;;)
		{
			auto b{ibuffer_curr(in)};
			auto e{ibuffer_end(in)};
			for(;b!=e&&pred(*b);++b);
			ibuffer_set_curr(in,b);
			if(b==e)[[unlikely]]
			{
				if constexpr(!contiguous_buffer_input_stream<input>)
				if(underflow(in))[[likely]]
					continue;
				return false;
			}
			return true;
		}
	}
	else
	{
		auto ig{igenerator(in)};
		auto b{begin(ig)};
		auto e{end(ig)};
		for(;b!=e&&pred(*b);++b);
		return b!=e;
	}
}
template<character_input_stream input,typename UnaryPredicate>
[[nodiscard]] inline constexpr bool skip_until(input& in,UnaryPredicate&& pred)
{
	if constexpr(buffer_input_stream<input>)
	{
		for(;;)
		{
			auto b{ibuffer_curr(in)};
			auto e{ibuffer_end(in)};
			for(;b!=e&&!pred(*b);++b);
			ibuffer_set_curr(in,b);
			if(b==e)[[unlikely]]
			{
				if constexpr(!contiguous_buffer_input_stream<input>)
				if(underflow(in))[[likely]]
					continue;
				return false;
			}
			return true;
		}
	}
	else
	{
		auto ig{igenerator(in)};
		auto b{begin(ig)};
		auto e{end(ig)};
		for(;b!=e&&!pred(*b);++b);
		return b!=e;
	}
}

template<character_input_stream input>
inline constexpr std::size_t discard(input& in)
{
/*	if constexpr(contiguous_input_stream<input>)
	{
		if(iempty(in))[[unlikely]]
			return 0;
		iremove_prefix(in,1);
		return 1;
	}
	else
	{*/
		auto gen(igenerator(in));
		auto b(begin(gen));
		auto e(end(gen));
		if(b==e)[[unlikely]]
			return 0;
		++b;
		return 1;
//	}
}


template<character_input_stream input>
inline constexpr std::size_t discard(input& in,std::size_t n)
{
/*
	if constexpr(contiguous_input_stream<input>)
	{
		auto sz{isize(in)};
		if(sz<n)[[unlikely]]
			n=sz;
		iremove_prefix(in,n);
		return n;
	}
	else
*/
	if constexpr(buffer_input_stream<input>)
	{
		std::size_t discarded{};
		for(;;)
		{
			auto b{ibuffer_curr(in)};
			auto e{ibuffer_end(in)};
			if(e-b<n)
			{
				discarded+=e-b;
				if constexpr(!contiguous_buffer_input_stream<input>)
				if(!underflow(in))[[unlikely]]
					return discarded;
			}
			b+=n;
			discarded+=n;
			set_ibuffer_curr(in,b);
		}
		return discarded;
	}
	else
	{
		auto gen(igenerator(in));
		auto b(begin(gen));
		auto e(end(gen));
		std::size_t discarded{};
		for(;b!=e&&discarded!=n;++b)
			++n;
		return discarded;
	}
}

template<character_input_stream input>
[[nodiscard]] inline constexpr bool skip_space(input& in)
{
	return skip_until(in,details::is_none_space{});
}

template<std::size_t sign=false,std::uint8_t base=0xA,character_input_stream input>
[[nodiscard]] inline constexpr bool skip_none_numerical(input& in)
{
	return skip_until(in,details::is_numerical<sign,base>{});
}

template<character_input_stream input>
[[nodiscard]] inline constexpr std::size_t skip_line(input& in)
{
	if constexpr(buffer_input_stream<input>)
	{
		std::size_t skipped{};
		for(;;)
		{
			auto b{ibuffer_curr(in)};
			auto e{ibuffer_end(in)};
			for(;b!=e&&*b!=u8'\n';++b)
				++skipped;
			if(b!=e)
			{
				ibuffer_set_curr(in,b+1);
				return skipped;
			}
			else
			{
				if(!underflow(in))[[unlikely]]
					return skipped;
			}
		}
	}
	else
	{
		auto ig{igenerator(in)};
		auto b{begin(ig)};
		auto e{end(ig)};
		std::size_t n{};
		for(;b!=e&&*b!=u8'\n';++b)
			++n;
		if(b!=e)[[likely]]
			++b;
		return n;
	}
}

template<character_input_stream input,typename T,typename Func>
requires space_scanable<input,T&>
[[nodiscard]] inline constexpr bool scan_define(input& in,manip::space<T&,Func&> t)
{
	if(!skip_while(in,t.function))
		return false;
	space_scan_define(in,t.reference);
	return true;
}
}