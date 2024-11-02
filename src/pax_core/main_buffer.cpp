#include <stdio.h>
#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    pax_trace();

    Array<byte, 128> temp = {};

    auto buff = buff_from(temp.addr, temp.size);

    buff_write_s8(&buff, "0xffffffff");

    pax_success(s8_from(&buff));

    buff_move(&buff, _buff_end_head);

    u64  val = 0;
    auto res = buff_read_u64(&buff, &val, _read_radix_hex);

    if ( res.error != _read_err_none )
        pax_error(READ_ERR_TITLE[res.error]);

    printf("read = %d, value = %llu\n", res.bytes, val);
}
