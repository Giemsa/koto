// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_DEFINES_HPP
#define MW_KOTO_DEFINES_HPP

namespace koto
{
    // in encoding.hpp
    class encoding;
    class dynamic_encoding;
    class encoding_utf8;
    class encoding_ascii;

    // in base.hpp
    /*
    namespace detail
    {
        template<typename E, bool D>
        class string_base;
    }
    */

    // in vchar.h
    template<size_t S, typename E>
    class basic_vchar_t;

    // in string.hpp
    template<typename E>
    class basic_string;

    // in fixed_string.hpp
    template<typename E, size_t S>
    class basic_fixed_string;
}

#endif
