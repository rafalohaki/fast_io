#include<fast_io.h>

int main() {
    std::string str;
    fast_io::ostring_ref ref{str};
    print(ref,"hello world",14," fa ",16846);

    println("result of do concat with print(): ",str);

    println("result of do concat with concat(): ",
            fast_io::concat("hello world",14," fa ",16846));
}
