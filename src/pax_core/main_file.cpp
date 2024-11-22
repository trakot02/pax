#include <pax_core/export.hpp>

#include <direct.h>
#include <stdio.h>

#define getcwd _getcwd

using namespace pax;

static Arena arena = arena_init(100);

enum Match_Error : isize {
    MATCH_ERROR_NONE = 0,
    MATCH_ERROR_SOME = 1,

    MATCH_ERROR_COUNT = 2,
};

struct CSV_Match {
    isize       line;
    isize       count;
    Match_Error error;
};

CSV_Match
match_column(Buff* buffer, Str8* token)
{
    pax_guard(buffer != 0, "`buffer` is null");

    isize state = 0;
    isize count = 0;
    isize line  = 0;

    byte* pntr = buffer->head;

    while ( true ) {
        if ( pntr >= buffer->tail )
            return {0, 0, MATCH_ERROR_SOME};

        if ( state == 1 && *pntr == 0x22 )
            state = 0, pntr += 1;

        if ( pntr >= buffer->tail )
            return {0, 0, MATCH_ERROR_SOME};

        if ( state == 0 ) {
            line = (*pntr == 0x0a);

            if ( *pntr == 0x0a || *pntr == 0x2c )
                break;

            if ( *pntr == 0x22 ) state = 1;
        }

        pntr += 1;
    }

    pntr  += 1;
    count  = pntr - buffer->head;

    if ( token != 0 ) {
        token->block = buffer->head;
        token->count = count - 1;
    }

    buffer->head = pntr;

    return {line, count, MATCH_ERROR_NONE};
}

int
main(int argc, char* argv[])
{
    Buff  buff;
    File  file;
    isize error = 0;

    auto alloc = buff_init(&buff, &arena, arena.count);

    if ( alloc != ARENA_ERROR_NONE ) {
        pax_fatal("Unable to allocate memory");

        return 1;
    }

    byte* pntr = getcwd(buff.tail, buff.count);

    if ( pntr != 0 ) {
        while ( *buff.tail != 0 )
            buff.tail += 1;
    } else {
        pax_fatal("Unable to read current working directory");

        return 1;
    }

    auto write = buff_write_str8(&buff, "/MOCK_DATA.csv");

    if ( write.error != WRITE_ERROR_NONE ) {
        pax_fatal("Unable to read file name");

        return 1;
    }

    auto open = file_open(&file, str8_from(&buff));

    if ( open != FILE_ERROR_NONE ) {
        pax_fatal("Unable to open file for reading");
        pax_fatal(str8_from(&buff));

        return 1;
    }

    buff_clear(&buff);

    isize col   = 0;
    isize row   = 0;
    Str8  token = "";

    while ( true ) {
        auto read = file_read_buff(&file, &buff);

        if ( read.count == 0 ) {
            error = read.error;
            break;
        }

        while ( true ) {
            auto match = match_column(&buff, &token);

            isize c = col;
            isize r = row;

            if ( match.error != MATCH_ERROR_NONE )
                break;

            col += 1;

            if ( match.line != 0 )
                col = 0, row += 1;

            token = str8_trim(token);

            printf("<%i, %i> -> '%.*s' (%i)\n", (int) c, (int) r,
                (int) token.count, token.block, (int) match.count);
        }

        buff_shift(&buff);
    }

    if ( error != READ_ERROR_NONE )
        pax_fatal("Unable to read from file");

    if ( buff_size(&buff) != 0 )
        pax_fatal("Wrong file");
}
