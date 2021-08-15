/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "shared.rsh"

int gDimX;
int gDimY;
int gDimZ;
int gStart;
static bool failed = false;


// For each type, define 4 kernels, one per vector variant, that walk an
// allocation and validate each cell.  The value in a cell must be gStart +
// "index-of-the-cell-starting-from-zero".  For vector types, the 'x' field
// must have this value.  The expected values for 'y', 'z' and 'w' follow the
// 'x' value in increments of one.
//
// 'z' will be zero for 2D and 1D allocations.  'y' will be zero for 1D
// allocations.

// TODO When the requirement that kernels must return an output to be launched
// using rsForEach is relaxed, make the kernel not return its input.
#define VERIFY_KERNEL(CT)                                                      \
    CT RS_KERNEL verify_##CT(CT in, int x, int y, int z) {                     \
        int val = (gStart + x + y * gDimX + z * gDimY * gDimX);                \
        _RS_ASSERT_EQU(in, (CT) val);                                          \
        return in;                                                             \
    }                                                                          \
    CT##2 RS_KERNEL verify_##CT##2(CT##2 in, int x, int y, int z) {            \
        int val = (gStart + x + y * gDimX + z * gDimY * gDimX);                \
        _RS_ASSERT_EQU(in.x, (CT) val);                                        \
        _RS_ASSERT_EQU(in.y, (CT) (val + 1));                                  \
        return in;                                                             \
    }                                                                          \
    CT##3 RS_KERNEL verify_##CT##3(CT##3 in, int x, int y, int z) {            \
        int val = (gStart + x + y * gDimX + z * gDimY * gDimX);                \
        _RS_ASSERT_EQU(in.x, (CT) val);                                        \
        _RS_ASSERT_EQU(in.y, (CT) (val + 1));                                  \
        _RS_ASSERT_EQU(in.z, (CT) (val + 2));                                  \
        return in;                                                             \
    }                                                                          \
    CT##4 RS_KERNEL verify_##CT##4(CT##4 in, int x, int y, int z) {            \
        int val = (gStart + x + y * gDimX + z * gDimY * gDimX);                \
        _RS_ASSERT_EQU(in.x, (CT) val);                                        \
        _RS_ASSERT_EQU(in.y, (CT) (val + 1));                                  \
        _RS_ASSERT_EQU(in.z, (CT) (val + 2));                                  \
        _RS_ASSERT_EQU(in.w, (CT) (val + 3));                                  \
        return in;                                                             \
    }                                                                          \

#ifndef RSTEST_COMPAT
VERIFY_KERNEL(half)
#endif
VERIFY_KERNEL(float)
VERIFY_KERNEL(double)
VERIFY_KERNEL(char)
VERIFY_KERNEL(short)
VERIFY_KERNEL(int)
VERIFY_KERNEL(long)
VERIFY_KERNEL(uchar)
VERIFY_KERNEL(ushort)
VERIFY_KERNEL(uint)
VERIFY_KERNEL(ulong)


// Store to an allocation based on the vector size and dimensionality being
// tested.  SETLEMENTAT, STORE_TO_ALLOC capture the following variables from
// the context where they get instantiated:
//     vecSize, gAlloc, val, x, y, z

#define SETELEMENTAT(CT)                                                      \
    if (gDimZ != 0) {                                                         \
        rsSetElementAt_##CT(gAlloc, storeVal, x, y, z);                       \
    }                                                                         \
    else if (gDimY != 0) {                                                    \
        rsSetElementAt_##CT(gAlloc, storeVal, x, y);                          \
    }                                                                         \
    else {                                                                    \
        rsSetElementAt_##CT(gAlloc, storeVal, x);                             \
    }

