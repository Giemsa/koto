#ifndef MW_KOTO_UTILS_HPP
#define MW_KOTO_UTILS_HPP

#include <cmath>
#include <limits>

namespace koto
{
    namespace detail
    {
        template<typename T>
        struct type_cast_info
        {
            static const int bufsize = std::numeric_limits<T>::digits10 + 2;
            typedef T type;
        };

        template<>
        struct type_cast_info<float>
        {
            static const int bufsize = 32;
            typedef int type;
        };

        template<>
        struct type_cast_info<double>
        {
            static const int bufsize = 64;
            typedef long long type;
        };

        /*
        template<>
        struct cast_buffer_size<long double>
        {
            static const int value = 34;
            typedef long long type;
        };
        */
    }

    namespace util
    {
        template<typename T, typename N>
        T *to_string(
            T *buf,
            const unsigned int bufsize,
            N v,
            const typename detail::enable_if<
                detail::is_unsigned<N>::value &&
                !detail::is_floating_point<N>::value,
                N
            >::type* = 0
        )
        {
            buf += bufsize - 1;
            do
            {
                *(--buf) = (v % 10) + '0';
            } while((v = v / 10));
            return buf;
        }

        template<typename T, typename N>
        T *to_string(
            T *buf,
            const unsigned int bufsize,
            N v,
            const typename detail::enable_if<
                !detail::is_unsigned<N>::value &&
                !detail::is_floating_point<N>::value,
                N
            >::type* = 0
        )
        {
            const bool s = v < 0;

            buf += bufsize - 1;
            if(s)
            {
               *(--buf) = -(v % 10) + '0';
               v = -(v / 10);
            }

            do
            {
                *(--buf) = (v % 10) + '0';
            } while((v = v / 10));
            
            if(s)
            {
                *(--buf) = '-';
            }

            return buf;
        }

        template<typename T, typename N>
        T *to_string(
            T *buf,
            const unsigned int bufsize,
            N v,
            const typename detail::enable_if<detail::is_floating_point<N>::value, N>::type* = 0
        )
        {
            N _d;
            if(std::modf(v, &_d) == v)
            {
                return to_string(buf, bufsize, static_cast<typename detail::type_cast_info<N>::type>(v));
            }

            sprintf(buf, "%*f", bufsize - 1, v);
            while(*buf == ' ')
            {
                ++buf;
            }

            return buf;
        }
    };
}

#endif
