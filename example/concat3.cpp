#include<fast_io.h>

int main() {
    print(fast_io::concatln("This example show how to use fast_io::concatln()",
                            "to generate a std::string ending with newline",
                            "character"));
    print(fast_io::concatln("This can also take more than 1 parameters.",
                            "And even floating number: ",3.14159));
}
