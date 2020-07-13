#include"../../include/fast_io.h"

/*
Thanks Jsoniter Scala Andriy Plokhotnyuk author for providing test suite
https://github.com/plokhotnyuk/jsoniter-scala/blob/e8e246d6b716413d9fe45fc57bf214aaca3d843b/jsoniter-scala-core/shared/src/test/scala/com/github/plokhotnyuk/jsoniter_scala/core/JsonWriterSpec.scala#L488-L692
Great!
*/

template<std::floating_point F>
inline void check(F f)
{
	println(f);
}

template<std::floating_point F>
inline void test_speciail_case()
{
	using lm = std::numeric_limits<F>;
	println("Special cases:\n"
	"min():",lm::min(),"\n"
	"lowest():",lm::lowest(),"\n"
	"max():",lm::max(),"\n"
	"epsilon():",lm::epsilon(),"\n"
	"round_error():",lm::round_error(),"\n"
	"infinity():",lm::infinity(),"\n"
	"quiet_NaN():",lm::quiet_NaN(),"\n"
	"signaling_NaN():",lm::signaling_NaN(),"\n"
	"denorm_min():",lm::denorm_min());
}

int main()
{
//float
	print("------------------float--------------\n\n");
	check(0.0f);
	check(-0.0f);
	check(1.0f);
	check(-1.0f);
	check(1.0E7f);
	check(fast_io::bit_cast<float>(static_cast<std::uint32_t>(0x00800000))); // subnormal
	check(9999999.0f);
	check(0.001f);
	check(0.0009999999f);
	check(3.3554448E7f);
	check(8.999999E9f);
	check(3.4366717E10f);
	check(4.7223665E21f);
	check(8388608.0f);
	check(1.6777216E7f);
	check(3.3554436E7f);
	check(6.7131496E7f);
	check(1.9310392E-38f);
	check(-2.47E-43f);
	check(1.993244E-38f);
	check(4103.9003f);
	check(5.3399997E9f);
	check(6.0898E-39f);
	check(0.0010310042f);
	check(2.8823261E17f);
	check(7.038531E-26f);
	check(9.2234038E17f);
	check(6.7108872E7f);
	check(1.0E-44f);
	check(2.816025E14f);
	check(9.223372E18f);
	check(1.5846085E29f);
	check(1.1811161E19f);
	check(5.368709E18f);
	check(4.6143165E18f);
	check(0.007812537f);
	check(1.4E-45f);
	check(1.18697724E20f);
	check(1.00014165E-36f);
	check(200.0f);
	check(3.3554432E7f);
	check(1.26217745E-29f);
	check(1.1754944e-38f);
	check(1.17549430e-38f);
	test_speciail_case<float>();
	print("\n\n-----------------double--------------\n\n");
//double
	check(0.0);
	check(-0.0);
	check(1.0);
	check(-1.0);
	check(fast_io::bit_cast<double>(0x0010000000000000ULL)); // subnormal
	check(1.0E7);
	check(9999999.999999998);
	check(0.001);
	check(0.0009999999999999998);
	check(std::numeric_limits<double>::min());
	check(std::numeric_limits<double>::lowest());
	check(std::numeric_limits<double>::max());
	check(-2.109808898695963E16);
	check(4.940656E-318);
	check(1.18575755E-316);
	check(2.989102097996E-312);
	check(9.0608011534336E15);
	check(4.708356024711512E18);
	check(9.409340012568248E18);
	check(1.8531501765868567E21);
	check(-3.347727380279489E33);
	check(1.9430376160308388E16);
	check(-6.9741824662760956E19);
	check(4.3816050601147837E18);
	check(7.1202363472230444E-307);
	check(3.67301024534615E16);
	check(5.9604644775390625E-8);
	check(124781234971249873490812347589123570981235709813257801923578124781234971249873490812347589123570981235709813257801923578124781234971249873490812347589123570981235709813257801923578124781234971249873490812347589123570981235709813257801923578124781234971249873490812347589123570981235709813257801923578.0);
	test_speciail_case<double>();
	print("\n\n------------long double--------------\n\n");
//double
	check(0.0L);
	check(-0.0L);
	check(1.0L);
	check(-1.0L);
	check(1.0E7L);
	check(9999999.999999998L);
	check(0.001L);
	check(0.0009999999999999998L);
	check(-2.109808898695963E16L);
	check(4.940656E-318L);
	check(1.18575755E-316L);
	check(2.989102097996E-312L);
	check(9.0608011534336E15L);
	check(4.708356024711512E18L);
	check(9.409340012568248E18L);
	check(1.8531501765868567E21L);
	check(-3.347727380279489E33L);
	check(1.9430376160308388E16L);
	check(-6.9741824662760956E19L);
	check(4.3816050601147837E18L);
	check(7.1202363472230444E-307L);
	check(3.67301024534615E16L);
	check(5.9604644775390625E-8L);
	check(1.10169395793497080013e-4927L);
	check(1.1016939579349708003E-4927L);
	check(1.1016939579349708001E-4927L);
	test_speciail_case<long double>();
}