#include <pax_core/export.hpp>

#include <stdio.h>

using namespace pax;

static Arena arena = arena_from(base_alloc());

void
csv_read_header(void* self, Str8 token, isize col)
{
    printf("{%3i} => %.*s\n", (int) col,
        (int) token.count, token.block);
}

void
csv_read_column(void* self, Str8 token, isize col, isize row)
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
    Buff buff = buff_empty();
    File file = file_empty();

    if ( argc < 2 ) {
        pax_error("Unable to read file name");

        return 1;
    }

    auto alloc = buff_init(&buff, arena_alloc(&arena), 8192);

    if ( alloc != ALLOC_ERROR_NONE ) {
        pax_error(ALLOC_ERROR_TITLE[alloc]);

        return 1;
    }

    printf("buffer size = %li\n", buff.count);

    auto open = file_open(&file, argv[1]);

    if ( open != FILE_ERROR_NONE ) {
        pax_error("Unable to open file");

        stderr_write_str8(argv[1]);
        stderr_write_str8("\n");

        return 1;
    }

    CSV_Parse parse;

    parse.func_header = &csv_read_header;
    parse.func_column = &csv_read_column;

    auto error = csv_parse(parse, &buff, &file, CSV_HEADER_TRUE);

    if ( error != READ_ERROR_NONE ) {
        pax_error("Unable to read from file");

        return 1;
    }

    if ( buff_count(&buff) != 0 ) {
        pax_error("Malformed file");

        return 1;
    }
}
