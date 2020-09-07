#pragma once

namespace fast_io
{

template<stream stm>
struct io_ref
{
	stm* ptr{};
	constexpr io_ref()=default;
	constexpr io_ref(stm& sm):ptr(std::addressof(sm)){}
	constexpr io_ref(stm&&)=delete;
	constexpr auto& operator*() const noexcept
	{
		return *ptr;
	}
	constexpr auto operator->() const noexcept
	{
		return ptr;
	}
	constexpr auto& operator*() noexcept
	{
		return *ptr;
	}
	constexpr auto operator->() noexcept
	{
		return ptr;
	}
	constexpr operator bool() const noexcept
	{
		return ptr;
	}
};



}
