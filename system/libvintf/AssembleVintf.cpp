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

#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <android-base/file.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>

#include <vintf/AssembleVintf.h>
#include <vintf/KernelConfigParser.h>
#include <vintf/parse_string.h>
#include <vintf/parse_xml.h>
#include "utils.h"

#define BUFFER_SIZE sysconf(_SC_PAGESIZE)

namespace android {
namespace vintf {

static const std::string gConfigPrefix = "android-base-";
static const std::string gConfigSuffix = ".config";
static const std::string gBaseConfig = "android-base.config";

// An input stream with a name.
// The input stream may be an actual file, or a stringstream for testing.
// It takes ownership on the istream.
class NamedIstream {
   public:
    NamedIstream(const std::string& name, std::unique_ptr<std::istream>&& stream)
        : mName(name), mStream(std::move(stream)) {}
    const std::string& name() const { return mName; }
    std::istream& stream() { return *mStream; }

   private:
    std::string mName;
    std::unique_ptr<std::istream> mStream;
};

/**
 * Slurps the device manifest file and add build time flag to it.
 */
class AssembleVintfImpl : public AssembleVintf {
    using Condition = std::unique_ptr<KernelConfig>;
    using ConditionedConfig = std::pair<Condition, std::vector<KernelConfig> /* configs */>;

   public:
    void setFakeEnv(const std::string& key, const std::string& value) { mFakeEnv[key] = value; }

    std::string getEnv(const std::string& key) const {
        auto it = mFakeEnv.find(key);
        if (it != mFakeEnv.end()) {
            return it->second;
        }
        const char* envValue = getenv(key.c_str());
        return envValue != nullptr ? std::string(envValue) : std::string();
    }

    // Get environment variable and split with space.
    std::vector<std::string> getEnvList(const std::string& key) const {
        std::vector<std::string> ret;
        for (auto&& v : base::Split(getEnv(key), " ")) {
            v = base::Trim(v);
            if (!v.empty()) {
                ret.push_back(v);
            }
        }
        return ret;
    }

    template <typename T>
    bool getFlag(const std::string& key, T* value, bool log = true) const {
        std::string envValue = getEnv(key);
        if (envValue.empty()) {
            if (log) {
                std::cerr << "Warning: " << key << " is missing, defaulted to " << (*value) << "."
                          << std::endl;
            }
            return true;
        }

        if (!parse(envValue, value)) {
            std::cerr << "Cannot parse " << envValue << "." << std::endl;
            return false;
        }
        return true;
    }

    /**
     * Set *out to environment variable only if *out is a dummy value (i.e. default constructed).
     * Return true if *out is set to environment variable, otherwise false.
     */
    template <typename T>
    bool getFlagIfUnset(const std::string& envKey, T* out, bool log = true) const {
        bool hasExistingValue = !(*out == T{});

        bool hasEnvValue = false;
        T envValue;
        std::string envStrValue = getEnv(envKey);
        if (!envStrValue.empty()) {
            if (!parse(envStrValue, &envValue)) {
                if (log) {
                    std::cerr << "Cannot parse " << envValue << "." << std::endl;
                }
                return false;
            }
            hasEnvValue = true;
        }

        if (hasExistingValue) {
            if (hasEnvValue && log) {
                std::cerr << "Warning: cannot override existing value " << *out << " with "
                          << envKey << " (which is " << envValue << ")." << std::endl;
            }
            return false;
        }
        if (!hasEnvValue) {
            if (log) {
                std::cerr << "Warning: " << envKey << " is not specified. Default to " << T{} << "."
                          << std::endl;
            }
            return false;
        }
        *out = envValue;
        return true;
    }

    bool getBooleanFlag(const std::string& key) const { return getEnv(key) == std::string("true"); }

    size_t getIntegerFlag(const std::string& key, size_t defaultValue = 0) const {
        std::string envValue = getEnv(key);
        if (envValue.empty()) {
            return defaultValue;
        }
        size_t value;
        if (!base::ParseUint(envValue, &value)) {
            std::cerr << "Error: " << key << " must be a number." << std::endl;
            return defaultValue;
        }
        return value;
    }

