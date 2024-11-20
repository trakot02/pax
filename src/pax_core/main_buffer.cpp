#include <pax_core/export.hpp>

using namespace pax;

static Arena arena = arena_init(1000);

int
main(int argc, char* argv[])
{
    Str8 text =
        "[User]\n"
        "name = \"Roberto\"\n"
        "lang = \"IT\"\n";

    auto buff = buff_init(&arena, arena.size);
}