#define STORE_TO_ALLOC(RST, CT)                                               \
    case RST:                                                                 \
        switch (vecSize) {                                                    \
            case 1: {                                                         \
                CT storeVal = (CT) val;                                       \
                SETELEMENTAT(CT);                                             \
                     }                                                        \
                break;                                                        \
            case 2: {                                                         \
                CT##2 storeVal = {(CT) val, (CT) (val + 1)};                  \
                SETELEMENTAT(CT##2);                                          \
                    }                                                         \
                break;                                                        \
            case 3: {                                                         \
                CT##3 storeVal = {(CT) val, (CT) (val + 1), (CT) (val + 2)};  \
                SETELEMENTAT(CT##3);                                          \
                    }                                                         \
                break;                                                        \
            case 4: {                                                         \
                CT##4 storeVal = {(CT) val, (CT) (val + 1), (CT) (val + 2),   \
                                  (CT) (val + 3)};                            \
                SETELEMENTAT(CT##4);                                          \
                    }                                                         \
                break;                                                        \
        }                                                                     \
        break;                                                                \


// Launch the verify_kernel based on the appropriate type and vector size.
#define LAUNCH_VERIFY_KERNEL(RST, CT)                                         \
    case RST:                                                                 \
        if (vecSize == 1) rsForEach(verify_##CT, gAlloc, gAlloc);             \
        else if (vecSize == 2) rsForEach(verify_##CT##2, gAlloc, gAlloc);     \
        else if (vecSize == 3) rsForEach(verify_##CT##3, gAlloc, gAlloc);     \
        else if (vecSize == 4) rsForEach(verify_##CT##4, gAlloc, gAlloc);     \
        break;

void CreateAndTestAlloc(int dataType, int vecSize) {
    if (gDimZ != 0 && gDimY == 0) {
        _RS_ASSERT(false); // Invalid test
        return;
    }
    if (gDimX == 0) {
        _RS_ASSERT(false); // Invalid test
        return;
    }

    rs_data_type dt = (rs_data_type) dataType;

    rs_element element;
    rs_type type;
    rs_allocation gAlloc;

    // Create and validate the rs_element object
    if (vecSize == 1)
        element = rsCreateElement((rs_data_type) dt);
    else
        element = rsCreateVectorElement((rs_data_type) dt, vecSize);
    _RS_ASSERT(rsIsObject(element));
    if (!rsIsObject(element))
        return;

    // Create and validate the rs_type object
    type = rsCreateType(element, gDimX, gDimY, gDimZ);
    _RS_ASSERT(rsIsObject(type));
    if (!rsIsObject(type))
        return;

    // Create and validate the rs_allocation object
    gAlloc = rsCreateAllocation(type);
    if (!rsIsObject(gAlloc))
        return;

    // Handle RenderScript's distinction between Y or Z dimension being absent
    // and having a size of 1
    int zEnd = (gDimZ != 0) ? gDimZ: 1;
    int yEnd = (gDimY != 0) ? gDimY: 1;
    for (int z = 0; z < zEnd; z ++) {
        for (int y = 0; y < yEnd; y ++) {
            for (int x = 0; x < gDimX; x ++) {
                int val = gStart + (x + y * gDimX + z * gDimY * gDimX);

                // Store to a cell based on the type, vector size and
                // dimensionality
                switch (dt) {
#ifndef RSTEST_COMPAT
                    STORE_TO_ALLOC(RS_TYPE_FLOAT_16, half);
#else
                    // support lib doesn't support f16, skip
                    case RS_TYPE_FLOAT_16: break;
#endif
                    STORE_TO_ALLOC(RS_TYPE_FLOAT_32, float);
                    STORE_TO_ALLOC(RS_TYPE_FLOAT_64, double);
                    STORE_TO_ALLOC(RS_TYPE_SIGNED_8, char);
                    STORE_TO_ALLOC(RS_TYPE_SIGNED_16, short);
                    STORE_TO_ALLOC(RS_TYPE_SIGNED_32, int);
                    STORE_TO_ALLOC(RS_TYPE_SIGNED_64, long);
                    STORE_TO_ALLOC(RS_TYPE_UNSIGNED_8, uchar);
                    STORE_TO_ALLOC(RS_TYPE_UNSIGNED_16, ushort);
                    STORE_TO_ALLOC(RS_TYPE_UNSIGNED_32, uint);
                    STORE_TO_ALLOC(RS_TYPE_UNSIGNED_64, ulong);
                    default:
                        // Invalid test
                        _RS_ASSERT(false);
                        break;
                }
            }
        }
    }

    // Launch the appropriate verify_ kernel
    switch (dt) {
#ifndef RSTEST_COMPAT
        LAUNCH_VERIFY_KERNEL(RS_TYPE_FLOAT_16, half);
#else
        // support lib doesn't support f16, skip
        case RS_TYPE_FLOAT_16: break;
#endif
        LAUNCH_VERIFY_KERNEL(RS_TYPE_FLOAT_32, float);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_FLOAT_64, double);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_SIGNED_8, char);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_SIGNED_16, short);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_SIGNED_32, int);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_SIGNED_64, long);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_UNSIGNED_8, uchar);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_UNSIGNED_16, ushort);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_UNSIGNED_32, uint);
        LAUNCH_VERIFY_KERNEL(RS_TYPE_UNSIGNED_64, ulong);

        default:
            // Invalid test
            _RS_ASSERT(false);
            break;
    }
}

