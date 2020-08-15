
#include <cstdio>
#include "../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"

int main()
{
	{
        fast_io::timer tm("copy_igenerator");
        fast_io::ibuf_file ibf("ref.txt");
        fast_io::obuf_file obf("copy_igenerator.txt");
        for(auto ch:igenerator(ibf))
            print(obf, fast_io::chvw(ch));
	}

    return 0;
}

