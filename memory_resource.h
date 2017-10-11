#ifndef COOL_PMR_MEMORY_RESOURCE_H_
#define COOL_PMR_MEMORY_RESOURCE_H_

// Neither gcc7 nor clang5 support std::pmr::memory_resource yet
//
// cool::pmr::memory_resource contains the following memory_resource type:
//  if we have <memory_resource>, then std::pmr::memory_resource
//  else std::experimental::pmr::memory_resource if vailable in <experimental/memory_resource>
//  else boost::container::pmr::memory_resource if available in <boost/container/pmr/memory_resource.hpp>
//  else we roll our own
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

#endif /* COOL_PMR_MEMORY_RESOURCE_H_ */

