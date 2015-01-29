// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_STRING_BASE_HPP
#define MW_KOTO_STRING_BASE_HPP

namespace koto
{
    namespace detail
    {
        template<typename E, bool D>
        class string_base
        {
        public:
            static const bool is_dynamic_encoding = D;
        private:
            template<bool C = D>
            static const typename detail::enable_if<C, const encoding>::type *create_default_encoding();

            template<bool C = D>
            static const typename detail::enable_if<!C, const encoding>::type *create_default_encoding()
            {
                return NULL;
            }
        protected:
            static const encoding *default_encoding_;

        public:
            static void set_default_encoding(const encoding *encoding)
            {
                default_encoding_ = encoding;
            }

            static const encoding *get_default_encoding()
            {
                return default_encoding_;
            }
        };

        template<typename E, bool D>
        const encoding *string_base<E, D>::default_encoding_ = string_base<E, D>::create_default_encoding();
    }
}

#endif
