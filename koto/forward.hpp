// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_DEFINES_HPP
#define MW_KOTO_DEFINES_HPP

namespace koto
{
    // in encoding.hpp
    template<typename T>
    class encoding;

    template<typename T>
    class dynamic_encoding;

    template<typename T>
    class encoding_utf8;

    template<typename T>
    class encoding_ascii;

    // int base.hpp
    namespace detail
    {
        template<typename T, typename E, bool D>
        class string_base;
    }

    // in string.hpp
    template<typename T, typename E>
    class basic_string;

    // in fixed_string.hpp
    template<typename T, typename E, size_t S>
    class basic_fixed_string;
}

#endif
