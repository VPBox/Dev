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

#include <memory>
#include <utility>

#include <android-base/logging.h>
#include <android-base/file.h>
#include <android-base/strings.h>

#include "dso.h"
#include "event_attr.h"
#include "event_type.h"
#include "record_file.h"
#include "thread_tree.h"
#include "tracing.h"
#include "utils.h"

class ReportLib;

extern "C" {

#define EXPORT __attribute__((visibility("default")))

struct Sample {
  uint64_t ip;
  uint32_t pid;
  uint32_t tid;
  const char* thread_comm;
  uint64_t time;
  uint32_t in_kernel;
  uint32_t cpu;
  uint64_t period;
};

struct TracingFieldFormat {
  const char* name;
  uint32_t offset;
  uint32_t elem_size;
  uint32_t elem_count;
  uint32_t is_signed;
};

struct TracingDataFormat {
  uint32_t size;
  uint32_t field_count;
  TracingFieldFormat* fields;
};

struct Event {
  const char* name;
  TracingDataFormat tracing_data_format;
};

struct Mapping {
  uint64_t start;
  uint64_t end;
  uint64_t pgoff;
};

struct SymbolEntry {
  const char* dso_name;
  uint64_t vaddr_in_file;
  const char* symbol_name;
  uint64_t symbol_addr;
  uint64_t symbol_len;
  Mapping* mapping;
};

struct CallChainEntry {
  uint64_t ip;
  SymbolEntry symbol;
};

struct CallChain {
  uint32_t nr;
  CallChainEntry* entries;
};

struct FeatureSection {
  const char* data;
  uint32_t data_size;
};

// Create a new instance,
// pass the instance to the other functions below.
ReportLib* CreateReportLib() EXPORT;
void DestroyReportLib(ReportLib* report_lib) EXPORT;

// Set log severity, different levels are:
// verbose, debug, info, warning, error, fatal.
bool SetLogSeverity(ReportLib* report_lib, const char* log_level) EXPORT;
bool SetSymfs(ReportLib* report_lib, const char* symfs_dir) EXPORT;
bool SetRecordFile(ReportLib* report_lib, const char* record_file) EXPORT;
bool SetKallsymsFile(ReportLib* report_lib, const char* kallsyms_file) EXPORT;
void ShowIpForUnknownSymbol(ReportLib* report_lib) EXPORT;
void ShowArtFrames(ReportLib* report_lib, bool show) EXPORT;

Sample* GetNextSample(ReportLib* report_lib) EXPORT;
Event* GetEventOfCurrentSample(ReportLib* report_lib) EXPORT;
SymbolEntry* GetSymbolOfCurrentSample(ReportLib* report_lib) EXPORT;
CallChain* GetCallChainOfCurrentSample(ReportLib* report_lib) EXPORT;
const char* GetTracingDataOfCurrentSample(ReportLib* report_lib) EXPORT;

const char* GetBuildIdForPath(ReportLib* report_lib, const char* path) EXPORT;
FeatureSection* GetFeatureSection(ReportLib* report_lib, const char* feature_name) EXPORT;
}

struct EventInfo {
  perf_event_attr attr;
  std::string name;

  struct TracingInfo {
    TracingDataFormat data_format;
    std::vector<std::string> field_names;
    std::vector<TracingFieldFormat> fields;
  } tracing_info;
};

class ReportLib {
 public:
  ReportLib()
      : log_severity_(
            new android::base::ScopedLogSeverity(android::base::INFO)),
        record_filename_("perf.data"),
        current_thread_(nullptr),
        trace_offcpu_(false),
        show_art_frames_(false) {
  }

  bool SetLogSeverity(const char* log_level);

  bool SetSymfs(const char* symfs_dir) { return Dso::SetSymFsDir(symfs_dir); }

  bool SetRecordFile(const char* record_file) {
    record_filename_ = record_file;
    return true;
  }

  bool SetKallsymsFile(const char* kallsyms_file);

  void ShowIpForUnknownSymbol() { thread_tree_.ShowIpForUnknownSymbol(); }
  void ShowArtFrames(bool show) { show_art_frames_ = show; }

  Sample* GetNextSample();
  Event* GetEventOfCurrentSample() { return &current_event_; }
  SymbolEntry* GetSymbolOfCurrentSample() { return current_symbol_; }
  CallChain* GetCallChainOfCurrentSample() { return &current_callchain_; }
  const char* GetTracingDataOfCurrentSample() { return current_tracing_data_; }

