#ifndef PAX_CORE_BASE_ALLOC_HPP
#define PAX_CORE_BASE_ALLOC_HPP

#include <pax_core/types.hpp>
#include <pax_core/mem/alloc.hpp>

namespace pax
{
    Alloc_Error
    base_request(Alloc_Value* value);

    void
    base_release(Alloc_Value value);

    void
    base_clear();

    Alloc
    base_alloc();
} // namespace pax

#endif // PAX_CORE_BASE_ALLOC_HPP
