/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "event_type.h"

#include <inttypes.h>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "event_attr.h"
#include "utils.h"

#define EVENT_TYPE_TABLE_ENTRY(name, type, config, description, limited_arch) \
          {name, type, config, description, limited_arch},

static const std::vector<EventType> static_event_type_array = {
#include "event_type_table.h"
};

static std::string tracepoint_events;

bool SetTracepointEventsFilePath(const std::string& filepath) {
  if (!android::base::ReadFileToString(filepath, &tracepoint_events)) {
    PLOG(ERROR) << "Failed to read " << filepath;
    return false;
  }
  return true;
}

std::string GetTracepointEvents() {
  std::string result;
  for (auto& event : GetAllEventTypes()) {
    if (event.type != PERF_TYPE_TRACEPOINT) {
      continue;
    }
    if (!result.empty()) {
      result.push_back('\n');
    }
    result += android::base::StringPrintf("%s %" PRIu64, event.name.c_str(), event.config);
  }
  return result;
}

static std::vector<EventType> GetTracepointEventTypesFromString(const std::string& s) {
  std::vector<EventType> result;
  for (auto& line : android::base::Split(s, "\n")) {
    std::vector<std::string> items = android::base::Split(line, " ");
    CHECK_EQ(items.size(), 2u);
    std::string event_name = items[0];
    uint64_t id;
    CHECK(android::base::ParseUint(items[1].c_str(), &id));
    result.push_back(EventType(event_name, PERF_TYPE_TRACEPOINT, id, "", ""));
  }
  return result;
}

static std::vector<EventType> GetTracepointEventTypesFromTraceFs() {
  std::vector<EventType> result;
  const std::string tracepoint_dirname = "/sys/kernel/debug/tracing/events";
  for (const auto& system_name : GetSubDirs(tracepoint_dirname)) {
    std::string system_path = tracepoint_dirname + "/" + system_name;
    for (const auto& event_name : GetSubDirs(system_path)) {
      std::string id_path = system_path + "/" + event_name + "/id";
      std::string id_content;
      if (!android::base::ReadFileToString(id_path, &id_content)) {
        continue;
      }
      char* endptr;
      uint64_t id = strtoull(id_content.c_str(), &endptr, 10);
      if (endptr == id_content.c_str()) {
        LOG(DEBUG) << "unexpected id '" << id_content << "' in " << id_path;
        continue;
      }
      result.push_back(EventType(system_name + ":" + event_name, PERF_TYPE_TRACEPOINT, id, "", ""));
    }
  }
  return result;
}

static std::vector<EventType> GetTracepointEventTypes() {
  std::vector<EventType> result;
  if (!tracepoint_events.empty()) {
    result = GetTracepointEventTypesFromString(tracepoint_events);
  } else {
    result = GetTracepointEventTypesFromTraceFs();
  }
  std::sort(result.begin(), result.end(),
            [](const EventType& type1, const EventType& type2) { return type1.name < type2.name; });
  return result;
}

static std::set<EventType> g_event_types;

std::string ScopedEventTypes::BuildString(const std::vector<const EventType*>& event_types) {
  std::string result;
  for (auto type : event_types) {
    if (!result.empty()) {
      result.push_back('\n');
    }
    result += android::base::StringPrintf("%s,%u,%" PRIu64, type->name.c_str(), type->type,
                                          type->config);
  }
  return result;
}

ScopedEventTypes::ScopedEventTypes(const std::string& event_type_str) {
  saved_event_types_ = std::move(g_event_types);
  g_event_types.clear();
  for (auto& s : android::base::Split(event_type_str, "\n")) {
    std::string name = s.substr(0, s.find(','));
    uint32_t type;
    uint64_t config;
    sscanf(s.c_str() + name.size(), ",%u,%" PRIu64, &type, &config);
    g_event_types.emplace(name, type, config, "", "");
  }
}

ScopedEventTypes::~ScopedEventTypes() {
  g_event_types = std::move(saved_event_types_);
}

const std::set<EventType>& GetAllEventTypes() {
  if (g_event_types.empty()) {
    g_event_types.insert(static_event_type_array.begin(), static_event_type_array.end());
    std::vector<EventType> tracepoint_array = GetTracepointEventTypes();
    g_event_types.insert(tracepoint_array.begin(), tracepoint_array.end());
  }
  return g_event_types;
}

const EventType* FindEventTypeByName(const std::string& name, bool report_error) {
  const auto& event_types = GetAllEventTypes();
  auto it = event_types.find(EventType(name, 0, 0, "", ""));
  if (it != event_types.end()) {
    return &*it;
  }
  if (!name.empty() && name[0] == 'r') {
    char* end;
    uint64_t config = strtoull(&name[1], &end, 16);
    if (end != &name[1] && *end == '\0') {
      auto result = g_event_types.emplace(name, PERF_TYPE_RAW, config, "", "");
      CHECK(result.second);
      return &*(result.first);
    }
  }
  if (report_error) {
    LOG(ERROR) << "Unknown event_type '" << name
               << "', try `simpleperf list` to list all possible event type names";
  }
  return nullptr;
}

std::unique_ptr<EventTypeAndModifier> ParseEventType(const std::string& event_type_str) {
  static std::string modifier_characters = "ukhGHp";
  std::unique_ptr<EventTypeAndModifier> event_type_modifier(new EventTypeAndModifier);
  event_type_modifier->name = event_type_str;
  std::string event_type_name = event_type_str;
  std::string modifier;
  size_t comm_pos = event_type_str.rfind(':');
  if (comm_pos != std::string::npos) {
    bool match_modifier = true;
    for (size_t i = comm_pos + 1; i < event_type_str.size(); ++i) {
      char c = event_type_str[i];
      if (c != ' ' && modifier_characters.find(c) == std::string::npos) {
        match_modifier = false;
        break;
      }
    }
    if (match_modifier) {
      event_type_name = event_type_str.substr(0, comm_pos);
      modifier = event_type_str.substr(comm_pos + 1);
    }
  }
  const EventType* event_type = FindEventTypeByName(event_type_name);
  if (event_type == nullptr) {
    // Try if the modifier belongs to the event type name, like some tracepoint events.
    if (!modifier.empty()) {
      event_type_name = event_type_str;
      modifier.clear();
      event_type = FindEventTypeByName(event_type_name);
    }
    if (event_type == nullptr) {
      return nullptr;
    }
  }
  event_type_modifier->event_type = *event_type;
  if (modifier.find_first_of("ukh") != std::string::npos) {
    event_type_modifier->exclude_user = true;
    event_type_modifier->exclude_kernel = true;
    event_type_modifier->exclude_hv = true;
  }
  if (modifier.find_first_of("GH") != std::string::npos) {
    event_type_modifier->exclude_guest = true;
    event_type_modifier->exclude_host = true;
  }

  for (auto& c : modifier) {
    switch (c) {
      case 'u':
        event_type_modifier->exclude_user = false;
        break;
      case 'k':
        event_type_modifier->exclude_kernel = false;
        break;
      case 'h':
        event_type_modifier->exclude_hv = false;
        break;
      case 'G':
        event_type_modifier->exclude_guest = false;
        break;
      case 'H':
        event_type_modifier->exclude_host = false;
        break;
      case 'p':
        event_type_modifier->precise_ip++;
        break;
      case ' ':
        break;
      default:
        LOG(ERROR) << "Unknown event type modifier '" << c << "'";
    }
  }
  event_type_modifier->modifier = modifier;
  return event_type_modifier;
}
