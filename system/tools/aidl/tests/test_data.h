/*
 * Copyright (C) 2015, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AIDL_TESTS_TEST_DATA_H_
#define AIDL_TESTS_TEST_DATA_H_

namespace android {
namespace aidl {
namespace test_data {

namespace example_interface {

extern const char kCanonicalName[];
extern const char kJavaOutputPath[];
extern const char kInterfaceDefinition[];
extern const char kInterfaceDefinitionOutlining[];
extern const char* kImportedParcelables[];
extern const char* kImportedInterfaces[];

extern const char kExpectedJavaDepsOutput[];
extern const char kExpectedJavaOutput[];
extern const char kExpectedJavaOutputOutlining[];
extern const char kExpectedJavaOutputWithTransactionNames[];
extern const char kExpectedJavaOutputWithTrace[];
extern const char kExpectedJavaOutputWithVersion[];

}  // namespace example_interface

namespace ping_responder {

extern const char kCanonicalName[];
extern const char kInterfaceDefinition[];

extern const char kCppOutputPath[];
extern const char kCppParcelableHeader[];
extern const char* kImportedParcelables[];
extern const char* kImportedInterfaces[];

extern const char kGenHeaderDir[];
extern const char kGenInterfaceHeaderPath[];
extern const char kGenClientHeaderPath[];
extern const char kGenServerHeaderPath[];

extern const char kExpectedCppDepsOutput[];

extern const char kExpectedCppOutput[];
extern const char kExpectedIHeaderOutput[];
extern const char kExpectedBpHeaderOutput[];
extern const char kExpectedBnHeaderOutput[];

extern const char kExpectedCppOutputWithVersion[];
extern const char kExpectedIHeaderOutputWithVersion[];
extern const char kExpectedBpHeaderOutputWithVersion[];
extern const char kExpectedBnHeaderOutputWithVersion[];

}  // namespace ping_responder

namespace string_constants {

extern const char kCanonicalName[];
extern const char kInterfaceDefinition[];

extern const char kJavaOutputPath[];
extern const char kExpectedJavaOutput[];

extern const char kExpectedJavaOutputWithVersion[];

extern const char kCppOutputPath[];
extern const char kGenHeaderDir[];
extern const char kGenInterfaceHeaderPath[];
extern const char kExpectedIHeaderOutput[];
extern const char kExpectedCppOutput[];

extern const char kExpectedIHeaderOutputWithVersion[];
extern const char kExpectedCppOutputWithVersion[];

}  // namespace string_constants

}  // namespace test_data
}  // namespace aidl
}  // namespace android

#endif // AIDL_TESTS_TEST_DATA_H_
