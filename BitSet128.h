#pragma once

#include <cstdint>
#include <vector>

#include "highloadcup.h"

class BitSet128: public PAllocNew<BitSet128>
{
public:

    bool haveIntersection(const BitSet128& b) const noexcept;

    bool contains(const BitSet128& b)  const noexcept;

    bool operator==(const BitSet128& b) const noexcept;
    bool operator<(const BitSet128& b) const noexcept;
    bool operator>(const BitSet128& b) const noexcept;

    bool fromCString(char* str) noexcept;
    char* toCString() const noexcept;

    std::vector<uint8_t> keys() const;

private:

    uint128 m_bits = 0;
};

#define GETARG_BIT128(x) reinterpret_cast<BitSet128*>(PG_GETARG_POINTER(x))