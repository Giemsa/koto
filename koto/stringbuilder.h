#ifndef MW_KOTO_STRINGBUILDER_HPP
#define MW_KOTO_STRINGBUILDER_HPP

namespace koto
{
    namespace detail
    {
        template<typename T>
        class stringbuilder_base
        {
        public:
            class element /* final */
            {
            private:
                element() { }
                ~element() { }
            public:
                static element *create(const size_t size)
                {
                    new (allocator::allocate(sizeof(element) + sizeof(T) * size)) element();
                }

                static void destroy(element *p)
                {
                    p->~element();
                    allocator::deallocate(p);
                }

                T *get_buffer() { return reinterpret_cast<T *>(this + 1); }
                const T *get_buffer() const { return reinterpret_cast<const T *>(this + 1); }
            };
        };
    }

    template<typename E>
    class stringbuilder : private detail::stringbuilder_base<typename E::char_type>
    {
    private:
        size_t bufsize_;
    public:
        stringbuilder()
        { }

        stringbuilder(const size_t bufsize)
        : bufsize_(bufsize)
        { }
    };

    template<>
    class stringbuilder<dynamic_encoding> : private detail::stringbuilder_base<dynamic_encoding::char_type>
    {
    private:
        size_t bufsize_;
    public:
        stringbuilder()
        { }

        stringbuilder(const size_t bufsize)
        : bufsize_(bufsize)
        { }
    };
}

#endif
