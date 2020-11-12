#pragma once

namespace fast_io
{

struct nt_user_process_information
{
	void* hprocess{reinterpret_cast<void*>(-1)};
	void* hthread{reinterpret_cast<void*>(-1)};
	win32::nt::ps_create_info create_info{};
};

namespace win32::nt::details
{

inline void close_nt_user_process_information_not_null(nt_user_process_information* hnt_user_process_info) noexcept
{
	if(hnt_user_process_info->hthread!=reinterpret_cast<void*>(-1))[[likely]]
	{
		win32::nt::nt_close(hnt_user_process_info->hthread);
		hnt_user_process_info->hthread=reinterpret_cast<void*>(-1);
	}
	if(hnt_user_process_info->hprocess!=reinterpret_cast<void*>(-1))[[likely]]
	{
		win32::nt::nt_close(hnt_user_process_info->hprocess);
		hnt_user_process_info->hprocess=reinterpret_cast<void*>(-1);
	}
	delete hnt_user_process_info;
}

inline void close_nt_user_process_information(nt_user_process_information* hnt_user_process_info) noexcept
{
	if(hnt_user_process_info)[[likely]]
		return details::close_nt_user_process_information_not_null(hnt_user_process_info);
}

inline std::uint32_t nt_wait_user_process_or_thread(void* hprocess_thread) noexcept
{
	return win32::nt::nt_wait_for_single_object(hprocess_thread,false,nullptr);
}

template<bool throw_eh=false>
inline void nt_wait_and_close_user_process_or_thread(void*& handle) noexcept(!throw_eh)
{
	if(handle==reinterpret_cast<void*>(-1))
		return;
	auto status{nt_wait_user_process_or_thread(handle)};
	auto status2{win32::nt::nt_close(handle)};
	handle=reinterpret_cast<void*>(-1);
	if constexpr(throw_eh)
	{
		if(status)
			throw_nt_error(status);
		if(status2)
			throw_nt_error(status2);
	}
}

template<bool throw_eh=false>
inline void close_nt_user_process_information_and_wait(nt_user_process_information* hnt_user_process_info) noexcept(!throw_eh)
{
	if(hnt_user_process_info)[[likely]]
	{
		nt_wait_and_close_user_process_or_thread<throw_eh>(hnt_user_process_info->hthread);
		nt_wait_and_close_user_process_or_thread<throw_eh>(hnt_user_process_info->hprocess);
		delete hnt_user_process_info;
	}
}

inline nt_user_process_information* create_nt_user_process_impl(void* __restrict directory,wcstring_view filename)
{
	std::unique_ptr<nt_user_process_information> uptr(new nt_user_process_information);

	rtl_user_process_parameters user_parameters{};


	ps_attribute_list attributes{};

	constexpr std::uint32_t maximum_allowed{0x02000000L};

	std::size_t filename_size{filename.size()<<1};
	if constexpr(sizeof(std::size_t)>sizeof(std::uint16_t))
	{
		if(static_cast<std::size_t>(std::numeric_limits<std::uint16_t>::max())<filename_size)
			throw_nt_error(0xC0000106);
	}
	unicode_string path{static_cast<std::uint16_t>(filename_size),static_cast<std::uint16_t>(filename_size),
		const_cast<wchar_t*>(filename.c_str())};

	object_attributes obj{.Length=sizeof(win32::nt::object_attributes),
		.RootDirectory=directory,
		.ObjectName=std::addressof(path),
		.Attributes = 0x00000002U};
#if 0
	object_attributes thobj{.Length=sizeof(win32::nt::object_attributes),.Attributes = 0x00000002U};

	ps_create_info create_info{.Size=sizeof(ps_create_info),.u={.InitState={.u={.InitFlags=0x20000003},.AdditionalFileAccess=0x81}}};

	auto status{nt_create_user_process(std::addressof(uptr->hprocess),std::addressof(uptr->hthread),
		maximum_allowed,maximum_allowed,std::addressof(obj),
		std::addressof(thobj),0,0,nullptr,
		std::addressof(uptr->create_info),std::addressof(attributes))};
	if(status)
		throw_nt_error(status);
#endif
	void* const current_process{reinterpret_cast<void*>(static_cast<intptr_t>(-1))};
	puts("Before\n");
	auto status{nt_create_process(std::addressof(uptr->hprocess),0x0400/*PROCESS_QUERY_INFORMATION*/,
		std::addressof(obj),current_process,true,nullptr,nullptr,nullptr)};
	if(status)
		throw_nt_error(status);
	puts("Success\n");
	return uptr.release();
}

}

class nt_process_observer
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

inline void detach(nt_process_observer& ppob) noexcept
{
	win32::nt::details::close_nt_user_process_information(ppob.hnt_user_process_info);
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

class nt_process:public nt_process_observer
{
public:
	using native_handle_type = nt_user_process_information*;
	constexpr nt_process() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr nt_process(native_hd hd) noexcept:nt_process_observer{hd}{}

	explicit nt_process(nt_at_entry nate,wcstring_view filename):
		nt_process_observer{win32::nt::details::create_nt_user_process_impl(nate.handle,filename)}
		//to do. first test API
	{}

	nt_process(nt_process const& b)=delete;
	nt_process& operator=(nt_process const& b)=delete;
	constexpr nt_process(nt_process&& b) noexcept:nt_process_observer{b.release()}{}
	nt_process& operator=(nt_process&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			win32::nt::details::close_nt_user_process_information_and_wait(this->hnt_user_process_info);
			this->hnt_user_process_info = b.release();
		}
		return *this;
	}
	~nt_process()
	{
		win32::nt::details::close_nt_user_process_information_and_wait(this->hnt_user_process_info);
	}
};


}