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

#include <android-base/logging.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <unistd.h>

#include <android-base/strings.h>
#include <vintf/VintfObject.h>
#include <vintf/parse_string.h>
#include "utils-fake.h"

#include <hidl-util/FQName.h>

using namespace ::testing;

using android::FqInstance;

static AssertionResult In(const std::string& sub, const std::string& str) {
    return (str.find(sub) != std::string::npos ? AssertionSuccess() : AssertionFailure())
           << "Value is " << str;
}
#define EXPECT_IN(sub, str) EXPECT_TRUE(In((sub), (str)))
#define EXPECT_NOT_IN(sub, str) EXPECT_FALSE(In((sub), (str)))

namespace android {
namespace vintf {
namespace testing {

using namespace ::android::vintf::details;

//
// Set of Xml1 metadata compatible with each other.
//

const std::string systemMatrixXml1 =
    "<compatibility-matrix version=\"1.0\" type=\"framework\">\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hardware.camera</name>\n"
    "        <version>2.0-5</version>\n"
    "        <version>3.4-16</version>\n"
    "    </hal>\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hardware.nfc</name>\n"
    "        <version>1.0</version>\n"
    "        <version>2.0</version>\n"
    "    </hal>\n"
    "    <hal format=\"hidl\" optional=\"true\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <version>1.0</version>\n"
    "    </hal>\n"
    "    <kernel version=\"3.18.31\"></kernel>\n"
    "    <sepolicy>\n"
    "        <kernel-sepolicy-version>30</kernel-sepolicy-version>\n"
    "        <sepolicy-version>25.5</sepolicy-version>\n"
    "        <sepolicy-version>26.0-3</sepolicy-version>\n"
    "    </sepolicy>\n"
    "    <avb>\n"
    "        <vbmeta-version>0.0</vbmeta-version>\n"
    "    </avb>\n"
    "</compatibility-matrix>\n";

const std::string vendorManifestXml1 =
    "<manifest version=\"1.0\" type=\"device\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.camera</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>3.5</version>\n"
    "        <interface>\n"
    "            <name>IBetterCamera</name>\n"
    "            <instance>camera</instance>\n"
    "        </interface>\n"
    "        <interface>\n"
    "            <name>ICamera</name>\n"
    "            <instance>default</instance>\n"
    "            <instance>legacy/0</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.nfc</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>INfc</name>\n"
    "            <instance>nfc_nci</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.nfc</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>2.0</version>\n"
    "        <interface>\n"
    "            <name>INfc</name>\n"
    "            <instance>default</instance>\n"
    "            <instance>nfc_nci</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "    <sepolicy>\n"
    "        <version>25.5</version>\n"
    "    </sepolicy>\n"
    "</manifest>\n";

const std::string systemManifestXml1 =
    "<manifest version=\"1.0\" type=\"framework\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hidl.manager</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>IServiceManager</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "    <vndk>\n"
    "        <version>25.0.5</version>\n"
    "        <library>libbase.so</library>\n"
    "        <library>libjpeg.so</library>\n"
    "    </vndk>\n"
    "</manifest>\n";

const std::string vendorMatrixXml1 =
    "<compatibility-matrix version=\"1.0\" type=\"device\">\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hidl.manager</name>\n"
    "        <version>1.0</version>\n"
    "    </hal>\n"
    "    <vndk>\n"
    "        <version>25.0.1-5</version>\n"
    "        <library>libbase.so</library>\n"
    "        <library>libjpeg.so</library>\n"
    "    </vndk>\n"
    "</compatibility-matrix>\n";

//
// Set of Xml2 metadata compatible with each other.
//

const std::string systemMatrixXml2 =
    "<compatibility-matrix version=\"1.0\" type=\"framework\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <version>1.0</version>\n"
    "    </hal>\n"
    "    <kernel version=\"3.18.31\"></kernel>\n"
    "    <sepolicy>\n"
    "        <kernel-sepolicy-version>30</kernel-sepolicy-version>\n"
    "        <sepolicy-version>25.5</sepolicy-version>\n"
    "        <sepolicy-version>26.0-3</sepolicy-version>\n"
    "    </sepolicy>\n"
    "    <avb>\n"
    "        <vbmeta-version>0.0</vbmeta-version>\n"
    "    </avb>\n"
    "</compatibility-matrix>\n";

const std::string vendorManifestXml2 =
    "<manifest version=\"1.0\" type=\"device\">"
    "    <hal>"
    "        <name>android.hardware.foo</name>"
    "        <transport>hwbinder</transport>"
    "        <version>1.0</version>"
    "    </hal>"
    "    <sepolicy>\n"
    "        <version>25.5</version>\n"
    "    </sepolicy>\n"
    "</manifest>";

//
// Set of framework matrices of different FCM version.
//

const std::string systemMatrixLevel1 =
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"1\">\n"
    "    <hal format=\"hidl\" optional=\"true\">\n"
    "        <name>android.hardware.major</name>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>IMajor</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "    <hal format=\"hidl\" optional=\"true\">\n"
    "        <name>android.hardware.removed</name>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>IRemoved</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "    <hal format=\"hidl\" optional=\"true\">\n"
    "        <name>android.hardware.minor</name>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>IMinor</name>\n"
    "            <instance>default</instance>\n"
    "            <instance>legacy</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</compatibility-matrix>\n";

const std::string systemMatrixLevel2 =
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"2\">\n"
    "    <hal format=\"hidl\" optional=\"true\">\n"
    "        <name>android.hardware.major</name>\n"
    "        <version>2.0</version>\n"
    "        <interface>\n"
    "            <name>IMajor</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "    <hal format=\"hidl\" optional=\"true\">\n"
    "        <name>android.hardware.minor</name>\n"
    "        <version>1.1</version>\n"
    "        <interface>\n"
    "            <name>IMinor</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</compatibility-matrix>\n";

//
// Set of framework matrices of different FCM version with regex.
//

const static std::vector<std::string> systemMatrixRegexXmls = {
    // 1.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"1\">\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hardware.regex</name>\n"
    "        <version>1.0-1</version>\n"
    "        <interface>\n"
    "            <name>IRegex</name>\n"
    "            <instance>default</instance>\n"
    "            <instance>special/1.0</instance>\n"
    "            <regex-instance>regex/1.0/[0-9]+</regex-instance>\n"
    "            <regex-instance>regex_common/[0-9]+</regex-instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</compatibility-matrix>\n",
    // 2.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"2\">\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hardware.regex</name>\n"
    "        <version>1.1-2</version>\n"
    "        <interface>\n"
    "            <name>IRegex</name>\n"
    "            <instance>default</instance>\n"
    "            <instance>special/1.1</instance>\n"
    "            <regex-instance>regex/1.1/[0-9]+</regex-instance>\n"
    "            <regex-instance>[a-z]+_[a-z]+/[0-9]+</regex-instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</compatibility-matrix>\n",
    // 3.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"3\">\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hardware.regex</name>\n"
    "        <version>2.0</version>\n"
    "        <interface>\n"
    "            <name>IRegex</name>\n"
    "            <instance>default</instance>\n"
    "            <instance>special/2.0</instance>\n"
    "            <regex-instance>regex/2.0/[0-9]+</regex-instance>\n"
    "            <regex-instance>regex_[a-z]+/[0-9]+</regex-instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</compatibility-matrix>\n"};

//
// Set of metadata at different FCM version that has requirements
//

const std::vector<std::string> systemMatrixRequire = {
    // 1.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"1\">\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>IFoo</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</compatibility-matrix>\n",
    // 2.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"2\">\n"
    "    <hal format=\"hidl\" optional=\"false\">\n"
    "        <name>android.hardware.bar</name>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>IBar</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</compatibility-matrix>\n"};

const std::string vendorManifestRequire1 =
    "<manifest version=\"1.0\" type=\"device\" target-level=\"1\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <fqname>@1.0::IFoo/default</fqname>\n"
    "    </hal>\n"
    "</manifest>\n";

const std::string vendorManifestRequire2 =
    "<manifest version=\"1.0\" type=\"device\" target-level=\"2\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.bar</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <fqname>@1.0::IBar/default</fqname>\n"
    "    </hal>\n"
    "</manifest>\n";

//
// Set of metadata for kernel requirements
//

const std::string vendorManifestKernel318 =
    "<manifest version=\"1.0\" type=\"device\">\n"
    "    <kernel version=\"3.18.999\" />\n"
    "    <sepolicy>\n"
    "        <version>25.5</version>\n"
    "    </sepolicy>\n"
    "</manifest>\n";

const std::string systemMatrixKernel318 =
    "<compatibility-matrix version=\"1.0\" type=\"framework\">\n"
    "    <kernel version=\"3.18.999\"></kernel>\n"
    "    <sepolicy>\n"
    "        <kernel-sepolicy-version>30</kernel-sepolicy-version>\n"
    "        <sepolicy-version>25.5</sepolicy-version>\n"
    "    </sepolicy>\n"
    "</compatibility-matrix>\n";

class VintfObjectTestBase : public ::testing::Test {
   protected:
    MockFileSystem& fetcher() {
        return static_cast<MockFileSystem&>(*vintfObject->getFileSystem());
    }
    MockPropertyFetcher& propertyFetcher() {
        return static_cast<MockPropertyFetcher&>(*vintfObject->getPropertyFetcher());
    }

