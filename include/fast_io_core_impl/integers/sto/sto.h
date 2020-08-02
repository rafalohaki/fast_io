#pragma once

namespace fast_io
{
namespace details
{
template<char8_t base,my_unsigned_integral T>
inline constexpr void detect_overflow(T const t1,T const t2,std::size_t length)
{
	constexpr std::size_t max_size{cal_max_int_size<T,base>()};
	constexpr std::remove_cvref_t<T> mx_val(std::numeric_limits<std::remove_cvref_t<T>>::max()/static_cast<std::remove_cvref_t<T>>(base));
	if(max_size<=length)[[unlikely]]
	{
		if((max_size<length)|(t1<base)|(mx_val<t2))[[unlikely]]
			throw_input_overflow_error();
	}
}

template<char8_t base,my_unsigned_integral T>
inline constexpr void detect_signed_overflow(T const t1,T const t2,std::size_t length,bool sign)
{
	constexpr std::size_t max_size{cal_max_int_size<T,base>()};
	constexpr std::remove_cvref_t<T> mx_val(std::numeric_limits<std::remove_cvref_t<T>>::max()/static_cast<std::remove_cvref_t<T>>(base));
	if(max_size<=length)[[unlikely]]
	{
		if((max_size<length)|(t1<base)|(mx_val<t2)|(static_cast<T>(get_int_max_unsigned<T>()+sign)<t1))[[unlikely]]
			throw_input_overflow_error();
	}
}

template<my_integral T,char8_t base,bool no_dec=false,character_input_stream input>
inline constexpr T input_base_number(input& in)
{
	using unsigned_char_type = std::make_unsigned_t<typename input::char_type>;
	using unsigned_t = my_make_unsigned_t<std::remove_cvref_t<T>>;
	if constexpr(my_unsigned_integral<T>)
	{
		if constexpr(sizeof(unsigned_t)==16)
		{
			//special optimization for uint128_t
			std::size_t length{};
			auto ig{igenerator(in)};
			auto it{begin(ig)};
			auto ed{end(ig)};
			std::uint64_t t{};
			for(;it!=ed;++it)
			{
				unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
				if constexpr(base<=10)
				{
					if(base<=e)[[unlikely]]
						break;
					t*=base;
					t+=e;
				}
				else
				{
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				if(++length==19)[[unlikely]]
					break;
			}
			if(it!=ed&&length==19)	//phase 2
			{
				unsigned_t t2(t);
				unsigned_t t3{};
				for(++it;it!=ed;++it)
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
					if constexpr(base<=10)
					{
						if(base<=e)[[unlikely]]
							break;
						t3=t2;
						t2*=base;
						t2+=e;
					}
					else
					{
						constexpr char8_t bm10{base-10};
						if(e<=10)
						{
							t3=t2;
							t2*=base;
							t2+=e;
						}
						else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
						{
							t3=t2;
							t2*=base;
							t2+=e+10;
						}
						else[[unlikely]]
							break;
					}
					++length;
				}
				detect_overflow<base>(t2,t3,length);
				return t2;
			};
			if(!length)[[unlikely]]
				throw_malformed_input();
			return t;
		}
		else
		{
			unsigned_t t{},t1{};
			std::size_t length{};
			for(unsigned_char_type ch : igenerator(in))
			{
				if constexpr(base<=10)
				{
					unsigned_char_type const e(static_cast<unsigned_char_type>(ch)-u8'0');
					if(base<=e)[[unlikely]]
						break;
					t1=t;
					t*=base;
					t+=e;
				}
				else
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(ch)-u8'0');
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t1=t;
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t1=t;
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				++length;
			}

			if(!length)[[unlikely]]
				throw_malformed_input();
			detect_overflow<base>(t,t1,length);
			return t;
		}
	}
	else
	{
		
		std::size_t length{};
		auto ig{igenerator(in)};
		auto it{begin(ig)};
		auto ed{end(ig)};
		if(it==ed)
#ifdef __cpp_exceptions
			throw fast_io::eof();
#else
			fast_terminate();
#endif
		auto const sign{*it=='-'};
		if(sign)
			++it;
		if constexpr(sizeof(unsigned_t)==16)
		{
			std::uint64_t t{};
			for(;it!=ed;++it)
			{
				unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
				if constexpr(base<=10)
				{
					if(base<=e)[[unlikely]]
						break;
					t*=base;
					t+=e;
				}
				else
				{
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				if(++length==19)[[unlikely]]
					break;
			}
			if(it!=ed&&length==19)	//phase 2
			{
				unsigned_t t2(t);
				unsigned_t t3{};
				for(++it;it!=ed;++it)
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
					if constexpr(base<=10)
					{
						if(base<=e)[[unlikely]]
							break;
						t3=t2;
						t2*=base;
						t2+=e;
					}
					else
					{
						constexpr char8_t bm10{base-10};
						if(e<=10)
						{
							t3=t2;
							t2*=base;
							t2+=e;
						}
						else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
						{
							t3=t2;
							t2*=base;
							t2+=e+10;
						}
						else[[unlikely]]
							break;
					}
					++length;
				}
				detect_signed_overflow<base>(t2,t3,length,sign);
				if(sign)
					return -static_cast<T>(t2);
				else if(!length)[[unlikely]]
					throw_malformed_input();
				return static_cast<T>(t2);
			};
			if(sign)
				return -static_cast<T>(t);
			else if(!length)[[unlikely]]
				throw_malformed_input();
			return t;
		}
		else
		{
			unsigned_t t{},t1{};
			for(;it!=ed;++it)
			{
				if constexpr(base<=10)
				{
					unsigned_char_type const e(static_cast<unsigned_char_type>(*it)-u8'0');
					if(base<=e)[[unlikely]]
						break;
					t1=t;
					t*=base;
					t+=e;
				}
				else
				{
					unsigned_char_type e(static_cast<unsigned_char_type>(*it)-u8'0');
					constexpr char8_t bm10{base-10};
					if(e<=10)
					{
						t1=t;
						t*=base;
						t+=e;
					}
					else if(static_cast<unsigned_char_type>(e-=17)<bm10||static_cast<unsigned_char_type>(e-=32)<bm10)
					{
						t1=t;
						t*=base;
						t+=e+10;
					}
					else[[unlikely]]
						break;
				}
				++length;
			}
			detect_signed_overflow<base>(t,t1,length,sign);
			if(sign)
				return -static_cast<T>(t);
			else if(!length)[[unlikely]]
				throw_malformed_input();
			return static_cast<T>(t);
		}
	}
}
}

