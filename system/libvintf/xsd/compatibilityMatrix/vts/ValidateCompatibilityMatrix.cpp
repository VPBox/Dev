/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <unistd.h>
#include <string>

#include <android-base/properties.h>
#include "utility/ValidateXml.h"

TEST(CheckConfig, compatibilityMatrixValidation) {
    if (android::base::GetIntProperty("ro.product.first_api_level", 0) <= 28) {
        GTEST_SKIP();
    }

    RecordProperty("description",
                   "Verify that the compatibility matrix file "
                   "is valid according to the schema");

    std::vector<const char*> locations = {"/vendor/etc/vintf"};
    EXPECT_ONE_VALID_XML_MULTIPLE_LOCATIONS("compatibility_matrix.xml", locations,
                                            "/data/local/tmp/compatibility_matrix.xsd");
}
