#ifndef COOL_PMR_MEMORY_RESOURCE_H_
#define COOL_PMR_MEMORY_RESOURCE_H_

// Neither gcc7 nor clang5 support std::pmr::memory_resource yet
//
// cool::pmr::memory_resource contains the following memory_resource type:
//  if we have <memory_resource>, then std::pmr::memory_resource
//  else std::experimental::pmr::memory_resource if vailable in <experimental/memory_resource>
//  else boost::container::pmr::memory_resource if available in <boost/container/pmr/memory_resource.hpp>
#if __has_include(<memory_resource>)
#include <memory_resource>
namespace cool::pmr { using namespace std::pmr; }
#elif __has_include(<experimental/memory_resource>)
#include <experimental/memory_resource>
namespace cool::pmr { using namespace std::experimental::pmr; }
#elif __has_include(<boost/container/pmr/memory_resource.hpp>) && \
      __has_include(<boost/container/pmr/polymorphic_allocator.hpp>) && \
      __has_include(<boost/container/pmr/pool_options.hpp>) && \
      __has_include(<boost/container/pmr/synchronized_pool_resource.hpp>) && \
      __has_include(<boost/container/pmr/unsynchronized_pool_resource.hpp>) && \
      __has_include(<boost/container/pmr/monotonic_buffer_resource.hpp>)
#include <boost/container/pmr/memory_resource.hpp>
#include <boost/container/pmr/polymorphic_allocator.hpp>
#include <boost/container/pmr/pool_options.hpp>
#include <boost/container/pmr/synchronized_pool_resource.hpp>
#include <boost/container/pmr/unsynchronized_pool_resource.hpp>
#include <boost/container/pmr/monotonic_buffer_resource.hpp>
namespace cool::pmr { using namespace boost::container::pmr; }
#endif

#endif /* COOL_PMR_MEMORY_RESOURCE_H_ */

