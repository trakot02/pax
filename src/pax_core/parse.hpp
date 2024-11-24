#ifndef PAX_CORE_PARSE_HPP
#define PAX_CORE_PARSE_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    enum Parse_Error : isize {
        PARSE_ERROR_NONE   = 0,
        PARSE_ERROR_SYNTAX = 1,
        PARSE_ERROR_RANGE  = 2,

        PARSE_ERROR_COUNT = 3,
    };

    static const Array<Str8, PARSE_ERROR_COUNT> PARSE_ERROR_TITLE = {
        "PARSE_ERROR_NONE",
        "PARSE_ERROR_SYNTAX",
        "PARSE_ERROR_RANGE",
    };

    Parse_Error
    parse_i64(Str8 token, i64* value);
} // namespace pax

#endif // PAX_CORE_PARSE_HPP
