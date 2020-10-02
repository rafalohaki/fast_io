#pragma once

namespace fast_io
{

template<stream T,open_mode interface_mode>
class basic_file_wrapper:public T
{
public:
	using char_type = typename T::char_type;
	using native_handle_type = typename T::native_handle_type;
	constexpr basic_file_wrapper()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_file_wrapper(native_hd hd):T(hd){}
	template<open_mode om,perms pm>
	constexpr basic_file_wrapper(cstring_view file,open_interface_t<om>,perms_interface_t<pm>):
		T(file,open_interface<om|interface_mode>,perms_interface<pm>){}
	template<open_mode om>
	constexpr basic_file_wrapper(cstring_view file,open_interface_t<om>):
		T(file,open_interface<om|interface_mode>){}
	template<perms pm>
	constexpr basic_file_wrapper(cstring_view file,perms_interface_t<pm>):
		T(file,open_interface<interface_mode>,perms_interface<pm>){}
	constexpr basic_file_wrapper(cstring_view file):
		T(file,open_interface<interface_mode>){}
	constexpr basic_file_wrapper(cstring_view file,open_mode m,perms p=static_cast<perms>(436)):
		T(file,m|interface_mode,p){}
	constexpr basic_file_wrapper(cstring_view file,perms pm):T(file,open_interface<interface_mode>,pm){}
	constexpr basic_file_wrapper(io_temp_t):T(io_temp){}


	template<open_mode om,perms pm>
	constexpr basic_file_wrapper(native_at_entry nate,cstring_view file,open_interface_t<om>,perms_interface_t<pm>):
		T(nate,file,open_interface<om|interface_mode>,perms_interface<pm>){}
	template<open_mode om>
	constexpr basic_file_wrapper(native_at_entry nate,cstring_view file,open_interface_t<om>):
		T(nate,file,open_interface<om|interface_mode>){}
	template<perms pm>
	constexpr basic_file_wrapper(native_at_entry nate,cstring_view file,perms_interface_t<pm>):
		T(nate,file,open_interface<interface_mode>,perms_interface<pm>){}
	constexpr basic_file_wrapper(native_at_entry nate,cstring_view file):
		T(nate,file,open_interface<interface_mode>){}
	constexpr basic_file_wrapper(native_at_entry nate,cstring_view file,open_mode m,perms p=static_cast<perms>(436)):
		T(nate,file,m|interface_mode,p){}
	constexpr basic_file_wrapper(native_at_entry nate,cstring_view file,perms pm):
		T(nate,file,open_interface<interface_mode>,pm){}
};

template<input_stream T>
using input_file_wrapper = basic_file_wrapper<T,open_mode::in|open_mode::binary>;
template<output_stream T>
using output_file_wrapper = basic_file_wrapper<T,open_mode::out|open_mode::binary>;
template<io_stream T>
using io_file_wrapper = basic_file_wrapper<T,open_mode::in|open_mode::out|open_mode::binary>;

template<stream T>
using io_dir_file_wrapper = basic_file_wrapper<T,open_mode::in|open_mode::directory|open_mode::large_file|open_mode::binary>;

template<stream T,open_mode interface_mode>
class basic_wrapper:public T
{
public:
	using char_type = typename T::char_type;
	using native_handle_type = typename T::native_handle_type;
	template<open_mode om>
	constexpr basic_wrapper(open_interface_t<om>):T(open_interface<om|interface_mode>){}
	constexpr basic_wrapper():T(open_interface<interface_mode>){}
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_wrapper(open_mode m):T(m|interface_mode){}
};

template<input_stream T>
using input_wrapper = basic_wrapper<T,open_mode::in|open_mode::binary>;
template<output_stream T>
using output_wrapper = basic_wrapper<T,open_mode::out|open_mode::binary>;
template<io_stream T>
using io_wrapper = basic_wrapper<T,open_mode::in|open_mode::out|open_mode::binary>;


}
