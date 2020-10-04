#pragma once

namespace fast_io
{

class nt_error:public fast_io_error
{
public:
	std::uint32_t ntstatus{};
	explicit nt_error(std::uint32_t status):ntstatus(status){}
	inline constexpr std::uint32_t code() const noexcept
	{
		return ntstatus;
	}
	void report(error_reporter& report) const override
	{
		details::report_win32_error(report,win32::nt::rtl_nt_status_to_dos_error(ntstatus));
	}
};
[[noreturn]] inline void throw_nt_error(std::uint32_t err)
{
#ifdef __cpp_exceptions
	throw nt_error(err);
#else
	fast_terminate();
#endif
}
}