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
	std::string d_name;
};

namespace win32::nt::details
{

inline nt_dirent* set_nt_dirent(nt_dirent* entry,bool start)
{
	io_status_block block{};
	std::array<std::byte,0x4000> buffer;
	win32::nt::dir_information d_info{buffer.data()};
	auto status{nt_query_directory_file(entry->d_handle,nullptr,nullptr,nullptr,
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
	std::u16string_view u16_view(ful_dir_info->FileName,ful_dir_info->FileNameLength/sizeof(char16_t));
	fast_io::ostring_ref ostrf{entry->d_name};
	obuffer_set_curr(ostrf,obuffer_begin(ostrf));
	print_freestanding(ostrf,code_cvt(u16_view));
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

inline nt_dirent* set_nt_dirent_first(nt_dirent* entry)
{
	return set_nt_dirent(entry,true);
}

inline nt_dirent* nt_dirent_next(nt_dirent* entry)
{
	return set_nt_dirent(entry,false);
}

}

struct nt_directory_entry
{
	nt_dirent* entry{};
	constexpr operator win32_io_observer() const noexcept
	{
		if(entry==nullptr)
			return {};
		return {entry->d_handle};
	}
};

inline cstring_view filename(nt_directory_entry pioe) noexcept
{
	return pioe.entry->d_name;
}

inline constexpr std::common_type_t<std::size_t,std::uint64_t> inode(nt_directory_entry)  noexcept
{
	return 0;
}

inline constexpr file_type type(nt_directory_entry pioe) noexcept
{
	return pioe.entry->d_type;
}

struct nt_directory_iterator
{
	nt_dirent* entry{};
};

inline nt_directory_entry operator*(nt_directory_iterator pdit) noexcept
{
	return {pdit.entry};
}

inline nt_directory_iterator& operator++(nt_directory_iterator& pdit)
{
	pdit.entry=win32::nt::details::nt_dirent_next(pdit.entry);
	return pdit;
}

inline constexpr bool operator==(std::default_sentinel_t, nt_directory_iterator b) noexcept
{
	return b.entry==nullptr;
}
inline constexpr bool operator==(nt_directory_iterator b, std::default_sentinel_t other) noexcept
{
	return other==b;
}
inline constexpr bool operator!=(std::default_sentinel_t other, nt_directory_iterator b) noexcept
{
	return !(other==b);
}
inline constexpr bool operator!=(nt_directory_iterator b, std::default_sentinel_t other) noexcept
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

struct nt_directory_generator
{
	std::unique_ptr<nt_dirent> entry;
};

inline nt_directory_iterator begin(nt_directory_generator const& pdg)
{
	return {win32::nt::details::set_nt_dirent_first(pdg.entry.get())};
}

inline std::default_sentinel_t end(nt_directory_generator const&) noexcept
{
	return {};
}

inline nt_directory_generator current(win32_io_observer wiob)
{
	return {details::make_nt_dirent(wiob.handle)};
}

struct nt_recursive_directory_iterator
{
	void* root_handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	nt_dirent* entry{};
	std::vector<win32_file> stack;
	nt_recursive_directory_iterator()=default;
	nt_recursive_directory_iterator(void* root_han,nt_dirent* ent):root_handle(root_han),entry(ent){}
	nt_recursive_directory_iterator(nt_recursive_directory_iterator const&)=delete;
	nt_recursive_directory_iterator& operator=(nt_recursive_directory_iterator const&)=delete;
	nt_recursive_directory_iterator(nt_recursive_directory_iterator&&) noexcept=default;
	nt_recursive_directory_iterator& operator=(nt_recursive_directory_iterator&&) noexcept=default;
};

struct nt_recursive_directory_generator
{
	void* root_handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	std::unique_ptr<nt_dirent> entry;
};

inline std::size_t depth(nt_recursive_directory_iterator const& prdit) noexcept
{
	return prdit.stack.size();
}

inline nt_recursive_directory_iterator& operator++(nt_recursive_directory_iterator& prdit)
{
	for(;;)
	{
		if(prdit.stack.empty())
		{
			prdit.entry->d_handle=prdit.root_handle;
			if(!(prdit.entry=win32::nt::details::nt_dirent_next(prdit.entry)))
				return prdit;
		}
		else
		{
			prdit.entry->d_handle=prdit.stack.back().handle;
			auto entry=win32::nt::details::nt_dirent_next(prdit.entry);
			if(entry==nullptr)
			{
				prdit.stack.pop_back();
				continue;
			}
			prdit.entry=entry;
		}
		if(prdit.entry->d_type==file_type::directory)
		{
			cstring_view name{prdit.entry->d_name};
			if((name.size()==1&&name.front()==u8'.')||(name.size()==2&&name.front()==u8'.'&&name[1]==u8'.'))
				continue;
			prdit.stack.emplace_back(at,win32_io_observer{prdit.stack.empty()?prdit.root_handle:prdit.stack.back().handle},name,
				open_interface<open_mode::in|open_mode::directory|open_mode::large_file|open_mode::binary>);
		}
		return prdit;
	}
	return prdit;
}

inline void pop(nt_recursive_directory_iterator& prdit)
{
	if(prdit.stack.empty())
		prdit.entry=nullptr;
	else
	{
		prdit.stack.pop_back();
		++prdit;
	}
}

inline nt_recursive_directory_iterator begin(nt_recursive_directory_generator const& prg) noexcept
{
	nt_recursive_directory_iterator prdit{prg.root_handle,prg.entry.get()};
	prdit.entry->d_handle=prg.root_handle;
	prdit.entry=win32::nt::details::set_nt_dirent_first(prdit.entry);
	if(prdit.entry&&prdit.entry->d_type==file_type::directory)
	{
		cstring_view name{prdit.entry->d_name};
		if((name.size()==1&&name.front()==u8'.')||(name.size()==2&&name.front()==u8'.'&&name[1]==u8'.'))
			++prdit;
		else
			prdit.stack.emplace_back(at,win32_io_observer{prdit.root_handle},name,
				open_interface<open_mode::in|open_mode::directory|open_mode::large_file|open_mode::binary>);
	}
	return prdit;
}

inline std::default_sentinel_t end(nt_recursive_directory_generator const&) noexcept
{
	return {};
}

inline nt_directory_entry operator*(nt_recursive_directory_iterator const& prdit) noexcept
{
	return {prdit.entry};
}

inline bool operator==(std::default_sentinel_t, nt_recursive_directory_iterator const& b) noexcept
{
	return b.stack.empty()&&b.entry == nullptr;
}

inline bool operator==(nt_recursive_directory_iterator const& b, std::default_sentinel_t sntnl) noexcept
{
	return sntnl==b;
}

inline bool operator!=(std::default_sentinel_t sntnl, nt_recursive_directory_iterator const& b) noexcept
{
	return !(sntnl==b);
}

inline bool operator!=(nt_recursive_directory_iterator const& b, std::default_sentinel_t sntnl) noexcept
{
	return sntnl!=b;
}

inline nt_recursive_directory_generator recursive(win32_io_observer wiob)
{
	return {wiob.handle,std::unique_ptr<nt_dirent>(new nt_dirent)};
}

using directory_entry = nt_directory_entry;

}