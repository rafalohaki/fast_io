#include"../timer.h"

typedef uint32_t bignum25519[12];

extern "C" void curve25519_add_reduce(bignum25519 out, const bignum25519 a, const bignum25519 b) noexcept;

int main()
{
	fast_io::timer t("sse");
	for(std::size_t i{};i!=100000000;++i)
	{
		bignum25519 a{};
		bignum25519 b{};
		bignum25519 c;
		curve25519_add_reduce(c,a,b);
	}
}