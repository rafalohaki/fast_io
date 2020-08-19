#pragma once

namespace type_erasure
{

namespace details
{
struct shape_base
{
	virtual constexpr ~shape_base() = default;
	virtual constexpr double area_impl() const = 0;
	virtual constexpr shape_base* clone() const = 0;
};
template<typename T>
struct shape_derived: shape_base
{
	T t;
	template<typename... Args>
	constexpr shape_derived(std::in_place_t,Args&&... args):t{std::forward<Args>(args)...}{}
	constexpr double area_impl() const override
	{
		if constexpr(requires()
		{
			t.area();
		})
			return t.area();
		else
			return area(t);
	}
	constexpr shape_base* clone() const override
	{
		if constexpr(std::copyable<T>)
			return new shape_derived<T>(*this);
		else
			throw std::runtime_error("type not copyable");			
	}
};
}

struct shape_observer
{
	using native_handle_type = details::shape_base*;
	native_handle_type ptr{};
	inline constexpr auto& native_handle() const noexcept
	{
		return ptr;
	}
	inline constexpr auto& native_handle() noexcept
	{
		return ptr;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{ptr};
		ptr=nullptr;
		return temp;
	}
	inline constexpr operator bool() const noexcept
	{
		return ptr;
	}
	inline constexpr void reset(native_handle_type h=nullptr)
	{
		ptr=h;
	}
};

struct shape:shape_observer
{
	using native_handle_type = details::shape_base*;
	constexpr shape()=default;
	constexpr shape(native_handle_type bptr):shape_observer{bptr}{}
	template<typename T,typename... Args>
	constexpr shape(std::in_place_type_t<T>,Args&& ...args):shape_observer{new details::shape_derived<T>(std::in_place,std::forward<Args>(args)...)}{}
	constexpr void reset(native_handle_type h=nullptr)
	{
		delete this->native_handle();
		ptr=h;
	}
	constexpr shape(shape&& other) noexcept:shape_observer{other.native_handle()}
	{
		other.native_handle()=nullptr;
	}
	constexpr shape& operator=(shape&& other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		delete this->native_handle();
		this->native_handle()=other.release();
		return *this;
	}
	constexpr shape(shape const& other):shape_observer{other.ptr->clone()}{}
	constexpr shape& operator=(shape const& other)
	{
		auto temp{other.ptr->clone()};
		delete this->native_handle();
		this->native_handle()=temp;
		return *this;
	}
	constexpr ~shape()
	{
		delete this->native_handle();
	}
};


inline constexpr double area(shape_observer s)
{
	return s.ptr->area_impl();
}

struct rectangle
{
	double length{},width{};
};

inline constexpr double area(rectangle r)
{
	return r.length*r.width;
}

struct circle
{
	double radius{};
};

inline constexpr double area(circle c)
{
	return std::numbers::pi*c.radius*c.radius;
}

struct square
{
	double length{};
};

inline constexpr double area(square s)
{
	return s.length*s.length;
}

}