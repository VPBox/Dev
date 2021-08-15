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

#include <inttypes.h>

#include <map>
#include <string>
#include <vector>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "command.h"
#include "event_attr.h"
#include "event_type.h"
#include "perf_regs.h"
#include "record.h"
#include "record_file.h"
#include "utils.h"

using namespace PerfFileFormat;

class DumpRecordCommand : public Command {
 public:
  DumpRecordCommand()
      : Command("dump", "dump perf record file",
                "Usage: simpleperf dumprecord [options] [perf_record_file]\n"
                "    Dump different parts of a perf record file. Default file is perf.data.\n"),
        record_filename_("perf.data"), record_file_arch_(GetBuildArch()) {
  }

  bool Run(const std::vector<std::string>& args);

 private:
  bool ParseOptions(const std::vector<std::string>& args);
  void DumpFileHeader();
  void DumpAttrSection();
  bool DumpDataSection();
  bool DumpFeatureSection();

  std::string record_filename_;
  std::unique_ptr<RecordFileReader> record_file_reader_;
  ArchType record_file_arch_;
};

bool DumpRecordCommand::Run(const std::vector<std::string>& args) {
  if (!ParseOptions(args)) {
    return false;
  }
  record_file_reader_ = RecordFileReader::CreateInstance(record_filename_);
  if (record_file_reader_ == nullptr) {
    return false;
  }
  std::string arch = record_file_reader_->ReadFeatureString(FEAT_ARCH);
  if (!arch.empty()) {
    record_file_arch_ = GetArchType(arch);
    if (record_file_arch_ == ARCH_UNSUPPORTED) {
      return false;
    }
  }
  ScopedCurrentArch scoped_arch(record_file_arch_);
  std::unique_ptr<ScopedEventTypes> scoped_event_types;
  if (record_file_reader_->HasFeature(PerfFileFormat::FEAT_META_INFO)) {
    std::unordered_map<std::string, std::string> meta_info;
    if (!record_file_reader_->ReadMetaInfoFeature(&meta_info)) {
      return false;
    }
    auto it = meta_info.find("event_type_info");
    if (it != meta_info.end()) {
      scoped_event_types.reset(new ScopedEventTypes(it->second));
    }
  }
  DumpFileHeader();
  DumpAttrSection();
  if (!DumpDataSection()) {
    return false;
  }
  return DumpFeatureSection();
}

bool DumpRecordCommand::ParseOptions(const std::vector<std::string>& args) {
  if (args.size() == 1) {
    record_filename_ = args[0];
  } else if (args.size() > 1) {
    ReportUnknownOption(args, 1);
    return false;
  }
  return true;
}

static const std::string GetFeatureNameOrUnknown(int feature) {
  std::string name = GetFeatureName(feature);
  return name.empty() ? android::base::StringPrintf("unknown_feature(%d)", feature) : name;
}

void DumpRecordCommand::DumpFileHeader() {
  const FileHeader& header = record_file_reader_->FileHeader();
  printf("magic: ");
  for (size_t i = 0; i < 8; ++i) {
    printf("%c", header.magic[i]);
  }
  printf("\n");
  printf("header_size: %" PRId64 "\n", header.header_size);
  if (header.header_size != sizeof(header)) {
    PLOG(WARNING) << "record file header size " << header.header_size
                  << "doesn't match expected header size " << sizeof(header);
  }
  printf("attr_size: %" PRId64 "\n", header.attr_size);
  if (header.attr_size != sizeof(FileAttr)) {
    PLOG(WARNING) << "record file attr size " << header.attr_size
                  << " doesn't match expected attr size " << sizeof(FileAttr);
  }
  printf("attrs[file section]: offset %" PRId64 ", size %" PRId64 "\n", header.attrs.offset,
         header.attrs.size);
  printf("data[file section]: offset %" PRId64 ", size %" PRId64 "\n", header.data.offset,
         header.data.size);
  printf("event_types[file section]: offset %" PRId64 ", size %" PRId64 "\n",
         header.event_types.offset, header.event_types.size);

  std::vector<int> features;
  for (size_t i = 0; i < FEAT_MAX_NUM; ++i) {
    size_t j = i / 8;
    size_t k = i % 8;
    if ((header.features[j] & (1 << k)) != 0) {
      features.push_back(i);
    }
  }
  for (auto& feature : features) {
    printf("feature: %s\n", GetFeatureNameOrUnknown(feature).c_str());
  }
}

void DumpRecordCommand::DumpAttrSection() {
  std::vector<EventAttrWithId> attrs = record_file_reader_->AttrSection();
  for (size_t i = 0; i < attrs.size(); ++i) {
    const auto& attr = attrs[i];
    printf("attr %zu:\n", i + 1);
    DumpPerfEventAttr(*attr.attr, 1);
    if (!attr.ids.empty()) {
      printf("  ids:");
      for (const auto& id : attr.ids) {
        printf(" %" PRId64, id);
      }
      printf("\n");
    }
  }
}

