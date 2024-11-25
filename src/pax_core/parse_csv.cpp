#include <pax_core/parse_csv.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

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
    csv_parse_header(CSV_Parse self, Str8 token, isize col)
    {
        pax_guard(self.func_header != 0,
            "`self.func_header` is null");

        return (*self.func_header)(self.context, token, col);
    }

    void
    csv_parse_column(CSV_Parse self, Str8 token, isize col, isize row)
    {
        pax_guard(self.func_column != 0,
            "`self.func_column` is null");

        return (*self.func_column)(self.context, token, col, row);
    }
} // namespace pax
