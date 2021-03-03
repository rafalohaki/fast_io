#include<fast_io.h>

int main() {
    std::string answer{"42"};
    auto result{fast_io::to<uint32_t>(answer)};
    // tell to() what to do by specifying T
    println("the answer is ",result);
}
