#pragma once

namespace fast_io
{

/*
Referenced from man on Linux
*/

struct posix_file_status
{
	std::common_type_t<std::uint64_t,std::size_t> dev;     // ID of device containing file
	std::common_type_t<std::uint64_t,std::size_t> ino;     // Inode number
	perms perm;
	file_type type;
	std::common_type_t<std::uint64_t,std::size_t> nlink;   // Number of hard links
	std::common_type_t<std::uint64_t,std::size_t> uid;     // User ID of owner
	std::common_type_t<std::uint64_t,std::size_t> gid;     // Group ID of owner
	std::common_type_t<std::uint64_t,std::size_t> rdev;    // Device ID (if special file)
	std::common_type_t<std::int64_t,std::ptrdiff_t> size;    // Total size, in bytes 
	std::common_type_t<std::uint64_t,std::size_t> blksize; // Block size for filesystem I/O
	std::common_type_t<std::uint64_t,std::size_t> blocks;  // Number of 512B blocks allocated

	/* Since Linux 2.6, the kernel supports nanosecond
		precision for the following timestamp fields.
		For the details before Linux 2.6, see NOTES. */
	struct timespec atim;					// Time of last access
	struct timespec mtim;					// Time of last modification
	struct timespec ctim;					// Time of last status change
	std::common_type_t<std::uint64_t,std::size_t> flags;	// user defined flags for file
	std::common_type_t<std::uint64_t,std::size_t> gen;	// file generation number
};

inline constexpr void print_define(fast_io::buffer_output_stream auto bop,fast_io::posix_file_status const& status)
{
	print_freestanding(bop,"dev:",status.dev,"\nino:",status.ino,"\nperm:",status.perm,"\ntype:",status.type,
	"\nnlink:",status.nlink,"\nuid:",status.uid,"\ngid:",status.gid,"\nrdev:",status.rdev,"\nsize:",status.size,"\nblksize:",
	status.blksize,"\nblocks:",status.blocks,"\natim:",status.atim,"\nmtim:",status.mtim,"\nctim:",status.ctim,"\nflags:",status.flags,"\ngen:",status.gen);
}
}