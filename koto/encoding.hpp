// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_ENCODING_HPP
#define MW_KOTO_ENCODING_HPP

#include "utf8.h"

namespace koto
{
    class encoding
    {
    protected:
        typedef basic_vchar_t<vchar_buffer_size, dynamic_encoding> element_type;
    public:
        encoding() { }
        virtual ~encoding() { }

        virtual size_t length(const char *str, const size_t len) const = 0;
        virtual element_type element(const char *str, const size_t index) const = 0;
        virtual element_type element(const char *str) const = 0;
        virtual void advance(char *&str, const size_t n) const = 0;
        virtual void advance(const char *&str, const size_t n) const = 0;
        virtual void next(char *&str) const = 0;
        virtual void next(const char *&str) const = 0;
        virtual void prev(char *&str) const = 0;
        virtual void prev(const char *&str) const = 0;

        virtual bool accept_write_element() const = 0;
    };

    class dynamic_encoding
    {
    public:
        typedef char char_type;
        static const bool accept_write_element = true;
    };

    template<typename E>
    class static_encoding
    {
        class encoding_wrapper : public encoding
        {
        public:
            encoding_wrapper() { }
            encoding_wrapper(const static_encoding<E> &enc) { }

            size_t length(const char *str, const size_t len) const // override
            {
                return E::length(str, len);
            }

            element_type element(const char *str, const size_t index) const // override
            {
                return E::template element<dynamic_encoding>(str, index).with(this);
            }

            element_type element(const char *str) const // override
            {
                return E::template element<dynamic_encoding>(str).with(this);
            }

            void advance(char *&str, const size_t n) const // override
            {
                return E::advance(str, n);
            }

            void advance(const char *&str, const size_t n) const // override
            {
                return E::advance(str, n);
            }

            void next(char *&str) const // override
            {
                return E::next(str);
            }

            void next(const char *&str) const // override
            {
                return E::next(str);
            }

            void prev(char *&str) const // override
            {
                return E::prev(str);
            }

            void prev(const char *&str) const // override
            {
                return E::prev(str);
            }

            bool accept_write_element() const { return E::accept_write_element; } // override
        };
    protected:
        template<typename T>
        struct valid
        {
            typedef typename detail::enable_if<
                detail::is_same<
                    typename detail::get_raw_type<T>::type,
                    typename E::char_type
                >::value,
                T
            >::type type;
        };
    public:
        typedef encoding_wrapper dynamic_type;
        typedef E static_type;
    };

    /* utf 8 */
    class encoding_utf8 /* final */ : public static_encoding<encoding_utf8>
    {
    public:
        typedef char char_type;
        static const bool accept_write_element = false;
    private:
    public:
        encoding_utf8() { }
        ~encoding_utf8() { }

        static size_t length(const char_type *str, const size_t len)
        {
            return utf8::distance(str, str + len);
        }

        template<typename E>
        static basic_vchar_t<vchar_buffer_size, E> element(const char_type *str, const size_t index)
        {
            utf8::unchecked::advance(str, index);
            const char_type *f = str;
            utf8::unchecked::next(str);
            return basic_vchar_t<vchar_buffer_size, E>(f, str - f);
        }

        template<typename E>
        static basic_vchar_t<vchar_buffer_size, E> element(const char_type *str)
        {
            const char_type *f = str;
            utf8::unchecked::next(str);
            return basic_vchar_t<vchar_buffer_size, E>(f, str - f);
        }

        template<typename T>
        static void advance(T *&str, const size_t n) { utf8::unchecked::advance(str, n); }
        template<typename T>
        static void next(T *&str) { utf8::unchecked::next(str); }
        template<typename T>
        static void prev(T *&str) { utf8::unchecked::previous(str); }
    };

    /* ascii */
    class encoding_ascii /* final */ : public static_encoding<encoding_ascii>
    {
    public:
        typedef char char_type;
        static const bool accept_write_element = true;
    private:
    public:
        encoding_ascii() { }
        ~encoding_ascii() { }

        static size_t length(const char_type *str, const size_t len) { return len; }

        template<typename E>
        static basic_vchar_t<vchar_buffer_size, E> element(const char_type *str, const size_t index)
        {
            return basic_vchar_t<vchar_buffer_size, E>(&str[index], 1);
        }

        template<typename E>
        static basic_vchar_t<vchar_buffer_size, E> element(const char_type *str)
        {
            return basic_vchar_t<vchar_buffer_size, E>(str, 1);
        }

        template<typename T>
        static void advance(T *&str, const size_t n) { str += n; }
        template<typename T>
        static void next(T *&str) { ++str; }
        template<typename T>
        static void prev(T *&str) { --str; }
    };
}

#endif
