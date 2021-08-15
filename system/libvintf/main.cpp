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

#include <android-base/strings.h>
#include <vintf/VintfObject.h>
#include <vintf/parse_string.h>
#include <vintf/parse_xml.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace ::android::vintf;

static const std::string kColumnSeperator = "   ";

std::string existString(bool value) {
    return value ? "GOOD" : "DOES NOT EXIST";
}

std::string compatibleString(int32_t value) {
    switch (value) {
        case COMPATIBLE:
            return "GOOD";
        case INCOMPATIBLE:
            return "INCOMPATIBLE";
        default:
            return strerror(-value);
    }
}

std::string boolCompatString(bool value) {
    return compatibleString(value ? COMPATIBLE : INCOMPATIBLE);
}

std::string deprecateString(int32_t value) {
    switch (value) {
        case NO_DEPRECATED_HALS:
            return "GOOD";
        case DEPRECATED:
            return "DEPRECATED";
        default:
            return strerror(-value);
    }
}

enum Status : int {
    OK = 0,
    USAGE,
};

struct ParsedOptions {
    bool verbose = false;
};

struct Option {
    char shortOption = '\0';
    std::string longOption;
    std::string help;
    std::function<Status(ParsedOptions*)> op;
};

std::string getShortOptions(const std::vector<Option>& options) {
    std::stringstream ret;
    for (const auto& e : options)
        if (e.shortOption != '\0') ret << e.shortOption;
    return ret.str();
}

std::unique_ptr<struct option[]> getLongOptions(const std::vector<Option>& options,
                                                int* longOptFlag) {
    std::unique_ptr<struct option[]> ret{new struct option[options.size() + 1]};
    int i = 0;
    for (const auto& e : options) {
        ret[i].name = e.longOption.c_str();
        ret[i].has_arg = no_argument;
        ret[i].flag = longOptFlag;
        ret[i].val = i;

        i++;
    }
    // getopt_long last option has all zeros
    ret[i].name = NULL;
    ret[i].has_arg = 0;
    ret[i].flag = NULL;
    ret[i].val = 0;

    return ret;
}

Status parseOptions(int argc, char** argv, const std::vector<Option>& options, ParsedOptions* out) {
    int longOptFlag;
    std::unique_ptr<struct option[]> longOptions = getLongOptions(options, &longOptFlag);
    std::string shortOptions = getShortOptions(options);
    int optionIndex;
    for (;;) {
        int c = getopt_long(argc, argv, shortOptions.c_str(), longOptions.get(), &optionIndex);
        if (c == -1) {
            break;
        }
        const Option* found = nullptr;
        for (size_t i = 0; i < options.size(); ++i)
            if ((c == 0 && longOptFlag == static_cast<int>(i)) ||
                (c != 0 && c == options[i].shortOption))

                found = &options[i];

        if (found == nullptr) {
            // see unrecognized options
            std::cerr << "unrecognized option `" << argv[optind - 1] << "'" << std::endl;
            return USAGE;
        }

        Status status = found->op(out);
        if (status != OK) return status;
    }
    if (optind < argc) {
        // see non option
        std::cerr << "unrecognized option `" << argv[optind] << "'" << std::endl;
        return USAGE;
    }
    return OK;
}

void usage(char* me, const std::vector<Option>& options) {
    std::cerr << me << ": dump VINTF metadata via libvintf." << std::endl;
    for (const auto& e : options) {
        if (e.help.empty()) continue;
        std::cerr << "        ";
        if (e.shortOption != '\0') std::cerr << "-" << e.shortOption;
        if (e.shortOption != '\0' && !e.longOption.empty()) std::cerr << ", ";
        if (!e.longOption.empty()) std::cerr << "--" << e.longOption;
        std::cerr << ": "
                  << android::base::Join(android::base::Split(e.help, "\n"), "\n            ")
                  << std::endl;
    }
}

struct TableRow {
    // Whether the HAL version is in device manifest, framework manifest, device compatibility
    // matrix, framework compatibility matrix, respectively.
    bool dm = false;
    bool fm = false;
    bool dcm = false;
    bool fcm = false;
    // If the HAL version is in device / framework compatibility matrix, whether it is required
    // or not.
    bool required = false;

    // Return true if:
    // - not a required HAL version; OR
    // - required in device matrix and framework manifest;
    // - required in framework matrix and device manifest.
    bool meetsReqeuirement() const {
        if (!required) return true;
        if (dcm && !fm) return false;
        if (fcm && !dm) return false;
        return true;
    }
};

std::ostream& operator<<(std::ostream& out, const TableRow& row) {
    return out << (row.required ? "R" : " ") << (row.meetsReqeuirement() ? " " : "!")
               << kColumnSeperator << (row.dm ? "DM" : "  ") << kColumnSeperator
               << (row.fm ? "FM" : "  ") << kColumnSeperator << (row.fcm ? "FCM" : "   ")
               << kColumnSeperator << (row.dcm ? "DCM" : "   ");
}

using RowMutator = std::function<void(TableRow*)>;
using Table = std::map<std::string, TableRow>;

// Insert each fqInstanceName foo@x.y::IFoo/instance to the table by inserting the key
// if it does not exist and setting the corresponding indicator (as specified by "mutate").
void insert(const HalManifest* manifest, Table* table, const RowMutator& mutate) {
    if (manifest == nullptr) return;
    manifest->forEachInstance([&](const auto& manifestInstance) {
        std::string key = toFQNameString(manifestInstance.package(), manifestInstance.version(),
                                         manifestInstance.interface(), manifestInstance.instance());
        mutate(&(*table)[key]);
        return true;
    });
}