    static std::string read(std::basic_istream<char>& is) {
        std::stringstream ss;
        ss << is.rdbuf();
        return ss.str();
    }

    static bool isCommonConfig(const std::string& path) {
        return ::android::base::Basename(path) == gBaseConfig;
    }

    // nullptr on any error, otherwise the condition.
    static Condition generateCondition(const std::string& path) {
        std::string fname = ::android::base::Basename(path);
        if (fname.size() <= gConfigPrefix.size() + gConfigSuffix.size() ||
            !std::equal(gConfigPrefix.begin(), gConfigPrefix.end(), fname.begin()) ||
            !std::equal(gConfigSuffix.rbegin(), gConfigSuffix.rend(), fname.rbegin())) {
            return nullptr;
        }

        std::string sub = fname.substr(gConfigPrefix.size(),
                                       fname.size() - gConfigPrefix.size() - gConfigSuffix.size());
        if (sub.empty()) {
            return nullptr;  // should not happen
        }
        for (size_t i = 0; i < sub.size(); ++i) {
            if (sub[i] == '-') {
                sub[i] = '_';
                continue;
            }
            if (isalnum(sub[i])) {
                sub[i] = toupper(sub[i]);
                continue;
            }
            std::cerr << "'" << fname << "' (in " << path
                      << ") is not a valid kernel config file name. Must match regex: "
                      << "android-base(-[0-9a-zA-Z-]+)?\\" << gConfigSuffix
                      << std::endl;
            return nullptr;
        }
        sub.insert(0, "CONFIG_");
        return std::make_unique<KernelConfig>(std::move(sub), Tristate::YES);
    }

    static bool parseFileForKernelConfigs(std::basic_istream<char>& stream,
                                          std::vector<KernelConfig>* out) {
        KernelConfigParser parser(true /* processComments */, true /* relaxedFormat */);
        status_t err = parser.processAndFinish(read(stream));
        if (err != OK) {
            std::cerr << parser.error();
            return false;
        }

        for (auto& configPair : parser.configs()) {
            out->push_back({});
            KernelConfig& config = out->back();
            config.first = std::move(configPair.first);
            if (!parseKernelConfigTypedValue(configPair.second, &config.second)) {
                std::cerr << "Unknown value type for key = '" << config.first << "', value = '"
                          << configPair.second << "'\n";
                return false;
            }
        }
        return true;
    }

    static bool parseFilesForKernelConfigs(std::vector<NamedIstream>* streams,
                                           std::vector<ConditionedConfig>* out) {
        out->clear();
        ConditionedConfig commonConfig;
        bool foundCommonConfig = false;
        bool ret = true;

        for (auto& namedStream : *streams) {
            if (isCommonConfig(namedStream.name())) {
                ret &= parseFileForKernelConfigs(namedStream.stream(), &commonConfig.second);
                foundCommonConfig = true;
            } else {
                Condition condition = generateCondition(namedStream.name());
                ret &= (condition != nullptr);

                std::vector<KernelConfig> kernelConfigs;
                if ((ret &= parseFileForKernelConfigs(namedStream.stream(), &kernelConfigs)))
                    out->emplace_back(std::move(condition), std::move(kernelConfigs));
            }
        }

        if (!foundCommonConfig) {
            std::cerr << "No " << gBaseConfig << " is found in these paths:" << std::endl;
            for (auto& namedStream : *streams) {
                std::cerr << "    " << namedStream.name() << std::endl;
            }
        }
        ret &= foundCommonConfig;
        // first element is always common configs (no conditions).
        out->insert(out->begin(), std::move(commonConfig));
        return ret;
    }

    std::basic_ostream<char>& out() const { return mOutRef == nullptr ? std::cout : *mOutRef; }

