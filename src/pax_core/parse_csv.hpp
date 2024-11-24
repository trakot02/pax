#ifndef PAX_CORE_PARSE_CSV_HPP
#define PAX_CORE_PARSE_CSV_HPP

#include <pax_core/types.hpp>
#include <pax_core/parse.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    struct Buff;
    struct File;

    enum CSV_Header : isize {
        CSV_HEADER_FALSE = 0,
        CSV_HEADER_TRUE  = 1,

        CSV_HEADER_COUNT = 2,
    };

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
    csv_header(CSV_Parse self, Str8 token, isize col);

    void
    csv_column(CSV_Parse self, Str8 token, isize col, isize row);

    Read_Error
    csv_parse(CSV_Parse self, Buff* buffer, File* file, CSV_Header header = CSV_HEADER_FALSE);
} // namespace pax

#endif // PAX_CORE_PARSE_CSV_HPP
