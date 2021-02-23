#pragma once

namespace fast_io
{

/*
https://github.com/reactos/reactos/blob/7ffb6a09c370ef51de554a1ce913f871d72b76ca/dll/win32/kernel32/client/file/find.c#L647

HANDLE
WINAPI
FindFirstFileExW(IN LPCWSTR lpFileName,
                 IN FINDEX_INFO_LEVELS fInfoLevelId,
                 OUT LPVOID lpFindFileData,
                 IN FINDEX_SEARCH_OPS fSearchOp,
                 LPVOID lpSearchFilter,
                 IN DWORD dwAdditionalFlags)

*/

struct nt_dirent
{
	void* d_handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	file_type d_type{};
	std::array<wchar_t,0x2001> native_d_name_array{};
	std::size_t native_d_name_size{};
	std::array<char8_t,0x8004> d_name_array{};
	std::size_t d_name_size{};
	inline constexpr wcstring_view native_d_name() const noexcept
	{
		return wcstring_view(null_terminated,native_d_name_array.data(),native_d_name_size);
	}
	inline constexpr u8cstring_view d_name() const noexcept
	{
		return u8cstring_view(null_terminated,d_name_array.data(),d_name_size);
	}
};

namespace win32::nt::details
{

template<nt_family family>
inline nt_dirent* set_nt_dirent(nt_dirent* entry,bool start)
{
	io_status_block block{};
	std::array<std::byte,0x4000> buffer;
	win32::nt::dir_information d_info{buffer.data()};
	auto status{nt_query_directory_file<family==nt_family::zw>(entry->d_handle,nullptr,nullptr,nullptr,
	std::addressof(block),d_info.DirInfo,
	buffer.size(),file_information_class::FileFullDirectoryInformation,
	true,nullptr,start)};
	if(status)
	{
		if(status==2147483654)[[likely]]
			return nullptr;
		throw_nt_error(status);
	}
	auto ful_dir_info{d_info.FullDirInfo};
	entry->native_d_name_size=ful_dir_info->FileNameLength/sizeof(wchar_t);
	if(ful_dir_info->FileNameLength)
		memcpy(entry->native_d_name_array.data(),ful_dir_info->FileName,ful_dir_info->FileNameLength);
	entry->native_d_name_array[entry->native_d_name_size]=0;

	using char16_may_alias_pointer
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char16_t const*;
	char16_may_alias_pointer enstart{reinterpret_cast<char16_may_alias_pointer>(entry->native_d_name_array.data())};
	char16_may_alias_pointer enend{enstart+entry->native_d_name_size};
	entry->d_name_size=static_cast<std::size_t>(::fast_io::details::codecvt::general_code_cvt_full(enstart,enend,entry->d_name_array.data())-entry->d_name_array.data());
	entry->d_name_array[entry->d_name_size]=0;
/*
Referenced from win32 port dirent.h
https://github.com/win32ports/dirent_h/blob/5a40afce928f1780058f44e0dda37553c662a8a7/dirent.h#L249
		if (((w32fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT) && __islink(wname, buffer))
			data->entries[data->index].d_type = DT_LNK;
		else if ((w32fd.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) == FILE_ATTRIBUTE_DEVICE)
			data->entries[data->index].d_type = DT_CHR;
		else if ((w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			data->entries[data->index].d_type = DT_DIR;
		else
			data->entries[data->index].d_type = DT_REG;
*/

	std::uint32_t attribute{ful_dir_info->FileAttributes};
	if(attribute&0x400)
		entry->d_type=file_type::symlink;
	else if(attribute&0x40)
		entry->d_type=file_type::character;
	else if(attribute&0x10)
		entry->d_type=file_type::directory;
	else if(attribute&0x1000)
		entry->d_type=file_type::remote;
	else
		entry->d_type=file_type::regular;
	return entry;
}

template<nt_family family>
inline nt_dirent* set_nt_dirent_first(nt_dirent* entry)
{
	return set_nt_dirent<family>(entry,true);
}

template<nt_family family>
inline nt_dirent* nt_dirent_next(nt_dirent* entry)
{
	return set_nt_dirent<family>(entry,false);
}

}

struct nt_directory_entry
{
	using native_char_type = wchar_t;
	using char_type = char8_t;
	nt_dirent* entry{};
	template<nt_family family>
	explicit constexpr operator basic_nt_family_io_observer<family,char>() const noexcept
	{
		return {entry->d_handle};
	}
	explicit constexpr operator win32_io_observer() const noexcept
	{
		return {entry->d_handle};
	}
};

inline constexpr nt_at_entry at(nt_directory_entry ndet) noexcept
{
	return nt_at_entry{ndet.entry->d_handle};
}

inline constexpr wcstring_view native_filename(nt_directory_entry pioe) noexcept
{
	return pioe.entry->native_d_name();
}

inline constexpr nt_fs_dirent fs_dirent(nt_directory_entry pioe) noexcept
{
	return nt_fs_dirent{pioe.entry->d_handle,pioe.entry->native_d_name()};
}

inline constexpr u8cstring_view filename(nt_directory_entry pioe) noexcept
{
	return pioe.entry->d_name();
}

inline constexpr std::uintmax_t inode(nt_directory_entry)  noexcept
{
	return 0;
}

inline constexpr file_type type(nt_directory_entry pioe) noexcept
{
	return pioe.entry->d_type;
}

template<nt_family family>
struct nt_family_directory_iterator
{
	nt_dirent* entry{};
};

template<nt_family family>
inline nt_directory_entry operator*(nt_family_directory_iterator<family> pdit) noexcept
{
	return {pdit.entry};
}

template<nt_family family>
inline nt_family_directory_iterator<family>& operator++(nt_family_directory_iterator<family>& pdit)
{
	pdit.entry=win32::nt::details::nt_dirent_next<family>(pdit.entry);
	return pdit;
}

template<nt_family family>
inline constexpr bool operator==(std::default_sentinel_t, nt_family_directory_iterator<family> b) noexcept
{
	return b.entry==nullptr;
}

template<nt_family family>
inline constexpr bool operator==(nt_family_directory_iterator<family> b, std::default_sentinel_t other) noexcept
{
	return other==b;
}

template<nt_family family>
inline constexpr bool operator!=(std::default_sentinel_t other,nt_family_directory_iterator<family> b) noexcept
{
	return !(other==b);
}

template<nt_family family>
inline constexpr bool operator!=(nt_family_directory_iterator<family> b, std::default_sentinel_t other) noexcept
{
	return !(other==b);
}

namespace details
{

inline std::unique_ptr<nt_dirent> make_nt_dirent(void* handle)
{
	std::unique_ptr<nt_dirent> ent(new nt_dirent);
	ent->d_handle=handle;
	return ent;
}

}

template<nt_family family>
struct nt_family_directory_generator
{
	std::unique_ptr<nt_dirent> entry;
};

template<nt_family family>
inline nt_family_directory_iterator<family> begin(nt_family_directory_generator<family> const& pdg)
{
	return {win32::nt::details::set_nt_dirent_first<family>(pdg.entry.get())};
}

template<nt_family family>
inline std::default_sentinel_t end(nt_family_directory_generator<family> const&) noexcept
{
	return {};
}

using nt_directory_generator = nt_family_directory_generator<nt_family::nt>;
using zw_directory_generator = nt_family_directory_generator<nt_family::zw>;

inline nt_directory_generator current(nt_at_entry nate)
{
	return {details::make_nt_dirent(nate.handle)};
}

inline zw_directory_generator current(zw_at_entry nate)
{
	return {details::make_nt_dirent(nate.handle)};
}

template<nt_family family>
struct nt_family_recursive_directory_iterator
{
	void* root_handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	nt_dirent* entry{};
	details::naive_vector<basic_nt_family_file<family,char>> stack;
	nt_family_recursive_directory_iterator()=default;
	nt_family_recursive_directory_iterator(void* root_han,nt_dirent* ent):root_handle(root_han),entry(ent){}
	nt_family_recursive_directory_iterator(nt_family_recursive_directory_iterator const&)=delete;
	nt_family_recursive_directory_iterator& operator=(nt_family_recursive_directory_iterator const&)=delete;
	nt_family_recursive_directory_iterator(nt_family_recursive_directory_iterator&&) noexcept=default;
	nt_family_recursive_directory_iterator& operator=(nt_family_recursive_directory_iterator&&) noexcept=default;
};

template<nt_family family>
struct nt_family_recursive_directory_generator
{
	void* root_handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	std::unique_ptr<nt_dirent> entry;
};

template<nt_family family>
inline std::size_t depth(nt_family_recursive_directory_iterator<family> const& prdit) noexcept
{
	return prdit.stack.size();
}

template<nt_family family>
inline nt_family_recursive_directory_iterator<family>& operator++(nt_family_recursive_directory_iterator<family>& prdit)
{
	for(;;)
	{
		if(prdit.stack.empty())
		{
			prdit.entry->d_handle=prdit.root_handle;
			if(!(prdit.entry=win32::nt::details::nt_dirent_next<family>(prdit.entry)))
				return prdit;
		}
		else
		{
			prdit.entry->d_handle=prdit.stack.back().handle;
			auto entry=win32::nt::details::nt_dirent_next<family>(prdit.entry);
			if(entry==nullptr)
			{
				prdit.stack.pop_back();
				continue;
			}
			prdit.entry=entry;
		}
		if(prdit.entry->d_type==file_type::directory)
		{
			wcstring_view name{prdit.entry->d_name()};
			if((name.size()==1&&name.front()==u'.')||(name.size()==2&&name.front()==u'.'&&name[1]==u'.'))
				continue;
			prdit.stack.emplace_back(nt_at_entry{prdit.stack.empty()?prdit.root_handle:prdit.stack.back().handle},name,
				open_mode::directory);
		}
		return prdit;
	}
	return prdit;
}

template<nt_family family>
inline void pop(nt_family_recursive_directory_iterator<family>& prdit)
{
	if(prdit.stack.empty())
		prdit.entry=nullptr;
	else
	{
		prdit.stack.pop_back();
		++prdit;
	}
}

template<nt_family family>
inline nt_family_recursive_directory_iterator<family> begin(nt_family_recursive_directory_iterator<family> const& prg) noexcept
{
	nt_family_recursive_directory_iterator<family> prdit{prg.root_handle,prg.entry.get()};
	prdit.entry->d_handle=prg.root_handle;
	prdit.entry=win32::nt::details::set_nt_dirent_first<family>(prdit.entry);
	if(prdit.entry&&prdit.entry->d_type==file_type::directory)
	{
		wcstring_view name{prdit.entry->d_name()};
		if((name.size()==1&&name.front()==u'.')||(name.size()==2&&name.front()==u'.'&&name[1]==u'.'))
			++prdit;
		else
			prdit.stack.emplace_back(nt_at_entry{prdit.root_handle},name,
				open_mode::directory);
	}
	return prdit;
}

template<nt_family family>
inline std::default_sentinel_t end(nt_family_recursive_directory_generator<family> const&) noexcept
{
	return {};
}

template<nt_family family>
inline nt_directory_entry operator*(nt_family_recursive_directory_iterator<family> const& prdit) noexcept
{
	return {prdit.entry};
}

template<nt_family family>
inline bool operator==(std::default_sentinel_t, nt_family_recursive_directory_iterator<family> const& b) noexcept
{
	return b.stack.empty()&&b.entry == nullptr;
}

template<nt_family family>
inline bool operator==(nt_family_recursive_directory_iterator<family> const& b, std::default_sentinel_t sntnl) noexcept
{
	return sntnl==b;
}

template<nt_family family>
inline bool operator!=(std::default_sentinel_t sntnl, nt_family_recursive_directory_iterator<family> const& b) noexcept
{
	return !(sntnl==b);
}

template<nt_family family>
inline bool operator!=(nt_family_recursive_directory_iterator<family> const& b, std::default_sentinel_t sntnl) noexcept
{
	return sntnl!=b;
}

using nt_recursive_directory_generator = nt_family_recursive_directory_generator<nt_family::nt>;
using zw_recursive_directory_generator = nt_family_recursive_directory_generator<nt_family::zw>;

inline nt_recursive_directory_generator recursive(nt_at_entry nate)
{
	return {nate.handle,std::unique_ptr<nt_dirent>(new nt_dirent)};
}

inline zw_recursive_directory_generator recursive(zw_at_entry zate)
{
	return {zate.handle,std::unique_ptr<nt_dirent>(new nt_dirent)};
}

using directory_entry = nt_directory_entry;

}