#include <random>

#include <fstream>
#include "../../timer.h"

int main()
{
    std::mt19937 gen(1919810);
    std::uniform_int_distribution<> distrib(1, 1145141919);

    constexpr std::size_t N{100000000};
	{
        fast_io::timer tm("ofstream_random_numbers");
        std::ofstream ofs("ofstream_random_numbers.txt");
        for(std::size_t i{}; i != N; ++i)
            ofs << distrib(gen) << "\n";
	}

    return 0;
}
