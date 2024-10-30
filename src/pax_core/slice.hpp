#ifndef PAX_CORE_SLICE_HPP
#define PAX_CORE_SLICE_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/arena.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    template <class Type>
    struct Slice {
        //
        //
        // Variables.
        //
        //

        Type* addr;
        isize size;
        isize limit;

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

    template <class Type, isize Size>
    Slice<Type>
    slice_from(Array<Type, Size>&& value);

    template <class Type, isize Size>
    Slice<const Type>
    slice_from(const Array<Type, Size>& value);

    template <class Type, isize Size>
    Slice<Type>
    slice_from(Array<Type, Size>& value);

    template <class Type>
    Slice<Type>
    slice_init();

    template <class Type>
    void
    slice_empty(Slice<Type>* slice);

    template <class Type>
    void
    slice_fill(Slice<Type>* slice, const Type& value);

    template <class Type>
    Slice<Type>
    slice_resize(Slice<Type>* slice, Arena* arena, isize delta);

    template <class Type>
    Slice<Type>
    slice_insert_value(Slice<Type>* slice, Arena* arena, isize index, const Type& value, isize count = 1);

    template <class Type, isize Size>
    Slice<Type>
    slice_insert_array(Slice<Type>* slice, Arena* arena, isize index, const Array<Type, Size>& value);

    template <class Type>
    Slice<Type>
    slice_insert(Slice<Type>* slice, Arena* arena, isize index, const Slice<Type>* value);
/*
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
    // Implementation.
    //
    //

    template <class Type>
    const Type&
    Slice<Type>::operator[](isize index) const
    {
        pax_trace();
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }

    template <class Type>
    Type&
    Slice<Type>::operator[](isize index)
    {
        pax_trace();
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }

    template <class Type, isize Size>
    Slice<Type>
    slice_from(Array<Type, Size>&& value)
    {
        pax_trace();

        Slice<Type> self = {0};

        self.addr  = value.addr;
        self.size  = value.size;
        self.limit = value.size;

        return self;
    }

    template <class Type, isize Size>
    Slice<const Type>
    slice_from(const Array<Type, Size>& value)
    {
        pax_trace();

        Slice<const Type> self = {0};

        self.addr  = value.addr;
        self.size  = value.size;
        self.limit = value.size;

        return self;
    }

    template <class Type, isize Size>
    Slice<Type>
    slice_from(Array<Type, Size>& value)
    {
        pax_trace();

        Slice<Type> self = {0};

        self.addr  = value.addr;
        self.size  = value.size;
        self.limit = value.size;

        return self;
    }

    template <class Type>
    Slice<Type>
    slice_init()
    {
        pax_trace();

        Slice<Type> self = {0};

        return self;
    }

    template <class Type>
    void
    slice_empty(Slice<Type>* slice)
    {
        pax_trace();
        pax_guard(slice != 0, "`slice` is null");

        auto& self = *slice;

        self.size = 0;
    }

    template <class Type>
    void
    slice_fill(Slice<Type>* slice, const Type& value)
    {
        pax_trace();
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
    slice_resize(Slice<Type>* slice, Arena* arena, isize delta)
    {
        static const isize WIDTH_TYPE = pax_type_width(Type);
        static const isize ALIGN_TYPE = pax_type_align(Type);

        pax_trace();
        pax_guard(slice != 0, "`slice` is null");

        auto& self = *slice;
        auto  copy = self;
        byte* addr = (byte*) self.addr;

        if ( delta <= 0 ) {
            isize avail = self.size - self.limit;

            if ( avail > delta )
                self.size = self.limit + delta;

            return self;
        }

        auto resl = arena_request(arena,
            WIDTH_TYPE, ALIGN_TYPE, self.size + delta);

        if ( resl.error != arena_err_none )
            pax_warning(ARENA_ERR_TITLE[resl.error]);

        isize size   = resl.width * resl.count;
        auto  buffer = buff_from(resl.addr, size);
        auto  source = buff_from(addr, size, buff_full_true);
        auto  buff   = buff_copy(&buffer, &source);

        pax_guard(buff.error == buff_err_none,
            "The operation must succeed");

        copy.addr  = (Type*) resl.addr;
        copy.size  = self.size;
        copy.limit = self.size + delta;

        return copy;
    }

    template <class Type>
    Slice<Type>
    slice_insert_value(Slice<Type>* slice, Arena* arena, isize index, const Type& value, isize count)
    {
        pax_trace();
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

            copy = slice_resize(
                slice, arena, size - self.size);

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

        pax_trace();
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

            copy = slice_resize(
                slice, arena, size - self.size);

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
        pax_trace();
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
} // namespace pax

#endif // PAX_CORE_SLICE_HPP
