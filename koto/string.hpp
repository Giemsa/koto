// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_STRING_HPP
#define MW_KOTO_STRING_HPP

namespace koto
{
    template<typename E>
    class basic_string : public detail::string_base<E, detail::is_same<E, dynamic_encoding>::value>
    {
    public:
        typedef typename E::char_type char_type;
    private:
        template<typename V>
        friend class basic_string;

        template<typename V, size_t N>
        friend class basic_fixed_string;

        template<typename V, typename F>
        friend class detail::iterator;

        typedef basic_string<E> self_type;
        typedef E encoding_type;
        typedef std::char_traits<char_type> traits_type;
        typedef detail::string_base<E, detail::is_same<E, dynamic_encoding>::value> base_type;
    public:
        typedef basic_vchar_t<vchar_buffer_size, E> element_type;
        typedef detail::iterator<self_type, E> iterator;
        typedef detail::iterator<const self_type, E> const_iterator;

    private:
        class string_buffer_base
        {
        protected:
            size_t length_;   // string length
            size_t size_;     // byte size
            size_t capacity_;
            detail::counter ref_; // reference counting
        public:
            string_buffer_base(const size_t length, const size_t size, const size_t capacity)
            : length_(length), size_(size), capacity_(capacity), ref_(1)
            { }

            size_t length() const { return length_; }
            size_t size() const { return size_; }
            size_t capacity() const { return capacity_; }

            size_t unref() { return --ref_; }
            bool own() const { return ref_ == 1; }
        };

        class string_buffer_with_encoding : public string_buffer_base
        {
        private:
            const encoding *encoding_;

            string_buffer_with_encoding(const char_type *buffer, const size_t size, const size_t capacity, const encoding *encoding)
            : string_buffer_base(encoding->length(buffer, size), size, capacity), encoding_(encoding)
            {
                traits_type::copy(get_buffer(), buffer, size)[size] = 0;
            }
        public:
            string_buffer_with_encoding()
            : string_buffer_base(0, 0, 0)
            {
                get_buffer()[0] = 0;
            }

            ~string_buffer_with_encoding()
            { }

            template<size_t S>
            static string_buffer_with_encoding *create(const char_type (&buffer)[S], const encoding *encoding)
            {
                return new(allocator::allocate(sizeof(string_buffer_with_encoding) + sizeof(char_type) * S))
                    string_buffer_with_encoding(buffer, S - 1, S, encoding);
            }

            static string_buffer_with_encoding *create(const char_type *buffer, const size_t size, const size_t capacity, const encoding *encoding)
            {
                return new(allocator::allocate(sizeof(string_buffer_with_encoding) + sizeof(char_type) * capacity))
                    string_buffer_with_encoding(buffer, size, capacity, encoding);
            }

            static void destroy(string_buffer_with_encoding *p)
            {
                if(p != dummy_buffer_ && p->unref() == 0)
                {
                    p->~string_buffer_with_encoding();
                    allocator::deallocate(p);
                }
            }

            char_type *get_buffer() { return reinterpret_cast<char_type *>(this + 1); }
            const char_type *get_buffer() const { return reinterpret_cast<const char_type *>(this + 1); }
            const encoding *get_encoding() const { return encoding_; }

            void append(const string_buffer_with_encoding *buf)
            {
                traits_type::copy(get_buffer() + this->size_, buf->get_buffer(), buf->size_);
                this->size_ += buf->size_;
                this->length_ += buf->length_;
                get_buffer()[this->size_] = 0;
            }

            void append(const char_type c)
            {
                traits_type::copy(get_buffer() + this->size_, &c, sizeof(char_type));
                this->size_ += sizeof(char_type);
                this->length_ += sizeof(char_type);
                get_buffer()[this->size_] = 0;
            }

            void append(const char_type *v, const size_t size)
            {
                traits_type::copy(get_buffer() + this->size_, v, size);
                this->size_ += size;
                this->length_ += encoding_->length(v, size);
                get_buffer()[this->size_] = 0;
            }

