#include <pax_core/export.hpp>

#include <stdio.h>

using namespace pax;

static Arena arena = arena_init();

int
main(int argc, char* argv[])
{
    Buff  buff;
    File  file;
    isize error = 0;

    auto alloc = buff_init(&buff, &arena, 8192);

    if ( alloc != ARENA_ERROR_NONE ) {
        pax_fatal("Unable to allocate memory");
        pax_fatal(ARENA_ERROR_TITLE[alloc]);

        return 1;
    }

    if ( argc < 2 ) {
        pax_fatal("Unable to read file name");

        return 1;
    }

    auto open = file_open(&file, argv[1]);

    if ( open != FILE_ERROR_NONE ) {
        pax_fatal("Unable to open file for reading");
        pax_fatal(argv[1]);

        return 1;
    }

    isize col   = 0;
    isize row   = 0;
    Str8  token = "";

    while ( true ) {
        auto read = file_read_buff(&file, &buff);
        
        error = read.error;

        if ( read.count == 0 ) break;

        while ( true ) {
            auto match = csv_match_column(&buff, &token);

            isize c = col;
            isize r = row;

            if ( match.error != MATCH_ERROR_NONE ) break;

            col += 1;

            if ( match.line != 0 ) col = 0, row += 1;

            token = str8_trim(token);

            printf("(%4i, %4i) [%3i] => '%.*s'\n", (int) c, (int) r,
                (int) match.count, (int) token.count, token.block);
        }

        buff_shift(&buff);
    }

    if ( error != READ_ERROR_NONE ) {
        pax_fatal("Unable to read from file");

        return 1;
    }

    if ( buff_size(&buff) != 0 ) {
        pax_fatal("Malformed file");

        return 1;
    }
}
