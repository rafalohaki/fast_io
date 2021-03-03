#include"../include/fast_io.h"
int main() {
    std::wstring wstr1{L"hello, "};
    auto wstr2{fast_io::concat<std::wstring>(wstr1,L"world!")};
    println(fast_io::wc_stdout(),wstr2);
}
