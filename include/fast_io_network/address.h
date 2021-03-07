#pragma once
#include<variant>

namespace fast_io
{

struct ipv4
{
	std::array<unsigned char, 4> storage{};
};

template<std::integral char_type,buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<char_type,ipv4>,output& out,input& in)
{
	using namespace fast_io::scan_transmitter;
	return scan_transmit(out,in,until_none_digit<10>,single_dot,until_none_digit<10>,single_dot,until_none_digit<10>,single_dot,until_none_digit<10>);
}

namespace details
{

template<character_input_stream input>	
inline constexpr void ipv4_scan_sep(input& in)	
{
	auto ig{igenerator(in)};
	auto bg{begin(ig)},ed{end(ig)};
	if(bg==ed||*bg!=u8'.')
		throw_posix_error(EIO);
	++bg;
}

enum class v6_sep_result
{
space=0,column=1,digit=2,
};

template<character_input_stream input>	
inline constexpr v6_sep_result ipv6_scan_sep(input& in)	
{
	auto ig{igenerator(in)};
	auto bg{begin(ig)},ed{end(ig)};
	if(bg==ed||*bg!=u8':')
		return v6_sep_result::space;
	++bg;
	if(bg!=ed&&*bg==u8':')
	{
		++bg;
		return v6_sep_result::column;
	}
	if(bg==ed)
		return v6_sep_result::space;
	else
	{
		using unsigned_char_type = std::make_unsigned_t<typename input::char_type>;
		unsigned_char_type ch(*bg);
		if(ch==u8':')
			throw_malformed_input();
		else
		{
			unsigned_char_type ch1(ch-static_cast<unsigned_char_type>(u8'0'));
			unsigned_char_type ch2(ch-static_cast<unsigned_char_type>(u8'A'));
			unsigned_char_type ch3(ch-static_cast<unsigned_char_type>(u8'a'));
			if(ch1<10||ch2<6||ch3<6)
				return v6_sep_result::digit;
			else
				return v6_sep_result::space;
		}
		return v6_sep_result::digit;
	}
}

}

template<std::integral char_type,character_input_stream input>
inline constexpr void space_scan_define(input& in,ipv4& v4)
{
	space_scan_define(in,v4.storage[0]);
	details::ipv4_scan_sep(in);
	space_scan_define(in,v4.storage[1]);
	details::ipv4_scan_sep(in);
	space_scan_define(in,v4.storage[2]);
	details::ipv4_scan_sep(in);
	space_scan_define(in,v4.storage[3]);
}

template<std::integral char_type,bool end_test,std::contiguous_iterator Iter>
inline constexpr auto space_scan_reserve_define(io_reserve_type_t<char_type,ipv4,end_test>,Iter begin,Iter end,ipv4& t)
{
	using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
	for(std::size_t part{};part!=4;++part)
	{
		char8_t val{},val_last{};
		if(begin!=end&&(static_cast<unsigned_char_type>(9)<static_cast<unsigned_char_type>(static_cast<unsigned_char_type>(*begin)-static_cast<unsigned_char_type>(u8'0'))))
			throw_malformed_input();	
		auto start{std::find_if(begin,end,[](auto ch)
		{
			return ch!=u8'0';
		})};
		auto last{begin};
		for(begin=start;begin!=end;++begin)
		{
			unsigned_char_type const ch(static_cast<unsigned_char_type>(*begin)-u8'0');
			if(static_cast<unsigned_char_type>(10)<=ch)[[unlikely]]
			{
				bool const is_part3{part==3};
				bool const is_dot{static_cast<unsigned_char_type>(*begin)==static_cast<unsigned_char_type>(u8'.')};
				if(is_part3==is_dot)[[unlikely]]
					throw_malformed_input();
				if(!is_part3)[[unlikely]]
					++begin;
				break;
			}
			val=((val_last=val)*static_cast<char8_t>(10))+static_cast<char8_t>(ch);
		}
		if(last==begin)
		{
			if constexpr(end_test)
			{
				if(begin==end)
					return end;
				else
					throw_malformed_input();

			}
			else
			{
				throw_malformed_input();
			}
		}
		constexpr char8_t mxdv10(std::numeric_limits<char8_t>::max()/10);
		details::detect_overflow<10>(val,val_last,((begin-start)-(part!=3)));
		t.storage[part]=val;
	}
	return begin;
}

inline constexpr std::size_t native_socket_address_size(ipv4 const&)
{
	return sizeof(sockaddr_in);
}

inline constexpr auto family(ipv4 const&)
{
	return sock::family::ipv4;
}


struct socket_address_storage
{
	sockaddr sock;
	std::array<std::byte,sizeof(sockaddr_storage)<sizeof(sockaddr)?0x0:sizeof(sockaddr_storage)-sizeof(sockaddr)> storage;
};


