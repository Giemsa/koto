// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_TYPE_TRAITS_HPP
#define MW_KOTO_TYPE_TRAITS_HPP

namespace koto
{
    namespace detail
    {
        struct true_type
        {
            static const bool value = true;
        };

        struct false_type
        {
            static const bool value = false;
        };

        template<typename T, typename U>
        struct is_same : false_type { };
         
        template<typename T>
        struct is_same<T, T> : true_type { };

        template<bool C, typename T = void>
        struct enable_if { };

        template<typename T>
        struct enable_if<true, T> { typedef T type; };

        template<typename T>
        struct is_array : false_type { };

        template<typename T, size_t N>
        struct is_array<T[N]> : true_type
        {
            static const size_t size = N;
        };

        template<typename T, T N, T M>
        struct min
        {
            static const T value = (N > M) ? M: N;
        };

        template<bool C, typename T, typename U>
        struct select_type
        {
            typedef U type;
        };

        template<typename T, typename U>
        struct select_type<true, T, U>
        {
            typedef T type;
        };

        template<typename T>
        struct remove_pointer                     { typedef T type; };
        template<typename T>
        struct remove_pointer<T *>                { typedef T type; };
        template<typename T>
        struct remove_pointer<T * const>          { typedef T type; };
        template<typename T>
        struct remove_pointer<T * volatile>       { typedef T type; };
        template<typename T>
        struct remove_pointer<T * const volatile> { typedef T type; };


        template<typename T>
        struct remove_const          { typedef T type; };
        template<typename T>
        struct remove_const<const T> { typedef T type; };

        template<typename T>
        struct remove_volatile             { typedef T type; };
        template<typename T>
        struct remove_volatile<volatile T> { typedef T type; };

        template<typename T>
        struct remove_cv
        {
            typedef typename remove_volatile<
                typename remove_const<T>::type
            >::type type;
        };

        template<typename T>
        struct get_raw_type
        {
            typedef typename remove_cv<
                typename remove_pointer<T>::type
            >::type type;
        };
    }
}

#endif
