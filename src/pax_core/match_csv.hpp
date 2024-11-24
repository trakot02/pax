#ifndef PAX_CORE_MATCH_CSV_HPP
#define PAX_CORE_MATCH_CSV_HPP

#include <pax_core/types.hpp>
#include <pax_core/match.hpp>

namespace pax
{
    struct Buff;

    struct CSV_Match {
        isize       line;
        isize       count;
        Match_Error error;
    };

    CSV_Match
    csv_match_empty();

    CSV_Match
    csv_match_column(Buff* buffer, Str8* token, byte delim = ',');
} // namespace pax

#endif // PAX_CORE_MATCH_CSV_HPP
