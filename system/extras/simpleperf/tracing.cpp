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

#include "tracing.h"

#include <string.h>

#include <map>
#include <string>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "perf_event.h"
#include "utils.h"

const char TRACING_INFO_MAGIC[10] = {23,  8,   68,  't', 'r',
                                     'a', 'c', 'i', 'n', 'g'};

template <class T>
void AppendData(std::vector<char>& data, const T& s) {
  const char* p = reinterpret_cast<const char*>(&s);
  data.insert(data.end(), p, p + sizeof(T));
}

static void AppendData(std::vector<char>& data, const char* s) {
  data.insert(data.end(), s, s + strlen(s) + 1);
}

template <>
void AppendData(std::vector<char>& data, const std::string& s) {
  data.insert(data.end(), s.c_str(), s.c_str() + s.size() + 1);
}

template <>
void MoveFromBinaryFormat(std::string& data, const char*& p) {
  data.clear();
  while (*p != '\0') {
    data.push_back(*p++);
  }
  p++;
}

static void AppendFile(std::vector<char>& data, const std::string& file,
                       uint32_t file_size_bytes = 8) {
  if (file_size_bytes == 8) {
    uint64_t file_size = file.size();
    AppendData(data, file_size);
  } else if (file_size_bytes == 4) {
    uint32_t file_size = file.size();
    AppendData(data, file_size);
  }
  data.insert(data.end(), file.begin(), file.end());
}

static void DetachFile(const char*& p, std::string& file,
                       uint32_t file_size_bytes = 8) {
  uint64_t file_size = ConvertBytesToValue(p, file_size_bytes);
  p += file_size_bytes;
  file.clear();
  file.insert(file.end(), p, p + file_size);
  p += file_size;
}

struct TraceType {
  std::string system;
  std::string name;
};

class TracingFile {
 public:
  TracingFile();
  bool RecordHeaderFiles();
  void RecordFtraceFiles(const std::vector<TraceType>& trace_types);
  bool RecordEventFiles(const std::vector<TraceType>& trace_types);
  bool RecordKallsymsFile();
  bool RecordPrintkFormatsFile();
  std::vector<char> BinaryFormat() const;
  void LoadFromBinary(const std::vector<char>& data);
  void Dump(size_t indent) const;
  std::vector<TracingFormat> LoadTracingFormatsFromEventFiles() const;
  const std::string& GetKallsymsFile() const { return kallsyms_file; }
  uint32_t GetPageSize() const { return page_size; }

 private:
  char magic[10];
  std::string version;
  char endian;
  uint8_t size_of_long;
  uint32_t page_size;
  std::string header_page_file;
  std::string header_event_file;

  std::vector<std::string> ftrace_format_files;
  // pair of system, format_file_data.
  std::vector<std::pair<std::string, std::string>> event_format_files;

  std::string kallsyms_file;
  std::string printk_formats_file;
};

TracingFile::TracingFile() {
  memcpy(magic, TRACING_INFO_MAGIC, sizeof(TRACING_INFO_MAGIC));
  version = "0.5";
  endian = 0;
  size_of_long = static_cast<int>(sizeof(long)); // NOLINT(google-runtime-int)
  page_size = static_cast<uint32_t>(::GetPageSize());
}

bool TracingFile::RecordHeaderFiles() {
  if (!android::base::ReadFileToString(
          "/sys/kernel/debug/tracing/events/header_page", &header_page_file)) {
    PLOG(ERROR)
        << "failed to read /sys/kernel/debug/tracing/events/header_page";
    return false;
  }
  if (!android::base::ReadFileToString(
          "/sys/kernel/debug/tracing/events/header_event",
          &header_event_file)) {
    PLOG(ERROR)
        << "failed to read /sys/kernel/debug/tracing/events/header_event";
    return false;
  }
  return true;
}

void TracingFile::RecordFtraceFiles(const std::vector<TraceType>& trace_types) {
  for (const auto& type : trace_types) {
    std::string format_path = android::base::StringPrintf(
        "/sys/kernel/debug/tracing/events/ftrace/%s/format", type.name.c_str());
    std::string format_data;
    if (android::base::ReadFileToString(format_path, &format_data)) {
      ftrace_format_files.push_back(std::move(format_data));
    }
  }
}

bool TracingFile::RecordEventFiles(const std::vector<TraceType>& trace_types) {
  for (const auto& type : trace_types) {
    std::string format_path = android::base::StringPrintf(
        "/sys/kernel/debug/tracing/events/%s/%s/format", type.system.c_str(),
        type.name.c_str());
    std::string format_data;
    if (!android::base::ReadFileToString(format_path, &format_data)) {
      PLOG(ERROR) << "failed to read " << format_path;
      return false;
    }
    event_format_files.push_back(
        std::make_pair(type.system, std::move(format_data)));
  }
  return true;
}

bool TracingFile::RecordPrintkFormatsFile() {
  if (!android::base::ReadFileToString(
          "/sys/kernel/debug/tracing/printk_formats", &printk_formats_file)) {
    PLOG(ERROR) << "failed to read /sys/kernel/debug/tracing/printk_formats";
    return false;
  }
  return true;
}

