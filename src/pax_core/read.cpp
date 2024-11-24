#include <pax_core/read.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    Read
    read_empty()
    {
        Read read;

        read.context = 0;

        read.func_buff  = 0;
        read.func_close = 0;

        return read;
    }

    Read_Value
    read_value_empty()
    {
        Read_Value value;

        value.count  = 0;
        value.error  = READ_ERROR_NONE;
        value.system = 0;

        return value;
    }

    Read_Value
    read_buff(Read self, Buff* value)
    {
        pax_guard(self.func_buff != 0,
            "`self.func_buff` is null");

        return (*self.func_buff)(self.context, value);
    }

    void
    read_close(Read self)
    {
        pax_guard(self.func_close != 0,
            "`self.func_close` is null");

        return (*self.func_close)(self.context);
    }
} // namespace pax
