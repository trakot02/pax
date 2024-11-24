#ifndef PAX_CORE_ALLOC_HPP
#define PAX_CORE_ALLOC_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    enum Alloc_Error : isize {
        ALLOC_ERROR_NONE          = 0,
        ALLOC_ERROR_OUT_OF_MEMORY = 1,

        ALLOC_ERROR_COUNT = 2,
    };

    static const Array<Str8, ALLOC_ERROR_COUNT> ALLOC_ERROR_TITLE = {
        "ALLOC_ERROR_NONE",
        "ALLOC_ERROR_OUT_OF_MEMORY",
    };

    struct Alloc_Value {
        isize width;
        isize align;
        isize count;
        byte* block;
    };

    struct Alloc {
        void* context;

        Alloc_Error (*func_request) (
            void* ctxt, Alloc_Value* value
        );

        void (*func_release) (
            void* ctxt, Alloc_Value value
        );

        void (*func_clear) (void* ctxt);
    };

    Alloc
    alloc_empty();

    Alloc_Value
    alloc_value_empty();

    Alloc_Value
    alloc_value_from(isize width, isize align, isize count, byte* block = 0);

    Alloc_Error
    alloc_request(Alloc self, Alloc_Value* value);

    void
    alloc_release(Alloc self, Alloc_Value value);

    void
    alloc_clear(Alloc self);
} // namespace pax

#endif // PAX_CORE_ALLOC_HPP