    // Setup the MockFileSystem used by the fetchAllInformation template
    // so it returns the given metadata info instead of fetching from device.
    void setupMockFetcher(const std::string& vendorManifestXml, const std::string& systemMatrixXml,
                          const std::string& systemManifestXml, const std::string& vendorMatrixXml,
                          const std::string& productModel) {
        ON_CALL(fetcher(), listFiles(StrEq(kVendorManifestFragmentDir), _, _))
            .WillByDefault(Return(::android::OK));
        ON_CALL(fetcher(), listFiles(StrEq(kSystemManifestFragmentDir), _, _))
            .WillByDefault(Return(::android::OK));
        ON_CALL(fetcher(), listFiles(StrEq(kOdmManifestFragmentDir), _, _))
            .WillByDefault(Return(::android::OK));
        ON_CALL(fetcher(), listFiles(StrEq(kProductManifestFragmentDir), _, _))
            .WillByDefault(Return(::android::OK));

        if (!productModel.empty()) {
            ON_CALL(fetcher(),
                    fetch(StrEq(kOdmLegacyVintfDir + "manifest_" + productModel + ".xml"), _))
                .WillByDefault(Return(::android::NAME_NOT_FOUND));
            ON_CALL(fetcher(), fetch(StrEq(kOdmVintfDir + "manifest_" + productModel + ".xml"), _))
                .WillByDefault(Return(::android::NAME_NOT_FOUND));
        }
        ON_CALL(fetcher(), fetch(StrEq(kOdmLegacyManifest), _))
            .WillByDefault(Return(::android::NAME_NOT_FOUND));
        ON_CALL(fetcher(), fetch(StrEq(kOdmManifest), _))
            .WillByDefault(Return(::android::NAME_NOT_FOUND));
        ON_CALL(fetcher(), fetch(StrEq(kVendorManifest), _))
            .WillByDefault(Return(::android::NAME_NOT_FOUND));
        ON_CALL(fetcher(), fetch(StrEq(kVendorLegacyManifest), _))
            .WillByDefault(
                Invoke([vendorManifestXml](const std::string& path, std::string& fetched) {
                    (void)path;
                    fetched = vendorManifestXml;
                    return 0;
                }));
        ON_CALL(fetcher(), fetch(StrEq(kSystemManifest), _))
            .WillByDefault(
                Invoke([systemManifestXml](const std::string& path, std::string& fetched) {
                    (void)path;
                    fetched = systemManifestXml;
                    return 0;
                }));
        ON_CALL(fetcher(), fetch(StrEq(kVendorMatrix), _))
            .WillByDefault(Return(::android::NAME_NOT_FOUND));
        ON_CALL(fetcher(), fetch(StrEq(kVendorLegacyMatrix), _))
            .WillByDefault(Invoke([vendorMatrixXml](const std::string& path, std::string& fetched) {
                (void)path;
                fetched = vendorMatrixXml;
                return 0;
            }));
        ON_CALL(fetcher(), fetch(StrEq(kSystemLegacyMatrix), _))
            .WillByDefault(Invoke([systemMatrixXml](const std::string& path, std::string& fetched) {
                (void)path;
                fetched = systemMatrixXml;
                return 0;
            }));
        // Don't list /system/etc/vintf unless otherwise specified.
        ON_CALL(fetcher(), listFiles(StrEq(kSystemVintfDir), _, _))
            .WillByDefault(Return(::android::OK));
        // Don't fetch product matrix unless otherwise specified.
        ON_CALL(fetcher(), fetch(StrEq(kProductMatrix), _)).WillByDefault(Return(NAME_NOT_FOUND));
        // Don't fetch product manifest unless otherwise specified.
        ON_CALL(fetcher(), fetch(StrEq(kProductManifest), _)).WillByDefault(Return(NAME_NOT_FOUND));
    }

    void setFakeProperties() {
        productModel = "fake_sku";
        ON_CALL(propertyFetcher(), getProperty("ro.boot.product.hardware.sku", _))
            .WillByDefault(Return(productModel));
    }

    virtual void SetUp() {
        vintfObject = VintfObject::Builder()
                          .setFileSystem(std::make_unique<NiceMock<MockFileSystem>>())
                          .setRuntimeInfoFactory(std::make_unique<NiceMock<MockRuntimeInfoFactory>>(
                              std::make_shared<NiceMock<MockRuntimeInfo>>()))
                          .setPropertyFetcher(std::make_unique<NiceMock<MockPropertyFetcher>>())
                          .build();
    }
    virtual void TearDown() {
        Mock::VerifyAndClear(&fetcher());
    }

    void expectVendorManifest(size_t times = 1) {
        EXPECT_CALL(fetcher(), fetch(StrEq(kVendorManifest), _)).Times(times);
        if (!productModel.empty()) {
            EXPECT_CALL(fetcher(),
                        fetch(StrEq(kOdmLegacyVintfDir + "manifest_" + productModel + ".xml"), _))
                .Times(times);
            EXPECT_CALL(fetcher(),
                        fetch(StrEq(kOdmVintfDir + "manifest_" + productModel + ".xml"), _))
                .Times(times);
        }
        EXPECT_CALL(fetcher(), fetch(StrEq(kOdmLegacyManifest), _)).Times(times);
        EXPECT_CALL(fetcher(), fetch(StrEq(kOdmManifest), _)).Times(times);
        EXPECT_CALL(fetcher(), fetch(StrEq(kVendorLegacyManifest), _)).Times(times);
    }

