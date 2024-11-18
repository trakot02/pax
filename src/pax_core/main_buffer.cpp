#include <pax_core/export.hpp>

using namespace pax;

Match_Value
match_u64(Buff* buff, s8* token)
{

}

int
main(int argc, char* argv[])
{
    auto arena = arena_init(200);
    auto buff  = buff_init(&arena, arena.size);

    buff_write_s8(&buff, "[User\n");

    s8 token = "";

    if ( match_exact(&buff, 0, "[").error ||
         match_alpha(&buff, 0).error ||
         match_exact(&buff, 0, "]").error )
        return 1;

    stdout_write_s8("Success\n");
}
