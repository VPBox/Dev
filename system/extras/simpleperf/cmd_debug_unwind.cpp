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
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "CallChainJoiner.h"
#include "command.h"
#include "environment.h"
#include "OfflineUnwinder.h"
#include "perf_regs.h"
#include "record_file.h"
#include "thread_tree.h"
#include "utils.h"
#include "workload.h"

using namespace simpleperf;

// Cache size used by CallChainJoiner to cache call chains in memory.
constexpr size_t DEFAULT_CALL_CHAIN_JOINER_CACHE_SIZE = 8 * 1024 * 1024;

struct MemStat {
  std::string vm_peak;
  std::string vm_size;
  std::string vm_hwm;
  std::string vm_rss;

  std::string ToString() const {
    return android::base::StringPrintf("VmPeak:%s;VmSize:%s;VmHWM:%s;VmRSS:%s", vm_peak.c_str(),
                                       vm_size.c_str(), vm_hwm.c_str(), vm_rss.c_str());
  }
};

static bool GetMemStat(MemStat* stat) {
  std::string s;
  if (!android::base::ReadFileToString(android::base::StringPrintf("/proc/%d/status", getpid()),
                                                                   &s)) {
    PLOG(ERROR) << "Failed to read process status";
    return false;
  }
  std::vector<std::string> lines = android::base::Split(s, "\n");
  for (auto& line : lines) {
    if (android::base::StartsWith(line, "VmPeak:")) {
      stat->vm_peak = android::base::Trim(line.substr(strlen("VmPeak:")));
    } else if (android::base::StartsWith(line, "VmSize:")) {
      stat->vm_size = android::base::Trim(line.substr(strlen("VmSize:")));
    } else if (android::base::StartsWith(line, "VmHWM:")) {
      stat->vm_hwm = android::base::Trim(line.substr(strlen("VmHWM:")));
    } else if (android::base::StartsWith(line, "VmRSS:")) {
      stat->vm_rss = android::base::Trim(line.substr(strlen("VmRSS:")));
    }
  }
  return true;
}

class DebugUnwindCommand : public Command {
 public:
  DebugUnwindCommand()
      : Command("debug-unwind", "Debug/test offline unwinding.",
                // clang-format off
"Usage: simpleperf debug-unwind [options]\n"
"       Given a perf.data generated with \"-g --no-unwind\", it converts\n"
"       regs/stack data of samples into callchains, and write result into\n"
"       a new perf.data. The new perf.data can be passed to\n"
"       unwind_result_reporter.py to generate a text report.\n"
"-i <file>  The path of record file generated with \"-g --no-unwind\".\n"
"           Default is perf.data.\n"
"-o <file>  The path ot write new perf.data. Default is perf.data.debug.\n"
"--symfs <dir>  Look for files with symbols relative to this directory.\n"
"--time time    Only unwind samples recorded at selected time.\n"
                // clang-format on
               ),
          input_filename_("perf.data"),
          output_filename_("perf.data.debug"),
          offline_unwinder_(true),
          callchain_joiner_(DEFAULT_CALL_CHAIN_JOINER_CACHE_SIZE, 1, true),
          selected_time_(0) {
  }

  bool Run(const std::vector<std::string>& args);

 private:
  bool ParseOptions(const std::vector<std::string>& args);
  bool UnwindRecordFile();
  bool ProcessRecord(Record* record);
  void CollectHitFileInfo(const SampleRecord& r, const std::vector<uint64_t>& ips);
  bool JoinCallChains();
  bool WriteFeatureSections();
  void PrintStat();

  struct Stat {
    // For testing unwinding performance.
    uint64_t unwinding_sample_count = 0u;
    uint64_t total_unwinding_time_in_ns = 0u;
    uint64_t max_unwinding_time_in_ns = 0u;

    // For memory consumption.
    MemStat mem_before_unwinding;
    MemStat mem_after_unwinding;
  };

