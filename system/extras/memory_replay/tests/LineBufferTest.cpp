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

#include <string>

#include <android-base/file.h>

#include "LineBuffer.h"

class LineBufferTest : public ::testing::Test {
 protected:
  void SetUp() override {
    tmp_file_ = new TemporaryFile();
    ASSERT_TRUE(tmp_file_->fd != -1);
  }

  void TearDown() override {
    delete tmp_file_;
  }

 TemporaryFile* tmp_file_ = nullptr;
};

TEST_F(LineBufferTest, single_line) {
  std::string line_data;
  line_data += "Single line with newline.\n";
  ASSERT_TRUE(TEMP_FAILURE_RETRY(
      write(tmp_file_->fd, line_data.c_str(), line_data.size())) != -1);
  ASSERT_TRUE(lseek(tmp_file_->fd, 0, SEEK_SET) != off_t(-1));

  char buffer[100];
  LineBuffer line_buf(tmp_file_->fd, buffer, sizeof(buffer));

  char* line;
  size_t line_len;
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Single line with newline.", line);
  ASSERT_EQ(sizeof("Single line with newline.") - 1, line_len);

  ASSERT_FALSE(line_buf.GetLine(&line, &line_len));
}

TEST_F(LineBufferTest, single_line_no_newline) {
  std::string line_data;
  line_data += "Single line with no newline.";
  ASSERT_TRUE(TEMP_FAILURE_RETRY(
      write(tmp_file_->fd, line_data.c_str(), line_data.size())) != -1);
  ASSERT_TRUE(lseek(tmp_file_->fd, 0, SEEK_SET) != off_t(-1));

  char buffer[100];
  LineBuffer line_buf(tmp_file_->fd, buffer, sizeof(buffer));

  char* line;
  size_t line_len;
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Single line with no newline.", line);
  ASSERT_EQ(sizeof("Single line with no newline.") - 1, line_len);

  ASSERT_FALSE(line_buf.GetLine(&line, &line_len));
}

TEST_F(LineBufferTest, single_read) {
  std::string line_data;
  line_data += "The first line.\n";
  line_data += "Second line here.\n";
  line_data += "Third line is last.\n";
  ASSERT_TRUE(TEMP_FAILURE_RETRY(
      write(tmp_file_->fd, line_data.c_str(), line_data.size())) != -1);
  ASSERT_TRUE(lseek(tmp_file_->fd, 0, SEEK_SET) != off_t(-1));

  char buffer[100];
  LineBuffer line_buf(tmp_file_->fd, buffer, sizeof(buffer));

  char* line;
  size_t line_len;
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The first line.", line);
  ASSERT_EQ(sizeof("The first line.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Second line here.", line);
  ASSERT_EQ(sizeof("Second line here.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Third line is last.", line);
  ASSERT_EQ(sizeof("Third line is last.") - 1, line_len);

  ASSERT_FALSE(line_buf.GetLine(&line, &line_len));
}

TEST_F(LineBufferTest, single_read_no_end_newline) {
  std::string line_data;
  line_data += "The first line.\n";
  line_data += "Second line here.\n";
  line_data += "Third line is last no newline.";
  ASSERT_TRUE(TEMP_FAILURE_RETRY(
      write(tmp_file_->fd, line_data.c_str(), line_data.size())) != -1);
  ASSERT_TRUE(lseek(tmp_file_->fd, 0, SEEK_SET) != off_t(-1));

  char buffer[100];
  LineBuffer line_buf(tmp_file_->fd, buffer, sizeof(buffer));

  char* line;
  size_t line_len;
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The first line.", line);
  ASSERT_EQ(sizeof("The first line.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Second line here.", line);
  ASSERT_EQ(sizeof("Second line here.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Third line is last no newline.", line);
  ASSERT_EQ(sizeof("Third line is last no newline.") - 1, line_len);

  ASSERT_FALSE(line_buf.GetLine(&line, &line_len));
}

TEST_F(LineBufferTest, one_line_per_read) {
  std::string line_data;
  line_data += "The first line.\n";
  line_data += "Second line here.\n";
  line_data += "Third line is last.\n";
  line_data += "The fourth line.\n";
  ASSERT_TRUE(TEMP_FAILURE_RETRY(
      write(tmp_file_->fd, line_data.c_str(), line_data.size())) != -1);
  ASSERT_TRUE(lseek(tmp_file_->fd, 0, SEEK_SET) != off_t(-1));

  char buffer[24];
  LineBuffer line_buf(tmp_file_->fd, buffer, sizeof(buffer));

  char* line;
  size_t line_len;
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The first line.", line);
  ASSERT_EQ(sizeof("The first line.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Second line here.", line);
  ASSERT_EQ(sizeof("Second line here.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Third line is last.", line);
  ASSERT_EQ(sizeof("Third line is last.") - 1, line_len);

  line_data += "The fourth line.\n";
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The fourth line.", line);
  ASSERT_EQ(sizeof("The fourth line.") - 1, line_len);

  ASSERT_FALSE(line_buf.GetLine(&line, &line_len));
}

TEST_F(LineBufferTest, multiple_line_per_read_multiple_reads) {
  std::string line_data;
  line_data += "The first line.\n";
  line_data += "Second line here.\n";
  line_data += "Third line is last.\n";
  line_data += "The fourth line.\n";
  ASSERT_TRUE(TEMP_FAILURE_RETRY(
      write(tmp_file_->fd, line_data.c_str(), line_data.size())) != -1);
  ASSERT_TRUE(lseek(tmp_file_->fd, 0, SEEK_SET) != off_t(-1));

  char buffer[60];
  LineBuffer line_buf(tmp_file_->fd, buffer, sizeof(buffer));

  char* line;
  size_t line_len;
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The first line.", line);
  ASSERT_EQ(sizeof("The first line.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Second line here.", line);
  ASSERT_EQ(sizeof("Second line here.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Third line is last.", line);
  ASSERT_EQ(sizeof("Third line is last.") - 1, line_len);

  line_data += "The fourth line.\n";
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The fourth line.", line);
  ASSERT_EQ(sizeof("The fourth line.") - 1, line_len);

  ASSERT_FALSE(line_buf.GetLine(&line, &line_len));
}

TEST_F(LineBufferTest, line_larger_than_buffer) {
  std::string line_data;
  line_data += "The first line.\n";
  line_data += "Second line here.\n";
  line_data += "This is a really, really, really, kind of long.\n";
  line_data += "The fourth line.\n";
  ASSERT_TRUE(TEMP_FAILURE_RETRY(
      write(tmp_file_->fd, line_data.c_str(), line_data.size())) != -1);
  ASSERT_TRUE(lseek(tmp_file_->fd, 0, SEEK_SET) != off_t(-1));

  char buffer[25];
  LineBuffer line_buf(tmp_file_->fd, buffer, sizeof(buffer));

  char* line;
  size_t line_len;
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The first line.", line);
  ASSERT_EQ(sizeof("The first line.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("Second line here.", line);
  ASSERT_EQ(sizeof("Second line here.") - 1, line_len);

  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("This is a really, really", line);
  ASSERT_EQ(sizeof(buffer) - 1, line_len);
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ(", really, kind of long.", line);
  ASSERT_EQ(sizeof(", really, kind of long.") - 1, line_len);

  line_data += "The fourth line.\n";
  ASSERT_TRUE(line_buf.GetLine(&line, &line_len));
  ASSERT_STREQ("The fourth line.", line);
  ASSERT_EQ(sizeof("The fourth line.") - 1, line_len);

  ASSERT_FALSE(line_buf.GetLine(&line, &line_len));
}
