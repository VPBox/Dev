/*
 * Copyright (C) 2018 The Android Open Source Project
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
 * See the License for the specic language governing permissions and
 * limitations under the License.
 */

#include <thread>

#include <getopt.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <sys/types.h>

#include "perfmgr/HintManager.h"

namespace android {
namespace perfmgr {

class NodeVerifier : public HintManager {
  public:
    static bool VerifyNodes(const std::string& config_path) {
        std::string json_doc;

        if (!android::base::ReadFileToString(config_path, &json_doc)) {
            LOG(ERROR) << "Failed to read JSON config from " << config_path;
            return false;
        }

        std::vector<std::unique_ptr<Node>> nodes = ParseNodes(json_doc);
        if (nodes.empty()) {
            LOG(ERROR) << "Failed to parse Nodes section from " << config_path;
            return false;
        }

        for (const auto& node : nodes) {
            std::vector<std::string> values = node->GetValues();
            std::string default_value = values[node->GetDefaultIndex()];
            // Always set to default first
            values.insert(values.begin(), default_value);
            // And reset to default after test
            values.push_back(default_value);
            for (const auto& value : values) {
                if (!android::base::WriteStringToFile(value, node->GetPath())) {
                    LOG(ERROR) << "Failed to write to node: " << node->GetPath()
                               << " with value: " << value;
                    return false;
                }
                LOG(VERBOSE) << "Wrote to node: " << node->GetPath()
                             << " with value: " << value;
            }
        }
        return true;
    }

  private:
    NodeVerifier(sp<NodeLooperThread> nm,
                 const std::map<std::string, std::vector<NodeAction>>& actions)
        : HintManager(std::move(nm), actions) {}
};

}  // namespace perfmgr
}  // namespace android

static void printUsage(const char* exec_name) {
    std::string usage = exec_name;
    usage =
        usage +
        " is a command-line tool to verify Nodes in Json config are writable.\n"
        "Usages:\n"
        "    [su system] " +
        exec_name +
        " [options]\n"
        "\n"
        "Options:\n"
        "   --config [PATH], -c\n"
        "       path to Json config file\n\n"
        "   --exec_hint, -e\n"
        "       do hints in Json config\n\n"
        "   --help, -h\n"
        "       print this message\n\n"
        "   --verbose, -v\n"
        "       print verbose log during execution\n\n";

    LOG(INFO) << usage;
}

static void execConfig(const std::string& json_file) {
    std::unique_ptr<android::perfmgr::HintManager> hm =
        android::perfmgr::HintManager::GetFromJSON(json_file);
    if (!hm.get() || !hm->IsRunning()) {
        LOG(ERROR) << "Failed to Parse JSON config";
    }
    std::vector<std::string> hints = hm->GetHints();
    for (const auto& hint : hints) {
        LOG(INFO) << "Do hint: " << hint;
        hm->DoHint(hint);
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        LOG(INFO) << "End hint: " << hint;
        hm->EndHint(hint);
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int argc, char* argv[]) {
    android::base::InitLogging(argv, android::base::StderrLogger);

    if (getuid() == 0) {
        LOG(WARNING) << "Running as root might mask node permission";
    }

    std::string config_path;
    bool exec_hint = false;
    while (true) {
        static struct option opts[] = {
            {"config", required_argument, nullptr, 'c'},
            {"exec_hint", no_argument, nullptr, 'e'},
            {"help", no_argument, nullptr, 'h'},
            {"verbose", no_argument, nullptr, 'v'},
            {0, 0, 0, 0}  // termination of the option list
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "c:ehv", opts, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'c':
                config_path = optarg;
                break;
            case 'e':
                exec_hint = true;
                break;
            case 'v':
                android::base::SetMinimumLogSeverity(android::base::VERBOSE);
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            default:
                // getopt already prints "invalid option -- %c" for us.
                return 1;
        }
    }

    if (config_path.empty()) {
        LOG(ERROR) << "Need specify JSON config";
        printUsage(argv[0]);
        return 1;
    }

    if (exec_hint) {
        execConfig(config_path);
        return 0;
    }

    if (android::perfmgr::NodeVerifier::VerifyNodes(config_path)) {
        LOG(INFO) << "Verified writing to JSON config";
        return 0;
    } else {
        LOG(ERROR) << "Failed to verify nodes in JSON config";
        return 1;
    }
}