template<char8_t base,bool uppercase,character_input_stream input,details::my_integral T>
inline constexpr void space_scan_define(input& in,manip::base_t<base,uppercase,T> v)
{
	v.reference=details::input_base_number<std::remove_cvref_t<T>,base>(in);
}

template<character_input_stream input,details::my_integral T>
inline constexpr void space_scan_define(input& in,T& a)
{
	a=details::input_base_number<std::remove_cvref_t<T>,10>(in);
}

template<char8_t base,bool uppercase,character_input_stream input,typename T>
requires std::same_as<std::byte,std::remove_cvref_t<T>>
inline constexpr void space_scan_define(input& in,manip::base_t<base,uppercase,T> v)
{
	v.reference=static_cast<std::byte>(details::input_base_number<char8_t,base>(in));
}

template<character_input_stream input>
inline constexpr void space_scan_define(input& in,std::byte& a)
{
	a=std::byte(details::input_base_number<char8_t,10>(in));
}

/*
template<char8_t base,bool uppercase,character_input_stream input,typename T>
requires std::same_as<std::byte,std::remove_cvref_t<T>>
inline constexpr void scan_define(input& in,manip::no_decoration<manip::base_t<base,uppercase,T>> v)
{
	v.reference.reference=static_cast<std::byte>(details::input_base_number<char8_t,base>(in));
}

template<character_input_stream input,std::integral T>
inline constexpr void scan_define(input& in,manip::no_decoration<T> v)
{
	v.reference=details::input_base_number<std::remove_cvref_t<T>,10>(in);
}

template<char8_t base,bool uppercase,character_input_stream input,std::integral T>
inline constexpr void scan_define(input& in,manip::no_decoration<manip::base_t<base,uppercase,T>> v)
{
	v.reference.reference=details::input_base_number<std::remove_cvref_t<T>,base>(in);
}
*/
}
