// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_ENCODING_HPP
#define MW_KOTO_ENCODING_HPP

#include "utf8.h"

namespace koto
{
    template<typename T>
    class encoding
    {
    private:
    public:
        encoding() { }
        virtual ~encoding() { }

        virtual size_t length(const T *str, const size_t len) const = 0;
    };

    template<typename T>
    class dynamic_encoding { };

    template<typename T, typename E>
    class static_encoding
    {
        class encoding_wrapper : public encoding<T>
        {
        public:
            encoding_wrapper() { }
            encoding_wrapper(const static_encoding<T, E> &enc) { }

            size_t length(const T *str, const size_t len) const // override
            {
                return E::length(str, len);
            }
        };
    public:
        typedef encoding_wrapper dynamic_type;
        typedef E static_type;
    };

    /* utf 8 */
    template<typename T>
    class encoding_utf8 /* final */ : public static_encoding<T, encoding_utf8<T> >
    {
    private:
    public:
        encoding_utf8() { }
        ~encoding_utf8() { }

        static size_t length(const T *str, const size_t len)
        {
            return utf8::distance(str, str + len);
        }
    };

    /* ascii */
    template<typename T>
    class encoding_ascii /* final */ : public static_encoding<T, encoding_ascii<T> >
    {
    private:
    public:
        encoding_ascii() { }
        ~encoding_ascii() { }

        static size_t length(const T *str, const size_t len)
        {
            return len;
        }
    };
}

#endif
