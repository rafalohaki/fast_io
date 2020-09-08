#pragma once

namespace fast_io
{

inline void remove_at(posix_io_observer dir_iob,cstring_view relative_path)
{
	if(::unlinkat(dir_iob.fd,relative_path.data(),0)<0)
		throw_posix_error();
}

inline void remove_directory_at(posix_io_observer dir_iob,cstring_view relative_path)
{
	if(::unlinkat(dir_iob.fd,relative_path.data(),AT_REMOVEDIR)<0)
		throw_posix_error();
}

inline void rename_at(posix_io_observer old_dir_iob,cstring_view old_relative_path,
			posix_io_observer new_dir_iob,cstring_view new_relative_path)
{
	if(::renameat(old_dir_iob.fd,old_relative_path.data(),new_dir_iob.fd,new_relative_path.data())<0)
		throw_posix_error();
}

inline void create_symlink_at(cstring_view old_relative_path,posix_io_observer dir_iob,cstring_view new_relative_path)
{
	if(::symlinkat(old_relative_path.data(),dir_iob.fd,new_relative_path.data())<0)
		throw_posix_error();
}

}