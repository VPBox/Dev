// Copyright 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <cstdio>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include <base/command_line.h>
#include <base/files/file_util.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_split.h>
#include <base/strings/string_util.h>
#include <base/strings/utf_string_conversions.h>
#include <base/threading/platform_thread.h>
#include <keystore/keymaster_types.h>
#include <keystore/keystore_client_impl.h>

#include <android/hardware/confirmationui/1.0/types.h>
#include <android/security/BnConfirmationPromptCallback.h>
#include <android/security/keystore/IKeystoreService.h>

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>

//#include <keystore/keystore.h>

using base::CommandLine;
using keystore::KeystoreClient;

using android::sp;
using android::String16;
using android::security::keystore::IKeystoreService;
using base::CommandLine;
using ConfirmationResponseCode = android::hardware::confirmationui::V1_0::ResponseCode;

namespace {
using namespace keystore;

struct TestCase {
    std::string name;
    bool required_for_brillo_pts;
    AuthorizationSet parameters;
};

void PrintUsageAndExit() {
    printf("Usage: keystore_client_v2 <command> [options]\n");
    printf("Commands: brillo-platform-test [--prefix=<test_name_prefix>] [--test_for_0_3]\n"
           "          list-brillo-tests\n"
           "          add-entropy --input=<entropy> [--seclevel=software|strongbox|tee(default)]\n"
           "          generate --name=<key_name> [--seclevel=software|strongbox|tee(default)]\n"
           "          get-chars --name=<key_name>\n"
           "          export --name=<key_name>\n"
           "          delete --name=<key_name>\n"
           "          delete-all\n"
           "          exists --name=<key_name>\n"
           "          list [--prefix=<key_name_prefix>]\n"
           "          list-apps-with-keys\n"
           "          sign-verify --name=<key_name>\n"
           "          [en|de]crypt --name=<key_name> --in=<file> --out=<file>\n"
           "                       [--seclevel=software|strongbox|tee(default)]\n"
           "          confirmation --prompt_text=<PromptText> --extra_data=<hex>\n"
           "                       --locale=<locale> [--ui_options=<list_of_ints>]\n"
           "                       --cancel_after=<seconds>\n");
    exit(1);
}

std::unique_ptr<KeystoreClient> CreateKeystoreInstance() {
    return std::unique_ptr<KeystoreClient>(
        static_cast<KeystoreClient*>(new keystore::KeystoreClientImpl));
}

void PrintTags(const AuthorizationSet& parameters) {
    for (auto iter = parameters.begin(); iter != parameters.end(); ++iter) {
        auto tag_str = toString(iter->tag);
        printf("  %s\n", tag_str.c_str());
    }
}

void PrintKeyCharacteristics(const AuthorizationSet& hardware_enforced_characteristics,
                             const AuthorizationSet& software_enforced_characteristics) {
    printf("Hardware:\n");
    PrintTags(hardware_enforced_characteristics);
    printf("Software:\n");
    PrintTags(software_enforced_characteristics);
}

bool TestKey(const std::string& name, bool required, const AuthorizationSet& parameters) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    AuthorizationSet hardware_enforced_characteristics;
    AuthorizationSet software_enforced_characteristics;
    auto result =
        keystore->generateKey("tmp", parameters, 0 /*flags*/, &hardware_enforced_characteristics,
                              &software_enforced_characteristics);
    const char kBoldRedAbort[] = "\033[1;31mABORT\033[0m";
    if (!result.isOk()) {
        LOG(ERROR) << "Failed to generate key: " << result;
        printf("[%s] %s\n", kBoldRedAbort, name.c_str());
        return false;
    }
    result = keystore->deleteKey("tmp");
    if (!result.isOk()) {
        LOG(ERROR) << "Failed to delete key: " << result;
        printf("[%s] %s\n", kBoldRedAbort, name.c_str());
        return false;
    }
    printf("===============================================================\n");
    printf("%s Key Characteristics:\n", name.c_str());
    PrintKeyCharacteristics(hardware_enforced_characteristics, software_enforced_characteristics);
    bool hardware_backed = (hardware_enforced_characteristics.size() > 0);
    if (software_enforced_characteristics.GetTagCount(TAG_ALGORITHM) > 0 ||
        software_enforced_characteristics.GetTagCount(TAG_KEY_SIZE) > 0 ||
        software_enforced_characteristics.GetTagCount(TAG_RSA_PUBLIC_EXPONENT) > 0) {
        VLOG(1) << "Hardware-backed key but required characteristics enforced in software.";
        hardware_backed = false;
    }
    const char kBoldRedFail[] = "\033[1;31mFAIL\033[0m";
    const char kBoldGreenPass[] = "\033[1;32mPASS\033[0m";
    const char kBoldYellowWarn[] = "\033[1;33mWARN\033[0m";
    printf("[%s] %s\n",
           hardware_backed ? kBoldGreenPass : (required ? kBoldRedFail : kBoldYellowWarn),
           name.c_str());

