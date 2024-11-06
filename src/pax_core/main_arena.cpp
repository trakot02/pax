#include <stdio.h>

#include <pax_core/export.hpp>

using namespace pax;

int
main(int argc, char* argv[])
{
    auto arena = arena_init(8192);
    auto slice = slice_from<i32, 3>({1, 2, 3});

    for ( isize i = 0; i < slice.size; i += 1 )
        printf("%ld (%ld/%ld). %d\n", i, slice.size, slice.limit, slice[i]);

    // slice = slice_insert_array<i32, 3>(&slice, &arena, 0, {250, 251, 252});

    for ( isize i = 0; i < slice.size; i += 1 )
        printf("%ld (%ld/%ld). %d\n", i, slice.size, slice.limit, slice[i]);
}
