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

#include <getopt.h>
#include <unistd.h>

#include <iostream>
#include <map>

#include <android-base/parseint.h>
#include <utils/Errors.h>
#include <vintf/VintfObject.h>
#include <vintf/parse_xml.h>
#include "utils.h"

namespace android {
namespace vintf {
namespace details {

// fake sysprops
using Properties = std::map<std::string, std::string>;

enum Option : int {
    DUMP_FILE_LIST = 1,
    ROOTDIR,
    HELP,
    PROPERTY,
    CHECK_COMPAT,
};
// command line arguments
using Args = std::multimap<Option, std::string>;

class HostFileSystem : public FileSystemUnderPath {
   public:
    HostFileSystem(const std::string& rootdir) : FileSystemUnderPath(rootdir) {}
    status_t fetch(const std::string& path, std::string* fetched,
                   std::string* error) const override {
        status_t status = FileSystemUnderPath::fetch(path, fetched, error);
        std::cerr << "Debug: Fetch '" << getRootDir() << path << "': " << toString(status)
                  << std::endl;
        return status;
    }
    status_t listFiles(const std::string& path, std::vector<std::string>* out,
                       std::string* error) const override {
        status_t status = FileSystemUnderPath::listFiles(path, out, error);
        std::cerr << "Debug: List '" << getRootDir() << path << "': " << toString(status)
                  << std::endl;
        return status;
    }

   private:
    static std::string toString(status_t status) {
        return status == OK ? "SUCCESS" : strerror(-status);
    }
};

class PresetPropertyFetcher : public PropertyFetcher {
   public:
    std::string getProperty(const std::string& key,
                            const std::string& defaultValue) const override {
        auto it = mProps.find(key);
        if (it == mProps.end()) {
            std::cerr << "Debug: Sysprop " << key << " is missing, default to '" << defaultValue
                      << "'" << std::endl;
            return defaultValue;
        }
        std::cerr << "Debug: Sysprop " << key << "=" << it->second << std::endl;
        return it->second;
    }
    uint64_t getUintProperty(const std::string& key, uint64_t defaultValue,
                             uint64_t max) const override {
        uint64_t result;
        std::string value = getProperty(key, "");
        if (!value.empty() && android::base::ParseUint(value, &result, max)) return result;
        return defaultValue;
    }
    bool getBoolProperty(const std::string& key, bool defaultValue) const override {
        std::string value = getProperty(key, "");
        if (value == "1" || value == "true") {
            return true;
        } else if (value == "0" || value == "false") {
            return false;
        }
        return defaultValue;
    }
    void setProperties(const Properties& props) { mProps.insert(props.begin(), props.end()); }