  const char* GetBuildIdForPath(const char* path);
  FeatureSection* GetFeatureSection(const char* feature_name);

 private:
  void SetCurrentSample();
  const EventInfo* FindEventOfCurrentSample();
  void CreateEvents();

  bool OpenRecordFileIfNecessary();
  Mapping* AddMapping(const MapEntry& map);

  std::unique_ptr<android::base::ScopedLogSeverity> log_severity_;
  std::string record_filename_;
  std::unique_ptr<RecordFileReader> record_file_reader_;
  ThreadTree thread_tree_;
  std::unique_ptr<SampleRecord> current_record_;
  const ThreadEntry* current_thread_;
  Sample current_sample_;
  Event current_event_;
  SymbolEntry* current_symbol_;
  CallChain current_callchain_;
  const char* current_tracing_data_;
  std::vector<std::unique_ptr<Mapping>> current_mappings_;
  std::vector<CallChainEntry> callchain_entries_;
  std::string build_id_string_;
  std::vector<EventInfo> events_;
  std::unique_ptr<ScopedEventTypes> scoped_event_types_;
  bool trace_offcpu_;
  std::unordered_map<pid_t, std::unique_ptr<SampleRecord>> next_sample_cache_;
  FeatureSection feature_section_;
  std::vector<char> feature_section_data_;
  bool show_art_frames_;
  std::unique_ptr<Tracing> tracing_;
};

bool ReportLib::SetLogSeverity(const char* log_level) {
  android::base::LogSeverity severity;
  if (!GetLogSeverity(log_level, &severity)) {
    LOG(ERROR) << "Unknown log severity: " << log_level;
    return false;
  }
  log_severity_ = nullptr;
  log_severity_.reset(new android::base::ScopedLogSeverity(severity));
  return true;
}

bool ReportLib::SetKallsymsFile(const char* kallsyms_file) {
  std::string kallsyms;
  if (!android::base::ReadFileToString(kallsyms_file, &kallsyms)) {
    LOG(WARNING) << "Failed to read in kallsyms file from " << kallsyms_file;
    return false;
  }
  Dso::SetKallsyms(std::move(kallsyms));
  return true;
}

bool ReportLib::OpenRecordFileIfNecessary() {
  if (record_file_reader_ == nullptr) {
    record_file_reader_ = RecordFileReader::CreateInstance(record_filename_);
    if (record_file_reader_ == nullptr) {
      return false;
    }
    record_file_reader_->LoadBuildIdAndFileFeatures(thread_tree_);
    std::unordered_map<std::string, std::string> meta_info_map;
    if (record_file_reader_->HasFeature(PerfFileFormat::FEAT_META_INFO) &&
        !record_file_reader_->ReadMetaInfoFeature(&meta_info_map)) {
      return false;
    }
    auto it = meta_info_map.find("event_type_info");
    if (it != meta_info_map.end()) {
      scoped_event_types_.reset(new ScopedEventTypes(it->second));
    }
    it = meta_info_map.find("trace_offcpu");
    if (it != meta_info_map.end()) {
      trace_offcpu_ = it->second == "true";
    }
  }
  return true;
}

Sample* ReportLib::GetNextSample() {
  if (!OpenRecordFileIfNecessary()) {
    return nullptr;
  }
  while (true) {
    std::unique_ptr<Record> record;
    if (!record_file_reader_->ReadRecord(record)) {
      return nullptr;
    }
    if (record == nullptr) {
      return nullptr;
    }
    thread_tree_.Update(*record);
    if (record->type() == PERF_RECORD_SAMPLE) {
      if (trace_offcpu_) {
        SampleRecord* r = static_cast<SampleRecord*>(record.release());
        auto it = next_sample_cache_.find(r->tid_data.tid);
        if (it == next_sample_cache_.end()) {
          next_sample_cache_[r->tid_data.tid].reset(r);
          continue;
        } else {
          record.reset(it->second.release());
          it->second.reset(r);
        }
      }
      current_record_.reset(static_cast<SampleRecord*>(record.release()));
      break;
    } else if (record->type() == PERF_RECORD_TRACING_DATA ||
               record->type() == SIMPLE_PERF_RECORD_TRACING_DATA) {
      const auto& r = *static_cast<TracingDataRecord*>(record.get());
      tracing_.reset(new Tracing(std::vector<char>(r.data, r.data + r.data_size)));
    }
  }
  SetCurrentSample();
  return &current_sample_;
}

