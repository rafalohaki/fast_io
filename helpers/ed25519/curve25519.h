#pragma once

namespace fast_io::details::curve25519
{

struct field_element
{
	std::uint64_t element[5];
	inline constexpr std::uint64_t& operator[](std::size_t position) noexcept
	{
		return element[position];
	}
	inline constexpr std::uint64_t const& operator[](std::size_t position) const noexcept
	{
		return element[position];
	}
};

inline constexpr field_element operator+(field_element const& a, field_element const& b) noexcept
{
	return {a[0]+b[0],a[1]+b[1],a[2]+b[2],a[3]+b[3],a[4]+b[4]};
}

template<std::uint64_t p0,std::uint64_t p1234>
inline constexpr field_element subtraction(field_element const& a,field_element const& b) noexcept
{
	return {a[0]+p0-b[0],
		a[1]+p1234-b[1],
		a[2]+p1234-b[2],
		a[3]+p1234-b[3],
		a[4]+p1234-b[4]};
}

inline constexpr field_element operator-(field_element const& a,field_element const& b) noexcept
{
	return subtraction<0x0fffffffffffda,0x0ffffffffffffe>(a,b);
}

inline constexpr field_element sub_after_basic(field_element const& a,field_element const& b) noexcept
{
	return subtraction<0x1fffffffffffb4,0x1ffffffffffffc>(a,b); 
}


inline constexpr field_element add_reduce(field_element const& a,field_element const& b) noexcept
{
	constexpr auto reduce_mask_51{(static_cast<std::uint64_t>(1) << 51) - 1};
	field_element result;
	std::uint64_t c{(a[0] + b[0])>>51};
	result[1] = a[1] + b[1] + c; c = (result[1] >> 51); result[1] &= reduce_mask_51;
	result[2] = a[2] + b[2] + c; c = (result[2] >> 51); result[2] &= reduce_mask_51;
	result[3] = a[3] + b[3] + c; c = (result[3] >> 51); result[3] &= reduce_mask_51;
	result[4] = a[4] + b[4] + c; c = (result[4] >> 51); result[4] &= reduce_mask_51;
	result[0] = c * 19;
	return result;
}

inline constexpr field_element sub_reduce(field_element const& a,field_element const& b) noexcept
{
	constexpr auto reduce_mask_51{(static_cast<std::uint64_t>(1) << 51) - 1};
	constexpr std::uint64_t p1234{0x1ffffffffffffc};
	field_element result;
	std::uint64_t c{(a[0] + static_cast<std::uint64_t>(0x1fffffffffffb4) - b[0])>>51};
	result[1] = a[1] + p1234 - b[1] + c; c = (result[1] >> 51); result[1] &= reduce_mask_51;
	result[2] = a[2] + p1234 - b[2] + c; c = (result[2] >> 51); result[2] &= reduce_mask_51;
	result[3] = a[3] + p1234 - b[3] + c; c = (result[3] >> 51); result[3] &= reduce_mask_51;
	result[4] = a[4] + p1234 - b[4] + c; c = (result[4] >> 51); result[4] &= reduce_mask_51;
	result[0] = c * 19;
	return result;
}

inline constexpr field_element operator-(field_element const& a) noexcept
{
	constexpr auto reduce_mask_51{(static_cast<std::uint64_t>(1) << 51) - 1};
	constexpr std::uint64_t p1234{0x1ffffffffffffc};
	field_element result;
	std::uint64_t c{(static_cast<std::uint64_t>(0x1fffffffffffb4) - a[0])>>51};
	result[1] = p1234 - a[1] + c; c = (result[1] >> 51); result[1] &= reduce_mask_51;
	result[2] = p1234 - a[2] + c; c = (result[2] >> 51); result[2] &= reduce_mask_51;
	result[3] = p1234 - a[3] + c; c = (result[3] >> 51); result[3] &= reduce_mask_51;
	result[4] = p1234 - a[4] + c; c = (result[4] >> 51); result[4] &= reduce_mask_51;
	result[0] = c * 19;
	return result;
}

inline constexpr field_element reduce(uint128_t t0,uint128_t t1,uint128_t t2,uint128_t t3,uint128_t t4) noexcept
{
	constexpr auto reduce_mask_51{(static_cast<std::uint64_t>(1) << 51) - 1};

	auto r0{low(t0)&reduce_mask_51}; std::uint64_t c=low(t0>>51);
	t1+=c; auto r1{low(t1)&reduce_mask_51}; c=low(t1>>51);
	t2+=c; auto r2{low(t2)&reduce_mask_51}; c=low(t2>>51);
	t3+=c; auto r3{low(t3)&reduce_mask_51}; c=low(t3>>51);
	t4+=c; auto r4{low(t4)&reduce_mask_51}; c=low(t4>>51);

	r0+=c*19; c=r0>>51; r0&=reduce_mask_51;
	r1+=c;

	return {{r0,r1,r2,r3,r4}};
}

inline constexpr field_element operator*(field_element r,field_element s) noexcept
{
	return reduce(
		mul_extend(r[0],s[0])+mul_extend(r[4],s[1]*19)+mul_extend(r[3],s[2]*19)+mul_extend(r[2],s[3]*19)+mul_extend(r[1],s[4]*19),
		mul_extend(r[0],s[1])+mul_extend(r[1],s[0])+mul_extend(r[4],s[2]*19)+mul_extend(r[3],s[3]*19)+mul_extend(r[2],s[4]*19),
		mul_extend(r[0],s[2])+mul_extend(r[1],s[1])+mul_extend(r[2],s[0])+mul_extend(r[4],s[3]*19)+mul_extend(r[3],s[4]*19),
		mul_extend(r[0],s[3])+mul_extend(r[1],s[2])+mul_extend(r[2],s[1])+mul_extend(r[3],s[0])+mul_extend(r[4],s[4]*19),
		mul_extend(r[0],s[4])+mul_extend(r[1],s[3])+mul_extend(r[2],s[2])+mul_extend(r[3],s[1])+mul_extend(r[4],s[0]));
}

inline constexpr field_element square(field_element a) noexcept
{
	auto b0{a[0]*2};
	auto b1{a[1]*2};
	auto b2{a[2]*2*19};
	auto b419{a[4]*19};
	auto b4{b419*2};

	return reduce(
		mul_extend(a[0],a[0])+mul_extend(b4,a[1])+mul_extend(b2,a[3]),
      		mul_extend(b0,a[1])+mul_extend(b4,a[2])+mul_extend(a[3],a[3]*19),
       		mul_extend(b0,a[2])+mul_extend(a[1],a[1])+mul_extend(b4,a[3]),
        	mul_extend(b0,a[3])+mul_extend(b1,a[2])+mul_extend(a[4],b419),
        	mul_extend(b0,a[4])+mul_extend(b1,a[3])+mul_extend(a[2],a[2]));
}

inline constexpr field_element square_times(field_element a,std::size_t count) noexcept
{
	for(std::size_t i{};i!=count;++i)
		a=square(a);
	return a;
}

inline field_element expand(std::span<std::byte,32> span) noexcept
{
	std::uint64_t x0,x1,x2,x3;
	memcpy(std::addressof(x0),span.data(),8);
	memcpy(std::addressof(x1),span.data()+8,8);
	memcpy(std::addressof(x2),span.data()+16,8);
	memcpy(std::addressof(x3),span.data()+24,8);
	if constexpr(std::endian::big==std::endian::native)
	{
		x0=byte_swap(x0);
		x1=byte_swap(x1);
		x2=byte_swap(x2);
		x3=byte_swap(x3);
	}
	constexpr auto reduce_mask_51{(static_cast<std::uint64_t>(1) << 51) - 1};
	std::uint64_t r0=x0&reduce_mask_51;
	x0=(x0>>51) | (x1<<13);
	std::uint64_t r1=x0&reduce_mask_51;
	x1=(x1>>38) | (x2<<26);
	std::uint64_t r2=x1&reduce_mask_51;
	x2=(x2>>25) | (x3<<39);
	std::uint64_t r3=x2&reduce_mask_51;
	x3=(x3>>12);
	std::uint64_t r4=x3&reduce_mask_51;
	return {{r0,r1,r2,r3,r4}};
}

inline constexpr std::size_t print_reserve_size(io_reserve_type_t<field_element>) noexcept
{
	return 32;
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<field_element>,Iter iter,field_element t) noexcept
{
	constexpr auto reduce_mask_51{(static_cast<std::uint64_t>(1) << 51) - 1};
	auto contract_carry([&]
	{
		t[1]+=t[0]>>51;	t[0]&=reduce_mask_51;
		t[2]+=t[1]>>51;	t[1]&=reduce_mask_51;
		t[3]+=t[2]>>51;	t[2]&=reduce_mask_51;
		t[4]+=t[3]>>51;	t[3]&=reduce_mask_51;
	});
	auto contract_carry_full([&]
	{
		contract_carry();
		t[0]+=19*(t[4]>>51);
		t[4]&=reduce_mask_51;
	});
	auto contract_carry_final([&]
	{
		contract_carry();
		t[4]&=reduce_mask_51;
	});
	contract_carry_full();
	contract_carry_full();
	t[0]+=reduce_mask_51-19;
	t[1]+=reduce_mask_51;
	t[2]+=reduce_mask_51;
	t[3]+=reduce_mask_51;
	t[4]+=reduce_mask_51;

	contract_carry_final();
	for(std::size_t i{};i!=4;++i)
	{
		std::uint64_t f{(t[i]>>(13*i))|(t[i+1]<<(51-13*i))};
		if constexpr(std::contiguous_iterator<Iter>&&sizeof(*iter)==1)
			memcpy(std::to_address(iter),std::addressof(f),sizeof(f));
		else
		{
			std::array<unsigned char,sizeof(f)> arr;
			memcpy(arr.data(),std::addressof(f),sizeof(f));
			non_overlapped_copy_n(arr.data(),sizeof(f),iter);
		}
		iter+=sizeof(f);
	}
	return iter;
}


}