std::vector<char> TracingFile::BinaryFormat() const {
  std::vector<char> ret;
  ret.insert(ret.end(), magic, magic + sizeof(magic));
  AppendData(ret, version);
  ret.push_back(endian);
  AppendData(ret, size_of_long);
  AppendData(ret, page_size);
  AppendData(ret, "header_page");
  AppendFile(ret, header_page_file);
  AppendData(ret, "header_event");
  AppendFile(ret, header_event_file);
  int count = static_cast<int>(ftrace_format_files.size());
  AppendData(ret, count);
  for (const auto& format : ftrace_format_files) {
    AppendFile(ret, format);
  }
  count = static_cast<int>(event_format_files.size());
  AppendData(ret, count);
  for (const auto& pair : event_format_files) {
    AppendData(ret, pair.first);
    AppendData(ret, 1);
    AppendFile(ret, pair.second);
  }
  AppendFile(ret, kallsyms_file, 4);
  AppendFile(ret, printk_formats_file, 4);
  return ret;
}

void TracingFile::LoadFromBinary(const std::vector<char>& data) {
  const char* p = data.data();
  const char* end = data.data() + data.size();
  CHECK(memcmp(p, magic, sizeof(magic)) == 0);
  p += sizeof(magic);
  MoveFromBinaryFormat(version, p);
  MoveFromBinaryFormat(endian, p);
  MoveFromBinaryFormat(size_of_long, p);
  MoveFromBinaryFormat(page_size, p);
  std::string filename;
  MoveFromBinaryFormat(filename, p);
  CHECK_EQ(filename, "header_page");
  DetachFile(p, header_page_file);
  MoveFromBinaryFormat(filename, p);
  CHECK_EQ(filename, "header_event");
  DetachFile(p, header_event_file);
  uint32_t count;
  MoveFromBinaryFormat(count, p);
  ftrace_format_files.resize(count);
  for (uint32_t i = 0; i < count; ++i) {
    DetachFile(p, ftrace_format_files[i]);
  }
  MoveFromBinaryFormat(count, p);
  event_format_files.clear();
  for (uint32_t i = 0; i < count; ++i) {
    std::string system;
    MoveFromBinaryFormat(system, p);
    uint32_t count_in_system;
    MoveFromBinaryFormat(count_in_system, p);
    for (uint32_t i = 0; i < count_in_system; ++i) {
      std::string format;
      DetachFile(p, format);
      event_format_files.push_back(std::make_pair(system, std::move(format)));
    }
  }
  DetachFile(p, kallsyms_file, 4);
  DetachFile(p, printk_formats_file, 4);
  CHECK_EQ(p, end);
}

void TracingFile::Dump(size_t indent) const {
  PrintIndented(indent, "tracing data:\n");
  PrintIndented(indent + 1, "magic: ");
  for (size_t i = 0; i < 3u; ++i) {
    printf("0x%x ", magic[i]);
  }
  for (size_t i = 3; i < sizeof(magic); ++i) {
    printf("%c", magic[i]);
  }
  printf("\n");
  PrintIndented(indent + 1, "version: %s\n", version.c_str());
  PrintIndented(indent + 1, "endian: %d\n", endian);
  PrintIndented(indent + 1, "header_page:\n%s\n\n", header_page_file.c_str());
  PrintIndented(indent + 1, "header_event:\n%s\n\n", header_event_file.c_str());
  for (size_t i = 0; i < ftrace_format_files.size(); ++i) {
    PrintIndented(indent + 1, "ftrace format file %zu/%zu:\n%s\n\n", i + 1,
                  ftrace_format_files.size(), ftrace_format_files[i].c_str());
  }
  for (size_t i = 0; i < event_format_files.size(); ++i) {
    PrintIndented(indent + 1, "event format file %zu/%zu %s:\n%s\n\n", i + 1,
                  event_format_files.size(),
                  event_format_files[i].first.c_str(),
                  event_format_files[i].second.c_str());
  }
  PrintIndented(indent + 1, "kallsyms:\n%s\n\n", kallsyms_file.c_str());
  PrintIndented(indent + 1, "printk_formats:\n%s\n\n",
                printk_formats_file.c_str());
}

enum class FormatParsingState {
  READ_NAME,
  READ_ID,
  READ_FIELDS,
  READ_PRINTFMT,
};

