#include"../../include/fast_io.h"

int main()
{
	std::array<char,40> array;
	fast_io::ospan osp(array);
	print(osp,"hello world\n");
	drain(fast_io::c_stdout(),osp);
}