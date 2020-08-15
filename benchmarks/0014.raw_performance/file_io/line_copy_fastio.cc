
#include <fstream>
#include <string>

#include "../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"

int main()
{
    std::string line;
    {
        fast_io::timer tm("line_fastio");
        fast_io::ibuf_file ibf("ref.txt");
        fast_io::obuf_file obf("line_copy_fastio.txt");
        for (;scan<true>(ibf, fast_io::line(line));) {
            println(obf, line);
        }
	}

    return 0;
}
