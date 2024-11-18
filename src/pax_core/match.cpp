#include <pax_core/match.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    Match_Value
    match_exact(Buff* buff, s8* token, s8 value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = value.size;

        if ( count >= avail )
            return {avail, MATCH_ERROR_SOME};

        for ( isize i = 0; i < count; i += 1 ) {
            if ( buff->head[i] != value[i] )
                return {i + 1, MATCH_ERROR_SOME};
        }

        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_lower(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( temp < 'a' || temp > 'z' )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_not_lower(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( temp >= 'a' && temp <= 'z' )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_upper(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( temp < 'A' || temp > 'Z' )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_not_upper(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( temp >= 'A' && temp <= 'Z' )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_alpha(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( (temp < 'a' || temp > 'z') &&
                 (temp < 'A' || temp > 'Z') )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_not_alpha(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( (temp >= 'a' && temp <= 'z') ||
                 (temp >= 'A' && temp <= 'Z') )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_alnum(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( (temp < 'a' || temp > 'z') &&
                 (temp < 'A' || temp > 'Z') &&
                 (temp < '0' || temp > '9') )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_not_alnum(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( (temp >= 'a' && temp <= 'z') ||
                 (temp >= 'A' && temp <= 'Z') ||
                 (temp >= '0' && temp <= '9') )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_digit(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( temp < '0' || temp > '9' )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }

    Match_Value
    match_not_digit(Buff* buff, s8* token)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = 0;

        if ( count >= avail )
            return {0, MATCH_ERROR_SOME};

        for ( isize i = 0; i < avail; i += 1 ) {
            byte temp = self.head[i];

            if ( temp >= '0' && temp <= '9' )
                count = i, i = avail;
        }

        if ( count <= 0 )
            return {0, MATCH_ERROR_SOME};
            
        if ( token != 0 ) {
            token->addr = self.head;
            token->size = count;
        }

        self.head += count;

        return {count, MATCH_ERROR_NONE};
    }


} // namespace pax
