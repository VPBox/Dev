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

#include <gtest/gtest.h>

#include <string.h>

#include <memory>

#include <android-base/file.h>

#include "environment.h"
#include "event_attr.h"
#include "event_type.h"
#include "record.h"
#include "record_file.h"

#include "record_equal_test.h"

using namespace PerfFileFormat;

class RecordFileTest : public ::testing::Test {
 protected:
  void AddEventType(const std::string& event_type_str) {
    std::unique_ptr<EventTypeAndModifier> event_type_modifier = ParseEventType(event_type_str);
    ASSERT_TRUE(event_type_modifier != nullptr);
    perf_event_attr attr = CreateDefaultPerfEventAttr(event_type_modifier->event_type);
    attr.sample_id_all = 1;
    attrs_.push_back(std::unique_ptr<perf_event_attr>(new perf_event_attr(attr)));
    EventAttrWithId attr_id;
    attr_id.attr = attrs_.back().get();
    attr_id.ids.push_back(attrs_.size());  // Fake id.
    attr_ids_.push_back(attr_id);
  }

  TemporaryFile tmpfile_;
  std::vector<std::unique_ptr<perf_event_attr>> attrs_;
  std::vector<EventAttrWithId> attr_ids_;
};

TEST_F(RecordFileTest, smoke) {
  // Write to a record file.
  std::unique_ptr<RecordFileWriter> writer = RecordFileWriter::CreateInstance(tmpfile_.path);
  ASSERT_TRUE(writer != nullptr);

  // Write attr section.
  AddEventType("cpu-cycles");
  ASSERT_TRUE(writer->WriteAttrSection(attr_ids_));

  // Write data section.
  MmapRecord mmap_record(*(attr_ids_[0].attr), true, 1, 1, 0x1000, 0x2000,
                         0x3000, "mmap_record_example", attr_ids_[0].ids[0]);
  ASSERT_TRUE(writer->WriteRecord(mmap_record));

  // Write feature section.
  ASSERT_TRUE(writer->BeginWriteFeatures(1));
  char p[BuildId::Size()];
  for (size_t i = 0; i < BuildId::Size(); ++i) {
    p[i] = i;
  }
  BuildId build_id(p);
  std::vector<BuildIdRecord> build_id_records;
  build_id_records.push_back(BuildIdRecord(false, getpid(), build_id, "init"));
  ASSERT_TRUE(writer->WriteBuildIdFeature(build_id_records));
  ASSERT_TRUE(writer->EndWriteFeatures());
  ASSERT_TRUE(writer->Close());

  // Read from a record file.
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile_.path);
  ASSERT_TRUE(reader != nullptr);
  std::vector<EventAttrWithId> attrs = reader->AttrSection();
  ASSERT_EQ(1u, attrs.size());
  ASSERT_EQ(0, memcmp(attrs[0].attr, attr_ids_[0].attr, sizeof(perf_event_attr)));
  ASSERT_EQ(attrs[0].ids, attr_ids_[0].ids);

  // Read and check data section.
  std::vector<std::unique_ptr<Record>> records = reader->DataSection();
  ASSERT_EQ(1u, records.size());
  CheckRecordEqual(mmap_record, *records[0]);

  // Read and check feature section.
  std::vector<BuildIdRecord> read_build_id_records = reader->ReadBuildIdFeature();
  ASSERT_EQ(1u, read_build_id_records.size());
  CheckRecordEqual(read_build_id_records[0], build_id_records[0]);

  ASSERT_TRUE(reader->Close());
}

TEST_F(RecordFileTest, record_more_than_one_attr) {
  // Write to a record file.
  std::unique_ptr<RecordFileWriter> writer = RecordFileWriter::CreateInstance(tmpfile_.path);
  ASSERT_TRUE(writer != nullptr);

  // Write attr section.
  AddEventType("cpu-cycles");
  AddEventType("cpu-clock");
  AddEventType("task-clock");
  ASSERT_TRUE(writer->WriteAttrSection(attr_ids_));

  ASSERT_TRUE(writer->Close());

  // Read from a record file.
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile_.path);
  ASSERT_TRUE(reader != nullptr);
  std::vector<EventAttrWithId> attrs = reader->AttrSection();
  ASSERT_EQ(3u, attrs.size());
  for (size_t i = 0; i < attrs.size(); ++i) {
    ASSERT_EQ(0, memcmp(attrs[i].attr, attr_ids_[i].attr, sizeof(perf_event_attr)));
    ASSERT_EQ(attrs[i].ids, attr_ids_[i].ids);
  }
}

TEST_F(RecordFileTest, write_meta_info_feature_section) {
  // Write to a record file.
  std::unique_ptr<RecordFileWriter> writer = RecordFileWriter::CreateInstance(tmpfile_.path);
  ASSERT_TRUE(writer != nullptr);
  AddEventType("cpu-cycles");
  ASSERT_TRUE(writer->WriteAttrSection(attr_ids_));

  // Write meta_info feature section.
  ASSERT_TRUE(writer->BeginWriteFeatures(1));
  std::unordered_map<std::string, std::string> info_map;
  for (int i = 0; i < 100; ++i) {
    std::string s = std::to_string(i);
    info_map[s] = s + s;
  }
  ASSERT_TRUE(writer->WriteMetaInfoFeature(info_map));
  ASSERT_TRUE(writer->EndWriteFeatures());
  ASSERT_TRUE(writer->Close());

  // Read from a record file.
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmpfile_.path);
  ASSERT_TRUE(reader != nullptr);
  std::unordered_map<std::string, std::string> read_info_map;
  ASSERT_TRUE(reader->ReadMetaInfoFeature(&read_info_map));
  ASSERT_EQ(read_info_map, info_map);
}
