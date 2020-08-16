
#include <fstream>
#include <string>

#include "../../timer.h"

int main()
{
    std::string line;
    {
        fast_io::timer tm("line_fstream");
        std::ifstream ifs("ref.in");
        std::ofstream ofs("line_copy_fstream.txt");
        while (std::getline(ifs, line)) {
            ofs << line << "\n";
        }
	}

    return 0;
}
