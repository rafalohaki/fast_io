#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf_file ibf("lines.txt");
	for(auto const& line: line_generator(ibf))
		println(line);
}
