#pragma once

namespace fast_io::openssl
{

using openssl_context_observer = details::observer<OPENSSL_CTX*>;

class openssl_context:public openssl_context_observer
{
public:
	openssl_context():openssl_context_observer(OPENSSL_CTX_new())
	{
		if(this->native_handle()==nullptr)
			throw_openssl_error();
	}
	openssl_context(openssl_context const&) = delete;
	openssl_context& operator=(openssl_context const&) = delete;
	constexpr openssl_context(openssl_context&& bmv) noexcept:openssl_context_observer(bmv.native_handle())
	{
		bmv.native_handle()=nullptr;
	}
	openssl_context& operator=(openssl_context&& bmv) noexcept
	{
		if(bmv.native_handle()==this->native_handle())
			return *this;
		if(this->native_handle())[[likely]]
			OPENSSL_CTX_free(this->native_handle());
		this->native_handle()=bmv.native_handle();
		bmv.native_handle()=nullptr;
		return *this;
	}
	~openssl_context()
	{
		if(this->native_handle())[[likely]]
			OPENSSL_CTX_free(this->native_handle());
	}
};

};