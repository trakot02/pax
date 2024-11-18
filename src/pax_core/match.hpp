#ifndef PAX_CORE_MATCH_HPP
#define PAX_CORE_MATCH_HPP

#include <pax_core/types.hpp>

namespace pax
{
    struct Buff;

    enum Match_Error : isize {
        MATCH_ERROR_NONE = 0,
        MATCH_ERROR_SOME = 1,

        MATCH_ERROR_COUNT = 2,
    };

    struct Match_Value {
        isize       count;
        Match_Error error;
    };

    Match_Value
    match_exact(Buff* buff, s8* token, s8 value);

    Match_Value
    match_lower(Buff* buff, s8* token, isize times);

    Match_Value
    match_not_lower(Buff* buff, s8* token, isize times);

    Match_Value
    match_upper(Buff* buff, s8* token, isize times);

    Match_Value
    match_not_upper(Buff* buff, s8* token, isize times);

    Match_Value
    match_alpha(Buff* buff, s8* token, isize times);

    Match_Value
    match_not_alpha(Buff* buff, s8* token, isize times);

    Match_Value
    match_alnum(Buff* buff, s8* token, isize times);

    Match_Value
    match_not_alnum(Buff* buff, s8* token, isize times);

    Match_Value
    match_digit(Buff* buff, s8* token, isize times);

    Match_Value
    match_not_digit(Buff* buff, s8* token, isize times);
} // namespace pax

#endif // PAX_CORE_MATCH_HPP
