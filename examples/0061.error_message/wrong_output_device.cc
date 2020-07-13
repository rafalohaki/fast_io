#include"../../include/fast_io.h"

class not_a_character_stream
{
public:
};


int main()
{
	int v;
	not_a_character_stream q;
	fast_io::print(q,v);
}

/*

D:\hg\fast_io\examples\0061.error_message>g++ -o wrong_output_device wrong_output_device.cc -Ofast -std=c++20 -s
wrong_output_device.cc: In function 'int main()':
wrong_output_device.cc:13:20: error: use of function 'constexpr void fast_io::print(output&&, Args&& ...) [with output = not_a_character_stream&; Args = {int&}]' with unsatisfied constraints
   13 |  fast_io::print(q,v);
      |                    ^
In file included from ../../include/fast_io_core.h:44,
                 from ../../include/fast_io_freestanding.h:5,
                 from ../../include/fast_io_hosted.h:9,
                 from ../../include/fast_io.h:3,
                 from wrong_output_device.cc:1:
../../include/fast_io_core_impl/print_scan.h:477:23: note: declared here
  477 | inline constexpr void print(output &&out,Args&& ...args)
      |                       ^~~~~
../../include/fast_io_core_impl/print_scan.h:477:23: note: constraints not satisfied
In file included from ../../include/fast_io_core.h:5,
                 from ../../include/fast_io_freestanding.h:5,
                 from ../../include/fast_io_hosted.h:9,
                 from ../../include/fast_io.h:3,
                 from wrong_output_device.cc:1:
wrong_output_device.cc: In instantiation of 'constexpr void fast_io::print(output&&, Args&& ...) [with output = not_a_character_stream&; Args = {int&}]':
wrong_output_device.cc:13:20:   required from here
../../include/fast_io_concept.h:191:9:   required for the satisfaction of 'stream<T>' [with T = not_a_character_stream&]
../../include/fast_io_concept.h:200:9:   required for the satisfaction of 'output_stream<output>' [with output = not_a_character_stream&]
../../include/fast_io_concept.h:191:47: note: no operand of the disjunction is satisfied
  191 | concept stream = details::input_stream_impl<T>||details::output_stream_impl<T>;
      |                  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cc1plus.exe: note: set '-fconcepts-diagnostics-depth=' to at least 2 for more detail

*/