#include<fast_io.h>
int main() {
    char const* str{"correct usage: "};
    println(fast_io::concat(fast_io::manipulators::chvw(str)),
            fast_io::manipulators::chvw('Y'));
}
