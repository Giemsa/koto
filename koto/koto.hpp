// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_HPP
#define MW_KOTO_HPP

#include <limits>
#include "type_traits.hpp"
#include "allocator.hpp"
#include "config.hpp"
#include "forward.hpp"
#include "exception.hpp"
#include "vchar.hpp"
#include "encoding.hpp"
#include "base.hpp"
#include "string.hpp"
#include "fixed_string.hpp"

namespace koto
{
#define SET_DEFAULT_ENCODING_(E) typedef encoding_##E<KOTO_DEFAULT_CHAR_TYPE> default_encoding
#define SET_DEFAULT_ENCODING(E) SET_DEFAULT_ENCODING_(E)
    SET_DEFAULT_ENCODING(KOTO_DEFAULT_ENCODING);
#undef SET_DEFAULT_ENCODING
#undef SET_DEFAULT_ENCODING_

    typedef basic_string<default_char_type, default_encoding::static_type > string;
    typedef basic_string<default_char_type, encoding_ascii<default_char_type>::static_type > simple_string;

    template<size_t S>
    struct fixed
    {
        typedef basic_fixed_string<default_char_type, default_encoding::static_type, S> string;
        typedef basic_fixed_string<default_char_type, encoding_ascii<default_char_type>::static_type, S> simple_string;
    };

    namespace denc
    {
        typedef basic_string<default_char_type, dynamic_encoding<default_char_type> > string;
        template<size_t S>
        struct fixed
        {
            typedef basic_fixed_string<default_char_type, dynamic_encoding<default_char_type>, S> string;
        };
    }
}

#include "undef_macros.hpp"
#endif
