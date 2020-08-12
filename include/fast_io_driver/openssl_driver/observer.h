#pragma once

namespace fast_io::openssl::details
{

template<typename T>
class observer
{
public:
	using native_handle_type = T;
	native_handle_type handle{};
	constexpr operator bool() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
	constexpr void reset(native_handle_type newhandle=nullptr) noexcept
	{
		handle=newhandle;
	}
	constexpr void swap(observer& other) noexcept
	{
		std::swap(handle,other.handle);
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=nullptr;
		return temp;
	}
};

}