void ReportLib::SetCurrentSample() {
  current_mappings_.clear();
  callchain_entries_.clear();
  SampleRecord& r = *current_record_;
  current_sample_.ip = r.ip_data.ip;
  current_sample_.pid = r.tid_data.pid;
  current_sample_.tid = r.tid_data.tid;
  current_thread_ = thread_tree_.FindThreadOrNew(r.tid_data.pid, r.tid_data.tid);
  current_sample_.thread_comm = current_thread_->comm;
  current_sample_.time = r.time_data.time;
  current_sample_.in_kernel = r.InKernel();
  current_sample_.cpu = r.cpu_data.cpu;
  if (trace_offcpu_) {
    uint64_t next_time = std::max(next_sample_cache_[r.tid_data.tid]->time_data.time,
                                  r.time_data.time + 1);
    current_sample_.period = next_time - r.time_data.time;
  } else {
    current_sample_.period = r.period_data.period;
  }

  size_t kernel_ip_count;
  std::vector<uint64_t> ips = r.GetCallChain(&kernel_ip_count);
  std::vector<std::pair<uint64_t, const MapEntry*>> ip_maps;
  bool near_java_method = false;
  auto is_map_for_interpreter = [](const MapEntry* map) {
    return android::base::EndsWith(map->dso->Path(), "/libart.so");
  };
  for (size_t i = 0; i < ips.size(); ++i) {
    const MapEntry* map = thread_tree_.FindMap(current_thread_, ips[i], i < kernel_ip_count);
    if (!show_art_frames_) {
      // Remove interpreter frames both before and after the Java frame.
      if (map->dso->IsForJavaMethod()) {
        near_java_method = true;
        while (!ip_maps.empty() && is_map_for_interpreter(ip_maps.back().second)) {
          ip_maps.pop_back();
        }
      } else if (is_map_for_interpreter(map)){
        if (near_java_method) {
          continue;
        }
      } else {
        near_java_method = false;
      }
    }
    ip_maps.push_back(std::make_pair(ips[i], map));
  }
  for (auto& pair : ip_maps) {
    uint64_t ip = pair.first;
    const MapEntry* map = pair.second;
    uint64_t vaddr_in_file;
    const Symbol* symbol = thread_tree_.FindSymbol(map, ip, &vaddr_in_file);
    CallChainEntry entry;
    entry.ip = ip;
    entry.symbol.dso_name = map->dso->Path().c_str();
    entry.symbol.vaddr_in_file = vaddr_in_file;
    entry.symbol.symbol_name = symbol->DemangledName();
    entry.symbol.symbol_addr = symbol->addr;
    entry.symbol.symbol_len = symbol->len;
    entry.symbol.mapping = AddMapping(*map);
    callchain_entries_.push_back(entry);
  }
  current_sample_.ip = callchain_entries_[0].ip;
  current_symbol_ = &(callchain_entries_[0].symbol);
  current_callchain_.nr = callchain_entries_.size() - 1;
  current_callchain_.entries = &callchain_entries_[1];
  const EventInfo* event = FindEventOfCurrentSample();
  current_event_.name = event->name.c_str();
  current_event_.tracing_data_format = event->tracing_info.data_format;
  if (current_event_.tracing_data_format.size > 0u && (r.sample_type & PERF_SAMPLE_RAW)) {
    CHECK_GE(r.raw_data.size, current_event_.tracing_data_format.size);
    current_tracing_data_ = r.raw_data.data;
  } else {
    current_tracing_data_ = nullptr;
  }
}

const EventInfo* ReportLib::FindEventOfCurrentSample() {
  if (events_.empty()) {
    CreateEvents();
  }
  size_t attr_index;
  if (trace_offcpu_) {
    // For trace-offcpu, we don't want to show event sched:sched_switch.
    attr_index = 0;
  } else {
    attr_index = record_file_reader_->GetAttrIndexOfRecord(current_record_.get());
  }
  return &events_[attr_index];
}

