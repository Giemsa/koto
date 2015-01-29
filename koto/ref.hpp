// Copyright Giemsa 2015
// Distributed under the Boost Software License, Version 1.0

#ifndef MW_KOTO_REF_HPP
#define MW_KOTO_REF_HPP

namespace koto
{
    namespace detail
    {
        // reference counter (not thread safe)
        class counter
        {
        private:
            size_t count_;
        public:
            counter()
            : count_(0)
            { }

            counter(const size_t c)
            : count_(c)
            { }

            counter &operator++()
            {
                ++count_;
                return *this;
            }

            counter &operator--()
            {
                --count_;
                return *this;
            }

            operator size_t() const { return count_; }
            size_t operator==(const size_t n) { return count_ == n; }
        };
    }
}

#endif
