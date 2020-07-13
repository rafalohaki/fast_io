#include"../../include/fast_io.h"

class not_a_character_stream
{
public:
	using char_type = char;
};

template<std::contiguous_iterator Iter>
constexpr Iter read(not_a_character_stream,Iter b,Iter)
{
	return b;
}

int main()
{
	int v;
	not_a_character_stream q;
	fast_io::scan(q,v);
}

/*
D:\hg\fast_io\examples\0061.error_message>g++ -o wrong_input_device wrong_input_device.cc -Ofast -std=c++20 -s
In file included from ../../include/fast_io_core.h:44,
                 from ../../include/fast_io_freestanding.h:5,
                 from ../../include/fast_io_hosted.h:9,
                 from ../../include/fast_io.h:3,
                 from wrong_input_device.cc:1:
../../include/fast_io_core_impl/print_scan.h: In instantiation of 'constexpr auto fast_io::scan(input&&, Args&& ...) [with bool report_eof = false; input = not_a_character_stream&; Args = {int&}]':
wrong_input_device.cc:19:19:   required from here
../../include/fast_io_core_impl/print_scan.h:274:18: error: static assertion failed:
        The input stream type is not a character_input_stream.

  274 |    static_assert(character_input_stream<input>,
      |                  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
../../include/fast_io_core_impl/print_scan.h:274:18: note: constraints not satisfied
In file included from ../../include/fast_io_core.h:5,
                 from ../../include/fast_io_freestanding.h:5,
                 from ../../include/fast_io_hosted.h:9,
                 from ../../include/fast_io.h:3,
                 from wrong_input_device.cc:1:
../../include/fast_io_concept.h:60:9:   required for the satisfaction of 'character_input_stream_impl<T>' [with T = not_a_character_stream&]
../../include/fast_io_concept.h:60:39:   in requirements with 'T&& in' [with T = not_a_character_stream&]
../../include/fast_io_concept.h:62:12: note: the required expression 'igenerator(in)' is invalid
   62 |  igenerator(in);
      |  ~~~~~~~~~~^~~~
cc1plus.exe: note: set '-fconcepts-diagnostics-depth=' to at least 2 for more detail


*/