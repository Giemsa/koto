// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_ALLOCATOR_HPP
#define MW_KOTO_ALLOCATOR_HPP

#include <cstdlib>

namespace koto
{
    class default_allocator;

    namespace detail
    {
        template<typename T>
        int is_complete_helper(int (*)[sizeof(T)]);

        template<typename>
        char is_complete_helper(...);

        template<typename T>
        struct is_complete
        {
            static const bool value = sizeof(is_complete_helper<T>(0)) != sizeof(char);
        };
    }

    struct fallback_allocator
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

    typedef typename detail::select_type<
        detail::is_complete<default_allocator>::value,
        default_allocator,
        fallback_allocator
    >::type allocator;
}

#endif
