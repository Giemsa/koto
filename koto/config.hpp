// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_CONFIG_HPP
#define MW_KOTO_CONFIG_HPP


// koto default config
#ifndef KOTO_DEFAULT_ENCODING
#   define KOTO_DEFAULT_ENCODING utf8
#endif

#ifndef KOTO_DISABLE_CPP11
#   define KOTO_DISABLE_CPP11 0
#endif

#undef KOTO_USE_CPP11
#if KOTO_DISABLE_CPP11 == 0
#   if defined(_MSC_VER) && _MSC_VER >= 1800
#      define KOTO_USE_CPP11
#   elif __cplusplus > 199711L
#       define KOTO_USE_CPP11
#   endif
#endif

namespace koto
{
    static const int vchar_buffer_size = 4;

#if __cplusplus <= 199711L
    typedef unsigned short char16_t;
    typedef unsigned int char32_t;
#endif
}

#endif
