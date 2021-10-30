#pragma once

namespace fast_io::details
{

template<typename T>
concept cxx_std_filesystem_pseudo_concept = requires(T t)
{
	typename T::value_type;
	{t.native().c_str()}->std::same_as<typename T::value_type const*>;
	{t.native().size()}->std::unsigned_integral;
	{t.root_name()};
};

}
