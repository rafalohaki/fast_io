#include <random>

#include "../../../include/fast_io.h"
#include "../../../include/fast_io_device.h"
#include "../../timer.h"

int main()
{
    std::mt19937 gen(1919810);
    std::uniform_int_distribution<> distrib(1, 1145141919);

    constexpr std::size_t N{100000000};
	{
        fast_io::timer tm("obuf_file_random_numbers");
        fast_io::obuf_file obf("obuf_file_random_numbers.txt");
        for(std::size_t i{}; i != N; ++i)
            println(obf, distrib(gen));
	}

    return 0;
}