    void expectSystemManifest(size_t times = 1) {
        EXPECT_CALL(fetcher(), fetch(StrEq(kSystemManifest), _)).Times(times);
        EXPECT_CALL(fetcher(), fetch(StrEq(kProductManifest), _)).Times(times);
    }

    void expectVendorMatrix(size_t times = 1) {
        EXPECT_CALL(fetcher(), fetch(StrEq(kVendorMatrix), _)).Times(times);
        EXPECT_CALL(fetcher(), fetch(StrEq(kVendorLegacyMatrix), _)).Times(times);
    }

    void expectSystemMatrix(size_t times = 1) {
        EXPECT_CALL(fetcher(), fetch(StrEq(kProductMatrix), _)).Times(times);
        EXPECT_CALL(fetcher(), fetch(StrEq(kSystemLegacyMatrix), _)).Times(times);
    }

    // Expect that a file exist and should be fetched once.
    void expectFetch(const std::string& path, const std::string& content) {
        EXPECT_CALL(fetcher(), fetch(StrEq(path), _))
            .WillOnce(Invoke([content](const auto&, auto& out) {
                out = content;
                return ::android::OK;
            }));
    }

    // Expect that a file exist and can be fetched 0 or more times.
    void expectFetchRepeatedly(const std::string& path, const std::string& content) {
        EXPECT_CALL(fetcher(), fetch(StrEq(path), _))
            .Times(AnyNumber())
            .WillRepeatedly(Invoke([content](const auto&, auto& out) {
                out = content;
                return ::android::OK;
            }));
    }

    // Expect that the file should never be fetched (whether it exists or not).
    void expectNeverFetch(const std::string& path) {
        EXPECT_CALL(fetcher(), fetch(StrEq(path), _)).Times(0);
    }

    // Expect that the file does not exist, and can be fetched 0 or more times.
    template <typename Matcher>
    void expectFileNotExist(const Matcher& matcher) {
        EXPECT_CALL(fetcher(), fetch(matcher, _))
            .Times(AnyNumber())
            .WillRepeatedly(Return(::android::NAME_NOT_FOUND));
    }

    // Access to private method.
    int checkCompatibility(const std::vector<std::string>& xmls, std::string* error) {
        return vintfObject->checkCompatibility(xmls, error);
    }

    std::string productModel;
    std::unique_ptr<VintfObject> vintfObject;
};

// Test fixture that provides compatible metadata from the mock device.
class VintfObjectCompatibleTest : public VintfObjectTestBase {
   protected:
    virtual void SetUp() {
        VintfObjectTestBase::SetUp();
        setFakeProperties();
        setupMockFetcher(vendorManifestXml1, systemMatrixXml1, systemManifestXml1, vendorMatrixXml1,
                         productModel);
    }
};

// Tests that local info is checked.
TEST_F(VintfObjectCompatibleTest, TestDeviceCompatibility) {
    std::string error;
    std::vector<std::string> packageInfo;

    expectVendorManifest();
    expectSystemManifest();
    expectVendorMatrix();
    expectSystemMatrix();

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 0) << "Fail message:" << error.c_str();
    // Check that nothing was ignored.
    ASSERT_STREQ(error.c_str(), "");
}

// Tests that input info is checked against device and passes.
TEST_F(VintfObjectCompatibleTest, TestInputVsDeviceSuccess) {
    std::string error;
    std::vector<std::string> packageInfo = {systemMatrixXml1};

    expectVendorManifest();
    expectSystemManifest();
    expectVendorMatrix();
    expectSystemMatrix(0);

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 0) << "Fail message:" << error.c_str();
    ASSERT_STREQ(error.c_str(), "");
}

// Tests that input info is checked against device and fails.
TEST_F(VintfObjectCompatibleTest, TestInputVsDeviceFail) {
    std::string error;
    std::vector<std::string> packageInfo = {systemMatrixXml2};

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 1) << "Should have failed:" << error.c_str();
    EXPECT_IN(
        "Device manifest and framework compatibility matrix are incompatible: HALs "
        "incompatible.",
        error);
    EXPECT_IN("android.hardware.foo", error);
}

// Tests that complementary info is checked against itself.
TEST_F(VintfObjectCompatibleTest, TestInputSuccess) {
    std::string error;
    std::vector<std::string> packageInfo = {systemMatrixXml2, vendorManifestXml2};

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 0) << "Failed message:" << error.c_str();
    ASSERT_STREQ(error.c_str(), "");
}

TEST_F(VintfObjectCompatibleTest, TestFrameworkOnlyOta) {
    std::string error;
    std::vector<std::string> packageInfo = {systemMatrixXml1, systemManifestXml1};

    expectVendorManifest();
    expectSystemManifest(0);
    expectVendorMatrix();
    expectSystemMatrix(0);

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 0) << "Fail message:" << error.c_str();
    ASSERT_STREQ(error.c_str(), "");
}

TEST_F(VintfObjectCompatibleTest, TestFullOta) {
    std::string error;
    std::vector<std::string> packageInfo = {systemMatrixXml1, systemManifestXml1,
            vendorMatrixXml1, vendorManifestXml1};

    expectVendorManifest(0);
    expectSystemManifest(0);
    expectVendorMatrix(0);
    expectSystemMatrix(0);

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 0) << "Fail message:" << error.c_str();
    ASSERT_STREQ(error.c_str(), "");
}

// Test that framework-only OTA fails when kernel is not compatible with incoming system.
TEST_F(VintfObjectCompatibleTest, KernelInfoIncompatible) {
    std::string error;
    std::vector<std::string> packageInfo = {systemMatrixKernel318};

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, INCOMPATIBLE) << "Should have failed.";
    EXPECT_IN("Framework is incompatible with kernel version 3.18.31", error);
}

// Test that full OTA is successful when the OTA package provides a compatible kernel.
TEST_F(VintfObjectCompatibleTest, UpdateKernel) {
    std::string error;
    std::vector<std::string> packageInfo = {vendorManifestKernel318, systemMatrixKernel318};

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, COMPATIBLE) << "Fail message:" << error;
    ASSERT_STREQ(error.c_str(), "");
}

// Test fixture that provides incompatible metadata from the mock device.
class VintfObjectIncompatibleTest : public VintfObjectTestBase {
   protected:
    virtual void SetUp() {
        VintfObjectTestBase::SetUp();
        setFakeProperties();
        setupMockFetcher(vendorManifestXml1, systemMatrixXml2, systemManifestXml1, vendorMatrixXml1,
                         productModel);
    }
};

// Fetch all metadata from device and ensure that it fails.
TEST_F(VintfObjectIncompatibleTest, TestDeviceCompatibility) {
    std::string error;
    std::vector<std::string> packageInfo;

    expectVendorManifest();
    expectSystemManifest();
    expectVendorMatrix();
    expectSystemMatrix();

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 1) << "Should have failed:" << error.c_str();
}

// Pass in new metadata that fixes the incompatibility.
TEST_F(VintfObjectIncompatibleTest, TestInputVsDeviceSuccess) {
    std::string error;
    std::vector<std::string> packageInfo = {systemMatrixXml1};

    expectVendorManifest();
    expectSystemManifest();
    expectVendorMatrix();
    expectSystemMatrix(0);

    int result = checkCompatibility(packageInfo, &error);

    ASSERT_EQ(result, 0) << "Failed message:" << error.c_str();
    ASSERT_STREQ(error.c_str(), "");
}

