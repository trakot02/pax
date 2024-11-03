#ifndef PAX_CORE_READ_HPP
#define PAX_CORE_READ_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    struct Buff;

    enum Read_Err : isize {
        _read_err_none     = 0,
        _read_err_overflow = 1,
        _read_err_syntax   = 2,

        _read_err_count = 3,
    };

    enum Read_Radix : isize {
        _read_radix_dec = 0,
        _read_radix_hex = 1,
        _read_radix_bin = 2,
        _read_radix_oct = 3,

        _read_radix_count = 4,
    };

    struct Read_Res {
        isize    bytes;
        Read_Err error;
    };

    struct Read {
        Read_Res (*buff_func)
            (void* self, Buff* value, byte delim);

        Read_Res (*s8_func)
            (void* self, s8* value, byte delim);

        Read_Res (*u64_func)
            (void* self, u64* value, Read_Radix radix);

        Read_Res (*i64_func)
            (void* self, i64* value, Read_Radix radix);

        void* self;
    };

    extern const Array<s8, _read_err_count> READ_ERR_TITLE;

    Read_Res
    read_buff(const Read* read, Buff* value, byte delim);

    Read_Res
    read_s8(const Read* read, s8* value, byte delim);

    Read_Res
    read_u64(const Read* read, u64* value, Read_Radix radix = _read_radix_dec);

    Read_Res
    read_i64(const Read* read, i64* value, Read_Radix radix = _read_radix_dec);
} // namespace pax

#endif // PAX_CORE_READ_HPP
