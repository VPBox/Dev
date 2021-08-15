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

#ifndef SIMPLE_PERF_EVENT_H_
#define SIMPLE_PERF_EVENT_H_

#include <stdint.h>
#include <strings.h>
#include <memory>
#include <set>
#include <string>
#include <vector>

// A uint32_t value far from 0 is picked, so it is unlikely to conflict with further
// PERF_TYPE_* events.
static constexpr uint32_t SIMPLEPERF_TYPE_USER_SPACE_SAMPLERS = 32768;

enum {
  SIMPLEPERF_CONFIG_INPLACE_SAMPLER,
};

// EventType represents one type of event, like cpu_cycle_event, cache_misses_event.
// The user knows one event type by its name, and the kernel knows one event type by its
// (type, config) pair. EventType connects the two representations, and tells the user if
// the event type is supported by the kernel.

struct EventType {
  EventType(const std::string& name, uint32_t type, uint64_t config,
            const std::string& description, const std::string& limited_arch)
      : name(name), type(type), config(config), description(description),
        limited_arch(limited_arch) {
  }

  EventType() : type(0), config(0) {
  }

  bool operator<(const EventType& other) const {
    return strcasecmp(name.c_str(), other.name.c_str()) < 0;
  }

  std::string name;
  uint32_t type;
  uint64_t config;
  std::string description;
  std::string limited_arch;
};

bool SetTracepointEventsFilePath(const std::string& filepath);
std::string GetTracepointEvents();

// Used to temporarily change event types returned by GetAllEventTypes().
class ScopedEventTypes {
 public:
  static std::string BuildString(const std::vector<const EventType*>& event_types);

  ScopedEventTypes(const std::string& event_type_str);
  ~ScopedEventTypes();

 private:
  std::set<EventType> saved_event_types_;
};

const std::set<EventType>& GetAllEventTypes();
const EventType* FindEventTypeByName(const std::string& name, bool report_error = true);

struct EventTypeAndModifier {
  std::string name;
  EventType event_type;
  std::string modifier;
  bool exclude_user;
  bool exclude_kernel;
  bool exclude_hv;
  bool exclude_host;
  bool exclude_guest;
  int precise_ip : 2;

  EventTypeAndModifier()
      : exclude_user(false),
        exclude_kernel(false),
        exclude_hv(false),
        exclude_host(false),
        exclude_guest(false),
        precise_ip(0) {
  }
};

std::unique_ptr<EventTypeAndModifier> ParseEventType(const std::string& event_type_str);

#endif  // SIMPLE_PERF_EVENT_H_
