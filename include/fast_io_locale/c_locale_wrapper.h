#pragma once

namespace fast_io
{

struct c_locale_dummy_field
{};

template<stream stm,typename issp=c_ctype_isspace,typename lcvtp=basic_c_lconv_storage<typename stm::char_type>>
class c_locale_collection
{
public:
	using isspace_type = std::conditional_t<character_input_stream<stm>,issp&,c_locale_dummy_field>;
	using lconv_type = std::conditional_t<output_stream<stm>,lcvtp&,c_locale_dummy_field>;
	isspace_type isspace;
	lconv_type lconv;
};

template<stream stm,typename collectionType=c_locale_collection<stm>>
class c_locale_wrapper:public stm
{
public:
	using handle_type = stm;
	using char_type = stm::char_type;
	using locale_collection_type = collectionType;
	using status_type = collectionType;
	locale_collection_type locale_collection;
};

namespace details
{

template<typename T,typename S,typename U>
constexpr decltype(auto) construct_lcv([[maybe_unused]]S&& s,U&& u)
{
	if constexpr(general_printable<T&,manip::lcv<typename T::char_type,U&>>)
		return lcv(std::forward<S>(s),std::forward<U>(u));
	else
		return std::forward<U>(u);
}
template<typename T,typename S,typename U>
constexpr decltype(auto) construct_isp([[maybe_unused]]S&& s,U&& u)
{
	if constexpr(space_scanable<T&,manip::space<S&,U&>>)
		return manip::space(std::forward<S>(s),std::forward<U>(u));
	else
		return std::forward<S>(s);
}


}

template<output_stream stm,typename... Args>
constexpr void print_status_define(c_locale_wrapper<stm>& sm,Args&& ...args)
{
//	print_freestanding(static_cast<stm&>(sm),details::construct_lcv<stm>(sm.locale_collection.lconv,args)...);
}

template<output_stream stm,typename... Args>
constexpr void println_status_define(c_locale_wrapper<stm>& sm,Args&& ...args)
{
//	println(static_cast<stm&>(sm),details::construct_lcv<stm>(sm.locale_collection.lconv,args)...);
}


template<bool report_eof,input_stream stm,typename... Args>
constexpr decltype(auto) scan_status_define(c_locale_wrapper<stm>& sm,Args&& ...args)
{
	return scan<report_eof>(static_cast<stm&>(sm),
		details::construct_isp<stm>(args,sm.locale_collection.isspace)...);
}

}