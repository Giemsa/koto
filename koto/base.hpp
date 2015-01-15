// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_BASE_HPP
#define MW_KOTO_BASE_HPP

namespace koto
{
    namespace detail
    {
        template<typename T, typename E, bool D>
        class string_base
        {
        public:
            static const bool is_dynamic_encoding = D;
        private:
            typedef typename encoding_utf8<T>::dynamic_type default_dynamic_encoding;

            template<bool C = D>
            static const typename detail::enable_if<C, default_dynamic_encoding>::type *create_default_encoding()
            {
                static default_dynamic_encoding instance;
                return &instance;
            }

            template<bool C = D>
            static const typename detail::enable_if<!C, default_dynamic_encoding>::type *create_default_encoding()
            {
                return NULL;
            }
        protected:
            static const encoding<T> *default_encoding_;

        public:
            static void set_default_encoding(const encoding<T> *encoding)
            {
                default_encoding_ = encoding;
            }

            static const encoding<T> *get_default_encoding()
            {
                return default_encoding_;
            }
        };

        template<typename T, typename E, bool D>
        const encoding<T> *string_base<T, E, D>::default_encoding_ = string_base<T, E, D>::create_default_encoding();
    }
}

#endif
