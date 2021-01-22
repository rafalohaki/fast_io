#pragma once

namespace fast_io
{

namespace details
{

inline constexpr int to_qt_open_mode(open_mode mode) noexcept
{
	int res{};
	if((mode&open_mode::in)==open_mode::in)
		res|=QIODevice::ReadOnly;
	if((mode&open_mode::out)==open_mode::out)
		res|=QIODevice::WriteOnly;
	if((mode&open_mode::app)==open_mode::app)
		res|=QIODevice::Append;
	if((mode&open_mode::trunc)==open_mode::trunc)
		res|=QIODevice::Truncate;
	if((mode&open_mode::text)==open_mode::text)
		res|=QIODevice::Text;
	if((mode&open_mode::excl)==open_mode::excl)
		res|=QIODevice::NewOnly;
	if((mode&open_mode::directory)==open_mode::directory)
	{
		if((mode&open_mode::in)!=open_mode::in)
			res|=QIODevice::ReadOnly;
	}
	return res;
}

}

template<std::integral ch_type>
class basic_qt_file:public basic_qt_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = typename basic_qt_io_observer<ch_type>::native_handle_type;
	constexpr basic_qt_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_qt_file(native_hd qdevice):basic_qt_io_observer<char_type>{qdevice}{}
	basic_qt_file(basic_c_io_handle<ch_type>&& cioh,open_mode mode)
	{
		basic_qt_file<ch_type> hd(new QFile);
		hd.qdevice->open(cioh.fp,static_cast<QIODevice::OpenModeFlag>(details::to_qt_open_mode(mode)),QFileDevice::AutoCloseHandle);
		cioh.release();
		this->qdevice=hd.release();
	}
	basic_qt_file(basic_c_io_handle_unlocked<ch_type>&& cioh,open_mode mode)
	{
		basic_qt_file<ch_type> hd(new QFile);
		hd.qdevice->open(cioh.fp,static_cast<QIODevice::OpenModeFlag>(details::to_qt_open_mode(mode)),QFileDevice::AutoCloseHandle);
		cioh.release();
		this->qdevice=hd.release();
	}
	basic_qt_file(basic_posix_io_handle<ch_type>&& pioh,open_mode mode)
		:basic_qt_file(basic_c_file_unlocked<ch_type>(std::move(pioh),mode),mode){}
#ifdef _WIN32
	basic_qt_file(basic_win32_io_handle<ch_type>&& wioh,open_mode mode):
		basic_qt_file(basic_posix_file<ch_type>(std::move(wioh),mode),mode)
	{}
	template<nt_family family>
	basic_qt_file(basic_nt_family_io_handle<family,ch_type>&& wioh,open_mode mode):
		basic_qt_file(basic_posix_file<ch_type>(std::move(wioh),mode),mode)
	{}
	basic_qt_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_qt_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_qt_file(native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_qt_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
#endif

	basic_qt_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_qt_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_qt_file(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_qt_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}

	basic_qt_file(basic_qt_file const&)=delete;
	basic_qt_file& operator=(basic_qt_file const&)=delete;
	
	void close()
	{
		if(this->qdevice)[[likely]]
		{
			this->qdevice->close();
			delete this->qdevice;
		}
	}

private:
	void close_nothrow() noexcept
	{
		if(this->qdevice)[[likely]]
		{
#ifdef __cpp_exceptions
			try
			{
#endif
				this->qdevice->close();
#ifdef __cpp_exceptions
			}
			catch(...)
			{
			}
#endif
			delete this->qdevice;
		}
	}
public:

	constexpr basic_qt_file(basic_qt_file&& other) noexcept:
		basic_qt_io_observer<ch_type>{other.release()}
	{}
	basic_qt_file& operator=(basic_qt_file&& other) noexcept
	{
		if(this->qdevice==other.device)
			return *this;
		close_nothrow();
		this->qdevice=other.release();
		return *this;
	}
	~basic_qt_file()
	{
		close_nothrow();
	}
};

using qt_file = basic_qt_file<char>;
#ifndef __MSDOS__
using wqt_file = basic_qt_file<wchar_t>;
#endif
using u8qt_file = basic_qt_file<char8_t>;
using u16qt_file = basic_qt_file<char16_t>;
using u32qt_file = basic_qt_file<char32_t>;

}