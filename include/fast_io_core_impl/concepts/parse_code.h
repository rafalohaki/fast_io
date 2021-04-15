#pragma once

namespace fast_io
{

enum class parse_code:char8_t
{
ok = 0,
end_of_file = 1,
partial = 2,
invalid = 3,
overflow = 4
};

}