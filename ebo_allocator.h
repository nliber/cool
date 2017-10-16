#ifndef COOL_EBO_ALLOCATOR_H_
#define COOL_EBO_ALLOCATOR_H_

#include <cool/ebo_wrapper.h>
#include <memory>

namespace cool
{
    template<typename A>
    class ebo_allocator : private ebo_wrapper<A>
    {
        using ebo_wrapper<A>::ref;
        using traits = std::allocator_traits<A>;

    public:
        using value_type                             = typename      A::value_type;

        using pointer                                = typename traits::pointer;
        using const_pointer                          = typename traits::const_pointer;

        using void_pointer                           = typename traits::void_pointer;
        using const_void_pointer                     = typename traits::const_void_pointer;

        using size_type                              = typename traits::size_type;
        using difference_type                        = typename traits::difference_type;

        using propagate_on_container_copy_assignment = typename traits::propagate_on_container_copy_assignment;
        using propagate_on_container_move_assignment = typename traits::propagate_on_container_move_assignment;
        using propagate_on_container_swap            = typename traits::propagate_on_container_move_swap;
        using is_always_equal                        = typename traits::is_always_equal;

        template<typename U>
        struct rebind { using other = ebo_allocator<typename traits::template rebind_alloc<U>>; };

        constexpr ebo_allocator() = default;

        template<typename U, typename = std::enable_if_t<!std::is_convertible_v<U, ebo_allocator>>>
        constexpr ebo_allocator(U&& u)
        noexcept(noexcept(ebo_wrapper<A>(std::forward<U>(u))))
        : ebo_wrapper<A>(std::forward<U>(u))
        {}

        template<typename U0, typename U1, typename... Us>
        constexpr ebo_allocator(U0&& u0, U1&& u1, Us&&... us)
        noexcept(noexcept(ebo_wrapper<A>(std::forward<U0>, std::forward<U1>, std::forward<Us>(us)...)))
        : ebo_wrapper<A>(std::forward<U0>(u0), std::forward<U1>(u1), std::forward<Us>(us)...)
        {}

        ebo_allocator(ebo_allocator const& that) noexcept
        : ebo_wrapper<A>(traits::select_on_copy_container_construction(that.ref()))
        {}

        constexpr ebo_allocator& operator=(ebo_allocator const& that) noexcept
        {
            if constexpr(propagate_on_container_copy_assignment{})
                ref() = that.ref();

            return *this;
        }

        constexpr ebo_allocator& operator=(ebo_allocator&& that) noexcept
        {
            if constexpr(propagate_on_container_move_assignment{})
                ref() = std::move(that.ref());

            return *this;
        }

        constexpr friend void swap(ebo_allocator& l, ebo_allocator& r) noexcept
        {
            if constexpr(propagate_on_container_swap{})
            {
                using std::swap;
                swap(l.ref(), r.ref());
            }
        }

        pointer allocate(size_type n)
        { return traits::allocate(ref(), n); }

        pointer allocate(size_type n, const_void_pointer y)
        { return traits::allocate(ref(), n, y); }

        void deallocate(pointer p, size_type n)
        { traits::deallocate(ref(), p, n); }

        size_type max_size() const noexcept
        { return traits::max_size(ref()); }

        template<typename... Args>
        void construct(value_type* c, Args&&... args)
        { traits::construct(ref(), c, std::forward<Args>(args)...); }

        void destroy(value_type* c)
        { traits::destroy(ref(), c); }

        ebo_allocator select_on_container_copy_construction() const
        { return ebo_allocator(traits::select_on_container_copy_construction(ref())); }

        constexpr bool friend operator==(ebo_allocator const& l, ebo_allocator const& r) noexcept
        {
            if constexpr(is_always_equal{})
                return true;
            else
                return l.ref() == r.ref();
        }

        constexpr bool friend operator!=(ebo_allocator const& l, ebo_allocator const& r) noexcept
        { return !(l == r); }

    };

} // cool namespace

#endif /* COOL_EBO_ALLOCATOR_H_ */

