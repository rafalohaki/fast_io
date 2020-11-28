#pragma once

namespace fast_io
{

struct nt_user_process_information
{
	void* hprocess{reinterpret_cast<void*>(-1)};
	void* hthread{reinterpret_cast<void*>(-1)};
};

namespace win32::nt::details
{
template<nt_family family>
inline void close_nt_user_process_information_not_null(nt_user_process_information* hnt_user_process_info) noexcept
{
	if(hnt_user_process_info->hthread!=reinterpret_cast<void*>(-1))[[likely]]
	{
		win32::nt::nt_close<family==nt_family::zw>(hnt_user_process_info->hthread);
		hnt_user_process_info->hthread=reinterpret_cast<void*>(-1);
	}
	if(hnt_user_process_info->hprocess!=reinterpret_cast<void*>(-1))[[likely]]
	{
		win32::nt::nt_close<family==nt_family::zw>(hnt_user_process_info->hprocess);
		hnt_user_process_info->hprocess=reinterpret_cast<void*>(-1);
	}
	delete hnt_user_process_info;
}
template<nt_family family>
inline void close_nt_user_process_information(nt_user_process_information* hnt_user_process_info) noexcept
{
	if(hnt_user_process_info)[[likely]]
		return details::close_nt_user_process_information_not_null<family>(hnt_user_process_info);
}
template<nt_family family>
inline std::uint32_t nt_wait_user_process_or_thread(void* hprocess_thread) noexcept
{
	return win32::nt::nt_wait_for_single_object<family>(hprocess_thread,false,nullptr);
}

template<nt_family family,bool throw_eh=false>
inline void nt_wait_and_close_user_process_or_thread(void*& handle) noexcept(!throw_eh)
{
	if(handle==reinterpret_cast<void*>(-1))
		return;
	auto status{nt_wait_user_process_or_thread<family>(handle)};
	auto status2{win32::nt::nt_close<family>(handle)};
	handle=reinterpret_cast<void*>(-1);
	if constexpr(throw_eh)
	{
		if(status)
			throw_nt_error(status);
		if(status2)
			throw_nt_error(status2);
	}
}

template<nt_family family,bool throw_eh=false>
inline void close_nt_user_process_information_and_wait(nt_user_process_information* hnt_user_process_info) noexcept(!throw_eh)
{
	if(hnt_user_process_info)[[likely]]
	{
		nt_wait_and_close_user_process_or_thread<family,throw_eh>(hnt_user_process_info->hthread);
		nt_wait_and_close_user_process_or_thread<family,throw_eh>(hnt_user_process_info->hprocess);
		delete hnt_user_process_info;
	}
}

template<nt_family family>
inline void nt_duplicate_object_std(void* parent_process,void*& standard_io_handle,void* process_handle)
{
	if(standard_io_handle==nullptr)
		return;
	check_nt_status(nt_duplicate_object<family>(parent_process,standard_io_handle,process_handle,std::addressof(standard_io_handle),0,0,0x00000002|0x00000004));
}

inline void check_nt_status(std::uint32_t status)
{
	if(status)
		throw_nt_error(status);
}

template<nt_family family>
inline nt_user_process_information* nt_process_create_impl(void* __restrict fhandle)
{
	std::unique_ptr<nt_user_process_information> uptr(new nt_user_process_information);
	void* hsection{reinterpret_cast<void*>(-1)};
	check_nt_status(nt_create_section<family==nt_family::zw>(std::addressof(hsection),0xf001f /*SECTION_ALL_ACCESS*/,
		nullptr,nullptr,0x02 /*PAGE_READONLY*/,0x1000000 /*SEC_IMAGE*/,fhandle));
	basic_nt_family_file<family,char> section(hsection);
	void* const current_process{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	void* hprocess{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	check_nt_status(nt_create_process<family==nt_family::zw>(std::addressof(hprocess),0x000F0000U|0x00100000U|0xFFF
		/*PROCESS_ALL_ACCESS==(STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFF)*/,
		nullptr,current_process,true,hsection,nullptr,nullptr));
	basic_nt_family_file<family,char> process(hprocess);
	process_basic_information pb_info{};
	check_nt_status(nt_query_information_process<family==nt_family::zw>(hprocess,process_information_class::ProcessBasicInformation,
		std::addressof(pb_info),sizeof(pb_info),nullptr));
	section_image_information sec_info{};
	check_nt_status(nt_query_section<family==nt_family::zw>(hprocess,section_information_class::SectionImageInformation,
		std::addressof(sec_info),sizeof(sec_info),nullptr));
	rtl_user_process_parameters rtl_up{};
	check_nt_status(rtl_p_init_environment(hprocess,pb_info.PebBaseAddress,std::addressof(rtl_up)));
	void* hthread{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	client_id cid{};
	check_nt_status(rtl_create_user_thread(hprocess,nullptr,true,sec_info.ZeroBits,sec_info.MaximumStackSize,
			sec_info.CommittedStackSize,sec_info.TransferAddress,pb_info.PebBaseAddress,std::addressof(hthread),std::addressof(cid)));
	*uptr={process.release(),hthread};
	return uptr.release();
}

template<nt_family family,typename T>
requires (std::same_as<T,char>||std::same_as<T,wchar_t>)
inline nt_user_process_information* nt_create_process_overloads(nt_at_entry entry,basic_cstring_view<T> filename)
{
	basic_nt_family_file<family,char> nf(entry,filename,open_mode::in|open_mode::excl);
	return nt_process_create_impl<family>(nf.handle);
}

template<nt_family family,typename T>
requires (std::same_as<T,char>||std::same_as<T,wchar_t>)
inline nt_user_process_information* nt_create_process_overloads(basic_cstring_view<T> filename)
{
	basic_nt_family_file<family,char> nf(filename,open_mode::in|open_mode::excl);
	return nt_process_create_impl<family>(nf.handle);
}

}

template<nt_family family>
requires (family==nt_family::nt||family==nt_family::zw)
class nt_family_process_observer
{
public:
	using native_handle_type = nt_user_process_information*;
	native_handle_type hnt_user_process_info{};
	constexpr auto& native_handle() noexcept
	{
		return hnt_user_process_info;
	}
	constexpr auto& native_handle() const noexcept
	{
		return hnt_user_process_info;
	}
	explicit constexpr operator bool() const noexcept
	{
		return hnt_user_process_info;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{hnt_user_process_info};
		hnt_user_process_info={};
		return temp;
	}
};

template<nt_family family>
inline void detach(nt_family_process_observer<family>& ppob) noexcept
{
	win32::nt::details::close_nt_user_process_information<family==nt_family::zw>(ppob.hnt_user_process_info);
	ppob.hnt_user_process_info=nullptr;
}
#if 0
struct nt_wait_status
{
	std::uint32_t exit_code{};
};

inline nt_wait_status query_process_basic_info(nt_process_observer ppob)
{
	
}

inline nt_wait_status wait(nt_process_observer ppob)
{
	if(handle==nullptr)
		throw_nt_status(0xC0000008);
	win32::nt::details_wait_and_close_user_process_or_thread<true>(ppob.hnt_user_process_info->hthread);
	auto& hprocess=ppob.hnt_user_process_info->hprocess;
	auto status{nt_wait_user_process_or_thread(hprocess)};

	auto status2{win32::nt::nt_close(hprocess)};

	hprocess=reinterpret_cast<void*>(-1);
	if constexpr(throw_eh)
	{
		if(status)
			throw_nt_error(status);
		if(status2)
			throw_nt_error(status2);
	}
}
#endif


#if 0
struct nt_process_args
{
	char const* const* args{};
	bool is_dynamic_allocated{};
	inline constexpr nt_process_args(char const* const* envir) noexcept:args(envir){}
	template<std::random_access_iterator Iter>
	requires (std::convertible_to<std::iter_value_t<Iter>,char const*>||requires(std::iter_value_t<Iter> v)
	{
		{v.c_str()}->std::convertible_to<char const*>;
	})
	inline constexpr nt_process_args(Iter begin,Iter end):
		args(details::dup_enviro_entry(begin,end)),is_dynamic_allocated(true)
	{}
	template<std::ranges::random_access_range range>
	requires (std::convertible_to<std::ranges::range_value_t<range>,char const*>||requires(std::ranges::range_value_t<range> v)
	{
		{v.c_str()}->std::convertible_to<char const*>;
	})
	inline constexpr nt_process_args(range&& rg):nt_process_args(std::ranges::cbegin(rg),std::ranges::cend(rg))
	{}
	inline constexpr nt_process_args(std::initializer_list<char const*> ilist):
		nt_process_args(ilist.begin(),ilist.end()){}
	nt_process_args(nt_process_args const&)=delete;
	nt_process_args& operator=(nt_process_args const&)=delete;
#if __cpp_constexpr_dynamic_alloc >= 201907L
	inline constexpr
#endif
	~nt_process_args()
	{
		if(is_dynamic_allocated)
			delete[] args;
	}
};
#endif

template<nt_family family>
class nt_family_process:public nt_family_process_observer<family>
{
public:
	using native_handle_type = nt_user_process_information*;
	constexpr nt_family_process() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr nt_family_process(native_hd hd) noexcept:nt_family_process_observer<family>{hd}{}

	explicit nt_family_process(nt_at_entry nate,wcstring_view filename):
		nt_family_process_observer<family>{win32::nt::details::nt_create_process_overloads<family>(nate,filename)}
		//to do. first test API
	{}

	nt_family_process(nt_family_process const& b)=delete;
	nt_family_process& operator=(nt_family_process const& b)=delete;
	constexpr nt_family_process(nt_family_process&& b) noexcept:nt_family_process_observer<family>{b.release()}{}
	nt_family_process& operator=(nt_family_process&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			win32::nt::details::close_nt_user_process_information_and_wait<family>(this->hnt_user_process_info);
			this->hnt_user_process_info = b.release();
		}
		return *this;
	}
	~nt_family_process()
	{
		win32::nt::details::close_nt_user_process_information_and_wait<family>(this->hnt_user_process_info);
	}
};

using nt_process_observer = nt_family_process_observer<nt_family::nt>;
using nt_process = nt_family_process<nt_family::nt>;

using zw_process_observer = nt_family_process_observer<nt_family::zw>;
using zw_process = nt_family_process<nt_family::zw>;

}