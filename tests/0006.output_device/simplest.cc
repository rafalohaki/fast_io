#include<fast_io.h>

struct foo
{
	using char_type = char8_t;
};

inline constexpr foo io_value_handle(foo f) noexcept
{
	return f;
}

inline constexpr void write(foo,char8_t const* first,char8_t const* last)
{
	char8_t val{};
	auto diff{last-first};
	for(;first!=last;++first)
		val^=*first;
	println(diff," ",val);
}

int main()
{
	foo f;
	println(f,u8"Hello World\n",4,u8" ",20);
}