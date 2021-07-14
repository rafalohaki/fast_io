#pragma once

namespace fast_io
{

template<output_stream Ohandler,typename Ohandler::char_type flush_character>
class char_flush:public Ohandler
{
public:
	using char_type = typename Ohandler::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	constexpr char_flush(Args&&... args):Ohandler(::fast_io::freestanding::forward<Args>(args)...){}
};

template<output_stream Ohandler,typename Ohandler::char_type flush_character,::fast_io::freestanding::contiguous_iterator Iter>
inline constexpr void write(char_flush<Ohandler,flush_character>& ob,Iter b,Iter e)
{
	using char_type = typename Ohandler::char_type;
	auto pb(::fast_io::freestanding::to_address(b)),pe(pb+(e-b)*sizeof(*b)/sizeof(char_type));
	for(auto pi(pb);pi!=pe;++pi)
		if(*pi==flush_character)
		{
			write(static_cast<Ohandler&>(ob),pb,pi+1);
			flush(ob);
			pb=pi+1;
		}
	write(static_cast<Ohandler&>(ob),pb,pe);
}

template<character_output_stream Ohandler,typename Ohandler::char_type flush_character>
inline constexpr void put(char_flush<Ohandler,flush_character>& ob,typename Ohandler::char_type ch)
{
	put(static_cast<Ohandler&>(ob),ch);
	if(ch==flush_character)
		flush(ob);
}

template<typename T>
using line_flush = char_flush<T,0xA>;

}
