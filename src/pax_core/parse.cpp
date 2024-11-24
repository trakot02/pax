#include <pax_core/parse.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    static const Array<byte, 10> DIGITS_DEC = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    static const Array<i8, MAX_BYTE> VALUES_DEC = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };

    //
    //
    // Exposed.
    //
    //

    Parse_Error
    parse_i64(Str8 token, i64* value)
    {
        auto& values = VALUES_DEC;
        auto& digits = DIGITS_DEC;

        const byte* head = token.block;
        const byte* tail = token.block + token.count;
        const byte* pntr = head;

        if ( pntr >= tail ) return PARSE_ERROR_SYNTAX;

        i64 mul = (*pntr != 0x2d) - (*pntr == 0x2d);
        u64 max = (*pntr == 0x2d) + MAX_I64;
        u64 acc = 0;

        if ( *pntr == 0x2d || *pntr == 0x2b )
            pntr += 1;

        for ( ; pntr < tail; pntr += 1 ) {
            isize digit = values[*pntr];

            if ( digit < 0 ) return PARSE_ERROR_SYNTAX;

            if ( acc <= (max - digit) / digits.count )
                acc = acc * digits.count + digit;
            else
                return PARSE_ERROR_RANGE;
        }

        if ( value != 0 ) *value = acc * mul;

        return PARSE_ERROR_NONE;
    }
} // namespace pax
