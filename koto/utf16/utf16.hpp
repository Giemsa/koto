#ifndef MW_KOTO_UTF16_HPP
#define MW_KOTO_UTF16_HPP

namespace koto
{
    namespace utf16
    {
        namespace detail
        {
            template<typename T>
            class utf16_base_t
            {
            private:
                template<typename U>
                static bool is_high(const U c)
                {
                    return (static_cast<char32_t>(c) & T::mask) == T::high_min;
                }

                template<typename U>
                static bool is_low(const U c)
                {
                    return (static_cast<char32_t>(c) & T::mask) == T::low_min;
                }

            public:
                template<typename U>
                static bool is_surrogate_pair(const U h, const U l)
                {
                    return is_high(h) && is_low(l);
                }
            };
        }

        class utf16le_t : public detail::utf16_base_t<utf16le_t>
        {
            friend class detail::utf16_base_t<utf16le_t>;
        private:
            // surrogate pair mask
            static const char32_t mask = 0x00FC;

            // surrogate pair range
            static const char16_t high_min = 0x00D8;
            static const char16_t high_max = 0xFFDB;
            static const char16_t low_min  = 0x00DC;
            static const char16_t low_max  = 0xFFDF;

        public:
        };

        class utf16be_t : public detail::utf16_base_t<utf16be_t>
        {
            friend class detail::utf16_base_t<utf16be_t>;
        private:
            // surrogate pair mask
            static const char32_t mask = 0xFC00;

            // surrogate pair range
            static const char16_t high_min = 0xD800;
            static const char16_t high_max = 0xDBFF;
            static const char16_t low_min  = 0xDC00;
            static const char16_t low_max  = 0xDFFF;

        public:

        };

        template<typename E, typename T>
        size_t length(T it, T end)
        {
            size_t len = 0;
            while(it != end)
            {
                const char16_t c = *it;
                if(c != 0)
                {
                    ++it;
                    ++len;
                    if(it != end && E::is_surrogate_pair(c, *it))
                    {
                        ++it;
                    }
                }
            }

            return len;
        }
    }
}

#endif