    // If -c is provided, check it.
    bool checkDualFile(const HalManifest& manifest, const CompatibilityMatrix& matrix) {
        if (getBooleanFlag("PRODUCT_ENFORCE_VINTF_MANIFEST")) {
            std::string error;
            if (!manifest.checkCompatibility(matrix, &error)) {
                std::cerr << "Not compatible: " << error << std::endl;
                return false;
            }
        }

        // Check HALs in device manifest that are not in framework matrix.
        if (getBooleanFlag("VINTF_ENFORCE_NO_UNUSED_HALS")) {
            auto unused = manifest.checkUnusedHals(matrix);
            if (!unused.empty()) {
                std::cerr << "Error: The following instances are in the device manifest but "
                          << "not specified in framework compatibility matrix: " << std::endl
                          << "    " << android::base::Join(unused, "\n    ") << std::endl
                          << "Suggested fix:" << std::endl
                          << "1. Check for any typos in device manifest or framework compatibility "
                          << "matrices with FCM version >= " << matrix.level() << "." << std::endl
                          << "2. Add them to any framework compatibility matrix with FCM "
                          << "version >= " << matrix.level() << " where applicable." << std::endl
                          << "3. Add them to DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE "
                          << "or DEVICE_PRODUCT_COMPATIBILITY_MATRIX_FILE." << std::endl;

                return false;
            }
        }
        return true;
    }

    template <typename S>
    using Schemas = std::vector<Named<S>>;
    using HalManifests = Schemas<HalManifest>;
    using CompatibilityMatrices = Schemas<CompatibilityMatrix>;

    template <typename M>
    void outputInputs(const Schemas<M>& inputs) {
        out() << "<!--" << std::endl;
        out() << "    Input:" << std::endl;
        for (const auto& e : inputs) {
            if (!e.name.empty()) {
                out() << "        " << base::Basename(e.name) << std::endl;
            }
        }
        out() << "-->" << std::endl;
    }

    // Parse --kernel arguments and write to output manifest.
    bool setDeviceManifestKernel(HalManifest* manifest) {
        if (mKernels.empty()) {
            return true;
        }
        if (mKernels.size() > 1) {
            std::cerr << "Warning: multiple --kernel is specified when building device manifest. "
                      << "Only the first one will be used." << std::endl;
        }
        auto& kernelArg = *mKernels.begin();
        const auto& kernelVer = kernelArg.first;
        auto& kernelConfigFiles = kernelArg.second;
        // addKernel() guarantees that !kernelConfigFiles.empty().
        if (kernelConfigFiles.size() > 1) {
            std::cerr << "Warning: multiple config files are specified in --kernel when building "
                      << "device manfiest. Only the first one will be used." << std::endl;
        }

        KernelConfigParser parser(true /* processComments */, false /* relaxedFormat */);
        status_t err = parser.processAndFinish(read(kernelConfigFiles[0].stream()));
        if (err != OK) {
            std::cerr << parser.error();
            return false;
        }
        manifest->device.mKernel = std::make_optional<KernelInfo>();
        manifest->device.mKernel->mVersion = kernelVer;
        manifest->device.mKernel->mConfigs = parser.configs();
        return true;
    }

