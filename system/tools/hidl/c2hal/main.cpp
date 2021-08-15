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

#include "AST.h"

#include <android-base/logging.h>
#include <android-base/macros.h>
#include <set>
#include <map>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>

using namespace android;

extern status_t parseFile(android::AST *ast);

static void usage(const char *me) {
    fprintf(stderr,
            "usage: %s [-g] [-o dir] -p package (-r interface-root)+ (header-filepath)+\n",
            me);

    fprintf(stderr, "         -h print this message\n");
    fprintf(stderr, "         -o output path\n");
    fprintf(stderr, "            (example: ~/android/master)\n");
    fprintf(stderr, "         -p package\n");
    fprintf(stderr, "            (example: android.hardware.baz@1.0)\n");
    fprintf(stderr, "         -g (enable open-gl mode) \n");
    fprintf(stderr, "         -r package:path root "
                    "(e.g., android.hardware:hardware/interfaces)\n");
}

static void addPackageRootToMap(const std::string &val,
                                std::map<std::string, std::string> &packageRootPaths) {
    auto index = val.find_first_of(':');
    CHECK(index != std::string::npos);

    auto package = val.substr(0, index);
    auto path = val.substr(index + 1);

    packageRootPaths[package] = path;
}

static bool isPathPrefix(const std::string &prefix, const std::string &base) {
    if (prefix.size() >= base.size()) {
        LOG(DEBUG) << "Not long enough";
        return false;
    }

    if (base[prefix.size()] != '.') {
        LOG(DEBUG) << "not full";
        return false;
    }

    return prefix == base.substr(0, prefix.size());
}

static void applyPackageRootPath(
        const std::map<std::string, std::string> &packageRootPaths,
        const std::string &package,
        std::string &outputPath) {

    auto index = package.find_first_of('@');
    CHECK(index != std::string::npos);

    auto packagePath = package.substr(0, index);
    auto packageVersion = package.substr(index + 1);

    for (auto const& pair : packageRootPaths) {
        const std::string& rootPackage = pair.first;
        const std::string& rootPath = pair.second;

        if (isPathPrefix(rootPackage, packagePath)) {

            packagePath = packagePath.substr(rootPackage.size() + 1);
            std::replace(packagePath.begin(), packagePath.end(), '.', '/');
            packagePath += '/' + packageVersion;

            if (outputPath.empty()) {
                outputPath = rootPath;
            }

            outputPath += '/' + packagePath + '/';
            return;
        }
    }

    CHECK(!outputPath.empty()) << "No package root path provided for: " << package;

    outputPath += '/';
}

int main(int argc, char **argv) {
    const char *me = argv[0];

    std::string outputDir;
    std::string package;
    std::map<std::string, std::string> packageRootPaths;
    bool isOpenGl = false;
    bool verbose = false;

    int res;
    while ((res = getopt(argc, argv, "ghvo:p:r:")) >= 0) {
        switch (res) {
            case 'o': {
                outputDir = optarg;
                break;
            }
            case 'p': {
                package = optarg;
                break;
            }
            case 'g': {
                isOpenGl = true;
                break;
            }
            case 'v': {
                verbose = true;
                break;
            }
            case 'r':
            {
                addPackageRootToMap(optarg, packageRootPaths);
                break;
            }
            case 'h':
            default:
            {
                usage(me);
                exit(1);
                break;
            }
        }
    }

    // if no arguments are provided, show usage instead of specific errors
    if (optind == 1) {
        usage(me);
        exit(0);
    }

    if (verbose) {
        SetMinimumLogSeverity(android::base::VERBOSE);
    }

    applyPackageRootPath(packageRootPaths, package, outputDir);

    if (package.empty()) {
        LOG(WARNING) << "You must provide a package.";
        usage(me);
        exit(0);
    }

    if (optind == argc) {
        LOG(WARNING) << "You must provide a header-filepath.";
        usage(me);
        exit(0);
    }

    for(int i = optind; i < argc; i++) {
        std::string path = argv[i];

        LOG(DEBUG) << "Processing " << path;

        AST ast(path, outputDir, package, isOpenGl);

        int res = parseFile(&ast);

        if (res != 0) {
            LOG(ERROR) << "Could not parse: " << res;
            exit(1);
        }

        ast.processContents();

        ast.generateCode();
    }

    return 0;
}
