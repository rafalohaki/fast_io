
#include <cstdio>
#include "../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"

int main()
{
	{
        fast_io::timer tm("copy_transmit");
        fast_io::ibuf_file ibf("ref.txt");
        fast_io::obuf_file obf("copy_transmit.txt");
        transmit(obf, ibf);
	}

    return 0;
}

