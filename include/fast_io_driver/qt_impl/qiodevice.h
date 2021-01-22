#pragma once
namespace fast_io
{

template<std::integral ch_type,typename T>
class basic_general_qdevice_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = T*;
	native_handle_type qdevice{};

	inline constexpr auto& native_handle() noexcept
	{
		return qdevice;
	}
	inline constexpr auto& native_handle() const noexcept
	{
		return qdevice;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{qdevice};
		qdevice=nullptr;
		return temp;
	}
};

namespace details
{

inline std::size_t qio_device_write_impl(QIODevice* __restrict qdevice,void const* data,std::size_t bytes)
{
	std::int64_t res{qdevice->write(reinterpret_cast<char const*>(data),static_cast<std::int64_t>(bytes))};
	if(res<0)
		throw_posix_error(EIO);
	return static_cast<std::size_t>(res);
}

inline std::size_t qio_device_read_impl(QIODevice* __restrict qdevice,void const* data,std::size_t bytes)
{
	std::int64_t res{qdevice->read(reinterpret_cast<char const*>(data),static_cast<std::int64_t>(bytes))};
	if(res<0)
		throw_posix_error(EIO);
	return static_cast<std::size_t>(res);
}


inline std::uintmax_t qio_device_seek_impl(QIODevice* __restrict qdevice,std::intmax_t offset,seekdir dir)
{
	if(dir==seekdir::beg)
		offset=static_cast<std::intmax_t>(qdevice->pos())+offset;
	else(dir==seekdir::end)
		offset=static_cast<std::intmax_t>(qdevice->size()-qdevice->pos())+offset;
	if(!qdevice->seek(static_cast<std::int64_t>(offset)))
		throw_posix_error(EIO);
	return static_cast<std::uintmax_t>(qdevice->pos());
}

}

template<std::integral ch_type,typename T,std::contiguous_access_iterator Iter>
inline Iter write(basic_general_qdevice_io_observer<ch_type,T> qiob,Iter begin,Iter end)
{
	return begin+details::qio_device_write_impl(qiob.qdevice,std::to_address(begin),
		(std::to_address(end)-std::to_address(begin))*sizeof(*begin))/sizeof(*begin);
}

template<std::integral ch_type,typename T,std::contiguous_access_iterator Iter>
inline Iter read(basic_general_qdevice_io_observer<ch_type,T> qiob,Iter begin,Iter end)
{
	return begin+details::qio_device_read_impl(qiob.qdevice,std::to_address(begin),
		(std::to_address(end)-std::to_address(begin))*sizeof(*begin))/sizeof(*begin);
}

template<std::integral ch_type,typename T>
requires (sizeof(ch_type)==1)
inline void try_unget(basic_general_qdevice_io_observer<ch_type,T> qiob,ch_type ch)
{
	qiob->qdevice->ungetChar(ch);
}

template<std::integral char_type,typename T>
inline std::uintmax_t seek(basic_general_qdevice_io_observer<ch_type,T> qiob,std::intmax_t offset=0,seekdir s=seekdir::cur)
{
	return qio_device_seek_impl(qiob->qdevice,offset,s);
}
template<std::integral char_type>
using basic_qiodevice_io_observer = basic_general_qdevice_io_observer<char_type,QIODevice>;
template<std::integral char_type>
using basic_qt_io_observer = basic_general_qdevice_io_observer<char_type,QFile>;


using qiodevice_io_observer = basic_qiodevice_io_observer<char>;
#ifndef __MSDOS__
using wqiodevice_io_observer = basic_qiodevice_io_observer<wchar_t>;
#endif
using u8qiodevice_io_observer = basic_qiodevice_io_observer<char8_t>;
using u16qiodevice_io_observer = basic_qiodevice_io_observer<char16_t>;
using u32qiodevice_io_observer = basic_qiodevice_io_observer<char32_t>;

using qt_io_observer = basic_qt_io_observer<char>;
#ifndef __MSDOS__
using wqt_io_observer = basic_qt_io_observer<wchar_t>;
#endif
using u8qt_io_observer = basic_qt_io_observer<char8_t>;
using u16qt_io_observer = basic_qt_io_observer<char16_t>;
using u32qt_io_observer = basic_qt_io_observer<char32_t>;


}
