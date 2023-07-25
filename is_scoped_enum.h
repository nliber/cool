#ifndef COOL_IS_SCOPED_ENUM_H_
#define COOL_IS_SCOPED_ENUM_H_

#include <type_traits>

namespace cool {
///////////////////////////////////////////////////////////////////////////
// underlying_type<E>
//
// This is the C++20 version of std::underlying_type backported to C++11
//
// If T is a complete enumeration (enum) type, provides a member typedef type
// that names the underlying type of T.
// Otherwise, if T is not an enumeration type, there is no member type.
// Otherwise (T is an incomplete enumeration type), the program is ill-formed.
///////////////////////////////////////////////////////////////////////////

#if __cplusplus >= 202002

using std::underlying_type;
using std::underlying_type_t;

#else

namespace detail {
template <typename E, bool = std::is_enum<E>::value>
struct underlying_type {};

template <typename E>
struct underlying_type<E, true> {
    using type = typename std::underlying_type<E>::type;
};
}  // namespace detail

template <typename E>
using underlying_type = detail::underlying_type<E>;

template <typename E>
using underlying_type_t = typename underlying_type<E>::type;

#endif

///////////////////////////////////////////////////////////////////////////
// to_underlying
//
// This is the C++23 version of std::to_underlying backported
// Converts an enumeration to its underlying type.
///////////////////////////////////////////////////////////////////////////

#if defined(__cpp_lib_to_underlying)

using std::to_underlying;

#else

template <typename E>
constexpr underlying_type_t<E> to_underlying(E e) noexcept {
    return static_cast<underlying_type_t<E> >(e);
}

#endif

///////////////////////////////////////////////////////////////////////////
// is_scoped_enum
//
// This is the C++ 23 version of std::to_underlying backported
// Checks whether T is a scoped enumeration type.
// Provides the member constant
// value which is equal to true, if T is a scoped enumeration type.
// Otherwise, value is equal to false.
///////////////////////////////////////////////////////////////////////////

#if defined(__cpp_lib_is_scoped_enum)

using std::is_scoped_enum;
using std::is_scoped_enum_v;

#else

namespace detail {
template <typename E, bool = std::is_enum<E>::value>
struct is_scoped_enum : std::false_type {};

template <typename E>
struct is_scoped_enum<E, true>
    : std::integral_constant<
          bool, !std::is_convertible<E, underlying_type_t<E> >::value> {};
}  // namespace detail

template <typename E>
using is_scoped_enum = typename detail::is_scoped_enum<E>::type;

template <typename E>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<E>::value;

#endif

}  // namespace cool

#endif /* IS_SCOPED_ENUM_H_ */

