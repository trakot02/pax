#include <pax_core/match_csv.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    enum CSV_State : isize {
        CSV_STATE_BASE   = 0,
        CSV_STATE_ESCAPE = 1,

        CSV_STATE_COUNT = 2,
    };

    //
    //
    // Exposed.
    //
    //

    CSV_Match
    csv_match_empty()
    {
        CSV_Match match;

        match.line  = 0;
        match.count = 0;
        match.error = MATCH_ERROR_NONE;

        return match;
    }

    CSV_Match
    csv_match_column(Buff* buffer, Str8* token, byte delim)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        isize state = CSV_STATE_BASE;
        isize count = 0;
        isize line  = 0;

        byte* pntr = buffer->head;

        for ( ; pntr < buffer->tail; pntr += 1 ) {
            if ( state == CSV_STATE_BASE ) {
                state = (*pntr == 0x22) * CSV_STATE_ESCAPE;
                line  = (*pntr == 0x0a);

                if ( *pntr == 0x0a || *pntr == delim )
                    break;

                continue;
            }

            if ( state == CSV_STATE_ESCAPE ) {
                state = (*pntr != 0x22) * CSV_STATE_ESCAPE;

                continue;
            }
        }

        if ( pntr >= buffer->tail )
            return {0, 0, MATCH_ERROR_SOME};

        count = pntr - buffer->head;

        if ( token != 0 ) {
            token->block = buffer->head;
            token->count = count;
        }

        buffer->head = pntr + 1;

        return {line, count + 1, MATCH_ERROR_NONE};
    }
} // namespace pax
