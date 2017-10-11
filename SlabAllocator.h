#ifndef COOL_SLABALLOCATOR_H_
#define COOL_SLABALLOCATOR_H_

#include <memory>
#include <vector>

// Neither gcc7 nor clang5 support std::pmr::memory_resource
// so use std::experimental::pmr::memory_resource
// and dump it in namespace cool::pmr
#if __has_include(<memory_resource>)
#include <memory_resource>
namespace cool::pmr { using namespace std::pmr; }
#elif __has_include(<experimental/memory_resource>)
#include <experimental/memory_resource>
namespace cool::pmr { using namespace std::experimental::pmr; }
#elif __has_include(<boost/container/pmr/memory_resource.hpp>)
#include <boost/container/pmr/memory_resource.hpp>
namespace cool::pmr { using namespace boost::container::pmr; }
#else
#include <cstddef>
namespace cool::pmr
{
    class memory_resource
    {
    public:
        virtual ~memory_resource() = default;

        void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t))
        { return do_allocate(bytes, alignment); }

        void deallocate(void* p, size_t bytes, size_t alignment = alignof(std::max_align_t))
        { return do_deallocate(p, bytes, alignment); }

        bool is_equal(const memory_resource& other) const noexcept
        { return do_is_equal(other); }

        friend bool operator==(const memory_resource& a, const memory_resource& b) noexcept
        { return &a == &b || a.is_equal(b); }

        friend bool operator!=(const memory_resource& a, const memory_resource& b) noexcept
        { return !(a == b); }

    private:
        virtual void* do_allocate(size_t bytes, size_t alignment) = 0;
        virtual void do_deallocate(void* p, size_t bytes, size_t alignment) = 0;

        virtual bool do_is_equal(const memory_resource& other) const noexcept = 0;
    };

} // cool::pmr namespace
#endif

namespace cool
{

class SlabMemoryResource : public pmr::memory_resource
{
public:
    constexpr static size_t defaultslabsize = 2 * 1024 * 1024;

    explicit SlabMemoryResource(size_t slabsize = defaultslabsize) noexcept
    : m_slabsize{slabsize}
    {}

    // Make it moveable 
    SlabMemoryResource(SlabMemoryResource&& that) noexcept
    : SlabMemoryResource{that.m_slabsize}
    { swap(*this, that); }

    SlabMemoryResource& operator=(SlabMemoryResource&& that) noexcept
    { swap(*this, that); return *this; }

    // Make it swappable (since we use swap for moving)
    friend void swap(SlabMemoryResource& l, SlabMemoryResource& r) noexcept
    {
        using std::swap;

        swap(l.m_free,     r.m_free);
        swap(l.m_space,    r.m_space);
        swap(l.m_slabsize, r.m_slabsize);
        swap(l.m_slabs,    r.m_slabs);
    }

    void* allocateBytes(size_t size, size_t alignment = alignof(std::max_align_t))
    {
        // Will the allocation fit in the current slab?
        if (void* allocated = std::align(alignment, size, m_free, m_space))
        {
            m_free   = static_cast<uint8_t*>(m_free) + size;
            m_space -= size;

            return allocated;
        }

        // Allocate and track a new slab
        size_t   slabsize = std::max(m_slabsize, size);
        uint8_t* slab     = m_slabs.emplace_back(std::make_unique<uint8_t[]>(slabsize)).get();

        // Are we only using part of this new slab?
        if (size != slabsize)
        {
            m_free  = slab + size;
            m_space = slabsize - size;
        }

        return slab;
    }

    template<typename T>
    T* allocateUninitialized(size_t n = 1, size_t alignment = alignof(T))
    { return static_cast<T*>(allocateBytes(n * sizeof(T), alignment)); }

private:
    // pmr::memory_resource virtual functions

    void* do_allocate(size_t bytes, size_t alignment) override
    { return allocateBytes(bytes, alignment); }

    void do_deallocate(void* /* p */, size_t /* bytes */, size_t /* alignment */) override {}

    bool do_is_equal(const memory_resource& /* other */) const noexcept override
    { return true; }


    struct Slabs : std::vector<std::unique_ptr<uint8_t[]>> {};

    void*  m_free = nullptr;
    size_t m_space = 0;
    size_t m_slabsize;
    Slabs  m_slabs;
};

template<typename T>
class SlabAllocator
{
public:
    constexpr static size_t defaultslabsize = SlabMemoryResource::defaultslabsize;
    using memory_resource_pointer                = std::shared_ptr<SlabMemoryResource>;

    using value_type                             = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;

    explicit SlabAllocator(size_t slabsize = defaultslabsize)
    : m_slabmemoryresource{std::make_shared<SlabMemoryResource>(slabsize)}
    {}

    SlabAllocator(SlabMemoryResource&& smr)
    : m_slabmemoryresource{std::make_shared<SlabMemoryResource>(std::move(smr))}
    {}

    SlabAllocator(std::shared_ptr<SlabMemoryResource> sp)
    : m_slabmemoryresource{std::move(sp)}
    {}

    template<typename U>
    SlabAllocator(const SlabAllocator<U>& u) noexcept
    : m_slabmemoryresource{u.get_memory_resource()}
    {}

    T* allocate(size_t n)
    { return m_slabmemoryresource->allocateUninitialized<T>(n); }

    void deallocate(T* /* p */, size_t /* n */) noexcept {}

    memory_resource_pointer get_memory_resource() const noexcept
    { return m_slabmemoryresource; }

    friend bool operator==(SlabAllocator const& l, SlabAllocator const& r) noexcept
    { return *l.m_slabmemoryresource == *r.m_slabmemoryresource; }

    friend bool operator!=(SlabAllocator const& l, SlabAllocator const& r) noexcept
    { return !(l == r); }

private:
    memory_resource_pointer m_slabmemoryresource;
};

} // cool namespace

#endif /* COOL_SLABALLOCATOR_H_ */

