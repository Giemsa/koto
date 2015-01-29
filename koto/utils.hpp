#ifndef MW_KOTO_UTILS_HPP
#define MW_KOTO_UTILS_HPP

namespace koto
{
    namespace util
    {
        template<typename T>
        static T *to_string(T *buf, unsigned int bufsize, unsigned int v)
        {
            buf += bufsize;
            do
            {
                *(--buf) = (v % 10) + '0';
            } while((v = v / 10));
            return buf;
        }

        template<typename T>
        static T *to_string(T *buf, unsigned int bufsize, int v)
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

        template<typename T>
        static T *to_string(T *buf, unsigned int bufsize, unsigned long v)
        {
            buf += bufsize;
            do
            {
                *(--buf) = (v % 10) + '0';
            } while((v = v / 10));
            return buf;
        }

        template<typename T>
        static T *to_string(T *buf, unsigned int bufsize, long v)
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
