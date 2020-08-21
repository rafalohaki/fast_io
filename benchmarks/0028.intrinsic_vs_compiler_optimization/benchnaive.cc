#include"../timer.h"
#include"naive.h"

extern number add_reduce(number const& a,number const& b) noexcept;

int main()
{
	fast_io::timer t("naive");
	number a{},b{};
	for(std::size_t i{};i!=100000000;++i)
	{
		a=add_reduce(a,b);
	}
}