#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include<thread>


int main()
try
{
	auto t0{std::chrono::high_resolution_clock::now()};
	std::size_t threads(std::thread::hardware_concurrency()*4);
	auto dns_query_result{fast_io::dns_once("soft.wzhszm.com")};
	{
	std::size_t content_length{};
	{
	fast_io::ibuf_tcp_client hd(dns_query_result,80);
	print(hd,u8"GET /yy_24400.zip HTTP/1.1\r\n"
		"Host:soft.wzhszm.com\r\n"
		"User-agent:whatever\r\n"
		"Accept-Type:*/*\r\n"
		"Connection:close\r\n\r\n");
	fast_io::http_status status;
	scan(hd,status);
	for(auto line:scan_http_header(hd))
	{
		if(line.key=="Content-Length")
			content_length=fast_io::to<std::size_t>(line.value);
	}
	}
	std::vector<std::jthread> pools;
	pools.reserve(threads);
	std::size_t const content_per_thread{content_length/threads};
	for(std::size_t i{};i!=threads;++i)
		pools.emplace_back([number = i,content_per_thread,content_length,threads,&dns_query_result]()
		{
			for(;;)
			{
				try
				{
					fast_io::obuf_file obf(fast_io::concat("yy_24400_multithread_part_",number,".zip"));
					std::size_t bytes_start{content_per_thread*number};
					std::size_t bytes_end{content_per_thread*(number+1)};
					if(number==threads-1)
						bytes_end = content_length;
					for(;;)
					{
						fast_io::ibuf_tcp_client hd(dns_query_result,80);
						print(hd,u8"GET /yy_24400.zip HTTP/1.1\r\n"
							"Host:soft.wzhszm.com\r\n"
							"User-agent:whatever\r\n"
							"Accept-Type:*/*\r\n"
							"Range: bytes=",bytes_start,u8"-",bytes_end-1,
							u8"\r\nConnection:close\r\n\r\n");
						fast_io::http_status status;
						scan(hd,status);
						std::size_t thread_content_length{};
						for(auto line:scan_http_header(hd))
						{
							if(line.key=="Content-Length")
								thread_content_length=fast_io::to<std::size_t>(line.value);
						}
						bytes_start+=transmit(obf,hd,thread_content_length);
						if(bytes_start==bytes_end)[[likely]]
							return;
						std::this_thread::sleep_for(std::chrono::seconds{1});
					}
				}
				catch(...)
				{
					std::this_thread::sleep_for(std::chrono::seconds{1});
				}
			}
		});
	}
	std::size_t bytes{};
	{
	fast_io::onative_file osf("yy_24400_multithread.zip");
	for(std::size_t i{};i!=threads;++i)
	{
		fast_io::inative_file inv(fast_io::concat("yy_24400_multithread_part_",i,".zip"));
		bytes+=transmit(osf,inv);
	}
	}
	double elapsed{std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-t0).count()};
	if(elapsed==0)[[unlikely]]
		println("Transmitted:",bytes,u8" bytes\tElapsed Time:",elapsed);
	else
		println("Transmitted:",bytes,u8" bytes\tElapsed Time:",elapsed,"\tMB/sec:",static_cast<double>(bytes)/elapsed/1048576.0);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}