    bool assembleHalManifest(HalManifests* halManifests) {
        std::string error;
        HalManifest* halManifest = &halManifests->front().object;
        for (auto it = halManifests->begin() + 1; it != halManifests->end(); ++it) {
            const std::string& path = it->name;
            HalManifest& manifestToAdd = it->object;

            if (manifestToAdd.level() != Level::UNSPECIFIED) {
                if (halManifest->level() == Level::UNSPECIFIED) {
                    halManifest->mLevel = manifestToAdd.level();
                } else if (halManifest->level() != manifestToAdd.level()) {
                    std::cerr << "Inconsistent FCM Version in HAL manifests:" << std::endl
                              << "    File '" << halManifests->front().name << "' has level "
                              << halManifest->level() << std::endl
                              << "    File '" << path << "' has level " << manifestToAdd.level()
                              << std::endl;
                    return false;
                }
            }

            if (!halManifest->addAll(&manifestToAdd, &error)) {
                std::cerr << "File \"" << path << "\" cannot be added: " << error << std::endl;
                return false;
            }
        }

        if (halManifest->mType == SchemaType::DEVICE) {
            (void)getFlagIfUnset("BOARD_SEPOLICY_VERS", &halManifest->device.mSepolicyVersion);

            if (!setDeviceFcmVersion(halManifest)) {
                return false;
            }

            if (!setDeviceManifestKernel(halManifest)) {
                return false;
            }
        }

        if (halManifest->mType == SchemaType::FRAMEWORK) {
            for (auto&& v : getEnvList("PROVIDED_VNDK_VERSIONS")) {
                halManifest->framework.mVendorNdks.emplace_back(std::move(v));
            }

            for (auto&& v : getEnvList("PLATFORM_SYSTEMSDK_VERSIONS")) {
                halManifest->framework.mSystemSdk.mVersions.emplace(std::move(v));
            }
        }

        outputInputs(*halManifests);

        if (mOutputMatrix) {
            CompatibilityMatrix generatedMatrix = halManifest->generateCompatibleMatrix();
            if (!halManifest->checkCompatibility(generatedMatrix, &error)) {
                std::cerr << "FATAL ERROR: cannot generate a compatible matrix: " << error
                          << std::endl;
            }
            out() << "<!-- \n"
                     "    Autogenerated skeleton compatibility matrix. \n"
                     "    Use with caution. Modify it to suit your needs.\n"
                     "    All HALs are set to optional.\n"
                     "    Many entries other than HALs are zero-filled and\n"
                     "    require human attention. \n"
                     "-->\n"
                  << gCompatibilityMatrixConverter(generatedMatrix, mSerializeFlags);
        } else {
            out() << gHalManifestConverter(*halManifest, mSerializeFlags);
        }
        out().flush();

        if (mCheckFile != nullptr) {
            CompatibilityMatrix checkMatrix;
            if (!gCompatibilityMatrixConverter(&checkMatrix, read(*mCheckFile), &error)) {
                std::cerr << "Cannot parse check file as a compatibility matrix: " << error
                          << std::endl;
                return false;
            }
            if (!checkDualFile(*halManifest, checkMatrix)) {
                return false;
            }
        }

        return true;
    }

    // Parse --kernel arguments and write to output matrix.
    bool assembleFrameworkCompatibilityMatrixKernels(CompatibilityMatrix* matrix) {
        for (auto& pair : mKernels) {
            std::vector<ConditionedConfig> conditionedConfigs;
            if (!parseFilesForKernelConfigs(&pair.second, &conditionedConfigs)) {
                return false;
            }
            for (ConditionedConfig& conditionedConfig : conditionedConfigs) {
                MatrixKernel kernel(KernelVersion{pair.first}, std::move(conditionedConfig.second));
                if (conditionedConfig.first != nullptr)
                    kernel.mConditions.push_back(std::move(*conditionedConfig.first));
                std::string error;
                if (!matrix->addKernel(std::move(kernel), &error)) {
                    std::cerr << "Error:" << error << std::endl;
                    return false;
                };
            }
        }
        return true;
    }

    bool setDeviceFcmVersion(HalManifest* manifest) {
        // Not needed for generating empty manifest for DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE.
        if (getBooleanFlag("VINTF_IGNORE_TARGET_FCM_VERSION")) {
            return true;
        }

        size_t shippingApiLevel = getIntegerFlag("PRODUCT_SHIPPING_API_LEVEL");

        if (manifest->level() != Level::UNSPECIFIED) {
            return true;
        }
        if (!getBooleanFlag("PRODUCT_ENFORCE_VINTF_MANIFEST")) {
            manifest->mLevel = Level::LEGACY;
            return true;
        }
        // TODO(b/70628538): Do not infer from Shipping API level.
        if (shippingApiLevel) {
            std::cerr << "Warning: Shipping FCM Version is inferred from Shipping API level. "
                      << "Declare Shipping FCM Version in device manifest directly." << std::endl;
            manifest->mLevel = details::convertFromApiLevel(shippingApiLevel);
            if (manifest->mLevel == Level::UNSPECIFIED) {
                std::cerr << "Error: Shipping FCM Version cannot be inferred from Shipping API "
                          << "level " << shippingApiLevel << "."
                          << "Declare Shipping FCM Version in device manifest directly."
                          << std::endl;
                return false;
            }
            return true;
        }
        // TODO(b/69638851): should be an error if Shipping API level is not defined.
        // For now, just leave it empty; when framework compatibility matrix is built,
        // lowest FCM Version is assumed.
        std::cerr << "Warning: Shipping FCM Version cannot be inferred, because:" << std::endl
                  << "    (1) It is not explicitly declared in device manifest;" << std::endl
                  << "    (2) PRODUCT_ENFORCE_VINTF_MANIFEST is set to true;" << std::endl
                  << "    (3) PRODUCT_SHIPPING_API_LEVEL is undefined." << std::endl
                  << "Assuming 'unspecified' Shipping FCM Version. " << std::endl
                  << "To remove this warning, define 'level' attribute in device manifest."
                  << std::endl;
        return true;
    }