// Parse lines like: field:char comm[16]; offset:8; size:16;  signed:1;
static TracingField ParseTracingField(const std::string& s) {
  TracingField field;
  size_t start = 0;
  std::string name;
  std::string value;
  for (size_t i = 0; i < s.size(); ++i) {
    if (!isspace(s[i]) && (i == 0 || isspace(s[i - 1]))) {
      start = i;
    } else if (s[i] == ':') {
      name = s.substr(start, i - start);
      start = i + 1;
    } else if (s[i] == ';') {
      value = s.substr(start, i - start);
      if (name == "field") {
        // Parse value with brackets like "comm[16]", or just a field name.
        size_t left_bracket_pos = value.find('[');
        if (left_bracket_pos == std::string::npos) {
          field.name = value;
          field.elem_count = 1;
        } else {
          field.name = value.substr(0, left_bracket_pos);
          field.elem_count = 1;
          size_t right_bracket_pos = value.find(']', left_bracket_pos);
          if (right_bracket_pos != std::string::npos) {
            size_t len = right_bracket_pos - left_bracket_pos - 1;
            size_t elem_count;
            // Array size may not be a number, like field:u32 rates[IEEE80211_NUM_BANDS].
            if (android::base::ParseUint(value.substr(left_bracket_pos + 1, len), &elem_count)) {
              field.elem_count = elem_count;
            }
          }
        }
      } else if (name == "offset") {
        field.offset =
            static_cast<size_t>(strtoull(value.c_str(), nullptr, 10));
      } else if (name == "size") {
        size_t size = static_cast<size_t>(strtoull(value.c_str(), nullptr, 10));
        CHECK_EQ(size % field.elem_count, 0u);
        field.elem_size = size / field.elem_count;
      } else if (name == "signed") {
        int is_signed = static_cast<int>(strtoull(value.c_str(), nullptr, 10));
        field.is_signed = (is_signed == 1);
      }
    }
  }
  return field;
}

std::vector<TracingFormat> TracingFile::LoadTracingFormatsFromEventFiles()
    const {
  std::vector<TracingFormat> formats;
  for (const auto& pair : event_format_files) {
    TracingFormat format;
    format.system_name = pair.first;
    std::vector<std::string> strs = android::base::Split(pair.second, "\n");
    FormatParsingState state = FormatParsingState::READ_NAME;
    for (const auto& s : strs) {
      if (state == FormatParsingState::READ_NAME) {
        size_t pos = s.find("name:");
        if (pos != std::string::npos) {
          format.name = android::base::Trim(s.substr(pos + strlen("name:")));
          state = FormatParsingState::READ_ID;
        }
      } else if (state == FormatParsingState::READ_ID) {
        size_t pos = s.find("ID:");
        if (pos != std::string::npos) {
          format.id =
              strtoull(s.substr(pos + strlen("ID:")).c_str(), nullptr, 10);
          state = FormatParsingState::READ_FIELDS;
        }
      } else if (state == FormatParsingState::READ_FIELDS) {
        size_t pos = s.find("field:");
        if (pos != std::string::npos) {
          TracingField field = ParseTracingField(s);
          format.fields.push_back(field);
        }
      }
    }
    formats.push_back(format);
  }
  return formats;
}

Tracing::Tracing(const std::vector<char>& data) {
  tracing_file_ = new TracingFile;
  tracing_file_->LoadFromBinary(data);
}

Tracing::~Tracing() { delete tracing_file_; }

void Tracing::Dump(size_t indent) { tracing_file_->Dump(indent); }

TracingFormat Tracing::GetTracingFormatHavingId(uint64_t trace_event_id) {
  if (tracing_formats_.empty()) {
    tracing_formats_ = tracing_file_->LoadTracingFormatsFromEventFiles();
  }
  for (const auto& format : tracing_formats_) {
    if (format.id == trace_event_id) {
      return format;
    }
  }
  LOG(FATAL) << "no tracing format for id " << trace_event_id;
  return TracingFormat();
}

std::string Tracing::GetTracingEventNameHavingId(uint64_t trace_event_id) {
  if (tracing_formats_.empty()) {
    tracing_formats_ = tracing_file_->LoadTracingFormatsFromEventFiles();
  }
  for (const auto& format : tracing_formats_) {
    if (format.id == trace_event_id) {
      return android::base::StringPrintf("%s:%s", format.system_name.c_str(),
                                         format.name.c_str());
    }
  }
  return "";
}

const std::string& Tracing::GetKallsyms() const {
  return tracing_file_->GetKallsymsFile();
}

uint32_t Tracing::GetPageSize() const { return tracing_file_->GetPageSize(); }

bool GetTracingData(const std::vector<const EventType*>& event_types,
                    std::vector<char>* data) {
  data->clear();
  std::vector<TraceType> trace_types;
  for (const auto& type : event_types) {
    CHECK_EQ(static_cast<uint32_t>(PERF_TYPE_TRACEPOINT), type->type);
    size_t pos = type->name.find(':');
    TraceType trace_type;
    trace_type.system = type->name.substr(0, pos);
    trace_type.name = type->name.substr(pos + 1);
    trace_types.push_back(trace_type);
  }
  TracingFile tracing_file;
  if (!tracing_file.RecordHeaderFiles()) {
    return false;
  }
  tracing_file.RecordFtraceFiles(trace_types);
  if (!tracing_file.RecordEventFiles(trace_types)) {
    return false;
  }
  // Don't record /proc/kallsyms here, as it will be contained in
  // KernelSymbolRecord.
  if (!tracing_file.RecordPrintkFormatsFile()) {
    return false;
  }
  *data = tracing_file.BinaryFormat();
  return true;
}
