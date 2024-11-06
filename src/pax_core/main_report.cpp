#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    REPORT_LEVEL = REPORT_LEVEL_DEBUG;

    pax_fatal("");
    pax_error("");
    pax_warning("");
    pax_message("");
    pax_success("");
    pax_debug("");
    pax_panic("");
    // pax_guard(false, "");

    s8             text  = "Prova";
    Array<byte, 2> array = {};

    for ( isize i = 0; i < text.size; i += 1 )
        array[i] = text[i];

    REPORT_LEVEL = REPORT_LEVEL_BASE;
}