            template<typename U>
            void append(const U &v)
            {
                char_type buf[detail::type_cast_info<U>::bufsize + 1];
                const char *e = util::to_string(buf, sizeof(buf), v);
                const size_t l = sizeof(buf) - 1 - (e - buf);
                traits_type::copy(get_buffer() + this->size_, e, l);
                this->size_ += l;
                this->length_ += l;
                get_buffer()[this->size_] = 0;
            }

            void assign(const char_type *str, const size_t size)
            {
                traits_type::copy(get_buffer(), str, size);
                this->size_ = size;
                this->length_ = encoding_->length(str, size);
                get_buffer()[size] = 0;
            }

            element_type element(const size_t index)
            {
                return encoding_->element(get_buffer(), index);
            }

            const element_type element(const size_t index) const
            {
                return encoding_->element(get_buffer(), index);
            }

            string_buffer_with_encoding *copy()
            {
                ++this->ref_;
                return this;
            }
        };

        class string_buffer /* final */ : public string_buffer_base
        {
        private:
            string_buffer(const char_type *buffer, const size_t size, const size_t capacity)
            : string_buffer_base(encoding_type::length(buffer, size), size, capacity)
            {
                traits_type::copy(get_buffer(), buffer, size)[size] = 0;
            }

            string_buffer(const char_type *buffer, const size_t len, const size_t size, const size_t capacity)
            : string_buffer_base(len, size, capacity)
            {
                traits_type::copy(get_buffer(), buffer, size)[size] = 0;
            }

        public:
            string_buffer()
            : string_buffer_base(0, 0, 0)
            {
                get_buffer()[0] = 0;
            }

            ~string_buffer()
            { }

