#ifndef PAX_CORE_STR8_HPP
#define PAX_CORE_STR8_HPP

#include <pax_core/types.hpp>

namespace pax
{
    struct Buff;
    struct str8;

    str8
    str8_from(const Buff* buffer);

    struct str8 {
        //
        //
        // Variables.
        //
        //

        const byte* addr;
        isize       size;

        //
        //
        // Operations.
        //
        //

        template <isize Size>
        constexpr str8(const byte (&addr)[Size]);

        const byte&
        operator[](isize index) const;
    };

    //
    //
    // Implementation.
    //
    //

    template <isize Size>
    constexpr str8::str8(const byte (&addr)[Size])
        : addr {addr}
        , size {Size - 1}
    {}
} // namespace pax

#endif // PAX_CORE_STR8_HPP
