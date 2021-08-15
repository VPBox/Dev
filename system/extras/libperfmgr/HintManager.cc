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
 * See the License for the specic language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "libperfmgr"

#include <algorithm>
#include <set>

#include <android-base/file.h>
#include <android-base/logging.h>

#include <json/reader.h>
#include <json/value.h>

#include "perfmgr/FileNode.h"
#include "perfmgr/HintManager.h"
#include "perfmgr/PropertyNode.h"

namespace android {
namespace perfmgr {

bool HintManager::ValidateHint(const std::string& hint_type) const {
    if (nm_.get() == nullptr) {
        LOG(ERROR) << "NodeLooperThread not present";
        return false;
    }
    if (actions_.find(hint_type) == actions_.end()) {
        LOG(ERROR) << "PowerHint type not present in actions: " << hint_type;
        return false;
    }
    return true;
}

bool HintManager::DoHint(const std::string& hint_type) {
    LOG(VERBOSE) << "Do Powerhint: " << hint_type;
    return ValidateHint(hint_type)
               ? nm_->Request(actions_.at(hint_type), hint_type)
               : false;
}

bool HintManager::DoHint(const std::string& hint_type,
                         std::chrono::milliseconds timeout_ms_override) {
    LOG(VERBOSE) << "Do Powerhint: " << hint_type << " for "
                 << timeout_ms_override.count() << "ms";
    if (!ValidateHint(hint_type)) {
        return false;
    }
    std::vector<NodeAction> actions_override = actions_.at(hint_type);
    for (auto& action : actions_override) {
        action.timeout_ms = timeout_ms_override;
    }
    return nm_->Request(actions_override, hint_type);
}

bool HintManager::EndHint(const std::string& hint_type) {
    LOG(VERBOSE) << "End Powerhint: " << hint_type;
    return ValidateHint(hint_type)
               ? nm_->Cancel(actions_.at(hint_type), hint_type)
               : false;
}

bool HintManager::IsRunning() const {
    return (nm_.get() == nullptr) ? false : nm_->isRunning();
}

std::vector<std::string> HintManager::GetHints() const {
    std::vector<std::string> hints;
    for (auto const& action : actions_) {
        hints.push_back(action.first);
    }
    return hints;
}

void HintManager::DumpToFd(int fd) {
    std::string header(
        "========== Begin perfmgr nodes ==========\n"
        "Node Name\t"
        "Node Path\t"
        "Current Index\t"
        "Current Value\n");
    if (!android::base::WriteStringToFd(header, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }
    nm_->DumpToFd(fd);
    std::string footer("==========  End perfmgr nodes  ==========\n");
    if (!android::base::WriteStringToFd(footer, fd)) {
        LOG(ERROR) << "Failed to dump fd: " << fd;
    }
    fsync(fd);
}

std::unique_ptr<HintManager> HintManager::GetFromJSON(
    const std::string& config_path) {
    std::string json_doc;

    if (!android::base::ReadFileToString(config_path, &json_doc)) {
        LOG(ERROR) << "Failed to read JSON config from " << config_path;
        return nullptr;
    }

    std::vector<std::unique_ptr<Node>> nodes = ParseNodes(json_doc);
    if (nodes.empty()) {
        LOG(ERROR) << "Failed to parse Nodes section from " << config_path;
        return nullptr;
    }
    std::map<std::string, std::vector<NodeAction>> actions =
        HintManager::ParseActions(json_doc, nodes);

    if (actions.empty()) {
        LOG(ERROR) << "Failed to parse Actions section from " << config_path;
        return nullptr;
    }

    sp<NodeLooperThread> nm = new NodeLooperThread(std::move(nodes));
    std::unique_ptr<HintManager> hm =
        std::make_unique<HintManager>(std::move(nm), actions);

    LOG(INFO) << "Initialized HintManager from JSON config: " << config_path;
    return hm;
}

std::vector<std::unique_ptr<Node>> HintManager::ParseNodes(
    const std::string& json_doc) {
    // function starts
    std::vector<std::unique_ptr<Node>> nodes_parsed;
    std::set<std::string> nodes_name_parsed;
    std::set<std::string> nodes_path_parsed;
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(json_doc, root)) {
        LOG(ERROR) << "Failed to parse JSON config";
        return nodes_parsed;
    }

    Json::Value nodes = root["Nodes"];
    for (Json::Value::ArrayIndex i = 0; i < nodes.size(); ++i) {
        std::string name = nodes[i]["Name"].asString();
        LOG(VERBOSE) << "Node[" << i << "]'s Name: " << name;
        if (name.empty()) {
            LOG(ERROR) << "Failed to read "
                       << "Node[" << i << "]'s Name";
            nodes_parsed.clear();
            return nodes_parsed;
        }

        auto result = nodes_name_parsed.insert(name);
        if (!result.second) {
            LOG(ERROR) << "Duplicate Node[" << i << "]'s Name";
            nodes_parsed.clear();
            return nodes_parsed;
        }

        std::string path = nodes[i]["Path"].asString();
        LOG(VERBOSE) << "Node[" << i << "]'s Path: " << path;
        if (path.empty()) {
            LOG(ERROR) << "Failed to read "
                       << "Node[" << i << "]'s Path";
            nodes_parsed.clear();
            return nodes_parsed;
        }

        result = nodes_path_parsed.insert(path);
        if (!result.second) {
            LOG(ERROR) << "Duplicate Node[" << i << "]'s Path";
            nodes_parsed.clear();
            return nodes_parsed;
        }

        bool is_file = true;
        std::string node_type = nodes[i]["Type"].asString();
        LOG(VERBOSE) << "Node[" << i << "]'s Type: " << node_type;
        if (node_type.empty()) {
            LOG(ERROR) << "Failed to read "
                       << "Node[" << i << "]'s Type, set to 'File' as default";
        } else if (node_type == "File") {
            is_file = true;
        } else if (node_type == "Property") {
            is_file = false;
        } else {
            LOG(ERROR) << "Invalid Node[" << i
                       << "]'s Type: only File and Property supported.";
            nodes_parsed.clear();
            return nodes_parsed;
        }

        std::vector<RequestGroup> values_parsed;
        std::set<std::string> values_set_parsed;
        Json::Value values = nodes[i]["Values"];
        for (Json::Value::ArrayIndex j = 0; j < values.size(); ++j) {
            std::string value = values[j].asString();
            LOG(VERBOSE) << "Node[" << i << "]'s Value[" << j << "]: " << value;
            auto result = values_set_parsed.insert(value);
            if (!result.second) {
                LOG(ERROR) << "Duplicate value parsed in Node[" << i << "]'s Value[" << j
                           << "]";
                nodes_parsed.clear();
                return nodes_parsed;
            }
            if (is_file && value.empty()) {
                LOG(ERROR) << "Failed to read Node[" << i << "]'s Value[" << j << "]";
                nodes_parsed.clear();
                return nodes_parsed;
            }
            values_parsed.emplace_back(value);
        }
        if (values_parsed.size() < 1) {
            LOG(ERROR) << "Failed to read Node[" << i << "]'s Values";
            nodes_parsed.clear();
            return nodes_parsed;
        }

        Json::UInt64 default_index = values_parsed.size() - 1;
        if (nodes[i]["DefaultIndex"].empty() ||
            !nodes[i]["DefaultIndex"].isUInt64()) {
            LOG(INFO) << "Failed to read Node[" << i
                      << "]'s DefaultIndex, set to last index: "
                      << default_index;
        } else {
            default_index = nodes[i]["DefaultIndex"].asUInt64();
        }
        if (default_index > values_parsed.size() - 1) {
            default_index = values_parsed.size() - 1;
            LOG(ERROR) << "Node[" << i
                       << "]'s DefaultIndex out of bound, max value index: "
                       << default_index;
            nodes_parsed.clear();
            return nodes_parsed;
        }
        LOG(VERBOSE) << "Node[" << i << "]'s DefaultIndex: " << default_index;

        bool reset = false;
        if (nodes[i]["ResetOnInit"].empty() ||
            !nodes[i]["ResetOnInit"].isBool()) {
            LOG(INFO) << "Failed to read Node[" << i
                      << "]'s ResetOnInit, set to 'false'";
        } else {
            reset = nodes[i]["ResetOnInit"].asBool();
        }
        LOG(VERBOSE) << "Node[" << i << "]'s ResetOnInit: " << std::boolalpha
                     << reset << std::noboolalpha;

        if (is_file) {
            bool hold_fd = false;
            if (nodes[i]["HoldFd"].empty() || !nodes[i]["HoldFd"].isBool()) {
                LOG(INFO) << "Failed to read Node[" << i
                          << "]'s HoldFd, set to 'false'";
            } else {
                hold_fd = nodes[i]["HoldFd"].asBool();
            }
            LOG(VERBOSE) << "Node[" << i << "]'s HoldFd: " << std::boolalpha
                         << hold_fd << std::noboolalpha;

            nodes_parsed.emplace_back(std::make_unique<FileNode>(
                name, path, values_parsed,
                static_cast<std::size_t>(default_index), reset, hold_fd));
        } else {
            nodes_parsed.emplace_back(std::make_unique<PropertyNode>(
                name, path, values_parsed,
                static_cast<std::size_t>(default_index), reset));
        }
    }
    LOG(INFO) << nodes_parsed.size() << " Nodes parsed successfully";
    return nodes_parsed;
}

std::map<std::string, std::vector<NodeAction>> HintManager::ParseActions(
    const std::string& json_doc,
    const std::vector<std::unique_ptr<Node>>& nodes) {
    // function starts
    std::map<std::string, std::vector<NodeAction>> actions_parsed;
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(json_doc, root)) {
        LOG(ERROR) << "Failed to parse JSON config";
        return actions_parsed;
    }

