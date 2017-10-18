#ifndef COOL_DEFAULT_INIT_ALLOCATOR_H_
#define COOL_DEFAULT_INIT_ALLOCATOR_H_

#include <memory>
#include <cool/ebo_allocator.h>

///////////////////////////////////////////////////////////////////////////////
// default_init_allocator
//
//  An allocator which default initializes (as opposed to value initializes)
//  its members when construct(T*) is called.
//
// Template parameters:
//  T - the value_type for the allocations
//  A - the underlying stateless allocator to call on all other operations
//      (defaults to std::allocator<T>)
///////////////////////////////////////////////////////////////////////////////
namespace cool
{
    template<typename T, typename A>
    class default_init_allocator;

    template<typename LT, typename LA, typename RT, typename RA>
    constexpr bool operator==(default_init_allocator<LT, LA> const& l, default_init_allocator<RT, RA> const& r) noexcept
    { return l.inner_allocator() == r.inner_allocator(); }

    template<typename LT, typename LA, typename RT, typename RA>
    constexpr bool operator!=(default_init_allocator<LT, LA> const& l, default_init_allocator<RT, RA> const& r) noexcept
    { return !(l == r); }

    template<typename T, typename A = std::allocator<T>>
    class default_init_allocator
    : private ebo_allocator<A>
    {
        template<typename TT, typename AA>
        friend class default_init_allocator;

        template<typename LT, typename LA, typename RT, typename RA>
        friend constexpr bool operator==(default_init_allocator<LT, LA> const&, default_init_allocator<RT, RA> const&) noexcept;

        using ebo_alloc = ebo_allocator<A>;
        using ebo_alloc::inner_allocator;

    public:
        using typename ebo_alloc::pointer;
        using typename ebo_alloc::const_pointer;
        using typename ebo_alloc::void_pointer;
        using typename ebo_alloc::const_void_pointer;
        using typename ebo_alloc::value_type;
        using typename ebo_alloc::size_type;
        using typename ebo_alloc::difference_type;
        using typename ebo_alloc::propagate_on_container_copy_assignment;
        using typename ebo_alloc::propagate_on_container_move_assignment;
        using typename ebo_alloc::propagate_on_container_swap;
        using typename ebo_alloc::is_always_equal;

        template<typename U>
        struct rebind { using other = default_init_allocator<U, typename std::allocator_traits<A>::template rebind_alloc<U>>; };

        constexpr default_init_allocator() = default;

        template<typename TT, typename AA>
        constexpr default_init_allocator(default_init_allocator<TT, AA> const& that) noexcept
        : ebo_alloc(that.inner_allocator())
        {}

        template<typename TT, typename AA>
        constexpr default_init_allocator(default_init_allocator<TT, AA>&& that) noexcept
        : ebo_alloc(std::move(that.inner_allocator()))
        {}

        template<typename U, typename = std::enable_if_t<!std::is_convertible_v<U, ebo_allocator>>>
        constexpr default_init_allocator(U&& u)
        noexcept(noexcept(ebo_alloc(std::forward<U>(u))))
        : ebo_alloc(std::forward<U>(u))
        {}

        template<typename U0, typename U1, typename... Us>
        constexpr default_init_allocator(U0&& u0, U1&& u1, Us... us)
        noexcept(noexcept(ebo_alloc(std::forward<U0>(u0), std::forward<U1>(u1), std::forward<Us>(us)...)))
        : ebo_alloc(std::forward<U0>(u0), std::forward<U1>(u1), std::forward<Us>(us)...)
        {}

        using ebo_alloc::allocate;
        using ebo_alloc::deallocate;
        using ebo_alloc::destroy;
        using ebo_alloc::max_size;
        using ebo_alloc::select_on_container_copy_construction;

        void construct(T* p)
        { ::new (static_cast<void*>(p)) T; }

        template<typename... Args>
        void construct(T* p, Args&&... args)
        { ebo_alloc::construct(p, std::forward<Args>(args)...); }
    };

} // cool namespace

#endif /* COOL_DEFAULT_INIT_ALLOCATOR_H_ */

