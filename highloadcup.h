#pragma once

#include <new>

extern "C" {
#include <postgres.h>
}

template<typename T>
class PAllocNew
{
public:

    static void* operator new(std::size_t count, const std::nothrow_t& tag)
    {
        return palloc(count);
    }
};
