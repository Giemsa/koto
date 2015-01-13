#ifndef MW_KOTO_EXCEPTION_HPP
#define MW_KOTO_EXCEPTION_HPP

#include <cstdarg>
#include <stdexcept>

namespace koto
{
    class exception : public std::logic_error
    {
    public:
        explicit exception(const std::string &msg)
        : logic_error(msg)
        { }
    };

    template<typename T, size_t S = 256>
    T create_exception(const char *format, ...)
    {
        char buf[S];
        va_list arg;
        va_start(arg, format);
        vsnprintf(buf, S, format, arg);
        va_end(arg);
        return T(buf);
    }
}

#endif
