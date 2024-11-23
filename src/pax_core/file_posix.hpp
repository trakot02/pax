#ifndef PAX_CORE_FILE_POSIX_HPP
#define PAX_CORE_FILE_POSIX_HPP

#include <pax_core/types.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    struct Buff;

    enum File_Error : isize {
        FILE_ERROR_NONE   = 0,
        FILE_ERROR_SYSTEM = 1,

        FILE_ERROR_COUNT = 2,
    };

    struct File {
        isize handle = -1;
    };

    extern File STDOUT;
    extern File STDERR;
    extern File STDIN;

    //
    // Opens a file in read mode. If the file does not exist
    // it returns an error.
    //
    File_Error
    file_open(File* file, Str8 name);

    //
    // Opens a file in write mode. If the file does not exist
    // it gets truncated; if it does already exist it gets
    // truncated.
    //
    File_Error
    file_create(File* file, Str8 name);

    void
    file_close(File* file);

    Write
    file_write(File* file);

    Read
    file_read(File* file);

    Write_Value
    file_write_str8(File* file, Str8 value);

    Write_Value
    file_write_buff(File* file, Buff* value);

    Read_Value
    file_read_buff(File* file, Buff* value);
} // namespace pax

#endif // PAX_CORE_FILE_POSIX_HPP
