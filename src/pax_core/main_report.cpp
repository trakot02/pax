#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    REPORT_LEVEL = report_level_trace;

    str8 prova = "Prova";

    Arr<byte, 2> array = {};

    for ( isize i = 0; i < prova.size; i += 1 )
        array[i] = prova[i];

    REPORT_LEVEL = report_level_message;
}
