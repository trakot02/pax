#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    REPORT_LEVEL = report_level_trace;

    auto text  = s8 {"Prova"};
    auto array = Array<byte, 2> {};

    for ( isize i = 0; i < text.size; i += 1 )
        array[i] = text[i];

    REPORT_LEVEL = REPORT_LEVEL_BASE;
}
