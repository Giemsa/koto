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

    class buffer_too_small_error : public std::runtime_error
    {
    public:
        explicit buffer_too_small_error()
        : runtime_error("buffer too small")
        { }
    };

    template<typename T, size_t S = 128>
    T create_exception(const char *format, ...)
    {
        char buf[S];
        va_list arg;
        va_start(arg, format);
        vsnprintf(buf, S, format, arg);
        va_end(arg);
        return T(buf);
    }

    namespace detail
    {
        template<bool>
        struct compile_time_error;
        template<>
        struct compile_time_error<true> { };

#ifdef KOTO_USE_CPP11
#   define KOTO_STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
#   define KOTO_STATIC_ASSERT(expr, msg) { typedef char assert_failed [(expr) ? 1 : -1]; }
#endif
    }
}

#endif
