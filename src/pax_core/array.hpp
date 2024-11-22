#ifndef PAX_CORE_ARRAY_HPP
#define PAX_CORE_ARRAY_HPP

#include <pax_core/types.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    template <class Type, isize COUNT>
    struct Array {
        const Type&
        operator[](isize index) const;

        Type&
        operator[](isize index);

        //
        //
        // Variables.
        //
        //

        static const isize count = COUNT;

        Type block[COUNT];
    };

    //
    //
    // Exposed.
    //
    //

    template <class Type, isize COUNT>
    const Type&
    Array<Type, COUNT>::operator[](isize index) const
    {
        pax_guard(0 <= index && index < count,
            "`index` is out of bounds");

        return block[index];
    }

    template <class Type, isize COUNT>
    Type&
    Array<Type, COUNT>::operator[](isize index)
    {
        pax_guard(0 <= index && index < count,
            "`index` is out of bounds");

        return block[index];
    }
} // namespace pax

#endif // PAX_CORE_ARRAY_HPP