    return (hardware_backed || !required);
}

AuthorizationSet GetRSASignParameters(uint32_t key_size, bool sha256_only) {
    AuthorizationSetBuilder parameters;
    parameters.RsaSigningKey(key_size, 65537)
        .Digest(Digest::SHA_2_256)
        .Padding(PaddingMode::RSA_PKCS1_1_5_SIGN)
        .Padding(PaddingMode::RSA_PSS)
        .Authorization(TAG_NO_AUTH_REQUIRED);
    if (!sha256_only) {
        parameters.Digest(Digest::SHA_2_224).Digest(Digest::SHA_2_384).Digest(Digest::SHA_2_512);
    }
    return std::move(parameters);
}

AuthorizationSet GetRSAEncryptParameters(uint32_t key_size) {
    AuthorizationSetBuilder parameters;
    parameters.RsaEncryptionKey(key_size, 65537)
        .Padding(PaddingMode::RSA_PKCS1_1_5_ENCRYPT)
        .Padding(PaddingMode::RSA_OAEP)
        .Authorization(TAG_NO_AUTH_REQUIRED);
    return std::move(parameters);
}

AuthorizationSet GetECDSAParameters(uint32_t key_size, bool sha256_only) {
    AuthorizationSetBuilder parameters;
    parameters.EcdsaSigningKey(key_size)
        .Digest(Digest::SHA_2_256)
        .Authorization(TAG_NO_AUTH_REQUIRED);
    if (!sha256_only) {
        parameters.Digest(Digest::SHA_2_224).Digest(Digest::SHA_2_384).Digest(Digest::SHA_2_512);
    }
    return std::move(parameters);
}

AuthorizationSet GetAESParameters(uint32_t key_size, bool with_gcm_mode) {
    AuthorizationSetBuilder parameters;
    parameters.AesEncryptionKey(key_size).Authorization(TAG_NO_AUTH_REQUIRED);
    if (with_gcm_mode) {
        parameters.Authorization(TAG_BLOCK_MODE, BlockMode::GCM)
            .Authorization(TAG_MIN_MAC_LENGTH, 128);
    } else {
        parameters.Authorization(TAG_BLOCK_MODE, BlockMode::ECB);
        parameters.Authorization(TAG_BLOCK_MODE, BlockMode::CBC);
        parameters.Authorization(TAG_BLOCK_MODE, BlockMode::CTR);
        parameters.Padding(PaddingMode::NONE);
    }
    return std::move(parameters);
}

AuthorizationSet GetHMACParameters(uint32_t key_size, Digest digest) {
    AuthorizationSetBuilder parameters;
    parameters.HmacKey(key_size)
        .Digest(digest)
        .Authorization(TAG_MIN_MAC_LENGTH, 224)
        .Authorization(TAG_NO_AUTH_REQUIRED);
    return std::move(parameters);
}

