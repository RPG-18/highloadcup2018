-- Phone
CREATE TYPE phone;

CREATE OR REPLACE FUNCTION phone_in ( cstring )
RETURNS phone AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION phone_out ( phone )
RETURNS cstring AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION phone_equal ( phone, phone )
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION phone_equal_internal ( phone, phone )
RETURNS integer AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION phone_lt ( phone, phone )
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION phone_le ( phone, phone )
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION phone_ge ( phone, phone )
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
CREATE OR REPLACE FUNCTION phone_gt ( phone, phone )

RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION get_code ( phone )
RETURNS int AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


CREATE TYPE phone (
   INTERNALLENGTH = 8,
   INPUT = phone_in,
   OUTPUT = phone_out,
   ALIGNMENT = int4,
   COLLATE TRUE
);

CREATE OPERATOR = (
   PROCEDURE = phone_equal,
   LEFTARG = phone,
   RIGHTARG = phone,
   COMMUTATOR = =,
   NEGATOR = !=
);

CREATE OPERATOR < (
  PROCEDURE = phone_lt,
  LEFTARG = phone,
  RIGHTARG = phone,
  NEGATOR = >
);

CREATE OPERATOR <= (
  PROCEDURE = phone_le,
  LEFTARG = phone,
  RIGHTARG = phone
);

CREATE OPERATOR >= (
  PROCEDURE = phone_ge,
  LEFTARG = phone,
  RIGHTARG = phone
);

CREATE OPERATOR > (
  PROCEDURE = phone_gt,
  LEFTARG = phone,
  RIGHTARG = phone,
  NEGATOR = <
);

CREATE OPERATOR CLASS btree_phone_ops
  DEFAULT FOR TYPE phone USING btree AS
   OPERATOR        1        <,
   OPERATOR        2        <=,
   OPERATOR        3        =,
   OPERATOR        4        >=,
   OPERATOR        5        >,
   FUNCTION        1       phone_equal_internal ( phone, phone );

-- BitSet128

CREATE TYPE bit128;

CREATE OR REPLACE FUNCTION bit128_in ( cstring )
RETURNS bit128 AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION bit128_out ( bit128 )
RETURNS cstring AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION bit128_equal ( bit128, bit128 )
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION bit128_intersection ( bit128, bit128 )
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION bit128_containts ( bit128, bit128 )
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION bit128_cmp ( int2, int2 )
RETURNS int AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE TYPE bit128 (
   INTERNALLENGTH = 16,
   INPUT = bit128_in,
   OUTPUT = bit128_out,
   ALIGNMENT = int4

);

CREATE OPERATOR = (
   PROCEDURE = bit128_equal,
   LEFTARG = bit128,
   RIGHTARG = bit128,
   COMMUTATOR = =,
   NEGATOR = !=
);

CREATE OPERATOR && (
   PROCEDURE = bit128_intersection,
   LEFTARG = bit128,
   RIGHTARG = bit128
);

CREATE OPERATOR @> (
   PROCEDURE = bit128_containts,
   LEFTARG = bit128,
   RIGHTARG = bit128
);

-- GIN Support
CREATE OR REPLACE FUNCTION gin_extract_value_bit128 ( bit128, internal, internal)
RETURNS internal AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION gin_extract_query_bit128(bit128, internal, int2, internal, internal)
RETURNS internal AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION gin_bit128_consistent(internal, int2, anyelement, int4, internal, internal)
RETURNS bool AS
'libhighloadcup'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OPERATOR CLASS bit128_ops
DEFAULT FOR TYPE bit128 USING gin
AS
    OPERATOR        3       &&,
    OPERATOR        6       =,
    OPERATOR        7       @>,
    FUNCTION        1       bit128_cmp (int2, int2 ),
    FUNCTION        2       gin_extract_value_bit128(bit128, internal, internal),
    FUNCTION        3       gin_extract_query_bit128(bit128, internal, int2, internal, internal),
    FUNCTION        4       gin_bit128_consistent(internal, int2, anyelement, int4, internal, internal),
STORAGE  int2;