#ifndef PAX_CORE_FORMAT_HPP
#define PAX_CORE_FORMAT_HPP

#include <pax_core/types.hpp>

namespace pax
{
    struct Buff;

    enum Format_Base : isize {
        FORMAT_BASE_DEC = 0,
        FORMAT_BASE_HEX = 1,
        FORMAT_BASE_BIN = 2,
        FORMAT_BASE_OCT = 3,

        FORMAT_BASE_COUNT = 4,
    };

    enum Format_Error : isize {
        FORMAT_ERROR_NONE     = 0,
        FORMAT_ERROR_OVERFLOW = 1,

        FORMAT_ERROR_COUNT = 2,
    };

    struct Format_Value {
        isize        count;
        Format_Error error;
    };

    Format_Value
    format_byte(Buff* buff, byte value, isize times = 1);

    Format_Value
    format_s8(Buff* buff, s8 value, isize index = 0, isize count = -1, isize times = 1);

    Format_Value
    format_u64(Buff* buff, u64 value, Format_Base base = FORMAT_BASE_DEC);

    Format_Value
    format_u32(Buff* buff, u32 value, Format_Base base = FORMAT_BASE_DEC);

    Format_Value
    format_u16(Buff* buff, u16 value, Format_Base base = FORMAT_BASE_DEC);

    Format_Value
    format_u8(Buff* buff, u8 value, Format_Base base = FORMAT_BASE_DEC);

    Format_Value
    format_i64(Buff* buff, i64 value);

    Format_Value
    format_i32(Buff* buff, i32 value);

    Format_Value
    format_i16(Buff* buff, i16 value);

    Format_Value
    format_i8(Buff* buff, i8 value);

    Format_Value
    format_bool(Buff* buff, bool value);
} // namespace pax

#endif // PAX_CORE_FORMAT_HPP
