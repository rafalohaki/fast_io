#include<fast_io.h>

int main() {
    std::string with_point{"123456789"};
    std::string with_char{"123456A789"};
    std::string with_star{"123456*789"};
    // if the first character is illegal, 0 will be returned.
    std::string with_etc{"~!@#$%^&*()_+{}|:\"<>?`-=[]\\;',./'`"};

    println("with point: ",with_point," -> ",fast_io::to<int32_t>(with_point));
    println("with char: ",with_char," -> ",fast_io::to<int32_t>(with_char));
    println("with star: ",with_star," -> ",fast_io::to<int32_t>(with_star));
    println("with other: ",with_etc," -> ",fast_io::to<int32_t>(with_etc));
}
