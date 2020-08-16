
#include <cstdio>
#include <string>

#include "../../timer.h"

int main()
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    {
        fast_io::timer tm("line_cfile");
        std::FILE *ifs(std::fopen("ref.in", "r"));
        std::FILE *ofs(std::fopen("line_copy_cfile.txt", "w"));
        while ((read = getline(&line, &len, ifs)) != -1) {
            fputs(line, ofs);
        }
        fclose(ifs);
        fclose(ofs);
	}

    return 0;
}
