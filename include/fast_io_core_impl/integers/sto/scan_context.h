#pragma once

namespace fast_io
{

namespace details
{
	template<char8_t base, my_unsigned_integral T>
	inline constexpr bool detect_overflow_v2(T const t1, T const t2, std::size_t length)
	{
		constexpr std::size_t max_size{ cal_max_int_size<T,base>() };
		constexpr std::remove_cvref_t<T> mx_val(std::numeric_limits<std::remove_cvref_t<T>>::max() / static_cast<std::remove_cvref_t<T>>(base));
		if (max_size <= length) [[unlikely]]
		{
			if ((max_size < length) | (t1 < base) | (mx_val < t2)) [[unlikely]]
				return true;
		}
		return false;
	}

	template<char8_t base, my_unsigned_integral T>
	inline constexpr bool detect_signed_overflow_v2(T const t1, T const t2, std::size_t length, bool sign)
	{
		constexpr std::size_t max_size{ cal_max_int_size<T,base>() };
		constexpr std::remove_cvref_t<T> mx_val(std::numeric_limits<std::remove_cvref_t<T>>::max() / static_cast<std::remove_cvref_t<T>>(base));
		if (max_size <= length) [[unlikely]]
		{
			if ((max_size < length) | (t1 < base) | (mx_val < t2) | (static_cast<T>(get_int_max_unsigned<T>() + sign) < t1)) [[unlikely]]
				return true;
		}
		return false;
	}
	template<std::random_access_iterator Iter>
	inline constexpr Iter scan_skip_space(Iter begin, Iter end) noexcept
	{
		for (; begin != end && fast_io::details::is_space(*begin); ++begin);
		return begin;
	}
}

template<details::my_integral T, std::random_access_iterator Iter>
inline constexpr Iter scan_skip_define(scan_skip_type_t<parameter<T&>>, Iter beg, Iter ed) noexcept
{
	return details::scan_skip_space(beg, ed);
}

namespace details::ctx_scan_integer
{

	enum class phase_code :char8_t
	{
		empty = 0,
		zero = 1,
		numbers = 2
	};


	/*
	phase 0 begin===end
	phase 1 sign

	*/

	template<std::random_access_iterator Iter>
	inline constexpr Iter skip0(Iter begin, Iter end)
	{
		for (; begin != end && *begin == u8'0'; ++begin);
		return begin;
	}

	template<char8_t base = 10, std::random_access_iterator Iter>
	[[nodiscard]]
	inline constexpr std::pair<Iter, bool> phase0(Iter begin, Iter end) noexcept
	{
		using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
		if constexpr (base <= 10)
		{
			unsigned_char_type ch(static_cast<unsigned_char_type>(*begin) - static_cast<unsigned_char_type>(u8'0'));
			if (static_cast<unsigned_char_type>(base) <= ch) [[unlikely]]
				return { begin,true };
		}
		else
		{
			constexpr unsigned_char_type mns{ base - 10 };
			unsigned_char_type const ch(static_cast<unsigned_char_type>(*begin));
			unsigned_char_type ch1(ch - static_cast<unsigned_char_type>(u8'0'));
			unsigned_char_type ch2(ch - static_cast<unsigned_char_type>(u8'A'));
			unsigned_char_type ch3(ch - static_cast<unsigned_char_type>(u8'a'));
			if (ch2 < mns)
				ch1 = ch2 + static_cast<unsigned_char_type>(10);
			else if (ch3 < mns)
				ch1 = ch3 + static_cast<unsigned_char_type>(10);
			else if (static_cast<unsigned_char_type>(9) < ch1) [[unlikely]]
				return { begin,true };
		}
		return { skip0(begin,end),false };
	}

	template<char8_t base = 10, std::random_access_iterator Iter, my_unsigned_integral U>
	[[nodiscard]]
	inline constexpr Iter phase1(Iter i, Iter end, U& val, U& val_last) noexcept
	{
		using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
		for (; i != end; ++i)
		{
			if constexpr (base <= 10)
			{
				unsigned_char_type ch(static_cast<unsigned_char_type>(*i) - static_cast<unsigned_char_type>(u8'0'));
				if (static_cast<unsigned_char_type>(base) <= ch) [[unlikely]]
					break;
				val = (val_last = val) * static_cast<unsigned_char_type>(base) + ch;
			}
			else
			{
				constexpr unsigned_char_type mns{ base - 10 };
				unsigned_char_type const ch(static_cast<unsigned_char_type>(*i));
				unsigned_char_type ch1(ch - static_cast<unsigned_char_type>(u8'0'));
				unsigned_char_type ch2(ch - static_cast<unsigned_char_type>(u8'A'));
				unsigned_char_type ch3(ch - static_cast<unsigned_char_type>(u8'a'));
				if (ch2 < mns)
					ch1 = ch2 + static_cast<unsigned_char_type>(10);
				else if (ch3 < mns)
					ch1 = ch3 + static_cast<unsigned_char_type>(10);
				else if (static_cast<unsigned_char_type>(9) < ch1) [[unlikely]]
					break;
				val = (val_last = val) * static_cast<unsigned_char_type>(base) + ch1;
			}
		}
		return i;
	}

