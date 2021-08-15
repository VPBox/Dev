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

#ifndef SIMPLE_PERF_RECORD_FILE_H_
#define SIMPLE_PERF_RECORD_FILE_H_

#include <stdio.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <android-base/macros.h>

#include "dso.h"
#include "event_attr.h"
#include "perf_event.h"
#include "record.h"
#include "record_file_format.h"
#include "thread_tree.h"

// RecordFileWriter writes to a perf record file, like perf.data.
// User should call RecordFileWriter::Close() to finish writing the file, otherwise the file will
// be removed in RecordFileWriter::~RecordFileWriter().
class RecordFileWriter {
 public:
  static std::unique_ptr<RecordFileWriter> CreateInstance(const std::string& filename);

  ~RecordFileWriter();

  bool WriteAttrSection(const std::vector<EventAttrWithId>& attr_ids);
  bool WriteRecord(const Record& record);

  uint64_t GetDataSectionSize() const { return data_section_size_; }
  bool ReadDataSection(const std::function<void(const Record*)>& callback);

  bool BeginWriteFeatures(size_t feature_count);
  bool WriteBuildIdFeature(const std::vector<BuildIdRecord>& build_id_records);
  bool WriteFeatureString(int feature, const std::string& s);
  bool WriteCmdlineFeature(const std::vector<std::string>& cmdline);
  bool WriteBranchStackFeature();
  bool WriteFileFeatures(const std::vector<Dso*>& files);
  bool WriteMetaInfoFeature(const std::unordered_map<std::string, std::string>& info_map);
  bool WriteFeature(int feature, const std::vector<char>& data);
  bool EndWriteFeatures();

  bool Close();

 private:
  RecordFileWriter(const std::string& filename, FILE* fp);
  void GetHitModulesInBuffer(const char* p, const char* end,
                             std::vector<std::string>* hit_kernel_modules,
                             std::vector<std::string>* hit_user_files);
  bool WriteFileHeader();
  bool WriteData(const void* buf, size_t len);
  bool Write(const void* buf, size_t len);
  bool Read(void* buf, size_t len);
  bool GetFilePos(uint64_t* file_pos);
  bool WriteStringWithLength(const std::string& s);
  bool WriteFileFeature(const std::string& file_path,
                        uint32_t file_type,
                        uint64_t min_vaddr,
                        uint64_t file_offset_of_min_vaddr,
                        const std::vector<const Symbol*>& symbols,
                        const std::vector<uint64_t>* dex_file_offsets);
  bool WriteFeatureBegin(int feature);
  bool WriteFeatureEnd(int feature);

  const std::string filename_;
  FILE* record_fp_;

  perf_event_attr event_attr_;
  uint64_t attr_section_offset_;
  uint64_t attr_section_size_;
  uint64_t data_section_offset_;
  uint64_t data_section_size_;
  uint64_t feature_section_offset_;

  std::map<int, PerfFileFormat::SectionDesc> features_;
  size_t feature_count_;

  DISALLOW_COPY_AND_ASSIGN(RecordFileWriter);
};

// RecordFileReader read contents from a perf record file, like perf.data.
class RecordFileReader {
 public:
  static std::unique_ptr<RecordFileReader> CreateInstance(const std::string& filename);

  ~RecordFileReader();

  const PerfFileFormat::FileHeader& FileHeader() const {
    return header_;
  }

  std::vector<EventAttrWithId> AttrSection() const {
    std::vector<EventAttrWithId> result(file_attrs_.size());
    for (size_t i = 0; i < file_attrs_.size(); ++i) {
      result[i].attr = &file_attrs_[i].attr;
      result[i].ids = event_ids_for_file_attrs_[i];
    }
    return result;
  }

  const std::map<int, PerfFileFormat::SectionDesc>& FeatureSectionDescriptors() const {
    return feature_section_descriptors_;
  }
  bool HasFeature(int feature) const {
    return feature_section_descriptors_.find(feature) != feature_section_descriptors_.end();
  }
  bool ReadFeatureSection(int feature, std::vector<char>* data);

  // There are two ways to read records in data section: one is by calling
  // ReadDataSection(), and [callback] is called for each Record. the other
  // is by calling ReadRecord() in a loop.

  // If sorted is true, sort records before passing them to callback function.
  bool ReadDataSection(const std::function<bool(std::unique_ptr<Record>)>& callback);

  // Read next record. If read successfully, set [record] and return true.
  // If there is no more records, set [record] to nullptr and return true.
  // Otherwise return false.
  bool ReadRecord(std::unique_ptr<Record>& record);

  size_t GetAttrIndexOfRecord(const Record* record);

  std::vector<std::string> ReadCmdlineFeature();
  std::vector<BuildIdRecord> ReadBuildIdFeature();
  std::string ReadFeatureString(int feature);

  // File feature section contains many file information. This function reads
  // one file information located at [read_pos]. [read_pos] is 0 at the first
  // call, and is updated to point to the next file information. Return true
  // if read successfully, and return false if there is no more file
  // information.
  bool ReadFileFeature(size_t& read_pos, std::string* file_path, uint32_t* file_type,
                       uint64_t* min_vaddr, uint64_t* file_offset_of_min_vaddr,
                       std::vector<Symbol>* symbols, std::vector<uint64_t>* dex_file_offsets);
  bool ReadMetaInfoFeature(std::unordered_map<std::string, std::string>* info_map);

  void LoadBuildIdAndFileFeatures(ThreadTree& thread_tree);

  bool Close();

  // For testing only.
  std::vector<std::unique_ptr<Record>> DataSection();

 private:
  RecordFileReader(const std::string& filename, FILE* fp);
  bool ReadHeader();
  bool ReadAttrSection();
  bool ReadIdsForAttr(const PerfFileFormat::FileAttr& attr, std::vector<uint64_t>* ids);
  bool ReadFeatureSectionDescriptors();
  std::unique_ptr<Record> ReadRecord(uint64_t* nbytes_read);
  bool Read(void* buf, size_t len);
  void ProcessEventIdRecord(const EventIdRecord& r);

  const std::string filename_;
  FILE* record_fp_;

  PerfFileFormat::FileHeader header_;
  std::vector<PerfFileFormat::FileAttr> file_attrs_;
  std::vector<std::vector<uint64_t>> event_ids_for_file_attrs_;
  std::unordered_map<uint64_t, size_t> event_id_to_attr_map_;
  std::map<int, PerfFileFormat::SectionDesc> feature_section_descriptors_;

  size_t event_id_pos_in_sample_records_;
  size_t event_id_reverse_pos_in_non_sample_records_;

  uint64_t read_record_size_;

  DISALLOW_COPY_AND_ASSIGN(RecordFileReader);
};

#endif  // SIMPLE_PERF_RECORD_FILE_H_