    Json::Value actions = root["Actions"];
    std::size_t total_parsed = 0;

    std::map<std::string, std::size_t> nodes_index;
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        nodes_index[nodes[i]->GetName()] = i;
    }

    for (Json::Value::ArrayIndex i = 0; i < actions.size(); ++i) {
        const std::string& hint_type = actions[i]["PowerHint"].asString();
        LOG(VERBOSE) << "Action[" << i << "]'s PowerHint: " << hint_type;
        if (hint_type.empty()) {
            LOG(ERROR) << "Failed to read "
                       << "Action[" << i << "]'s PowerHint";
            actions_parsed.clear();
            return actions_parsed;
        }

        std::string node_name = actions[i]["Node"].asString();
        LOG(VERBOSE) << "Action[" << i << "]'s Node: " << node_name;
        std::size_t node_index;

        if (nodes_index.find(node_name) == nodes_index.end()) {
            LOG(ERROR) << "Failed to find "
                       << "Action[" << i
                       << "]'s Node from Nodes section: [" << node_name << "]";
            actions_parsed.clear();
            return actions_parsed;
        }
        node_index = nodes_index[node_name];

        std::string value_name = actions[i]["Value"].asString();
        LOG(VERBOSE) << "Action[" << i << "]'s Value: " << value_name;
        std::size_t value_index = 0;

        if (!nodes[node_index]->GetValueIndex(value_name, &value_index)) {
            LOG(ERROR) << "Failed to read Action[" << i << "]'s Value";
            LOG(ERROR) << "Action[" << i << "]'s Value " << value_name
                       << " is not defined in Node[" << node_name << "]";
            actions_parsed.clear();
            return actions_parsed;
        }
        LOG(VERBOSE) << "Action[" << i << "]'s ValueIndex: " << value_index;

        Json::UInt64 duration = 0;
        if (actions[i]["Duration"].empty() ||
            !actions[i]["Duration"].isUInt64()) {
            LOG(ERROR) << "Failed to read Action[" << i << "]'s Duration";
            actions_parsed.clear();
            return actions_parsed;
        } else {
            duration = actions[i]["Duration"].asUInt64();
        }
        LOG(VERBOSE) << "Action[" << i << "]'s Duration: " << duration;

        if (actions_parsed.find(hint_type) == actions_parsed.end()) {
            actions_parsed[hint_type] = std::vector<NodeAction>{
                {node_index, value_index, std::chrono::milliseconds(duration)}};
        } else {
            for (const auto& action : actions_parsed[hint_type]) {
                if (action.node_index == node_index) {
                    LOG(ERROR)
                        << "Action[" << i
                        << "]'s NodeIndex is duplicated with another Action";
                    actions_parsed.clear();
                    return actions_parsed;
                }
            }
            actions_parsed[hint_type].emplace_back(
                node_index, value_index, std::chrono::milliseconds(duration));
        }

        ++total_parsed;
    }

    LOG(INFO) << total_parsed << " Actions parsed successfully";

    for (const auto& action : actions_parsed) {
        LOG(INFO) << "PowerHint " << action.first << " has "
                  << action.second.size() << " actions parsed";
    }

    return actions_parsed;
}

}  // namespace perfmgr
}  // namespace android
