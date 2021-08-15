#include "shared.rsh"

uint32_t x;
uint32_t y;
uint32_t z;

rs_element elemNull;
rs_element elemNonNull;
rs_type typeNull;
rs_type typeNonNull;
rs_allocation allocNull;
rs_allocation allocNonNull;
rs_sampler samplerNull;
rs_sampler samplerNonNull;
rs_script scriptNull;
rs_script scriptNonNull;

volatile rs_data_type dt;
volatile rs_data_kind dk;

volatile rs_sampler_value rsv;

volatile rs_time_t rst;
volatile static rs_tm rstm;

char *allocPtr;
rs_allocation allocDst;

volatile bool b;
volatile char c;
volatile char2 c2;
volatile char3 c3;
volatile char4 c4;
volatile uchar uc;
volatile uchar2 uc2;
volatile uchar3 uc3;
volatile uchar4 uc4;
volatile short s;
volatile short2 s2;
volatile short3 s3;
volatile short4 s4;
volatile ushort us;
volatile ushort2 us2;
volatile ushort3 us3;
volatile ushort4 us4;
volatile int i;
volatile int2 i2;
volatile int3 i3;
volatile int4 i4;
volatile uint ui;
volatile uint2 ui2;
volatile uint3 ui3;
volatile uint4 ui4;
volatile long l;
volatile long2 l2;
volatile long3 l3;
volatile long4 l4;
volatile ulong ul;
volatile ulong2 ul2;
volatile ulong3 ul3;
volatile ulong4 ul4;
volatile long long ll;
volatile unsigned long long ull;
volatile float f;
volatile float2 f2;
volatile float3 f3;
volatile float4 f4;
volatile double d;
volatile double2 d2;
volatile double3 d3;
volatile double4 d4;
float fa[16];

rs_allocation aChar;
rs_allocation aChar2;
rs_allocation aChar3;
rs_allocation aChar4;
rs_allocation aUChar;
rs_allocation aUChar2;
rs_allocation aUChar3;
rs_allocation aUChar4;
rs_allocation aShort;
rs_allocation aShort2;
rs_allocation aShort3;
rs_allocation aShort4;
rs_allocation aUShort;
rs_allocation aUShort2;
rs_allocation aUShort3;
rs_allocation aUShort4;
rs_allocation aInt;
rs_allocation aInt2;
rs_allocation aInt3;
rs_allocation aInt4;
rs_allocation aUInt;
rs_allocation aUInt2;
rs_allocation aUInt3;
rs_allocation aUInt4;
rs_allocation aLong;
rs_allocation aLong2;
rs_allocation aLong3;
rs_allocation aLong4;
rs_allocation aULong;
rs_allocation aULong2;
rs_allocation aULong3;
rs_allocation aULong4;
rs_allocation aFloat;
rs_allocation aFloat2;
rs_allocation aFloat3;
rs_allocation aFloat4;
rs_allocation aDouble;
rs_allocation aDouble2;
rs_allocation aDouble3;
rs_allocation aDouble4;

rs_matrix2x2 m2x2;
rs_matrix3x3 m3x3;
rs_matrix4x4 m4x4;

