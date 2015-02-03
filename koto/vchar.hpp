// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_VCHAR_HPP
#define MW_KOTO_VCHAR_HPP

namespace koto
{
    typedef unsigned int uint32;
    typedef unsigned short uint16;

    template<size_t S, typename E>
    class basic_vchar_t : public detail::string_base<E, detail::is_same<E, dynamic_encoding>::value>
    {
        typedef detail::string_base<E, detail::is_same<E, dynamic_encoding>::value> base_type;
        typedef basic_vchar_t<S, E> self_type;
        typedef E encoding_type;

        typedef unsigned char value_type;

        class vchar_base
        {
            template<size_t N, typename F>
            friend class basic_vchar_t;
        protected:
            char char_buf_[S];
            unsigned char size_;

            template<typename U>
            void write(const U *str, const size_t size)
            {
                if(size > S / sizeof(U))
                {
                    throw bad_char_error();
                }

                std::memcpy(&char_buf_, str, size * sizeof(U));
            }
        public:
           vchar_base(const size_t size)
           : size_(size)
           { }

            template<typename T>
            void input(T &stream)
            {
                throw not_implemented();
            }

            template<typename T>
            void output(T &stream) const
            {
                stream.write(reinterpret_cast<const typename T::char_type *>(char_buf_), size_);
            }

            const char *c_str() const { return char_buf_; }
        };

        class vchar : public vchar_base
        {
            template<size_t N, typename F>
            friend class basic_vchar_t;

            typedef vchar_base base;
        public:
            template<typename T>
            vchar(const T *str, const size_t size, const encoding *enc)
            : base(size)
            {
                base::write(str, size);
            }
        };

        class vchar_with_encoding : public vchar_base
        {
            template<size_t N, typename F>
            friend class basic_vchar_t;

            typedef vchar_base base;
        private:
            const encoding *encoding_;
        public:
            template<typename T>
            vchar_with_encoding(const T *str, const size_t size, const encoding *enc)
            : base(size)
            {
                base::write(str, size);
            }

            void set_encoding(const encoding *enc)
            {
                encoding_ = enc;
            }
        };

        typedef typename detail::select_type<
            base_type::is_dynamic_encoding,
            vchar_with_encoding, vchar
        >::type vchar_type;
    private:
        vchar_type value_;

        value_type get_type() const { return value_.value_type_; }
    public:
        basic_vchar_t(const char *str, const size_t size, const encoding *enc = base_type::default_encoding_)
        : value_(str, size, enc)
        { }

        basic_vchar_t(const char *str, const encoding *enc = base_type::default_encoding_)
        : value_(str, std::char_traits<char>::length(str), enc)
        { }

        basic_vchar_t(const wchar_t *str, const size_t size, const encoding *enc = base_type::default_encoding_)
        : value_(str, size, enc)
        { }

        basic_vchar_t(const char16_t *str, const size_t size, const encoding *enc = base_type::default_encoding_)
        : value_(str, size, enc)
        { }

        basic_vchar_t(const char32_t *str, const size_t size, const encoding *enc = base_type::default_encoding_)
        : value_(str, size, enc)
        { }

        ~basic_vchar_t()
        {
        }

        template<bool C = base_type::is_dynamic_encoding>
        self_type &with(const encoding *enc, const typename detail::enable_if<C>::type* = 0)
        {
            value_.set_encoding(enc);
            return *this;
        }

        const char *c_str() const { return value_.c_str(); }

        // overload
        template<size_t N, typename F>
        friend std::ostream &operator<<(std::ostream& stream, const basic_vchar_t<N, F>& c)
        {
            c.value_.output(stream);
            return stream;
        }

        template<size_t N, typename F>
        friend std::wostream &operator<<(std::wostream& stream, const basic_vchar_t<N, F>& c)
        {
            c.value_.output(stream);
            return stream;
        }

        template<size_t N, typename F>
        friend std::istream &operator>>(std::istream& stream, const basic_vchar_t<N, F>& c)
        {
            c.value_.input(stream);
            return stream;
        }

        template<size_t N, typename F>
        friend std::wistream &operator>>(std::wistream& stream, const basic_vchar_t<N, F>& c)
        {
            c.value_.input(stream);
            return stream;
        }
    };

    typedef basic_vchar_t<vchar_buffer_size, encoding_utf8> vchar_t;
}

#endif
