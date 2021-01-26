#pragma once

namespace fast_io
{

class nt_error:public std::exception
{
public:
	std::uint32_t ntstatus{};
	explicit nt_error(std::uint32_t status):ntstatus(status){}
	inline constexpr std::uint32_t code() const noexcept
	{
		return ntstatus;
	}
#if 0
	void report(error_reporter& report) const override
	{
		details::report_win32_error(report,win32::nt::rtl_nt_status_to_dos_error(ntstatus));
	}
#endif
};
[[noreturn]] inline void throw_nt_error([[maybe_unused]] std::uint32_t err)
{
#ifdef __cpp_exceptions
	throw nt_error(err);
#else
	fast_terminate();
#endif
}
}