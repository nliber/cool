#ifndef COOL_MAYBE_UNUSED_H_
#define COOL_MAYBE_UNUSED_H_

///////////////////////////////////////////////////////////////////////////////
// maybe_unused
//
//  Silences unused warnings about variables passed to it
///////////////////////////////////////////////////////////////////////////////
namespace cool
{

    template<typename... Ts>
    constexpr void maybe_unused(Ts&&...) noexcept
    {}

} // cool namespace

#endif /* COOL_MAYBE_UNUSED_H_ */