//use std::memcpy is THE only way to do correct ip address punning
inline auto to_socket_address_storage(ipv4 const& add,std::uint16_t port)
{
	sockaddr_in v4st{};
	v4st.sin_family=static_cast<sock::details::address_family>(fast_io::sock::family::ipv4);
	v4st.sin_port=details::big_endian(port);
	::fast_io::details::my_memcpy(std::addressof(v4st.sin_addr),add.storage.data(),sizeof(add.storage));
	socket_address_storage stor{};
	::fast_io::details::my_memcpy(std::addressof(stor),std::addressof(v4st),sizeof(sockaddr_in));
	return stor;
}
template<std::integral char_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,ipv4>)
{
	return 15;
}

template<std::integral char_type,std::random_access_iterator Iter>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,ipv4>,Iter it,ipv4 const& v4)
{
	*(it=print_reserve_define(io_reserve_type<char_type,char unsigned>,it,v4.storage.front()))=u8'.';
	*(it=print_reserve_define(io_reserve_type<char_type,char unsigned>,++it,v4.storage[1]))=u8'.';
	*(it=print_reserve_define(io_reserve_type<char_type,char unsigned>,++it,v4.storage[2]))=u8'.';
	return print_reserve_define(io_reserve_type<char_type,char unsigned>,++it,v4.storage[3]);
}

struct ipv6
{
	std::array<std::uint16_t, 8> storage{};
};

inline constexpr std::size_t native_socket_address_size(ipv6 const&)
{
	return sizeof(sockaddr_in6);
}

inline constexpr auto family(ipv6 const&)
{
	return sock::family::ipv6;
}
/*

inline constexpr auto valid_ipv6(st_single([](std::integral auto ch)
{
	using namespace fast_io::scan_transmitter;
	return ch==u8':'||until_none_digit<16>(ch);
}));

template<buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<ipv6>,output& out,input& in)
{
	using namespace fast_io::scan_transmitter;
	return scan_transmit(out,in,valid_ipv6);
}
*/

// // from https://gist.github.com/njh/84125c8ededdeb74ec5cc80a4003f308
// template<character_input_stream input>
// inline constexpr void space_scan_define(input& in,ipv6& v6)
// {
// 	auto asciiToHex([](auto c)
// 	{
// 		c |= 0x20;

// 		if (c >= '0' && c <= '9') {
// 			return c - '0';
// 		} else if (c >= 'a' && c <= 'f') {
// 			return (c - 'a') + 10;
// 		} else {
// 			return -1;
// 		}
// 	});
// 	std::array<std::uint8_t, 16> tmp;
// 	std::uint8_t colon_count(0);
// 	std::uint8_t lastchar(0);
// 	std::uint16_t accumulator = 0;
// 	std::uint8_t pos = 0;
// 	auto ig{igenerator(in)};
// 	auto cur{begin(ig)}, ed{end(ig)};
// 	for(uint8_t i(1); i < 40; ++i) {
// 		if(cur==ed)
// 			break;
// 		auto curchar(*cur);
//         if (curchar == u8':') {
//             if (lastchar == u8':') {
//                 // Double colon!
//                 colon_count = 14;
//             } else if (colon_count) {
//                 // Count backwards the number of colons after the ::
//                 colon_count -= 2;
//             }
//         }
// 		cur++;
// 		lastchar = curchar;
//     }

// 	lastchar=0;
// 	ig=igenerator(in);
// 	cur=begin(ig);
// 	for(uint8_t i(0); i < 40 && pos < 16; ++i,cur++) {
// 		if(cur==ed||*cur==u8':')
// 		{
// 			tmp[pos] = accumulator>>8;
// 			tmp[pos+1] = accumulator;
// 			accumulator = 0;
// 			if (colon_count && i && lastchar == u8':') {
//                 pos = colon_count;
//             } else {
//                 pos += 2;
//             }
// 		} else {
//             int8_t val = asciiToHex(*cur);
// 			if(val==-1)
// 				throw_malformed_input();
//             accumulator <<= 4;
//             accumulator |= val;
//         }
// 		if(cur==ed)
// 		break;
// 		lastchar=*cur;
// 	}
// 	::fast_io::details::my_memcpy(v6.storage.data(), tmp.data(), 16);
// }

template<character_input_stream input>
inline constexpr void space_scan_define(input& in,ipv6& v6)
{
	std::size_t double_npos{std::numeric_limits<std::size_t>::max()};
	for(std::size_t i{};i!=v6.storage.size();++i)
	{
		v6.storage[i]=details::input_base_number<std::uint16_t,16>(in);
		if(i==7)[[unlikely]]
			break;
		details::v6_sep_result s{details::ipv6_scan_sep(in)};
		if(s==details::v6_sep_result::column)
		{
			if(double_npos!=std::numeric_limits<std::size_t>::max())
				throw_malformed_input();
			double_npos=i;
		}
		else if(s==details::v6_sep_result::space)
		{
			if(double_npos==std::numeric_limits<std::size_t>::max())
				throw_malformed_input();
			break;
		}
	}
	++double_npos;
	std::size_t copy_mid{10-double_npos};
	if(double_npos!=std::numeric_limits<std::size_t>::max())
		std::fill(v6.storage.begin()+double_npos,std::copy_backward(v6.storage.begin()+double_npos,v6.storage.begin()+copy_mid,v6.storage.end()),0);
}

