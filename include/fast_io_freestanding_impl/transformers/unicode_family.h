#pragma once



namespace fast_io
{

namespace transforms
{

struct codecvt_base
{
	virtual ~codecvt_base() = default;
	virtual std::pair<char32_t *, char const *> do_convert(char32_t *dst, char const *begin, char const *end) const = 0;
	virtual std::size_t required_size(std::size_t length) const noexcept = 0;
	virtual codecvt_base* clone() const = 0;
};

template<typename T>
struct codecvt_derived: codecvt_base
{
	T t;
	std::pair<char32_t *, char const *> do_convert(char32_t *dst, char const *begin, char const *end) const
	{
		return t(dst, begin, end);
	}
	std::size_t required_size(std::size_t length) const noexcept override
	{
		return t.required_size(length);
	}
	codecvt_derived* clone() const override
	{
		if constexpr(std::copyable<T>)
			return new codecvt_derived<T>(*this);
		else
			throw std::runtime_error("type not copyable");			
	}
};



struct utf8_char32
{
	std::pair<char32_t *, char const *> operator()(char32_t *dst, char const *begin, char const *end) const
	{
		auto ret{fast_io::details::utf::utf_code_convert_details<true>(begin,end,dst)};
		return {ret, begin};
	}
	std::size_t required_size(std::size_t length) const noexcept
	{
		return length * 4;
	}
};

template<bool native_endian>
struct utf16_char32
{
	std::pair<char32_t *, char const *> operator()(char32_t *dst, char const *begin, char const *end) const
	{
		using char16_t_a = char16_t
#ifndef _MSC_VER
		__attribute__((__may_alias__))
#endif
;
		auto begin16{reinterpret_cast<char16_t_a const *>(begin)};
		auto begin16_tmp{begin16};
		std::size_t length(static_cast<std::size_t>(end-begin)/2);
		auto end16{begin16 + length};
		auto ret{fast_io::details::utf::utf_code_convert_details<true>(begin16_tmp,end16,dst)};
		if constexpr (!native_endian)
			for (auto i{dst}; i != ret; ++i)
					*i=fast_io::details::byte_swap(*i);
		return {ret, begin + (begin16_tmp - begin16)};
	}
	std::size_t required_size(std::size_t length) const noexcept
	{
		return length * 4;
	}
};

template<bool native_endian>
struct utf32_char32
{
	std::pair<char32_t *, char const *> operator()(char32_t *dst, char const *begin, char const *end) const
	{
		std::size_t length_div4{(static_cast<std::size_t>(end-begin)/4)};
		std::size_t length{length_div4*4};
		if(length)
			std::memcpy(dst,begin,length);
		if constexpr (!native_endian)
			for (std::size_t i{}; i != length_div4; ++i)
					dst[i]=fast_io::details::byte_swap(dst[i]);
		return {dst + length_div4, begin + length};
	}
	std::size_t required_size(std::size_t length) const noexcept
	{
		return length * 4;
	}
};

// template<bool native_endian>
// struct gb18030_char32
// {
// 	constexpr std::pair<char32_t *, char const *> operator()(char32_t *dst, char const *begin, char const *end) const
// 	{
// 		// TODO: ..
// 		throw std::
// 		if constexpr (!native_endian)
// 			for (std::size_t i{}; i != length_div4; ++i)
// 					dst[i]=fast_io::details::byte_swap(dst[i]);
// 		return {dst + length_div4, begin + length};
// 	}
// 	constexpr std::size_t required_size(std::size_t length) const noexcept override
// 	{
// 		return length * 4;
// 	}
// };

struct unicode_family
{
	std::unique_ptr<codecvt_base> cvt_ptr{};
	template<output_stream output,std::contiguous_iterator Iter>
	requires std::same_as<std::iter_value_t<Iter>, char> && std::same_as<typename std::remove_cvref_t<output>::char_type, char32_t>
	constexpr auto operator()(output& out,Iter begin,Iter end)
	{
		if(!cvt_ptr) {
			std::size_t len(end-begin);
			if(3<len) {
				std::array<unsigned char, 4> bom{};
				std::memcpy(bom.data(), std::to_address(begin), 4);
				// from http://www.0x08.org/posts/UTF8-BOM
				if (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF) {
					// utf8 bom
					cvt_ptr.reset(new codecvt_derived<utf8_char32>);
					begin += 3;
				} else if (bom[0] == 0xFE && bom[1] == 0xFF && bom[2] != 0 && bom[3] != 0) {
					// utf16 be
					if constexpr (std::endian::native == std::endian::big)
						cvt_ptr.reset(new codecvt_derived<utf16_char32<false>>);
					else
						cvt_ptr.reset(new codecvt_derived<utf16_char32<true>>);
					begin += 2;
				} else if (bom[0] == 0xFF && bom[1] == 0xFE && bom[2] != 0 && bom[3] != 0) {
					// utf16 le
					if constexpr (std::endian::native == std::endian::little)
						cvt_ptr.reset(new codecvt_derived<utf16_char32<false>>);
					else
						cvt_ptr.reset(new codecvt_derived<utf16_char32<true>>);
					begin += 2;
				} else if (bom[0] == 0 && bom[1] == 0 && bom[2] == 0xFE && bom[3] == 0xFF) {
					// utf32 be
					if constexpr (std::endian::native == std::endian::big)
						cvt_ptr.reset(new codecvt_derived<utf32_char32<false>>);
					else
						cvt_ptr.reset(new codecvt_derived<utf32_char32<true>>);
					begin += 4;
				} else if (bom[0] == 0xFF && bom[1] == 0xFE && bom[2] == 0 && bom[3] == 0) {
					// utf32 le
					if constexpr (std::endian::native == std::endian::little)
						cvt_ptr.reset(new codecvt_derived<utf32_char32<false>>);
					else
						cvt_ptr.reset(new codecvt_derived<utf32_char32<true>>);
					begin += 4;
				} else if (bom[0] == 0x84 && bom[1] == 0x32 && bom[2] == 0x95 && bom[3] == 0x33) {
					// GB18030
					throw_posix_error(EIO);
					begin += 4;
				} else {
					cvt_ptr.reset(new codecvt_derived<utf8_char32>);
				}
			} else {
				cvt_ptr.reset(new codecvt_derived<utf8_char32>);
			}
		}
		reserve_write(out,cvt_ptr->required_size(end-begin),[&](auto ptr)
		{
			auto ret{cvt_ptr->do_convert(ptr,std::to_address(begin),std::to_address(end))};
			begin += ret.second - std::to_address(begin);
			return ret.first;
		});
		return begin;
	}

};

}

}