  std::string input_filename_;
  std::string output_filename_;
  std::unique_ptr<RecordFileReader> reader_;
  std::unique_ptr<RecordFileWriter> writer_;
  ThreadTree thread_tree_;
  OfflineUnwinder offline_unwinder_;
  CallChainJoiner callchain_joiner_;
  Stat stat_;
  uint64_t selected_time_;
};

bool DebugUnwindCommand::Run(const std::vector<std::string>& args) {
  // 1. Parse options.
  if (!ParseOptions(args)) {
    return false;
  }
  ScopedTempFiles scoped_temp_files(android::base::Dirname(output_filename_));

  // 2. Read input perf.data, and generate new perf.data.
  if (!UnwindRecordFile()) {
    return false;
  }

  // 3. Show stat of unwinding.
  PrintStat();
  return true;
}

bool DebugUnwindCommand::ParseOptions(const std::vector<std::string>& args) {
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i] == "-i") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      input_filename_ = args[i];
    } else if (args[i] == "-o") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      output_filename_ = args[i];
    } else if (args[i] == "--symfs") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      if (!Dso::SetSymFsDir(args[i])) {
        return false;
      }
    } else if (args[i] == "--time") {
      if (!GetUintOption(args, &i, &selected_time_)) {
        return false;
      }
    } else {
      ReportUnknownOption(args, i);
      return false;
    }
  }
  return true;
}

bool DebugUnwindCommand::UnwindRecordFile() {
  // 1. Check input file.
  reader_ = RecordFileReader::CreateInstance(input_filename_);
  if (!reader_) {
    return false;
  }
  reader_->LoadBuildIdAndFileFeatures(thread_tree_);
  std::string record_cmd = android::base::Join(reader_->ReadCmdlineFeature(), " ");
  if (record_cmd.find("--no-unwind") == std::string::npos ||
      (record_cmd.find("-g") == std::string::npos &&
          record_cmd.find("--call-graph dwarf") == std::string::npos)) {
    LOG(ERROR) << input_filename_ << " isn't recorded with \"-g --no-unwind\"";
    return false;
  }
  ScopedCurrentArch scoped_arch(GetArchType(reader_->ReadFeatureString(PerfFileFormat::FEAT_ARCH)));

  // 2. Copy attr section.
  writer_ = RecordFileWriter::CreateInstance(output_filename_);
  if (!writer_ || !writer_->WriteAttrSection(reader_->AttrSection())) {
    return false;
  }

  // 3. Process records in data section.
  if (!GetMemStat(&stat_.mem_before_unwinding)) {
    return false;
  }
  auto callback = [this](std::unique_ptr<Record> record) {
    return ProcessRecord(record.get());
  };
  if (!reader_->ReadDataSection(callback)) {
    return false;
  }
  if (!JoinCallChains()) {
    return false;
  }
  if (!GetMemStat(&stat_.mem_after_unwinding)) {
    return false;
  }

  // 4. Write feature sections.
  return WriteFeatureSections();
}

