#include <random>

#include <cstdio>
#include "../../timer.h"

int main()
{
    std::mt19937 gen(1919810);
    std::uniform_int_distribution<> distrib(1, 1145141919);

    constexpr std::size_t N{100000000};
	{
        fast_io::timer tm("cfile_random_numbers");
        std::FILE *fp(std::fopen("cfile_random_numbers.txt", "w"));
        for(std::size_t i{}; i != N; ++i)
            fprintf(fp, "%d\n", distrib(gen));
        fclose(fp);
	}

    return 0;
}