    Level getLowestFcmVersion(const CompatibilityMatrices& matrices) {
        Level ret = Level::UNSPECIFIED;
        for (const auto& e : matrices) {
            if (ret == Level::UNSPECIFIED || ret > e.object.level()) {
                ret = e.object.level();
            }
        }
        return ret;
    }

    bool assembleCompatibilityMatrix(CompatibilityMatrices* matrices) {
        std::string error;
        CompatibilityMatrix* matrix = nullptr;
        std::unique_ptr<HalManifest> checkManifest;
        std::unique_ptr<CompatibilityMatrix> builtMatrix;

        if (mCheckFile != nullptr) {
            checkManifest = std::make_unique<HalManifest>();
            if (!gHalManifestConverter(checkManifest.get(), read(*mCheckFile), &error)) {
                std::cerr << "Cannot parse check file as a HAL manifest: " << error << std::endl;
                return false;
            }
        }

        if (matrices->front().object.mType == SchemaType::DEVICE) {
            builtMatrix = CompatibilityMatrix::combineDeviceMatrices(matrices, &error);
            matrix = builtMatrix.get();

            if (matrix == nullptr) {
                std::cerr << error << std::endl;
                return false;
            }

            auto vndkVersion = base::Trim(getEnv("REQUIRED_VNDK_VERSION"));
            if (!vndkVersion.empty()) {
                auto& valueInMatrix = matrix->device.mVendorNdk;
                if (!valueInMatrix.version().empty() && valueInMatrix.version() != vndkVersion) {
                    std::cerr << "Hard-coded <vendor-ndk> version in device compatibility matrix ("
                              << matrices->front().name << "), '" << valueInMatrix.version()
                              << "', does not match value inferred "
                              << "from BOARD_VNDK_VERSION '" << vndkVersion << "'" << std::endl;
                    return false;
                }
                valueInMatrix = VendorNdk{std::move(vndkVersion)};
            }

            for (auto&& v : getEnvList("BOARD_SYSTEMSDK_VERSIONS")) {
                matrix->device.mSystemSdk.mVersions.emplace(std::move(v));
            }
        }

        if (matrices->front().object.mType == SchemaType::FRAMEWORK) {
            Level deviceLevel =
                checkManifest != nullptr ? checkManifest->level() : Level::UNSPECIFIED;
            if (deviceLevel == Level::UNSPECIFIED) {
                deviceLevel = getLowestFcmVersion(*matrices);
                if (checkManifest != nullptr && deviceLevel != Level::UNSPECIFIED) {
                    std::cerr << "Warning: No Target FCM Version for device. Assuming \""
                              << to_string(deviceLevel)
                              << "\" when building final framework compatibility matrix."
                              << std::endl;
                }
            }
            builtMatrix = CompatibilityMatrix::combine(deviceLevel, matrices, &error);
            matrix = builtMatrix.get();

            if (matrix == nullptr) {
                std::cerr << error << std::endl;
                return false;
            }

            if (!assembleFrameworkCompatibilityMatrixKernels(matrix)) {
                return false;
            }

            // Add PLATFORM_SEPOLICY_* to sepolicy.sepolicy-version. Remove dupes.
            std::set<Version> sepolicyVersions;
            auto sepolicyVersionStrings = getEnvList("PLATFORM_SEPOLICY_COMPAT_VERSIONS");
            auto currentSepolicyVersionString = getEnv("PLATFORM_SEPOLICY_VERSION");
            if (!currentSepolicyVersionString.empty()) {
                sepolicyVersionStrings.push_back(currentSepolicyVersionString);
            }
            for (auto&& s : sepolicyVersionStrings) {
                Version v;
                if (!parse(s, &v)) {
                    std::cerr << "Error: unknown sepolicy version '" << s << "' specified by "
                              << (s == currentSepolicyVersionString
                                      ? "PLATFORM_SEPOLICY_VERSION"
                                      : "PLATFORM_SEPOLICY_COMPAT_VERSIONS")
                              << ".";
                    return false;
                }
                sepolicyVersions.insert(v);
            }
            for (auto&& v : sepolicyVersions) {
                matrix->framework.mSepolicy.mSepolicyVersionRanges.emplace_back(v.majorVer,
                                                                                v.minorVer);
            }

            getFlagIfUnset("POLICYVERS", &matrix->framework.mSepolicy.mKernelSepolicyVersion,
                           false /* log */);
            getFlagIfUnset("FRAMEWORK_VBMETA_VERSION", &matrix->framework.mAvbMetaVersion,
                           false /* log */);
            // Hard-override existing AVB version
            getFlag("FRAMEWORK_VBMETA_VERSION_OVERRIDE", &matrix->framework.mAvbMetaVersion,
                    false /* log */);
        }
        outputInputs(*matrices);
        out() << gCompatibilityMatrixConverter(*matrix, mSerializeFlags);
        out().flush();

        if (checkManifest != nullptr && !checkDualFile(*checkManifest, *matrix)) {
            return false;
        }

        return true;
    }

