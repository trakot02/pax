#ifndef PAX_CORE_ARRAY_HPP
#define PAX_CORE_ARRAY_HPP

#include <pax_core/types.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    template <class Type, isize CNT>
    struct Array {
        //
        //
        // Variables.
        //
        //

        static const isize cnt = CNT;

        Type ptr[CNT];

        //
        //
        // Operations.
        //
        //

        const Type&
        operator[](isize idx) const;

        Type&
        operator[](isize idx);
    };

    //
    //
    // Exposed.
    //
    //

    template <class Type, isize CNT>
    const Type&
    Array<Type, CNT>::operator[](isize idx) const
    {
        pax_guard(0 <= idx && idx < cnt,
            "`idx` is out of bounds");

        return ptr[idx];
    }

    template <class Type, isize CNT>
    Type&
    Array<Type, CNT>::operator[](isize idx)
    {
        pax_guard(0 <= idx && idx < cnt,
            "`idx` is out of bounds");

        return ptr[idx];
    }
} // namespace pax

#endif // PAX_CORE_ARRAY_HPP
