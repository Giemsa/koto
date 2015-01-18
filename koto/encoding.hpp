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
    protected:
        typedef basic_vchar_t<T, vchar_buffer_size, dynamic_encoding<T> > char_type;
    private:
    public:
        encoding() { }
        virtual ~encoding() { }

        virtual size_t length(const T *str, const size_t len) const = 0;
        virtual bool accept_write_element() const = 0;
        virtual char_type element(const T *str, const size_t index) = 0;
        virtual const char_type element(const T *str, const size_t index) const = 0;
    };

    template<typename T>
    class dynamic_encoding
    {
    public:
        static const bool accept_write_element = true;
    };

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

            typename encoding<T>::char_type element(const T *str, const size_t index) // override
            {
                return E::template element<dynamic_encoding<T> >(str, index).with(this);
            }

            const typename encoding<T>::char_type element(const T *str, const size_t index) const // override
            {
                return E::template element<dynamic_encoding<T> >(str, index).with(this);
            }

            bool accept_write_element() const { return E::accept_write_element; } // override
        };
    public:
        typedef encoding_wrapper dynamic_type;
        typedef E static_type;
    };

    /* utf 8 */
    template<typename T>
    class encoding_utf8 /* final */ : public static_encoding<T, encoding_utf8<T> >
    {
    public:
        static const bool accept_write_element = false;
    private:
    public:
        encoding_utf8() { }
        ~encoding_utf8() { }

        static size_t length(const T *str, const size_t len)
        {
            return utf8::distance(str, str + len);
        }

        template<typename E>
        static basic_vchar_t<T, vchar_buffer_size, E> element(const T *str, const size_t index)
        {
            utf8::unchecked::advance(str, index);
            const T *f = str;
            utf8::unchecked::next(str);
            return basic_vchar_t<T, vchar_buffer_size, E>(f, str - f);
        }
    };

    /* ascii */
    template<typename T>
    class encoding_ascii /* final */ : public static_encoding<T, encoding_ascii<T> >
    {
    public:
        static const bool accept_write_element = true;
    private:
    public:
        encoding_ascii() { }
        ~encoding_ascii() { }

        static size_t length(const T *str, const size_t len)
        {
            return len;
        }

        template<typename E>
        static basic_vchar_t<T, vchar_buffer_size, E> element(const T *str, const size_t index)
        {
            return basic_vchar_t<T, vchar_buffer_size, E>(&str[index], 1);
        }
    };
}

#endif
