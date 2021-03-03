#include<fast_io.h>
int main() {
    char ch{'Y'};
    println(ch);
    // this will print the ASCII code (89) of 'Y' instead of  `Y`
    println(fast_io::manipulators::chvw(ch));
}
