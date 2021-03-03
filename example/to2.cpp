#include<fast_io.h>
int main() {
    std::string part1{"123"};
    std::string part2{"456"};
    std::string part3{"789"};

    auto result = fast_io::to<int32_t>(part1,part2,part3);
    println("the result is: ",result);
}
