#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    Arena arena = arena_init(2000);

    File file;

    auto err = file_create(&file, "temp.txt");

    if ( err != FILE_ERR_NONE )
        pax_panic("Unable to open file for writing");

    auto buff = buff_init(&arena, arena.size);

    auto write = buff_write_s8(&buff, "Prova: ");

    if ( write.error != WRITE_ERR_NONE )
        pax_panic("Unable to write to buffer");

    write = buff_write_i64(&buff, -11, BUFF_BASE_BIN);

    if ( write.error != WRITE_ERR_NONE )
        pax_panic("Unable to write to buffer");

    pax_success(s8_from_buff(&buff));

    write = file_write_buff(&file, &buff);

    if ( write.error != WRITE_ERR_NONE )
        pax_panic("Unable to write to file");

    buff_clear(&buff);

    file_close(&file);

    err = file_open(&file, "temp.txt");

    if ( err != FILE_ERR_NONE )
        pax_panic("Unable to open file for reading");

    auto read = file_read_buff(&file, &buff);

    if ( read.error != READ_ERR_NONE )
        pax_panic("Unable to read from file");

    pax_success(s8_from_buff(&buff));
}
