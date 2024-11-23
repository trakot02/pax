#ifndef PAX_CORE_MATCH_HPP
#define PAX_CORE_MATCH_HPP

#include <pax_core/types.hpp>

namespace pax
{
    enum Match_Error : isize {
        MATCH_ERROR_NONE = 0,
        MATCH_ERROR_SOME = 1,

        MATCH_ERROR_COUNT = 2,
    };
} // namespace pax

#endif // PAX_CORE_MATCH_HPP