bool DebugUnwindCommand::ProcessRecord(Record* record) {
  if (record->type() == PERF_RECORD_SAMPLE) {
    auto& r = *static_cast<SampleRecord*>(record);
    if (selected_time_ != 0u && r.Timestamp() != selected_time_) {
      return true;
    }
    uint64_t need_type = PERF_SAMPLE_CALLCHAIN | PERF_SAMPLE_REGS_USER | PERF_SAMPLE_STACK_USER;
    if ((r.sample_type & need_type) == need_type && r.regs_user_data.reg_mask != 0 &&
        r.GetValidStackSize() > 0) {
      ThreadEntry* thread = thread_tree_.FindThreadOrNew(r.tid_data.pid, r.tid_data.tid);
      RegSet regs(r.regs_user_data.abi, r.regs_user_data.reg_mask, r.regs_user_data.regs);
      std::vector<uint64_t> ips;
      std::vector<uint64_t> sps;
      if (!offline_unwinder_.UnwindCallChain(*thread, regs, r.stack_user_data.data,
                                             r.GetValidStackSize(), &ips, &sps)) {
        return false;
      }

      const UnwindingResult& unwinding_result = offline_unwinder_.GetUnwindingResult();
      stat_.unwinding_sample_count++;
      stat_.total_unwinding_time_in_ns += unwinding_result.used_time;
      stat_.max_unwinding_time_in_ns = std::max(stat_.max_unwinding_time_in_ns,
                                                unwinding_result.used_time);
      if (!writer_->WriteRecord(UnwindingResultRecord(r.time_data.time, unwinding_result))) {
        return false;
      }
      // We want to keep both reg/stack data and callchain of a sample. However, storing both
      // can exceed the size limit of a SampleRecord. So instead we store one sample with reg/stack
      // data and one sample with callchain.
      if (!writer_->WriteRecord(r)) {
        return false;
      }
      r.ReplaceRegAndStackWithCallChain(ips);
      if (!callchain_joiner_.AddCallChain(r.tid_data.pid, r.tid_data.tid,
                                          CallChainJoiner::ORIGINAL_OFFLINE, ips, sps)) {
        return false;
      }
      CollectHitFileInfo(r, ips);
    }
  } else {
    thread_tree_.Update(*record);
  }
  return writer_->WriteRecord(*record);
}

void DebugUnwindCommand::CollectHitFileInfo(const SampleRecord& r,
                                            const std::vector<uint64_t>& ips) {
  const ThreadEntry* thread = thread_tree_.FindThreadOrNew(r.tid_data.pid, r.tid_data.tid);
  for (auto ip : ips) {
    const MapEntry* map = thread_tree_.FindMap(thread, ip, false);
    Dso* dso = map->dso;
    if (!dso->HasDumpId() && dso->type() != DSO_UNKNOWN_FILE) {
      dso->CreateDumpId();
    }
    const Symbol* symbol = thread_tree_.FindSymbol(map, ip, nullptr, &dso);
    if (!symbol->HasDumpId()) {
      dso->CreateSymbolDumpId(symbol);
    }
  }
}

bool DebugUnwindCommand::JoinCallChains() {
  // 1. Prepare joined callchains.
  if (!callchain_joiner_.JoinCallChains()) {
    return false;
  }
  // 2. Move records from record_filename_ to a temporary file.
  if (!writer_->Close()) {
    return false;
  }
  writer_.reset();
  std::unique_ptr<TemporaryFile> tmp_file = ScopedTempFiles::CreateTempFile();
  if (!Workload::RunCmd({"mv", output_filename_, tmp_file->path})) {
    return false;
  }

  // 3. Read records from the temporary file, and write records with joined call chains back
  // to record_filename_.
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmp_file->path);
  if (!reader) {
    return false;
  }
  writer_ = RecordFileWriter::CreateInstance(output_filename_);
  if (!writer_ || !writer_->WriteAttrSection(reader->AttrSection())) {
    return false;
  }

  auto record_callback = [&](std::unique_ptr<Record> r) {
    if (r->type() != PERF_RECORD_SAMPLE) {
      return writer_->WriteRecord(*r);
    }
    SampleRecord& sr = *static_cast<SampleRecord*>(r.get());
    if (!sr.HasUserCallChain()) {
      return writer_->WriteRecord(sr);
    }
    pid_t pid;
    pid_t tid;
    CallChainJoiner::ChainType type;
    std::vector<uint64_t> ips;
    std::vector<uint64_t> sps;
    do {
      if (!callchain_joiner_.GetNextCallChain(pid, tid, type, ips, sps)) {
        return false;
      }
      if (!writer_->WriteRecord(CallChainRecord(pid, tid, type, sr.Timestamp(), ips, sps))) {
        return false;
      }
    } while (type != CallChainJoiner::JOINED_OFFLINE);
    CHECK_EQ(pid, static_cast<pid_t>(sr.tid_data.pid));
    CHECK_EQ(tid, static_cast<pid_t>(sr.tid_data.tid));
    sr.UpdateUserCallChain(ips);
    return writer_->WriteRecord(sr);
  };
  return reader->ReadDataSection(record_callback);
}

