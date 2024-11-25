#include <pax_core/export.hpp>

#include <stdio.h>

using namespace pax;

void
csv_parse_header(void* self, Str8 token, isize col)
{
    printf("{%3i} => %.*s\n", (int) col,
        (int) token.count, token.block);
}

void
csv_parse_column(void* self, Str8 token, isize col, isize row)
{
    switch ( col ) {
        case 0:
        case 1: {
            i64  value = 0;
            auto error = parse_i64(token, &value);

            if ( error != PARSE_ERROR_NONE )
                pax_warning(PARSE_ERROR_TITLE[error]);

            break;
        }

        case 2:
        case 3: break;

        default: {
            pax_error("Too many columns");
            break;
        }
    }

    printf("{%3i, %3i} => '%.*s'\n", (int) col, (int) row,
        (int) token.count, token.block);
}

int
main(int argc, char* argv[])
{
    Arena arena = arena_from(base_alloc());
    Alloc alloc = arena_alloc(&arena);

    Buff buff = buff_empty();
    File file = file_empty();

    if ( argc < 2 ) {
        pax_error("Unable to read file name");

        return 1;
    }

    auto init = buff_init(&buff, alloc, 8192);

    if ( init != ALLOC_ERROR_NONE ) {
        pax_error(ALLOC_ERROR_TITLE[init]);

        return 1;
    }

    printf("buffer size = %i\n", (int) buff.count);

    auto open = file_open(&file, argv[1]);

    if ( open != FILE_ERROR_NONE ) {
        pax_error("Unable to open file");

        stderr_write_str8(argv[1]);
        stderr_write_str8("\n");

        return 1;
    }

    auto parse = csv_parse_empty();

    parse.func_header = &csv_parse_header;
    parse.func_column = &csv_parse_column;

    auto error = csv_read(parse, &file, &buff, CSV_HEADER_TRUE);

    if ( error != READ_ERROR_NONE ) {
        pax_error("Unable to read from file");

        return 1;
    }

    if ( buff_count(&buff) != 0 ) {
        pax_error("Malformed file");

        return 1;
    }
}
