#include <stdio.h>
#include <pax_core/export.hpp>

using namespace pax;

void
test_buff_read_i64()
{
    pax_trace();

    static const Array<s8, 12> STRINGS = {
         "0x",  "0x0",  "0x0x",
        "+0x", "+0x0", "+0x0x",
        "-0x", "-0x0", "-0x0x",
        "0x1", "+0x1", "-0x1",
    };

    static const Array<Read_Res, 12> RESULTS = {
        Read_Res {2, _read_err_syntax},
        Read_Res {3, _read_err_none},
        Read_Res {4, _read_err_syntax},
        Read_Res {3, _read_err_syntax},
        Read_Res {4, _read_err_none},
        Read_Res {5, _read_err_syntax},
        Read_Res {3, _read_err_syntax},
        Read_Res {4, _read_err_none},
        Read_Res {5, _read_err_syntax},
        Read_Res {3, _read_err_none},
        Read_Res {4, _read_err_none},
        Read_Res {4, _read_err_none},
    };

    static const Array<i64, 12> VALUES = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, -1,
    };

    Array<byte, 32> array = {};

    for ( isize i = 0; i < STRINGS.size; i += 1 ) {
        auto buff = buff_from(array.addr, array.size);

        buff_fill_byte(&buff, 0);

        buff = buff_from(array.addr, STRINGS[i].size);

        buff_write_s8(&buff, STRINGS[i]);
        buff_move(&buff, _buff_end_head);

        i64  value  = 0;
        auto result = buff_read_i64(&buff, &value, _read_radix_hex);

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

int
main(int argc, char* argv[])
{
    pax_trace();

    test_buff_read_i64();
}