// Test fixture that provides compatible metadata from the mock device.
class VintfObjectRuntimeInfoTest : public VintfObjectTestBase {
   protected:
    virtual void SetUp() {
        VintfObjectTestBase::SetUp();
        // clear fetch flags
        runtimeInfoFactory().getInfo()->failNextFetch();
        vintfObject->getRuntimeInfo(true /* skipCache */, RuntimeInfo::FetchFlag::ALL);
    }
    virtual void TearDown() {
        Mock::VerifyAndClear(&runtimeInfoFactory());
        Mock::VerifyAndClear(runtimeInfoFactory().getInfo().get());
    }
    MockRuntimeInfoFactory& runtimeInfoFactory() {
        return static_cast<MockRuntimeInfoFactory&>(*vintfObject->getRuntimeInfoFactory());
    }
};

TEST_F(VintfObjectRuntimeInfoTest, GetRuntimeInfo) {
    InSequence s;

    EXPECT_CALL(*runtimeInfoFactory().getInfo(),
                fetchAllInformation(RuntimeInfo::FetchFlag::CPU_VERSION));
    EXPECT_CALL(*runtimeInfoFactory().getInfo(), fetchAllInformation(RuntimeInfo::FetchFlag::NONE));
    EXPECT_CALL(*runtimeInfoFactory().getInfo(),
                fetchAllInformation(RuntimeInfo::FetchFlag::CPU_VERSION));
    EXPECT_CALL(
        *runtimeInfoFactory().getInfo(),
        fetchAllInformation(RuntimeInfo::FetchFlag::ALL & ~RuntimeInfo::FetchFlag::CPU_VERSION));
    EXPECT_CALL(*runtimeInfoFactory().getInfo(), fetchAllInformation(RuntimeInfo::FetchFlag::ALL));
    EXPECT_CALL(*runtimeInfoFactory().getInfo(), fetchAllInformation(RuntimeInfo::FetchFlag::NONE));

    vintfObject->getRuntimeInfo(false /* skipCache */, RuntimeInfo::FetchFlag::CPU_VERSION);
    vintfObject->getRuntimeInfo(false /* skipCache */, RuntimeInfo::FetchFlag::CPU_VERSION);
    vintfObject->getRuntimeInfo(true /* skipCache */, RuntimeInfo::FetchFlag::CPU_VERSION);
    vintfObject->getRuntimeInfo(false /* skipCache */, RuntimeInfo::FetchFlag::ALL);
    vintfObject->getRuntimeInfo(true /* skipCache */, RuntimeInfo::FetchFlag::ALL);
    vintfObject->getRuntimeInfo(false /* skipCache */, RuntimeInfo::FetchFlag::ALL);
}

// Test fixture that provides incompatible metadata from the mock device.
class VintfObjectTest : public VintfObjectTestBase {
   protected:
    virtual void SetUp() {
        VintfObjectTestBase::SetUp();

        // By default use empty filesystem
        EXPECT_CALL(fetcher(), listFiles(_, _, _))
            .Times(AnyNumber())
            .WillRepeatedly(Return(::android::OK));
        EXPECT_CALL(fetcher(), fetch(_, _))
            .Times(AnyNumber())
            .WillRepeatedly(Return(::android::NAME_NOT_FOUND));
    }
};

// Test framework compatibility matrix is combined at runtime
TEST_F(VintfObjectTest, FrameworkCompatibilityMatrixCombine) {
    EXPECT_CALL(fetcher(), listFiles(StrEq(kSystemVintfDir), _, _))
        .WillOnce(Invoke([](const auto&, auto* out, auto*) {
            *out = {
                "compatibility_matrix.1.xml",
                "compatibility_matrix.empty.xml",
            };
            return ::android::OK;
        }));
    expectFetch(kSystemVintfDir + "compatibility_matrix.1.xml",
                "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"1\"/>");
    expectFetch(kSystemVintfDir + "compatibility_matrix.empty.xml",
                "<compatibility-matrix version=\"1.0\" type=\"framework\"/>");
    expectFileNotExist(StrEq(kProductMatrix));
    expectFetch(kVendorManifest, "<manifest version=\"1.0\" type=\"device\" />\n");
    expectNeverFetch(kSystemLegacyMatrix);

    EXPECT_NE(nullptr, vintfObject->getFrameworkCompatibilityMatrix(true /* skipCache */));
}

// Test product compatibility matrix is fetched
TEST_F(VintfObjectTest, ProductCompatibilityMatrix) {
    EXPECT_CALL(fetcher(), listFiles(StrEq(kSystemVintfDir), _, _))
        .WillOnce(Invoke([](const auto&, auto* out, auto*) {
            *out = {
                "compatibility_matrix.1.xml",
                "compatibility_matrix.empty.xml",
            };
            return ::android::OK;
        }));
    expectFetch(kSystemVintfDir + "compatibility_matrix.1.xml",
                "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"1\"/>");
    expectFetch(kSystemVintfDir + "compatibility_matrix.empty.xml",
                "<compatibility-matrix version=\"1.0\" type=\"framework\"/>");
    expectFetch(kProductMatrix,
                "<compatibility-matrix version=\"1.0\" type=\"framework\">\n"
                "    <hal format=\"hidl\" optional=\"true\">\n"
                "        <name>android.hardware.foo</name>\n"
                "        <version>1.0</version>\n"
                "        <interface>\n"
                "            <name>IFoo</name>\n"
                "            <instance>default</instance>\n"
                "        </interface>\n"
                "    </hal>\n"
                "</compatibility-matrix>\n");
    expectFetch(kVendorManifest, "<manifest version=\"1.0\" type=\"device\" />\n");
    expectNeverFetch(kSystemLegacyMatrix);

    auto fcm = vintfObject->getFrameworkCompatibilityMatrix(true /* skipCache */);
    ASSERT_NE(nullptr, fcm);

    FqInstance expectInstance;
    EXPECT_TRUE(expectInstance.setTo("android.hardware.foo@1.0::IFoo/default"));
    bool found = false;
    fcm->forEachInstance([&found, &expectInstance](const auto& matrixInstance) {
        found |= matrixInstance.isSatisfiedBy(expectInstance);
        return !found;  // continue if not found
    });
    EXPECT_TRUE(found) << "android.hardware.foo@1.0::IFoo/default should be found in matrix:\n"
                       << gCompatibilityMatrixConverter(*fcm);
}

const std::string vendorEtcManifest =
    "<manifest version=\"1.0\" type=\"device\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>1.0</version>\n"
    "        <version>2.0</version>\n"
    "        <interface>\n"
    "            <name>IVendorEtc</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</manifest>\n";

const std::string vendorManifest =
    "<manifest version=\"1.0\" type=\"device\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>1.0</version>\n"
    "        <interface>\n"
    "            <name>IVendor</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</manifest>\n";

const std::string odmProductManifest =
    "<manifest version=\"1.0\" type=\"device\">\n"
    "    <hal format=\"hidl\" override=\"true\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>1.1</version>\n"
    "        <interface>\n"
    "            <name>IOdmProduct</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</manifest>\n";

const std::string odmManifest =
    "<manifest version=\"1.0\" type=\"device\">\n"
    "    <hal format=\"hidl\" override=\"true\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <version>1.1</version>\n"
    "        <interface>\n"
    "            <name>IOdm</name>\n"
    "            <instance>default</instance>\n"
    "        </interface>\n"
    "    </hal>\n"
    "</manifest>\n";

