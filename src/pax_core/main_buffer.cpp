#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    pax_trace();

    auto temp = Array<byte, 56> {};
    auto buff = buff_from(temp.addr, temp.size);

    buff_copy_s8(&buff, "Prova: ");
    buff_copy_addr(&buff, &buff);

    pax_message(s8_from(&buff));
}
