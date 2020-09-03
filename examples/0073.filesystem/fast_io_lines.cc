#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

inline std::size_t count_line(std::string_view relative_path,std::string_view ext)
{
	fast_io::dir_file dfl(relative_path);
	std::size_t counts{};
	for(auto const & e :recursive(dfl))
	{
		if(type(e)!=fast_io::file_type::regular||extension(e)!=ext)
			continue;
		fast_io::ibuf_file ibf(fast_io::at,e,filename(e));
		for(auto const & ch : igenerator(ibf))
			if(ch==u8'\n')
				++counts;
	}
	print("lines in ",relative_path,":",counts," (extension:",ext,")\n");
	return counts;
}

int main()
{
	std::size_t sum{count_line("../../include",".h")};
	sum+=count_line("../../examples",".cc");
	sum+=count_line("../../benchmarks",".cc");
	println("total:",sum);
}