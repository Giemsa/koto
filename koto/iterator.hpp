// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_ITERATOR_HPP
#define MW_KOTO_ITERATOR_HPP

#include <iterator>

namespace koto
{
    namespace detail
    {
        template<typename T, typename U, typename E>
        class bidirectional_iterator : public std::iterator<std::bidirectional_iterator_tag, typename T::element_type>
        {
            template<typename V, typename F>
            friend class koto::basic_string;

            template<typename V, typename W, typename F>
            friend class bidirectional_iterator;

            typedef bidirectional_iterator<T, U, E> self_type;
        private:
            T *str_;
            U *buffer_;

            bidirectional_iterator(T *str)
            : str_(str), buffer_(str->get_buffer())
            { }

            bidirectional_iterator(T *str, U *buffer)
            : str_(str), buffer_(buffer)
            { }
        public:
            bidirectional_iterator(const self_type &it)
            : str_(it.str_), buffer_(it.buffer_)
            { }

            template<typename V, typename W>
            bidirectional_iterator(const bidirectional_iterator<V, W, E> &it)
            : str_(it.str_), buffer_(it.buffer_)
            { }

            self_type &operator++()
            {
                E::next(buffer_);
                return *this;
            }

            self_type operator++(int)
            {
                self_type i = *this;
                E::next(buffer_);
                return i;
            }

            self_type &operator--()
            {
                E::prev(buffer_);
                return *this;
            }

            self_type operator--(int)
            {
                self_type i = *this;
                E::prev(buffer_);
                return i;
            }

            basic_vchar_t<typename T::char_type, vchar_buffer_size, E> operator*()
            {
                return E::template element<E>(buffer_);
            }

            const basic_vchar_t<typename T::char_type, vchar_buffer_size, E> operator*() const
            {
                return E::template element<E>(buffer_);
            }

            basic_vchar_t<typename T::char_type, vchar_buffer_size, E> operator->()
            {
                return E::template element<E>(buffer_);
            }

            const basic_vchar_t<typename T::char_type, vchar_buffer_size, E> operator->() const
            {
                return E::template element<E>(buffer_);
            }

            bool operator==(const self_type& rhs)
            {
                return buffer_ == rhs.buffer_;
            }

            bool operator!=(const self_type& rhs)
            {
                return buffer_ != rhs.buffer_;
            }
        };

        template<typename T, typename U>
        class bidirectional_iterator<
            T, U, dynamic_encoding<typename T::char_type>
        > : public std::iterator<std::bidirectional_iterator_tag, typename T::element_type>
        {
            template<typename V, typename F>
            friend class koto::basic_string;

            template<typename V, typename W, typename F>
            friend class bidirectional_iterator;

            typedef dynamic_encoding<typename T::char_type> encoding_type;
            typedef bidirectional_iterator<T, U, encoding_type > self_type;
        private:
            T *str_;
            U *buffer_;

            bidirectional_iterator(T *str)
            : str_(str), buffer_(str->get_buffer())
            { }

            bidirectional_iterator(T *str, U *buffer)
            : str_(str), buffer_(buffer)
            { }
        public:
            bidirectional_iterator(const self_type &it)
            : str_(it.str_), buffer_(it.buffer_)
            { }

            template<typename V, typename W>
            bidirectional_iterator(const bidirectional_iterator<V, W, encoding_type > &it)
            : str_(it.str_), buffer_(it.buffer_)
            { }

            self_type &operator++()
            {
                str_->get_encoding()->next(buffer_);
                return *this;
            }

            self_type operator++(int)
            {
                self_type i = *this;
                str_->get_encoding()->::next(buffer_);
                return i;
            }

            self_type &operator--()
            {
                str_->get_encoding()->::prev(buffer_);
                return *this;
            }

            self_type operator--(int)
            {
                self_type i = *this;
                str_->get_encoding()->::prev(buffer_);
                return i;
            }

            basic_vchar_t<typename T::char_type, vchar_buffer_size, encoding_type> operator*()
            {
                return str_->get_encoding()->template element(buffer_);
            }

            const basic_vchar_t<typename T::char_type, vchar_buffer_size, encoding_type> operator*() const
            {
                return str_->get_encoding()->template element(buffer_);
            }

            basic_vchar_t<typename T::char_type, vchar_buffer_size, encoding_type> operator->()
            {
                return str_->get_encoding()->template element(buffer_);
            }

            const basic_vchar_t<typename T::char_type, vchar_buffer_size, encoding_type> operator->() const
            {
                return str_->get_encoding()->template element(buffer_);
            }

            bool operator==(const self_type& rhs)
            {
                return buffer_ == rhs.buffer_;
            }

            bool operator!=(const self_type& rhs)
            {
                return buffer_ != rhs.buffer_;
            }
        };
    }
}

#endif