void insert(const CompatibilityMatrix* matrix, Table* table, const RowMutator& mutate) {
    if (matrix == nullptr) return;
    matrix->forEachInstance([&](const auto& matrixInstance) {
        for (auto minorVer = matrixInstance.versionRange().minMinor;
             minorVer <= matrixInstance.versionRange().maxMinor; ++minorVer) {
            std::string key = toFQNameString(
                matrixInstance.package(), Version{matrixInstance.versionRange().majorVer, minorVer},
                matrixInstance.interface(),
                matrixInstance.isRegex() ? matrixInstance.regexPattern()
                                         : matrixInstance.exactInstance());
            auto it = table->find(key);
            if (it == table->end()) {
                mutate(&(*table)[key]);
            } else {
                mutate(&it->second);
                if (minorVer == matrixInstance.versionRange().minMinor) {
                    it->second.required = !matrixInstance.optional();
                }
            }
        }
        return true;
    });
}

Table generateHalSummary(const HalManifest* vm, const HalManifest* fm,
                         const CompatibilityMatrix* vcm, const CompatibilityMatrix* fcm) {
    Table table;
    insert(vm, &table, [](auto* row) { row->dm = true; });
    insert(fm, &table, [](auto* row) { row->fm = true; });
    insert(vcm, &table, [](auto* row) { row->dcm = true; });
    insert(fcm, &table, [](auto* row) { row->fcm = true; });

    return table;
}

static const std::vector<Option> gAvailableOptions{
    {'h', "help", "Print help message.", [](auto) { return USAGE; }},
    {'v', "verbose", "Dump detailed and raw content, including kernel configurations", [](auto o) {
         o->verbose = true;
         return OK;
     }}};
// A convenience binary to dump information available through libvintf.
int main(int argc, char** argv) {
    ParsedOptions options;
    Status status = parseOptions(argc, argv, gAvailableOptions, &options);
    if (status == USAGE) usage(argv[0], gAvailableOptions);
    if (status != OK) return status;

    auto vm = VintfObject::GetDeviceHalManifest();
    auto fm = VintfObject::GetFrameworkHalManifest();
    auto vcm = VintfObject::GetDeviceCompatibilityMatrix();
    auto fcm = VintfObject::GetFrameworkCompatibilityMatrix();
    auto ki = VintfObject::GetRuntimeInfo();

    if (!options.verbose) {
        std::cout << "======== HALs =========" << std::endl
                  << "R: required. (empty): optional or missing from matrices. "
                  << "!: required and not in manifest." << std::endl
                  << "DM: device manifest. FM: framework manifest." << std::endl
                  << "FCM: framework compatibility matrix. DCM: device compatibility matrix."
                  << std::endl
                  << std::endl;
        auto table = generateHalSummary(vm.get(), fm.get(), vcm.get(), fcm.get());

        for (const auto& pair : table)
            std::cout << pair.second << kColumnSeperator << pair.first << std::endl;

        std::cout << std::endl;
    }

    SerializeFlags::Type flags = SerializeFlags::EVERYTHING;
    if (!options.verbose) {
        flags = flags.disableHals().disableKernel();
    }
    std::cout << "======== Device HAL Manifest =========" << std::endl;
    if (vm != nullptr) std::cout << gHalManifestConverter(*vm, flags);
    std::cout << "======== Framework HAL Manifest =========" << std::endl;
    if (fm != nullptr) std::cout << gHalManifestConverter(*fm, flags);
    std::cout << "======== Device Compatibility Matrix =========" << std::endl;
    if (vcm != nullptr) std::cout << gCompatibilityMatrixConverter(*vcm, flags);
    std::cout << "======== Framework Compatibility Matrix =========" << std::endl;
    if (fcm != nullptr) std::cout << gCompatibilityMatrixConverter(*fcm, flags);

    std::cout << "======== Runtime Info =========" << std::endl;
    if (ki != nullptr) std::cout << dump(*ki, options.verbose);

    std::cout << std::endl;

    std::cout << "======== Summary =========" << std::endl;
    std::cout << "Device Manifest?    " << existString(vm != nullptr) << std::endl
              << "Device Matrix?      " << existString(vcm != nullptr) << std::endl
              << "Framework Manifest? " << existString(fm != nullptr) << std::endl
              << "Framework Matrix?   " << existString(fcm != nullptr) << std::endl;
    std::string error;
    if (vm && fcm) {
        bool compatible = vm->checkCompatibility(*fcm, &error);
        std::cout << "Device HAL Manifest <==> Framework Compatibility Matrix? "
                  << boolCompatString(compatible);
        if (!compatible)
            std::cout << ", " << error;
        std::cout << std::endl;
    }
    if (fm && vcm) {
        bool compatible = fm->checkCompatibility(*vcm, &error);
        std::cout << "Framework HAL Manifest <==> Device Compatibility Matrix? "
                  << boolCompatString(compatible);
        if (!compatible)
            std::cout << ", " << error;
        std::cout << std::endl;
    }
    if (ki && fcm) {
        bool compatible = ki->checkCompatibility(*fcm, &error);
        std::cout << "Runtime info <==> Framework Compatibility Matrix?        "
                  << boolCompatString(compatible);
        if (!compatible) std::cout << ", " << error;
        std::cout << std::endl;
    }

    {
        auto compatible = VintfObject::CheckCompatibility({}, &error);
        std::cout << "VintfObject::CheckCompatibility?                         "
                  << compatibleString(compatible);
        if (compatible != COMPATIBLE) std::cout << ", " << error;
        std::cout << std::endl;
    }

    if (vm && fcm) {
        auto deprecate = VintfObject::CheckDeprecation(&error);
        std::cout << "VintfObject::CheckDeprecation (against device manifest)? "
                  << deprecateString(deprecate);
        if (deprecate != NO_DEPRECATED_HALS) std::cout << ", " << error;
        std::cout << std::endl;
    }
}
