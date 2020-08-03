#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_driver/pulseaudio.h"

int main(int argc,char** argv)
try
{
	if(argc<2)
	{
		perr("Usage: ",fast_io::chvw(*argv)," <file name>\n");
		return 1;
	}
	constexpr pa_sample_spec ss
	{
		.format = PA_SAMPLE_S16LE,
		.rate = 44100,
		.channels = 2
	};
	fast_io::pulseaudio_simple_file pasf(fast_io::native_interface,nullptr, "my_server", PA_STREAM_PLAYBACK, nullptr, "playback", &ss, nullptr, nullptr);
	fast_io::ibuf_file ibf(argv[1]);
	transmit(pasf,ibf);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}

/*
g++ -o pulseaudio_simple pulseaudio_simple.cc -Ofast -std=c++20 -s -lpulse -lpulse-simple
*/