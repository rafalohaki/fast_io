#include"naive.h"


number add_reduce(number const& a,number const& b) noexcept
{
    number out;
    constexpr auto reduce_mask_51{((uint64_t)1 << 51) - 1};
	std::uint64_t c;
	out[0] = a[0] + b[0]    ; c = (out[0] >> 51); out[0] &= reduce_mask_51;
	out[1] = a[1] + b[1] + c; c = (out[1] >> 51); out[1] &= reduce_mask_51;
	out[2] = a[2] + b[2] + c; c = (out[2] >> 51); out[2] &= reduce_mask_51;
	out[3] = a[3] + b[3] + c; c = (out[3] >> 51); out[3] &= reduce_mask_51;
	out[4] = a[4] + b[4] + c; c = (out[4] >> 51); out[4] &= reduce_mask_51;
	out[0] = c * 19;
    return out;
}