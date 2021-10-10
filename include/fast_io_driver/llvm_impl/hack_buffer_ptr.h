#pragma once

namespace fast_io::details
{

template<std::size_t position>
requires (position<3u)
inline constexpr std::size_t llvm_raw_ostream_cal_obuffer_ptr_position() noexcept
{
	constexpr std::size_t offset{sizeof(void*) /*vptr*/+sizeof(OStreamKind)};
	if constexpr(position==0u)
		return offset;
	else if constexpr(position==1u)
		return offset+2*sizeof(char*);
	else
		return offset+sizeof(char*);
}

template<std::size_t position,std::integral char_type>
requires (position<3u)
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char_type* llvm_raw_ostream_obuffer_ptr(raw_ostream* os) noexcept
{
	constexpr std::size_t offset{llvm_raw_ostream_cal_obuffer_ptr_position<position>()};
	char_type* ptr;
	::fast_io::details::my_memcpy(__builtin_addressof(ptr),reinterpret_cast<char*>(os+offset),sizeof(char_type*));
	return ptr;
}

inline void llvm_raw_ostream_set_obuffer_curr_ptr(raw_ostream* os,void* vdptr)
{
	constexpr std::size_t offset{llvm_raw_ostream_cal_obuffer_ptr_position<1>()};
	::fast_io::details::my_memcpy(reinterpret_cast<char*>(os+offset),__builtin_addressof(vptr),sizeof(char*));
}

inline void llvm_raw_ostream_overflow(raw_ostream* os,char ch)
{
	os->flush();
	os->write(__builtin_addressof(ch),__builtin_addressof(ch)+1);
}

}