    enum AssembleStatus { SUCCESS, FAIL_AND_EXIT, TRY_NEXT };
    template <typename Schema, typename AssembleFunc>
    AssembleStatus tryAssemble(const XmlConverter<Schema>& converter, const std::string& schemaName,
                               AssembleFunc assemble, std::string* error) {
        Schemas<Schema> schemas;
        Schema schema;
        if (!converter(&schema, read(mInFiles.front().stream()), error)) {
            return TRY_NEXT;
        }
        auto firstType = schema.type();
        schemas.emplace_back(mInFiles.front().name(), std::move(schema));

        for (auto it = mInFiles.begin() + 1; it != mInFiles.end(); ++it) {
            Schema additionalSchema;
            const std::string& fileName = it->name();
            if (!converter(&additionalSchema, read(it->stream()), error)) {
                std::cerr << "File \"" << fileName << "\" is not a valid " << firstType << " "
                          << schemaName << " (but the first file is a valid " << firstType << " "
                          << schemaName << "). Error: " << *error << std::endl;
                return FAIL_AND_EXIT;
            }
            if (additionalSchema.type() != firstType) {
                std::cerr << "File \"" << fileName << "\" is a " << additionalSchema.type() << " "
                          << schemaName << " (but a " << firstType << " " << schemaName
                          << " is expected)." << std::endl;
                return FAIL_AND_EXIT;
            }

            schemas.emplace_back(fileName, std::move(additionalSchema));
        }
        return assemble(&schemas) ? SUCCESS : FAIL_AND_EXIT;
    }

    bool assemble() override {
        using std::placeholders::_1;
        if (mInFiles.empty()) {
            std::cerr << "Missing input file." << std::endl;
            return false;
        }

        std::string manifestError;
        auto status = tryAssemble(gHalManifestConverter, "manifest",
                                  std::bind(&AssembleVintfImpl::assembleHalManifest, this, _1),
                                  &manifestError);
        if (status == SUCCESS) return true;
        if (status == FAIL_AND_EXIT) return false;

        resetInFiles();

        std::string matrixError;
        status = tryAssemble(gCompatibilityMatrixConverter, "compatibility matrix",
                             std::bind(&AssembleVintfImpl::assembleCompatibilityMatrix, this, _1),
                             &matrixError);
        if (status == SUCCESS) return true;
        if (status == FAIL_AND_EXIT) return false;

        std::cerr << "Input file has unknown format." << std::endl
                  << "Error when attempting to convert to manifest: " << manifestError << std::endl
                  << "Error when attempting to convert to compatibility matrix: " << matrixError
                  << std::endl;
        return false;
    }

    std::ostream& setOutputStream(Ostream&& out) override {
        mOutRef = std::move(out);
        return *mOutRef;
    }

