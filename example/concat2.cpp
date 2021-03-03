#include<fast_io.h>

int main() {
    std::size_t answer{42u};
    std::string color{"red"};
    println(fast_io::concat("The answer is ",answer, 
                            "and the color is ",color,"."));
}
