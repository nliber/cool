#ifndef COOL_DEFAULT_INIT_ALLOCATOR_H_
#define COOL_DEFAULT_INIT_ALLOCATOR_H_

#include <memory>

namespace cool
{
    template<typename T>
    class  default_init_allocator
    {
        using inner_alloc  = std::allocator<T>;
        using inner_traits = std::allocator_traits<inner_alloc>;

    public:
        using allocator_type = default_init_allocator;
        using value_type = T;

        using pointer                                = typename inner_traits::pointer;
        using const_pointer                          = typename inner_traits::const_pointer;
        using void_pointer                           = typename inner_traits::void_pointer;
        using const_void_pointer                     = typename inner_traits::const_void_pointer;

        using difference_type                        = typename inner_traits::difference_type;
        using size_type                              = typename inner_traits::size_type;

        using propagate_on_container_copy_assignment = typename inner_traits::propagate_on_container_copy_assignment;
        using propagate_on_container_move_assignment = typename inner_traits::propagate_on_container_move_assignment;
        using propagate_on_container_swap            = typename inner_traits::propagate_on_container_swap;
        using is_always_equal                        = typename inner_traits::is_always_equal;

        template<typename U>
        using rebind_alloc                           = default_init_allocator<U>;

        default_init_allocator() noexcept = default;
        default_init_allocator(default_init_allocator const&) noexcept = default;

        template<typename U>
        default_init_allocator(default_init_allocator<U> const&) noexcept {}

        ~default_init_allocator() = default;

        pointer allocate(size_type n)
        { inner_alloc a; return inner_traits::allocate(a, n); }

        pointer allocate(size_type n, const_void_pointer hint)
        { inner_alloc a; return inner_traits::allocate(a, n, hint); }

        void deallocate(pointer p, size_type n)
        { inner_alloc a; inner_traits::deallocate(a, p, n); }

        void construct(T* p)
        { ::new (static_cast<void*>(p)) T; }

        template<typename... Args>
        void construct(T* p, Args&&... args)
        { inner_alloc a; inner_traits::construct(a, p, std::forward<Args>(args)...); }

        void destroy(T* p)
        { inner_alloc a; inner_traits::destroy(a, p); }

        size_type max_size() const noexcept
        { inner_alloc a; return inner_traits::max_size(a); }

        allocator_type select_on_container_copy_construction(const allocator_type& rhs)
        { return rhs; }

    };

} // cool namespace


#endif /* COOL_DEFAULT_INIT_ALLOCATOR_H_ */

