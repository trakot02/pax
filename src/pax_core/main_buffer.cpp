#include <stdio.h>
#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    pax_trace();

    auto temp = Array<byte, 256> {};
    auto buff = buff_from(temp.addr, temp.size);

    auto res = buff_write_i64(&buff, MIN_I64, write_radix_bin);

    printf("wrote = %d\n", res.bytes);

    pax_success(s8_from(&buff));
}
