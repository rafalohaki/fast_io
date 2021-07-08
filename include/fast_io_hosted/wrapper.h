#pragma once

namespace fast_io
{

template<stream T,open_mode interface_mode>
struct basic_file_wrapper:public T
{
	using typename T::char_type;
	using typename T::native_handle_type;
	using typename T::file_factory_type;
	constexpr basic_file_wrapper()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_file_wrapper(native_hd hd):T(hd){}
	explicit constexpr basic_file_wrapper(file_factory_type&& factory):T(static_cast<file_factory_type&&>(factory)){}
	constexpr basic_file_wrapper(native_fs_dirent fsdirent,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(fsdirent,m|interface_mode,p){}

	constexpr basic_file_wrapper(cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(file,m|interface_mode,p){}
	constexpr basic_file_wrapper(native_at_entry nate,cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(nate,file,m|interface_mode,p){}

	constexpr basic_file_wrapper(wcstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(file,m|interface_mode,p){}
	constexpr basic_file_wrapper(native_at_entry nate,wcstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(nate,file,m|interface_mode,p){}

	constexpr basic_file_wrapper(u8cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(file,m|interface_mode,p){}
	constexpr basic_file_wrapper(native_at_entry nate,u8cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(nate,file,m|interface_mode,p){}

	constexpr basic_file_wrapper(u16cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(file,m|interface_mode,p){}
	constexpr basic_file_wrapper(native_at_entry nate,u16cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(nate,file,m|interface_mode,p){}

	constexpr basic_file_wrapper(u32cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(file,m|interface_mode,p){}
	constexpr basic_file_wrapper(native_at_entry nate,u32cstring_view file,open_mode m=interface_mode,perms p=static_cast<perms>(436)):
		T(nate,file,m|interface_mode,p){}
};

template<input_stream T>
using input_file_wrapper = basic_file_wrapper<T,open_mode::in>;
template<output_stream T>
using output_file_wrapper = basic_file_wrapper<T,open_mode::out>;

template<stream T>
using directory_file_wrapper = basic_file_wrapper<T,open_mode::directory>;


}