std::vector<TestCase> GetTestCases() {
    TestCase test_cases[] = {
        {"RSA-2048 Sign", true, GetRSASignParameters(2048, true)},
        {"RSA-2048 Sign (more digests)", false, GetRSASignParameters(2048, false)},
        {"RSA-3072 Sign", false, GetRSASignParameters(3072, false)},
        {"RSA-4096 Sign", false, GetRSASignParameters(4096, false)},
        {"RSA-2048 Encrypt", true, GetRSAEncryptParameters(2048)},
        {"RSA-3072 Encrypt", false, GetRSAEncryptParameters(3072)},
        {"RSA-4096 Encrypt", false, GetRSAEncryptParameters(4096)},
        {"ECDSA-P256 Sign", true, GetECDSAParameters(256, true)},
        {"ECDSA-P256 Sign (more digests)", false, GetECDSAParameters(256, false)},
        {"ECDSA-P224 Sign", false, GetECDSAParameters(224, false)},
        {"ECDSA-P384 Sign", false, GetECDSAParameters(384, false)},
        {"ECDSA-P521 Sign", false, GetECDSAParameters(521, false)},
        {"AES-128", true, GetAESParameters(128, false)},
        {"AES-256", true, GetAESParameters(256, false)},
        {"AES-128-GCM", false, GetAESParameters(128, true)},
        {"AES-256-GCM", false, GetAESParameters(256, true)},
        {"HMAC-SHA256-16", true, GetHMACParameters(16, Digest::SHA_2_256)},
        {"HMAC-SHA256-32", true, GetHMACParameters(32, Digest::SHA_2_256)},
        {"HMAC-SHA256-64", false, GetHMACParameters(64, Digest::SHA_2_256)},
        {"HMAC-SHA224-32", false, GetHMACParameters(32, Digest::SHA_2_224)},
        {"HMAC-SHA384-32", false, GetHMACParameters(32, Digest::SHA_2_384)},
        {"HMAC-SHA512-32", false, GetHMACParameters(32, Digest::SHA_2_512)},
    };
    return std::vector<TestCase>(&test_cases[0], &test_cases[arraysize(test_cases)]);
}

int BrilloPlatformTest(const std::string& prefix, bool test_for_0_3) {
    const char kBoldYellowWarning[] = "\033[1;33mWARNING\033[0m";
    if (test_for_0_3) {
        printf("%s: Testing for keymaster v0.3. "
               "This does not meet Brillo requirements.\n",
               kBoldYellowWarning);
    }
    int test_count = 0;
    int fail_count = 0;
    std::vector<TestCase> test_cases = GetTestCases();
    for (const auto& test_case : test_cases) {
        if (!prefix.empty() &&
            !base::StartsWith(test_case.name, prefix, base::CompareCase::SENSITIVE)) {
            continue;
        }
        if (test_for_0_3 &&
            (base::StartsWith(test_case.name, "AES", base::CompareCase::SENSITIVE) ||
             base::StartsWith(test_case.name, "HMAC", base::CompareCase::SENSITIVE))) {
            continue;
        }
        ++test_count;
        if (!TestKey(test_case.name, test_case.required_for_brillo_pts, test_case.parameters)) {
            VLOG(1) << "Test failed: " << test_case.name;
            ++fail_count;
        }
    }
    return fail_count;
}

int ListTestCases() {
    const char kBoldGreenRequired[] = "\033[1;32mREQUIRED\033[0m";
    const char kBoldYellowRecommended[] = "\033[1;33mRECOMMENDED\033[0m";
    std::vector<TestCase> test_cases = GetTestCases();
    for (const auto& test_case : test_cases) {
        printf("%s : %s\n", test_case.name.c_str(),
               test_case.required_for_brillo_pts ? kBoldGreenRequired : kBoldYellowRecommended);
    }
    return 0;
}

std::string ReadFile(const std::string& filename) {
    std::string content;
    base::FilePath path(filename);
    if (!base::ReadFileToString(path, &content)) {
        printf("Failed to read file: %s\n", filename.c_str());
        exit(1);
    }
    return content;
}

