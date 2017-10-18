#ifndef COOL_EBO_ALLOCATOR_H_
#define COOL_EBO_ALLOCATOR_H_

#include <cool/ebo_wrapper.h>
#include <memory>
#include <type_traits>
#include <utility>

///////////////////////////////////////////////////////////////////////////////
// ebo_allocator
//
//  ebo_allocator is an allocator (meets the C++17 allocator requirements)
//  which wraps another allocator.
//
//  The intended use is to inherit from ebo_allocator<A> and call
//  inner_allocator() to get a reference to the held allocator.
//
//  Assignment, copy construction, etc. are already implemented based on the
//  POC?A typedefs.
//
///////////////////////////////////////////////////////////////////////////////

namespace cool
{
    template<typename A>
    class ebo_allocator;

    template<typename L, typename R>
    constexpr bool operator==(ebo_allocator<L> const& l, ebo_allocator<R> const& r) noexcept
    { return l.inner_allocator() == r.inner_allocator(); }

    template<typename L, typename R>
    constexpr bool operator!=(ebo_allocator<L> const& l, ebo_allocator<R> const& r) noexcept
    { return !(l == r); }

    template<typename A>
    class ebo_allocator : private ebo_wrapper<A>
    {
        using traits = std::allocator_traits<A>;

        template<typename L, typename R>
        constexpr friend bool operator==(ebo_allocator<L> const&, ebo_allocator<R> const&) noexcept;

        template<typename U>
        friend class ebo_allocator;

    public:
        using inner_allocator_type                   = A;

        using value_type                             = typename      A::value_type;

        using pointer                                = typename traits::pointer;
        using const_pointer                          = typename traits::const_pointer;

        using void_pointer                           = typename traits::void_pointer;
        using const_void_pointer                     = typename traits::const_void_pointer;

        using size_type                              = typename traits::size_type;
        using difference_type                        = typename traits::difference_type;

        using propagate_on_container_copy_assignment = typename traits::propagate_on_container_copy_assignment;
        using propagate_on_container_move_assignment = typename traits::propagate_on_container_move_assignment;
        using propagate_on_container_swap            = typename traits::propagate_on_container_swap;
        using is_always_equal                        = typename traits::is_always_equal;

        inner_allocator_type      & inner_allocator()       noexcept
        { return ebo_wrapper<A>::ref(); }

        inner_allocator_type const& inner_allocator() const noexcept
        { return ebo_wrapper<A>::ref(); }

        template<typename U>
        struct rebind { using other = ebo_allocator<typename traits::template rebind_alloc<U>>; };

        template<typename U>
        using other = typename rebind<U>::other;

        // Constructors

        constexpr ebo_allocator() = default;

        template<typename U, typename = std::enable_if_t<!std::is_convertible_v<U, ebo_allocator>>>
        constexpr ebo_allocator(U&& u)
        noexcept(noexcept(ebo_wrapper<A>(std::forward<U>(u))))
        : ebo_wrapper<A>(std::forward<U>(u))
        {}

        template<typename U>
        constexpr ebo_allocator(ebo_allocator<U> const& that) noexcept
        : ebo_wrapper<A>(that.inner_allocator())
        {}

        template<typename U>
        constexpr ebo_allocator(ebo_allocator<U>&& that) noexcept
        : ebo_wrapper<A>(std::move(that.inner_allocator()))
        {}

        template<typename U0, typename U1, typename... Us>
        constexpr ebo_allocator(U0&& u0, U1&& u1, Us&&... us)
        noexcept(noexcept(ebo_wrapper<A>(std::forward<U0>, std::forward<U1>, std::forward<Us>(us)...)))
        : ebo_wrapper<A>(std::forward<U0>(u0), std::forward<U1>(u1), std::forward<Us>(us)...)
        {}

        ebo_allocator(ebo_allocator const& that) noexcept
        : ebo_wrapper<A>(traits::select_on_container_copy_construction(that.inner_allocator()))
        {}

        constexpr ebo_allocator& operator=(ebo_allocator const& that) noexcept
        {
            if constexpr(propagate_on_container_copy_assignment{})
                inner_allocator() = that.inner_allocator();

            return *this;
        }

        constexpr ebo_allocator& operator=(ebo_allocator&& that) noexcept
        {
            if constexpr(propagate_on_container_move_assignment{})
                inner_allocator() = std::move(that.inner_allocator());

            return *this;
        }

        constexpr friend void swap(ebo_allocator& l, ebo_allocator& r) noexcept
        {
            if constexpr(propagate_on_container_swap{})
            {
                using std::swap;
                swap(l.inner_allocator(), r.inner_allocator());
            }
        }

        pointer allocate(size_type n)
        { return traits::allocate(inner_allocator(), n); }

        pointer allocate(size_type n, const_void_pointer y)
        { return traits::allocate(inner_allocator(), n, y); }

        void deallocate(pointer p, size_type n)
        { traits::deallocate(inner_allocator(), p, n); }

        size_type max_size() const noexcept
        { return traits::max_size(inner_allocator()); }

        template<typename... Args>
        void construct(value_type* c, Args&&... args)
        { traits::construct(inner_allocator(), c, std::forward<Args>(args)...); }

        void destroy(value_type* c)
        { traits::destroy(inner_allocator(), c); }

        ebo_allocator select_on_container_copy_construction() const
        { return ebo_allocator(traits::select_on_container_copy_construction(inner_allocator())); }

        constexpr friend bool operator==(ebo_allocator const& l, ebo_allocator const& r) noexcept
        {
            if constexpr(is_always_equal{})
                return true;
            else
                return l.inner_allocator() == r.inner_allocator();
        }

        constexpr friend bool operator!=(ebo_allocator const& l, ebo_allocator const& r) noexcept
        { return !(l == r); }

    };

} // cool namespace

#endif /* COOL_EBO_ALLOCATOR_H_ */

