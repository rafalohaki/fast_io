#include"../../include/fast_io_device.h"
#include"../../include/fast_io.h"



namespace fast_io::details
{


inline int create_posix_temp_file()
{
	posix_file urandom("/dev/urandom",fast_io::open_interface<fast_io::open_mode::in|fast_io::open_mode::binary>);
	std::array<char,256> arr;
	ospan arr_osp(arr);
	print(arr_osp,"/tmp/tmp");
	int local_errno{};
	for(std::size_t i{};i!=128;++i)
	{
		std::array<char,512> buffer1;
		secure_clear_guard<char> guard(buffer1.data(),buffer1.size());

		std::ptrdiff_t readed {::read(urandom.fd,buffer1.data(),buffer1.size())};
		if(readed<0)[[unlikely]]
		{
			if(errno==EAGAIN||errno==EINTR)
				continue;
			throw_posix_error();
		}
		if(readed!=buffer1.size())[[unlikely]]
			continue;

		fast_io::sha256 sha;
		fast_io::hash_processor processor(sha);
		write(processor,buffer1.begin(),buffer1.end());
		processor.do_final();
		auto osp{arr_osp};
		print(osp,sha);
		put(osp,0);
/*
https://man7.org/linux/man-pages/man3/mkstemp.3.html
*/

#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		int fd{system_call<
#if defined(__x86_64__)
		257
#elif defined(__arm64__) || defined(__aarch64__)
		56
#endif
		,int>(AT_FDCWD,arr.data(),O_EXCL|O_RDWR|O_NOFOLLOW|O_APPEND|O_CREAT|O_NOATIME,S_IRUSR | S_IWUSR)};
		if(fd<0)
		{
			local_errno=-fd;
			if(fd==-EINTR||fd==-EEXIST)
				continue;
			throw_posix_error(-fd);
		}
#else
		int fd{::openat(AT_FDCWD,arr.data(),O_EXCL|O_RDWR|O_NOFOLLOW|O_APPEND|O_CREAT|O_NOATIME,0600)};

		if(fd<0)[[unlikely]]
		{
			local_errno=errno;
			if(errno==EINTR||errno==EEXIST)
				continue;
			throw_posix_error();
		}
#endif

/*

https://code.woboq.org/userspace/glibc/stdio-common/tmpfile.c.html


	It looks like a TOCTTOU vuln in glibc.

       Note that this relies on the Unix semantics that
         a file is not really removed until it is closed.  
      (void) __unlink (buf);

	unlink the filename before closing the file????

*/

		return fd;
	}
	throw_posix_error(local_errno);
	return -1;
}
}

int main()
{
	fast_io::posix_file pf{fast_io::details::create_posix_temp_file()};
	
}