void WriteFile(const std::string& filename, const std::string& content) {
    base::FilePath path(filename);
    int size = content.size();
    if (base::WriteFile(path, content.data(), size) != size) {
        printf("Failed to write file: %s\n", filename.c_str());
        exit(1);
    }
}

int AddEntropy(const std::string& input, int32_t flags) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    int32_t result = keystore->addRandomNumberGeneratorEntropy(input, flags).getErrorCode();
    printf("AddEntropy: %d\n", result);
    return result;
}

// Note: auth_bound keys created with this tool will not be usable.
int GenerateKey(const std::string& name, int32_t flags, bool auth_bound) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    AuthorizationSetBuilder params;
    params.RsaSigningKey(2048, 65537)
        .Digest(Digest::SHA_2_224)
        .Digest(Digest::SHA_2_256)
        .Digest(Digest::SHA_2_384)
        .Digest(Digest::SHA_2_512)
        .Padding(PaddingMode::RSA_PKCS1_1_5_SIGN)
        .Padding(PaddingMode::RSA_PSS);
    if (auth_bound) {
        // Gatekeeper normally generates the secure user id.
        // Using zero allows the key to be created, but it will not be usuable.
        params.Authorization(TAG_USER_SECURE_ID, 0);
    } else {
        params.Authorization(TAG_NO_AUTH_REQUIRED);
    }
    AuthorizationSet hardware_enforced_characteristics;
    AuthorizationSet software_enforced_characteristics;
    auto result = keystore->generateKey(name, params, flags, &hardware_enforced_characteristics,
                                        &software_enforced_characteristics);
    printf("GenerateKey: %d\n", result.getErrorCode());
    if (result.isOk()) {
        PrintKeyCharacteristics(hardware_enforced_characteristics,
                                software_enforced_characteristics);
    }
    return result.getErrorCode();
}

int GetCharacteristics(const std::string& name) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    AuthorizationSet hardware_enforced_characteristics;
    AuthorizationSet software_enforced_characteristics;
    auto result = keystore->getKeyCharacteristics(name, &hardware_enforced_characteristics,
                                                  &software_enforced_characteristics);
    printf("GetCharacteristics: %d\n", result.getErrorCode());
    if (result.isOk()) {
        PrintKeyCharacteristics(hardware_enforced_characteristics,
                                software_enforced_characteristics);
    }
    return result.getErrorCode();
}

int ExportKey(const std::string& name) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    std::string data;
    int32_t result = keystore->exportKey(KeyFormat::X509, name, &data).getErrorCode();
    printf("ExportKey: %d (%zu)\n", result, data.size());
    return result;
}

int DeleteKey(const std::string& name) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    int32_t result = keystore->deleteKey(name).getErrorCode();
    printf("DeleteKey: %d\n", result);
    return result;
}

int DeleteAllKeys() {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    int32_t result = keystore->deleteAllKeys().getErrorCode();
    printf("DeleteAllKeys: %d\n", result);
    return result;
}

int DoesKeyExist(const std::string& name) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    printf("DoesKeyExist: %s\n", keystore->doesKeyExist(name) ? "yes" : "no");
    return 0;
}

int List(const std::string& prefix) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    std::vector<std::string> key_list;
    if (!keystore->listKeys(prefix, &key_list)) {
        printf("ListKeys failed.\n");
        return 1;
    }
    printf("Keys:\n");
    for (const auto& key_name : key_list) {
        printf("  %s\n", key_name.c_str());
    }
    return 0;
}

