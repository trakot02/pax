#ifndef PAX_CORE_TYPES_HPP
#define PAX_CORE_TYPES_HPP

#include <cstdint>

#define pax_type_width \
    (isize) sizeof

#define pax_type_align \
    (isize) alignof

namespace pax
{
    using byte = char;

    using usize = uintptr_t;
    using u64   = uint64_t;
    using u32   = uint32_t;
    using u16   = uint16_t;
    using u8    = uint8_t;

    using isize = intptr_t;
    using i64   = int64_t;
    using i32   = int32_t;
    using i16   = int16_t;
    using i8    = int8_t;

    using f64 = double;
    using f32 = float;

    static const isize WIDTH_BYTE  = pax_type_width(byte);
    static const isize WIDTH_USIZE = pax_type_width(usize);
    static const isize WIDTH_U64   = pax_type_width(u64);
    static const isize WIDTH_U32   = pax_type_width(u32);
    static const isize WIDTH_U16   = pax_type_width(u16);
    static const isize WIDTH_U8    = pax_type_width(u8);
    static const isize WIDTH_ISIZE = pax_type_width(isize);
    static const isize WIDTH_I64   = pax_type_width(i64);
    static const isize WIDTH_I32   = pax_type_width(i32);
    static const isize WIDTH_I16   = pax_type_width(i16);
    static const isize WIDTH_I8    = pax_type_width(i8);
    static const isize WIDTH_F64   = pax_type_width(f64);
    static const isize WIDTH_F32   = pax_type_width(f32);

    static const isize ALIGN_BYTE  = pax_type_align(byte);
    static const isize ALIGN_USIZE = pax_type_align(usize);
    static const isize ALIGN_U64   = pax_type_align(u64);
    static const isize ALIGN_U32   = pax_type_align(u32);
    static const isize ALIGN_U16   = pax_type_align(u16);
    static const isize ALIGN_U8    = pax_type_align(u8);
    static const isize ALIGN_ISIZE = pax_type_align(isize);
    static const isize ALIGN_I64   = pax_type_align(i64);
    static const isize ALIGN_I32   = pax_type_align(i32);
    static const isize ALIGN_I16   = pax_type_align(i16);
    static const isize ALIGN_I8    = pax_type_align(i8);
    static const isize ALIGN_F64   = pax_type_align(f64);
    static const isize ALIGN_F32   = pax_type_align(f32);

    static const isize WIDTH_MAX = WIDTH_ISIZE;
    static const isize WIDTH_MIN = WIDTH_BYTE;
    static const isize ALIGN_MAX = ALIGN_ISIZE;
    static const isize ALIGN_MIN = ALIGN_BYTE;

    static const byte  MAX_BYTE  = 0xff;
    static const usize MAX_USIZE = UINTPTR_MAX;
    static const u64   MAX_U64   = UINT64_MAX;
    static const u32   MAX_U32   = UINT32_MAX;
    static const u16   MAX_U16   = UINT16_MAX;
    static const u8    MAX_U8    = UINT8_MAX;
    static const isize MAX_ISIZE = INTPTR_MAX;
    static const i64   MAX_I64   = INT64_MAX;
    static const i32   MAX_I32   = INT32_MAX;
    static const i16   MAX_I16   = INT16_MAX;
    static const i8    MAX_I8    = INT8_MAX;

    static const byte  MIN_BYTE  = 0x0;
    static const usize MIN_USIZE = 0x0;
    static const u64   MIN_U64   = 0x0;
    static const u32   MIN_U32   = 0x0;
    static const u16   MIN_U16   = 0x0;
    static const u8    MIN_U8    = 0x0;
    static const isize MIN_ISIZE = INTPTR_MIN;
    static const i64   MIN_I64   = INT64_MIN;
    static const i32   MIN_I32   = INT32_MIN;
    static const i16   MIN_I16   = INT16_MIN;
    static const i8    MIN_I8    = INT8_MIN;
} // namespace pax

#endif // PAX_CORE_TYPES_HPP
