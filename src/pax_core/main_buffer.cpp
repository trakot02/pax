#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    File file = file_empty();

    auto error = file_create(&file, "src/prova.txt");

    if ( error != FILE_ERROR_NONE ) {
        pax_error("Unable to open file");

        return 1;
    }

    auto write = file_write_str8(&file, "Prova\n");

    if ( write.error != WRITE_ERROR_NONE ) {
        pax_error("Unable to write to file");

        return 1;
    }
}