bool containsVendorManifest(const std::shared_ptr<const HalManifest>& p) {
    return !p->getInstances("android.hardware.foo", {1, 0}, "IVendor").empty();
}

bool containsVendorEtcManifest(const std::shared_ptr<const HalManifest>& p) {
    return !p->getInstances("android.hardware.foo", {2, 0}, "IVendorEtc").empty();
}

bool vendorEtcManifestOverridden(const std::shared_ptr<const HalManifest>& p) {
    return p->getInstances("android.hardware.foo", {1, 0}, "IVendorEtc").empty();
}

bool containsOdmManifest(const std::shared_ptr<const HalManifest>& p) {
    return !p->getInstances("android.hardware.foo", {1, 1}, "IOdm").empty();
}

bool containsOdmProductManifest(const std::shared_ptr<const HalManifest>& p) {
    return !p->getInstances("android.hardware.foo", {1, 1}, "IOdmProduct").empty();
}

class DeviceManifestTest : public VintfObjectTestBase {
   protected:
    virtual void SetUp() {
        VintfObjectTestBase::SetUp();
        setFakeProperties();
    }

    // Expect that /vendor/etc/vintf/manifest.xml is fetched.
    void expectVendorManifest() { expectFetch(kVendorManifest, vendorEtcManifest); }
    // /vendor/etc/vintf/manifest.xml does not exist.
    void noVendorManifest() { expectFileNotExist(StrEq(kVendorManifest)); }
    // Expect some ODM manifest is fetched.
    void expectOdmManifest() {
        if (!productModel.empty()) {
            expectFileNotExist(StrEq(kOdmVintfDir + "manifest_" + productModel + ".xml"));
        }
        expectFetch(kOdmManifest, odmManifest);
    }
    void noOdmManifest() { expectFileNotExist(StartsWith("/odm/")); }
    std::shared_ptr<const HalManifest> get() {
        return vintfObject->getDeviceHalManifest(true /* skipCache */);
    }
};

// Test /vendor/etc/vintf/manifest.xml + ODM manifest
TEST_F(DeviceManifestTest, Combine1) {
    expectVendorManifest();
    expectOdmManifest();
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_TRUE(containsVendorEtcManifest(p));
    EXPECT_TRUE(vendorEtcManifestOverridden(p));
    EXPECT_TRUE(containsOdmManifest(p));
    EXPECT_FALSE(containsVendorManifest(p));
}

// Test /vendor/etc/vintf/manifest.xml
TEST_F(DeviceManifestTest, Combine2) {
    expectVendorManifest();
    noOdmManifest();
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_TRUE(containsVendorEtcManifest(p));
    EXPECT_FALSE(vendorEtcManifestOverridden(p));
    EXPECT_FALSE(containsOdmManifest(p));
    EXPECT_FALSE(containsVendorManifest(p));
}

// Test ODM manifest
TEST_F(DeviceManifestTest, Combine3) {
    noVendorManifest();
    expectOdmManifest();
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_FALSE(containsVendorEtcManifest(p));
    EXPECT_TRUE(vendorEtcManifestOverridden(p));
    EXPECT_TRUE(containsOdmManifest(p));
    EXPECT_FALSE(containsVendorManifest(p));
}

// Test /vendor/manifest.xml
TEST_F(DeviceManifestTest, Combine4) {
    noVendorManifest();
    noOdmManifest();
    expectFetch(kVendorLegacyManifest, vendorManifest);
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_FALSE(containsVendorEtcManifest(p));
    EXPECT_TRUE(vendorEtcManifestOverridden(p));
    EXPECT_FALSE(containsOdmManifest(p));
    EXPECT_TRUE(containsVendorManifest(p));
}

class OdmManifestTest : public VintfObjectTestBase {
   protected:
    virtual void SetUp() override {
        VintfObjectTestBase::SetUp();
        // Assume /vendor/etc/vintf/manifest.xml does not exist to simplify
        // testing logic.
        expectFileNotExist(StrEq(kVendorManifest));
        // Expect that the legacy /vendor/manifest.xml is never fetched.
        expectNeverFetch(kVendorLegacyManifest);
        // Assume no files exist under /odm/ unless otherwise specified.
        expectFileNotExist(StartsWith("/odm/"));
    }
    std::shared_ptr<const HalManifest> get() {
        return vintfObject->getDeviceHalManifest(true /* skipCache */);
    }
};

TEST_F(OdmManifestTest, OdmProductManifest) {
    if (productModel.empty()) return;
    expectFetch(kOdmVintfDir + "manifest_" + productModel + ".xml", odmProductManifest);
    // /odm/etc/vintf/manifest.xml should not be fetched when the product variant exists.
    expectNeverFetch(kOdmManifest);
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_TRUE(containsOdmProductManifest(p));
}

TEST_F(OdmManifestTest, OdmManifest) {
    expectFetch(kOdmManifest, odmManifest);
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_TRUE(containsOdmManifest(p));
}

TEST_F(OdmManifestTest, OdmLegacyProductManifest) {
    if (productModel.empty()) return;
    expectFetch(kOdmLegacyVintfDir + "manifest_" + productModel + ".xml", odmProductManifest);
    // /odm/manifest.xml should not be fetched when the product variant exists.
    expectNeverFetch(kOdmLegacyManifest);
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_TRUE(containsOdmProductManifest(p));
}

TEST_F(OdmManifestTest, OdmLegacyManifest) {
    expectFetch(kOdmLegacyManifest, odmManifest);
    auto p = get();
    ASSERT_NE(nullptr, p);
    EXPECT_TRUE(containsOdmManifest(p));
}

struct CheckedFqInstance : FqInstance {
    CheckedFqInstance(const char* s) : CheckedFqInstance(std::string(s)) {}
    CheckedFqInstance(const std::string& s) { CHECK(setTo(s)) << s; }

    Version getVersion() const { return FqInstance::getVersion(); }
};

static VintfObject::ListInstances getInstanceListFunc(
    const std::vector<CheckedFqInstance>& instances) {
    return [instances](const std::string& package, Version version, const std::string& interface,
                       const auto& /* instanceHint */) {
        std::vector<std::pair<std::string, Version>> ret;
        for (auto&& existing : instances) {
            if (existing.getPackage() == package && existing.getVersion().minorAtLeast(version) &&
                existing.getInterface() == interface) {
                ret.push_back(std::make_pair(existing.getInstance(), existing.getVersion()));
            }
        }

        return ret;
    };
}

class DeprecateTest : public VintfObjectTestBase {
   protected:
    virtual void SetUp() override {
        VintfObjectTestBase::SetUp();
        EXPECT_CALL(fetcher(), listFiles(StrEq(kSystemVintfDir), _, _))
            .WillRepeatedly(Invoke([](const auto&, auto* out, auto*) {
                *out = {
                    "compatibility_matrix.1.xml",
                    "compatibility_matrix.2.xml",
                };
                return ::android::OK;
            }));
        EXPECT_CALL(fetcher(), listFiles(StrEq(kVendorManifestFragmentDir), _, _))
            .WillOnce(Return(::android::OK));
        EXPECT_CALL(fetcher(), listFiles(StrEq(kOdmManifestFragmentDir), _, _))
            .WillOnce(Return(::android::OK));
        expectFetch(kSystemVintfDir + "compatibility_matrix.1.xml", systemMatrixLevel1);
        expectFetch(kSystemVintfDir + "compatibility_matrix.2.xml", systemMatrixLevel2);
        expectFileNotExist(StrEq(kProductMatrix));
        expectNeverFetch(kSystemLegacyMatrix);

        expectFetch(kVendorManifest,
                    "<manifest version=\"1.0\" type=\"device\" target-level=\"2\"/>");
        expectFileNotExist(StartsWith("/odm/"));

        // Update the device manifest cache because CheckDeprecate does not fetch
        // device manifest again if cache exist.
        vintfObject->getDeviceHalManifest(true /* skipCache */);
    }

};

