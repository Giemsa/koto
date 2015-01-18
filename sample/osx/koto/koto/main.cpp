#include <iostream>

#define USE_DYNAMIC_ENCODING

/* install custom default allocator */
/*
namespace koto
{
    struct default_allocator
    {
        static void *allocate(const size_t size)
        {
            return std::malloc(size);
        }

        static void deallocate(void *p)
        {
            std::free(p);
        }
    };
}
*/

// include koto
#include "../../../../koto/koto.hpp"

int main(int argc, const char * argv[]) {
#ifdef USE_DYNAMIC_ENCODING
    typedef koto::basic_string<
        koto::default_char_type,
        koto::dynamic_encoding<koto::default_char_type>
    > string;
#else
    using namespace koto;
#endif

    koto::basic_fixed_string<koto::default_char_type, koto::dynamic_encoding<koto::default_char_type>, 21> fs = "123あいう";
    // koto::fixed<22>::string fs = "123あいう";

    koto::vchar_t v = "あ";
    std::cout << v << std::endl;

    string s = "12345あいうえお";
    string t;
    fs = s;
    std::cout << fs.c_str() << "," << fs.length() << "," << fs.size() << std::endl;
    return 0;
}