inline auto to_socket_address_storage(ipv6 add,std::uint16_t port)
{
	sockaddr_in6 v6st{};
	v6st.sin6_family=static_cast<sock::details::address_family>(fast_io::sock::family::ipv6);
	static_assert(sizeof(v6st.sin6_port)==sizeof(port));
	v6st.sin6_port=details::big_endian(port);
	if constexpr(std::endian::little==std::endian::native)
		for(auto& e : add.storage)
			e=details::byte_swap(e);
	std::::fast_io::details::my_memcpy(std::addressof(v6st.sin6_addr),add.storage.data(),sizeof(add.storage));
	socket_address_storage stor{};
	std::::fast_io::details::my_memcpy(std::addressof(stor),std::addressof(v6st),sizeof(sockaddr_in6));
	return stor;
}
template<std::integral char_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,ipv6>)
{
	return 39;
}

namespace details
{

template<bool uppercase=false,char8_t base=16,std::random_access_iterator Iter>
constexpr Iter print_ipv6(Iter it,ipv6 const& v6)
{
	std::size_t max_zero_start{std::numeric_limits<std::size_t>::max()},max_zero_len{};
	std::size_t zero_start{std::numeric_limits<std::size_t>::max()},zero_len{};
	for(std::size_t i{};i!=v6.storage.size();++i)
	{
		if(v6.storage[i])
		{
			if(max_zero_len<zero_len)
			{
				max_zero_len=zero_len;
				max_zero_start=zero_start;
				zero_len=0;
				zero_start=std::numeric_limits<std::size_t>::max();
			}
		}
		else
		{
			++zero_len;
			if(!i||v6.storage[i-1])
				zero_start=i;
		}
	}
	if(max_zero_len<zero_len)
	{
		max_zero_start=zero_start;
		max_zero_len=zero_len;
	}
	if(max_zero_len<2)
	{
		for(std::size_t i{};i!=v6.storage.size();++i)
		{
			if(i)
			{
				*it=u8':';
				++it;
			}
			it=details::process_integer_output<base,uppercase>(it,v6.storage[i]);
		}
	}
	else
	{
		std::size_t i{};
		for(;i!=max_zero_start;++i)
		{
			if(i)
			{
				*it=u8':';
				++it;
			}
			it=details::process_integer_output<base,uppercase>(it,v6.storage[i]);
		}
		non_overlapped_copy_n(u8"::",2,it);
		i+=max_zero_len;
		it+=2;
		if(i!=8)
		{
			it=details::process_integer_output<base,uppercase>(it,v6.storage[i]);
			for(++i;i!=8;++i)
			{
				*it=u8':';
				it=details::process_integer_output<base,uppercase>(++it,v6.storage[i]);
			}
		}
	}
	return it;
}

}

template<std::integral char_type,std::random_access_iterator Iter>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,ipv6>,Iter it,ipv6 const& v6)
{
	return details::print_ipv6(it,v6);
}


class address
{
public:
	using variant_type = std::variant<ipv4, ipv6>;
	variant_type var;
	template<typename... Args>
	requires std::constructible_from<variant_type, Args...>
	explicit constexpr address(Args &&... args) :var(std::forward<Args>(args)...){}

	constexpr auto& variant() { return var; }
	constexpr auto& variant() const { return var; }
};

template<std::integral U>
inline constexpr auto to_socket_address_storage(address const& v,U port)
{
	return std::visit([port](auto const &arg) {
		return to_socket_address_storage(arg,port);
	}, v.variant());
}

inline constexpr std::size_t native_socket_address_size(address const& v)
{
	return std::visit([](auto const &arg) {
		return native_socket_address_size(arg);
	}, v.variant());
}


inline constexpr auto family(address const& v)
{
	return std::visit([](auto const &arg) {
		return family(arg);
	}, v.variant());
}
template<std::integral char_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,address>)
{
	return 39;
}


template<std::integral char_type,std::random_access_iterator Iter>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,address>,Iter it,address const& v)
{
	return std::visit([&](auto&& arg) {
		return print_reserve_define(io_reserve_type<char_type,std::decay_t<decltype(arg)>>,it,arg);
	}, v.variant());
}

struct endpoint
{
	socket_address_storage storage={};
	socklen_t storage_size=sizeof(socket_address_storage);
};

struct ip_endpoint:endpoint
{
	template<typename T>
	requires requires(T t)
	{
		{family(t)}->std::convertible_to<sock::family>;
	}
	constexpr ip_endpoint(T const& addr,std::uint16_t port):endpoint{to_socket_address_storage(addr,port)}{}
	constexpr ip_endpoint(std::uint16_t port):ip_endpoint(ipv4{},port){}
	constexpr ip_endpoint():ip_endpoint(ipv4{},0){}
};

}