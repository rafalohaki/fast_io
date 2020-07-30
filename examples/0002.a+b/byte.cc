#include"../../include/fast_io.h"

int main()
{
	std::byte a,b;
	scan(a,b);
	println(std::byte(std::to_integer<char8_t>(a)+std::to_integer<char8_t>(b)));
}