	template<char8_t base, bool contiguous_only, std::random_access_iterator Iter, details::my_integral T>
	struct voldmort
	{
		Iter iter;
		scan_context_status_code code;
		[[no_unique_address]] std::conditional_t<contiguous_only, details::empty, T*> pointer;
		[[no_unique_address]] std::conditional_t<contiguous_only, details::empty, std::size_t> length{};
		[[no_unique_address]] std::conditional_t<contiguous_only, details::empty, details::my_make_unsigned_t<T>> val{}, val_last{};
		[[no_unique_address]] std::conditional_t<contiguous_only, details::empty, phase_code> p0_phase{};
		[[no_unique_address]] std::conditional_t<details::my_signed_integral<T>, bool, empty> minus{};

		inline constexpr void finalize()
		{
			if constexpr (details::my_unsigned_integral<T>)
			{
				if (detect_overflow_v2<base>(val, val_last, length)) [[unlikely]]
				{
					code = scan_context_status_code::syntax_error;
					return;
				}
				*pointer = val;
			}
			else
			{
				if (detect_signed_overflow_v2<base>(val, val_last, length, minus)) [[unlikely]]
				{
					code = scan_context_status_code::syntax_error;
					return;
				}
					if (minus)
						*pointer = 0 - val;
					else
						*pointer = val;
			}
			code = scan_context_status_code::success;
		}


		inline constexpr void operator()(Iter begin, Iter end) noexcept
		{
			switch (p0_phase)
			{
			case phase_code::empty:
			{
				if (begin == end)
				{
					iter = begin;
					code = scan_context_status_code::need_more_data;
					return;
				}
				if constexpr (my_signed_integral<T>)
				{
					if ((*begin == u8'-') | (*begin == u8'+'))
					{
						minus = (*begin == u8'-');
						++begin;
					}
				}
				auto p0_ret{ phase0<base>(begin,end) };
				iter = p0_ret.first;
				auto iter_skip0{ iter };
				if (p0_ret.second)
				{
					code = scan_context_status_code::lexical_error;
					return;
				}
				if (iter == end)
				{
					// last number is still zero
					p0_phase = phase_code::zero;
				}
				else {
					// non-zeros from iter to end
					p0_phase = phase_code::numbers;
					iter = phase1<base>(iter, end, val, val_last);
					length += iter - begin;
					if (length > 0 && iter != end) {
						code = scan_context_status_code::success;
						finalize();
						break;
					}
				}
				code = scan_context_status_code::need_more_data;
			}
			break;
			case phase_code::zero:
			{
				auto p0_ret{ phase0<base>(begin,end) };
				iter = p0_ret.first;
				auto iter_skip0{ iter };
				if (p0_ret.second)
				{
					code = scan_context_status_code::lexical_error;
					return;
				}
				if (iter == end)
				{
					// last number is still zero
					p0_phase = phase_code::zero;
				}
				else {
					// non-zeros from iter to end
					p0_phase = phase_code::numbers;
					iter = phase1<base>(iter, end, val, val_last);
					length += iter - begin;
					if (length > 0 && iter != end) {
						code = scan_context_status_code::success;
						finalize();
						break;
					}
				}
				code = scan_context_status_code::need_more_data;
			}
				break;
			case phase_code::numbers:
			{
				iter = phase1<base>(begin, end, val, val_last);
				length += iter - begin;
				if (iter == end) {
					code = scan_context_status_code::need_more_data;
				}
				else {
					finalize();
				}
			}
			break;
			}
		}

		inline constexpr voldmort(Iter begin, Iter end, T& t) noexcept
		{
			using namespace details::ctx_scan_integer;
			code = scan_context_status_code::need_more_data;
			pointer = std::addressof(t);
			length = 0;
			if (begin == end)
			{
				iter = begin;
				if constexpr (contiguous_only)
					code = scan_context_status_code::lexical_error;
				else
					code = scan_context_status_code::need_more_data;
				return;
			}
			if constexpr (my_signed_integral<T>)
			{
				if ((*begin == u8'-') | (*begin == u8'+'))
				{
					minus = (*begin == u8'-');
					++begin;
				}
			}
			if constexpr (contiguous_only)
			{
				auto p0_ret{ phase0<base>(begin,end) };
				iter = p0_ret.first;
				auto iter_skip0{ iter };
				if (p0_ret.second)
				{
					code = scan_context_status_code::lexical_error;
					return;
				}
				details::my_make_unsigned_t<T> val{}, val_last{};
				iter = phase1<base>(iter, end, val, val_last);
				if constexpr (details::my_unsigned_integral<T>)
				{
					if (detect_overflow_v2<base>(val, val_last, static_cast<std::size_t>(iter - iter_skip0))) [[unlikely]]
					{
						code = scan_context_status_code::syntax_error;
						return;
					}
					t = val;
				}
				else
				{
					if (detect_signed_overflow_v2<base>(val, val_last, static_cast<std::size_t>(iter - iter_skip0), minus)) [[unlikely]]
					{
						code = scan_context_status_code::syntax_error;
						return;
					}
						if (minus)
							t = 0 - val;
						else
							t = val;
				}
			}
			else
			{
				operator()(begin, end);
			}
		}
	};

}
template<bool contiguous_only, std::random_access_iterator Iter, details::my_integral T>
inline constexpr auto scan_context_define(scan_context_t<contiguous_only>, Iter begin, Iter end, parameter<T&> t) noexcept
{
	return details::ctx_scan_integer::voldmort<10, contiguous_only, Iter, T>(begin, end, t.reference);
}
}

