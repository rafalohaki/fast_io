#include"../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_legacy.h"
#include<thread>

template<bool need_rewind=true>
inline void worker_thread(fast_io::obuf_file& obf,std::size_t from,std::size_t to)
{
	for(;from!=to;++from)
		println(obf,from);
	if constexpr(need_rewind)
		rewind(obf);
}

int main()
{
	constexpr std::size_t N(10000000);
	{
	fast_io::timer t("output");
	std::size_t threads{std::thread::hardware_concurrency()*2};
	if(threads<2)
	{
		panic("Not support multithreads\n");
	}
	std::size_t item_per_thread{N/threads};
	std::vector<fast_io::obuf_file> files;
	files.reserve(threads);
	fast_io::obuf_file obf;
	{
	std::vector<std::jthread> thvec;
	thvec.reserve(threads);
	for(std::size_t i{};i!=threads-1;++i)
	{
		files.emplace_back(fast_io::io_temp);
		if(!i)
			thvec.emplace_back(worker_thread<true>,std::ref(files[i]),(threads-i-1)*item_per_thread,N);
		else
			thvec.emplace_back(worker_thread<true>,std::ref(files[i]),(threads-i-1)*item_per_thread,(threads-i)*item_per_thread);
	}
	obf=fast_io::obuf_file("multithreads_obuf_file_temp.txt");
	worker_thread<false>(obf,0,item_per_thread);
	}
	for(auto it{files.rbegin()};it!=files.rend();++it)
		transmit(obf,*it);
	}
}
