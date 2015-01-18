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

    typedef koto::basic_vchar_t<
        koto::default_char_type,
        koto::vchar_buffer_size,
        koto::dynamic_encoding<koto::default_char_type>
    > vchar_t;
#else
    using namespace koto;
#endif
    koto::encoding_ascii<koto::default_char_type>::dynamic_type enc;

    string::set_default_encoding(&enc);
    string s = "12345あいうえお";
    
    vchar_t c = s[4];
    string t;
    std::cout << /*s.c_str()*/ c << "," << s.length() << "," << s.size() << std::endl;
    return 0;
}