   private:
    std::map<std::string, std::string> mProps;
};

// helper functions
template <typename T>
std::unique_ptr<T> readObject(FileSystem* fileSystem, const std::string& path,
                              const XmlConverter<T>& converter) {
    std::string xml;
    std::string error;
    status_t err = fileSystem->fetch(path, &xml, &error);
    if (err != OK) {
        std::cerr << "Error: Cannot read '" << path << "' (" << strerror(-err) << "): " << error
                  << std::endl;
        return nullptr;
    }
    auto ret = std::make_unique<T>();
    if (!converter(ret.get(), xml, &error)) {
        std::cerr << "Error: Cannot parse '" << path << "': " << error << std::endl;
        return nullptr;
    }
    return ret;
}

int checkCompatibilityForFiles(const std::string& manifestPath, const std::string& matrixPath) {
    auto fileSystem = std::make_unique<FileSystemImpl>();
    auto manifest = readObject(fileSystem.get(), manifestPath, gHalManifestConverter);
    auto matrix = readObject(fileSystem.get(), matrixPath, gCompatibilityMatrixConverter);
    if (manifest == nullptr || matrix == nullptr) {
        return -1;
    }

    std::string error;
    if (!manifest->checkCompatibility(*matrix, &error)) {
        std::cerr << "Error: Incompatible: " << error << std::endl;
        std::cout << "false" << std::endl;
        return 1;
    }

    std::cout << "true" << std::endl;
    return 0;
}

Args parseArgs(int argc, char** argv) {
    int longOptFlag;
    int optionIndex;
    Args ret;
    std::vector<struct option> longopts{
        {"dump-file-list", no_argument, &longOptFlag, DUMP_FILE_LIST},
        {"rootdir", required_argument, &longOptFlag, ROOTDIR},
        {"help", no_argument, &longOptFlag, HELP},
        {"property", required_argument, &longOptFlag, PROPERTY},
        {"check-compat", no_argument, &longOptFlag, CHECK_COMPAT},
        {0, 0, 0, 0}};
    std::map<int, Option> shortopts{
        {'h', HELP}, {'D', PROPERTY}, {'c', CHECK_COMPAT},
    };
    for (;;) {
        int c = getopt_long(argc, argv, "hcD:", longopts.data(), &optionIndex);
        if (c == -1) {
            break;
        }
        std::string argValue = optarg ? optarg : std::string{};
        if (c == 0) {
            ret.emplace(static_cast<Option>(longOptFlag), std::move(argValue));
        } else {
            ret.emplace(shortopts[c], std::move(argValue));
        }
    }
    if (optind < argc) {
        // see non option
        std::cerr << "unrecognized option `" << argv[optind] << "'" << std::endl;
        return {{HELP, ""}};
    }
    return ret;
}

template <typename T>
Properties getProperties(const T& args) {
    Properties ret;
    for (const auto& arg : args) {
        auto pos = arg.find('=');
        auto key = arg.substr(0, pos);
        auto value = pos == std::string::npos ? std::string{} : arg.substr(pos + 1);
        ret[key] = value;
    }
    return ret;
}

int usage(const char* me) {
    std::cerr
        << me << ": check VINTF metadata." << std::endl
        << "    Options:" << std::endl
        << "        --dump-file-list: Dump a list of directories / files on device" << std::endl
        << "                that is required to be used by --check-compat." << std::endl
        << "        -c, --check-compat: check compatibility for files under the root" << std::endl
        << "                directory specified by --root-dir." << std::endl
        << "        --rootdir=<dir>: specify root directory for all metadata." << std::endl
        << "        -D, --property <key>=<value>: specify sysprops." << std::endl
        << "        --help: show this message." << std::endl
        << std::endl
        << "    Example:" << std::endl
        << "        # Get the list of required files." << std::endl
        << "        " << me << " --dump-file-list > /tmp/files.txt" << std::endl
        << "        # Pull from ADB, or use your own command to extract files from images"
        << std::endl
        << "        ROOTDIR=/tmp/device/" << std::endl
        << "        cat /tmp/files.txt | xargs -I{} bash -c \"mkdir -p $ROOTDIR`dirname {}` && adb "
           "pull {} $ROOTDIR{}\""
        << std::endl
        << "        # Check compatibility." << std::endl
        << "        " << me << " --check-compat --rootdir=$ROOTDIR \\" << std::endl
        << "            --property ro.product.first_api_level=`adb shell getprop "
           "ro.product.first_api_level` \\"
        << std::endl
        << "            --property ro.boot.product.hardware.sku=`adb shell getprop "
           "ro.boot.product.hardware.sku`"
        << std::endl;
    return 1;
}

int checkAllFiles(const std::string& rootdir, const Properties& props, std::string* error) {
    auto hostPropertyFetcher = std::make_unique<PresetPropertyFetcher>();
    hostPropertyFetcher->setProperties(props);
    auto vintfObject = VintfObject::Builder()
                           .setFileSystem(std::make_unique<HostFileSystem>(rootdir))
                           .setPropertyFetcher(std::move(hostPropertyFetcher))
                           .build();
    return vintfObject->checkCompatibility(error, CheckFlags::DISABLE_RUNTIME_INFO);
}

}  // namespace details
}  // namespace vintf
}  // namespace android

int main(int argc, char** argv) {
    using namespace android::vintf;
    using namespace android::vintf::details;
    // legacy usage: check_vintf <manifest.xml> <matrix.xml>
    if (argc == 3) {
        int ret = checkCompatibilityForFiles(argv[1], argv[2]);
        if (ret >= 0) return ret;
    }

    Args args = parseArgs(argc, argv);

    if (!iterateValues(args, HELP).empty()) {
        return usage(argv[0]);
    }

    if (!iterateValues(args, DUMP_FILE_LIST).empty()) {
        for (const auto& file : dumpFileList()) {
            std::cout << file << std::endl;
        }
        return 0;
    }

    auto rootdirs = iterateValues(args, ROOTDIR);
    auto properties = getProperties(iterateValues(args, PROPERTY));

    auto checkCompat = iterateValues(args, CHECK_COMPAT);
    if (!checkCompat.empty()) {
        if (rootdirs.empty()) {
            std::cerr << "Missing --rootdir option." << std::endl;
            return usage(argv[0]);
        }
        int ret = COMPATIBLE;
        for (const auto& rootdir : rootdirs) {
            std::cerr << "Debug: checking files under " << rootdir << "..." << std::endl;
            std::string error;
            int compat = checkAllFiles(rootdir, properties, &error);
            std::cerr << "Debug: files under " << rootdir
                      << (compat == COMPATIBLE
                              ? " is compatible"
                              : compat == INCOMPATIBLE ? " are incompatible"
                                                       : (" has encountered an error: " + error))
                      << std::endl;
        }
        if (ret == COMPATIBLE) {
            std::cout << "true" << std::endl;
        }
        return ret;
    }

    return usage(argv[0]);
}
