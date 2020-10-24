#pragma once

namespace manip
{
template<typename T>
concept value_based_stream = requires(T t)
{
	{io_value_handle(t)};
};

template<typename T>
concept manipulator = std::is_trivially_copyable_v<T>&&requires(T t)
{
	t.data;
	ref_decay(t);
};

template<typename T>
concept print_alias_manipulator = manipulator<T>&&requires(T t)
{
	print_alias_ref_decay(t);
};


template<typename U,typename T>
inline constexpr auto make_trivial_manipulator(T&& t) noexcept;

template<typename U,typename T>
struct trivial_manipulator
{
	using tag_type = U;
	using element_type = T;
	using value_type = std::remove_cvref_t<T>;
	[[no_unique_address]] element_type data;
};

template<typename U,typename T>
[[gnu::always_inline]] inline constexpr auto make_trivial_manipulator_ref(T&& t) noexcept
{
	if constexpr(manipulator<std::remove_cvref_t<T>>||value_based_stream<std::remove_cvref_t<T>>)
		return trivial_manipulator<U,std::remove_cvref_t<T>>{t};
	else
		return trivial_manipulator<U,std::remove_reference_t<T>&>{t};
};

template<typename U,typename T>
[[gnu::always_inline]] inline constexpr auto make_trivial_manipulator(T&& t) noexcept
{
	if constexpr(manipulator<std::remove_cvref_t<T>>||value_based_stream<std::remove_cvref_t<T>>)
		return trivial_manipulator<U,std::remove_cvref_t<T>>{t};
	else
	{
		if constexpr(std::is_trivially_copyable_v<std::remove_cvref_t<T>>&&sizeof(std::remove_cvref_t<T>)<=alignof(std::max_align_t))
			return trivial_manipulator<U,std::remove_cvref_t<T>>{t};
		else
			return trivial_manipulator<U,std::remove_reference_t<T>&>{t};
	}
};

template<typename U,typename T>
[[gnu::always_inline]] inline constexpr auto ref_decay(trivial_manipulator<U,T> s) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(manipulator<value_type>)
	{
		if constexpr(std::same_as<decltype(make_trivial_manipulator<U>(ref_decay(s.data))),trivial_manipulator<U,T>>)
			return s;
		else
			return make_trivial_manipulator<U>(ref_decay(s.data));
	}
	else if constexpr(std::is_trivially_copyable_v<std::remove_cvref_t<T>>&&sizeof(std::remove_cvref_t<T>)<=alignof(std::max_align_t))
		return make_trivial_manipulator<U>(s.data);
	else
		return s;
}

template<std::integral char_type,typename U,typename T>
[[gnu::always_inline]] inline constexpr auto print_alias_ref_decay(trivial_manipulator<U,T> s) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(manipulator<value_type>)
	{
		if constexpr(std::same_as<decltype(make_trivial_manipulator<U>(print_alias_ref_decay(s.data))),trivial_manipulator<U,T>>)
			return s;
		else
			return make_trivial_manipulator<U>(print_alias_ref_decay(s.data));
	}
	else if constexpr(std::is_trivially_copyable_v<std::remove_cvref_t<T>>&&sizeof(std::remove_cvref_t<T>)<=alignof(std::max_align_t))
		return make_trivial_manipulator<U>(s.data);
	else
		return s;
}

template<typename U,typename S,typename T>
inline constexpr auto make_state_manipulator(T&& t,S&& s) noexcept;

template<typename U,typename S,typename T>
requires (std::is_trivially_copyable_v<S>)
struct state_manipulator
{
	using tag_type = U;
	using element_type = T;
	using value_type = std::remove_cvref_t<T>;
	using state_type = S;
	[[no_unique_address]] T data;
	[[no_unique_address]] state_type state;
};


template<typename U,typename S,typename T>
[[gnu::always_inline]] inline constexpr auto ref_decay(state_manipulator<U,S,T> s) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(manipulator<value_type>)
	{
		if constexpr(std::same_as<decltype(make_state_manipulator<U,S>(ref_decay(s.data))),state_manipulator<U,S,T>>)
			return s;
		else
			return make_state_manipulator<U,S>(ref_decay(s.data),s.state);
	}
	else if constexpr(std::is_trivially_copyable_v<std::remove_cvref_t<T>>&&sizeof(std::remove_cvref_t<T>)<=alignof(std::max_align_t))
		return make_state_manipulator<U,S>(s.data,s.state);
	else
		return s;
}

template<typename U,typename S,typename T>
[[gnu::always_inline]] inline constexpr auto make_state_manipulator_ref(T&& t,S&& s) noexcept
{
	if constexpr(manipulator<std::remove_cvref_t<T>>||value_based_stream<std::remove_cvref_t<T>>)
		return state_manipulator<U,S,std::remove_cvref_t<T>>{t,s};
	else
		return state_manipulator<U,S,std::remove_reference_t<T>&>{t,s};
};

template<typename U,typename S,typename T>
[[gnu::always_inline]] inline constexpr auto make_state_manipulator(T&& t,S&& s) noexcept
{
	if constexpr(manipulator<std::remove_cvref_t<T>>||value_based_stream<std::remove_cvref_t<T>>)
		return state_manipulator<U,S,std::remove_cvref_t<T>>{t,s};
	else
	{
		if constexpr(std::is_trivially_copyable_v<std::remove_cvref_t<T>>&&sizeof(std::remove_cvref_t<T>)<=alignof(std::max_align_t))
			return state_manipulator<U,S,std::remove_cvref_t<T>>{t,s};
		else
			return state_manipulator<U,S,std::remove_reference_t<T>&>{t,s};
	}
};

namespace tags
{

struct chvw_tag{};

}

template<typename T>
using chvw_t = trivial_manipulator<tags::chvw_tag,T>;

template<typename T>
inline constexpr auto chvw(T&& t) noexcept
{
	return make_trivial_manipulator_ref<tags::chvw_tag,T>(std::forward<T>(t));
}

}