#ifndef PAX_CORE_READ_CSV_HPP
#define PAX_CORE_READ_CSV_HPP

#include <pax_core/types.hpp>
#include <pax_core/read.hpp>
#include <pax_core/parse_csv.hpp>

namespace pax
{
    struct Buff;
    struct File;

    enum CSV_Header : isize {
        CSV_HEADER_FALSE = 0,
        CSV_HEADER_TRUE  = 1,

        CSV_HEADER_COUNT = 2,
    };

    Read_Error
    csv_read_head(CSV_Parse parse, File* file, Buff* buffer);

    Read_Error
    csv_read_body(CSV_Parse parse, File* file, Buff* buffer);

    Read_Error
    csv_read(CSV_Parse parse, File* file, Buff* buffer, CSV_Header header = CSV_HEADER_FALSE);
} // namespace pax

#endif // PAX_CORE_READ_CSV_HPP
