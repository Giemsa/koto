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
        koto::dynamic_encoding
    > string;

    typedef koto::basic_vchar_t<
        koto::vchar_buffer_size,
        koto::dynamic_encoding
    > vchar_t;
#else
    using namespace koto;
#endif
    // koto::encoding_utf8::dynamic_type enc;

    //string::set_default_encoding(&enc);
    /*
    string s;
    {
        string t = "こんにちは123ABCこんばんは";
        s = t;
    }

    s.append(2355948379894859379834789375498379623.0);
    s.append("かきくけこ");

    for(string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        std::cout << *it << ",";
    }

    std::cout << "\n";
    std::cout << s.length() << std::endl;
    */

    char16_t bs[] = {
        0x3042, 0xD842, 0xDFB7, 0x0000
        //0x4230, 0x42DB, 0xB7DF, 0x0000
    };

    koto::basic_string<koto::encoding_utf16BE> str = bs;

    std::cout << str.length() << std::endl;

    return 0;
}
