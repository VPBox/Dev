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

#ifndef SIMPLE_PERF_EVENT_ATTR_H_
#define SIMPLE_PERF_EVENT_ATTR_H_

#include <stddef.h>

#include <string>
#include <vector>

#include "perf_event.h"

struct EventType;

struct EventAttrWithId {
  const perf_event_attr* attr;
  std::vector<uint64_t> ids;
};

perf_event_attr CreateDefaultPerfEventAttr(const EventType& event_type);
void DumpPerfEventAttr(const perf_event_attr& attr, size_t indent = 0);
bool GetCommonEventIdPositionsForAttrs(std::vector<perf_event_attr>& attrs,
                                       size_t* event_id_pos_in_sample_records,
                                       size_t* event_id_reverse_pos_in_non_sample_records);
bool IsTimestampSupported(const perf_event_attr& attr);
bool IsCpuSupported(const perf_event_attr& attr);
// Return event name with modifier if the event is found, otherwise return "unknown".
std::string GetEventNameByAttr(const perf_event_attr& attr);

#endif  // SIMPLE_PERF_EVENT_ATTR_H_
