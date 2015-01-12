// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_STRING_HPP
#define MW_KOTO_STRING_HPP

namespace koto
{
    template<typename T, typename E>
    class basic_string
    {
        typedef basic_string<T, E> self_type;
        typedef E encoding_type;
        typedef typename encoding_utf8<T>::dynamic_type default_dynamic_encoding;

        class string_buffer_base
        {
        protected:
            size_t length_;   // string length
            size_t size_;     // byte size
            size_t capacity_;
        public:
            string_buffer_base(const size_t length, const size_t size, const size_t capacity)
            : length_(length), size_(size), capacity_(capacity)
            { }

            size_t length() const { return length_; }
            size_t size() const { return size_; }
            size_t capacity() const { return capacity_; }
        };

        class string_buffer_with_encoding : public string_buffer_base
        {
        private:
            encoding<T> *encoding_;

            string_buffer_with_encoding(const T *buffer, const size_t size, const size_t capacity, const encoding<T> &encoding)
            : string_buffer_base(encoding.length(buffer, size), size, capacity)
            {
                std::char_traits<T>::copy(get_buffer(), buffer, size)[size] = 0;
            }

            ~string_buffer_with_encoding()
            { }
        public:
            template<size_t S>
            static string_buffer_with_encoding *create(const T (&buffer)[S], const encoding<T> &encoding)
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(T) * S)) string_buffer_with_encoding(buffer, S - 1, S, encoding);
            }

            static void destroy(string_buffer_with_encoding *p)
            {
                p->~string_buffer_with_encoding();
                allocator::deallocate(p);
            }

            T *get_buffer() { return reinterpret_cast<T *>(this + 1); }

            void append(const T *str, const size_t len)
            {
                std::char_traits<T>::copy(get_buffer() + this->length_, str, len);
                this->length_ += len;
                get_buffer()[this->length_] = 0;
            }
        };

        class string_buffer /* final */ : public string_buffer_base
        {
        private:
            string_buffer(const T *buffer, const size_t size, const size_t capacity)
            : string_buffer_base(encoding_type::length(buffer, size), size, capacity)
            {
                std::char_traits<T>::copy(get_buffer(), buffer, size)[size] = 0;
            }

            string_buffer(const T *buffer, const size_t len, const size_t size, const size_t capacity)
            : string_buffer_base(len, size, capacity)
            {
                std::char_traits<T>::copy(get_buffer(), buffer, size)[size] = 0;
            }

            ~string_buffer()
            { }

        public:
            static string_buffer *create(const T *buffer, const size_t size)
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(T) * (size + 1))) string_buffer(buffer, size, size + 1);
            }

            static string_buffer *create(const T *buffer, const size_t size, const size_t capacity)
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(T) * capacity)) string_buffer(buffer, size, capacity);
            }

            template<size_t S>
            static string_buffer *create_from_buf(const T (&buffer)[S])
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(T) * S)) string_buffer(buffer, S - 1, S);
            }

            template<size_t S>
            static string_buffer *create_from_buf(const T (&buffer)[S], const size_t len, const size_t size)
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(T) * S)) string_buffer(buffer, len, size, S);
            }

            static void destroy(string_buffer *p)
            {
                p->~string_buffer();
                allocator::deallocate(p);
            }

            T *get_buffer() { return reinterpret_cast<T *>(this + 1); }
            const T *get_buffer() const { return reinterpret_cast<const T *>(this + 1); }

            void append(const string_buffer *buf)
            {
                std::char_traits<T>::copy(get_buffer() + this->size_, buf->get_buffer(), buf->size_);
                this->size_ += buf->size_;
                this->length_ += buf->length_;
                get_buffer()[this->size_] = 0;
            }
        };

        static const bool is_dynamic_encoding = detail::is_same<E, dynamic_encoding<T> >::value;

        typedef typename detail::select_type<
            is_dynamic_encoding,
            string_buffer_with_encoding, string_buffer
        >::type buffer_type;

    private:
        static const encoding<T> *default_encoding_;
        buffer_type *buffer_;
        bool has_buffer_;

        // keep default encoding
        template<bool C = is_dynamic_encoding>
        static const typename detail::enable_if<C, default_dynamic_encoding>::type *create_default_encoding()
        {
            static default_dynamic_encoding instance;
            return &instance;
        }

        template<bool C = is_dynamic_encoding>
        static const typename detail::enable_if<!C, default_dynamic_encoding>::type *create_default_encoding()
        {
            return NULL;
        }

        bool expand(const size_t size)
        {
            string_buffer *buf = string_buffer::create(buffer_->get_buffer(), buffer_->size(), size);
            if(has_buffer_)
            {
                string_buffer::destroy(buffer_);
            }
            else
            {
                has_buffer_ = true;
            }

            buffer_ = buf;
            return buf;
        }

        void expand_buffer(const size_t size)
        {
            const size_t s = buffer_->length() + size;
            if(s >= buffer_->capacity() || !has_buffer_)
            {
                // TODO: いい感じにサイズ増やす
                expand(s);
            }
        }

    public:
        typedef T char_type;

        template<typename U>
        basic_string(
            const U &str,
            typename detail::enable_if<detail::is_array<U>::value && !is_dynamic_encoding>::type* = 0
        )
        : buffer_(buffer_type::create_from_buf(str))
        , has_buffer_(true)
        { }

        template<typename U>
        basic_string(
            const U &str,
            typename detail::enable_if<detail::is_array<U>::value && is_dynamic_encoding>::type* = 0
        )
        : buffer_(buffer_type::create(str, *default_encoding_))
        , has_buffer_(true)
        { }

        template<typename U>
        basic_string(
            const U &str,
            const encoding<T> &encoding,
            typename detail::enable_if<detail::is_array<U>::value && is_dynamic_encoding>::type* = 0
        )
        : buffer_(buffer_type::create(str, encoding))
        , has_buffer_(true)
        { }

        template<typename U>
        basic_string(
            const U &str,
            typename detail::enable_if<
                !detail::is_array<U>::value &&
                detail::is_same<typename detail::remove_pointer<U>::type, T>::value
            >::type* = 0
        )
        : buffer_(buffer_type::create(str, std::char_traits<T>::length(str)))
        , has_buffer_(true)
        { }

        basic_string(const T *str, const size_t len)
        : buffer_(buffer_type::create(str, len))
        , has_buffer_(true)
        { }

        // CoW
        basic_string(const basic_string &rhs)
        : buffer_(rhs.buffer_)
        , has_buffer_(false)
        { }

        template<size_t S>
        basic_string(const basic_fixed_string<T, E, S> &str)
        : buffer_(buffer_type::create_from_buf(str.buffer_, str.length_, str.size_))
        , has_buffer_(true)
        { }

        ~basic_string()
        {
            if(has_buffer_)
            {
                buffer_type::destroy(buffer_);
            }
        }

        const T *c_str() const { return buffer_->get_buffer(); }
        size_t length() const { return buffer_->length(); }
        size_t size() const { return buffer_->size(); }
        size_t capacity() const { return buffer_->capacity(); }

        // append
        self_type &append(const T c)
        {
            return *this;
        }

        self_type &append(const unsigned int)
        {
            return *this;
        }

        self_type &append(const basic_string<T, E> &str)
        {
            expand_buffer(str.size());
            buffer_->append(str.buffer_);
            return *this;
        }

        static void set_default_encoding(const encoding<T> *encoding)
        {
            default_encoding_ = encoding;
        }

        static const encoding<T> *get_default_encoding()
        {
            return default_encoding_;
        }
    };

    template<typename T, typename E>
    const encoding<T> *basic_string<T, E>::default_encoding_ = basic_string<T, E>::create_default_encoding();
}

#endif
