#include <pax_core/mem/alloc.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    Alloc
    alloc_empty()
    {
        Alloc alloc;

        alloc.context = 0;

        alloc.func_request = 0;
        alloc.func_release = 0;
        alloc.func_clear   = 0;

        return alloc;
    }

    Alloc_Value
    alloc_value_empty()
    {
        Alloc_Value value;

        value.width = 0;
        value.align = 0;
        value.count = 0;
        value.block = 0;

        return value;
    }

    Alloc_Value
    alloc_value_from(isize width, isize align, isize count, byte* block)
    {
        auto value = alloc_value_empty();

        value.width = width;
        value.align = align;
        value.count = count;
        value.block = block;

        return value;
    }

    Alloc_Error
    alloc_request(Alloc self, Alloc_Value* value)
    {
        pax_guard(self.func_request != 0,
            "`alloc.func_request` is null");

        return (*self.func_request)(self.context, value);
    }

    void
    alloc_release(Alloc self, Alloc_Value value)
    {
        pax_guard(self.func_release != 0,
            "`self.func_release` is null");

        return (*self.func_release)(self.context, value);
    }

    void
    alloc_clear(Alloc self)
    {
        pax_guard(self.func_clear != 0,
            "`self.func_clear` is null");

        return (*self.func_clear)(self.context);
    }
} // namespace pax
