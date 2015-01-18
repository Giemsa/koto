// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_FIXED_STRING_HPP
#define MW_KOTO_FIXED_STRING_HPP

namespace koto
{
    template<typename T, typename E, size_t S>
    class basic_fixed_string : public detail::string_base<T, E, detail::is_same<E, dynamic_encoding<T> >::value>
    {
        template<typename U, typename V>
        friend class basic_string;

        template<typename U, typename V, size_t N>
        friend class basic_fixed_string;

        typedef E encoding_type;
        typedef basic_fixed_string<T, E, S> self_type;
        typedef std::char_traits<T> traits_type;
        typedef detail::string_base<T, E, detail::is_same<E, dynamic_encoding<T> >::value> base_type;
    public:
        typedef T char_type;
    private:
        class string_buffer_base
        {
        protected:
            T buffer_[S];
            size_t size_, length_;
        public:
            string_buffer_base(const T *str, const size_t size, const size_t length)
            : size_(size), length_(length)
            {
                traits_type::copy(this->buffer_, str, size);
                this->buffer_[size] = 0;
            }

            T *get_buffer() { return buffer_; }
            const T *get_buffer() const { return buffer_; }

            size_t length() const { return length_; }
            size_t size() const { return size_; }
            size_t capacity() const { return S; }

            void assign(const T *str, const size_t size, const size_t length)
            {
                traits_type::copy(this->buffer_, str, size);
                this->buffer_[size] = 0;
                this->size_ = size;
                this->length_ = length;
            }
        };

        class string_buffer : public string_buffer_base
        {
        public:
            string_buffer(const T *str, const size_t size, const encoding<T> *encoding)
            : string_buffer_base(str, size, encoding_type::length(str, size))
            { }

            string_buffer(const T *str, const size_t size)
            : string_buffer_base(str, size, encoding_type::length(str, size))
            { }

            void assign(const T *str, const size_t size)
            {
                string_buffer_base::assign(str, size, encoding_type::length(str, size));
            }
        };

        class string_buffer_with_encoding : public string_buffer_base
        {
        private:
            const encoding<T> *encoding_;
        public:
            string_buffer_with_encoding(const T *str, const size_t size, const encoding<T> *encoding)
            : string_buffer_base(str, size, encoding->length(str, size)), encoding_(encoding)
            { }

            void assign(const T *str, const size_t size)
            {
                string_buffer_base::assign(str, size, encoding_->length(str, size));
            }
        };

        typedef typename detail::select_type<
            base_type::is_dynamic_encoding,
            string_buffer_with_encoding, string_buffer
        >::type buffer_type;

    private:
        buffer_type buffer_;

    public:
        template<typename U>
        basic_fixed_string(
            const U &str,
            typename detail::enable_if<
                !detail::is_array<U>::value &&
                detail::is_same<typename detail::get_raw_type<U>::type, T>::value
            >::type* = 0
        )
        : buffer_(str, std::min(S, std::char_traits<T>::length(str)), this->default_encoding_)
        { }

        template<typename U>
        basic_fixed_string(
            const U &str,
            typename detail::enable_if<detail::is_array<U>::value && base_type::is_dynamic_encoding>::type* = 0
        )
        : buffer_(str, detail::min<size_t, S, detail::is_array<U>::size>::value, this->default_encoding_)
        { }

        template<typename U>
        basic_fixed_string(
            const U &str,
            typename detail::enable_if<detail::is_array<U>::value && !base_type::is_dynamic_encoding>::type* = 0
        )
        : buffer_(str, detail::min<size_t, S, detail::is_array<U>::size>::value)
        { }

        template<typename U>
        basic_fixed_string(
            const U &str,
            const encoding<T> *encoding,
            typename detail::enable_if<detail::is_array<U>::value && base_type::is_dynamic_encoding>::type* = 0
        )
        : buffer_(str, detail::min<size_t, S, detail::is_array<U>::size>::value, encoding)
        { }

        basic_fixed_string(const basic_string<T, E> &str)
        : buffer_(str.get_buffer(), std::min(S, str.length()), this->default_encoding_)
        { }

        template<size_t N>
        basic_fixed_string(const basic_fixed_string<T, E, N> &rhs)
        : buffer_(rhs.buffer_, detail::min<size_t, S, N>::value, this->default_encoding_)
        { }

        const T *c_str() const { return buffer_.get_buffer(); }
        size_t length() const { return buffer_.length(); }
        size_t size() const { return buffer_.size(); }
        size_t capacity() const { return S; }

        // assign
        template<typename U>
        void assign(
            const U &str,
            typename detail::enable_if<
                !detail::is_array<U>::value &&
                detail::is_same<typename detail::get_raw_type<U>::type, T>::value
            >::type* = 0
        )
        {
            const size_t size = traits_type::length(str);
            if(size >= S - 1)
            {
                throw buffer_too_small_error();
            }

            buffer_.assign(str, size);
        }

        template<typename U, size_t N>
        void assign(
            const U (&str)[N],
            const typename detail::enable_if<N <= S>::type* = 0
        )
        {
            buffer_.assign(str, N - 1);
        }

        void assign(const basic_string<T, E> &str)
        {
            if(str.size() >= S)
            {
                throw buffer_too_small_error();
            }

            buffer_.string_buffer_base::assign(str.get_buffer(), str.size(), str.length());
        }

        template<size_t N>
        void assign(
            const basic_fixed_string<T, E, N> &str,
            const typename detail::enable_if<N <= S>::type* = 0
        )
        {
            buffer_.string_buffer_base::assign(str.buffer_.get_buffer(), str.size(), str.length());
        }

        // operator
        template<typename U>
        typename detail::enable_if<
            !detail::is_array<U>::value &&
            detail::is_same<typename detail::get_raw_type<U>::type, T>::value
        , self_type>::type &operator=(const U &str)
        {
            assign<U>(str);
            return *this;
        }

        template<typename U, size_t N>
        self_type &operator=(const U (&str)[N])
        {
            KOTO_STATIC_ASSERT(N <= S, "buffer too small");
            assign<U, N>(str);
            return *this;
        }

        self_type &operator=(const basic_string<T, E> &str)
        {
            assign(str);
            return *this;
        }

        template<size_t N>
        self_type &operator=(const basic_fixed_string<T, E, N> &str)
        {
            KOTO_STATIC_ASSERT(N <= S, "buffer too small");
            assign(str);
            return *this;
        }
    };
}

#endif
