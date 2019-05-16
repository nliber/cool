#ifndef COOL_ENUM_H_
#define COOL_ENUM_H_

#include <cool/Out.h>
#include <ostream>
#include <type_traits>

namespace cool
{
    template<typename E>
    struct enum_bit_ops : std::false_type {};

    template<typename E>
    inline constexpr bool enum_bit_ops_v = enum_bit_ops<E>::value;


    template<typename E>
    struct enum_ops : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_ops_v = enum_ops<E>::value;


    template<typename E>
    struct enum_unary_plus : enum_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_unary_plus_v = enum_unary_plus<E>::value;


    template<typename E>
    struct enum_not : enum_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_not_v = enum_not<E>::value;


    template<typename E>
    struct enum_stream_inserter : enum_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_stream_inserter_v = enum_stream_inserter<E>::value;


    template<typename E>
    struct enum_compl : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_compl_v = enum_compl<E>::value;


    template<typename E>
    struct enum_bitand : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_bitand_v = enum_bitand<E>::value;


    template<typename E>
    struct enum_and_eq : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_and_eq_v = enum_and_eq<E>::value;


    template<typename E>
    struct enum_bitor : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_bitor_v = enum_bitor<E>::value;


    template<typename E>
    struct enum_or_eq : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_or_eq_v = enum_or_eq<E>::value;


    template<typename E>
    struct enum_xor : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_xor_v = enum_xor<E>::value;


    template<typename E>
    struct enum_xor_eq : enum_bit_ops<E>::type {};

    template<typename E>
    inline constexpr bool enum_xor_eq_v = enum_xor_eq<E>::value;

} // cool namespace

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_unary_plus_v<E>>>
constexpr std::underlying_type_t<E> operator+(E e) noexcept
{ return static_cast<std::underlying_type_t<E>>(e); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_not_v<E>>>
constexpr bool operator!(E e) noexcept
{ return !static_cast<std::underlying_type_t<E>>(e); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_stream_inserter_v<E>>>
std::ostream& operator<<(std::ostream& os, E e)
{ return os << cool::Out<E, true>(e); }


template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_compl_v<E>>>
constexpr E operator~(E e) noexcept
{ return static_cast<E>(~static_cast<std::underlying_type_t<E>>(e)); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_bitand_v<E>>>
constexpr E operator&(E e1, E e2) noexcept
{ return static_cast<E>(static_cast<std::underlying_type_t<E>>(e1) & static_cast<std::underlying_type_t<E>>(e2)); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_and_eq_v<E>>>
constexpr E& operator&=(E& e1, E e2) noexcept
{ return e1 = static_cast<E>(static_cast<std::underlying_type_t<E>>(e1) & static_cast<std::underlying_type_t<E>>(e2)); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_bitor_v<E>>>
constexpr E operator|(E e1, E e2) noexcept
{ return static_cast<E>(static_cast<std::underlying_type_t<E>>(e1) | static_cast<std::underlying_type_t<E>>(e2)); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_or_eq_v<E>>>
constexpr E& operator|=(E& e1, E e2) noexcept
{ return e1 = static_cast<E>(static_cast<std::underlying_type_t<E>>(e1) | static_cast<std::underlying_type_t<E>>(e2)); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_xor_v<E>>>
constexpr E operator^(E e1, E e2) noexcept
{ return static_cast<E>(static_cast<std::underlying_type_t<E>>(e1) ^ static_cast<std::underlying_type_t<E>>(e2)); }

template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && cool::enum_xor_eq_v<E>>>
constexpr E& operator^=(E& e1, E e2) noexcept
{ return e1 = static_cast<E>(static_cast<std::underlying_type_t<E>>(e1) ^ static_cast<std::underlying_type_t<E>>(e2)); }


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