            static string_buffer *create(const char_type *buffer, const size_t size)
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(char_type) * (size + 1)))
                    string_buffer(buffer, size, size + 1);
            }

            static string_buffer *create(const char_type *buffer, const size_t size, const size_t capacity, const encoding *)
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(char_type) * capacity))
                    string_buffer(buffer, size, capacity);
            }

            template<size_t S>
            static string_buffer *create_from_buf(const char_type (&buffer)[S])
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(char_type) * S))
                    string_buffer(buffer, S - 1, S);
            }

            template<size_t S>
            static string_buffer *create_from_buf(const char_type (&buffer)[S], const size_t len, const size_t size)
            {
                return new(allocator::allocate(sizeof(string_buffer) + sizeof(char_type) * S)) string_buffer(buffer, len, size, S);
            }

            static void destroy(string_buffer *p)
            {
                if(p != dummy_buffer_ && p->unref() == 0)
                {
                    p->~string_buffer();
                    allocator::deallocate(p);
                }
            }

            char_type *get_buffer() { return reinterpret_cast<char_type *>(this + 1); }
            const char_type *get_buffer() const { return reinterpret_cast<const char_type *>(this + 1); }
            const encoding *get_encoding() const { return NULL; }

            void append(const string_buffer *buf)
            {
                traits_type::copy(get_buffer() + this->size_, buf->get_buffer(), buf->size_);
                this->size_ += buf->size_;
                this->length_ += buf->length_;
                get_buffer()[this->size_] = 0;
            }

            void append(const char_type c)
            {
                traits_type::copy(get_buffer() + this->size_, &c, sizeof(char_type));
                this->size_ += sizeof(char_type);
                this->length_ += sizeof(char_type);
                get_buffer()[this->size_] = 0;
            }

            void append(const char_type *v, const size_t size)
            {
                traits_type::copy(get_buffer() + this->size_, v, size);
                this->size_ += size;
                this->length_ += encoding_type::length(v, size);
                get_buffer()[this->size_] = 0;
            }

            template<typename U>
            void append(const U &v)
            {
                char_type buf[detail::type_cast_info<U>::bufsize + 1];
                const char *e = util::to_string(buf, sizeof(buf), v);
                const size_t l = sizeof(buf) - (e - buf);
                traits_type::copy(get_buffer() + this->size_, e, l);
                this->size_ += l;
                this->length_ += l;
                get_buffer()[this->size_] = 0;
            }

            void assign(const char_type *str, const size_t size)
            {
                traits_type::copy(get_buffer(), str, size);
                this->size_ = size;
                this->length_ = encoding_type::length(str, size);
                get_buffer()[size] = 0;
            }

            template<bool C = encoding_type::accept_write_element>
            element_type element(const size_t index, const typename detail::enable_if<C>::type* = 0)
            {
                return encoding_type::element(get_buffer(), index);
            }

            const element_type element(const size_t index) const
            {
                return encoding_type::element(get_buffer(), index);
            }

            string_buffer *copy()
            {
                ++this->ref_;
                return this;
            }
        };

        typedef typename detail::select_type<
            base_type::is_dynamic_encoding,
            string_buffer_with_encoding, string_buffer
        >::type buffer_type;

    private:
        static buffer_type *dummy_buffer_;
        buffer_type *buffer_;

        static buffer_type *create_dummy_buffer()
        {
            static char buf[sizeof(buffer_type) + sizeof(char_type)];
            return new(buf) buffer_type();
        }

        char_type *get_buffer() { return buffer_->get_buffer(); }
        const char_type *get_buffer() const { return buffer_->get_buffer(); }

        bool expand(const size_t size)
        {
            buffer_type *buf = buffer_type::create(buffer_->get_buffer(), buffer_->size(), size, this->default_encoding_);
            buffer_type::destroy(buffer_);
            buffer_ = buf;
            return buf;
        }

        void expand_buffer(const size_t size)
        {
            const size_t s = buffer_->size() + size;
            if(s >= buffer_->capacity() || !buffer_->own())
            {
                // TODO: いい感じにサイズ増やす
                expand(s);
            }
        }

        void replace_buffer(const self_type &str)
        {
            buffer_type::destroy(buffer_);

            buffer_ = str.buffer_->copy();
        }

        template<typename U>
        self_type &_append(const U &v)
        {
            expand_buffer(detail::type_cast_info<U>::bufsize + 1);
            buffer_->append(v);
            return *this;
        }

    public:
        basic_string()
        : buffer_(dummy_buffer_)
        { }

        template<typename U>
        basic_string(
            const U &str,
            typename detail::enable_if<detail::is_array<U>::value && !base_type::is_dynamic_encoding>::type* = 0
        )
        : buffer_(buffer_type::create_from_buf(str))
        { }

        template<typename U>
        basic_string(
            const U &str,
            typename detail::enable_if<detail::is_array<U>::value && base_type::is_dynamic_encoding>::type* = 0
        )
        : buffer_(buffer_type::create(str, this->default_encoding_))
        { }

        template<typename U>
        basic_string(
            const U &str,
            const encoding *encoding,
            typename detail::enable_if<detail::is_array<U>::value && base_type::is_dynamic_encoding>::type* = 0
        )
        : buffer_(buffer_type::create(str, encoding))
        { }

        template<typename U>
        basic_string(
            const U &str,
            typename detail::enable_if<
                !detail::is_array<U>::value &&
                detail::is_same<typename detail::get_raw_type<U>::type, char_type>::value
            >::type* = 0
        )
        : buffer_(buffer_type::create(str, traits_type::length(str)))
        { }

        basic_string(const char_type *str, const size_t len)
        : buffer_(buffer_type::create(str, len))
        { }

        template<size_t S>
        basic_string(const basic_fixed_string<E, S> &str)
        : buffer_(buffer_type::create_from_buf(str.buffer_, str.length_, str.size_))
        { }

        // CoW
        basic_string(const basic_string &rhs)
        : buffer_(rhs.buffer_->copy())
        { }

        ~basic_string()
        {
            buffer_type::destroy(buffer_);
        }

        const char_type *c_str() const { return buffer_->get_buffer(); }
        size_t length() const { return buffer_->length(); }
        size_t size() const { return buffer_->size(); }
        size_t capacity() const { return buffer_->capacity(); }
        const encoding *get_encoding() const { return buffer_->get_encoding(); }

        // append
        self_type &append(const basic_string<E> &str)
        {
            expand_buffer(str.size());
            buffer_->append(str.buffer_);
            return *this;
        }

        self_type &append(const char_type *v, const size_t size)
        {
            expand_buffer(size);
            buffer_->append(v, size);
            return *this;
        }

        self_type &append(const char_type *v) { return append(v, traits_type::length(v)); }

        self_type &append(const char_type v)
        {
            expand_buffer(sizeof(char_type));
            buffer_->append(v);
            return *this;
        }

        self_type &append(const short v)              { return _append(v); }
        self_type &append(const unsigned short v)     { return _append(v); }
        self_type &append(const int v)                { return _append(v); }
        self_type &append(const unsigned int v)       { return _append(v); }
        self_type &append(const long v)               { return _append(v); }
        self_type &append(const unsigned long v)      { return _append(v); }

        self_type &append(const float v)              { return _append(v); }
        self_type &append(const double v)             { return _append(v); }
        /*
        self_type &append(const long long v)          { return _append(v); }
        self_type &append(const unsigned long long v) { return _append(v); }
        */

        // assign
        template<typename U>
        void assign(
            const U &str,
            typename detail::enable_if<
                !detail::is_array<U>::value &&
                detail::is_same<typename detail::get_raw_type<U>::type, char_type>::value
            >::type* = 0
        )
        {
            const size_t size = traits_type::length(str);
            expand_buffer(size);
            buffer_->assign(str, size);
        }

        template<typename U, size_t S>
        void assign(const U (&str)[S])
        {
            expand_buffer(S);
            buffer_->assign(str, S);
        }

        void assign(const self_type &str)
        {
            replace_buffer(str);
        }

        template<size_t S>
        void assign(const basic_fixed_string<E, S> &str)
        {
            expand_buffer(str.size());
            buffer_->assign(str.buffer_, str.size());
        }

        // at
        template<bool C = encoding_type::accept_write_element>
        typename detail::enable_if<C, element_type>::type at(const size_t index)
        {
            if(index >= buffer_->length())
            {
                throw std::out_of_range("out of range");
            }

            return buffer_->element(index);
        }

        const element_type at(const size_t index) const
        {
            if(index >= buffer_->length())
            {
                throw std::out_of_range("out of range");
            }

            return buffer_->element(index);
        }

        // operator
        template<typename U>
        typename detail::enable_if<
            !detail::is_array<U>::value &&
            detail::is_same<typename detail::get_raw_type<U>::type, char_type>::value
        , self_type>::type &operator=(const U &str)
        {
            assign<U>(str);
            return *this;
        }

        template<typename U, size_t S>
        self_type &operator=(const U (&str)[S])
        {
            assign<U, S>(str);
            return *this;
        }

        self_type &operator=(const self_type &str)
        {
            assign(str);
            return *this;
        }

        template<size_t S>
        self_type &operator=(const basic_fixed_string<E, S> &str)
        {
            assign(str);
            return *this;
        }

        template<bool C = encoding_type::accept_write_element>
        typename detail::enable_if<C, element_type>::type operator[](const size_t index)
        {
            return buffer_->element(index);
        }

        const element_type operator[](const size_t index) const
        {
            return buffer_->element(index);
        }

        // iterator
        iterator begin()
        {
            return iterator(this);
        }

        iterator end()
        {
            return iterator(this, get_buffer() + size());
        }

        const_iterator cbegin() const
        {
            return const_iterator(this);
        }

        const_iterator cend() const
        {
            return const_iterator(this, get_buffer() + size());
        }

        const_iterator begin() const { return cbegin(); }
        const_iterator end() const { return cend(); }
    };

    template<typename E>
    typename basic_string<E>::buffer_type *basic_string<E>::dummy_buffer_ = basic_string<E>::create_dummy_buffer();
}

#endif
