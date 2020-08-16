
#include <fstream>
#include <string>

#include "../../timer.h"

int main()
{
    {
        fast_io::timer tm("file_copy_fstream");
        std::ifstream ifs("ref.in");
        std::ofstream ofs("file_copy_fstream.txt");
        char c;
        while (ifs.get(c)) {
            ofs.put(c);
        }
	}

    return 0;
}
