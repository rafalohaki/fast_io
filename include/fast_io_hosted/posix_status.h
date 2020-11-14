#pragma once

namespace fast_io
{

/*
Referenced from man on Linux
*/

struct posix_file_status
{
	std::uintmax_t dev;     // ID of device containing file
	std::uintmax_t ino;     // Inode number
	perms perm;
	file_type type;
	std::uintmax_t nlink;   // Number of hard links
	std::uintmax_t uid;     // User ID of owner
	std::uintmax_t gid;     // Group ID of owner
	std::uintmax_t rdev;    // Device ID (if special file)
	std::uintmax_t size;    // Total size, in bytes 
	std::uintmax_t blksize; // Block size for filesystem I/O
	std::uintmax_t blocks;  // Number of 512B blocks allocated

	/* Since Linux 2.6, the kernel supports nanosecond
		precision for the following timestamp fields.
		For the details before Linux 2.6, see NOTES. */
	struct timespec atim;					// Time of last access
	struct timespec mtim;					// Time of last modification
	struct timespec ctim;					// Time of last status change
	std::uintmax_t flags;	// user defined flags for file
	std::uintmax_t gen;	// file generation number
};

#if 0

/*
Though i am not going to use this code any more, this can be reported to GCC folks to make them realize their inliner does not do the right thing by default.
*/

inline constexpr void print_define(fast_io::buffer_output_stream auto bop,fast_io::posix_file_status const& status)
{
	print_freestanding(bop,"dev:",status.dev,"\nino:",status.ino,"\nperm:",status.perm,"\ntype:",status.type,
	"\nnlink:",status.nlink,"\nuid:",status.uid,"\ngid:",status.gid,"\nrdev:",status.rdev,"\nsize:",status.size,"\nblksize:",
	status.blksize,"\nblocks:",status.blocks,"\natim:",status.atim,"\nmtim:",status.mtim,"\nctim:",status.ctim,"\nflags:",status.flags,"\ngen:",status.gen);
}
#endif
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,fast_io::posix_file_status>)
{
	constexpr std::size_t res{
	print_reserve_size(io_reserve_type<char_type,std::uintmax_t>)*11+
	sizeof(u8"dev:")+sizeof(u8"\nino:")+sizeof(u8"\nperm:")+sizeof(u8"\ntype:")+
	sizeof(u8"\nnlink:")+sizeof(u8"\nuid:")+sizeof(u8"\ngid:")+sizeof(u8"\nrdev:")+
	sizeof(u8"\nsize:")+sizeof(u8"\nblksize:")+sizeof(u8"\nblocks:")+sizeof(u8"\natim:")+
	sizeof(u8"\nmtim:")+sizeof(u8"\nctim:")+sizeof(u8"\nflags:")+sizeof(u8"\ngen:")-16+
	print_reserve_size(io_reserve_type<char_type,perms>)+
	print_reserve_size(io_reserve_type<char_type,file_type>)+
	print_reserve_size(io_reserve_type<char_type,struct timespec>)*3};
	return res;
}

namespace details
{
template<std::integral char_type,std::random_access_iterator raiter>
inline constexpr raiter print_file_status_impl(raiter iter,fast_io::posix_file_status const& status)
{
	iter=details::copy_string_literal(u8"dev:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.dev);
	iter=details::copy_string_literal(u8"\nino:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.ino);
	iter=details::copy_string_literal(u8"\nperm:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::common_type_t<perms>>,iter,status.perm);
	iter=details::copy_string_literal(u8"\ntype:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::common_type_t<file_type>>,iter,status.type);
	iter=details::copy_string_literal(u8"\nnlink:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.nlink);
	iter=details::copy_string_literal(u8"\nuid:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.uid);
	iter=details::copy_string_literal(u8"\ngid:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.gid);
	iter=details::copy_string_literal(u8"\nrdev:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.rdev);
	iter=details::copy_string_literal(u8"\nsize:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.size);
	iter=details::copy_string_literal(u8"\nblksize:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.blksize);
	iter=details::copy_string_literal(u8"\nblocks:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.blocks);


	iter=details::copy_string_literal(u8"\natim:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,struct timespec>,iter,status.atim);
	iter=details::copy_string_literal(u8"\nmtim:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,struct timespec>,iter,status.mtim);
	iter=details::copy_string_literal(u8"\nctim:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,struct timespec>,iter,status.ctim);

	iter=details::copy_string_literal(u8"\nflags:",iter);
	iter=print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.flags);
	iter=details::copy_string_literal(u8"\ngen:",iter);
	return print_reserve_define(io_reserve_type<char_type,std::uintmax_t>,iter,status.gen);
}
}

template<std::integral char_type,std::random_access_iterator raiter>
inline constexpr raiter print_reserve_define(io_reserve_type_t<char_type,fast_io::posix_file_status>,raiter iter,fast_io::posix_file_status const& status)
{
	return details::print_file_status_impl<char_type>(iter,status);
}

}