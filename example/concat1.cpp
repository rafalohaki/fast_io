#include<fast_io.h>

int main() {
    std::string weather{"sunny"};
    println(fast_io::concat("It's ",weather," day!"));
    // type of str is std::string as default
}
