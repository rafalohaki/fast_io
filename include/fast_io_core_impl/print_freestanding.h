#pragma once

namespace fast_io
{

namespace details
{
template<std::size_t mx_size,std::random_access_iterator Iter>
inline constexpr Iter output_unsigned_serialize_size(std::size_t val,Iter iter) noexcept;



}

namespace details
{

template<std::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr void output_unsigned_with_size(Iter str,U value,std::size_t len) noexcept;
}


namespace details::decay
{

enum class print_control_impl
{
normal,serialize
};


inline constexpr std::size_t reserve_len(std::size_t sz) noexcept
{
	std::size_t len{};
	for(;sz;sz/=10)
		++len;
	return len;
}

template<print_control_impl pci,std::integral char_type,typename value_type>
requires reserve_printable<char_type,value_type>
inline constexpr std::size_t print_reserve_control_size_impl() noexcept
{
	constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,value_type>)};
	if constexpr(pci==print_control_impl::serialize)
	{
		constexpr std::size_t len(reserve_len(sz)+sz+1);
		return len;
	}
	else
		return sz;
}

template<print_control_impl pci,std::integral char_type,typename value_type>
requires dynamic_reserve_printable<char_type,value_type>
inline constexpr std::size_t print_dynamic_reserve_control_size_impl(value_type v) noexcept
{
	constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,value_type>,v)};
	if constexpr(pci==print_control_impl::serialize)
	{
		constexpr std::size_t len(reserve_len(sz)+sz+1);
		return len;
	}
	else
		return sz;
}

template<print_control_impl pci,std::integral char_type,typename value_type,typename Iter>
inline constexpr Iter print_reserve_control_define_impl(Iter it,value_type v)
{
	if constexpr(pci==print_control_impl::serialize)
	{
		if constexpr(reserve_serializable<char_type,value_type>)
			return print_reserve_define(io_serial_type<char_type,value_type>,it,v);
		else
		{
			constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,value_type>)};
			constexpr std::size_t reserved_sz{reserve_len(sz)};
			constexpr std::size_t reservedln{reserved_sz+1};
			auto start{it+reservedln};
			auto eds{print_reserve_define(io_reserve_type<char_type,value_type>,start,v)};
			std::size_t real_size(eds-start);
			std::size_t real_size_size{chars_len<10,false,reserved_sz>(real_size)};
			output_unsigned_with_size(it,real_size,real_size_size);
			if constexpr(std::same_as<char,char_type>)
				it[real_size_size]=' ';
			else if constexpr(std::same_as<wchar_t,char_type>)
				it[real_size_size]=L' ';
			else
				it[real_size_size]=u8' ';
			auto realszp1{it+real_size_size+1};
			if(start==realszp1)
				return eds;
			return my_copy_n(start,real_size,realszp1);
		}
	}
	else
		return print_reserve_define(io_reserve_type<char_type,value_type>,it,v);
}

template<std::integral char_type,print_control_impl pci=print_control_impl::normal,typename T>
inline constexpr void scatter_print_recursive(io_scatter_t* arr,T t)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,t);
}

template<std::integral char_type,print_control_impl pci=print_control_impl::normal,typename T,typename... Args>
inline constexpr void scatter_print_recursive(io_scatter_t* arr,T t, Args ...args)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,t);
	scatter_print_recursive<char_type>(arr+1,args...);
}

template<std::integral char_type,typename T>
inline constexpr std::size_t calculate_scatter_reserve_size_unit()
{
	using real_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,real_type>)
	{
		constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,real_type>)};
		return sz;
	}
	else
		return 0;
}

template<std::integral char_type,typename T>
inline constexpr std::size_t calculate_scatter_reserve_size()
{
	return calculate_scatter_reserve_size_unit<char_type,T>();
}

template<std::integral char_type,typename T,typename... Args>
requires (sizeof...(Args)!=0)
inline constexpr std::size_t calculate_scatter_reserve_size()
{
	return calculate_scatter_reserve_size_unit<char_type,T>()+
		calculate_scatter_reserve_size<char_type,Args...>();
}



