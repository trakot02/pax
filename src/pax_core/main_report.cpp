#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    REPORT_LEVEL = _report_level_trace;
    REPORT_GUARD = _report_guard_false;

    s8             text  = "Prova";
    Array<byte, 2> array = {};

    for ( isize i = 0; i < text.size; i += 1 )
        array[i] = text[i];

    REPORT_LEVEL = REPORT_LEVEL_BASE;
}
