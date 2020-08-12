#pragma once

namespace fast_io
{

class iocp_overlapped_base:public fast_io::win32::overlapped
{
public:
	constexpr iocp_overlapped_base():fast_io::win32::overlapped{}
	{}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	virtual void invoke(std::size_t) noexcept = 0;
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	virtual ~iocp_overlapped_base()=default;
};

template<typename T>
class iocp_overlapped_derived:public iocp_overlapped_base
{
public:
	T callback;
	template<typename... Args>
	requires std::constructible_from<T,Args...>
	constexpr iocp_overlapped_derived(std::in_place_t,Args&& ...args):callback(std::forward<Args>(args)...){}
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void invoke(std::size_t res) noexcept override
	{
		callback(res);
	}
};

class iocp_overlapped_observer
{
public:
	using native_handle_type = iocp_overlapped_base*;
	native_handle_type handle{};
#if __cpp_constexpr >= 201907L
	constexpr
#endif
	void operator()(std::size_t res) noexcept
	{
		handle->invoke(res);
	}
	constexpr native_handle_type const& native_handle() const noexcept
	{
		return handle;
	}
	constexpr native_handle_type& native_handle() noexcept
	{
		return handle;
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{handle};
		handle={};
		return temp;
	}
	inline constexpr void reset(native_handle_type newhandle=nullptr) noexcept
	{
		handle=newhandle;
	}
	inline constexpr void swap(iocp_overlapped_observer& other) noexcept
	{
		std::swap(handle, other.handle);
	}
};

class iocp_overlapped:public iocp_overlapped_observer
{
public:
	using native_handle_type = iocp_overlapped_base*;
	constexpr iocp_overlapped()=default;
	constexpr iocp_overlapped(native_handle_type hd):iocp_overlapped_observer{hd}{}

	template<typename T,typename... Args>
	requires std::constructible_from<T,Args...>
#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	iocp_overlapped(std::in_place_type_t<T>,Args&& ...args):
		iocp_overlapped_observer{new iocp_overlapped_derived<T>(std::in_place,std::forward<Args>(args)...)}{}
	template<typename Func>
#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	iocp_overlapped(std::in_place_t,Func&& func):iocp_overlapped(std::in_place_type<std::remove_cvref_t<Func>>,std::forward<Func>(func)){}

	iocp_overlapped(iocp_overlapped const&)=delete;
	iocp_overlapped& operator=(iocp_overlapped const&)=delete;
	constexpr iocp_overlapped(iocp_overlapped&& bmv) noexcept : iocp_overlapped_observer{bmv.release()}{}

#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif	
	iocp_overlapped& operator=(iocp_overlapped&& bmv) noexcept
	{
		if(bmv.native_handle()==this->native_handle())
			return *this;
		delete this->native_handle();
		this->native_handle() = bmv.release();
		return *this;
	}
	

#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	~iocp_overlapped()
	{
		delete this->native_handle();
	}
};

}
