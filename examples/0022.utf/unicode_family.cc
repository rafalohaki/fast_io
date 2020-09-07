#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"
#include"../../include/fast_io_freestanding_impl/transformers/unicode_family.h"

namespace fast_io{

template<input_stream input>
using basic_u32ibuf_unicode_family = basic_indirect_ibuffer_constructor_source_type<basic_ibuf<input>,vector_buffer<char32_t>,transforms::unicode_family>;

using iunicode_family_file = basic_u32ibuf_unicode_family<inative_file>;
}

int main()
{
	fast_io::iunicode_family_file ibf("w.txt");
	fast_io::obuf_utf8_file<char32_t> obf("result_utf8.txt");
	transmit(obf,ibf);
}
