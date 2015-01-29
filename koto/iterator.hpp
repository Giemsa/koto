// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_ITERATOR_HPP
#define MW_KOTO_ITERATOR_HPP

#include <iterator>

namespace koto
{
    namespace detail
    {
        template<typename T, typename E>
        class bidirectional_iterator : public std::iterator<std::bidirectional_iterator_tag, typename T::element_type>
        {
            template<typename F>
            friend class koto::basic_string;

            template<typename U, typename F>
            friend class bidirectional_iterator;

            typedef bidirectional_iterator<T, E> self_type;
        private:
            T *str_;
            typename detail::select_type<
                detail::is_const<T>::value,
                const typename E::char_type,
                typename E::char_type
            >::type *buffer_;

            bidirectional_iterator(T *str)
            : str_(str), buffer_(str->get_buffer())
            { }

            bidirectional_iterator(T *str, typename E::char_type *buffer)
            : str_(str), buffer_(buffer)
            { }
        public:
            bidirectional_iterator(const self_type &it)
            : str_(it.str_), buffer_(it.buffer_)
            { }

            template<typename U>
            bidirectional_iterator(const bidirectional_iterator<U, E> &it)
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

            basic_vchar_t<vchar_buffer_size, E> operator*()
            {
                return E::template element<E>(buffer_);
            }

            const basic_vchar_t<vchar_buffer_size, E> operator*() const
            {
                return E::template element<E>(buffer_);
            }

            basic_vchar_t<vchar_buffer_size, E> operator->()
            {
                return E::template element<E>(buffer_);
            }

            const basic_vchar_t<vchar_buffer_size, E> operator->() const
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

        template<typename T>
        class bidirectional_iterator<
            T, dynamic_encoding
        > : public std::iterator<std::bidirectional_iterator_tag, typename T::element_type>
        {
            template<typename F>
            friend class koto::basic_string;

            template<typename U, typename F>
            friend class bidirectional_iterator;

            typedef dynamic_encoding encoding_type;
            typedef bidirectional_iterator<T, encoding_type> self_type;
        private:
            T *str_;
            typename detail::select_type<
                detail::is_const<T>::value,
                const encoding_type::char_type,
                encoding_type::char_type
            >::type *buffer_;

            bidirectional_iterator(T *str)
            : str_(str), buffer_(str->get_buffer())
            { }

            bidirectional_iterator(T *str, encoding_type::char_type *buffer)
            : str_(str), buffer_(buffer)
            { }
        public:
            bidirectional_iterator(const self_type &it)
            : str_(it.str_), buffer_(it.buffer_)
            { }

            template<typename U>
            bidirectional_iterator(const bidirectional_iterator<U, encoding_type> &it)
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

            basic_vchar_t<vchar_buffer_size, encoding_type> operator*()
            {
                return str_->get_encoding()->template element(buffer_);
            }

            const basic_vchar_t<vchar_buffer_size, encoding_type> operator*() const
            {
                return str_->get_encoding()->template element(buffer_);
            }

            basic_vchar_t<vchar_buffer_size, encoding_type> operator->()
            {
                return str_->get_encoding()->template element(buffer_);
            }

            const basic_vchar_t<vchar_buffer_size, encoding_type> operator->() const
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