    std::istream& addInputStream(const std::string& name, Istream&& in) override {
        auto it = mInFiles.emplace(mInFiles.end(), name, std::move(in));
        return it->stream();
    }

    std::istream& setCheckInputStream(Istream&& in) override {
        mCheckFile = std::move(in);
        return *mCheckFile;
    }

    bool hasKernelVersion(const KernelVersion& kernelVer) const override {
        return mKernels.find(kernelVer) != mKernels.end();
    }

    std::istream& addKernelConfigInputStream(const KernelVersion& kernelVer,
                                             const std::string& name, Istream&& in) override {
        auto&& kernel = mKernels[kernelVer];
        auto it = kernel.emplace(kernel.end(), name, std::move(in));
        return it->stream();
    }

    void resetInFiles() {
        for (auto& inFile : mInFiles) {
            inFile.stream().clear();
            inFile.stream().seekg(0);
        }
    }

    void setOutputMatrix() override { mOutputMatrix = true; }

    bool setHalsOnly() override {
        if (mHasSetHalsOnlyFlag) {
            std::cerr << "Error: Cannot set --hals-only with --no-hals." << std::endl;
            return false;
        }
        // Just override it with HALS_ONLY because other flags that modify mSerializeFlags
        // does not interfere with this (except --no-hals).
        mSerializeFlags = SerializeFlags::HALS_ONLY;
        mHasSetHalsOnlyFlag = true;
        return true;
    }

    bool setNoHals() override {
        if (mHasSetHalsOnlyFlag) {
            std::cerr << "Error: Cannot set --hals-only with --no-hals." << std::endl;
            return false;
        }
        mSerializeFlags = mSerializeFlags.disableHals();
        mHasSetHalsOnlyFlag = true;
        return true;
    }

    bool setNoKernelRequirements() override {
        mSerializeFlags = mSerializeFlags.disableKernelConfigs().disableKernelMinorRevision();
        return true;
    }

   private:
    std::vector<NamedIstream> mInFiles;
    Ostream mOutRef;
    Istream mCheckFile;
    bool mOutputMatrix = false;
    bool mHasSetHalsOnlyFlag = false;
    SerializeFlags::Type mSerializeFlags = SerializeFlags::EVERYTHING;
    std::map<KernelVersion, std::vector<NamedIstream>> mKernels;
    std::map<std::string, std::string> mFakeEnv;
};

bool AssembleVintf::openOutFile(const std::string& path) {
    return static_cast<std::ofstream&>(setOutputStream(std::make_unique<std::ofstream>(path)))
        .is_open();
}

bool AssembleVintf::openInFile(const std::string& path) {
    return static_cast<std::ifstream&>(addInputStream(path, std::make_unique<std::ifstream>(path)))
        .is_open();
}

bool AssembleVintf::openCheckFile(const std::string& path) {
    return static_cast<std::ifstream&>(setCheckInputStream(std::make_unique<std::ifstream>(path)))
        .is_open();
}

bool AssembleVintf::addKernel(const std::string& kernelArg) {
    auto tokens = base::Split(kernelArg, ":");
    if (tokens.size() <= 1) {
        std::cerr << "Unrecognized --kernel option '" << kernelArg << "'" << std::endl;
        return false;
    }
    KernelVersion kernelVer;
    if (!parse(tokens.front(), &kernelVer)) {
        std::cerr << "Unrecognized kernel version '" << tokens.front() << "'" << std::endl;
        return false;
    }
    if (hasKernelVersion(kernelVer)) {
        std::cerr << "Multiple --kernel for " << kernelVer << " is specified." << std::endl;
        return false;
    }
    for (auto it = tokens.begin() + 1; it != tokens.end(); ++it) {
        bool opened =
            static_cast<std::ifstream&>(
                addKernelConfigInputStream(kernelVer, *it, std::make_unique<std::ifstream>(*it)))
                .is_open();
        if (!opened) {
            std::cerr << "Cannot open file '" << *it << "'." << std::endl;
            return false;
        }
    }
    return true;
}

std::unique_ptr<AssembleVintf> AssembleVintf::newInstance() {
    return std::make_unique<AssembleVintfImpl>();
}

}  // namespace vintf
}  // namespace android
