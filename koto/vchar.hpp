// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_VCHAR_HPP
#define MW_KOTO_VCHAR_HPP

namespace koto
{
    typedef unsigned int uint32;
    typedef unsigned short uint16;

    template<typename T, size_t S, typename E>
    class basic_vchar_t : public detail::string_base<T, E, detail::is_same<E, dynamic_encoding<T> >::value>
    {
        template<typename U, size_t N, typename F>
        friend std::ostream &operator<<(std::ostream& stream, const basic_vchar_t<U, N, F>& c);

        template<typename U, size_t N, typename F>
        friend std::istream &operator>>(std::istream& stream, const basic_vchar_t<U, N, F>& c);

        typedef detail::string_base<T, E, detail::is_same<E, dynamic_encoding<T> >::value> base_type;
        typedef basic_vchar_t<T, S, E> self_type;
        typedef E encoding_type;

        struct value_types
        {
            static const char char_value = 0;
            static const char u16_value  = 1;
            static const char u32_value  = 2;
        };

        typedef unsigned char value_type;

        class vchar_base
        {
            template<typename U, size_t N, typename F>
            friend class basic_vchar_t;
        protected:
            value_type value_type_;
            char char_buf_[S + 1];

            template<typename U>
            void write(const U *str, const size_t size)
            {
                if(size > S)
                {
                    throw bad_char_error();
                }

                static_cast<char *>(std::memcpy(&char_buf_, str, size * sizeof(U)))[size] = 0;
            }
        public:
           vchar_base(const value_type vt)
           : value_type_(vt)
           { }

            void input(std::istream &stream)
            {
                switch(value_type_)
                {
                    case value_types::char_value:
                        throw not_implemented();
                        break;
                    case value_types::u16_value:
                        throw not_implemented();
                        break;
                    case value_types::u32_value:
                        throw not_implemented();
                        break;
                }
            }

            void output(std::ostream &stream) const
            {
                switch(value_type_)
                {
                    case value_types::char_value:
                        stream << char_buf_;
                        break;
                    case value_types::u16_value:
                        throw not_implemented();
                        break;
                    case value_types::u32_value:
                        throw not_implemented();
                        break;
                }
            }

            const char *c_str() const { return char_buf_; }
        };

        class vchar : public vchar_base
        {
            template<typename U, size_t N, typename F>
            friend class basic_vchar_t;

            typedef vchar_base base;
        public:
            vchar(const char *str, const size_t size, const encoding<T> *enc)
            : base(value_types::char_value)
            {
                if(E::length(str, size) != 1)
                {
                    throw bad_char_error();
                }

                base::write(str, size);
            }

            vchar(const wchar_t *str, const size_t size, const encoding<T> *enc)
            : base(value_types::u32_value)
            {
                throw not_implemented();
                base::write(str, size);
            }

            vchar(const uint16 *str, const size_t size, const encoding<T> *enc)
            : base(value_types::u16_value)
            {
                throw not_implemented();
                base::write(str, size);
            }

            vchar(const uint32 *str, const size_t size, const encoding<T> *enc)
            : base(value_types::u32_value)
            {
                throw not_implemented();
                base::write(str, size);
            }
        };

        class vchar_with_encoding : public vchar_base
        {
            template<typename U, size_t N, typename F>
            friend class basic_vchar_t;

            typedef vchar_base base;
        private:
            encoding<T> *encoding_;
        public:
            /*
            vchar_with_encoding(const char *str, const size_t size, const encoding<T> *enc)
            : value_type_(value_types::char_value)
            {
                write(str, size);
            }

            vchar_with_encoding(const wchar_t *str, const size_t size, const encoding<T> *enc)
            : value_type_(value_types::u32_value)
            {
                write(str, size);
            }

            vchar_with_encoding(const uint16 *str, const size_t size, const encoding<T> *enc)
            : value_type_(value_types::u16_value)
            {
                write(str, size);
            }

            vchar_with_encoding(const uint32 *str, const size_t size, const encoding<T> *enc)
            : value_type_(value_types::u32_value)
            {
                write(str, size);
            }
            */
        };

        typedef typename detail::select_type<
            base_type::is_dynamic_encoding,
            vchar_with_encoding, vchar
        >::type vchar_type;
    private:
        vchar_type value_;

        value_type get_type() const { return value_.value_type_; }
        void input(std::istream &stream) { value_.input(stream); }
        void output(std::ostream &stream) const { value_.output(stream); }
    public:
        basic_vchar_t(const char *str, const size_t size, const encoding<T> *enc = base_type::default_encoding_)
        : value_(str, size, enc)
        { }

        basic_vchar_t(const char *str, const encoding<T> *enc = base_type::default_encoding_)
        : value_(str, std::char_traits<char>::length(str), enc)
        { }

        basic_vchar_t(const wchar_t *str, const size_t size)
        : value_(str, size)
        { }

        basic_vchar_t(const uint16 *str, const size_t size)
        : value_(str, size)
        { }

        basic_vchar_t(const uint32 *str, const size_t size)
        : value_(str, size)
        { }

        ~basic_vchar_t()
        {
        }

        const char *c_str() const { return value_.c_str(); }
    };

    typedef basic_vchar_t<default_char_type, 4, encoding_utf8<default_char_type> > vchar_t;

    // overload
    template<typename T, size_t S, typename E>
    std::ostream &operator<<(std::ostream& stream, const basic_vchar_t<T, S, E>& c)
    {
        c.output(stream);
        return stream;
    }

    template<typename T, size_t S, typename E>
    std::istream &operator>>(std::istream& stream, basic_vchar_t<T, S, E>& c)
    {
        c.input(stream);
        return stream;
    }
}

#endif
