#include"../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_legacy.h"
#include<thread>



inline void worker_thread(fast_io::native_memory_map_io_observer nvmfs,std::size_t from,std::size_t to,std::size_t& result)
{
	fast_io::basic_omemory_map<char> bomp(nvmfs);
	for(;from!=to;++from)
		println(bomp,from);
	result=bomp.written_bytes();
}

int main()
{
	constexpr std::size_t N(10000000);
	
	fast_io::timer t("output");
	std::size_t threads{std::thread::hardware_concurrency()-1};
	if(threads<2)
	{
		panic("Not support multithreads\n");
	}
	std::size_t item_per_thread{N/threads};
	std::vector<fast_io::onative_file> files;
	files.reserve(threads);
	constexpr std::size_t preallocate_file_size{78888890};
	constexpr std::size_t preallocate_file_size_per{15000000};
	std::vector<fast_io::native_memory_map_file> nvmfs;
	std::vector<std::size_t> results(threads);
	std::vector<std::jthread> thvec;
	nvmfs.reserve(threads);
	thvec.reserve(threads);
	for(std::size_t i{};i!=threads;++i)
	{
		auto& back{files.emplace_back(fast_io::io_temp)};
		truncate(back,preallocate_file_size);
		fast_io::native_memory_map_io_observer mm_back=nvmfs.emplace_back(files[i],fast_io::file_map_attribute::read_write,preallocate_file_size_per);
		if(!i)
			thvec.emplace_back(worker_thread,mm_back,(threads-i-1)*item_per_thread,N,std::ref(results[i]));
		else
			thvec.emplace_back(worker_thread,mm_back,(threads-i-1)*item_per_thread,(threads-i)*item_per_thread,std::ref(results[i]));
	}
	fast_io::native_file result_file("multithreads_memory_map_temp.txt",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::app|fast_io::open_mode::trunc);
	fast_io::native_memory_map_file mmf(result_file,fast_io::file_map_attribute::read_write,preallocate_file_size);
	truncate(result_file,preallocate_file_size);
	fast_io::omemory_map bomp(mmf);
	for(std::size_t i{threads};i--;)
	{
		thvec[i].join();
		write(bomp,reinterpret_cast<char const*>(nvmfs[i].address_begin),reinterpret_cast<char const*>(nvmfs[i].address_begin)+results[i]);
	}

}
