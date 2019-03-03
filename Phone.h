#pragma once

#include "highloadcup.h"

class Phone : public PAllocNew<Phone>
{
public:

    bool fromCString(char* str) noexcept;
    char* toCString() const noexcept;

    int code() const noexcept;

    bool operator==(const Phone& b) const noexcept;
    bool operator<(const Phone& b) const noexcept;
    bool operator<=(const Phone& b) const noexcept;
    bool operator>(const Phone& b) const noexcept;
    bool operator>=(const Phone& b) const noexcept;

private:

    uint64_t m_data = 0;
};

#define GETARG_PHONE(x) reinterpret_cast<Phone*>(PG_GETARG_POINTER(x))