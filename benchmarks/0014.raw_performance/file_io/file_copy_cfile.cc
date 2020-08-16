
#include <cstdio>
#include "../../timer.h"

int main()
{
	{
        fast_io::timer tm("copy_cfile");
        std::FILE *fpin(std::fopen("ref.in", "rb"));
        std::FILE *fpout(std::fopen("copy_cfile.txt", "wb"));
        char c;
        while ((c = std::fgetc(fpin)) != EOF) {
            fputc(c, fpout);
        }
        fclose(fpin);
        fclose(fpout);
	}

    return 0;
}

