#include"../../include/fast_io.h"

class gooo{};

int main()
{
	gooo ff;
	scan(ff);
	println(ff);
}

/*
D:\hg\fast_io\examples\0061.error_message>g++ -o wrong_type wrong_type.cc -Ofast -std=c++20 -s
In file included from ../../include/fast_io_core.h:44,
                 from ../../include/fast_io_freestanding.h:5,
                 from ../../include/fast_io_hosted.h:9,
                 from ../../include/fast_io.h:3,
                 from wrong_type.cc:1:
../../include/fast_io_core_impl/print_scan.h: In instantiation of 'constexpr auto fast_io::scan(input&&, Args&& ...) [with bool report_eof = false; input = fast_io::basic_c_io_observer_unlocked<char>&; Args = {gooo&}]':
../../include/fast_io_core_impl/print_scan.h:260:26:   required from 'constexpr auto fast_io::scan(input&&, Args&& ...) [with bool report_eof = false; input = fast_io::basic_c_io_observer<char>; Args = {gooo&}]'
../../include/fast_io.h:205:26:   required from 'constexpr auto scan(T&&, Args&& ...) [with bool report_eof = false; T = gooo&; Args = {}]'
wrong_type.cc:8:9:   required from here
../../include/fast_io_core_impl/print_scan.h:270:18: error: static assertion failed:

        The type is not defined for scanning. Please consider defining as with scan_define or space_scan_define.

  270 |    static_assert(!character_input_stream<input>,
      |                  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
../../include/fast_io_core_impl/print_scan.h: In instantiation of 'constexpr void fast_io::details::print_fallback(output&, Args&& ...) [with bool line = true; output = fast_io::basic_c_io_observer_unlocked<char>; Args = {gooo&}]':
../../include/fast_io_core_impl/print_scan.h:558:32:   required from 'constexpr void fast_io::println(output&&, Args&& ...) [with output = fast_io::basic_c_io_observer_unlocked<char>&; Args = {gooo&}]'
../../include/fast_io_core_impl/print_scan.h:512:10:   required from 'constexpr void fast_io::println(output&&, Args&& ...) [with output = fast_io::basic_c_io_observer<char>; Args = {gooo&}]'
../../include/fast_io.h:102:19:   required from 'constexpr void println(T&&, Args&& ...) [with T = gooo&; Args = {}]'
wrong_type.cc:9:12:   required from here
../../include/fast_io_core_impl/print_scan.h:451:18: error: static assertion failed:

        This type is not defined for printing. Please consider define it with print_define or print_reserve_define.
        See wiki page https://github.com/expnkx/fast_io/wiki/0018.-custom-type

  451 |    static_assert(!buffer_output_stream<internal_buffer_type>,
      |                  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/