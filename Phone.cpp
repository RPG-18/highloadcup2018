#include "Phone.h"

extern "C" {

#include <fmgr.h>

PG_FUNCTION_INFO_V1(phone_in);
Datum phone_in(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(phone_out);
Datum phone_out(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(get_code);
Datum get_code(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(phone_equal);
Datum phone_equal(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(phone_lt);
Datum phone_lt(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(phone_le);
Datum phone_le(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(phone_ge);
Datum phone_ge(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(phone_gt);
Datum phone_gt(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(phone_equal_internal);
Datum phone_equal_internal(PG_FUNCTION_ARGS);
}

namespace
{

constexpr int8_t CODE_OFFSET = 32;
constexpr int8_t COUNTRY_OFFSET = 28;

constexpr uint64_t CODE_MASK = 0x3FF00000000;
constexpr uint64_t COUNTRY_MASK = 0x000F0000000;
constexpr uint64_t NUMBER_MASK =  0x00007FFFFFF;
}

bool Phone::fromCString(char* str) noexcept
{
    uint64_t country = 0;
    uint64_t code = 0;
    uint64_t number = 0;

    if (sscanf(str, "%lu(%lu)%lu", &country, &code, &number) != 3)
    {
        return false;
    }

    code = code << CODE_OFFSET;
    country = country << COUNTRY_OFFSET;

    m_data = number | country | code;
    return true;
}

char* Phone::toCString() const noexcept
{
    uint64_t country = (m_data & COUNTRY_MASK) >> COUNTRY_OFFSET;
    uint64_t code = (m_data & CODE_MASK) >> CODE_OFFSET;
    uint64_t number = m_data & NUMBER_MASK;

    return psprintf("%lu(%lu)%07lu", country, code, number);
}

int Phone::code() const noexcept
{
    const auto code = (m_data & CODE_MASK) >> CODE_OFFSET;
    return code;
}


bool Phone::operator==(const Phone& b) const noexcept
{
    return m_data == b.m_data;
}

bool Phone::operator<(const Phone& b) const noexcept
{
    return m_data < b.m_data;
}

bool Phone::operator<=(const Phone& b) const noexcept
{
    return m_data <= b.m_data;
}

bool Phone::operator>(const Phone& b) const noexcept
{
    return m_data > b.m_data;
}

bool Phone::operator>=(const Phone& b) const noexcept
{
    return m_data >= b.m_data;
}


////////////////////////////////////////////////////////////////////

Datum phone_in(PG_FUNCTION_ARGS)
{
    char* str = PG_GETARG_CSTRING(0);

    auto v = new(std::nothrow) Phone;
    if (!v->fromCString(str))
    {
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                errmsg(
                    "invalid input syntax for phone: \"%s\"",
                    str
                )));
    }

    PG_RETURN_POINTER(v);
}

Datum phone_out(PG_FUNCTION_ARGS)
{
    const auto phone = GETARG_PHONE(0);

    PG_RETURN_CSTRING(phone->toCString());
}

Datum get_code(PG_FUNCTION_ARGS)
{
    const auto phone = GETARG_PHONE(0);
    PG_RETURN_INT32(phone->code());
}

Datum phone_equal(PG_FUNCTION_ARGS)
{
    const auto a = GETARG_PHONE(0);
    const auto b = GETARG_PHONE(1);
    PG_RETURN_BOOL(*a == *b);
}

Datum phone_lt(PG_FUNCTION_ARGS)
{
    const auto a = GETARG_PHONE(0);
    const auto b = GETARG_PHONE(1);
    PG_RETURN_BOOL(*a < *b);
}

Datum phone_le(PG_FUNCTION_ARGS)
{
    const auto a = GETARG_PHONE(0);
    const auto b = GETARG_PHONE(1);
    PG_RETURN_BOOL(*a <= *b);
}

Datum phone_ge(PG_FUNCTION_ARGS)
{
    const auto a = GETARG_PHONE(0);
    const auto b = GETARG_PHONE(1);
    PG_RETURN_BOOL(*a >= *b);
}

Datum phone_gt(PG_FUNCTION_ARGS)
{
    const auto a = GETARG_PHONE(0);
    const auto b = GETARG_PHONE(1);
    PG_RETURN_BOOL(*a > *b);
}

Datum phone_equal_internal(PG_FUNCTION_ARGS)
{
    const auto a = GETARG_PHONE(0);
    const auto b = GETARG_PHONE(1);
    if (*a < *b)
    {
        PG_RETURN_INT32(-1);
    }

    if (*a > *b)
    {
        PG_RETURN_INT32(1);
    }

    PG_RETURN_INT32(0);
}