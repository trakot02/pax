#ifndef PAX_CORE_PARSE_CSV_HPP
#define PAX_CORE_PARSE_CSV_HPP

#include <pax_core/types.hpp>

namespace pax
{
    struct CSV_Parse {
        void* context;

        void (*func_header) (
            void* ctxt, Str8 token, isize col
        );

        void (*func_column) (
            void* ctxt, Str8 token, isize col, isize row
        );
    };

    CSV_Parse
    csv_parse_empty();

    void
    csv_parse_header(CSV_Parse self, Str8 token, isize col);

    void 
    csv_parse_column(CSV_Parse self, Str8 token, isize col, isize row);
} // namespace pax

#endif // PAX_CORE_PARSE_CSV_HPP