// This function just checks that all of the called functions are
// able to be linked. It is not intended to be executed!
void check_api_presence() {
    /********************************
     * DO NOT EXECUTE THIS FUNCTION *
     ********************************/
    rsSendToClientBlocking(RS_MSG_TEST_FAILED);

    // rs_allocation.rsh
    c = rsGetElementAt_char(aChar, 0);
    rsSetElementAt_char(aChar, c, 0);
    c = rsGetElementAt_char(aChar, 0, 0);
    rsSetElementAt_char(aChar, c, 0, 0);
    c = rsGetElementAt_char(aChar, 0, 0, 0);
    rsSetElementAt_char(aChar, c, 0, 0, 0);
    c2 = rsGetElementAt_char2(aChar2, 0);
    rsSetElementAt_char2(aChar2, c2, 0);
    c2 = rsGetElementAt_char2(aChar2, 0, 0);
    rsSetElementAt_char2(aChar2, c2, 0, 0);
    c2 = rsGetElementAt_char2(aChar2, 0, 0, 0);
    rsSetElementAt_char2(aChar2, c2, 0, 0, 0);
    c3 = rsGetElementAt_char3(aChar3, 0);
    rsSetElementAt_char3(aChar3, c3, 0);
    c3 = rsGetElementAt_char3(aChar3, 0, 0);
    rsSetElementAt_char3(aChar3, c3, 0, 0);
    c3 = rsGetElementAt_char3(aChar3, 0, 0, 0);
    rsSetElementAt_char3(aChar3, c3, 0, 0, 0);
    c4 = rsGetElementAt_char4(aChar4, 0);
    rsSetElementAt_char4(aChar4, c4, 0);
    c4 = rsGetElementAt_char4(aChar4, 0, 0);
    rsSetElementAt_char4(aChar4, c4, 0, 0);
    c4 = rsGetElementAt_char4(aChar4, 0, 0, 0);
    rsSetElementAt_char4(aChar4, c4, 0, 0, 0);

    uc = rsGetElementAt_uchar(aUChar, 0);
    rsSetElementAt_uchar(aUChar, uc, 0);
    uc = rsGetElementAt_uchar(aUChar, 0, 0);
    rsSetElementAt_uchar(aUChar, uc, 0, 0);
    uc = rsGetElementAt_uchar(aUChar, 0, 0, 0);
    rsSetElementAt_uchar(aUChar, uc, 0, 0, 0);
    uc2 = rsGetElementAt_uchar2(aUChar2, 0);
    rsSetElementAt_uchar2(aUChar2, uc2, 0);
    uc2 = rsGetElementAt_uchar2(aUChar2, 0, 0);
    rsSetElementAt_uchar2(aUChar2, uc2, 0, 0);
    uc2 = rsGetElementAt_uchar2(aUChar2, 0, 0, 0);
    rsSetElementAt_uchar2(aUChar2, uc2, 0, 0, 0);
    uc3 = rsGetElementAt_uchar3(aUChar3, 0);
    rsSetElementAt_uchar3(aUChar3, uc3, 0);
    uc3 = rsGetElementAt_uchar3(aUChar3, 0, 0);
    rsSetElementAt_uchar3(aUChar3, uc3, 0, 0);
    uc3 = rsGetElementAt_uchar3(aUChar3, 0, 0, 0);
    rsSetElementAt_uchar3(aUChar3, uc3, 0, 0, 0);
    uc4 = rsGetElementAt_uchar4(aUChar4, 0);
    rsSetElementAt_uchar4(aUChar4, uc4, 0);
    uc4 = rsGetElementAt_uchar4(aUChar4, 0, 0);
    rsSetElementAt_uchar4(aUChar4, uc4, 0, 0);
    uc4 = rsGetElementAt_uchar4(aUChar4, 0, 0, 0);
    rsSetElementAt_uchar4(aUChar4, uc4, 0, 0, 0);

    s = rsGetElementAt_short(aShort, 0);
    rsSetElementAt_short(aShort, s, 0);
    s = rsGetElementAt_short(aShort, 0, 0);
    rsSetElementAt_short(aShort, s, 0, 0);
    s = rsGetElementAt_short(aShort, 0, 0, 0);
    rsSetElementAt_short(aShort, s, 0, 0, 0);
    s2 = rsGetElementAt_short2(aShort2, 0);
    rsSetElementAt_short2(aShort2, s2, 0);
    s2 = rsGetElementAt_short2(aShort2, 0, 0);
    rsSetElementAt_short2(aShort2, s2, 0, 0);
    s2 = rsGetElementAt_short2(aShort2, 0, 0, 0);
    rsSetElementAt_short2(aShort2, s2, 0, 0, 0);
    s3 = rsGetElementAt_short3(aShort3, 0);
    rsSetElementAt_short3(aShort3, s3, 0);
    s3 = rsGetElementAt_short3(aShort3, 0, 0);
    rsSetElementAt_short3(aShort3, s3, 0, 0);
    s3 = rsGetElementAt_short3(aShort3, 0, 0, 0);
    rsSetElementAt_short3(aShort3, s3, 0, 0, 0);
    s4 = rsGetElementAt_short4(aShort4, 0);
    rsSetElementAt_short4(aShort4, s4, 0);
    s4 = rsGetElementAt_short4(aShort4, 0, 0);
    rsSetElementAt_short4(aShort4, s4, 0, 0);
    s4 = rsGetElementAt_short4(aShort4, 0, 0, 0);
    rsSetElementAt_short4(aShort4, s4, 0, 0, 0);

    us = rsGetElementAt_ushort(aUShort, 0);
    rsSetElementAt_ushort(aUShort, us, 0);
    us = rsGetElementAt_ushort(aUShort, 0, 0);
    rsSetElementAt_ushort(aUShort, us, 0, 0);
    us = rsGetElementAt_ushort(aUShort, 0, 0, 0);
    rsSetElementAt_ushort(aUShort, us, 0, 0, 0);
    us2 = rsGetElementAt_ushort2(aUShort2, 0);
    rsSetElementAt_ushort2(aUShort2, us2, 0);
    us2 = rsGetElementAt_ushort2(aUShort2, 0, 0);
    rsSetElementAt_ushort2(aUShort2, us2, 0, 0);
    us2 = rsGetElementAt_ushort2(aUShort2, 0, 0, 0);
    rsSetElementAt_ushort2(aUShort2, us2, 0, 0, 0);
    us3 = rsGetElementAt_ushort3(aUShort3, 0);
    rsSetElementAt_ushort3(aUShort3, us3, 0);
    us3 = rsGetElementAt_ushort3(aUShort3, 0, 0);
    rsSetElementAt_ushort3(aUShort3, us3, 0, 0);
    us3 = rsGetElementAt_ushort3(aUShort3, 0, 0, 0);
    rsSetElementAt_ushort3(aUShort3, us3, 0, 0, 0);
    us4 = rsGetElementAt_ushort4(aUShort4, 0);
    rsSetElementAt_ushort4(aUShort4, us4, 0);
    us4 = rsGetElementAt_ushort4(aUShort4, 0, 0);
    rsSetElementAt_ushort4(aUShort4, us4, 0, 0);
    us4 = rsGetElementAt_ushort4(aUShort4, 0, 0, 0);
    rsSetElementAt_ushort4(aUShort4, us4, 0, 0, 0);

    i = rsGetElementAt_int(aInt, 0);
    rsSetElementAt_int(aInt, i, 0);
    i = rsGetElementAt_int(aInt, 0, 0);
    rsSetElementAt_int(aInt, i, 0, 0);
    i = rsGetElementAt_int(aInt, 0, 0, 0);
    rsSetElementAt_int(aInt, i, 0, 0, 0);
    i2 = rsGetElementAt_int2(aInt2, 0);
    rsSetElementAt_int2(aInt2, i2, 0);
    i2 = rsGetElementAt_int2(aInt2, 0, 0);
    rsSetElementAt_int2(aInt2, i2, 0, 0);
    i2 = rsGetElementAt_int2(aInt2, 0, 0, 0);
    rsSetElementAt_int2(aInt2, i2, 0, 0, 0);
    i3 = rsGetElementAt_int3(aInt3, 0);
    rsSetElementAt_int3(aInt3, i3, 0);
    i3 = rsGetElementAt_int3(aInt3, 0, 0);
    rsSetElementAt_int3(aInt3, i3, 0, 0);
    i3 = rsGetElementAt_int3(aInt3, 0, 0, 0);
    rsSetElementAt_int3(aInt3, i3, 0, 0, 0);
    i4 = rsGetElementAt_int4(aInt4, 0);
    rsSetElementAt_int4(aInt4, i4, 0);
    i4 = rsGetElementAt_int4(aInt4, 0, 0);
    rsSetElementAt_int4(aInt4, i4, 0, 0);
    i4 = rsGetElementAt_int4(aInt4, 0, 0, 0);
    rsSetElementAt_int4(aInt4, i4, 0, 0, 0);

    ui = rsGetElementAt_uint(aUInt, 0);
    rsSetElementAt_uint(aUInt, ui, 0);
    ui = rsGetElementAt_uint(aUInt, 0, 0);
    rsSetElementAt_uint(aUInt, ui, 0, 0);
    ui = rsGetElementAt_uint(aUInt, 0, 0, 0);
    rsSetElementAt_uint(aUInt, ui, 0, 0, 0);
    ui2 = rsGetElementAt_uint2(aUInt2, 0);
    rsSetElementAt_uint2(aUInt2, ui2, 0);
    ui2 = rsGetElementAt_uint2(aUInt2, 0, 0);
    rsSetElementAt_uint2(aUInt2, ui2, 0, 0);
    ui2 = rsGetElementAt_uint2(aUInt2, 0, 0, 0);
    rsSetElementAt_uint2(aUInt2, ui2, 0, 0, 0);
    ui3 = rsGetElementAt_uint3(aUInt3, 0);
    rsSetElementAt_uint3(aUInt3, ui3, 0);
    ui3 = rsGetElementAt_uint3(aUInt3, 0, 0);
    rsSetElementAt_uint3(aUInt3, ui3, 0, 0);
    ui3 = rsGetElementAt_uint3(aUInt3, 0, 0, 0);
    rsSetElementAt_uint3(aUInt3, ui3, 0, 0, 0);
    ui4 = rsGetElementAt_uint4(aUInt4, 0);
    rsSetElementAt_uint4(aUInt4, ui4, 0);
    ui4 = rsGetElementAt_uint4(aUInt4, 0, 0);
    rsSetElementAt_uint4(aUInt4, ui4, 0, 0);
    ui4 = rsGetElementAt_uint4(aUInt4, 0, 0, 0);
    rsSetElementAt_uint4(aUInt4, ui4, 0, 0, 0);

    l = rsGetElementAt_long(aLong, 0);
    rsSetElementAt_long(aLong, l, 0);
    l = rsGetElementAt_long(aLong, 0, 0);
    rsSetElementAt_long(aLong, l, 0, 0);
    l = rsGetElementAt_long(aLong, 0, 0, 0);
    rsSetElementAt_long(aLong, l, 0, 0, 0);
    l2 = rsGetElementAt_long2(aLong2, 0);
    rsSetElementAt_long2(aLong2, l2, 0);
    l2 = rsGetElementAt_long2(aLong2, 0, 0);
    rsSetElementAt_long2(aLong2, l2, 0, 0);
    l2 = rsGetElementAt_long2(aLong2, 0, 0, 0);
    rsSetElementAt_long2(aLong2, l2, 0, 0, 0);
    l3 = rsGetElementAt_long3(aLong3, 0);
    rsSetElementAt_long3(aLong3, l3, 0);
    l3 = rsGetElementAt_long3(aLong3, 0, 0);
    rsSetElementAt_long3(aLong3, l3, 0, 0);
    l3 = rsGetElementAt_long3(aLong3, 0, 0, 0);
    rsSetElementAt_long3(aLong3, l3, 0, 0, 0);
    l4 = rsGetElementAt_long4(aLong4, 0);
    rsSetElementAt_long4(aLong4, l4, 0);
    l4 = rsGetElementAt_long4(aLong4, 0, 0);
    rsSetElementAt_long4(aLong4, l4, 0, 0);
    l4 = rsGetElementAt_long4(aLong4, 0, 0, 0);
    rsSetElementAt_long4(aLong4, l4, 0, 0, 0);

    ul = rsGetElementAt_ulong(aULong, 0);
    rsSetElementAt_ulong(aULong, ul, 0);
    ul = rsGetElementAt_ulong(aULong, 0, 0);
    rsSetElementAt_ulong(aULong, ul, 0, 0);
    ul = rsGetElementAt_ulong(aULong, 0, 0, 0);
    rsSetElementAt_ulong(aULong, ul, 0, 0, 0);
    ul2 = rsGetElementAt_ulong2(aULong2, 0);
    rsSetElementAt_ulong2(aULong2, ul2, 0);
    ul2 = rsGetElementAt_ulong2(aULong2, 0, 0);
    rsSetElementAt_ulong2(aULong2, ul2, 0, 0);
    ul2 = rsGetElementAt_ulong2(aULong2, 0, 0, 0);
    rsSetElementAt_ulong2(aULong2, ul2, 0, 0, 0);
    ul3 = rsGetElementAt_ulong3(aULong3, 0);
    rsSetElementAt_ulong3(aULong3, ul3, 0);
    ul3 = rsGetElementAt_ulong3(aULong3, 0, 0);
    rsSetElementAt_ulong3(aULong3, ul3, 0, 0);
    ul3 = rsGetElementAt_ulong3(aULong3, 0, 0, 0);
    rsSetElementAt_ulong3(aULong3, ul3, 0, 0, 0);
    ul4 = rsGetElementAt_ulong4(aULong4, 0);
    rsSetElementAt_ulong4(aULong4, ul4, 0);
    ul4 = rsGetElementAt_ulong4(aULong4, 0, 0);
    rsSetElementAt_ulong4(aULong4, ul4, 0, 0);
    ul4 = rsGetElementAt_ulong4(aULong4, 0, 0, 0);
    rsSetElementAt_ulong4(aULong4, ul4, 0, 0, 0);

    f = rsGetElementAt_float(aFloat, 0);
    rsSetElementAt_float(aFloat, f, 0);
    f = rsGetElementAt_float(aFloat, 0, 0);
    rsSetElementAt_float(aFloat, f, 0, 0);
    f = rsGetElementAt_float(aFloat, 0, 0, 0);
    rsSetElementAt_float(aFloat, f, 0, 0, 0);
    f2 = rsGetElementAt_float2(aFloat2, 0);
    rsSetElementAt_float2(aFloat2, f2, 0);
    f2 = rsGetElementAt_float2(aFloat2, 0, 0);
    rsSetElementAt_float2(aFloat2, f2, 0, 0);
    f2 = rsGetElementAt_float2(aFloat2, 0, 0, 0);
    rsSetElementAt_float2(aFloat2, f2, 0, 0, 0);
    f3 = rsGetElementAt_float3(aFloat3, 0);
    rsSetElementAt_float3(aFloat3, f3, 0);
    f3 = rsGetElementAt_float3(aFloat3, 0, 0);
    rsSetElementAt_float3(aFloat3, f3, 0, 0);
    f3 = rsGetElementAt_float3(aFloat3, 0, 0, 0);
    rsSetElementAt_float3(aFloat3, f3, 0, 0, 0);
    f4 = rsGetElementAt_float4(aFloat4, 0);
    rsSetElementAt_float4(aFloat4, f4, 0);
    f4 = rsGetElementAt_float4(aFloat4, 0, 0);
    rsSetElementAt_float4(aFloat4, f4, 0, 0);
    f4 = rsGetElementAt_float4(aFloat4, 0, 0, 0);
    rsSetElementAt_float4(aFloat4, f4, 0, 0, 0);

    d = rsGetElementAt_double(aDouble, 0);
    rsSetElementAt_double(aDouble, d, 0);
    d = rsGetElementAt_double(aDouble, 0, 0);
    rsSetElementAt_double(aDouble, d, 0, 0);
    d = rsGetElementAt_double(aDouble, 0, 0, 0);
    rsSetElementAt_double(aDouble, d, 0, 0, 0);
    d2 = rsGetElementAt_double2(aDouble2, 0);
    rsSetElementAt_double2(aDouble2, d2, 0);
    d2 = rsGetElementAt_double2(aDouble2, 0, 0);
    rsSetElementAt_double2(aDouble2, d2, 0, 0);
    d2 = rsGetElementAt_double2(aDouble2, 0, 0, 0);
    rsSetElementAt_double2(aDouble2, d2, 0, 0, 0);
    d3 = rsGetElementAt_double3(aDouble3, 0);
    rsSetElementAt_double3(aDouble3, d3, 0);
    d3 = rsGetElementAt_double3(aDouble3, 0, 0);
    rsSetElementAt_double3(aDouble3, d3, 0, 0);
    d3 = rsGetElementAt_double3(aDouble3, 0, 0, 0);
    rsSetElementAt_double3(aDouble3, d3, 0, 0, 0);
    d4 = rsGetElementAt_double4(aDouble4, 0);
    rsSetElementAt_double4(aDouble4, d4, 0);
    d4 = rsGetElementAt_double4(aDouble4, 0, 0);
    rsSetElementAt_double4(aDouble4, d4, 0, 0);
    d4 = rsGetElementAt_double4(aDouble4, 0, 0, 0);
    rsSetElementAt_double4(aDouble4, d4, 0, 0, 0);

    uc3.x = rsGetElementAtYuv_uchar_Y(aUChar4, 0, 0);
    uc3.y = rsGetElementAtYuv_uchar_U(aUChar4, 0, 0);
    uc3.z = rsGetElementAtYuv_uchar_V(aUChar4, 0, 0);

    c3.x = *(char*)rsGetElementAt(aChar3, 0);
    c3.y = *(char*)rsGetElementAt(aChar3, 0, 0);
    c3.z = *(char*)rsGetElementAt(aChar3, 0, 0, 0);

    rsAllocationIoSend(aInt);
    rsAllocationIoReceive(aInt);

    elemNonNull = rsAllocationGetElement(aInt);

    rsAllocationCopy1DRange(allocDst, 0, 0, 0, allocDst, 0, 0);
    rsAllocationCopy2DRange(allocDst, 0, 0, 0, 0, 0, 0, allocDst, 0, 0, 0, 0);

    // rsSample routines
    f4 += rsSample(allocDst, samplerNonNull, f);
    f4 += rsSample(allocDst, samplerNonNull, f, f);
    f4 += rsSample(allocDst, samplerNonNull, f2);
    f4 += rsSample(allocDst, samplerNonNull, f2, f);

    // rs_atomic.rsh
    rsAtomicInc(&i);
    rsAtomicDec(&i);
    rsAtomicAdd(&i, 1);
    rsAtomicSub(&i, 2);
    rsAtomicAnd(&i, 3);
    rsAtomicOr(&i, 4);
    rsAtomicXor(&i, 5);
    rsAtomicMin(&i, 6);
    rsAtomicMin(&ui, 6);
    rsAtomicMax(&i, 7);
    rsAtomicMax(&ui, 7);
    rsAtomicCas(&i, 8, 9);
    rsAtomicCas(&ui, 8, 9);

    // rs_cl.rsh
    c2 = convert_char2(c2);
    c2 = convert_char2(uc2);
    c2 = convert_char2(s2);
    c2 = convert_char2(us2);
    c2 = convert_char2(i2);
    c2 = convert_char2(ui2);
    c2 = convert_char2(f2);
    c3 = convert_char3(c3);
    c3 = convert_char3(uc3);
    c3 = convert_char3(s3);
    c3 = convert_char3(us3);
    c3 = convert_char3(i3);
    c3 = convert_char3(ui3);
    c3 = convert_char3(f3);
    c4 = convert_char4(c4);
    c4 = convert_char4(uc4);
    c4 = convert_char4(s4);
    c4 = convert_char4(us4);
    c4 = convert_char4(i4);
    c4 = convert_char4(ui4);
    c4 = convert_char4(f4);

    uc2 = convert_uchar2(c2);
    uc2 = convert_uchar2(uc2);
    uc2 = convert_uchar2(s2);
    uc2 = convert_uchar2(us2);
    uc2 = convert_uchar2(i2);
    uc2 = convert_uchar2(ui2);
    uc2 = convert_uchar2(f2);
    uc3 = convert_uchar3(c3);
    uc3 = convert_uchar3(uc3);
    uc3 = convert_uchar3(s3);
    uc3 = convert_uchar3(us3);
    uc3 = convert_uchar3(i3);
    uc3 = convert_uchar3(ui3);
    uc3 = convert_uchar3(f3);
    uc4 = convert_uchar4(c4);
    uc4 = convert_uchar4(uc4);
    uc4 = convert_uchar4(s4);
    uc4 = convert_uchar4(us4);
    uc4 = convert_uchar4(i4);
    uc4 = convert_uchar4(ui4);
    uc4 = convert_uchar4(f4);

    s2 = convert_short2(c2);
    s2 = convert_short2(uc2);
    s2 = convert_short2(s2);
    s2 = convert_short2(us2);
    s2 = convert_short2(i2);
    s2 = convert_short2(ui2);
    s2 = convert_short2(f2);
    s3 = convert_short3(c3);
    s3 = convert_short3(uc3);
    s3 = convert_short3(s3);
    s3 = convert_short3(us3);
    s3 = convert_short3(i3);
    s3 = convert_short3(ui3);
    s3 = convert_short3(f3);
    s4 = convert_short4(c4);
    s4 = convert_short4(uc4);
    s4 = convert_short4(s4);
    s4 = convert_short4(us4);
    s4 = convert_short4(i4);
    s4 = convert_short4(ui4);
    s4 = convert_short4(f4);

    us2 = convert_ushort2(c2);
    us2 = convert_ushort2(uc2);
    us2 = convert_ushort2(s2);
    us2 = convert_ushort2(us2);
    us2 = convert_ushort2(i2);
    us2 = convert_ushort2(ui2);
    us2 = convert_ushort2(f2);
    us3 = convert_ushort3(c3);
    us3 = convert_ushort3(uc3);
    us3 = convert_ushort3(s3);
    us3 = convert_ushort3(us3);
    us3 = convert_ushort3(i3);
    us3 = convert_ushort3(ui3);
    us3 = convert_ushort3(f3);
    us4 = convert_ushort4(c4);
    us4 = convert_ushort4(uc4);
    us4 = convert_ushort4(s4);
    us4 = convert_ushort4(us4);
    us4 = convert_ushort4(i4);
    us4 = convert_ushort4(ui4);
    us4 = convert_ushort4(f4);

    i2 = convert_int2(c2);
    i2 = convert_int2(uc2);
    i2 = convert_int2(s2);
    i2 = convert_int2(us2);
    i2 = convert_int2(i2);
    i2 = convert_int2(ui2);
    i2 = convert_int2(f2);
    i3 = convert_int3(c3);
    i3 = convert_int3(uc3);
    i3 = convert_int3(s3);
    i3 = convert_int3(us3);
    i3 = convert_int3(i3);
    i3 = convert_int3(ui3);
    i3 = convert_int3(f3);
    i4 = convert_int4(c4);
    i4 = convert_int4(uc4);
    i4 = convert_int4(s4);
    i4 = convert_int4(us4);
    i4 = convert_int4(i4);
    i4 = convert_int4(ui4);
    i4 = convert_int4(f4);

    ui2 = convert_uint2(c2);
    ui2 = convert_uint2(uc2);
    ui2 = convert_uint2(s2);
    ui2 = convert_uint2(us2);
    ui2 = convert_uint2(i2);
    ui2 = convert_uint2(ui2);
    ui2 = convert_uint2(f2);
    ui3 = convert_uint3(c3);
    ui3 = convert_uint3(uc3);
    ui3 = convert_uint3(s3);
    ui3 = convert_uint3(us3);
    ui3 = convert_uint3(i3);
    ui3 = convert_uint3(ui3);
    ui3 = convert_uint3(f3);
    ui4 = convert_uint4(c4);
    ui4 = convert_uint4(uc4);
    ui4 = convert_uint4(s4);
    ui4 = convert_uint4(us4);
    ui4 = convert_uint4(i4);
    ui4 = convert_uint4(ui4);
    ui4 = convert_uint4(f4);

    f2 = convert_float2(c2);
    f2 = convert_float2(uc2);
    f2 = convert_float2(s2);
    f2 = convert_float2(us2);
    f2 = convert_float2(i2);
    f2 = convert_float2(ui2);
    f2 = convert_float2(f2);
    f3 = convert_float3(c3);
    f3 = convert_float3(uc3);
    f3 = convert_float3(s3);
    f3 = convert_float3(us3);
    f3 = convert_float3(i3);
    f3 = convert_float3(ui3);
    f3 = convert_float3(f3);
    f4 = convert_float4(c4);
    f4 = convert_float4(uc4);
    f4 = convert_float4(s4);
    f4 = convert_float4(us4);
    f4 = convert_float4(i4);
    f4 = convert_float4(ui4);
    f4 = convert_float4(f4);

    // FIXME: No support for long/double (either from/to).

    // math
    f = acos(f);
    f2 = acos(f2);
    f3 = acos(f3);
    f4 = acos(f4);
    f = acosh(f);
    f2 = acosh(f2);
    f3 = acosh(f3);
    f4 = acosh(f4);
    f = acospi(f);
    f2 = acospi(f2);
    f3 = acospi(f3);
    f4 = acospi(f4);

    f = asin(f);
    f2 = asin(f2);
    f3 = asin(f3);
    f4 = asin(f4);
    f = asinh(f);
    f2 = asinh(f2);
    f3 = asinh(f3);
    f4 = asinh(f4);
    f = asinpi(f);
    f2 = asinpi(f2);
    f3 = asinpi(f3);
    f4 = asinpi(f4);

    f = atan(f);
    f2 = atan(f2);
    f3 = atan(f3);
    f4 = atan(f4);
    f = atanh(f);
    f2 = atanh(f2);
    f3 = atanh(f3);
    f4 = atanh(f4);
    f = atanpi(f);
    f2 = atanpi(f2);
    f3 = atanpi(f3);
    f4 = atanpi(f4);

    f = atan2(f, f);
    f2 = atan2(f2, f2);
    f3 = atan2(f3, f3);
    f4 = atan2(f4, f4);
    f = atan2pi(f, f);
    f2 = atan2pi(f2, f2);
    f3 = atan2pi(f3, f3);
    f4 = atan2(f4, f4);

    f = cbrt(f);
    f2 = cbrt(f2);
    f3 = cbrt(f3);
    f4 = cbrt(f4);

    f = ceil(f);
    f2 = ceil(f2);
    f3 = ceil(f3);
    f4 = ceil(f4);

    f = copysign(f, f);
    f2 = copysign(f2, f2);
    f3 = copysign(f3, f3);
    f4 = copysign(f4, f4);

    f = cos(f);
    f2 = cos(f2);
    f3 = cos(f3);
    f4 = cos(f4);
    f = cosh(f);
    f2 = cosh(f2);
    f3 = cosh(f3);
    f4 = cosh(f4);
    f = cospi(f);
    f2 = cospi(f2);
    f3 = cospi(f3);
    f4 = cospi(f4);

    f = erfc(f);
    f2 = erfc(f2);
    f3 = erfc(f3);
    f4 = erfc(f4);
    f = erf(f);
    f2 = erf(f2);
    f3 = erf(f3);
    f4 = erf(f4);

    f = exp(f);
    f2 = exp(f2);
    f3 = exp(f3);
    f4 = exp(f4);
    f = exp2(f);
    f2 = exp2(f2);
    f3 = exp2(f3);
    f4 = exp2(f4);

    f = pow(f, f);
    f2 = pow(f2, f2);
    f3 = pow(f3, f3);
    f4 = pow(f4, f4);

    f = exp10(f);
    f2 = exp10(f2);
    f3 = exp10(f3);
    f4 = exp10(f4);

    f = expm1(f);
    f2 = expm1(f2);
    f3 = expm1(f3);
    f4 = expm1(f4);

    f = fabs(f);
    f2 = fabs(f2);
    f3 = fabs(f3);
    f4 = fabs(f4);

    f = fabs(f);
    f2 = fabs(f2);
    f3 = fabs(f3);
    f4 = fabs(f4);

    f = fdim(f, f);
    f2 = fdim(f2, f2);
    f3 = fdim(f3, f3);
    f4 = fdim(f4, f4);

    f = floor(f);
    f2 = floor(f2);
    f3 = floor(f3);
    f4 = floor(f4);

    f = fma(f, f, f);
    f2 = fma(f2, f2, f2);
    f3 = fma(f3, f3, f3);
    f4 = fma(f4, f4, f4);

    f = fmax(f, f);
    f2 = fmax(f2, f2);
    f3 = fmax(f3, f3);
    f4 = fmax(f4, f4);

    f = fmin(f, f);
    f2 = fmin(f2, f2);
    f3 = fmin(f3, f3);
    f4 = fmin(f4, f4);

    f = fmod(f, f);
    f2 = fmod(f2, f2);
    f3 = fmod(f3, f3);
    f4 = fmod(f4, f4);

    f = fract(f, (float *)&f);
    f2 = fract(f2, (float2 *)&f2);
    f3 = fract(f3, (float3 *)&f3);
    f4 = fract(f4, (float4 *)&f4);
    f = fract(f);
    f2 = fract(f2);
    f3 = fract(f3);
    f4 = fract(f4);

    f = frexp(f, (int *)&i);
    f2 = frexp(f2, (int2 *)&i2);
    f3 = frexp(f3, (int3 *)&i3);
    f4 = frexp(f4, (int4 *)&i4);

    f = hypot(f, f);
    f2 = hypot(f2, f2);
    f3 = hypot(f3, f3);
    f4 = hypot(f4, f4);

    i = ilogb(f);
    i2 = ilogb(f2);
    i3 = ilogb(f3);
    i4 = ilogb(f4);

    f = ldexp(f, i);
    f2 = ldexp(f2, i2);
    f3 = ldexp(f3, i3);
    f4 = ldexp(f4, i4);
    f2 = ldexp(f2, i);
    f3 = ldexp(f3, i);
    f4 = ldexp(f4, i);

    f = lgamma(f);
    f2 = lgamma(f2);
    f3 = lgamma(f3);
    f4 = lgamma(f4);
    f = lgamma(f, (int *)&i);
    f2 = lgamma(f2, (int2 *)&i2);
    f3 = lgamma(f3, (int3 *)&i3);
    f4 = lgamma(f4, (int4 *)&i4);

    f = log(f);
    f2 = log(f2);
    f3 = log(f3);
    f4 = log(f4);

    f = log10(f);
    f2 = log10(f2);
    f3 = log10(f3);
    f4 = log10(f4);

    f = log2(f);
    f2 = log2(f2);
    f3 = log2(f3);
    f4 = log2(f4);

    f = log1p(f);
    f2 = log1p(f2);
    f3 = log1p(f3);
    f4 = log1p(f4);

    f = logb(f);
    f2 = logb(f2);
    f3 = logb(f3);
    f4 = logb(f4);

    f = mad(f, f, f);
    f2 = mad(f2, f2, f2);
    f3 = mad(f3, f3, f3);
    f4 = mad(f4, f4, f4);

    f = modf(f, (float *)&f);
    f2 = modf(f2, (float2 *)&f2);
    f3 = modf(f3, (float3 *)&f3);
    f4 = modf(f4, (float4 *)&f4);

    f = nan(ui);

    f = nextafter(f, f);
    f2 = nextafter(f2, f2);
    f3 = nextafter(f3, f3);
    f4 = nextafter(f4, f4);

    f = pown(f, i);
    f2 = pown(f2, i2);
    f3 = pown(f3, i3);
    f4 = pown(f4, i4);

    f = powr(f, f);
    f2 = powr(f2, f2);
    f3 = powr(f3, f3);
    f4 = powr(f4, f4);

    f = remainder(f, f);
    f2 = remainder(f2, f2);
    f3 = remainder(f3, f3);
    f4 = remainder(f4, f4);

    f = remquo(f, f, (int *)&i);
    f2 = remquo(f2, f2, (int2 *)&i2);
    f3 = remquo(f3, f3, (int3 *)&i3);
    f4 = remquo(f4, f4, (int4 *)&i4);

    f = rint(f);
    f2 = rint(f2);
    f3 = rint(f3);
    f4 = rint(f4);

    f = rootn(f, i);
    f2 = rootn(f2, i2);
    f3 = rootn(f3, i3);
    f4 = rootn(f4, i4);

    f = round(f);
    f2 = round(f2);
    f3 = round(f3);
    f4 = round(f4);

    f = rsqrt(f);
    f2 = rsqrt(f2);
    f3 = rsqrt(f3);
    f4 = rsqrt(f4);

    f = sin(f);
    f2 = sin(f2);
    f3 = sin(f3);
    f4 = sin(f4);
    f = sinh(f);
    f2 = sinh(f2);
    f3 = sinh(f3);
    f4 = sinh(f4);
    f = sinpi(f);
    f2 = sinpi(f2);
    f3 = sinpi(f3);
    f4 = sinpi(f4);

    f = sincos(f, (float *)&f);
    f2 = sincos(f2, (float2 *)&f2);
    f3 = sincos(f3, (float3 *)&f3);
    f4 = sincos(f4, (float4 *)&f4);

    f = tan(f);
    f2 = tan(f2);
    f3 = tan(f3);
    f4 = tan(f4);
    f = tanh(f);
    f2 = tanh(f2);
    f3 = tanh(f3);
    f4 = tanh(f4);
    f = tanpi(f);
    f2 = tanpi(f2);
    f3 = tanpi(f3);
    f4 = tanpi(f4);

    f = tgamma(f);
    f2 = tgamma(f2);
    f3 = tgamma(f3);
    f4 = tgamma(f4);

    f = trunc(f);
    f2 = trunc(f2);
    f3 = trunc(f3);
    f4 = trunc(f4);

    uc = abs(c);
    uc2 = abs(c2);
    uc3 = abs(c3);
    uc4 = abs(c4);
    us = abs(s);
    us2 = abs(s2);
    us3 = abs(s3);
    us4 = abs(s4);
    ui = abs(i);
    ui2 = abs(i2);
    ui3 = abs(i3);
    ui4 = abs(i4);

    c = clz(c);
    c2 = clz(c2);
    c3 = clz(c3);
    c4 = clz(c4);
    uc = clz(uc);
    uc2 = clz(uc2);
    uc3 = clz(uc3);
    uc4 = clz(uc4);
    s = clz(s);
    s2 = clz(s2);
    s3 = clz(s3);
    s4 = clz(s4);
    us = clz(us);
    us2 = clz(us2);
    us3 = clz(us3);
    us4 = clz(us4);
    i = clz(i);
    i2 = clz(i2);
    i3 = clz(i3);
    i4 = clz(i4);
    ui = clz(ui);
    ui2 = clz(ui2);
    ui3 = clz(ui3);
    ui4 = clz(ui4);

    c = min(c, c);
    c2 = min(c2, c2);
    c3 = min(c3, c3);
    c4 = min(c4, c4);
    uc = min(uc, uc);
    uc2 = min(uc2, uc2);
    uc3 = min(uc3, uc3);
    uc4 = min(uc4, uc4);
    s = min(s, s);
    s2 = min(s2, s2);
    s3 = min(s3, s3);
    s4 = min(s4, s4);
    us = min(us, us);
    us2 = min(us2, us2);
    us3 = min(us3, us3);
    us4 = min(us4, us4);
    i = min(i, i);
    i2 = min(i2, i2);
    i3 = min(i3, i3);
    i4 = min(i4, i4);
    ui = min(ui, ui);
    ui2 = min(ui2, ui2);
    ui3 = min(ui3, ui3);
    ui4 = min(ui4, ui4);
    f = min(f, f);
    f2 = min(f2, f2);
    f3 = min(f3, f3);
    f4 = min(f4, f4);
    f2 = min(f2, f);
    f3 = min(f3, f);
    f4 = min(f4, f);

    c = max(c, c);
    c2 = max(c2, c2);
    c3 = max(c3, c3);
    c4 = max(c4, c4);
    uc = max(uc, uc);
    uc2 = max(uc2, uc2);
    uc3 = max(uc3, uc3);
    uc4 = max(uc4, uc4);
    s = max(s, s);
    s2 = max(s2, s2);
    s3 = max(s3, s3);
    s4 = max(s4, s4);
    us = max(us, us);
    us2 = max(us2, us2);
    us3 = max(us3, us3);
    us4 = max(us4, us4);
    i = max(i, i);
    i2 = max(i2, i2);
    i3 = max(i3, i3);
    i4 = max(i4, i4);
    ui = max(ui, ui);
    ui2 = max(ui2, ui2);
    ui3 = max(ui3, ui3);
    ui4 = max(ui4, ui4);
    f = max(f, f);
    f2 = max(f2, f2);
    f3 = max(f3, f3);
    f4 = max(f4, f4);
    f2 = max(f2, f);
    f3 = max(f3, f);
    f4 = max(f4, f);

    f = clamp(f, f, f);
    f2 = clamp(f2, f2, f2);
    f3 = clamp(f3, f3, f3);
    f4 = clamp(f4, f4, f4);
    f2 = clamp(f2, f, f);
    f3 = clamp(f3, f, f);
    f4 = clamp(f4, f, f);
    // FIXME: other clamps only in 19+

    f = degrees(f);
    f2 = degrees(f2);
    f3 = degrees(f3);
    f4 = degrees(f4);

    f = mix(f, f, f);
    f2 = mix(f2, f2, f2);
    f3 = mix(f3, f3, f3);
    f4 = mix(f4, f4, f4);
    f2 = mix(f2, f2, f);
    f3 = mix(f3, f3, f);
    f4 = mix(f4, f4, f);

    f = radians(f);
    f2 = radians(f2);
    f3 = radians(f3);
    f4 = radians(f4);

    f = step(f, f);
    f2 = step(f2, f2);
    f3 = step(f3, f3);
    f4 = step(f4, f4);
    f2 = step(f2, f);
    f3 = step(f3, f);
    f4 = step(f4, f);

    f = sign(f);
    f2 = sign(f2);
    f3 = sign(f3);
    f4 = sign(f4);

    f3 = cross(f3, f3);
    f4 = cross(f4, f4);

    f = dot(f, f);
    f = dot(f2, f2);
    f = dot(f3, f3);
    f = dot(f4, f4);

    f = length(f);
    f = length(f2);
    f = length(f3);
    f = length(f4);

    f = distance(f, f);
    f = distance(f2, f2);
    f = distance(f3, f3);
    f = distance(f4, f4);

    f = normalize(f);
    f2 = normalize(f2);
    f3 = normalize(f3);
    f4 = normalize(f4);

    f = half_recip(f);
    f2 = half_recip(f2);
    f3 = half_recip(f3);
    f4 = half_recip(f4);

    f = half_sqrt(f);
    f2 = half_sqrt(f2);
    f3 = half_sqrt(f3);
    f4 = half_sqrt(f4);

    f = half_rsqrt(f);
    f2 = half_rsqrt(f2);
    f3 = half_rsqrt(f3);
    f4 = half_rsqrt(f4);

    f = fast_length(f);
    f = fast_length(f2);
    f = fast_length(f3);
    f = fast_length(f4);

    f = fast_distance(f, f);
    f = fast_distance(f2, f2);
    f = fast_distance(f3, f3);
    f = fast_distance(f4, f4);

    f = fast_normalize(f);
    f2 = fast_normalize(f2);
    f3 = fast_normalize(f3);
    f4 = fast_normalize(f4);

    f = native_exp2(f);
    f2 = native_exp2(f2);
    f3 = native_exp2(f3);
    f4 = native_exp2(f4);

    f = native_exp(f);
    f2 = native_exp(f2);
    f3 = native_exp(f3);
    f4 = native_exp(f4);

    f = native_exp10(f);
    f2 = native_exp10(f2);
    f3 = native_exp10(f3);
    f4 = native_exp10(f4);

    f = native_log2(f);
    f2 = native_log2(f2);
    f3 = native_log2(f3);
    f4 = native_log2(f4);

    f = native_log(f);
    f2 = native_log(f2);
    f3 = native_log(f3);
    f4 = native_log(f4);

    f = native_log10(f);
    f2 = native_log10(f2);
    f3 = native_log10(f3);
    f4 = native_log10(f4);

    f = native_powr(f, f);
    f2 = native_powr(f2, f2);
    f3 = native_powr(f3, f3);
    f4 = native_powr(f4, f4);

    // rs_core.rsh
    b = rsSendToClient(0);
    b = rsSendToClient(0, NULL, 0);
    rsSendToClientBlocking(0);
    rsSendToClientBlocking(0, NULL, 0);

    rs_script_call_t sc;
    rsForEach(scriptNonNull, allocNonNull, allocNonNull, NULL, 0, &sc);
    rsForEach(scriptNonNull, allocNonNull, allocNonNull, NULL, 0);
    rsForEach(scriptNonNull, allocNonNull, allocNonNull);

    // rs_debug.rsh
    rsDebug("", f);
    rsDebug("", f, f);
    rsDebug("", f, f, f);
    rsDebug("", f, f, f, f);
    rsDebug("", f2);
    rsDebug("", f3);
    rsDebug("", f4);
    rsDebug("", d);
    rsDebug("", &m4x4);
    rsDebug("", &m3x3);
    rsDebug("", &m2x2);
    rsDebug("", i);
    rsDebug("", ui);
    rsDebug("", l);
    rsDebug("", ul);
    rsDebug("", ll);
    rsDebug("", ull);
    rsDebug("", (const void *)&i);
    rsDebug("", c);
    rsDebug("", c2);
    rsDebug("", c3);
    rsDebug("", c4);
    rsDebug("", uc);
    rsDebug("", uc2);
    rsDebug("", uc3);
    rsDebug("", uc4);
    rsDebug("", s);
    rsDebug("", s2);
    rsDebug("", s3);
    rsDebug("", s4);
    rsDebug("", us);
    rsDebug("", us2);
    rsDebug("", us3);
    rsDebug("", us4);
    rsDebug("", i2);
    rsDebug("", i3);
    rsDebug("", i4);
    rsDebug("", ui2);
    rsDebug("", ui3);
    rsDebug("", ui4);
    rsDebug("", l2);
    rsDebug("", l3);
    rsDebug("", l4);
    rsDebug("", ul2);
    rsDebug("", ul3);
    rsDebug("", ul4);

    // rs_element.rsh
    ui = rsElementGetSubElementCount(elemNonNull);
    elemNull = rsElementGetSubElement(elemNonNull, 0);
    ui = rsElementGetSubElementNameLength(elemNonNull, 0);
    ui = rsElementGetSubElementName(elemNonNull, 0, NULL, 0);
    ui = rsElementGetSubElementArraySize(elemNonNull, 0);
    ui = rsElementGetSubElementOffsetBytes(elemNonNull, 0);
    ui = rsElementGetBytesSize(elemNonNull);

    dt = rsElementGetDataType(elemNonNull);
    dk = rsElementGetDataKind(elemNonNull);

    ui = rsElementGetVectorSize(elemNonNull);

    // rs_math.rsh
    i = rsRand(i);
    i = rsRand(i, i);
    f = rsRand(f);
    f = rsFrac(f);
    ui = rsClamp(ui, ui, ui);
    i = rsClamp(i, i, i);
    us = rsClamp(us, us, us);
    s = rsClamp(s, s, s);
    uc = rsClamp(uc, uc, uc);
    c = rsClamp(c, c, c);

    // skip always-inlined rsExtractFrustumPlanes
    // skip always-inlined rsIsSphereInFrustum

    uc4 = rsPackColorTo8888(f, f, f);
    uc4 = rsPackColorTo8888(f, f, f, f);
    uc4 = rsPackColorTo8888(f3);
    uc4 = rsPackColorTo8888(f4);
    f4 = rsUnpackColor8888(uc4);
    uc4 = rsYuvToRGBA_uchar4(uc, uc, uc);
    f4 = rsYuvToRGBA_float4(uc, uc, uc);

    // rs_matrix.rsh
    rsMatrixSet(&m4x4, ui, ui, f);
    rsMatrixSet(&m3x3, ui, ui, f);
    rsMatrixSet(&m2x2, ui, ui, f);
    f = rsMatrixGet(&m4x4, ui, ui);
    f = rsMatrixGet(&m3x3, ui, ui);
    f = rsMatrixGet(&m2x2, ui, ui);
    rsMatrixLoadIdentity(&m4x4);
    rsMatrixLoadIdentity(&m3x3);
    rsMatrixLoadIdentity(&m2x2);
    rsMatrixLoad(&m4x4, fa);
    rsMatrixLoad(&m3x3, fa);
    rsMatrixLoad(&m2x2, fa);
    rsMatrixLoad(&m4x4, &m4x4);
    rsMatrixLoad(&m4x4, &m3x3);
    rsMatrixLoad(&m4x4, &m2x2);
    rsMatrixLoad(&m3x3, &m3x3);
    rsMatrixLoad(&m2x2, &m2x2);
    rsMatrixLoadRotate(&m4x4, f, f, f, f);
    rsMatrixLoadScale(&m4x4, f, f, f);
    rsMatrixLoadTranslate(&m4x4, f, f, f);
    rsMatrixLoadMultiply(&m4x4, &m4x4, &m4x4);
    rsMatrixLoadMultiply(&m3x3, &m3x3, &m3x3);
    rsMatrixLoadMultiply(&m2x2, &m2x2, &m2x2);
    rsMatrixMultiply(&m4x4, &m4x4);
    rsMatrixMultiply(&m3x3, &m3x3);
    rsMatrixMultiply(&m2x2, &m2x2);
    rsMatrixRotate(&m4x4, f, f, f, f);
    rsMatrixScale(&m4x4, f, f, f);
    rsMatrixTranslate(&m4x4, f, f, f);
    rsMatrixLoadOrtho(&m4x4, f, f, f, f, f, f);
    rsMatrixLoadFrustum(&m4x4, f, f, f, f, f, f);
    rsMatrixLoadPerspective(&m4x4, f, f, f, f);
    f4 = rsMatrixMultiply(&m4x4, f4);
    f4 = rsMatrixMultiply(&m4x4, f3);
    f4 = rsMatrixMultiply(&m4x4, f2);
    f3 = rsMatrixMultiply(&m3x3, f3);
    f3 = rsMatrixMultiply(&m3x3, f2);
    f2 = rsMatrixMultiply(&m2x2, f2);
    b = rsMatrixInverse(&m4x4);
    b = rsMatrixInverseTranspose(&m4x4);
    rsMatrixTranspose(&m4x4);
    rsMatrixTranspose(&m3x3);
    rsMatrixTranspose(&m2x2);

    // rs_object.rsh
    rsSetObject(&elemNonNull, elemNonNull);
    rsSetObject(&typeNonNull, typeNonNull);
    rsSetObject(&allocNonNull, allocNonNull);
    rsSetObject(&samplerNonNull, samplerNonNull);
    rsSetObject(&scriptNonNull, scriptNonNull);
    rsClearObject(&elemNonNull);
    rsClearObject(&typeNonNull);
    rsClearObject(&allocNonNull);
    rsClearObject(&samplerNonNull);
    rsClearObject(&scriptNonNull);
    b = rsIsObject(elemNonNull);
    b = rsIsObject(typeNonNull);
    b = rsIsObject(allocNonNull);
    b = rsIsObject(samplerNonNull);
    b = rsIsObject(scriptNonNull);

    // rs_quaternion.rsh skipped because all functions are static.

    // rs_sampler.rsh
    rsv = rsSamplerGetMinification(samplerNonNull);
    rsv = rsSamplerGetMagnification(samplerNonNull);
    rsv = rsSamplerGetWrapS(samplerNonNull);
    rsv = rsSamplerGetWrapT(samplerNonNull);
    f = rsSamplerGetAnisotropy(samplerNonNull);

    // rs_time.rsh
    rst = rsTime(NULL);
    rsLocaltime((rs_tm *) &rstm, (rs_time_t *) &rst);
    ll = rsUptimeMillis();
    ll = rsUptimeNanos();
    f = rsGetDt();

    /********************************
     * DO NOT EXECUTE THIS FUNCTION *
     ********************************/
}