TEST_F(DeprecateTest, CheckNoDeprecate) {
    auto pred = getInstanceListFunc({
        "android.hardware.minor@1.1::IMinor/default",
        "android.hardware.major@2.0::IMajor/default",
    });
    std::string error;
    EXPECT_EQ(NO_DEPRECATED_HALS, vintfObject->checkDeprecation(pred, &error)) << error;
}

TEST_F(DeprecateTest, CheckRemoved) {
    auto pred = getInstanceListFunc({
        "android.hardware.removed@1.0::IRemoved/default",
        "android.hardware.minor@1.1::IMinor/default",
        "android.hardware.major@2.0::IMajor/default",
    });
    std::string error;
    EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
        << "removed@1.0 should be deprecated. " << error;
}

TEST_F(DeprecateTest, CheckMinor) {
    auto pred = getInstanceListFunc({
        "android.hardware.minor@1.0::IMinor/default",
        "android.hardware.major@2.0::IMajor/default",
    });
    std::string error;
    EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
        << "minor@1.0 should be deprecated. " << error;
}

TEST_F(DeprecateTest, CheckMinorDeprecatedInstance1) {
    auto pred = getInstanceListFunc({
        "android.hardware.minor@1.0::IMinor/legacy",
        "android.hardware.minor@1.1::IMinor/default",
        "android.hardware.major@2.0::IMajor/default",
    });
    std::string error;
    EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
        << "minor@1.0::IMinor/legacy should be deprecated. " << error;
}

TEST_F(DeprecateTest, CheckMinorDeprecatedInstance2) {
    auto pred = getInstanceListFunc({
        "android.hardware.minor@1.1::IMinor/default",
        "android.hardware.minor@1.1::IMinor/legacy",
        "android.hardware.major@2.0::IMajor/default",
    });
    std::string error;
    EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
        << "minor@1.1::IMinor/legacy should be deprecated. " << error;
}

TEST_F(DeprecateTest, CheckMajor1) {
    auto pred = getInstanceListFunc({
        "android.hardware.minor@1.1::IMinor/default",
        "android.hardware.major@1.0::IMajor/default",
        "android.hardware.major@2.0::IMajor/default",
    });
    std::string error;
    EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
        << "major@1.0 should be deprecated. " << error;
}

TEST_F(DeprecateTest, CheckMajor2) {
    auto pred = getInstanceListFunc({
        "android.hardware.minor@1.1::IMinor/default",
        "android.hardware.major@1.0::IMajor/default",
    });
    std::string error;
    EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
        << "major@1.0 should be deprecated. " << error;
}

class MultiMatrixTest : public VintfObjectTestBase {
   protected:
    static std::string getFileName(size_t i) {
        return "compatibility_matrix." + std::to_string(static_cast<Level>(i)) + ".xml";
    }
    void SetUpMockSystemMatrices(const std::vector<std::string>& xmls) {
        EXPECT_CALL(fetcher(), listFiles(_, _, _))
            .Times(AnyNumber())
            .WillRepeatedly(Return(::android::OK));
        EXPECT_CALL(fetcher(), listFiles(StrEq(kSystemVintfDir), _, _))
            .WillRepeatedly(Invoke([=](const auto&, auto* out, auto*) {
                size_t i = 1;
                for (const auto& content : xmls) {
                    (void)content;
                    out->push_back(getFileName(i));
                    ++i;
                }
                return ::android::OK;
            }));
        size_t i = 1;
        for (const auto& content : xmls) {
            expectFetchRepeatedly(kSystemVintfDir + getFileName(i), content);
            ++i;
        }
        expectFileNotExist(kProductMatrix);
        expectNeverFetch(kSystemLegacyMatrix);
        expectFileNotExist(StartsWith("/odm/"));
    }
    void expectTargetFcmVersion(size_t level) {
        expectFetch(kVendorManifest, "<manifest version=\"1.0\" type=\"device\" target-level=\"" +
                                         to_string(static_cast<Level>(level)) + "\"/>");
        vintfObject->getDeviceHalManifest(true /* skipCache */);
    }
};

class RegexTest : public MultiMatrixTest {
   protected:
    virtual void SetUp() {
        MultiMatrixTest::SetUp();
        SetUpMockSystemMatrices(systemMatrixRegexXmls);
    }
};

TEST_F(RegexTest, CombineLevel1) {
    expectTargetFcmVersion(1);
    auto matrix = vintfObject->getFrameworkCompatibilityMatrix(true /* skipCache */);
    ASSERT_NE(nullptr, matrix);
    std::string xml = gCompatibilityMatrixConverter(*matrix);

    EXPECT_IN(
        "    <hal format=\"hidl\" optional=\"false\">\n"
        "        <name>android.hardware.regex</name>\n"
        "        <version>1.0-2</version>\n"
        "        <version>2.0</version>\n"
        "        <interface>\n"
        "            <name>IRegex</name>\n"
        "            <instance>default</instance>\n"
        "        </interface>\n"
        "    </hal>\n",
        xml);
    EXPECT_IN(
        "    <hal format=\"hidl\" optional=\"false\">\n"
        "        <name>android.hardware.regex</name>\n"
        "        <version>1.0-1</version>\n"
        "        <interface>\n"
        "            <name>IRegex</name>\n"
        "            <instance>special/1.0</instance>\n"
        "            <regex-instance>regex/1.0/[0-9]+</regex-instance>\n"
        "            <regex-instance>regex_common/[0-9]+</regex-instance>\n"
        "        </interface>\n"
        "    </hal>\n",
        xml);
    EXPECT_IN(
        "    <hal format=\"hidl\" optional=\"true\">\n"
        "        <name>android.hardware.regex</name>\n"
        "        <version>1.1-2</version>\n"
        "        <interface>\n"
        "            <name>IRegex</name>\n"
        "            <instance>special/1.1</instance>\n"
        "            <regex-instance>[a-z]+_[a-z]+/[0-9]+</regex-instance>\n"
        "            <regex-instance>regex/1.1/[0-9]+</regex-instance>\n"
        "        </interface>\n"
        "    </hal>\n",
        xml);
    EXPECT_IN(
        "    <hal format=\"hidl\" optional=\"true\">\n"
        "        <name>android.hardware.regex</name>\n"
        "        <version>2.0</version>\n"
        "        <interface>\n"
        "            <name>IRegex</name>\n"
        "            <instance>special/2.0</instance>\n"
        "            <regex-instance>regex/2.0/[0-9]+</regex-instance>\n"
        "            <regex-instance>regex_[a-z]+/[0-9]+</regex-instance>\n"
        "        </interface>\n"
        "    </hal>\n",
        xml);
}

