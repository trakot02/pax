#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    pax_trace();

    Arr<byte, 56> temp = {};

    auto buff = buff_from(temp.addr, temp.size);

    buff_copy_str8(&buff, "Prova: ");
    buff_copy_addr(&buff, &buff);

    str8 rev = str8_from(&buff);

    pax_message(rev);
}