static bool test_obj_api() {
    bool failed = false;

    _RS_ASSERT(!rsIsObject(elemNull));
    _RS_ASSERT(rsIsObject(elemNonNull));
    rsSetObject(&elemNull, elemNonNull);
    _RS_ASSERT(rsIsObject(elemNull));
    rsClearObject(&elemNull);
    _RS_ASSERT(!rsIsObject(elemNull));

    _RS_ASSERT(!rsIsObject(typeNull));
    _RS_ASSERT(rsIsObject(typeNonNull));
    rsSetObject(&typeNull, typeNonNull);
    _RS_ASSERT(rsIsObject(typeNull));
    rsClearObject(&typeNull);
    _RS_ASSERT(!rsIsObject(typeNull));

    _RS_ASSERT(!rsIsObject(allocNull));
    _RS_ASSERT(rsIsObject(allocNonNull));
    rsSetObject(&allocNull, allocNonNull);
    _RS_ASSERT(rsIsObject(allocNull));
    rsClearObject(&allocNull);
    _RS_ASSERT(!rsIsObject(allocNull));

    _RS_ASSERT(!rsIsObject(samplerNull));
    _RS_ASSERT(rsIsObject(samplerNonNull));
    rsSetObject(&samplerNull, samplerNonNull);
    _RS_ASSERT(rsIsObject(samplerNull));
    rsClearObject(&samplerNull);
    _RS_ASSERT(!rsIsObject(samplerNull));

    _RS_ASSERT(!rsIsObject(scriptNull));
    _RS_ASSERT(rsIsObject(scriptNonNull));
    rsSetObject(&scriptNull, scriptNonNull);
    _RS_ASSERT(rsIsObject(scriptNull));
    rsClearObject(&scriptNull);
    _RS_ASSERT(!rsIsObject(scriptNull));

    if (failed) {
        rsDebug("test_obj_api FAILED", -1);
    }
    else {
        rsDebug("test_obj_api PASSED", 0);
    }

    return failed;
}


static bool test_rs_alloc_api() {
    bool failed = false;
    rs_allocation a = rsGetAllocation(allocPtr);
    _RS_ASSERT(rsIsObject(a));
    _RS_ASSERT(rsAllocationGetDimX(a) == x);
    _RS_ASSERT(rsAllocationGetDimY(a) == y);
    _RS_ASSERT(rsAllocationGetDimZ(a) == z);
    _RS_ASSERT(rsAllocationGetDimLOD(a) == 0);
    _RS_ASSERT(rsAllocationGetDimFaces(a) == 0);

    rsSetElementAt_char(a, 5, 1, 0);
    rsAllocationCopy1DRange(allocDst, 0, 0, x, a, 0, 0);
    _RS_ASSERT(rsGetElementAt_char(allocDst, 1, 0) == 5);

    if (failed) {
        rsDebug("test_obj_api FAILED", -1);
    }
    else {
        rsDebug("test_obj_api PASSED", 0);
    }

    return failed;
}


void api_test() {
    bool failed = false;
    failed |= test_obj_api();
    failed |= test_rs_alloc_api();

    if (failed) {
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}

