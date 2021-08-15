/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_VERSION_SANITY_TEST_H_
#define _GTS_NANOAPPS_GENERAL_TEST_VERSION_SANITY_TEST_H_

#include <general_test/test.h>

namespace general_test {

/**
 * Checks that the chre_version.h methods give sane responses.
 *
 * Simple protocol.
 *
 * (Note: TODO(b/30077401): This test will have a non-simple protocol.)
 */
class VersionSanityTest : public Test {
 public:
  VersionSanityTest();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void* eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_VERSION_SANITY_TEST_H_