void ReportLib::CreateEvents() {
  std::vector<EventAttrWithId> attrs = record_file_reader_->AttrSection();
  events_.resize(attrs.size());
  for (size_t i = 0; i < attrs.size(); ++i) {
    events_[i].attr = *attrs[i].attr;
    events_[i].name = GetEventNameByAttr(events_[i].attr);
    EventInfo::TracingInfo& tracing_info = events_[i].tracing_info;
    if (events_[i].attr.type == PERF_TYPE_TRACEPOINT && tracing_) {
      TracingFormat format = tracing_->GetTracingFormatHavingId(events_[i].attr.config);
      tracing_info.field_names.resize(format.fields.size());
      tracing_info.fields.resize(format.fields.size());
      for (size_t i = 0; i < format.fields.size(); ++i) {
        tracing_info.field_names[i] = format.fields[i].name;
        TracingFieldFormat& field = tracing_info.fields[i];
        field.name = tracing_info.field_names[i].c_str();
        field.offset = format.fields[i].offset;
        field.elem_size = format.fields[i].elem_size;
        field.elem_count = format.fields[i].elem_count;
        field.is_signed = format.fields[i].is_signed;
      }
      if (tracing_info.fields.empty()) {
        tracing_info.data_format.size = 0;
      } else {
        TracingFieldFormat& field = tracing_info.fields.back();
        tracing_info.data_format.size = field.offset + field.elem_size * field.elem_count;
      }
      tracing_info.data_format.field_count = tracing_info.fields.size();
      tracing_info.data_format.fields = &tracing_info.fields[0];
    } else {
      tracing_info.data_format.size = 0;
      tracing_info.data_format.field_count = 0;
      tracing_info.data_format.fields = nullptr;
    }
  }
}

Mapping* ReportLib::AddMapping(const MapEntry& map) {
  current_mappings_.emplace_back(std::unique_ptr<Mapping>(new Mapping));
  Mapping* mapping = current_mappings_.back().get();
  mapping->start = map.start_addr;
  mapping->end = map.start_addr + map.len;
  mapping->pgoff = map.pgoff;
  return mapping;
}

const char* ReportLib::GetBuildIdForPath(const char* path) {
  if (!OpenRecordFileIfNecessary()) {
    build_id_string_.clear();
    return build_id_string_.c_str();
  }
  BuildId build_id = Dso::FindExpectedBuildIdForPath(path);
  if (build_id.IsEmpty()) {
    build_id_string_.clear();
  } else {
    build_id_string_ = build_id.ToString();
  }
  return build_id_string_.c_str();
}

FeatureSection* ReportLib::GetFeatureSection(const char* feature_name) {
  if (!OpenRecordFileIfNecessary()) {
    return nullptr;
  }
  int feature = PerfFileFormat::GetFeatureId(feature_name);
  if (feature == -1 || !record_file_reader_->ReadFeatureSection(feature, &feature_section_data_)) {
    return nullptr;
  }
  feature_section_.data = feature_section_data_.data();
  feature_section_.data_size = feature_section_data_.size();
  return &feature_section_;
}

// Exported methods working with a client created instance
ReportLib* CreateReportLib() {
  return new ReportLib();
}

void DestroyReportLib(ReportLib* report_lib) {
  delete report_lib;
}

bool SetLogSeverity(ReportLib* report_lib, const char* log_level) {
  return report_lib->SetLogSeverity(log_level);
}

bool SetSymfs(ReportLib* report_lib, const char* symfs_dir) {
  return report_lib->SetSymfs(symfs_dir);
}

bool SetRecordFile(ReportLib* report_lib, const char* record_file) {
  return report_lib->SetRecordFile(record_file);
}

void ShowIpForUnknownSymbol(ReportLib* report_lib) {
  return report_lib->ShowIpForUnknownSymbol();
}

void ShowArtFrames(ReportLib* report_lib, bool show) {
  return report_lib->ShowArtFrames(show);
}

bool SetKallsymsFile(ReportLib* report_lib, const char* kallsyms_file) {
  return report_lib->SetKallsymsFile(kallsyms_file);
}

Sample* GetNextSample(ReportLib* report_lib) {
  return report_lib->GetNextSample();
}

Event* GetEventOfCurrentSample(ReportLib* report_lib) {
  return report_lib->GetEventOfCurrentSample();
}

SymbolEntry* GetSymbolOfCurrentSample(ReportLib* report_lib) {
  return report_lib->GetSymbolOfCurrentSample();
}

CallChain* GetCallChainOfCurrentSample(ReportLib* report_lib) {
  return report_lib->GetCallChainOfCurrentSample();
}

const char* GetTracingDataOfCurrentSample(ReportLib* report_lib) {
  return report_lib->GetTracingDataOfCurrentSample();
}

const char* GetBuildIdForPath(ReportLib* report_lib, const char* path) {
  return report_lib->GetBuildIdForPath(path);
}

FeatureSection* GetFeatureSection(ReportLib* report_lib, const char* feature_name) {
  return report_lib->GetFeatureSection(feature_name);
}
