#ifndef PAX_CORE_FILE_HPP
#define PAX_CORE_FILE_HPP

#include <pax_core/types.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

#if defined(PAX_SYSTEM_WINDOWS)

    #include <pax_core/file_windows.hpp>

#elif defined(PAX_SYSTEM_POSIX)

    // #include <pax_core/file_posix.hpp>

#endif

namespace pax
{
    Write
    stdout_write();

    Write
    stderr_write();

    Read
    stdin_read();

    Write_Res
    stdout_write_byte(byte value);

    Write_Res
    stdout_write_s8(s8 value);

    Write_Res
    stdout_write_buff(Buff* value);

    Write_Res
    stderr_write_byte(byte value);

    Write_Res
    stderr_write_s8(s8 value);

    Write_Res
    stderr_write_buff(Buff* value);

    Read_Res
    stdin_read_byte(byte* value);

    Read_Res
    stdin_read_buff(Buff* value);
} // namespace pax

#endif PAX_CORE_FILE_HPP
