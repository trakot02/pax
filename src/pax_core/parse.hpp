#ifndef PAX_CORE_PARSE_HPP
#define PAX_CORE_PARSE_HPP

#include <pax_core/types.hpp>

namespace pax
{
    enum Parse_Base : isize {
        PARSE_BASE_DEC = 0,
        PARSE_BASE_HEX = 1,
        PARSE_BASE_BIN = 2,
        PARSE_BASE_OCT = 3,

        PARSE_BASE_COUNT = 4,
    };

    enum Parse_Error : isize {
        PARSE_ERROR_NONE     = 0,
        PARSE_ERROR_SYNTAX   = 1,
        PARSE_ERROR_OVERFLOW = 2,
    };

    struct Parse_Value {
        isize       count;
        Parse_Error error;
    };

    Parse_Value
    parse_u64(s8 token, u64* value, Parse_Base base = PARSE_BASE_DEC);

    Parse_Value
    parse_u32(s8 token, u32* value, Parse_Base base = PARSE_BASE_DEC);

    Parse_Value
    parse_u16(s8 token, u16* value, Parse_Base base = PARSE_BASE_DEC);

    Parse_Value
    parse_u8(s8 token, u8* value, Parse_Base base = PARSE_BASE_DEC);

    Parse_Value
    parse_i64(s8 token, i64* value);

    Parse_Value
    parse_i32(s8 token, i32* value);

    Parse_Value
    parse_i16(s8 token, i16* value);

    Parse_Value
    parse_i8(s8 token, i8* value);

    Parse_Value
    parse_bool(s8 token, bool* value);
} // namespace pax

#endif // PAX_CORE_PARSE_HPP
