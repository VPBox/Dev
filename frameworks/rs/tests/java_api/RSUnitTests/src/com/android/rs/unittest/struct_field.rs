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

// Ensure that we can reflect correct Java code for a struct with
// multiple fields of type struct.  Java sets the script global
// variable "outer", and then calls a verification invokable
// "checkOuter()", passing the expected field values as scalar
// arguments.

struct InnerOne {
    int x;
    int y;
    float f;
};

typedef struct InnerOne s_innerOne;

struct InnerTwo {
    int8_t z;
    struct InnerOne innerOne;
};

struct Outer {
    struct InnerOne innerOneA;
    long l;  // to induce padding
    struct InnerOne innerOneB;
    struct InnerTwo innerTwo3[3];
    struct InnerTwo innerTwo2[2];
    struct InnerOne innerOne4[4];
    s_innerOne      innerOneC;  // does a typedef confuse reflection?
};

struct Outer outer;

// Fragment of incoming argument list: Fields for struct InnerOne.
// Arguments are named argPrefix"_x", argPrefix"_y", argPrefix"_f".
#define ARGS_INNERONE_(argPrefix) \
    int argPrefix ## _x, int argPrefix ## _y, float argPrefix ## _f

// Fragment of incoming argument list: Fields for struct InnerOne
// within struct Outer.
// Arguments are named outerFieldName"_x", outerFieldName"_y", outerFieldName"_f".
#define ARGS_INNERONE_SCALAR(outerFieldName) \
    ARGS_INNERONE_(outerFieldName)

// Fragment of incoming argument list: Fields for element of array of
// struct InnerOne within struct Outer.
// Arguments are named outerFieldName"_"index"_x", outerFieldName"_"index"_y",
// and outerFieldName"_"index"_f".
#define ARGS_INNERONE_ARRAY(outerFieldName, index) \
    ARGS_INNERONE_(outerFieldName ## _ ## index)

// Fragment of incoming argument list: Fields for element of array of
// struct InnerTwo (with nested InnerOne flattened out) within struct Outer.
// Arguments are named outerFieldName"_"index"_z", outerFieldName"_"index"_innerOne_x", etc.
#define ARGS_INNERTWO_ARRAY(outerFieldName, index) \
    int8_t outerFieldName ## _ ## index ## _z, ARGS_INNERONE_(outerFieldName ## _ ## index ## _innerOne)

// #define TRACE(...) rsDebug(__VA_ARGS__)
#define TRACE(...) (void)0

void checkOuter(ARGS_INNERONE_SCALAR(innerOneA),
                long l,
                ARGS_INNERONE_SCALAR(innerOneB),
                ARGS_INNERTWO_ARRAY(innerTwo3, 0),
                ARGS_INNERTWO_ARRAY(innerTwo3, 1),
                ARGS_INNERTWO_ARRAY(innerTwo3, 2),
                ARGS_INNERTWO_ARRAY(innerTwo2, 0),
                ARGS_INNERTWO_ARRAY(innerTwo2, 1),
                ARGS_INNERONE_ARRAY(innerOne4, 0),
                ARGS_INNERONE_ARRAY(innerOne4, 1),
                ARGS_INNERONE_ARRAY(innerOne4, 2),
                ARGS_INNERONE_ARRAY(innerOne4, 3),
                ARGS_INNERONE_SCALAR(innerOneC)) {
    bool failed = false;

    // Compare contents of a struct InnerOne instance against incoming argument values.
    // Compares instanceName".x" to argPrefix"_x", etc.
#define CHECK_INNERONE_(instanceName, argPrefix)       \
    do {                                               \
       TRACE(# instanceName, instanceName.x);          \
        _RS_ASSERT(instanceName.x == argPrefix ## _x); \
       TRACE(# instanceName, instanceName.y);          \
        _RS_ASSERT(instanceName.y == argPrefix ## _y); \
       TRACE(# instanceName, instanceName.f);          \
        _RS_ASSERT(instanceName.f == argPrefix ## _f); \
    } while(false)

    // Compare contents of a struct InnerOne instance within global
    // variable "outer" against incoming argument values.
    // Compares "outer."outerFieldName".x" to outerFieldName"_x", etc.
#define CHECK_INNERONE_SCALAR(outerFieldName) \
    CHECK_INNERONE_(outer.outerFieldName, outerFieldName)

    // Compare contents of element of array of struct InnerOne
    // instance within global variable "outer" against incoming argument values.
    // Compares "outer"outerFieldName"["index"].x" to outerFieldName"_"index"_x", etc.
#define CHECK_INNERONE_ARRAY(outerFieldName, index) \
    CHECK_INNERONE_(outer.outerFieldName[index], outerFieldName ## _ ## index)

    // Compare contents of element of array of struct InnerTwo
    // instance within global variable "outer" against incoming argument values.
    // Compares "outer."outerFieldName"["index"].z" to outerFieldName"_"index"_z",
    // "outer."outerFieldName"["index"].innerOne.x" to outerFieldName""index"_innerOne_x",
    // etc.
#define CHECK_INNERTWO_ARRAY(outerFieldName, index)                                                 \
    do {                                                                                            \
        TRACE(# index, outer.outerFieldName[index].z);                \
        _RS_ASSERT(outer.outerFieldName[index].z == outerFieldName ## _ ## index ## _z); \
        CHECK_INNERONE_(outer.outerFieldName[index].innerOne, outerFieldName ## _ ## index ## _innerOne); \
    } while (false);

    CHECK_INNERONE_SCALAR(innerOneA);
    TRACE("l", outer.l);
    _RS_ASSERT(outer.l == l);
    CHECK_INNERONE_SCALAR(innerOneB);
    CHECK_INNERTWO_ARRAY(innerTwo3, 0);
    CHECK_INNERTWO_ARRAY(innerTwo3, 1);
    CHECK_INNERTWO_ARRAY(innerTwo3, 2);
    CHECK_INNERTWO_ARRAY(innerTwo2, 0);
    CHECK_INNERTWO_ARRAY(innerTwo2, 1);
    CHECK_INNERONE_ARRAY(innerOne4, 0);
    CHECK_INNERONE_ARRAY(innerOne4, 1);
    CHECK_INNERONE_ARRAY(innerOne4, 2);
    CHECK_INNERONE_ARRAY(innerOne4, 3);
    CHECK_INNERONE_SCALAR(innerOneC);

    if (failed) {
        rsDebug("struct_field FAILED", 0);
        rsSendToClientBlocking(RS_MSG_TEST_FAILED);
    }
    else {
        rsDebug("struct_field PASSED", 0);
        rsSendToClientBlocking(RS_MSG_TEST_PASSED);
    }
}
