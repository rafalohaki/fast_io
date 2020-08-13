#include"../../include/fast_io_device.h"

int main()
{
	fast_io::onative_file onf("onf_close.txt");
	
	onf.close();
}