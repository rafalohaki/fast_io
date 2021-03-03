#include<fast_io.h>

int main() {
    char const* str {"hello world!"};
    println("str = ",str);
    // this will print the pointer's value instead of the string.
    // See th correct solutions below:
    println("str = ",std::string_view{str});
    println("str = ",fast_io::manipulators::chvw(str));
}
