#include <pax_core/parse_csv.hpp>
#include <pax_core/match_csv.hpp>
#include <pax_core/buff.hpp>
#include <pax_core/file.hpp>

namespace pax
{
    CSV_Parse
    csv_parse_empty()
    {
        CSV_Parse parse;

        parse.context = 0;

        parse.func_header = 0;
        parse.func_column = 0;

        return parse;
    }

    void
    csv_header(CSV_Parse self, Str8 token, isize col)
    {
        pax_guard(self.func_header != 0,
            "`self.func_header` is null");

        (*self.func_header)(self.context, token, col);
    }

    void
    csv_column(CSV_Parse self, Str8 token, isize col, isize row)
    {
        pax_guard(self.func_column != 0,
            "`self.func_column` is null");

        (*self.func_column)(self.context, token, col, row);
    }

    Read_Error
    csv_parse(CSV_Parse self, Buff* buffer, File* file, CSV_Header header)
    {
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(file   != 0, "`file` is null");

        pax_guard(0 <= header && header < CSV_HEADER_COUNT,
            "`header` is not a valid option");

        auto read  = read_value_empty();
        auto match = csv_match_empty();

        Str8  token = "";
        isize head  = 0;
        isize col   = 0;
        isize row   = 0;

        if ( header == CSV_HEADER_TRUE ) {
            do {
                for ( ; match.line == 0; col += 1 ) {
                    match = csv_match_column(buffer, &token);
                    token = str8_trim(token);

                    if ( match.error != MATCH_ERROR_NONE )
                        break;

                    csv_header(self, token, col);
                }

                if ( match.line == 1 ) break;

                buff_shift(buffer);

                read = file_read_buff(file, buffer);
            } while ( read.count != 0 );
        }

        col = 0;

        do {
            while ( true ) {
                match = csv_match_column(buffer, &token);
                token = str8_trim(token);

                if ( match.error != MATCH_ERROR_NONE )
                    break;

                csv_column(self, token, col, row);

                col += 1;

                if ( match.line != 0 )
                    col = 0, row += 1;
            }

            buff_shift(buffer);

            read = file_read_buff(file, buffer);
        } while ( read.count != 0 );

        return read.error;
    }
} // namespace pax