#define TEST_DATA_TYPE(dt, allowSimple, allowVector, allowPixel) {             \
    if (allowSimple)                                                           \
        _RS_ASSERT(rsIsObject(rsCreateElement(dt)));                           \
    else                                                                       \
        _RS_ASSERT(!rsIsObject(rsCreateElement(dt)));                          \
    if (allowVector)                                                           \
        _RS_ASSERT(rsIsObject(rsCreateVectorElement(dt, 3)));                  \
    else                                                                       \
        _RS_ASSERT(!rsIsObject(rsCreateVectorElement(dt, 3)));                 \
    if (allowPixel)                                                            \
        _RS_ASSERT(rsIsObject(rsCreatePixelElement(dt, RS_KIND_PIXEL_DEPTH))); \
    else                                                                       \
        _RS_ASSERT(!rsIsObject(rsCreatePixelElement(dt, RS_KIND_PIXEL_DEPTH)));\
}

#define TEST_SUPPORTED_PIXEL(dt, dk) {                                         \
    _RS_ASSERT(rsIsObject(rsCreatePixelElement(dt, dk)));                      \
}

#define TEST_UNSUPPORTED_PIXEL(dt, dk) {                                       \
    _RS_ASSERT(!rsIsObject(rsCreatePixelElement(dt, dk)));                     \
}

