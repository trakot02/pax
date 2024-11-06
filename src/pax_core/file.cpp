#include <pax_core/file.hpp>

namespace pax
{
    Write
    stdout_write()
    {
        return file_write(&STDOUT);
    }

    Write
    stderr_write()
    {
        return file_write(&STDERR);
    }

    Read
    stdin_read()
    {
        return file_read(&STDIN);
    }

    Write_Res
    stdout_write_byte(byte value)
    {
        return file_write_byte(&STDOUT, value);
    }

    Write_Res
    stdout_write_s8(s8 value)
    {
        return file_write_s8(&STDOUT, value);
    }

    Write_Res
    stdout_write_buff(Buff* value)
    {
        return file_write_buff(&STDOUT, value);
    }

    Write_Res
    stderr_write_byte(byte value)
    {
        return file_write_byte(&STDERR, value);
    }

    Write_Res
    stderr_write_s8(s8 value)
    {
        return file_write_s8(&STDERR, value);
    }

    Write_Res
    stderr_write_buff(Buff* value)
    {
        return file_write_buff(&STDERR, value);
    }

    Read_Res
    stdin_read_byte(byte* value)
    {
        return file_read_byte(&STDIN, value);
    }

    Read_Res
    stdin_read_buff(Buff* value)
    {
        return file_read_buff(&STDIN, value);
    }
} // namespace pax
