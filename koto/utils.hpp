#ifndef MW_KOTO_UTILS_HPP
#define MW_KOTO_UTILS_HPP

namespace koto
{
    namespace util
    {
        template<typename T, typename N>
        T *to_string(
            T *buf,
            const unsigned int bufsize,
            N v,
            const typename detail::enable_if<detail::is_unsigned<N>::value, N>::type* = 0
        )
        {
            buf += bufsize;
            do
            {
                *(--buf) = (v % 10) + '0';
            } while((v = v / 10));
            return buf;
        }

        template<typename T, typename N>
        T *to_string(
            T *buf, const unsigned int bufsize,
            N v,
            const typename detail::enable_if<!detail::is_unsigned<N>::value, N>::type* = 0
        )
        {
            const bool s = v < 0 ? (v = -v, true) : false;
            buf += bufsize;
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
    };
}

#endif
