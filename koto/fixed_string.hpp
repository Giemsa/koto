// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_FIXED_STRING_HPP
#define MW_KOTO_FIXED_STRING_HPP

namespace koto
{
    template<typename T, typename E, size_t S>
    class basic_fixed_string
    {
        template<typename U, typename V>
        friend class basic_string;

        template<typename U, typename V, size_t N>
        friend class basic_fixed_string;

        typedef E encoding_type;
    private:
        T buffer_[S];
        size_t size_, length_;

    public:
        template<typename U>
        basic_fixed_string(const U &str, typename detail::enable_if<detail::is_array<U>::value>::type* = 0)
        {
            const size_t s = std::min(S, std::char_traits<T>::length(str));
            std::char_traits<T>::copy(buffer_, str, s);
            size_ = s;
            buffer_[s] = 0;
            length_ = encoding_type::length(buffer_, s);
        }

        template<typename U>
        basic_fixed_string(const U &str, typename detail::enable_if<!detail::is_array<U>::value>::type* = 0)
        {
            static const size_t s = detail::min<size_t, S, detail::is_array<U>::size>::value;
            std::char_traits<T>::copy(buffer_, str, s);
            size_ = s;
            buffer_[s] = 0;
            length_ = encoding_type::length(buffer_, s);
        }

        basic_fixed_string(const basic_string<T, E> &str)
        {
            const size_t s = std::min(S, str.length());
            std::char_traits<T>::copy(buffer_, str.c_str(), s);
            size_ = s;
            buffer_[s] = 0;
            length_ = encoding_type::length(buffer_, s);
        }

        template<size_t N>
        basic_fixed_string(const basic_fixed_string<T, E, N> &rhs)
        {
            static const size_t s = detail::min<size_t, S, N>::value;
            std::char_traits<T>::copy(buffer_, rhs.buffer_, s);
            size_ = s;
            buffer_[s] = 0;
            length_ = encoding_type::length(buffer_, s);
        }

        const T *c_str() const { return buffer_; }
        size_t length() const { return length_; }
        size_t size() const { return size_; }
    };
}

#endif
