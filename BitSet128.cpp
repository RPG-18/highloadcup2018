#define RAPIDJSON_PARSE_DEFAULT_FLAGS kParseNoFlags

#include "Logger.h"
#include "BitSet128.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"

extern "C" {
#include <fmgr.h>
#include <utils/array.h>

PG_FUNCTION_INFO_V1(bit128_in);
Datum bit128_in(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(bit128_out);
Datum bit128_out(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(bit128_equal);
Datum bit128_equal(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(bit128_intersection);
Datum bit128_intersection(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(bit128_containts);
Datum bit128_containts(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(to_array);
Datum to_array(PG_FUNCTION_ARGS);

extern Datum array_in(PG_FUNCTION_ARGS);
}


namespace
{

struct Handler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, Handler>
{
    bool Uint(unsigned u)
    {
        uint128 v = 0x1;
        value = value | (v << u);

        return true;
    }

    uint128 value = 0;
};

}

bool BitSet128::haveIntersection(const BitSet128& b) const noexcept
{
    return (m_bits & b.m_bits) != 0;
}

bool BitSet128::contains(const BitSet128& b) const noexcept
{
    return (m_bits & b.m_bits) == b.m_bits;
}

bool BitSet128::operator==(const BitSet128& b) const noexcept
{
    return m_bits == b.m_bits;
}

bool BitSet128::operator<(const BitSet128& b) const noexcept
{
    return m_bits < b.m_bits;
}

bool BitSet128::operator>(const BitSet128& b) const noexcept
{
    return m_bits > b.m_bits;
}

bool BitSet128::fromCString(char* str) noexcept
{
    using namespace rapidjson;

    Handler h;

    Reader reader;
    StringStream ss(str);

    reader.Parse(ss, h);

    m_bits = h.value;
    return true;
}

char* BitSet128::toCString() const noexcept
{
    using namespace rapidjson;
    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartArray();

    auto c = m_bits;
    int indx = 0;
    while (c != 0)
    {
        if ((c & 0x01) != 0)
        {
            writer.Int(indx);

        }
        c = c >> 1;
        ++indx;
    }

    writer.EndArray();

    const auto size = s.GetSize() + 1;
    auto mem = static_cast<char*>(palloc(s.GetSize() + 1));
    if (mem == nullptr)
    {
        return nullptr;
    }
    memcpy(mem, s.GetString(), size);
    return mem;
}

std::vector<uint8_t> BitSet128::keys() const
{
    std::vector<uint8_t> out;
    out.reserve(10);

    auto c = m_bits;
    int indx = 0;
    while (c != 0)
    {
        if ((c & 0x01) != 0)
        {
            out.push_back(indx);
        }
        c = c >> 1;
        ++indx;
    }

    return out;
}


Datum bit128_in(PG_FUNCTION_ARGS)
{
    char* str = PG_GETARG_CSTRING(0);
    auto v = new(std::nothrow) BitSet128;
    if (!v->fromCString(str))
    {
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                errmsg(
                    "invalid input syntax for bit128: \"%s\"",
                    str
                )));
    }

    PG_RETURN_POINTER(v);
}

Datum bit128_out(PG_FUNCTION_ARGS)
{
    auto b = GETARG_BIT128(0);
    PG_RETURN_CSTRING(b->toCString());
}

Datum bit128_equal(PG_FUNCTION_ARGS)
{
    const auto a = GETARG_BIT128(0);
    const auto b = GETARG_BIT128(1);
    PG_RETURN_BOOL(*a == *b);
}

Datum bit128_intersection(PG_FUNCTION_ARGS)
{
//LOGGER->info("call bit128_intersection");

    const auto a = GETARG_BIT128(0);
    const auto b = GETARG_BIT128(1);
    PG_RETURN_BOOL(a->haveIntersection(*b));
}

//containts
Datum bit128_containts(PG_FUNCTION_ARGS)
{
//LOGGER->info("call bit128_containts");

    const auto a = GETARG_BIT128(0);
    const auto b = GETARG_BIT128(1);
    PG_RETURN_BOOL(a->contains(*b));
}