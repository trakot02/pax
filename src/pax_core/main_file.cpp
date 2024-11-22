#include <pax_core/export.hpp>

#include <direct.h>
#include <stdio.h>

#define getcwd _getcwd

using namespace pax;

static Arena arena = arena_init(2000);

isize
match_column(Buff* buffer, Str8* token, isize* col, isize* row)
{
    if ( buffer == 0 ) return 0;

    isize escap = 0;
    isize extra = 0;

    byte* pntr = buffer->head;

    do {
        if ( pntr >= buffer->tail ) return -1;

        if ( escap == 0 ) {
            if ( *pntr == 0x0a || *pntr == 0x2c ) {
                if ( col != 0 && row != 0 ) {
                    *col += 1;

                    if ( *pntr == 0x0a )
                        *col = 0, *row += 1;
                }

                extra = 1;
                break;
            }
        }

        pntr += 1;
    } while ( true );

    isize count = pntr - buffer->head;

    if ( token != 0 ) {
        token->block = buffer->head;
        token->count = count;
    }

    buffer->head = pntr + extra;

    return count + extra;
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

    auto write = buff_write_str8(&buff, "/test.csv");

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

    do {
        auto read = file_read_buff(&file, &buff);

        if ( read.count == 0 ) {
            error = read.error;
            break;
        }

        isize count = 0;
        isize col   = 0;
        isize row   = 0;
        Str8  token = "";

        do {
            printf("<%i, %i> -> ", (int) col, (int) row);

            count = match_column(&buff, &token, &col, &row);

            if ( count > 0 ) {
                printf("'%.*s' (%i)\n",
                    (int) token.count, token.block, (int) count);
            }
        } while ( count > 0 );

        buff_shift(&buff);
    } while ( true );

    if ( error != READ_ERROR_NONE )
        pax_fatal("Unable to read from file");
}
