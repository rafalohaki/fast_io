#include"../../../include/fast_io.h"
#include"../../../include/fast_io_driver/openssl_driver.h"

int main()
{
	fast_io::openssl::evp_pkey_ctx ctx(fast_io::native_interface,EVP_PKEY_X25519,nullptr);
	keygen_init(ctx);
	fast_io::openssl::evp_pkey key{keygen(ctx)};
	println(fast_io::openssl::private_key{key});
	println(fast_io::openssl::public_key{key});
}