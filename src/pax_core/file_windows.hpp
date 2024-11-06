#ifndef PAX_CORE_FILE_WINDOWS_HPP
#define PAX_CORE_FILE_WINDOWS_HPP

#include <pax_core/types.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    struct Buff;

    enum File_Err : isize {
        FILE_ERR_NONE = 0,

        FILE_ERR_COUNT = 1,
    };

    struct File {
        s8    name   = "";
        void* handle = 0;
    };

    extern File STDOUT;
    extern File STDERR;
    extern File STDIN;

    //
    // Opens a file in read mode. If the file does not exist
    // it returns an error.
    //
    File_Err
    file_open(File* file, s8 name);

    //
    // Opens a file in write mode. If the file does not exist
    // it gets created; if it does already exist it gets
    // truncated.
    //
    File_Err
    file_create(File* file, s8 name);

    void
    file_close(File* file);

    Write
    file_write(File* file);

    Read
    file_read(File* file);

    Write_Res
    file_write_byte(File* file, byte value);

    Write_Res
    file_write_s8(File* file, s8 value);

    Write_Res
    file_write_buff(File* file, Buff* value);

    Read_Res
    file_read_byte(File* file, byte* value);

    Read_Res
    file_read_buff(File* file, Buff* value);
} // namespace pax

#endif // PAX_CORE_FILE_WINDOWS_HPP