template<std::integral char_type,typename T,typename... Args>
inline constexpr std::size_t calculate_scatter_dynamic_reserve_size(T t,Args... args)
{
	if constexpr(!reserve_printable<char_type,T>&&
		dynamic_reserve_printable<char_type,T>)
	{
		std::size_t res{print_reserve_size(io_reserve_type<char_type,T>,t)};
		if constexpr(sizeof...(Args)==0)
			return res;
		else
			return res+calculate_scatter_dynamic_reserve_size<char_type>(args...);
	}
	else
	{
		if constexpr(sizeof...(Args)==0)
			return 0;
		else
			return calculate_scatter_dynamic_reserve_size<char_type>(args...);
	}
}


template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_reserve_recursive_unit(char_type*& start_ptr,
		io_scatter_t* arr,T t)
{
	using real_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,T>)
	{
#ifdef __SANITIZE_ADDRESS__
		constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,real_type>)};
		std::array<char_type,sz> sanitize_buffer;
		auto dt{print_reserve_define(io_reserve_type<char_type,real_type>,sanitize_buffer.data(),t)};
		auto end_ptr{non_overlapped_copy_n(sanitize_buffer.data(),static_cast<std::size_t>(dt-sanitize_buffer.data()),start_ptr)};
		*arr={start_ptr,(end_ptr-start_ptr)*sizeof(*start_ptr)};
		start_ptr=end_ptr;
#else
		auto end_ptr = print_reserve_define(io_reserve_type<char_type,real_type>,start_ptr,t);
		*arr={start_ptr,(end_ptr-start_ptr)*sizeof(*start_ptr)};
		start_ptr=end_ptr;
#endif
	}
	else
	{
		*arr=print_scatter_define(print_scatter_type<char_type>,t);
	}
}

