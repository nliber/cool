#ifndef COOL_DEFAULT_INIT_ALLOCATOR_H_
#define COOL_DEFAULT_INIT_ALLOCATOR_H_

#include <memory>

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
    namespace detail
    {
        // allocator_base is to use aggregation for final
        //  allocators vs. private inheritance for non-final allocators,
        //  the latter to take advantage of EBO
        template<typename A, typename = void>
        class allocator_base : A
        {
        protected:
            constexpr A      & inner()       noexcept { return *this; }
            constexpr A const& inner() const noexcept { return *this; }
        };

        template<typename A>
        class allocator_base<A, std::enable_if_t<std::is_final_v<A>>>
        {
        protected:
            constexpr A      & inner()       noexcept { return a; }
            constexpr A const& inner() const noexcept { return a; }

        private:
            A a;
        };
    } // detail namespace

    template<typename T, typename A = std::allocator<T>>
    class default_init_allocator : detail::allocator_base<A>
    {
        static_assert(std::is_empty_v<A>);

        using inner_traits = std::allocator_traits<A>;

        template<typename U>
        using rebind_inner = typename inner_traits::template rebind_alloc<U>;

        using detail::allocator_base<A>::inner;

    public:
        using allocator_type                         = default_init_allocator;

        using pointer                                = typename inner_traits::pointer;
        using const_pointer                          = typename inner_traits::const_pointer;
        using void_pointer                           = typename inner_traits::void_pointer;
        using const_void_pointer                     = typename inner_traits::const_void_pointer;

        using value_type                             = T;

        using size_type                              = typename inner_traits::size_type;
        using difference_type                        = typename inner_traits::difference_type;

        using propagate_on_container_copy_assignment = typename inner_traits::propagate_on_container_copy_assignment;
        using propagate_on_container_move_assignment = typename inner_traits::propagate_on_container_move_assignment;
        using propagate_on_container_swap            = typename inner_traits::propagate_on_container_swap;
        using is_always_equal                        = typename inner_traits::is_always_equal;

        template<typename U>
        using rebind_alloc                           = default_init_allocator<U, typename inner_traits::template rebind_alloc<U>>;

        template<typename U>
        struct rebind { using other = default_init_allocator<U, typename inner_traits::template rebind_alloc<U>>; };

        default_init_allocator() noexcept = default;
        default_init_allocator(default_init_allocator const&) noexcept = default;

        template<typename U>
        default_init_allocator(rebind_alloc<U> const&) noexcept {}

        ~default_init_allocator() = default;

        pointer allocate(size_type n)
        { return inner_traits::allocate(inner(), n); }

        pointer allocate(size_type n, const_void_pointer hint)
        { return inner_traits::allocate(inner(), n, hint); }

        void deallocate(pointer p, size_type n)
        { inner_traits::deallocate(inner(), p, n); }

        void construct(T* p)
        { ::new (static_cast<void*>(p)) T; }

        template<typename... Args>
        void construct(T* p, Args&&... args)
        { inner_traits::construct(inner(), p, std::forward<Args>(args)...); }

        void destroy(T* p)
        { inner_traits::destroy(inner(), p); }

        size_type max_size() const noexcept
        { return inner_traits::max_size(inner()); }

        allocator_type select_on_container_copy_construction(const allocator_type& rhs)
        { return rhs; }

        template<typename U>
        friend bool operator==(default_init_allocator const& l, rebind_alloc<U> const&)
        { return l.inner() == rebind_inner<U>(); }

        template<typename U>
        friend bool operator!=(default_init_allocator const& l, rebind_alloc<U> const& r)
        { return !(l == r); }
    };

} // cool namespace


#endif /* COOL_DEFAULT_INIT_ALLOCATOR_H_ */

