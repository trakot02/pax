#include <pax_core/export.hpp>

#include <stdio.h>

using namespace pax;

int
main(int argc, char* argv[])
{
    auto slice = slice_from_mut<i32, 3>({0, 1, 2});

    isize i = 0;
    isize j = slice.count - 1;

    for ( ; i < j; i += 1, j -= 1 ) {
        i32 temp = slice[i];

        slice[i] = slice[j];
        slice[j] = temp;
    }

    for ( isize i = 0; i < slice.count; i += 1 )
        printf("%i\n", slice[i]);
}
