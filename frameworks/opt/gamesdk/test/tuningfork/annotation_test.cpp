/*
 * Copyright 2019 The Android Open Source Project
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

#include "tuningfork/annotation_util.h"

#include "gtest/gtest.h"

using namespace annotation_util;

std::vector<int> TestSetup(const std::vector<int>& enum_sizes,
                           const std::vector<int>& expected){
    std::vector<int> radix_mult;
    SetUpAnnotationRadixes(radix_mult, enum_sizes);
    EXPECT_EQ(radix_mult, expected);
    return radix_mult;
}

TEST(Annotation, Setup) {
    TestSetup( {}, { 1 } );
    TestSetup( { 1 }, { 2 } );
    TestSetup( { 1, 1 }, { 2, 4 } );
    TestSetup( { 1, 1, 1 }, { 2, 4, 8 } );
    TestSetup( { 2, 1, 1 }, { 3, 6, 12 } );
}

void CheckEncodeDecode(AnnotationId id,
                       const std::vector<int>& radix_mult, const std::string& err) {
    SerializedAnnotation ser;
    EXPECT_EQ(SerializeAnnotationId(id, ser, radix_mult), 0) << err << ": error serializing";
    auto back = DecodeAnnotationSerialization( ser, radix_mult);
    EXPECT_EQ(id, back) << err;
}
TEST(Annotation, EncodeDecodeGood) {
    auto radix_mult = TestSetup( {2, 3, 4} , {3, 12, 60} );
    CheckEncodeDecode( 1, radix_mult, "First");
    CheckEncodeDecode( 2, radix_mult, "Second");
    CheckEncodeDecode( 3, radix_mult, "Third");
}
void CheckGood(SerializedAnnotation ser, AnnotationId id,
               const std::vector<int>& radix_mult) {
    auto back = DecodeAnnotationSerialization( ser, radix_mult);
    EXPECT_EQ(back, id) << "Good";
}
void CheckBad(SerializedAnnotation ser,
              const std::vector<int>& radix_mult) {
    auto back = DecodeAnnotationSerialization( ser, radix_mult);
    EXPECT_EQ(back, kAnnotationError) << "Bad";
}
TEST(Annotation, Decode) {
    auto radix_mult = TestSetup( {2, 3, 4} , {3, 12, 60} );
    CheckBad( {1}, radix_mult);
    CheckBad( {5<<3, 0 }, radix_mult);
    CheckBad( {1<<3, 0 }, radix_mult);
    CheckBad( {1<<3, 8 }, radix_mult);
    CheckBad( {1<<3, 1, 2<<3, 2, 3<<3}, radix_mult);
    CheckBad( {1<<3, 1, 2<<3, 2, 3<<3, 3, 0}, radix_mult);
    CheckGood( {1<<3, 1 }, 1, radix_mult);
    CheckGood( {1<<3, 2 }, 2, radix_mult);
    CheckGood( {1<<3, 1, 2<<3, 1}, 4, radix_mult);
    CheckGood( {1<<3, 2, 2<<3, 3, 3<<3, 4}, 59, radix_mult);
}
