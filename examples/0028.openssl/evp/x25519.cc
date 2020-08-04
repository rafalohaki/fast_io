#include"../../../include/fast_io.h"
#include"../../../include/fast_io_driver/openssl_driver.h"

int main()
try
{
	using namespace fast_io::openssl;

	evp_pkey_ctx ctx(fast_io::native_interface,EVP_PKEY_X25519,nullptr);
	keygen_init(ctx);
	evp_pkey key{keygen(ctx)};
	println(private_key{key});
	println(public_key{key});
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}