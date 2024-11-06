#include <stdio.h>
#include <pax_core/export.hpp>

using namespace pax;

void
test_buff_read_i64()
{
    static const Array<s8, 12> STRINGS = {
         "0x",  "0x0",  "0x0x",
        "+0x", "+0x0", "+0x0x",
        "-0x", "-0x0", "-0x0x",
        "0x1", "+0x1", "-0x1",
    };

    static const Array<Read_Res, 12> RESULTS = {
        Read_Res {2, READ_ERR_SYNTAX},
        Read_Res {3, READ_ERR_NONE},
        Read_Res {4, READ_ERR_SYNTAX},
        Read_Res {3, READ_ERR_SYNTAX},
        Read_Res {4, READ_ERR_NONE},
        Read_Res {5, READ_ERR_SYNTAX},
        Read_Res {3, READ_ERR_SYNTAX},
        Read_Res {4, READ_ERR_NONE},
        Read_Res {5, READ_ERR_SYNTAX},
        Read_Res {3, READ_ERR_NONE},
        Read_Res {4, READ_ERR_NONE},
        Read_Res {4, READ_ERR_NONE},
    };

    static const Array<i64, 12> VALUES = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, -1,
    };

    Array<byte, 32> array = {};

    for ( isize i = 0; i < STRINGS.size; i += 1 ) {
        auto buff = buff_from_addr(array.addr, array.size);

        buff_fill_byte(&buff, 0);

        buff = buff_from_addr(array.addr, STRINGS[i].size);

        buff_write_s8(&buff, STRINGS[i]);
        buff_move(&buff, BUFF_SIDE_HEAD);

        i64  value  = 0;
        auto result = buff_read_i64(&buff, &value, BUFF_BASE_HEX);

        if ( result.bytes != RESULTS[i].bytes ||
             result.error != RESULTS[i].error ||
             value != VALUES[i] ) {
            auto expect = RESULTS[i];

            pax_error("Wrong result");

            printf(
                "%li => [%s]\n"
                "    result = (bytes %li, error %s), value = %li\n"
                "    expect = (bytes %li, error %s), value = %li\n",
                i, buff.head, result.bytes, READ_ERR_TITLE[result.error].addr, value,
                expect.bytes, READ_ERR_TITLE[expect.error].addr, VALUES[i]
            );
        }
    }

}

void
test_buff_write_i64()
{
    Array<byte, 4> temp = {};

    auto buff = buff_from_addr(temp.addr, temp.size);

    buff_write_i64(&buff, -1, BUFF_BASE_BIN);

    pax_success(s8_from_buff(&buff));
}

void
test_buff_read()
{
    s8 text = "Ciao sono mario ho 42 anni";

    Array<byte, 256> temp = {};

    for ( isize i = 0; i < text.size; i += 1 )
        temp[i] = text[i];

    auto src = buff_from_addr(temp.addr +  0, 26);
    auto dst = buff_from_addr(temp.addr + 26,  8);

    while ( true ) {
        auto read = buff_read_buff(&src, &dst);

        if ( read.bytes == 0 ) {
            if ( read.error != READ_ERR_NONE )
                pax_panic("Unable to read");

            break;
        }

        pax_success(s8_from_buff(&dst));

        buff_move(&dst, BUFF_SIDE_HEAD);
    }
}

void
test_buff_read_addr()
{
    Array<byte, 256> temp = {};

    auto buff = buff_from_addr(temp.addr, temp.size);

    buff_write_addr(&buff, &buff);

    pax_message(s8_from_buff(&buff));

    void* addr = 0;

    buff_move(&buff, BUFF_SIDE_HEAD);
    buff_read_addr(&buff, &addr);

    if ( addr == &buff )
        pax_success(s8_from_buff(&buff));
    else
        pax_error(s8_from_buff(&buff));
}

int
main(int argc, char* argv[])
{
    test_buff_read_i64();
    test_buff_write_i64();
    test_buff_read();
    test_buff_read_addr();
}
