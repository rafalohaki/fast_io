#include<fast_io.h>
#include<fast_io_crypto.h>

constexpr auto evaluate_sha512() noexcept
{
	std::array<char8_t,print_reserve_size(fast_io::io_reserve_type<char8_t,fast_io::sha512>)> array{};
	fast_io::sha512 sha;
	fast_io::hash_processor processor{sha};
	print(processor,"Hello");
	processor.do_final();
	print_reserve_define(fast_io::io_reserve_type<char8_t,fast_io::sha512>,array.data(),sha);
	return array;
}

int main()
{
	constexpr auto arr{evaluate_sha512()};
	print(fast_io::u8out(),std::u8string_view(arr.data(),arr.data()+arr.size()));
}