TEST_F(RegexTest, CombineLevel2) {
    expectTargetFcmVersion(2);
    auto matrix = vintfObject->getFrameworkCompatibilityMatrix(true /* skipCache */);
    ASSERT_NE(nullptr, matrix);
    std::string xml = gCompatibilityMatrixConverter(*matrix);

    EXPECT_IN(
        "    <hal format=\"hidl\" optional=\"false\">\n"
        "        <name>android.hardware.regex</name>\n"
        "        <version>1.1-2</version>\n"
        "        <version>2.0</version>\n"
        "        <interface>\n"
        "            <name>IRegex</name>\n"
        "            <instance>default</instance>\n"
        "        </interface>\n"
        "    </hal>\n",
        xml);
    EXPECT_IN(
        "    <hal format=\"hidl\" optional=\"false\">\n"
        "        <name>android.hardware.regex</name>\n"
        "        <version>1.1-2</version>\n"
        "        <interface>\n"
        "            <name>IRegex</name>\n"
        "            <instance>special/1.1</instance>\n"
        "            <regex-instance>[a-z]+_[a-z]+/[0-9]+</regex-instance>\n"
        "            <regex-instance>regex/1.1/[0-9]+</regex-instance>\n"
        "        </interface>\n"
        "    </hal>\n",
        xml);
    EXPECT_IN(
        "    <hal format=\"hidl\" optional=\"true\">\n"
        "        <name>android.hardware.regex</name>\n"
        "        <version>2.0</version>\n"
        "        <interface>\n"
        "            <name>IRegex</name>\n"
        "            <instance>special/2.0</instance>\n"
        "            <regex-instance>regex/2.0/[0-9]+</regex-instance>\n"
        "            <regex-instance>regex_[a-z]+/[0-9]+</regex-instance>\n"
        "        </interface>\n"
        "    </hal>\n",
        xml);
}

TEST_F(RegexTest, DeprecateLevel2) {
    std::string error;
    expectTargetFcmVersion(2);

    auto pred = getInstanceListFunc({
        "android.hardware.regex@1.1::IRegex/default",
        "android.hardware.regex@1.1::IRegex/special/1.1",
        "android.hardware.regex@1.1::IRegex/regex/1.1/1",
        "android.hardware.regex@1.1::IRegex/regex_common/0",
        "android.hardware.regex@2.0::IRegex/default",
    });
    EXPECT_EQ(NO_DEPRECATED_HALS, vintfObject->checkDeprecation(pred, &error)) << error;

    for (const auto& deprecated : {
             "android.hardware.regex@1.0::IRegex/default",
             "android.hardware.regex@1.0::IRegex/special/1.0",
             "android.hardware.regex@1.0::IRegex/regex/1.0/1",
             "android.hardware.regex@1.0::IRegex/regex_common/0",
             "android.hardware.regex@1.1::IRegex/special/1.0",
             "android.hardware.regex@1.1::IRegex/regex/1.0/1",
         }) {
        // 2.0/default ensures compatibility.
        pred = getInstanceListFunc({
            deprecated,
            "android.hardware.regex@2.0::IRegex/default",
        });
        error.clear();
        EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
            << deprecated << " should be deprecated. " << error;
    }
}

TEST_F(RegexTest, DeprecateLevel3) {
    std::string error;
    expectTargetFcmVersion(3);

    auto pred = getInstanceListFunc({
        "android.hardware.regex@2.0::IRegex/special/2.0",
        "android.hardware.regex@2.0::IRegex/regex/2.0/1",
        "android.hardware.regex@2.0::IRegex/default",
    });
    EXPECT_EQ(NO_DEPRECATED_HALS, vintfObject->checkDeprecation(pred, &error)) << error;

    for (const auto& deprecated : {
             "android.hardware.regex@1.0::IRegex/default",
             "android.hardware.regex@1.0::IRegex/special/1.0",
             "android.hardware.regex@1.0::IRegex/regex/1.0/1",
             "android.hardware.regex@1.0::IRegex/regex_common/0",
             "android.hardware.regex@1.1::IRegex/special/1.0",
             "android.hardware.regex@1.1::IRegex/regex/1.0/1",
             "android.hardware.regex@1.1::IRegex/special/1.1",
             "android.hardware.regex@1.1::IRegex/regex/1.1/1",
             "android.hardware.regex@1.1::IRegex/regex_common/0",
         }) {
        // 2.0/default ensures compatibility.
        pred = getInstanceListFunc({
            deprecated,
            "android.hardware.regex@2.0::IRegex/default",
        });

        error.clear();
        EXPECT_EQ(DEPRECATED, vintfObject->checkDeprecation(pred, &error))
            << deprecated << " should be deprecated.";
    }
}

//
// Set of framework matrices of different FCM version with <kernel>.
//

#define FAKE_KERNEL(__version__, __key__)                   \
    "    <kernel version=\"" __version__ "\">\n"            \
    "        <config>\n"                                    \
    "            <key>CONFIG_" __key__ "</key>\n"           \
    "            <value type=\"tristate\">y</value>\n"      \
    "        </config>\n"                                   \
    "    </kernel>\n"

const static std::vector<std::string> systemMatrixKernelXmls = {
    // 1.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"1\">\n"
    FAKE_KERNEL("1.0.0", "A1")
    FAKE_KERNEL("2.0.0", "B1")
    "</compatibility-matrix>\n",
    // 2.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"2\">\n"
    FAKE_KERNEL("2.0.0", "B2")
    FAKE_KERNEL("3.0.0", "C2")
    FAKE_KERNEL("4.0.0", "D2")
    "</compatibility-matrix>\n",
    // 3.xml
    "<compatibility-matrix version=\"1.0\" type=\"framework\" level=\"3\">\n"
    FAKE_KERNEL("4.0.0", "D3")
    FAKE_KERNEL("5.0.0", "E3")
    "</compatibility-matrix>\n",
};

class KernelTest : public MultiMatrixTest {};

// Assume that we are developing level 2. Test that old <kernel> requirements should
// not change and new <kernel> versions are added.
TEST_F(KernelTest, Level1AndLevel2) {
    SetUpMockSystemMatrices({systemMatrixKernelXmls[0], systemMatrixKernelXmls[1]});

    expectTargetFcmVersion(1);
    auto matrix = vintfObject->getFrameworkCompatibilityMatrix(true /* skipCache */);
    ASSERT_NE(nullptr, matrix);
    std::string xml = gCompatibilityMatrixConverter(*matrix);

    EXPECT_IN(FAKE_KERNEL("1.0.0", "A1"), xml) << "\nOld requirements must not change.";
    EXPECT_IN(FAKE_KERNEL("2.0.0", "B1"), xml) << "\nOld requirements must not change.";
    EXPECT_IN(FAKE_KERNEL("3.0.0", "C2"), xml) << "\nShould see <kernel> from new matrices";
    EXPECT_IN(FAKE_KERNEL("4.0.0", "D2"), xml) << "\nShould see <kernel> from new matrices";

    EXPECT_NOT_IN(FAKE_KERNEL("2.0.0", "B2"), xml) << "\nOld requirements must not change";
}

