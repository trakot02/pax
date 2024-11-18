#include <pax_core/file.hpp>

namespace pax
{
    Write
    stdout_write()
    {
        return file_write(&STDOUT);
    }

    Write_Value
    stdout_write_s8(s8 value)
    {
        return file_write_s8(&STDOUT, value);
    }

    Write_Value
    stdout_write_buff(Buff* value)
    {
        return file_write_buff(&STDOUT, value);
    }

    Write
    stderr_write()
    {
        return file_write(&STDERR);
    }

    Write_Value
    stderr_write_s8(s8 value)
    {
        return file_write_s8(&STDERR, value);
    }

    Write_Value
    stderr_write_buff(Buff* value)
    {
        return file_write_buff(&STDERR, value);
    }

    Read
    stdin_read()
    {
        return file_read(&STDIN);
    }

    Read_Value
    stdin_read_buff(Buff* value)
    {
        return file_read_buff(&STDIN, value);
    }
} // namespace pax
