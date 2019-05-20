#ifndef COOL_ENUM_H_
#define COOL_ENUM_H_

#include <cool/Out.h>
#include <ostream>
#include <type_traits>

///////////////////////////////////////////////////////////////////////////////
// Enum
//
//  This header adds common operations to enums, in one of two ways:
//      (1) traits
//      (2) macros
//
//  Enum traits
//      Specializing these traits to add the following operators to the
//      global namespace for a given enum E:
//
//      enum_unary_plus         +e          (returns underlying_type_t<E>)
//      enum_not                !e          (returns bool)
//      enum_stream_inserter    ostrm << e  (returns ostream&)
//
//      enum_compl              ~e          (returns E)
//      enum_bitand             e1 & e2     (returns E)
//      enum_and_eq             e1 &= e2    (returns E&)
//      enum_bitor              e1 | e2     (returns E)
//      enum_or_eq              e1 |= e2    (returns E&)
//      enum_xor                e1 ^ e2     (returns E)
//      enum_xor_eq             e1 ^= e2    (returns E&)
//
//      enum_ops                Implicit opt-in:
//                                  +e, !e, ostrm << e
//
//      enum_bit_ops            Implicit opt-in:
//                                  enum_ops, ~e,
//                                  e1 & e2, e1 &= e2,
//                                  e1 | e2, e1 |= e2,
//                                  e1 ^ e2, e1 ^= e2
//
//      All traits are implicitly opt-out.  To explicitly opt-in:
//          template<> struct cool::enum_some_trait<E> : std::true_type {};
//
//      If you have implicitly opted into a trait
//      (by explicitly opting in to either enum_ops or enum_bit_ops),
//      you can explicitly opt-out of the ones you don't want by:
//          template<> struct cool::enum_some_trait<E> : std::false_type {};
//
//
//  Enum macros
//      Call one of these macros to add the following operators to the
//      current namespace for a given enum E:
//
//      COOL_ENUM_OPS(E)        +e, !e, ostrm << e
//
//      COOL_ENUM_BIT_OPS(E)    +e, !e, ostrm << e, ~e,
//                              e1 & e2, e1 &= e2,
//                              e1 | e2, e1 |= e2,
//                              e1 ^ e2, e1 ^= e2
//
///////////////////////////////////////////////////////////////////////////////

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