#define TEST_HELPER(suffix) {                                     \
    _RS_ASSERT(rsIsObject(rsCreateAllocation_##suffix(3)));       \
    _RS_ASSERT(rsIsObject(rsCreateAllocation_##suffix(3, 4)));    \
    _RS_ASSERT(rsIsObject(rsCreateAllocation_##suffix(3, 4, 5))); \
    }

#define TEST_HELPERS(CT) { \
    TEST_HELPER(CT);       \
    TEST_HELPER(CT##2);    \
    TEST_HELPER(CT##3);    \
    TEST_HELPER(CT##4);    \
}

void TestAllCases() {
    // vector_width must be at least 2
    rs_element oneElt = rsCreateVectorElement(RS_TYPE_SIGNED_32, 1);
    _RS_ASSERT(!rsIsObject(oneElt));

    // vector_width must be at most 4
    rs_element fiveElt = rsCreateVectorElement(RS_TYPE_SIGNED_32, 5);
    _RS_ASSERT(!rsIsObject(fiveElt));

    /////////////////////////////////////////////////////////////////
    // Element validation
    /////////////////////////////////////////////////////////////////
    // These types are valid for scalar and vectors, but don't support pixel
    TEST_DATA_TYPE(RS_TYPE_BOOLEAN,     true, true, false);
    TEST_DATA_TYPE(RS_TYPE_FLOAT_32,    true, true, false);
    TEST_DATA_TYPE(RS_TYPE_FLOAT_64,    true, true, false);
    TEST_DATA_TYPE(RS_TYPE_SIGNED_8,    true, true, false);
    TEST_DATA_TYPE(RS_TYPE_SIGNED_16,   true, true, false);
    TEST_DATA_TYPE(RS_TYPE_SIGNED_32,   true, true, false);
    TEST_DATA_TYPE(RS_TYPE_SIGNED_64,   true, true, false);
    TEST_DATA_TYPE(RS_TYPE_UNSIGNED_32, true, true, false);
    TEST_DATA_TYPE(RS_TYPE_UNSIGNED_64, true, true, false);

    // These types are valid only for scalars
    TEST_DATA_TYPE(RS_TYPE_MATRIX_4X4, true, false, false);
    TEST_DATA_TYPE(RS_TYPE_MATRIX_3X3, true, false, false);
    TEST_DATA_TYPE(RS_TYPE_MATRIX_2X2, true, false, false);
    TEST_DATA_TYPE(RS_TYPE_ELEMENT,    true, false, false);
    TEST_DATA_TYPE(RS_TYPE_TYPE,       true, false, false);
    TEST_DATA_TYPE(RS_TYPE_ALLOCATION, true, false, false);
    TEST_DATA_TYPE(RS_TYPE_SCRIPT,     true, false, false);

    // U8, U16 are valid for scalar, vector and pixel
    TEST_DATA_TYPE(RS_TYPE_UNSIGNED_8,  true, true, true);
    TEST_DATA_TYPE(RS_TYPE_UNSIGNED_16, true, true, true);

    // These data types are only for pixels and a particular data_kind
    TEST_SUPPORTED_PIXEL  (RS_TYPE_UNSIGNED_5_6_5,   RS_KIND_PIXEL_RGB);
    TEST_UNSUPPORTED_PIXEL(RS_TYPE_UNSIGNED_5_6_5,   RS_KIND_PIXEL_L);
    TEST_SUPPORTED_PIXEL  (RS_TYPE_UNSIGNED_5_5_5_1, RS_KIND_PIXEL_RGBA);
    TEST_UNSUPPORTED_PIXEL(RS_TYPE_UNSIGNED_5_5_5_1, RS_KIND_PIXEL_L);
    TEST_SUPPORTED_PIXEL  (RS_TYPE_UNSIGNED_4_4_4_4, RS_KIND_PIXEL_RGBA);
    TEST_UNSUPPORTED_PIXEL(RS_TYPE_UNSIGNED_4_4_4_4, RS_KIND_PIXEL_L);
    TEST_SUPPORTED_PIXEL  (RS_TYPE_UNSIGNED_16,      RS_KIND_PIXEL_DEPTH);
    TEST_UNSUPPORTED_PIXEL(RS_TYPE_UNSIGNED_4_4_4_4, RS_KIND_PIXEL_L);

    // These data types are not supported from single-source
    TEST_DATA_TYPE(RS_TYPE_NONE,             false, false, false);
    TEST_DATA_TYPE(RS_TYPE_SAMPLER,          false, false, false);
    TEST_DATA_TYPE(RS_TYPE_MESH,             false, false, false);
    TEST_DATA_TYPE(RS_TYPE_PROGRAM_FRAGMENT, false, false, false);
    TEST_DATA_TYPE(RS_TYPE_PROGRAM_VERTEX,   false, false, false);
    TEST_DATA_TYPE(RS_TYPE_PROGRAM_RASTER,   false, false, false);
    TEST_DATA_TYPE(RS_TYPE_PROGRAM_STORE,    false, false, false);
    TEST_DATA_TYPE(RS_TYPE_FONT,             false, false, false);
    TEST_DATA_TYPE(RS_TYPE_INVALID,          false, false, false);

    /////////////////////////////////////////////////////////////////
    // Test rs_type creation
    /////////////////////////////////////////////////////////////////
    rs_element I32_3 = rsCreateVectorElement(RS_TYPE_SIGNED_32, 3);

    // Create 1D, 2D, 3D types
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 3)));
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 3, 4)));
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 3, 4, 5)));
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 0))); // x = 0 is allowed

    // Invalid dimensionality
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 0, 4))); // x is 0 but y isn't
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 0, 4, 5))); // x is 0 but z isn't
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 0, 5))); // y is 0 but z isn't

    // shape attributes
    // Valid yuv_format
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 3, 4, 0, false, false,
                    RS_YUV_NONE)));
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 3, 4, 0, false, false,
                    RS_YUV_YV12)));
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 3, 4, 0, false, false,
                    RS_YUV_NV21)));
    _RS_ASSERT(rsIsObject(rsCreateType(I32_3, 3, 4, 0, false, false,
                    RS_YUV_420_888)));

    // Invalid yuv_format
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 4, 5, false, false, 1024)));
    // yuv_format with 1D or 3D is invalid
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 0, 0, false, false,
                    RS_YUV_YV12)));
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 4, 5, false, false,
                    RS_YUV_YV12)));

    // yuv_format with mipmaps or cubemap is invalid
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 4, 0, false, true,
                    RS_YUV_YV12)));
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 4, 0, true, false,
                    RS_YUV_YV12)));

    // mipmaps with 1D or 3D is invalid
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 0, 0, true, false,
                    RS_YUV_NONE)));
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 4, 5, true, false,
                    RS_YUV_NONE)));

    // cubemap with 1D or 3D is invalid
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 0, 0, false, true,
                    RS_YUV_NONE)));
    _RS_ASSERT(!rsIsObject(rsCreateType(I32_3, 3, 4, 5, false, true,
                    RS_YUV_NONE)));

    /////////////////////////////////////////////////////////////////
    // Test rs_allocation creation
    /////////////////////////////////////////////////////////////////
    rs_type I32_3_2D = rsCreateType(I32_3, 3, 4);

    // Valid uses
    _RS_ASSERT(rsIsObject(rsCreateAllocation(I32_3_2D)));
    _RS_ASSERT(rsIsObject(rsCreateAllocation(I32_3_2D,\
                    (uint32_t) RS_ALLOCATION_USAGE_SCRIPT)));
    _RS_ASSERT(rsIsObject(rsCreateAllocation(I32_3_2D,\
                    (uint32_t) RS_ALLOCATION_USAGE_GRAPHICS_TEXTURE)));
    _RS_ASSERT(rsIsObject(rsCreateAllocation(I32_3_2D,
                    (uint32_t) RS_ALLOCATION_USAGE_GRAPHICS_TEXTURE |
                               RS_ALLOCATION_USAGE_SCRIPT)));

    // Invalid uses
    _RS_ASSERT(!rsIsObject(rsCreateAllocation(I32_3_2D,
                    (uint32_t) RS_ALLOCATION_USAGE_GRAPHICS_VERTEX)));
    _RS_ASSERT(!rsIsObject(rsCreateAllocation(I32_3_2D,
                    (uint32_t) RS_ALLOCATION_USAGE_GRAPHICS_CONSTANTS)));
    _RS_ASSERT(!rsIsObject(rsCreateAllocation(I32_3_2D,
                    (uint32_t) RS_ALLOCATION_USAGE_GRAPHICS_RENDER_TARGET)));
    _RS_ASSERT(!rsIsObject(rsCreateAllocation(I32_3_2D,
                    (uint32_t) RS_ALLOCATION_USAGE_IO_INPUT)));
    _RS_ASSERT(!rsIsObject(rsCreateAllocation(I32_3_2D,
                    (uint32_t) RS_ALLOCATION_USAGE_IO_OUTPUT)));
    _RS_ASSERT(!rsIsObject(rsCreateAllocation(I32_3_2D,
                    (uint32_t) RS_ALLOCATION_USAGE_SHARED)));

#ifndef RSTEST_COMPAT
    TEST_HELPER(half);
#endif
    TEST_HELPERS(float);
    TEST_HELPERS(double);
    TEST_HELPERS(char);
    TEST_HELPERS(short);
    TEST_HELPERS(int);
    TEST_HELPERS(long);
    TEST_HELPERS(uchar);
    TEST_HELPERS(ushort);
    TEST_HELPERS(uint);
    TEST_HELPERS(ulong);
}

void single_source_alloc_test() {
    if (failed) {
        rsDebug("Single Source Alloc Test Failed", 0);
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsDebug("Single Source Alloc Test Passed", 0);
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}
