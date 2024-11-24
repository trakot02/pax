#ifndef PAX_CORE_SLICE_HPP
#define PAX_CORE_SLICE_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    template <class Type>
    struct Slice {
        const Type&
        operator[](isize index) const;

        Type&
        operator[](isize index);

        //
        //
        // Variables.
        //
        //

        Type* block;
        isize count;
        isize total;
    };

    template <class Type>
    Slice<Type>
    slice_empty();

    template <class Type, isize COUNT>
    Slice<Type>
    slice_from_arr(Array<Type, COUNT>&& array);

    template <class Type, isize COUNT>
    Slice<const Type>
    slice_from_arr(const Array<Type, COUNT>& array);

/*
    template <class Type>
    void
    slice_clear(Slice<Type>* slice);

    template <class Type>
    void
    slice_fill(Slice<Type>* slice, const Type& value);

    template <class Type>
    Slice<Type>
    slice_copy(Slice<Type>* slice, Arena* arena, isize delta);

    template <class Type>
    Slice<Type>
    slice_insert_val(Slice<Type>* slice, Arena* arena, isize index, const Type& value, isize count = 1);

    template <class Type, isize Size>
    Slice<Type>
    slice_insert_arr(Slice<Type>* slice, Arena* arena, isize index, const Array<Type, Size>& value);

    template <class Type>
    Slice<Type>
    slice_insert(Slice<Type>* slice, Arena* arena, isize index, const Slice<Type>* value);

    template <class Type>
    void
    slice_remove(Slice<Type>* slice, isize index, isize count);

    template <class Type>
    Slice<Type>
    slice_give_value(Slice<Type>* slice, Arena* arena, isize index, const Type& value, isize count = 1);

    template <class Type, isize Size>
    Slice<Type>
    slice_give_array(Slice<Type>* slice, Arena* arena, isize index, const Arr<Type, Size>& value);

    template <class Type>
    Slice<Type>
    slice_give(Slice<Type>* slice, Arena* arena, isize index, const Slice<Type>* value);

    template <class Type>
    void
    slice_take(Slice<Type>* slice, isize index, isize count);
*/

    //
    //
    // Exposed.
    //
    //

    template <class Type>
    const Type&
    Slice<Type>::operator[](isize index) const
    {
        pax_guard(0 <= index && index < count,
            "`index` is out of bounds");

        return block[index];
    }

    template <class Type>
    Type&
    Slice<Type>::operator[](isize index)
    {
        pax_guard(0 <= index && index < count,
            "`index` is out of bounds");

        return block[index];
    }

    template <class Type>
    Slice<Type>
    slice_empty()
    {
        Slice<Type> slice;

        slice.block = 0;
        slice.count = 0;
        slice.total = 0;

        return slice;
    }

    template <class Type, isize COUNT>
    Slice<Type>
    slice_from_arr(Array<Type, COUNT>&& array)
    {
        auto slice = slice_empty<Type>();

        slice.block = array.block;
        slice.count = array.count;
        slice.total = array.count;

        return slice;
    }

    template <class Type, isize COUNT>
    Slice<const Type>
    slice_from_arr(const Array<Type, COUNT>& array)
    {
        auto slice = slice_empty<const Type>();

        slice.block = array.block;
        slice.count = array.count;
        slice.total = array.count;

        return slice;
    }

