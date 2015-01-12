#include <iostream>


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
    using namespace koto;

    fixed<21>::string s = "ABCあいう";
    string t = s;
    std::cout << t.c_str() << "," << t.length() << "," << t.size() << std::endl;
    t.append(t);
    std::cout << t.c_str() << "," << t.length() << "," << t.size() << std::endl;
    return 0;
}
