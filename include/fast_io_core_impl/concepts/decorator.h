#pragma once

namespace fast_io
{

template<std::input_or_output_iterator FromIter,std::input_or_output_iterator ToIter>
requires (std::is_trivially_copyable_v<std::iter_value_t<FromIter>>&&
	std::is_trivially_copyable_v<std::iter_value_t<ToIter>>)
struct basic_io_decorate_t
{
	using from_iter_type = FromIter;
	using to_iter_type = ToIter;
	using from_value_type = std::iter_value_t<from_iter_type>;
	using to_value_type = std::iter_value_t<to_iter_type>;
	from_iter_type from_iter;
	to_iter_type to_iter;
};

namespace details
{
template<typename T>
concept zero_reserve_decorator_impl=requires(T t,typename std::remove_cvref_t<T>::from_type const* from_iter,typename std::remove_cvref_t<T>::to_type* to_iter)
{
	{deco_reserve_define(t,from_iter,from_iter,to_iter)}->std::convertible_to<typename std::remove_cvref_t<T>::to_type*>;
};

template<typename T>
concept normal_decorator_impl=requires(T t,typename std::remove_cvref_t<T>::from_type const* from_iter,typename std::remove_cvref_t<T>::to_type* to_iter)
{
	{deco_reserve_define(t,from_iter,from_iter,to_iter)}->std::convertible_to<basic_io_decorate_t<typename std::remove_cvref_t<T>::from_type const*,typename std::remove_cvref_t<T>::to_type*>>;
};

}

template<typename T>
concept decorator=std::is_trivially_copyable_v<typename std::remove_cvref_t<T>::from_type>&&std::is_trivially_copyable_v<typename std::remove_cvref_t<T>::to_type>&&requires(T t)
{
	typename std::remove_cvref_t<T>::from_type;
	typename std::remove_cvref_t<T>::to_type;
	requires (details::zero_reserve_decorator_impl<T>||details::normal_decorator_impl<T>);
};

template<typename T>
concept zero_reserve_decorator=decorator<T>&&details::zero_reserve_decorator_impl<T>;

template<typename T>
concept nop_decorator=decorator<T>&&requires(T t)
{
	{deco_is_nop(t)}->std::convertible_to<bool>;
};

template<typename T>
concept decorated_input_stream = input_stream<T>&&requires(T t)
{
	typename std::remove_cvref_t<T>::input_decorator_type;
	{get_idecorator(t)}->decorator;
};

template<typename T>
concept decorated_output_stream = output_stream<T>&&requires(T t)
{
	typename std::remove_cvref_t<T>::output_decorator_type;
	{get_odecorator(t)}->decorator;
};

}