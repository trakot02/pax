#ifndef PAX_CORE_FILE_HPP
#define PAX_CORE_FILE_HPP

#include <pax_core/types.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

#if defined(PAX_SYSTEM_WINDOWS)

    #include <pax_core/file_windows.hpp>

#elif defined(PAX_SYSTEM_POSIX)

    #include <pax_core/file_posix.hpp>

#endif

namespace pax
{
    Write
    stdout_write();

    Write_Value
    stdout_write_str8(Str8 value);

    Write_Value
    stdout_write_buff(Buff* value);

    Write
    stderr_write();

    Write_Value
    stderr_write_str8(Str8 value);

    Write_Value
    stderr_write_buff(Buff* value);

    Read
    stdin_read();

    Read_Value
    stdin_read_buff(Buff* value);
} // namespace pax

#endif // PAX_CORE_FILE_HPP
