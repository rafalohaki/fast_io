#include<fast_io_legacy.h>
#include<fstream>

int main()
{
/*
This is an example to explain how fast_io's files work with each other, and how you could use fast_io with existing FILE* or fstream apis
*/

#ifdef _WIN32
	fast_io::nt_file nf("nt_file.txt",fast_io::open_mode::out);
	fast_io::posix_file pf(std::move(nf),fast_io::open_mode::out);
#else
	fast_io::posix_file pf("posix_file.txt",fast_io::open_mode::out);
#endif
	fast_io::c_file cf(std::move(pf),fast_io::open_mode::out);
	fast_io::filebuf_file fbf(std::move(cf),fast_io::open_mode::out);
	std::ofstream fout;
	*fout.rdbuf()=std::move(*fbf.fb);
	fast_io::filebuf_io_observer fiob{fout.rdbuf()};
	fast_io::posix_tzset();
	auto unix_ts{fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)};
	println(
	"Unix Timestamp:",unix_ts,"\n"
	"Universe Timestamp:",static_cast<fast_io::universe_timestamp>(unix_ts),"\n"
	"UTC:",utc(unix_ts),"\n",
	"Local:",local(unix_ts)," Timezone:",fast_io::timezone_name(),"\n",
#ifdef __clang__
	"LLVM clang ",__clang_version__,"\n"
#elif defined(__GNUC__)
	"gcc ",__GNUC__,"\n"
#elif defined(_MSC_VER)
	"Microsoft Visual C++ ",_MSC_VER,"\n"
#else
	"Unknown C++ compiler\n"
#endif
#if defined(__GLIBCXX__)
	"GCC libstdc++ ", __GLIBCXX__, "\n"
#elif defined(_LIBCPP_VERSION)
	"LLVM libc++ ", _LIBCPP_VERSION, "\n"
#elif defined(_MSVC_STL_UPDATE)
	"Microsoft Visual C++ STL ", _MSVC_STL_UPDATE, "\n"
#else
	"Unknown C++ standard library\n"
#endif
	"fstream.rdbuf():",fiob.fb,"\n"
	"FILE*:",static_cast<fast_io::c_io_observer>(fiob).fp,"\n"
	"fd:",static_cast<fast_io::posix_io_observer>(fiob).fd
#ifdef _WIN32
	,"\n"
	"win32 HANDLE:",static_cast<fast_io::win32_io_observer>(fiob).handle,"\n"
	"zw HANDLE:",static_cast<fast_io::zw_io_observer>(fiob).handle,"\n"
	"nt HANDLE:",static_cast<fast_io::nt_io_observer>(fiob).handle
#endif
);
}
/*
MinGW-w64 needs -lntdll
g++ -o construct_fstream_from_syscall construct_fstream_from_syscall.cc -Ofast -std=c++20 -s -flto -march=native -lntdll
*/