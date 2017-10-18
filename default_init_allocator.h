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
    { return static_cast<ebo_allocator<LA> const&>(l) == static_cast<ebo_allocator<RA> const&>(r); }

    template<typename LT, typename LA, typename RT, typename RA>
    constexpr bool operator!=(default_init_allocator<LT, LA> const& l, default_init_allocator<RT, RA> const& r) noexcept
    { return !(l == r); }

    template<typename T, typename A = std::allocator<T>>
    class default_init_allocator : private ebo_allocator<A>
    {
        template<typename TT, typename AA>
        friend class default_init_allocator;

        template<typename LT, typename LA, typename RT, typename RA>
        friend constexpr bool operator==(default_init_allocator<LT, LA> const&, default_init_allocator<RT, RA> const&) noexcept;

        using ebo_allocator<A>::inner_allocator;

    public:
        using typename ebo_allocator<A>::pointer;
        using typename ebo_allocator<A>::const_pointer;
        using typename ebo_allocator<A>::void_pointer;
        using typename ebo_allocator<A>::const_void_pointer;
        using typename ebo_allocator<A>::value_type;
        using typename ebo_allocator<A>::size_type;
        using typename ebo_allocator<A>::difference_type;
        using typename ebo_allocator<A>::propagate_on_container_copy_assignment;
        using typename ebo_allocator<A>::propagate_on_container_move_assignment;
        using typename ebo_allocator<A>::propagate_on_container_swap;
        using typename ebo_allocator<A>::is_always_equal;

        template<typename U>
        struct rebind { using other = default_init_allocator<U, typename std::allocator_traits<A>::template rebind_alloc<U>>; };

        default_init_allocator() = default;

        template<typename TT, typename AA>
        default_init_allocator(default_init_allocator<TT, AA> const& that)
        : ebo_allocator<A>(that.inner_allocator())
        {}

        using ebo_allocator<A>::allocate;
        using ebo_allocator<A>::deallocate;
        using ebo_allocator<A>::destroy;
        using ebo_allocator<A>::max_size;
        using ebo_allocator<A>::select_on_container_copy_construction;

        void construct(T* p)
        { ::new (static_cast<void*>(p)) T; }

        template<typename... Args>
        void construct(T* p, Args&&... args)
        { ebo_allocator<A>::construct(p, std::forward<Args>(args)...); }
    };

} // cool namespace

#endif /* COOL_DEFAULT_INIT_ALLOCATOR_H_ */