template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_reserve_recursive(char_type* ptr,
		io_scatter_t* arr,T t)
{
	scatter_print_with_reserve_recursive_unit(ptr,arr,t);
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void scatter_print_with_reserve_recursive(char_type* ptr,
	io_scatter_t* arr,T t, Args ...args)
{
	scatter_print_with_reserve_recursive_unit(ptr,arr,t);
	scatter_print_with_reserve_recursive(ptr,arr+1,args...);
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void scatter_print_with_dynamic_reserve_recursive(io_scatter_t* __restrict arr,
	char_type* __restrict ptr,
	char_type* __restrict dynamic_buffer_ptr,T t, Args ...args)
{
	if constexpr(reserve_printable<char_type,T>)
	{
		auto end_ptr = print_reserve_define(io_reserve_type<char_type,T>,ptr,t);
		*arr={ptr,(end_ptr-ptr)*sizeof(*ptr)};
		if constexpr(sizeof...(Args)!=0)
			ptr=end_ptr;
	}
	else if constexpr(dynamic_reserve_printable<char_type,T>)
	{
		auto end_ptr = print_reserve_define(io_reserve_type<char_type,T>,dynamic_buffer_ptr,t);
		*arr={dynamic_buffer_ptr,(end_ptr-dynamic_buffer_ptr)*sizeof(*dynamic_buffer_ptr)};
		if constexpr(sizeof...(Args)!=0)
			dynamic_buffer_ptr = end_ptr;
	}
	else
		*arr=print_scatter_define(print_scatter_type<char_type>,t);
	if constexpr(sizeof...(Args)!=0)
		scatter_print_with_dynamic_reserve_recursive(arr+1,ptr,dynamic_buffer_ptr,args...);
}


template<std::integral char_type,typename T>
requires scatter_type_printable<char_type,T>
inline constexpr auto extract_one_scatter(T t)
{
	return print_scatter_define(print_scatter_type<char_type>,t);
}

template<typename output>
inline constexpr void print_serialize_size_bad_path(output out,std::size_t sz)
{
	using char_type = typename output::char_type;
	constexpr std::size_t size_t_reserve_length{print_reserve_size(io_reserve_type<char_type,std::size_t>)+1};
	std::array<char_type,size_t_reserve_length> array;
	auto it{print_reserve_define(io_reserve_type<char_type,std::size_t>,array.data(),sz)};
	if constexpr(std::same_as<char,char_type>)
		*it=' ';
	else if constexpr(std::same_as<wchar_t,char_type>)
		*it=L' ';
	else
		*it=u8' ';
	++it;
	write(out,array.data(),it);
}

template<bool line,print_control_impl pci,output_stream output,typename value_type>
requires (reserve_printable<typename output::char_type,value_type>)
inline constexpr void print_control_reserve_bad_path(output out,value_type t)
{
	using char_type = typename output::char_type;
	constexpr std::size_t size{print_reserve_control_size_impl<pci,char_type,value_type>()+static_cast<std::size_t>(line)};
	std::array<char_type,size> array;
	if constexpr(line)
	{
		auto it{print_reserve_control_define_impl<pci,char_type,value_type>(array.data(),t)};
		if constexpr(std::same_as<char,char_type>)
			*it='\n';
		else if constexpr(std::same_as<wchar_t,char_type>)
			*it=L'\n';
		else
			*it=u8'\n';
		write(out,array.data(),++it);
	}
	else
		write(out,array.data(),print_reserve_control_define_impl<pci,char_type,value_type>(array.data(),t));

}

template<bool line,print_control_impl pci,typename value_type,output_stream output>
requires (!reserve_printable<typename output::char_type,value_type>&&dynamic_reserve_printable<typename output::char_type,value_type>)
inline constexpr void print_control_dynamic_reserve_bad_path(output out,value_type t,std::size_t sizep1)
{
	using char_type = typename output::char_type;
	local_operator_new_array_ptr<char_type> ptr(sizep1);
	auto it{dynamic_print_reserve_control_define_impl<pci,char_type,value_type>(ptr.ptr,t)};
	if constexpr(line)
	{
		if constexpr(std::same_as<char,char_type>)
			*it='\n';
		else if constexpr(std::same_as<wchar_t,char_type>)
			*it=L'\n';
		else
			*it=u8'\n';
		++it;
	}
	write(out,ptr.ptr,it);
}

template<bool line=false,print_control_impl pci=print_control_impl::normal,output_stream output,typename T>
requires (std::is_trivially_copyable_v<output>&&std::is_trivially_copyable_v<T>)
inline constexpr void print_control(output out,T t)
{
	using char_type = typename output::char_type;
	using value_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,value_type>)
	{
		constexpr std::size_t size{print_reserve_control_size_impl<pci,char_type,value_type>()+static_cast<std::size_t>(line)};
#ifndef __SANITIZE_ADDRESS__

#if 0	
		if constexpr(contiguous_output_stream<output>)
		{
			auto it{print_reserve_control_define_impl<pci,char_type,value_type>(obuffer_curr(out),t)};
			if constexpr(line)
			{
				if constexpr(std::same_as<char,char_type>)
					*it='\n';
				else if constexpr(std::same_as<wchar_t,char_type>)
					*it=L'\n';
				else
					*it=u8'\n';
				++it;
			}
			obuffer_set_curr(out,it);
		}
		else
#endif
		if constexpr(buffer_output_stream<output>)
		{
			auto bcurr{obuffer_curr(out)};
			auto bend{obuffer_end(out)};
			std::ptrdiff_t const diff(bend-bcurr);
			if(static_cast<std::ptrdiff_t>(size)<diff)[[likely]]
			{
				//To check whether this affects performance.
				if constexpr(line)
				{
					auto it{print_reserve_control_define_impl<pci,char_type,value_type>(bcurr,t)};
					if constexpr(line)
					{
						if constexpr(std::same_as<char,char_type>)
							*it='\n';
						else if constexpr(std::same_as<wchar_t,char_type>)
							*it=L'\n';
						else
							*it=u8'\n';
						++it;
					}	
					obuffer_set_curr(out,it);
				}
				else
					obuffer_set_curr(out,print_reserve_control_define_impl<pci,char_type,value_type>(bcurr,t));
			}
			else
				print_control_reserve_bad_path<line,pci>(out,t);
		}
		else if constexpr(reserve_output_stream<output>)	//To do: Remove reserve_output_stream concepts
		{
			if constexpr(std::is_pointer_v<std::remove_cvref_t<decltype(oreserve(out,size))>>)
			{
				auto ptr{oreserve(out,size)};
				if(ptr==nullptr)[[unlikely]]
				{
					print_control_reserve_bad_path<line,pci>(out,t);
					return;
				}
				if constexpr(line)
				{
					auto it{print_reserve_control_define_impl<pci,char_type,value_type>(ptr,t)};
					if constexpr(std::same_as<char,char_type>)
						*it='\n';
					else if constexpr(std::same_as<wchar_t,char_type>)
						*it=L'\n';
					else
						*it=u8'\n';
					orelease(out,++it);
				}
				else
					orelease(out,print_reserve_control_define_impl<pci,char_type,value_type>(ptr,t));
			}
			else
			{
				if constexpr(line)
				{
					auto it{print_reserve_control_define_impl<pci,char_type,value_type>(oreserve(out,size),t)};
					if constexpr(std::same_as<char,char_type>)
						*it='\n';
					else if constexpr(std::same_as<wchar_t,char_type>)
						*it=L'\n';
					else
						*it=u8'\n';
					orelease(out,++it);
				}
				else
					orelease(out,print_reserve_control_define_impl<pci,char_type,value_type>(oreserve(out,size),t));
			}
		}
		else
#endif
		{
			print_control_reserve_bad_path(out,t);
		}
	}
	else if constexpr(dynamic_reserve_printable<char_type,value_type>)
	{
		std::size_t size{print_dynamic_reserve_control_size_impl<pci,char_type,value_type>(t)};
		if constexpr(line)
			++size;
#ifndef __SANITIZE_ADDRESS__

#if 0
		if constexpr(contiguous_output_stream<output>)
		{
			auto it{dynamic_print_reserve_control_define_impl<pci,char_type,value_type>(obuffer_curr(out),t,size)};
			if constexpr(line)
			{
				if constexpr(std::same_as<char,char_type>)
					*it='\n';
				else if constexpr(std::same_as<wchar_t,char_type>)
					*it=L'\n';
				else
					*it=u8'\n';
				++it;
			}
			obuffer_set_curr(out,it);
		}
		else
#endif	
		if constexpr(buffer_output_stream<output>)
		{
			auto curr{obuffer_curr(out)};

			auto ed{obuffer_end(out)};
			std::ptrdiff_t diff(ed-curr);
			if(static_cast<std::ptrdiff_t>(size)<diff)
			{
				auto it{print_reserve_control_define_impl<pci,char_type,value_type>(curr,t)};
				if constexpr(line)
				{
					if constexpr(std::same_as<char,char_type>)
						*it='\n';
					else if constexpr(std::same_as<wchar_t,char_type>)
						*it=L'\n';
					else
						*it=u8'\n';
					++it;
				}
				obuffer_set_curr(out,it);
			}
			else
				print_control_dynamic_reserve_bad_path<line,pci,value_type>(out,t,size);
		}
		else
#endif
		{
			print_control_dynamic_reserve_bad_path<line,pci,value_type>(out,t,size);
		}
	}
	else if constexpr(scatter_type_printable<char_type,value_type>)
	{
		basic_io_scatter_t<char_type> scatter{print_scatter_define(print_scatter_type<char_type>,t)};
		if constexpr(line)
		{
#if 0
			if constexpr(contiguous_output_stream<output>)
			{
				obuffer_set_curr(out,non_overlapped_copy_n(scatter.base,scatter.len,obuffer_curr(out)));
			}
			else
#endif			
			if constexpr(buffer_output_stream<output>)
			{
				auto curr=obuffer_curr(out);
				auto end=obuffer_end(out);
				std::size_t const len{scatter.len};
				std::ptrdiff_t sz(end-curr);
				std::size_t lenp1(len+1);
				constexpr std::size_t size_t_reserve_length{print_reserve_size(io_reserve_type<char_type,std::size_t>)+1};
				if constexpr(pci==print_control_impl::serialize)
				{
					lenp1+=size_t_reserve_length;
				}
				if(static_cast<std::ptrdiff_t>(lenp1)<sz)[[likely]]
				{
					if constexpr(pci==print_control_impl::serialize)
					{
					curr=print_reserve_define(io_reserve_type<char_type,std::size_t>,curr,len);
					if constexpr(std::same_as<char,char_type>)
						*curr=' ';
					else if constexpr(std::same_as<wchar_t,char_type>)
						*curr=L' ';
					else
						*curr=u8' ';
					++curr;
					}
					curr=details::non_overlapped_copy_n(scatter.base,len,curr);
					if constexpr(std::same_as<char,char_type>)
						put(out,'\n');
					else if constexpr(std::same_as<wchar_t,char_type>)
						put(out,L'\n');
					else
						put(out,u8'\n');
					++curr;
					obuffer_set_curr(out,curr);
				}
				else
				{
					if constexpr(pci==print_control_impl::serialize)
					{
					print_serialize_size_bad_path(out,scatter.len);
					}
					write(out,scatter.base,scatter.base+scatter.len);
					if constexpr(std::same_as<char,char_type>)
						put(out,'\n');
					else if constexpr(std::same_as<wchar_t,char_type>)
						put(out,L'\n');
					else
						put(out,u8'\n');
				}
			}
			else
			{
				if constexpr(pci==print_control_impl::serialize)
					print_serialize_size_bad_path(out,scatter.len);
				write(out,scatter.base,scatter.base+scatter.len);
				if constexpr(details::exec_charset_is_ebcdic<char_type>())
					put(out,0x25);
				else
					put(out,u8'\n');
			}
		}
		else
		{
			if constexpr(pci==print_control_impl::serialize)
				print_serialize_size_bad_path(out,scatter.len);
			write(out,scatter.base,scatter.base+scatter.len);
		}
	}
	else if constexpr(printable<output,value_type>)
	{
		if constexpr(pci==print_control_impl::serialize)
		{
			internal_temporary_buffer<char_type> buffer;
			print_control(io_ref(buffer),t);
			auto beg{obuffer_begin(buffer)};
			auto curr{obuffer_curr(buffer)};
			std::size_t size(curr-beg);
			print_control(out,size);
			write(out,beg,curr);
		}
		else
			print_define(out,t);
		if constexpr(line)
		{
			if constexpr(std::same_as<char_type,char>)
				put(out,'\n');
			else if constexpr(std::same_as<char_type,wchar_t>)
				put(out,L'\n');
			else
				put(out,u8'\n');
		}
	}
	else
	{
		static_assert(printable<output,value_type>,"type not printable");
	}
}

template<typename T,typename U>
concept test_print_control = requires(T t,U arg)
{
	print_control(t,arg);
};

template<bool line,output_stream output,typename ...Args>
inline constexpr void print_fallback(output out,Args ...args)
{
	using char_type = typename output::char_type;
	if constexpr(scatter_output_stream<output>&&((scatter_printable<typename output::char_type,Args>||reserve_printable<typename output::char_type,Args>||dynamic_reserve_printable<typename output::char_type,Args>)&&...))
	{
		std::array<io_scatter_t,(sizeof...(Args))+static_cast<std::size_t>(line)> scatters;
		if constexpr((scatter_printable<typename output::char_type,Args>&&...))
		{
			scatter_print_recursive<typename output::char_type>(scatters.data(),args...);
			if constexpr(line)
			{
				if constexpr(std::same_as<char_type,char>)
				{
					char_type ch('\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
				else if constexpr(std::same_as<char_type,wchar_t>)
				{
					char_type ch(L'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
				else
				{
					char_type ch(u8'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
			}
			else
				scatter_write(out,scatters);
		}
		else if constexpr(((scatter_printable<char_type,Args>||
			reserve_printable<char_type,Args>)&&...))
		{
			std::array<char_type,calculate_scatter_reserve_size<char_type,Args...>()> array;
			scatter_print_with_reserve_recursive(array.data(),scatters.data(),args...);
			if constexpr(line)
			{
				if constexpr(std::same_as<char_type,char>)
				{
					char_type ch('\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
				else if constexpr(std::same_as<char_type,wchar_t>)
				{
					char_type ch(L'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
				else
				{
					char_type ch(u8'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
			}
			else
				scatter_write(out,scatters);
		}
		else
		{
			std::array<char_type,calculate_scatter_reserve_size<char_type,Args...>()> array;
			local_operator_new_array_ptr<char_type> new_ptr(calculate_scatter_dynamic_reserve_size<char_type>(args...));
			scatter_print_with_dynamic_reserve_recursive(scatters.data(),array.data(),new_ptr.ptr,args...);
			if constexpr(line)
			{
				if constexpr(std::same_as<char_type,char>)
				{
					char_type ch('\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
				else if constexpr(std::same_as<char_type,wchar_t>)
				{
					char_type ch(L'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
				else
				{
					char_type ch(u8'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,scatters);
				}
			}
			else
				scatter_write(out,scatters);
		}
	}
	else
	{
		using internal_buffer_type = internal_temporary_buffer<typename output::char_type>;
		internal_buffer_type buffer;
		auto ref{io_ref(buffer)};
		if constexpr((test_print_control<decltype(ref),Args>&&...))
		{
			if constexpr(line)
			{
				if constexpr((sizeof...(Args)==1)&&(reserve_printable<typename output::char_type,Args>&&...))
				{
					((print_control<true>(ref,args)),...);
				}
				else
				{
					((print_control(ref,args)),...);
					if constexpr(details::exec_charset_is_ebcdic<char_type>())
						put(buffer,0x25);
					else
						put(buffer,u8'\n');
				}
			}
			else
			{
				(print_control(ref,args),...);
			}
			write(out,buffer.beg_ptr,buffer.end_ptr);
		}
		else if constexpr(buffer_output_stream<internal_buffer_type>)
		{
			static_assert(!buffer_output_stream<internal_buffer_type>,
			"\n\n\tThis type is not defined for printing. Please consider define it with print_define or print_reserve_define."
			"\n\tSee wiki page https://github.com/expnkx/fast_io/wiki/0018.-custom-type\n");
		}
	}
}


template<bool line,print_control_impl pci=print_control_impl::normal,output_stream output,typename ...Args>
requires (std::is_trivially_copyable_v<output>&&(std::is_trivially_copyable_v<Args>&&...))
inline constexpr void print_freestanding_decay_normal(output out,Args ...args)
{
	using char_type = typename output::char_type;
	if constexpr(mutex_stream<output>)
	{
		lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		print_freestanding_decay_normal<line,pci>(io_ref(dout),args...);
	}
	else if constexpr(((printable<output,Args>||dynamic_reserve_printable<output,Args>||reserve_printable<char_type,Args>||scatter_type_printable<char_type,Args>)&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
	{
		(print_control<line,pci>(out,args),...);
	}
	else
	{
		if constexpr(sizeof...(Args)==1&&(scatter_type_printable<char_type,Args>&&...))
			((print_fallback<line,pci>(out,args)),...);
		else
		{
			if constexpr(pci==print_control_impl::normal)
			{
				print_fallback<line>(out,args...);
			}
			else
				((print_fallback<line,pci>(out,args)),...);
		}
	}
}

}

template<typename output,typename ...Args>
requires (output_stream<output>||status_output_stream<output>)&&(std::is_trivially_copyable_v<output>&&(std::is_trivially_copyable_v<Args>&&...))
inline constexpr void print_freestanding_decay(output out,Args ...args)
{
	if constexpr(status_output_stream<output>)
		print_status_define(out,args...);
	else if constexpr(output_stream<output>)
		details::decay::print_freestanding_decay_normal<false>(out,args...);
}

template<std::integral char_type,typename T>
inline constexpr decltype(auto) io_print_alias(T&& t)
{
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return print_alias_define(io_alias,std::forward<T>(t));
	else if constexpr(alias_type_printable<char_type,std::remove_cvref_t<T>>)
		return print_alias_define(io_alias_type<char_type>,std::forward<T>(t));
	else
		return std::forward<T>(t);
}

template<typename output,typename ...Args>
requires (output_stream<output>||status_output_stream<output>)
inline constexpr void print_freestanding(output&& out,Args&& ...args)
{
	print_freestanding_decay(io_ref(out),io_forward(io_print_alias<typename std::remove_cvref_t<output>::char_type>(args))...);
}


template<typename output,typename ...Args>
requires (output_stream<output>||status_output_stream<output>)
inline constexpr void println_freestanding_decay(output out,Args ...args)
{
	if constexpr(status_output_stream<output>)
	{
		println_status_define(out,args...);
	}
	else if constexpr(mutex_stream<output>)
	{
		details::lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		println_freestanding_decay(io_ref(dout),args...);
	}
	else if constexpr((sizeof...(Args)==1||buffer_output_stream<output>)&&((printable<output,Args>||reserve_printable<typename output::char_type,Args>||dynamic_reserve_printable<typename output::char_type,Args>||scatter_type_printable<typename output::char_type,Args>)&&...))
	{
		using char_type = typename std::remove_cvref_t<output>::char_type;
		if constexpr((sizeof...(Args)==1))
			((details::decay::print_control<true>(out,args)),...);
		else
		{
			((details::decay::print_control(out,args)),...);
			if constexpr(details::exec_charset_is_ebcdic<char_type>())
				put(out,0x25);
			else
				put(out,u8'\n');
		}
	}
	else if constexpr(output_stream<output>)
		details::decay::print_fallback<true>(out,args...);
}

template<output_stream output,typename ...Args>
inline constexpr void println_freestanding(output&& out,Args&& ...args)
{
	println_freestanding_decay(io_ref(out),io_forward(io_print_alias<typename std::remove_cvref_t<output>::char_type>(args))...);
}


}