bool DumpRecordCommand::DumpDataSection() {
  ThreadTree thread_tree;
  thread_tree.ShowIpForUnknownSymbol();
  record_file_reader_->LoadBuildIdAndFileFeatures(thread_tree);

  auto get_symbol_function = [&](uint32_t pid, uint32_t tid, uint64_t ip, std::string& dso_name,
                                 std::string& symbol_name, uint64_t& vaddr_in_file,
                                 bool in_kernel) {
    ThreadEntry* thread = thread_tree.FindThreadOrNew(pid, tid);
    const MapEntry* map = thread_tree.FindMap(thread, ip, in_kernel);
    Dso* dso;
    const Symbol* symbol = thread_tree.FindSymbol(map, ip, &vaddr_in_file, &dso);
    dso_name = dso->Path();
    symbol_name = symbol->DemangledName();
  };

  auto record_callback = [&](std::unique_ptr<Record> r) {
    r->Dump();
    thread_tree.Update(*r);
    if (r->type() == PERF_RECORD_SAMPLE) {
      SampleRecord& sr = *static_cast<SampleRecord*>(r.get());
      bool in_kernel = sr.InKernel();
      if (sr.sample_type & PERF_SAMPLE_CALLCHAIN) {
        PrintIndented(1, "callchain:\n");
        for (size_t i = 0; i < sr.callchain_data.ip_nr; ++i) {
          if (sr.callchain_data.ips[i] >= PERF_CONTEXT_MAX) {
            if (sr.callchain_data.ips[i] == PERF_CONTEXT_USER) {
              in_kernel = false;
            }
            continue;
          }
          std::string dso_name;
          std::string symbol_name;
          uint64_t vaddr_in_file;
          get_symbol_function(sr.tid_data.pid, sr.tid_data.tid, sr.callchain_data.ips[i],
                              dso_name, symbol_name, vaddr_in_file, in_kernel);
          PrintIndented(2, "%s (%s[+%" PRIx64 "])\n", symbol_name.c_str(), dso_name.c_str(),
                        vaddr_in_file);
        }
      }
    } else if (r->type() == SIMPLE_PERF_RECORD_CALLCHAIN) {
      CallChainRecord& cr = *static_cast<CallChainRecord*>(r.get());
      PrintIndented(1, "callchain:\n");
      for (size_t i = 0; i < cr.ip_nr; ++i) {
        std::string dso_name;
        std::string symbol_name;
        uint64_t vaddr_in_file;
        get_symbol_function(cr.pid, cr.tid, cr.ips[i], dso_name, symbol_name, vaddr_in_file,
                            false);
        PrintIndented(2, "%s (%s[+%" PRIx64 "])\n", symbol_name.c_str(), dso_name.c_str(),
                      vaddr_in_file);
      }
    }
    return true;
  };
  return record_file_reader_->ReadDataSection(record_callback);
}

bool DumpRecordCommand::DumpFeatureSection() {
  std::map<int, SectionDesc> section_map = record_file_reader_->FeatureSectionDescriptors();
  for (const auto& pair : section_map) {
    int feature = pair.first;
    const auto& section = pair.second;
    printf("feature section for %s: offset %" PRId64 ", size %" PRId64 "\n",
           GetFeatureNameOrUnknown(feature).c_str(), section.offset, section.size);
    if (feature == FEAT_BUILD_ID) {
      std::vector<BuildIdRecord> records = record_file_reader_->ReadBuildIdFeature();
      for (auto& r : records) {
        r.Dump(1);
      }
    } else if (feature == FEAT_OSRELEASE) {
      std::string s = record_file_reader_->ReadFeatureString(feature);
      PrintIndented(1, "osrelease: %s\n", s.c_str());
    } else if (feature == FEAT_ARCH) {
      std::string s = record_file_reader_->ReadFeatureString(feature);
      PrintIndented(1, "arch: %s\n", s.c_str());
    } else if (feature == FEAT_CMDLINE) {
      std::vector<std::string> cmdline = record_file_reader_->ReadCmdlineFeature();
      PrintIndented(1, "cmdline: %s\n", android::base::Join(cmdline, ' ').c_str());
    } else if (feature == FEAT_FILE) {
      std::string file_path;
      uint32_t file_type;
      uint64_t min_vaddr;
      uint64_t file_offset_of_min_vaddr;
      std::vector<Symbol> symbols;
      std::vector<uint64_t> dex_file_offsets;
      size_t read_pos = 0;
      PrintIndented(1, "file:\n");
      while (record_file_reader_->ReadFileFeature(read_pos, &file_path, &file_type,
                                                  &min_vaddr, &file_offset_of_min_vaddr,
                                                  &symbols, &dex_file_offsets)) {
        PrintIndented(2, "file_path %s\n", file_path.c_str());
        PrintIndented(2, "file_type %s\n", DsoTypeToString(static_cast<DsoType>(file_type)));
        PrintIndented(2, "min_vaddr 0x%" PRIx64 "\n", min_vaddr);
        PrintIndented(2, "file_offset_of_min_vaddr 0x%" PRIx64 "\n", file_offset_of_min_vaddr);
        PrintIndented(2, "symbols:\n");
        for (const auto& symbol : symbols) {
          PrintIndented(3, "%s [0x%" PRIx64 "-0x%" PRIx64 "]\n", symbol.DemangledName(),
                        symbol.addr, symbol.addr + symbol.len);
        }
        if (file_type == static_cast<uint32_t>(DSO_DEX_FILE)) {
          PrintIndented(2, "dex_file_offsets:\n");
          for (uint64_t offset : dex_file_offsets) {
            PrintIndented(3, "0x%" PRIx64 "\n", offset);
          }
        }
      }
    } else if (feature == FEAT_META_INFO) {
      std::unordered_map<std::string, std::string> info_map;
      if (!record_file_reader_->ReadMetaInfoFeature(&info_map)) {
        return false;
      }
      PrintIndented(1, "meta_info:\n");
      for (auto& pair : info_map) {
        PrintIndented(2, "%s = %s\n", pair.first.c_str(), pair.second.c_str());
      }
    }
  }
  return true;
}

void RegisterDumpRecordCommand() {
  RegisterCommand("dump", [] { return std::unique_ptr<Command>(new DumpRecordCommand); });
}