int ListAppsWithKeys() {

    sp<android::IServiceManager> sm = android::initdefaultServiceManager();
    sp<android::IBinder> binder = sm->getService(String16("android.security.keystore"));
    sp<IKeystoreService> service = android::interface_cast<IKeystoreService>(binder);
    if (service == nullptr) {
        fprintf(stderr, "Error connecting to keystore service.\n");
        return 1;
    }
    int32_t aidl_return;
    ::std::vector<::std::string> uids;
    android::binder::Status status = service->listUidsOfAuthBoundKeys(&uids, &aidl_return);
    if (!status.isOk()) {
        fprintf(stderr, "Requesting uids of auth bound keys failed with error %s.\n",
                status.toString8().c_str());
        return 1;
    }
    if (!KeyStoreNativeReturnCode(aidl_return).isOk()) {
        fprintf(stderr, "Requesting uids of auth bound keys failed with code %d.\n", aidl_return);
        return 1;
    }
    printf("Apps with auth bound keys:\n");
    for (auto i = uids.begin(); i != uids.end(); ++i) {
        printf("%s\n", i->c_str());
    }
    return 0;
}

int SignAndVerify(const std::string& name) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    AuthorizationSetBuilder sign_params;
    sign_params.Padding(PaddingMode::RSA_PKCS1_1_5_SIGN);
    sign_params.Digest(Digest::SHA_2_256);
    AuthorizationSet output_params;
    uint64_t handle;
    auto result =
        keystore->beginOperation(KeyPurpose::SIGN, name, sign_params, &output_params, &handle);
    if (!result.isOk()) {
        printf("Sign: BeginOperation failed: %d\n", result.getErrorCode());
        return result.getErrorCode();
    }
    AuthorizationSet empty_params;
    size_t num_input_bytes_consumed;
    std::string output_data;
    result = keystore->updateOperation(handle, empty_params, "data_to_sign",
                                       &num_input_bytes_consumed, &output_params, &output_data);
    if (!result.isOk()) {
        printf("Sign: UpdateOperation failed: %d\n", result.getErrorCode());
        return result.getErrorCode();
    }
    result = keystore->finishOperation(handle, empty_params, std::string() /*signature_to_verify*/,
                                       &output_params, &output_data);
    if (!result.isOk()) {
        printf("Sign: FinishOperation failed: %d\n", result.getErrorCode());
        return result.getErrorCode();
    }
    printf("Sign: %zu bytes.\n", output_data.size());
    // We have a signature, now verify it.
    std::string signature_to_verify = output_data;
    output_data.clear();
    result =
        keystore->beginOperation(KeyPurpose::VERIFY, name, sign_params, &output_params, &handle);
    if (!result.isOk()) {
        printf("Verify: BeginOperation failed: %d\n", result.getErrorCode());
        return result.getErrorCode();
    }
    result = keystore->updateOperation(handle, empty_params, "data_to_sign",
                                       &num_input_bytes_consumed, &output_params, &output_data);
    if (!result.isOk()) {
        printf("Verify: UpdateOperation failed: %d\n", result.getErrorCode());
        return result.getErrorCode();
    }
    result = keystore->finishOperation(handle, empty_params, signature_to_verify, &output_params,
                                       &output_data);
    if (result == ErrorCode::VERIFICATION_FAILED) {
        printf("Verify: Failed to verify signature.\n");
        return result.getErrorCode();
    }
    if (!result.isOk()) {
        printf("Verify: FinishOperation failed: %d\n", result.getErrorCode());
        return result.getErrorCode();
    }
    printf("Verify: OK\n");
    return 0;
}

int Encrypt(const std::string& key_name, const std::string& input_filename,
            const std::string& output_filename, int32_t flags) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    std::string input = ReadFile(input_filename);
    std::string output;
    if (!keystore->encryptWithAuthentication(key_name, input, flags, &output)) {
        printf("EncryptWithAuthentication failed.\n");
        return 1;
    }
    WriteFile(output_filename, output);
    return 0;
}

int Decrypt(const std::string& key_name, const std::string& input_filename,
            const std::string& output_filename) {
    std::unique_ptr<KeystoreClient> keystore = CreateKeystoreInstance();
    std::string input = ReadFile(input_filename);
    std::string output;
    if (!keystore->decryptWithAuthentication(key_name, input, &output)) {
        printf("DecryptWithAuthentication failed.\n");
        return 1;
    }
    WriteFile(output_filename, output);
    return 0;
}

