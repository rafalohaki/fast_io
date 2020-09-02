#include"../timer.h"
#include<vector>
#include"thread_local_pool.h"

int main()
{
	fast_io::timer tm("allocate");
	for(std::size_t i{};i!=10000000;++i)
	{
		std::vector<std::size_t> vec(10);
	}
}