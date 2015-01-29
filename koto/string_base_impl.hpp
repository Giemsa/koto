// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_STRING_BASE_IMPL_HPP
#define MW_KOTO_STRING_BASE_IMPL_HPP

namespace koto
{
    namespace detail
    {
        template<typename E, bool D>
        template<bool C>
        const typename detail::enable_if<C, const encoding>::type *string_base<E, D>::create_default_encoding()
        {
            static typename encoding_utf8::dynamic_type instance;
            return &instance;
        }
    }
}

#endif
