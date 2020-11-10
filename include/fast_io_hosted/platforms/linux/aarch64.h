#pragma once

namespace fast_io
{

template<std::size_t syscall_number,std::signed_integral return_value_type>
requires (1<sizeof(return_value_type))
inline auto system_call()
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number)
	: "memory", "cc","x8","x0"
	);
	return static_cast<return_value_type>(ret);
}

template<std::size_t syscall_number,std::signed_integral return_value_type>
requires (1<sizeof(return_value_type))
inline auto system_call(auto p1)
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"mov x0,%2\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number),"r"(p1)
	: "memory", "cc","x8","x0"
	);
	return static_cast<return_value_type>(ret);
}

template<std::size_t syscall_number>
inline void system_call_no_return(auto p1)
{
	std::size_t ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"mov x0,%2\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number),"r"(p1)
	: "memory", "cc","x8","x0"
	);
}

template<std::size_t syscall_number,std::signed_integral return_value_type>
requires (1<sizeof(return_value_type))
inline auto system_call(auto p1,auto p2)
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"mov x0,%2\n\t"
	"mov x1,%3\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number),"r"(p1),"r"(p2)
	: "memory", "cc","x8","x0","x1"
	);
	return static_cast<return_value_type>(ret);
}

template<std::size_t syscall_number,std::signed_integral return_value_type>
requires (1<sizeof(return_value_type))
inline auto system_call(auto p1,auto p2,auto p3)
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"mov x0,%2\n\t"
	"mov x1,%3\n\t"
	"mov x2,%4\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number),"r"(p1),"r"(p2),"r"(p3)
	: "memory", "cc","x8","x0","x1","x2"
	);
	return static_cast<return_value_type>(ret);
}

template<std::size_t syscall_number,std::signed_integral return_value_type>
requires (1<sizeof(return_value_type))
inline auto system_call(auto p1,auto p2,auto p3,auto p4)
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"mov x0,%2\n\t"
	"mov x1,%3\n\t"
	"mov x2,%4\n\t"
	"mov x3,%5\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number),"r"(p1),"r"(p2),"r"(p3),"r"(p4)
	: "memory", "cc","x8","x0","x1","x2","x3"
	);
	return static_cast<return_value_type>(ret);
}

template<std::size_t syscall_number,std::signed_integral return_value_type>
requires (1<sizeof(return_value_type))
inline auto system_call(auto p1,auto p2,auto p3,auto p4,auto p5)
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"mov x0,%2\n\t"
	"mov x1,%3\n\t"
	"mov x2,%4\n\t"
	"mov x3,%5\n\t"
	"mov x4,%6\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number),"r"(p1),"r"(p2),"r"(p3),"r"(p4),"r"(p5)
	: "memory", "cc","x8","x0","x1","x2","x3","x4"
	);
	return static_cast<return_value_type>(ret);
}

template<std::size_t syscall_number,std::signed_integral return_value_type>
requires (1<sizeof(return_value_type))
inline auto system_call(auto p1,auto p2,auto p3,auto p4,auto p5,auto p6)
{
	return_value_type ret;
	__asm__ __volatile__
	(
	"mov x8,%1\n\t"
	"mov x0,%2\n\t"
	"mov x1,%3\n\t"
	"mov x2,%4\n\t"
	"mov x3,%5\n\t"
	"mov x4,%6\n\t"
	"mov x5,%7\n\t"
	"svc 0\n\t"
	"mov %0,x0\n\t"
	: "=r" (ret)
	: "0"(syscall_number),"r"(p1),"r"(p2),"r"(p3),"r"(p4),"r"(p5),"r"(p6)
	: "memory", "cc","x8","x0","x1","x2","x3","x4","x5"
	);
	return static_cast<return_value_type>(ret);
}

template<std::integral I>
inline void fast_exit(I ret)
{
	system_call_no_return<__NR_exit>(ret);
}

}