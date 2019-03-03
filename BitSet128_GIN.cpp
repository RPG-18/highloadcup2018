#include "Logger.h"
#include "BitSet128.h"

extern "C" {
#include <fmgr.h>
#include <access/gin.h>
#include <access/stratnum.h>

PG_FUNCTION_INFO_V1(gin_extract_value_bit128);
Datum gin_extract_value_bit128(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(bit128_cmp);
Datum bit128_cmp(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(gin_extract_query_bit128);
Datum gin_extract_query_bit128(PG_FUNCTION_ARGS);


PG_FUNCTION_INFO_V1(gin_bit128_consistent);
Datum gin_bit128_consistent(PG_FUNCTION_ARGS);

}

Datum gin_extract_value_bit128(PG_FUNCTION_ARGS)
{
    auto bitset = GETARG_BIT128(0);
    const auto bits = bitset->keys();

//    LOGGER->info("call gin_extract_value_bit128; key {} / sz {}", bitset->toCString(), bits.size());
//    LOGGER->flush();

    int32* nentries = (int32*) PG_GETARG_POINTER(1);
    *nentries = bits.size();
    Datum* entries = NULL;

    entries = (Datum*) palloc(sizeof(Datum) * bits.size());
    for (int i = 0; i < bits.size(); ++i)
    {
        entries[i] = DatumGetInt16(bits[i]);
    }

    PG_RETURN_POINTER(entries);
}

Datum bit128_cmp(PG_FUNCTION_ARGS)
{
    const auto a = PG_GETARG_INT16(0);
    const auto b = PG_GETARG_INT16(1);

//    LOGGER->info("call bit128_cmp {} {}", a, b);
//    LOGGER->flush();

    if (a < b)
    {
        PG_RETURN_INT32(-1);
    }

    if (a > b)
    {
        PG_RETURN_INT32(1);
    }

    PG_RETURN_INT32(0);
}

Datum gin_extract_query_bit128(PG_FUNCTION_ARGS)
{
    int32* nentries = (int32*) PG_GETARG_POINTER(1);
    StrategyNumber strategy = PG_GETARG_UINT16(2);
    int32* searchMode = (int32*) PG_GETARG_POINTER(6);

    Datum* res = nullptr;

    const auto a = GETARG_BIT128(0);

    const auto keys = a->keys();

//    {
//        LOGGER->info("call gin_extract_query_bit128 ");
//        LOGGER->info("    strategy {}", strategy);
//        LOGGER->info("    nentries {}", *nentries);
//        LOGGER->info("    query {}", a->toCString());
//    }

    *nentries = keys.size();
    res = (Datum*) palloc(sizeof(Datum) * keys.size());
    for (int i = 0; i < keys.size(); ++i)
    {
        res[i] = DatumGetInt16(keys[i]);
    }

    switch (strategy)
    {
        case RTOverlapStrategyNumber:  //  &&
            if (*nentries > 0)
            {
                *searchMode = GIN_SEARCH_MODE_DEFAULT;
            }
            else
            {
                *searchMode = GIN_SEARCH_MODE_ALL;
            }
            break;
        case RTContainsStrategyNumber: //  @>
            if (*nentries > 0)
            {
                *searchMode = GIN_SEARCH_MODE_DEFAULT;
            }
            else
            {
                *searchMode = GIN_SEARCH_MODE_ALL;
            }
            break;
    }

    PG_RETURN_POINTER(res);
}

Datum gin_bit128_consistent(PG_FUNCTION_ARGS)
{
    bool* check = (bool*) PG_GETARG_POINTER(0);

    StrategyNumber strategy = PG_GETARG_UINT16(1);
    int32 nkeys = PG_GETARG_INT32(3);
    bool* recheck = (bool*) PG_GETARG_POINTER(5);
    bool res = true;
    switch (strategy)
    {
        case RTOverlapStrategyNumber:  //  &&
        {
            for (int i = 0; i < nkeys; ++i)
            {
                if (check[i])
                {
                    res = true;
                }
            };
            *recheck = false;
        }
            break;
        case RTContainsStrategyNumber: //  @>
        {
            for (int i = 0; i < nkeys; ++i)
            {
                if (check[i])
                {
                    res = true;
                }
            };
            *recheck = true;
        }
            break;
    }

    PG_RETURN_BOOL(res);
}