#include"../../include/fast_io.h"

int main()
{
	for(auto const& line: line_generator(fast_io::c_stdin()))
		println(line);
}
