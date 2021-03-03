#include<fast_io.h>
int main() {
    char const* ptr{"hello,pointer?"};
    // this will not dereference ptr to string literal "hello,pointer?"
    // but the literal value of ptr itself.
    println(fast_io::concat("The address that ptr describes is "),ptr);
    // this prints the address that ptr descibes is 0X...
    // where the dots(...) represents the value of  ptr
}


