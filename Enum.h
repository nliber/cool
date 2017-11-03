#ifndef COOL_ENUM_H_
#define COOL_ENUM_H_

#include <cool/Out.h>
#include <ostream>
#include <type_traits>

#define COOL_ENUM_OPS(E)                                           \
static_assert(std::is_enum_v<E>);                                  \
                                                                   \
inline constexpr std::underlying_type_t<E> operator+(E e) noexcept \
{ return static_cast<std::underlying_type_t<E>>(e); }              \
                                                                   \
inline constexpr bool operator!(E e) noexcept                      \
{ return !+e; }                                                    \
                                                                   \
inline std::ostream& operator<<(std::ostream& os, E e)             \
{ return os << cool::Out<E, true>{e}; }                            \

#define COOL_ENUM_BIT_OPS(E)                                       \
COOL_ENUM_OPS(E)                                                   \
                                                                   \
inline constexpr E operator~(E e) noexcept                         \
{ return static_cast<E>(~+e); }                                    \
                                                                   \
inline constexpr E operator&(E e1, E e2) noexcept                  \
{ return static_cast<E>(+e1 & +e2); }                              \
                                                                   \
inline constexpr E& operator&=(E& e1, E e2) noexcept               \
{ return e1 = e1 & e2; }                                           \
                                                                   \
inline constexpr E operator^(E e1, E e2) noexcept                  \
{ return static_cast<E>(+e1 ^ +e2); }                              \
                                                                   \
inline constexpr E& operator^=(E& e1, E e2) noexcept               \
{ return e1 = e1 ^ e2; }                                           \
                                                                   \
inline constexpr E operator|(E e1, E e2) noexcept                  \
{ return static_cast<E>(+e1 | +e2); }                              \
                                                                   \
inline constexpr E& operator|=(E& e1, E e2) noexcept               \
{ return e1 = e1 | e2; }                                           \

#endif /* COOL_ENUM_H_ */

