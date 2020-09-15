#pragma once
#include<thread>

namespace fast_io
{

namespace manip
{

template<typename Func>
struct parallel_counter
{
	Func callback;
	std::size_t count{};
	std::size_t chars_per_element{};
};
template<typename T,typename Func>
struct parallel
{
	T& reference;
	Func callback;
};

template<typename T>
struct parallel<T,void>
{
	T& reference;
};

template<typename Func>
struct parallel_unit
{
	Func callback;
	std::size_t count{};
};

}

template<typename Func>
inline constexpr manip::parallel_counter<Func> parallel_counter(Func func,std::size_t count,std::size_t chars_per_element){return {func,count,chars_per_element};}

template<std::ranges::random_access_range T>
inline constexpr manip::parallel<T,void> parallel(T& r){return {r};}
template<std::ranges::random_access_range T,typename Func>
inline constexpr manip::parallel<T,Func> parallel(T& r,Func callback){return {r,callback};}

template<typename Func>
inline constexpr manip::parallel_unit<Func> parallel_unit(Func callback,std::size_t count){return {callback,count};}

namespace details
{
template<std::integral ch_type,typename Func>
inline constexpr void print_out(fast_io::ospan<ch_type>* osp,Func func,std::size_t start_number,std::size_t stop_number)
{
	fast_io::ospan<ch_type> bas{*osp};
	func(bas,start_number,stop_number);
	*osp=bas;
}

template<std::integral ch_type>
struct span_raii
{
	using ospan_type = fast_io::ospan<ch_type>;
	ospan_type osp;
//	constexpr span_raii()=default;
	template<typename... Args>
	requires std::constructible_from<ospan_type,Args...>
	constexpr span_raii(Args&& ...args) noexcept:osp(std::forward<Args>(args)...){}
	constexpr span_raii(span_raii const&)=delete;
	constexpr span_raii(span_raii&& other) noexcept:osp(other.osp)
	{
		other.osp.span()={};
	}
	constexpr span_raii& operator=(span_raii const&)=delete;
	constexpr span_raii& operator=(span_raii&& other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		if(osp.span().data())[[likely]]
			delete[] osp.data();	
		osp.span()=other.osp.span();
		other.osp.span()={};
		return *this;
	}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	~span_raii()
	{
		if(osp.span().data())[[likely]]
			delete[] osp.span().data();
	}
};

template<output_stream stm,typename Func>
inline void parrallel_details_no_constexpr(stm& output,std::size_t count,std::size_t chars_per_element,Func& func)
{
	using char_type = typename stm::char_type;
	std::size_t thread_number{std::thread::hardware_concurrency()};

	if(thread_number<2)[[unlikely]]
	{
		if constexpr(reserve_output_stream<stm>)
		{
			func(output,0,count);
		}
		else
		{
			std::size_t const total_chars{count*chars_per_element};
			span_raii<char_type> osp{new char_type[total_chars],total_chars};
			func(osp.osp,0,count);
			write(output,osp.osp.span().data(),osp.osp.span().data()+osize(osp.osp));
		}
		return;
	}

	std::size_t range{count/thread_number};
	std::size_t const module{count-range*thread_number};
	std::size_t const allocation_size{range*chars_per_element};

	std::vector<details::span_raii<char_type>> thread_result;
	thread_result.reserve(thread_number);
	{
	std::vector<std::jthread> jth;
	jth.reserve(thread_number);
	std::size_t const offset{(range<<1)+module};
	std::size_t const offset_allocation_size{(range+module)*chars_per_element};
	jth.emplace_back(print_out<char_type,Func>,std::addressof(thread_result.emplace_back(new char_type[offset_allocation_size],offset_allocation_size).osp),
		func,range,offset);
	for(std::size_t i{2};i<thread_number;++i)
		jth.emplace_back(print_out<char_type,Func>,std::addressof(thread_result.emplace_back(new char_type[allocation_size],allocation_size).osp),
		func,offset+(i-2)*range,offset+(i-1)*range);
	if constexpr(reserve_output_stream<stm>)
		func(output,0,range);
	else
	{
		span_raii<char_type> osp{new char_type[allocation_size],allocation_size};
		func(osp.osp,0,range);
		write(output,osp.osp.span().data(),osp.osp.span().data()+osize(osp.osp));
	}
	}
	for(auto& e : thread_result)
		write(output,e.osp.span().data(),e.osp.span().data()+osize(e.osp));
}

template<output_stream stm,typename Func>
inline
#if __cpp_lib_is_constant_evaluated >= 201811L
constexpr
#endif
void parrallel_details(stm& output,std::size_t count,std::size_t chars_per_element,Func func)
{
	using char_type = typename stm::char_type;
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
	{
		if constexpr(reserve_output_stream<stm>)
		{
			func(output,0,count);
		}
		else
		{
			std::size_t const total_chars{count*chars_per_element};
			span_raii<char_type> osp{new char_type[total_chars],total_chars};
			func(osp.osp,0,count);
			write(output,osp.osp.span().data(),osp.osp.span().data()+osize(osp.osp));
		}
	}
	else
	{
#endif
		parrallel_details_no_constexpr(output,count,chars_per_element,func);
#if __cpp_lib_is_constant_evaluated >= 201811L
	}
#endif
}


}

template<output_stream stm,typename Func>
inline constexpr void print_define(stm& output,manip::parallel_counter<Func> ref)
{
	details::parrallel_details(output,ref.count,ref.chars_per_element,ref.callback);
}


template<output_stream stm,std::ranges::random_access_range R,typename Func>
inline constexpr void print_define(stm& output,manip::parallel<R,Func> ref)
{
	auto be{std::ranges::begin(ref.reference)};
	if constexpr(std::same_as<void,Func>)
	{
		constexpr std::size_t sz{print_reserve_size(io_reserve_type<std::remove_cvref_t<std::ranges::range_value_t<R>>>)};
		details::parrallel_details(output,std::ranges::size(ref.reference),sz,
			[be](reserve_output_stream auto& output,std::size_t start_number,std::size_t stop_number)
		{
			auto ed{be+stop_number};
			for(auto iter{be+start_number};iter!=ed;++iter)
				print_freestanding(output,*iter);
		});
	}
	else
	{
		constexpr std::size_t sz{print_reserve_size(io_reserve_type<std::remove_cvref_t<
		decltype(ref.callback(*std::ranges::begin(ref.reference)))>>)};

		details::parrallel_details(output,std::ranges::size(ref.reference),sz,
			[&](reserve_output_stream auto& output,std::size_t start_number,std::size_t stop_number)
		{
			auto ed{be+stop_number};
			for(auto iter{be+start_number};iter!=ed;++iter)
				print_freestanding(output,ref.callback(*iter));
		});
	}
}


template<output_stream stm,typename Func>
inline constexpr void print_define(stm& output,manip::parallel_unit<Func> ref)
{
	constexpr std::size_t sz{print_reserve_size(io_reserve_type<std::remove_cvref_t<
	decltype(ref.callback(static_cast<std::size_t>(0)))>>)};
	details::parrallel_details(output,ref.count,sz,
		[&ref](reserve_output_stream auto& output,std::size_t start_number,std::size_t stop_number)
	{
		for(;start_number!=stop_number;++start_number)
			print_freestanding(output,ref.callback(start_number));
	});
}


}