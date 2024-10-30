#ifndef PAX_CORE_ARRAY_HPP
#define PAX_CORE_ARRAY_HPP

#include <pax_core/types.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    template <class Type, isize Size>
    struct Array {
        //
        //
        // Variables.
        //
        //

        static const isize size =
            Size;

        Type addr[size];

        //
        //
        // Operations.
        //
        //

        const Type&
        operator[](isize index) const;

        Type&
        operator[](isize index);
    };

    //
    //
    // Implementation.
    //
    //

    template <class Type, isize Size>
    const Type&
    Array<Type, Size>::operator[](isize index) const
    {
        pax_trace();
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }

    template <class Type, isize Size>
    Type&
    Array<Type, Size>::operator[](isize index)
    {
        pax_trace();
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }
} // namespace pax

#endif // PAX_CORE_ARRAY_HPP