// Assume that we are developing level 3. Test that old <kernel> requirements should
// not change and new <kernel> versions are added.
TEST_F(KernelTest, Level1AndMore) {
    SetUpMockSystemMatrices({systemMatrixKernelXmls});

    expectTargetFcmVersion(1);
    auto matrix = vintfObject->getFrameworkCompatibilityMatrix(true /* skipCache */);
    ASSERT_NE(nullptr, matrix);
    std::string xml = gCompatibilityMatrixConverter(*matrix);

    EXPECT_IN(FAKE_KERNEL("1.0.0", "A1"), xml) << "\nOld requirements must not change.";
    EXPECT_IN(FAKE_KERNEL("2.0.0", "B1"), xml) << "\nOld requirements must not change.";
    EXPECT_IN(FAKE_KERNEL("3.0.0", "C2"), xml) << "\nOld requirements must not change.";
    EXPECT_IN(FAKE_KERNEL("4.0.0", "D2"), xml) << "\nOld requirements must not change.";
    EXPECT_IN(FAKE_KERNEL("5.0.0", "E3"), xml) << "\nShould see <kernel> from new matrices";

    EXPECT_NOT_IN(FAKE_KERNEL("2.0.0", "B2"), xml) << "\nOld requirements must not change";
    EXPECT_NOT_IN(FAKE_KERNEL("4.0.0", "D3"), xml) << "\nOld requirements must not change";
}

class VintfObjectPartialUpdateTest : public MultiMatrixTest {
   protected:
    void SetUp() override {
        MultiMatrixTest::SetUp();
        setFakeProperties();
    }
};

TEST_F(VintfObjectPartialUpdateTest, DeviceCompatibility) {
    setupMockFetcher(vendorManifestRequire1, "", systemManifestXml1, vendorMatrixXml1,
                     productModel);
    SetUpMockSystemMatrices(systemMatrixRequire);

    expectSystemManifest();
    expectVendorMatrix();
    expectVendorManifest();

    std::string error;
    EXPECT_TRUE(checkCompatibility({}, &error)) << error;
}

TEST_F(VintfObjectPartialUpdateTest, VendorOnlyCompatible) {
    setupMockFetcher("", "", systemManifestXml1, vendorMatrixXml1, productModel);
    SetUpMockSystemMatrices(systemMatrixRequire);

    expectSystemManifest();
    expectVendorMatrix();
    // Should not load vendor manifest from device
    expectVendorManifest(0);

    std::string error;
    EXPECT_TRUE(checkCompatibility({vendorManifestRequire2}, &error)) << error;
}

const std::string systemEtcManifest =
    "<manifest version=\"1.0\" type=\"framework\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <fqname>@1.0::ISystemEtc/default</fqname>\n"
    "    </hal>\n"
    "</manifest>\n";

const std::string systemEtcManifestFrag =
    "<manifest version=\"1.0\" type=\"framework\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <fqname>@1.0::ISystemEtcFragment/default</fqname>\n"
    "    </hal>\n"
    "</manifest>\n";

const std::string productEtcManifest =
    "<manifest version=\"1.0\" type=\"framework\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <fqname>@1.0::IProductEtc/default</fqname>\n"
    "    </hal>\n"
    "</manifest>\n";

const std::string productEtcManifestFrag =
    "<manifest version=\"1.0\" type=\"framework\">\n"
    "    <hal format=\"hidl\">\n"
    "        <name>android.hardware.foo</name>\n"
    "        <transport>hwbinder</transport>\n"
    "        <fqname>@1.0::IProductEtcFragment/default</fqname>\n"
    "    </hal>\n"
    "</manifest>\n";

using FrameworkManifestTestParam =
    std::tuple<bool /* Existence of /system/etc/vintf/manifest.xml */,
               bool /* Existence of /system/etc/vintf/manifest/fragment.xml */,
               bool /* Existence of /product/etc/vintf/manifest.xml */,
               bool /* Existence of /product/etc/vintf/manifest/fragment.xml */>;
class FrameworkManifestTest : public VintfObjectTestBase,
                              public ::testing::WithParamInterface<FrameworkManifestTestParam> {
   protected:
    virtual void SetUp() {
        VintfObjectTestBase::SetUp();
        setFakeProperties();
    }

    // Set the existence of /system/etc/vintf/manifest.xml
    void expectSystemManifest(bool exists) {
        if (exists) {
            expectFetchRepeatedly(kSystemManifest, systemEtcManifest);
        } else {
            expectFileNotExist(StrEq(kSystemManifest));
        }
        expectFileNotExist(StrEq(kSystemLegacyManifest));
    }
    // Set the existence of /system/etc/vintf/manifest/fragment.xml
    void expectSystemManifestFragment(bool exists) {
        if (exists) {
            EXPECT_CALL(fetcher(), listFiles(StrEq(kSystemManifestFragmentDir), _, _))
                .Times(AnyNumber())
                .WillRepeatedly(Invoke([](const auto&, auto* out, auto*) {
                    *out = {"fragment.xml"};
                    return ::android::OK;
                }));
            expectFetchRepeatedly(kSystemManifestFragmentDir + "fragment.xml",
                                  systemEtcManifestFrag);
        } else {
            EXPECT_CALL(fetcher(), listFiles(StrEq(kSystemManifestFragmentDir), _, _))
                .Times(AnyNumber())
                .WillRepeatedly(Return(::android::OK));
            expectFileNotExist(kSystemManifestFragmentDir + "fragment.xml");
        }
    }
    // Set the existence of /product/etc/vintf/manifest.xml
    void expectProductManifest(bool exists) {
        if (exists) {
            expectFetchRepeatedly(kProductManifest, productEtcManifest);
        } else {
            expectFileNotExist(kProductManifest);
        }
    }
    // Set the existence of /product/etc/vintf/manifest/fragment.xml
    void expectProductManifestFragment(bool exists) {
        if (exists) {
            EXPECT_CALL(fetcher(), listFiles(StrEq(kProductManifestFragmentDir), _, _))
                .Times(AnyNumber())
                .WillRepeatedly(Invoke([](const auto&, auto* out, auto*) {
                    *out = {"fragment.xml"};
                    return ::android::OK;
                }));
            expectFetchRepeatedly(kProductManifestFragmentDir + "fragment.xml",
                                  productEtcManifestFrag);
        } else {
            EXPECT_CALL(fetcher(), listFiles(StrEq(kProductManifestFragmentDir), _, _))
                .Times(AnyNumber())
                .WillRepeatedly(Return(::android::OK));
            expectFileNotExist(kProductManifestFragmentDir + "fragment.xml");
        }
    }

    void expectContainsInterface(const std::string& interface, bool contains = true) {
        auto manifest = vintfObject->getFrameworkHalManifest();
        ASSERT_NE(nullptr, manifest);
        EXPECT_NE(manifest->getInstances("android.hardware.foo", {1, 0}, interface).empty(),
                  contains)
            << interface << " is missing.";
    }
};

TEST_P(FrameworkManifestTest, Existence) {
    expectSystemManifest(std::get<0>(GetParam()));
    expectSystemManifestFragment(std::get<1>(GetParam()));
    expectProductManifest(std::get<2>(GetParam()));
    expectProductManifestFragment(std::get<3>(GetParam()));

    if (!std::get<0>(GetParam())) {
        EXPECT_EQ(nullptr, vintfObject->getFrameworkHalManifest())
            << "getFrameworkHalManifest must return nullptr if " << kSystemManifest
            << " does not exist";
    } else {
        expectContainsInterface("ISystemEtc", std::get<0>(GetParam()));
        expectContainsInterface("ISystemEtcFragment", std::get<1>(GetParam()));
        expectContainsInterface("IProductEtc", std::get<2>(GetParam()));
        expectContainsInterface("IProductEtcFragment", std::get<3>(GetParam()));
    }
}
INSTANTIATE_TEST_SUITE_P(, FrameworkManifestTest,
                         ::testing::Combine(Bool(), Bool(), Bool(), Bool()));

}  // namespace testing
}  // namespace vintf
}  // namespace android

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