/*
    template <class Type>
    void
    slice_clear(Slice<Type>* slice)
    {
        pax_guard(slice != 0, "`slice` is null");

        auto& self = *slice;

        self.size = 0;
    }

    template <class Type>
    void
    slice_fill(Slice<Type>* slice, const Type& value)
    {
        pax_guard(slice != 0, "`slice` is null");

        auto& self  = *slice;
        isize avail = self.limit - self.size;
        isize index = self.size;

        for ( isize i = 0; i < avail; i += 1 )
            self.addr[index + i] = value;

        self.size = self.limit;
    }

    template <class Type>
    Slice<Type>
    slice_copy(Slice<Type>* slice, Arena* arena, isize delta)
    {
        static const isize WIDTH_TYPE = pax_type_width(Type);
        static const isize ALIGN_TYPE = pax_type_align(Type);

        pax_guard(slice != 0, "`slice` is null");

        auto& self  = *slice;
        auto  other = self;
        isize avail = self.limit - self.size;
        isize count = self.size + delta;
        byte* addr  = (byte*) self.addr;

        if ( delta <= 0 )

        if ( delta <= 0 ) {
            if ( count < 0 ) count = 0;

            other.size = count;

            return other;
        }

        if ( self.size > self.limit - delta ) {
            auto value = arena_request(arena, {
                WIDTH_TYPE, ALIGN_TYPE, count,
            });

            if ( value.addr == 0 ) return self;

            isize size = count * WIDTH_TYPE;
            auto  buff = buff_init(value.addr, size);

            buff_fill_addr(&buff, addr);

            other.addr  = (Type*) value.addr;
            other.size  = self.size;
            other.limit = count;
        }

        return Slice<Type> {};
    }

    template <class Type>
    Slice<Type>
    slice_insert_val(Slice<Type>* slice, Arena* arena, isize index, const Type& value, isize count)
    {
        pax_guard(slice != 0, "`slice` is null");

        auto& self = *slice;
        auto  copy = self;
        isize size = 16;

        if ( index < 0 || index > self.size )
            return self;

        if ( count < 0 ) return self;

        if ( self.limit - count < self.size ) {
            while ( size < count) {
                isize next = size * 2;

                size = (size >= next) * MAX_ISIZE +
                       (size  < next) * next;
            }

            copy = slice_resize(slice, arena,
                size - self.size);

            if ( copy.limit < size ) return self;
        }

        for ( isize i = copy.size; i > index; i -= 1 )
            copy.addr[count + i - 1] = copy.addr[i - 1];

        for ( isize i = 0; i < count; i += 1 )
            copy.addr[index + i] = value;

        copy.size += count;

        return copy;
    }

    template <class Type, isize Size>
    Slice<Type>
    slice_insert_array(Slice<Type>* slice, Arena* arena, isize index, const Array<Type, Size>& value)
    {
        isize count = value.size;

        pax_guard(slice != 0, "`slice` is null");
        pax_guard(count >= 0, "`count` is negative");

        auto& self = *slice;
        auto  copy = self;
        isize size = 16;

        if ( index < 0 || index > self.size )
            return self;

        if ( self.limit - count < self.size ) {
            while ( size < count) {
                isize next = size * 2;

                size = (size >= next) * MAX_ISIZE +
                       (size  < next) * next;
            }

            copy = slice_resize(slice, arena,
                size - self.size);

            if ( copy.limit < size ) return self;
        }

        for ( isize i = copy.size; i > index; i -= 1 )
            copy.addr[count + i - 1] = copy.addr[i - 1];

        for ( isize i = 0; i < count; i += 1 )
            copy.addr[index + i] = value[i];

        copy.size += count;

        return copy;
    }

    template <class Type>
    Slice<Type>
    slice_insert(Slice<Type>* slice, Arena* arena, isize index, const Slice<Type>* value)
    {
        pax_guard(slice != 0, "`slice` is null");
        pax_guard(value != 0, "`value` is null");

        auto& other = *value;
        isize count = other.size;

        pax_guard(count >= 0, "`count` is negative");

        auto& self = *slice;
        auto  copy = self;
        isize size = 16;

        if ( index < 0 || index > self.size )
            return self;

        if ( self.limit - count < self.size ) {
            while ( size < count) {
                isize next = size * 2;

                size = (size >= next) * MAX_ISIZE +
                       (size  < next) * next;
            }

            copy = slice_resize(
                slice, arena, size - self.size);

            if ( copy.limit < size ) return self;
        }

        for ( isize i = copy.size; i > index; i -= 1 )
            copy.addr[count + i - 1] = copy.addr[i - 1];

        for ( isize i = 0; i < count; i += 1 )
            copy.addr[index + i] = other[i];

        copy.size += count;

        return copy;
    }
*/
} // namespace pax

#endif // PAX_CORE_SLICE_HPP
