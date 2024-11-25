#include <pax_core/read_csv.hpp>
#include <pax_core/match_csv.hpp>
#include <pax_core/buff.hpp>
#include <pax_core/file.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    Read_Error
    csv_read_head(CSV_Parse parse, File* file, Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        auto value = read_value_empty();
        auto match = csv_match_empty();
        auto token = str8_empty();

        isize col = 0;

        do {
            for ( ; match.line == 0; col += 1 ) {
                match = csv_match_column(buffer, &token);
                token = str8_trim(token);

                if ( match.error != MATCH_ERROR_NONE )
                    break;

                csv_parse_header(parse, token, col);
            }

            value = read_value_empty();

            buff_shift(buffer);

            if ( file != 0 && match.line == 0 )
                value = file_read_buff(file, buffer);
        } while ( value.count != 0 );

        return value.error;
    }

    Read_Error
    csv_read_body(CSV_Parse parse, File* file, Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        auto value = read_value_empty();
        auto match = csv_match_empty();
        auto token = str8_empty();

        isize col = 0;
        isize row = 0;

        do {
            for ( ; true; col += 1 ) {
                if ( match.line != 0 ) col = 0, row += 1;

                match = csv_match_column(buffer, &token);
                token = str8_trim(token);

                if ( match.error != MATCH_ERROR_NONE )
                    break;

                csv_parse_column(parse, token, col, row);
            }

            buff_shift(buffer);

            if ( file != 0 )
                value = file_read_buff(file, buffer);
        } while ( value.count != 0 );

        return value.error;
    }

    Read_Error
    csv_read(CSV_Parse parse, File* file, Buff* buffer, CSV_Header header)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        pax_guard(0 <= header && header < CSV_HEADER_COUNT,
            "`header` is not a valid option");

        if ( header == CSV_HEADER_TRUE ) {
            auto error = csv_read_head(parse, file, buffer);

            if ( error != READ_ERROR_NONE )
                return error;
        }

        return csv_read_body(parse, file, buffer);
    }
} // namespace pax
