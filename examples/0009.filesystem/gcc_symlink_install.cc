#include<string>
#include<fast_io.h>
#include<fast_io_device.h>
#include<string_view>
#include<vector>
#include<algorithm>
#include<ranges>

int main(int argc,char const** argv)
{
	if(argc<2)
	{
		perr("Usage: ",fast_io::mnp::chvw(argv[0])," <dir path>\n");
		return 1;
	}
	std::vector<std::u8string> names;
	fast_io::dir_file df(argv[1]);
	using namespace std::string_view_literals;
	for(auto const& ent:current(at(df)))
	{
		auto ftype{type(ent)};
		if(ftype==fast_io::file_type::regular||ftype==fast_io::file_type::symlink)
		{
			auto exts{extension(ent)};
			std::u8string_view view(exts.cbegin(),exts.cend());
			if(view==u8".py"sv)	//excluse .py like libstdc++.so.6.0.29-gdb.py
				continue;
			auto name{filename(ent)};
			names.emplace_back(name.cbegin(),name.cend());
		}
	}
	if(names.empty())
		return 0;
	std::ranges::sort(names);
	std::size_t const sz{names.size()-1};
	for(std::size_t i{};i!=sz;++i)
	{
		if(names[i+1].size()<=names[i].size())
			continue;
		if(!std::equal(names[i].cbegin(),names[i].cend(),names[i+1].cbegin()))
			continue;
		perrln(fast_io::u8out(),names[i],u8" -> ",names[i+1]);
		posix_unlinkat(at(df),names[i]);
		posix_symlinkat(names[i+1],at(df),names[i]);
	}
}
