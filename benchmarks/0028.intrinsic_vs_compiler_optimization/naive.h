#include<array>
#include<cstdint>

struct number
{
	std::array<std::uint64_t,5> num;

	inline constexpr number& operator+=(number const& other)  noexcept
	{
		num.front()+=other.num.front();
		num[1]+=other.num[1];
		num[2]+=other.num[2];
		num[3]+=other.num[3];
		num[4]+=other.num[4];
        return *this;
	}

	inline constexpr std::uint64_t& operator[](std::size_t position) noexcept
	{
		return num[position];
	}
	inline constexpr std::uint64_t const& operator[](std::size_t position) const noexcept
	{
		return num[position];
	}

	inline constexpr number& operator^=(std::uint64_t constant) noexcept
	{
		num.front()^=constant;
		num[1]^=constant;
		num[2]^=constant;
		num[3]^=constant;
		num[4]^=constant;
        return *this;
	}

	inline constexpr number& operator&=(std::uint64_t constant) noexcept
	{
		num.front()&=constant;
		num[1]&=constant;
		num[2]&=constant;
		num[3]&=constant;
		num[4]&=constant;
        return *this;
	}

	inline constexpr number& operator>>=(std::uint32_t constant) noexcept
	{
		num.front()>>=constant;
		num[1]>>=constant;
		num[2]>>=constant;
		num[3]>>=constant;
		num[4]>>=constant;
        return *this;
	}

	inline constexpr number& round_add_assignment(number const& other) noexcept
	{
		num[1]+=other[0];
		num[2]+=other[1];
		num[3]+=other[2];
		num[4]+=other[3];
        return *this;
	}
};
