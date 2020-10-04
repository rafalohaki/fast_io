#include"../include/fast_io.h"

namespace fast_io
{

enum class scan_context_status_code:char8_t
{
success=0,need_more_data=1,lexical_error=2,syntax_error=3
};

template<bool contiguous_only=false>
struct scan_context_t
{
explicit inline constexpr scan_context_t() noexcept=default;
};

template<bool contiguous_only=false>
inline constexpr scan_context_t<contiguous_only> scan_context;

template<typename char_type,typename T,bool contiguous_only=false>
concept context_scannable = requires(char_type const* begin,char_type const* end,T& t)
{
	{scan_context_define(scan_context<char_type,contiguous_only>,begin,end,t).iter}->std::same_as<char_type const*>;
	{scan_context_define(scan_context<char_type,contiguous_only>,begin,end,t).code}->std::convertible_to<scan_context_status_code>;
};

template<typename T>
struct scan_skip_type_t
{
explicit inline constexpr scan_skip_type_t() noexcept=default;
};
template<typename T>
inline constexpr scan_skip_type_t<T> scan_skip_type{};

template<typename T>
concept scannable_skipping = requires(scan_skip_type_t<T> t,char8_t const* begin,char8_t const* end)
{
	{scan_skip_define(scan_skip_type<T>,begin,end)}->std::convertible_to<char8_t const*>;
};

namespace details
{
template<std::random_access_iterator Iter>
inline constexpr Iter scan_skip_space(Iter begin,Iter end) noexcept
{
	for(;begin!=end&&fast_io::details::is_space(*begin);++begin);
	return begin;
}
}

template<details::my_integral T,std::random_access_iterator Iter>
inline constexpr Iter scan_skip_define(scan_skip_type_t<T>,Iter beg,Iter ed) noexcept
{
	return details::scan_skip_space(beg,ed);
}

namespace details::ctx_scan_integer
{

enum class phase_code:char8_t
{
empty=0,
sign=1,
no_a_digit=2,
zero=3
};


/*
phase 0 begin===end
phase 1 sign

*/

template<std::random_access_iterator Iter>
inline constexpr Iter skip0(Iter begin,Iter end)
{
	for(;begin!=end&&*begin==u8'0';++begin);
	return begin;
}

template<char8_t base=10,std::random_access_iterator Iter>
[[nodiscard]]
inline constexpr std::pair<Iter,bool> phase0(Iter begin,Iter end) noexcept
{
	using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
	if constexpr(base <= 10)
	{
		unsigned_char_type ch(static_cast<unsigned_char_type>(*begin)-static_cast<unsigned_char_type>(u8'0'));
		if(static_cast<unsigned_char_type>(base)<=ch)[[unlikely]]
			return {begin,true};
	}
	else
	{
		constexpr unsigned_char_type mns{base-10};
		unsigned_char_type const ch(static_cast<unsigned_char_type>(*begin));
		unsigned_char_type ch1(ch-static_cast<unsigned_char_type>(u8'0'));
		unsigned_char_type ch2(ch-static_cast<unsigned_char_type>(u8'A'));
		unsigned_char_type ch3(ch-static_cast<unsigned_char_type>(u8'a'));
		if(ch2<mns)
			ch1=ch2+static_cast<unsigned_char_type>(10);
		else if(ch3<mns)
			ch1=ch3+static_cast<unsigned_char_type>(10);
		else if(static_cast<unsigned_char_type>(9)<ch1)[[unlikely]]
			return {begin,true};
	}
	return {skip0(begin,end),false};
}

template<char8_t base=10,std::random_access_iterator Iter,my_unsigned_integral U>
[[nodiscard]]
inline constexpr Iter phase1(Iter i,Iter end,U& val,U& val_last) noexcept
{
	using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
	for(;i!=end;++i)
	{
		if constexpr(base <= 10)
		{
			unsigned_char_type ch(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'0'));
			if(static_cast<unsigned_char_type>(base)<=ch)[[unlikely]]
				break;
			val=(val_last=val)*static_cast<unsigned_char_type>(base)+ch;
		}
		else
		{
			constexpr unsigned_char_type mns{base-10};
			unsigned_char_type const ch(static_cast<unsigned_char_type>(*i));
			unsigned_char_type ch1(ch-static_cast<unsigned_char_type>(u8'0'));
			unsigned_char_type ch2(ch-static_cast<unsigned_char_type>(u8'A'));
			unsigned_char_type ch3(ch-static_cast<unsigned_char_type>(u8'a'));
			if(ch2<mns)
				ch1=ch2+static_cast<unsigned_char_type>(10);
			else if(ch3<mns)
				ch1=ch3+static_cast<unsigned_char_type>(10);
			else if(static_cast<unsigned_char_type>(9)<ch1)[[unlikely]]
				break;
			val=(val_last=val)*static_cast<unsigned_char_type>(base)+ch1;
		}
	}
	return i;
}

template<char8_t base,bool contiguous_only,std::random_access_iterator Iter,details::my_integral T>
struct voldmort
{
Iter iter;
scan_context_status_code code;
[[no_unique_address]] std::conditional_t<contiguous_only,details::empty,T*> pointer;
[[no_unique_address]] std::conditional_t<contiguous_only,details::empty,std::size_t> length{};
[[no_unique_address]] std::conditional_t<contiguous_only,details::empty,details::my_make_unsigned_t<T>> val{},val_last{};
[[no_unique_address]] std::conditional_t<contiguous_only,details::empty,phase_code> p0_phase{};
[[no_unique_address]] std::conditional_t<details::my_signed_integral<T>,bool,empty> minus{};

inline constexpr void finalize()
{
	if constexpr(details::my_unsigned_integral<T>)
	{
		if(detect_overflow<base>(length,val,val_last))[[unlikely]]
		{
			code=scan_context_status_code::syntax_error;
			return;
		}
		*pointer=val;
	}
	else
	{
		if(detect_signed_overflow<base>(length,val,val_last,minus))[[unlikely]]
		{
			code=scan_context_status_code::syntax_error;
			return;
		}
		if(minus)
			*pointer= 0 - val;
		else
			*pointer= val;
	}
	code=scan_context_status_code::success;
}


inline constexpr void operator()(Iter begin,Iter end) noexcept
{
	if(begin==end)		//EOF
	{
		if(p0_phase!=phase_code::zero)
		{
			code=scan_context_status_code::lexical_error;
			return;
		}
		finalize();
		return;
	}
	if(p0_phase!=phase_code::zero)
	{
		if constexpr(my_signed_integral<T>)
		{
			if(p0_phase==phase_code::empty)
			{
				if((*begin==u8'-')|(*begin==u8'+'))
				{
					minus=(*begin==u8'-');
					++begin;
				}
			}
		}
		auto p0_ret{phase0<base>(begin,end)};
		begin=p0_ret.first;
		p0_phase=p0_ret.second;
		if(p0_phase==phase_code::no_a_digit)
			code=scan_context_status_code::lexical_error;
		else
			code=scan_context_status_code::need_more_data;
		return;
	}
	iter=phase1<base>(begin,end,val,val_last);
	length+=iter-begin;
	if(iter==end)
	{
		code=scan_context_status_code::need_more_data;
		return;
	}
	finalize();
}
inline constexpr voldmort(Iter begin,Iter end,T& t) noexcept
{
	using namespace details::ctx_scan_integer;
	if(begin==end)
	{
		iter=begin;
		if constexpr(contiguous_only)
			code=scan_context_status_code::lexical_error;
		else
			code=scan_context_status_code::need_more_data;
		return;
	}
	if constexpr(my_signed_integral<T>)
	{
		if((*begin==u8'-')|(*begin==u8'+'))
		{
			minus=(*begin==u8'-');
			++begin;
		}
	}
	if constexpr(contiguous_only)
	{
		auto p0_ret{phase0<base>(begin,end)};
		iter=p0_ret.first;
		if(p0_ret.second!=phase_code::zero)
		{
			code=scan_context_status_code::lexical_error;
			return;
		}
		details::my_make_unsigned_t<T> val{},val_last{};
		iter=phase1<base>(iter,end,val,val_last);
		if constexpr(details::my_unsigned_integral<T>)
		{
			if(detect_overflow<base>(static_cast<std::size_t>(iter-begin),val,val_last))[[unlikely]]
			{
				code=scan_context_status_code::syntax_error;
				return;
			}
			t = val;
		}
		else
		{
			if(detect_signed_overflow<base>(static_cast<std::size_t>(iter-begin),val,val_last,minus))[[unlikely]]
			{
				code=scan_context_status_code::syntax_error;
				return;
			}
			if(minus)
				t = 0 - val;
			else
				t = val;
		}
	}
	else
	{
		auto p0_ret{phase0<base>(begin,end)};
		iter=p0_ret.first;
		p0_phase=p0_ret.second;
		if(p0_phase!=phase_code::zero)
		{
			if(p0_phase==phase_code::no_a_digit)
				code=scan_context_status_code::lexical_error;
			else
				code=scan_context_status_code::need_more_data;
			return;
		}
		auto bg{iter};
		iter=phase1<base>(bg,end,val,val_last);
		if(iter==end)
		{
			length=bg-iter;
			code=scan_context_status_code::need_more_data;
			return;
		}
		if constexpr(details::my_unsigned_integral<T>)
		{
			if(detect_overflow<base>(static_cast<std::size_t>(iter-begin),val,val_last))[[unlikely]]
			{
				code=scan_context_status_code::syntax_error;
				return;
			}
			t=val;
		}
		else
		{
			if(detect_signed_overflow<base>(static_cast<std::size_t>(iter-begin),val,val_last,minus))[[unlikely]]
			{
				code=scan_context_status_code::syntax_error;
				return;
			}
			if(minus)
				t = 0 - val;
			else
				t = val;
		}
	}
}
};

}
template<bool contiguous_only,std::random_access_iterator Iter,details::my_integral T>
inline constexpr auto scan_context_define(scan_context_t<contiguous_only>,Iter begin,Iter end,T& t) noexcept
{
return details::ctx_scan_integer::voldmort<10,contiguous_only,Iter,T>(begin,end,t);
}
}