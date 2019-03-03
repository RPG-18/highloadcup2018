#include "highloadcup.h"
#include "Phone.h"
#include "BitSet128.h"

extern "C" {

#include <fmgr.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(highloadcup_version);
Datum highloadcup_version(PG_FUNCTION_ARGS);

}

Datum highloadcup_version(PG_FUNCTION_ARGS)
{
    const char* version = "0.1";
    auto bytes = (char*)palloc(strlen(version));
    memcpy(bytes, version, strlen(version));
    PG_RETURN_CSTRING(bytes);
}