bool DebugUnwindCommand::WriteFeatureSections() {
  // Add debug_unwind info in META_INFO section, and add symbol info in FILE section.
  const std::map<int, PerfFileFormat::SectionDesc>& features = reader_->FeatureSectionDescriptors();
  size_t new_feature_count = features.size();
  for (int feature : {PerfFileFormat::FEAT_FILE, PerfFileFormat::FEAT_META_INFO}) {
    if (features.find(feature) == features.end()) {
      new_feature_count++;
    }
  }
  if (!writer_->BeginWriteFeatures(new_feature_count)) {
    return false;
  }

  auto it = features.begin();
  // Copy all feature sections except FEAT_FILE and FEAT_META_INFO, which require special handling.
  while (it != features.end() && it->first < PerfFileFormat::FEAT_FILE) {
    std::vector<char> data;
    if (!reader_->ReadFeatureSection(it->first, &data) || !writer_->WriteFeature(it->first, data)) {
      return false;
    }
    ++it;
  }
  // Write a new file section.
  if (it != features.end() && it->first == PerfFileFormat::FEAT_FILE) {
    ++it;
  }
  if (!writer_->WriteFileFeatures(thread_tree_.GetAllDsos())) {
    return false;
  }
  // Write meta_info section.
  std::unordered_map<std::string, std::string> info_map;
  if (it != features.end() && it->first == PerfFileFormat::FEAT_META_INFO) {
    if (!reader_->ReadMetaInfoFeature(&info_map)) {
      return false;
    }
    ++it;
  }
  info_map["debug_unwind"] = "true";
  info_map["debug_unwind_mem_before"] = stat_.mem_before_unwinding.ToString();
  info_map["debug_unwind_mem_after"] = stat_.mem_after_unwinding.ToString();
  if (!writer_->WriteMetaInfoFeature(info_map)) {
    return false;
  }
  CHECK(it == features.end());
  return writer_->EndWriteFeatures() && writer_->Close();
}

void DebugUnwindCommand::PrintStat() {
  printf("Unwinding sample count: %" PRIu64 "\n", stat_.unwinding_sample_count);
  if (stat_.unwinding_sample_count > 0u) {
    printf("Average unwinding time: %f us\n", static_cast<double>(stat_.total_unwinding_time_in_ns)
           / 1000 / stat_.unwinding_sample_count);
    printf("Max unwinding time: %f us\n", static_cast<double>(stat_.max_unwinding_time_in_ns)
           / 1000);
  }
  printf("Memory change:\n");
  PrintIndented(1, "VmPeak: %s -> %s\n", stat_.mem_before_unwinding.vm_peak.c_str(),
                stat_.mem_after_unwinding.vm_peak.c_str());
  PrintIndented(1, "VmSize: %s -> %s\n", stat_.mem_before_unwinding.vm_size.c_str(),
                stat_.mem_after_unwinding.vm_size.c_str());
  PrintIndented(1, "VmHWM: %s -> %s\n", stat_.mem_before_unwinding.vm_hwm.c_str(),
                stat_.mem_after_unwinding.vm_hwm.c_str());
  PrintIndented(1, "VmRSS: %s -> %s\n", stat_.mem_before_unwinding.vm_rss.c_str(),
                stat_.mem_after_unwinding.vm_rss.c_str());
  callchain_joiner_.DumpStat();
  printf("Please use debug_unwind_reporter.py to get a report in details.\n");
}

void RegisterDebugUnwindCommand() {
  RegisterCommand("debug-unwind",
                  []{ return std::unique_ptr<Command>(new DebugUnwindCommand()); });
}