uint32_t securityLevelOption2Flags(const CommandLine& cmd) {
    if (cmd.HasSwitch("seclevel")) {
        auto str = cmd.GetSwitchValueASCII("seclevel");
        if (str == "strongbox") {
            return KEYSTORE_FLAG_STRONGBOX;
        } else if (str == "software") {
            return KEYSTORE_FLAG_FALLBACK;
        }
    }
    return KEYSTORE_FLAG_NONE;
}

class ConfirmationListener
    : public android::security::BnConfirmationPromptCallback,
      public std::promise<std::tuple<ConfirmationResponseCode, std::vector<uint8_t>>> {
  public:
    ConfirmationListener() {}

    virtual ::android::binder::Status
    onConfirmationPromptCompleted(int32_t result,
                                  const ::std::vector<uint8_t>& dataThatWasConfirmed) override {
        this->set_value({static_cast<ConfirmationResponseCode>(result), dataThatWasConfirmed});
        return ::android::binder::Status::ok();
    }
};

int Confirmation(const std::string& promptText, const std::string& extraDataHex,
                 const std::string& locale, const std::string& uiOptionsStr,
                 const std::string& cancelAfter) {
    sp<android::IServiceManager> sm = android::initdefaultServiceManager();
    sp<android::IBinder> binder = sm->getService(String16("android.security.keystore"));
    sp<IKeystoreService> service = android::interface_cast<IKeystoreService>(binder);
    if (service == nullptr) {
        printf("error: could not connect to keystore service.\n");
        return 1;
    }

    if (promptText.size() == 0) {
        printf("The --prompt_text parameter cannot be empty.\n");
        return 1;
    }

    std::vector<uint8_t> extraData;
    if (!base::HexStringToBytes(extraDataHex, &extraData)) {
        printf("The --extra_data parameter does not appear to be valid hexadecimal.\n");
        return 1;
    }

    std::vector<std::string> pieces =
        base::SplitString(uiOptionsStr, ",", base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
    int uiOptionsAsFlags = 0;
    for (auto& p : pieces) {
        int value;
        if (!base::StringToInt(p, &value)) {
            printf("Error parsing %s in --ui_options parameter as a number.\n", p.c_str());
            return 1;
        }
        uiOptionsAsFlags |= (1 << value);
    }

    double cancelAfterValue = 0.0;

    if (cancelAfter.size() > 0 && !base::StringToDouble(cancelAfter, &cancelAfterValue)) {
        printf("Error parsing %s in --cancel_after parameter as a double.\n", cancelAfter.c_str());
        return 1;
    }

    String16 promptText16(promptText.data(), promptText.size());
    String16 locale16(locale.data(), locale.size());

    sp<ConfirmationListener> listener = new ConfirmationListener();

    auto future = listener->get_future();
    int32_t aidl_return;
    android::binder::Status status = service->presentConfirmationPrompt(
        listener, promptText16, extraData, locale16, uiOptionsAsFlags, &aidl_return);
    if (!status.isOk()) {
        printf("Presenting confirmation prompt failed with binder status '%s'.\n",
               status.toString8().c_str());
        return 1;
    }
    ConfirmationResponseCode responseCode = static_cast<ConfirmationResponseCode>(aidl_return);
    if (responseCode != ConfirmationResponseCode::OK) {
        printf("Presenting confirmation prompt failed with response code %d.\n", responseCode);
        return 1;
    }
    printf("Waiting for prompt to complete - use Ctrl+C to abort...\n");

    if (cancelAfterValue > 0.0) {
        printf("Sleeping %.1f seconds before canceling prompt...\n", cancelAfterValue);
        auto fstatus =
            future.wait_for(std::chrono::milliseconds(uint64_t(cancelAfterValue * 1000)));
        if (fstatus == std::future_status::timeout) {
            status = service->cancelConfirmationPrompt(listener, &aidl_return);
            if (!status.isOk()) {
                printf("Canceling confirmation prompt failed with binder status '%s'.\n",
                       status.toString8().c_str());
                return 1;
            }
            responseCode = static_cast<ConfirmationResponseCode>(aidl_return);
            if (responseCode == ConfirmationResponseCode::Ignored) {
                // The confirmation was completed by the user so take the response
            } else if (responseCode != ConfirmationResponseCode::OK) {
                printf("Canceling confirmation prompt failed with response code %d.\n",
                       responseCode);
                return 1;
            }
        }
    }

    future.wait();

    auto [rc, dataThatWasConfirmed] = future.get();

    printf("Confirmation prompt completed\n"
           "responseCode = %d\n",
           rc);
    printf("dataThatWasConfirmed[%zd] = {", dataThatWasConfirmed.size());
    size_t newLineCountDown = 16;
    bool hasPrinted = false;
    for (uint8_t element : dataThatWasConfirmed) {
        if (hasPrinted) {
            printf(", ");
        }
        if (newLineCountDown == 0) {
            printf("\n  ");
            newLineCountDown = 32;
        }
        printf("0x%02x", element);
        hasPrinted = true;
    }
    printf("}\n");

    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    CommandLine::Init(argc, argv);
    CommandLine* command_line = CommandLine::ForCurrentProcess();
    CommandLine::StringVector args = command_line->GetArgs();

    android::ProcessState::self()->startThreadPool();

    if (args.empty()) {
        PrintUsageAndExit();
    }
    if (args[0] == "brillo-platform-test") {
        return BrilloPlatformTest(command_line->GetSwitchValueASCII("prefix"),
                                  command_line->HasSwitch("test_for_0_3"));
    } else if (args[0] == "list-brillo-tests") {
        return ListTestCases();
    } else if (args[0] == "add-entropy") {
        return AddEntropy(command_line->GetSwitchValueASCII("input"),
                          securityLevelOption2Flags(*command_line));
    } else if (args[0] == "generate") {
        return GenerateKey(command_line->GetSwitchValueASCII("name"),
                           securityLevelOption2Flags(*command_line),
                           command_line->HasSwitch("auth_bound"));
    } else if (args[0] == "get-chars") {
        return GetCharacteristics(command_line->GetSwitchValueASCII("name"));
    } else if (args[0] == "export") {
        return ExportKey(command_line->GetSwitchValueASCII("name"));
    } else if (args[0] == "delete") {
        return DeleteKey(command_line->GetSwitchValueASCII("name"));
    } else if (args[0] == "delete-all") {
        return DeleteAllKeys();
    } else if (args[0] == "exists") {
        return DoesKeyExist(command_line->GetSwitchValueASCII("name"));
    } else if (args[0] == "list") {
        return List(command_line->GetSwitchValueASCII("prefix"));
    } else if (args[0] == "list-apps-with-keys") {
        return ListAppsWithKeys();
    } else if (args[0] == "sign-verify") {
        return SignAndVerify(command_line->GetSwitchValueASCII("name"));
    } else if (args[0] == "encrypt") {
        return Encrypt(
            command_line->GetSwitchValueASCII("name"), command_line->GetSwitchValueASCII("in"),
            command_line->GetSwitchValueASCII("out"), securityLevelOption2Flags(*command_line));
    } else if (args[0] == "decrypt") {
        return Decrypt(command_line->GetSwitchValueASCII("name"),
                       command_line->GetSwitchValueASCII("in"),
                       command_line->GetSwitchValueASCII("out"));
    } else if (args[0] == "confirmation") {
        return Confirmation(command_line->GetSwitchValueNative("prompt_text"),
                            command_line->GetSwitchValueASCII("extra_data"),
                            command_line->GetSwitchValueASCII("locale"),
                            command_line->GetSwitchValueASCII("ui_options"),
                            command_line->GetSwitchValueASCII("cancel_after"));
    } else {
        PrintUsageAndExit();
    }
    return 0;
}
