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

#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>
#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <unordered_set>

#include <fcntl.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <android-base/thread_annotations.h>
#include <gtest/gtest.h>
#include <zlib.h>

#include "config.h"
#include "configreader.h"
#include "map_utils.h"
#include "perfprofdcore.h"
#include "perfprofd_cmdline.h"
#include "perfprofd_perf.h"
#include "perfprofd_threaded_handler.h"
#include "quipper_helper.h"
#include "symbolizer.h"

#include "perfprofd_record.pb.h"

using namespace android::perfprofd::quipper;

static_assert(android::base::kEnableDChecks, "Expected DCHECKs to be enabled");

//
// Set to argv[0] on startup
//
static std::string gExecutableRealpath;

namespace {

using android::base::LogId;
using android::base::LogSeverity;

class TestLogHelper {
 public:
  void Install() {
    using namespace std::placeholders;
    android::base::SetLogger(
        std::bind(&TestLogHelper::TestLogFunction, this, _1, _2, _3, _4, _5, _6));
  }

  std::string JoinTestLog(const char* delimiter) {
    std::unique_lock<std::mutex> ul(lock_);
    return android::base::Join(test_log_messages_, delimiter);
  }
  template <typename Predicate>
  std::string JoinTestLog(const char* delimiter, Predicate pred) {
    std::unique_lock<std::mutex> ul(lock_);
    std::vector<std::string> tmp;
    std::copy_if(test_log_messages_.begin(),
                 test_log_messages_.end(),
                 std::back_inserter(tmp),
                 pred);
    return android::base::Join(tmp, delimiter);
  }

 private:
  void TestLogFunction(LogId log_id,
                       LogSeverity severity,
                       const char* tag,
                       const char* file,
                       unsigned int line,
                       const char* message) {
    std::unique_lock<std::mutex> ul(lock_);
    constexpr char log_characters[] = "VDIWEFF";
    char severity_char = log_characters[severity];
    test_log_messages_.push_back(android::base::StringPrintf("%c: %s", severity_char, message));

    if (severity >= LogSeverity::FATAL_WITHOUT_ABORT) {
      android::base::StderrLogger(log_id, severity, tag, file, line, message);
    }
  }

 private:
  std::mutex lock_;

  std::vector<std::string> test_log_messages_;
};

}  // namespace

// Path to perf executable on device
#define PERFPATH "/system/bin/perf"

// Temporary config file that we will emit for the daemon to read
#define CONFIGFILE "perfprofd.conf"

static bool bothWhiteSpace(char lhs, char rhs)
{
  return (std::isspace(lhs) && std::isspace(rhs));
}

#ifdef __ANDROID__

static bool IsPerfSupported() {
  auto check_perf_supported = []() {
#if defined(__i386__) || defined(__x86_64__)
    // Cloud devices may suppress perf. Check for arch_perfmon.
    std::string cpuinfo;
    if (!android::base::ReadFileToString("/proc/cpuinfo", &cpuinfo)) {
      // This is pretty unexpected. Return true to see if we can run tests anyways.
      return true;
    }
    return cpuinfo.find("arch_perfmon") != std::string::npos;
#else
    // Expect other architectures to have perf support.
    return true;
#endif
  };
  static bool perf_supported = check_perf_supported();
  return perf_supported;
}

#endif

//
// Squeeze out repeated whitespace from expected/actual logs.
//
static std::string squeezeWhite(const std::string &str,
                                const char *tag,
                                bool dump=false)
{
  if (dump) { fprintf(stderr, "raw %s is %s\n", tag, str.c_str()); }
  std::string result(str);
  std::replace(result.begin(), result.end(), '\n', ' ');
  auto new_end = std::unique(result.begin(), result.end(), bothWhiteSpace);
  result.erase(new_end, result.end());
  while (result.begin() != result.end() && std::isspace(*result.rbegin())) {
    result.pop_back();
  }
  if (dump) { fprintf(stderr, "squeezed %s is %s\n", tag, result.c_str()); }
  return result;
}

//
// Replace all occurrences of a string with another string.
//
static std::string replaceAll(const std::string &str,
                              const std::string &from,
                              const std::string &to)
{
  std::string ret = "";
  size_t pos = 0;
  while (pos < str.size()) {
    size_t found = str.find(from, pos);
    if (found == std::string::npos) {
      ret += str.substr(pos);
      break;
    }
    ret += str.substr(pos, found - pos) + to;
    pos = found + from.size();
  }
  return ret;
}

//
// Replace occurrences of special variables in the string.
//
#ifdef __ANDROID__
static std::string expandVars(const std::string &str) {
#ifdef __LP64__
  return replaceAll(str, "$NATIVE_TESTS", "/data/nativetest64");
#else
  return replaceAll(str, "$NATIVE_TESTS", "/data/nativetest");
#endif
}
#endif

class PerfProfdTest : public testing::Test {
 protected:
  virtual void SetUp() {
    test_logger.Install();
    create_dirs();
  }

  virtual void TearDown() {
    android::base::SetLogger(android::base::StderrLogger);

    // TODO: proper management of test files. For now, use old system() code.
    for (const auto dir : { &dest_dir, &conf_dir }) {
      std::string cmd("rm -rf ");
      cmd += *dir;
      int ret = system(cmd.c_str());
      CHECK_EQ(0, ret);
    }
  }

 protected:
  //
  // Check to see if the log messages emitted by the daemon
  // match the expected result. By default we use a partial
  // match, e.g. if we see the expected excerpt anywhere in the
  // result, it's a match (for exact match, set exact to true)
  //
  ::testing::AssertionResult CompareLogMessages(const std::string& expected,
                                                bool exactMatch = false) {
     std::string sqexp = squeezeWhite(expected, "expected");

     // Strip out JIT errors.
     std::regex jit_regex("E: Failed to open ELF file: [^ ]*dalvik-jit-code-cache.*");
     auto strip_jit = [&](const std::string& str) {
       std::smatch jit_match;
       return !std::regex_match(str, jit_match, jit_regex);
     };
     std::string sqact = squeezeWhite(test_logger.JoinTestLog(" ", strip_jit), "actual");

     if (exactMatch) {
       if (sqexp == sqact) {
         return ::testing::AssertionSuccess() << sqexp << " is equal to " << sqact;
       }
       return ::testing::AssertionFailure() << "Expected:" << std::endl << sqexp << std::endl
                                            << "Received:" << std::endl << sqact;
     } else {
       if (sqact.find(sqexp) == std::string::npos) {
         return ::testing::AssertionFailure()
             << "Expected to find:" << std::endl << sqexp << std::endl
             << "in:" << std::endl << sqact;
       }
       return ::testing::AssertionSuccess() << sqexp << " was found in " << sqact;
     }
  }

  // test_dir is the directory containing the test executable and
  // any files associated with the test (will be created by the harness).
  std::string test_dir;

  // dest_dir is a temporary directory that we're using as the destination directory.
  // It is backed by temp_dir1.
  std::string dest_dir;

  // conf_dir is a temporary directory that we're using as the configuration directory.
  // It is backed by temp_dir2.
  std::string conf_dir;

  TestLogHelper test_logger;

 private:
  void create_dirs() {
    temp_dir1.reset(new TemporaryDir());
    temp_dir2.reset(new TemporaryDir());
    dest_dir = temp_dir1->path;
    conf_dir = temp_dir2->path;
    test_dir = android::base::Dirname(gExecutableRealpath);
  }

  std::unique_ptr<TemporaryDir> temp_dir1;
  std::unique_ptr<TemporaryDir> temp_dir2;
};

///
/// Helper class to kick off a run of the perfprofd daemon with a specific
/// config file.
///
class PerfProfdRunner {
 public:
  explicit PerfProfdRunner(const std::string& config_dir)
      : config_dir_(config_dir)
  {
    config_path_ = config_dir + "/" CONFIGFILE;
  }

  ~PerfProfdRunner()
  {
    remove_processed_file();
  }

  void addToConfig(const std::string &line)
  {
    config_text_ += line;
    config_text_ += "\n";
  }

  void remove_semaphore_file()
  {
    std::string semaphore(config_dir_);
    semaphore += "/" SEMAPHORE_FILENAME;
    unlink(semaphore.c_str());
  }

  void create_semaphore_file()
  {
    std::string semaphore(config_dir_);
    semaphore += "/" SEMAPHORE_FILENAME;
    close(open(semaphore.c_str(), O_WRONLY|O_CREAT, 0600));
  }

  void write_processed_file(int start_seq, int end_seq)
  {
    std::string processed = config_dir_ + "/" PROCESSED_FILENAME;
    FILE *fp = fopen(processed.c_str(), "w");
    for (int i = start_seq; i < end_seq; i++) {
      fprintf(fp, "%d\n", i);
    }
    fclose(fp);
  }

  void remove_processed_file()
  {
    std::string processed = config_dir_ + "/" PROCESSED_FILENAME;
    unlink(processed.c_str());
  }

  struct LoggingConfig : public Config {
    void Sleep(size_t seconds) override {
      // Log sleep calls but don't sleep.
      LOG(INFO) << "sleep " << seconds << " seconds";
    }

    bool IsProfilingEnabled() const override {
      //
      // Check for existence of semaphore file in config directory
      //
      if (access(config_directory.c_str(), F_OK) == -1) {
        PLOG(WARNING) << "unable to open config directory " << config_directory;
        return false;
      }

      // Check for existence of semaphore file
      std::string semaphore_filepath = config_directory
          + "/" + SEMAPHORE_FILENAME;
      if (access(semaphore_filepath.c_str(), F_OK) == -1) {
        return false;
      }

      return true;
    }
  };

  int invoke()
  {
    static const char *argv[3] = { "perfprofd", "-c", "" };
    argv[2] = config_path_.c_str();

    writeConfigFile(config_path_, config_text_);

    // execute daemon main
    LoggingConfig config;
    return perfprofd_main(3, (char **) argv, &config);
  }

 private:
  std::string config_dir_;
  std::string config_path_;
  std::string config_text_;

  void writeConfigFile(const std::string &config_path,
                       const std::string &config_text)
  {
    FILE *fp = fopen(config_path.c_str(), "w");
    ASSERT_TRUE(fp != nullptr);
    fprintf(fp, "%s\n", config_text.c_str());
    fclose(fp);
  }
};

//......................................................................

static std::string encoded_file_path(const std::string& dest_dir,
                                     int seq) {
  return android::base::StringPrintf("%s/perf.data.encoded.%d",
                                     dest_dir.c_str(), seq);
}

static void readEncodedProfile(const std::string& dest_dir,
                               bool compressed,
                               android::perfprofd::PerfprofdRecord& encodedProfile)
{
  struct stat statb;
  int perf_data_stat_result = stat(encoded_file_path(dest_dir, 0).c_str(), &statb);
  ASSERT_NE(-1, perf_data_stat_result);

  // read
  std::string encoded;
  encoded.resize(statb.st_size);
  FILE *ifp = fopen(encoded_file_path(dest_dir, 0).c_str(), "r");
  ASSERT_NE(nullptr, ifp);
  size_t items_read = fread((void*) encoded.data(), statb.st_size, 1, ifp);
  ASSERT_EQ(1, items_read);
  fclose(ifp);

  // uncompress
  if (compressed && !encoded.empty()) {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    {
      constexpr int kWindowBits = 15;
      constexpr int kGzipEncoding = 16;
      int init_result = inflateInit2(&stream, kWindowBits | kGzipEncoding);
      if (init_result != Z_OK) {
        LOG(ERROR) << "Could not initialize libz stream " << init_result;
        return;
      }
    }

    std::string buf;
    buf.reserve(2 * encoded.size());
    stream.avail_in = encoded.size();
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(encoded.data()));

    int result;
    do {
      uint8_t chunk[1024];
      stream.next_out = static_cast<Bytef*>(chunk);
      stream.avail_out = arraysize(chunk);

      result = inflate(&stream, 0);
      const size_t amount = arraysize(chunk) - stream.avail_out;
      if (amount > 0) {
        if (buf.capacity() - buf.size() < amount) {
          buf.reserve(buf.capacity() + 64u * 1024u);
          CHECK_LE(amount, buf.capacity() - buf.size());
        }
        size_t index = buf.size();
        buf.resize(buf.size() + amount);
        memcpy(reinterpret_cast<uint8_t*>(const_cast<char*>(buf.data())) + index, chunk, amount);
      }
    } while (result == Z_OK);
    inflateEnd(&stream);
    if (result != Z_STREAM_END) {
      LOG(ERROR) << "Finished with not-Z_STREAM_END " << result;
      return;
    }
    encoded = buf;
  }

  // decode
  encodedProfile.ParseFromString(encoded);
}

#define RAW_RESULT(x) #x

TEST_F(PerfProfdTest, TestUtil)
{
  EXPECT_EQ("", replaceAll("", "", ""));
  EXPECT_EQ("zzbc", replaceAll("abc", "a", "zz"));
  EXPECT_EQ("azzc", replaceAll("abc", "b", "zz"));
  EXPECT_EQ("abzz", replaceAll("abc", "c", "zz"));
  EXPECT_EQ("xxyyzz", replaceAll("abc", "abc", "xxyyzz"));
}

TEST_F(PerfProfdTest, MissingGMS)
{
  //
  // AWP requires cooperation between the daemon and the GMS core
  // piece. If we're running on a device that has an old or damaged
  // version of GMS core, then the config directory we're interested in
  // may not be there. This test insures that the daemon does the
  // right thing in this case.
  //
  PerfProfdRunner runner(conf_dir);
  runner.addToConfig("only_debug_build=0");
  runner.addToConfig("trace_config_read=0");
  runner.addToConfig("config_directory=/does/not/exist");
  runner.addToConfig("main_loop_iterations=1");
  runner.addToConfig("use_fixed_seed=1");
  runner.addToConfig("collection_interval=100");

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  EXPECT_EQ(0, daemon_main_return_code);

  // Verify log contents
  const std::string expected = RAW_RESULT(
      I: sleep 90 seconds
      W: unable to open config directory /does/not/exist: No such file or directory
      I: profile collection skipped (missing config directory)
                                          );

  // check to make sure entire log matches
  EXPECT_TRUE(CompareLogMessages(expected));
}


TEST_F(PerfProfdTest, MissingOptInSemaphoreFile)
{
  //
  // Android device owners must opt in to "collect and report usage
  // data" in order for us to be able to collect profiles. The opt-in
  // check is performed in the GMS core component; if the check
  // passes, then it creates a semaphore file for the daemon to pick
  // up on.
  //
  PerfProfdRunner runner(conf_dir);
  runner.addToConfig("only_debug_build=0");
  std::string cfparam("config_directory="); cfparam += conf_dir;
  runner.addToConfig(cfparam);
  std::string ddparam("destination_directory="); ddparam += dest_dir;
  runner.addToConfig(ddparam);
  runner.addToConfig("main_loop_iterations=1");
  runner.addToConfig("use_fixed_seed=1");
  runner.addToConfig("collection_interval=100");

  runner.remove_semaphore_file();

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  EXPECT_EQ(0, daemon_main_return_code);

  // Verify log contents
  const std::string expected = RAW_RESULT(
      I: profile collection skipped (missing config directory)
                                          );
  // check to make sure log excerpt matches
  EXPECT_TRUE(CompareLogMessages(expected));
}

TEST_F(PerfProfdTest, MissingPerfExecutable)
{
  //
  // Perfprofd uses the 'simpleperf' tool to collect profiles
  // (although this may conceivably change in the future). This test
  // checks to make sure that if 'simpleperf' is not present we bail out
  // from collecting profiles.
  //
  PerfProfdRunner runner(conf_dir);
  runner.addToConfig("only_debug_build=0");
  runner.addToConfig("trace_config_read=1");
  std::string cfparam("config_directory="); cfparam += conf_dir;
  runner.addToConfig(cfparam);
  std::string ddparam("destination_directory="); ddparam += dest_dir;
  runner.addToConfig(ddparam);
  runner.addToConfig("main_loop_iterations=1");
  runner.addToConfig("use_fixed_seed=1");
  runner.addToConfig("collection_interval=100");
  runner.addToConfig("perf_path=/does/not/exist");

  // Create semaphore file
  runner.create_semaphore_file();

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  EXPECT_EQ(0, daemon_main_return_code);

  // expected log contents
  const std::string expected = RAW_RESULT(
      I: profile collection skipped (missing 'perf' executable)
                                          );
  // check to make sure log excerpt matches
  EXPECT_TRUE(CompareLogMessages(expected));
}

TEST_F(PerfProfdTest, BadPerfRun)
{
  //
  // Perf tools tend to be tightly coupled with a specific kernel
  // version -- if things are out of sync perf could fail or
  // crash. This test makes sure that we detect such a case and log
  // the error.
  //
  PerfProfdRunner runner(conf_dir);
  runner.addToConfig("only_debug_build=0");
  std::string cfparam("config_directory="); cfparam += conf_dir;
  runner.addToConfig(cfparam);
  std::string ddparam("destination_directory="); ddparam += dest_dir;
  runner.addToConfig(ddparam);
  runner.addToConfig("main_loop_iterations=1");
  runner.addToConfig("use_fixed_seed=1");
  runner.addToConfig("collection_interval=100");
#ifdef __ANDROID__
  runner.addToConfig("perf_path=/system/bin/false");
#else
  runner.addToConfig("perf_path=/bin/false");
#endif

  // Create semaphore file
  runner.create_semaphore_file();

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  EXPECT_EQ(0, daemon_main_return_code);

  // Verify log contents. Because of perferr logging containing pids and test paths,
  // it is easier to have three expected parts.
  const std::string expected1 = "W: perf bad exit status 1";
  const std::string expected2 = "bin/false record";
  const std::string expected3 = "W: profile collection failed";

  // check to make sure log excerpt matches
  EXPECT_TRUE(CompareLogMessages(expected1));
  EXPECT_TRUE(CompareLogMessages(expected2));
  EXPECT_TRUE(CompareLogMessages(expected3));
}

TEST_F(PerfProfdTest, ConfigFileParsing)
{
  //
  // Gracefully handly malformed items in the config file
  //
  PerfProfdRunner runner(conf_dir);
  runner.addToConfig("only_debug_build=0");
  runner.addToConfig("main_loop_iterations=1");
  runner.addToConfig("collection_interval=100");
  runner.addToConfig("use_fixed_seed=1");
  runner.addToConfig("destination_directory=/does/not/exist");

  // assorted bad syntax
  runner.addToConfig("collection_interval=-1");
  runner.addToConfig("collection_interval=18446744073709551615");
  runner.addToConfig("nonexistent_key=something");
  runner.addToConfig("no_equals_stmt");

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  EXPECT_EQ(0, daemon_main_return_code);

  // Verify log contents
  const std::string expected = RAW_RESULT(
      W: line 6: value -1 cannot be parsed
      W: line 7: specified value 18446744073709551615 for 'collection_interval' outside permitted range [0 4294967295]
      W: line 8: unknown option 'nonexistent_key'
      W: line 9: line malformed (no '=' found)
                                          );

  // check to make sure log excerpt matches
  EXPECT_TRUE(CompareLogMessages(expected));
}

TEST_F(PerfProfdTest, ConfigFileParsing_Events) {
  auto check_event_config = [](const Config& config,
                               size_t index,
                               const std::vector<std::string>& names,
                               bool group,
                               uint32_t period) {
    if (config.event_config.size() <= index) {
      return ::testing::AssertionFailure() << "Not enough entries " << config.event_config.size()
                                                                    << " " << index;
    }
    const auto& elem = config.event_config[index];

    if (elem.group != group) {
      return ::testing::AssertionFailure() << "Type wrong " << elem.group << " " << group;
    }

    if (elem.sampling_period != period) {
      return ::testing::AssertionFailure() << "Period wrong " << elem.sampling_period << " "
                                                              << period;
    }

    auto strvec = [](const std::vector<std::string>& v) {
      return "[" + android::base::Join(v, ',') + "]";
    };
    if (elem.events.size() != names.size()) {
      return ::testing::AssertionFailure() << "Names wrong " << strvec(elem.events) << " "
                                                             << strvec(names);
    }
    for (size_t i = 0; i != elem.events.size(); ++i) {
      if (elem.events[i] != names[i]) {
        return ::testing::AssertionFailure() << "Names wrong at " << i << ": "
                                             << strvec(elem.events) << " "
                                             << strvec(names);
      }
    }
    return ::testing::AssertionSuccess();
  };

  {
    std::string data = "-e_hello,world=1\n"
                       "-g_foo,bar=2\n"
                       "-e_abc,xyz=3\n"
                       "-g_ftrace:test,ftrace:test2=4";

    ConfigReader reader;
    std::string error_msg;
    ASSERT_TRUE(reader.Read(data, true, &error_msg)) << error_msg;

    PerfProfdRunner::LoggingConfig config;
    reader.FillConfig(&config);

    EXPECT_TRUE(check_event_config(config, 0, { "hello", "world" }, false, 1));
    EXPECT_TRUE(check_event_config(config, 1, { "foo", "bar" }, true, 2));
    EXPECT_TRUE(check_event_config(config, 2, { "abc", "xyz" }, false, 3));
    EXPECT_TRUE(check_event_config(config, 3, { "ftrace:test", "ftrace:test2" }, true, 4));
  }

  {
    std::string data = "-e_hello,world=dummy";

    ConfigReader reader;
    std::string error_msg;
    EXPECT_FALSE(reader.Read(data, true, &error_msg));
  }

  {
    std::string data = "-g_hello,world=dummy";

    ConfigReader reader;
    std::string error_msg;
    EXPECT_FALSE(reader.Read(data, true, &error_msg));
  }
}


TEST_F(PerfProfdTest, ConfigDump) {
  constexpr const char* kConfigElems[] = {
      "collection_interval=14400",
      "use_fixed_seed=1",
      "main_loop_iterations=2",
      "destination_directory=/does/not/exist",
      "config_directory=a",
      "perf_path=/system/xbin/simpleperf2",
      "sampling_period=3",
      "sampling_frequency=4",
      "sample_duration=5",
      "only_debug_build=1",
      "hardwire_cpus=1",
      "hardwire_cpus_max_duration=6",
      "max_unprocessed_profiles=7",
      "stack_profile=1",
      "trace_config_read=1",
      "collect_cpu_utilization=1",
      "collect_charging_state=1",
      "collect_booting=1",
      "collect_camera_active=1",
      "process=8",
      "use_elf_symbolizer=1",
      "symbolize_everything=1",
      "compress=1",
      "dropbox=1",
      "fail_on_unsupported_events=1",
      "-e_hello,world=1",
      "-g_foo,bar=2",
      "-e_abc,xyz=3",
      "-g_ftrace:test,ftrace:test2=4",
  };

  std::string input;
  for (const char* elem : kConfigElems) {
    input.append(elem);
    input.append("\n");
  }

  ConfigReader reader;
  std::string error_msg;
  ASSERT_TRUE(reader.Read(input, false, &error_msg)) << error_msg;

  PerfProfdRunner::LoggingConfig config;
  reader.FillConfig(&config);

  std::string output = ConfigReader::ConfigToString(config);
  for (const char* elem : kConfigElems) {
    EXPECT_TRUE(output.find(elem) != std::string::npos) << elem << " not in " << output;
  }
}

TEST_F(PerfProfdTest, ProfileCollectionAnnotations)
{
  unsigned util1 = collect_cpu_utilization();
  EXPECT_LE(util1, 100);
  EXPECT_GE(util1, 0);

  // NB: expectation is that when we run this test, the device will be
  // completed booted, will be on charger, and will not have the camera
  // active.
  EXPECT_FALSE(get_booting());
#ifdef __ANDROID__
  EXPECT_TRUE(get_charging());
#endif
  EXPECT_FALSE(get_camera_active());
}

namespace {

template <typename Iterator>
size_t CountEvents(const quipper::PerfDataProto& proto) {
  size_t count = 0;
  for (Iterator it(proto); it != it.end(); ++it) {
    count++;
  }
  return count;
}

size_t CountCommEvents(const quipper::PerfDataProto& proto) {
  return CountEvents<CommEventIterator>(proto);
}
size_t CountMmapEvents(const quipper::PerfDataProto& proto) {
  return CountEvents<MmapEventIterator>(proto);
}
size_t CountSampleEvents(const quipper::PerfDataProto& proto) {
  return CountEvents<SampleEventIterator>(proto);
}
size_t CountForkEvents(const quipper::PerfDataProto& proto) {
  return CountEvents<ForkEventIterator>(proto);
}
size_t CountExitEvents(const quipper::PerfDataProto& proto) {
  return CountEvents<ExitEventIterator>(proto);
}

std::string CreateStats(const quipper::PerfDataProto& proto) {
  std::ostringstream oss;
  oss << "Mmap events: "   << CountMmapEvents(proto) << std::endl;
  oss << "Sample events: " << CountSampleEvents(proto) << std::endl;
  oss << "Comm events: "   << CountCommEvents(proto) << std::endl;
  oss << "Fork events: "   << CountForkEvents(proto) << std::endl;
  oss << "Exit events: "   << CountExitEvents(proto) << std::endl;
  return oss.str();
}

std::string FormatSampleEvent(const quipper::PerfDataProto_SampleEvent& sample) {
  std::ostringstream oss;
  if (sample.has_pid()) {
    oss << "pid=" << sample.pid();
  }
  if (sample.has_tid()) {
    oss << " tid=" << sample.tid();
  }
  if (sample.has_ip()) {
      oss << " ip=" << sample.ip();
    }
  if (sample.has_addr()) {
    oss << " addr=" << sample.addr();
  }
  if (sample.callchain_size() > 0) {
    oss << " callchain=";
    for (uint64_t cc : sample.callchain()) {
      oss << "->" << cc;
    }
  }
  return oss.str();
}

}

struct BasicRunWithCannedPerf : PerfProfdTest {
  void VerifyBasicCannedProfile(const android::perfprofd::PerfprofdRecord& encodedProfile) {
    const quipper::PerfDataProto& perf_data = encodedProfile;

    // Expect 21108 events.
    EXPECT_EQ(21108, perf_data.events_size()) << CreateStats(perf_data);

    EXPECT_EQ(48,    CountMmapEvents(perf_data)) << CreateStats(perf_data);
    EXPECT_EQ(19986, CountSampleEvents(perf_data)) << CreateStats(perf_data);
    EXPECT_EQ(1033,  CountCommEvents(perf_data)) << CreateStats(perf_data);
    EXPECT_EQ(15,    CountForkEvents(perf_data)) << CreateStats(perf_data);
    EXPECT_EQ(26,    CountExitEvents(perf_data)) << CreateStats(perf_data);

    if (HasNonfatalFailure()) {
      FAIL();
    }

    {
      MmapEventIterator mmap(perf_data);
      constexpr std::pair<const char*, uint64_t> kMmapEvents[] = {
          std::make_pair("[kernel.kallsyms]_text", 0),
          std::make_pair("/system/lib/libc.so", 3067412480u),
          std::make_pair("/system/vendor/lib/libdsutils.so", 3069911040u),
          std::make_pair("/system/lib/libc.so", 3067191296u),
          std::make_pair("/system/lib/libc++.so", 3069210624u),
          std::make_pair("/data/dalvik-cache/arm/system@framework@boot.oat", 1900048384u),
          std::make_pair("/system/lib/libjavacore.so", 2957135872u),
          std::make_pair("/system/vendor/lib/libqmi_encdec.so", 3006644224u),
          std::make_pair("/data/dalvik-cache/arm/system@framework@wifi-service.jar@classes.dex",
                         3010351104u),
                         std::make_pair("/system/lib/libart.so", 3024150528u),
                         std::make_pair("/system/lib/libz.so", 3056410624u),
                         std::make_pair("/system/lib/libicui18n.so", 3057610752u),
      };
      for (auto& pair : kMmapEvents) {
        EXPECT_STREQ(pair.first, mmap->mmap_event().filename().c_str());
        EXPECT_EQ(pair.second, mmap->mmap_event().start()) << pair.first;
        ++mmap;
      }
    }

    {
      CommEventIterator comm(perf_data);
      constexpr const char* kCommEvents[] = {
          "init", "kthreadd", "ksoftirqd/0", "kworker/u:0H", "migration/0", "khelper",
          "netns", "modem_notifier", "smd_channel_clo", "smsm_cb_wq", "rpm-smd", "kworker/u:1H",
      };
      for (auto str : kCommEvents) {
        EXPECT_STREQ(str, comm->comm_event().comm().c_str());
        ++comm;
      }
    }

    {
      SampleEventIterator samples(perf_data);
      constexpr const char* kSampleEvents[] = {
          "pid=0 tid=0 ip=3222720196",
          "pid=0 tid=0 ip=3222910876",
          "pid=0 tid=0 ip=3222910876",
          "pid=0 tid=0 ip=3222910876",
          "pid=0 tid=0 ip=3222910876",
          "pid=0 tid=0 ip=3222910876",
          "pid=0 tid=0 ip=3222910876",
          "pid=3 tid=3 ip=3231975108",
          "pid=5926 tid=5926 ip=3231964952",
          "pid=5926 tid=5926 ip=3225342428",
          "pid=5926 tid=5926 ip=3223841448",
          "pid=5926 tid=5926 ip=3069807920",
      };
      for (auto str : kSampleEvents) {
        EXPECT_STREQ(str, FormatSampleEvent(samples->sample_event()).c_str());
        ++samples;
      }

      // Skip some samples.
      for (size_t i = 0; i != 5000; ++i) {
        ++samples;
      }
      constexpr const char* kSampleEvents2[] = {
          "pid=5938 tid=5938 ip=3069630992",
          "pid=5938 tid=5938 ip=3069626616",
          "pid=5938 tid=5938 ip=3069626636",
          "pid=5938 tid=5938 ip=3069637212",
          "pid=5938 tid=5938 ip=3069637208",
          "pid=5938 tid=5938 ip=3069637252",
          "pid=5938 tid=5938 ip=3069346040",
          "pid=5938 tid=5938 ip=3069637128",
          "pid=5938 tid=5938 ip=3069626616",
      };
      for (auto str : kSampleEvents2) {
        EXPECT_STREQ(str, FormatSampleEvent(samples->sample_event()).c_str());
        ++samples;
      }

      // Skip some samples.
      for (size_t i = 0; i != 5000; ++i) {
        ++samples;
      }
      constexpr const char* kSampleEvents3[] = {
          "pid=5938 tid=5938 ip=3069912036",
          "pid=5938 tid=5938 ip=3069637260",
          "pid=5938 tid=5938 ip=3069631024",
          "pid=5938 tid=5938 ip=3069346064",
          "pid=5938 tid=5938 ip=3069637356",
          "pid=5938 tid=5938 ip=3069637144",
          "pid=5938 tid=5938 ip=3069912036",
          "pid=5938 tid=5938 ip=3069912036",
          "pid=5938 tid=5938 ip=3069631244",
      };
      for (auto str : kSampleEvents3) {
        EXPECT_STREQ(str, FormatSampleEvent(samples->sample_event()).c_str());
        ++samples;
      }
    }
  }
};

TEST_F(BasicRunWithCannedPerf, Basic)
{
  //
  // Verify the portion of the daemon that reads and encodes
  // perf.data files. Here we run the encoder on a canned perf.data
  // file and verify that the resulting protobuf contains what
  // we think it should contain.
  //
  std::string input_perf_data(test_dir);
  input_perf_data += "/canned.perf.data";

  // Set up config to avoid these annotations (they are tested elsewhere)
  ConfigReader config_reader;
  config_reader.overrideUnsignedEntry("collect_cpu_utilization", 0);
  config_reader.overrideUnsignedEntry("collect_charging_state", 0);
  config_reader.overrideUnsignedEntry("collect_camera_active", 0);

  // Disable compression.
  config_reader.overrideUnsignedEntry("compress", 0);

  PerfProfdRunner::LoggingConfig config;
  config_reader.FillConfig(&config);

  // Kick off encoder and check return code
  PROFILE_RESULT result =
      encode_to_proto(input_perf_data, encoded_file_path(dest_dir, 0).c_str(), config, 0, nullptr);
  ASSERT_EQ(OK_PROFILE_COLLECTION, result) << test_logger.JoinTestLog(" ");

  // Read and decode the resulting perf.data.encoded file
  android::perfprofd::PerfprofdRecord encodedProfile;
  readEncodedProfile(dest_dir, false, encodedProfile);

  VerifyBasicCannedProfile(encodedProfile);
}

TEST_F(BasicRunWithCannedPerf, Compressed)
{
  //
  // Verify the portion of the daemon that reads and encodes
  // perf.data files. Here we run the encoder on a canned perf.data
  // file and verify that the resulting protobuf contains what
  // we think it should contain.
  //
  std::string input_perf_data(test_dir);
  input_perf_data += "/canned.perf.data";

  // Set up config to avoid these annotations (they are tested elsewhere)
  ConfigReader config_reader;
  config_reader.overrideUnsignedEntry("collect_cpu_utilization", 0);
  config_reader.overrideUnsignedEntry("collect_charging_state", 0);
  config_reader.overrideUnsignedEntry("collect_camera_active", 0);

  // Enable compression.
  config_reader.overrideUnsignedEntry("compress", 1);

  PerfProfdRunner::LoggingConfig config;
  config_reader.FillConfig(&config);

  // Kick off encoder and check return code
  PROFILE_RESULT result =
      encode_to_proto(input_perf_data, encoded_file_path(dest_dir, 0).c_str(), config, 0, nullptr);
  ASSERT_EQ(OK_PROFILE_COLLECTION, result) << test_logger.JoinTestLog(" ");

  // Read and decode the resulting perf.data.encoded file
  android::perfprofd::PerfprofdRecord encodedProfile;
  readEncodedProfile(dest_dir, true, encodedProfile);

  VerifyBasicCannedProfile(encodedProfile);
}

class BasicRunWithCannedPerfWithSymbolizer : public BasicRunWithCannedPerf {
 protected:
  std::vector<::testing::AssertionResult> Run(bool symbolize_everything, size_t expected_count) {
    //
    // Verify the portion of the daemon that reads and encodes
    // perf.data files. Here we run the encoder on a canned perf.data
    // file and verify that the resulting protobuf contains what
    // we think it should contain.
    //
    std::string input_perf_data(test_dir);
    input_perf_data += "/canned.perf.data";

    // Set up config to avoid these annotations (they are tested elsewhere)
    ConfigReader config_reader;
    config_reader.overrideUnsignedEntry("collect_cpu_utilization", 0);
    config_reader.overrideUnsignedEntry("collect_charging_state", 0);
    config_reader.overrideUnsignedEntry("collect_camera_active", 0);

    // Disable compression.
    config_reader.overrideUnsignedEntry("compress", 0);

    if (symbolize_everything) {
      config_reader.overrideUnsignedEntry("symbolize_everything", 1);
    }

    PerfProfdRunner::LoggingConfig config;
    config_reader.FillConfig(&config);

    // Kick off encoder and check return code
    struct TestSymbolizer : public perfprofd::Symbolizer {
      std::string Decode(const std::string& dso, uint64_t address) override {
        return dso + "@" + std::to_string(address);
      }
      bool GetMinExecutableVAddr(const std::string& dso, uint64_t* addr) override {
        *addr = 4096;
        return true;
      }
    };
    TestSymbolizer test_symbolizer;
    PROFILE_RESULT result =
        encode_to_proto(input_perf_data,
                        encoded_file_path(dest_dir, 0).c_str(),
                        config,
                        0,
                        &test_symbolizer);
    if (result != OK_PROFILE_COLLECTION) {
      return { ::testing::AssertionFailure() << "Profile collection failed: " << result };
    }

    std::vector<::testing::AssertionResult> ret;

    // Read and decode the resulting perf.data.encoded file
    android::perfprofd::PerfprofdRecord encodedProfile;
    readEncodedProfile(dest_dir, false, encodedProfile);

    VerifyBasicCannedProfile(encodedProfile);

    auto find_symbol = [&](const std::string& filename) -> const quipper::SymbolInfo* {
      const size_t size = encodedProfile.ExtensionSize(quipper::symbol_info);
      for (size_t i = 0; i != size; ++i) {
        auto& symbol_info = encodedProfile.GetExtension(quipper::symbol_info, i);
        if (symbol_info.filename() == filename) {
          return &symbol_info;
        }
      }
      return nullptr;
    };
    auto all_filenames = [&]() {
      std::ostringstream oss;
      const size_t size = encodedProfile.ExtensionSize(quipper::symbol_info);
      for (size_t i = 0; i != size; ++i) {
        auto& symbol_info = encodedProfile.GetExtension(quipper::symbol_info, i);
        oss << " " << symbol_info.filename();
      }
      return oss.str();
    };

    auto check_dsos = [&](const char* const* dsos, const size_t len) {
      bool failed = false;
      for (size_t i = 0; i != len; ++i) {
        if (find_symbol(dsos[i]) == nullptr) {
          failed = true;
          ret.push_back(::testing::AssertionFailure() << "Did not find " << dsos[i]);
        }
      }
      return failed;
    };

    bool failed = false;

    constexpr const char* kDSOs[] = {
        "/data/app/com.google.android.apps.plus-1/lib/arm/libcronet.so",
        "/data/dalvik-cache/arm/system@framework@wifi-service.jar@classes.dex",
        "/data/dalvik-cache/arm/data@app@com.google.android.gms-2@base.apk@classes.dex",
        "/data/dalvik-cache/arm/system@framework@boot.oat",
    };
    failed |= check_dsos(kDSOs, arraysize(kDSOs));

    if (symbolize_everything) {
      constexpr const char* kDSOsWithBuildIDs[] = {
          "/system/lib/libz.so", "/system/lib/libutils.so",
      };
      failed |= check_dsos(kDSOsWithBuildIDs, arraysize(kDSOsWithBuildIDs));
    }

    if (failed) {
      ret.push_back(::testing::AssertionFailure() << "Found: " << all_filenames());
    }

    if (encodedProfile.ExtensionSize(quipper::symbol_info) != expected_count) {
      ret.push_back(
          ::testing::AssertionFailure() << "Expected " << expected_count
                                        << " symbolized libraries, found "
                                        << encodedProfile.ExtensionSize(quipper::symbol_info));
    }

    return ret;
  }
};

TEST_F(BasicRunWithCannedPerfWithSymbolizer, Default) {
  auto result = Run(false, 5);
  for (const auto& result_component : result) {
    EXPECT_TRUE(result_component);
  }
}

TEST_F(BasicRunWithCannedPerfWithSymbolizer, Everything) {
  auto result = Run(true, 26);
  for (const auto& result_component : result) {
    EXPECT_TRUE(result_component);
  }
}

TEST_F(PerfProfdTest, CallchainRunWithCannedPerf)
{
  // This test makes sure that the perf.data converter
  // can handle call chains.
  //
  std::string input_perf_data(test_dir);
  input_perf_data += "/callchain.canned.perf.data";

  // Set up config to avoid these annotations (they are tested elsewhere)
  ConfigReader config_reader;
  config_reader.overrideUnsignedEntry("collect_cpu_utilization", 0);
  config_reader.overrideUnsignedEntry("collect_charging_state", 0);
  config_reader.overrideUnsignedEntry("collect_camera_active", 0);

  // Disable compression.
  config_reader.overrideUnsignedEntry("compress", 0);

  PerfProfdRunner::LoggingConfig config;
  config_reader.FillConfig(&config);

  // Kick off encoder and check return code
  PROFILE_RESULT result =
      encode_to_proto(input_perf_data, encoded_file_path(dest_dir, 0).c_str(), config, 0, nullptr);
  ASSERT_EQ(OK_PROFILE_COLLECTION, result);

  // Read and decode the resulting perf.data.encoded file
  android::perfprofd::PerfprofdRecord encodedProfile;
  readEncodedProfile(dest_dir, false, encodedProfile);

  const quipper::PerfDataProto& perf_data = encodedProfile;

  // Expect 21108 events.
  EXPECT_EQ(2224, perf_data.events_size()) << CreateStats(perf_data);

  {
      SampleEventIterator samples(perf_data);
      constexpr const char* kSampleEvents[] = {
          "0: pid=6225 tid=6225 ip=18446743798834668032 callchain=->18446744073709551488->"
              "18446743798834668032->18446743798834782596->18446743798834784624->"
              "18446743798835055136->18446743798834788016->18446743798834789192->"
              "18446743798834789512->18446743798834790216->18446743798833756776",
          "1: pid=6225 tid=6225 ip=18446743798835685700 callchain=->18446744073709551488->"
              "18446743798835685700->18446743798835688704->18446743798835650964->"
              "18446743798834612104->18446743798834612276->18446743798835055528->"
              "18446743798834788016->18446743798834789192->18446743798834789512->"
              "18446743798834790216->18446743798833756776",
          "2: pid=6225 tid=6225 ip=18446743798835055804 callchain=->18446744073709551488->"
              "18446743798835055804->18446743798834788016->18446743798834789192->"
              "18446743798834789512->18446743798834790216->18446743798833756776",
          "3: pid=6225 tid=6225 ip=18446743798835991212 callchain=->18446744073709551488->"
              "18446743798835991212->18446743798834491060->18446743798834675572->"
              "18446743798834676516->18446743798834612172->18446743798834612276->"
              "18446743798835056664->18446743798834788016->18446743798834789192->"
              "18446743798834789512->18446743798834790216->18446743798833756776",
          "4: pid=6225 tid=6225 ip=18446743798844881108 callchain=->18446744073709551488->"
              "18446743798844881108->18446743798834836140->18446743798834846384->"
              "18446743798834491100->18446743798834675572->18446743798834676516->"
              "18446743798834612172->18446743798834612276->18446743798835056784->"
              "18446743798834788016->18446743798834789192->18446743798834789512->"
              "18446743798834790216->18446743798833756776",
      };
      size_t cmp_index = 0;
      for (size_t index = 0; samples != samples.end(); ++samples, ++index) {
        if (samples->sample_event().callchain_size() > 0) {
          std::ostringstream oss;
          oss << index << ": " << FormatSampleEvent(samples->sample_event());
          EXPECT_STREQ(kSampleEvents[cmp_index], oss.str().c_str());
          cmp_index++;
          if (cmp_index == arraysize(kSampleEvents)) {
            break;
          }
        }
      }
  }
}

#ifdef __ANDROID__

TEST_F(PerfProfdTest, GetSupportedPerfCounters)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  // Check basic perf counters.
  {
    struct DummyConfig : public Config {
      void Sleep(size_t seconds) override {}
      bool IsProfilingEnabled() const override { return false; }
    };
    DummyConfig config;
    ASSERT_TRUE(android::perfprofd::FindSupportedPerfCounters(config.perf_path));
  }
  const std::unordered_set<std::string>& counters = android::perfprofd::GetSupportedPerfCounters();
  EXPECT_TRUE(std::find(counters.begin(), counters.end(), std::string("cpu-cycles"))
                  != counters.end()) << android::base::Join(counters, ',');
  EXPECT_TRUE(std::find(counters.begin(), counters.end(), std::string("page-faults"))
                  != counters.end()) << android::base::Join(counters, ',');
}

TEST_F(PerfProfdTest, BasicRunWithLivePerf)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  //
  // Basic test to exercise the main loop of the daemon. It includes
  // a live 'perf' run
  //
  PerfProfdRunner runner(conf_dir);
  runner.addToConfig("only_debug_build=0");
  std::string ddparam("destination_directory="); ddparam += dest_dir;
  runner.addToConfig(ddparam);
  std::string cfparam("config_directory="); cfparam += conf_dir;
  runner.addToConfig(cfparam);
  runner.addToConfig("main_loop_iterations=1");
  runner.addToConfig("use_fixed_seed=12345678");
  runner.addToConfig("max_unprocessed_profiles=100");
  runner.addToConfig("collection_interval=9999");
  runner.addToConfig("sample_duration=2");
  // Avoid the symbolizer for spurious messages.
  runner.addToConfig("use_elf_symbolizer=0");

  // Disable compression.
  runner.addToConfig("compress=0");

  // Create semaphore file
  runner.create_semaphore_file();

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  ASSERT_EQ(0, daemon_main_return_code);

  // Read and decode the resulting perf.data.encoded file
  android::perfprofd::PerfprofdRecord encodedProfile;
  readEncodedProfile(dest_dir, false, encodedProfile);

  // Examine what we get back. Since it's a live profile, we can't
  // really do much in terms of verifying the contents.
  EXPECT_LT(0, encodedProfile.events_size());

  // Verify log contents
  const std::string expected = std::string(
      "I: starting Android Wide Profiling daemon ") +
      "I: config file path set to " + conf_dir + "/perfprofd.conf " +
      RAW_RESULT(
      I: random seed set to 12345678
      I: sleep 674 seconds
      I: initiating profile collection
      I: sleep 2 seconds
      I: profile collection complete
      I: sleep 9325 seconds
      I: finishing Android Wide Profiling daemon
                                          );
  // check to make sure log excerpt matches
  EXPECT_TRUE(CompareLogMessages(expandVars(expected), true));
}

class PerfProfdLiveEventsTest : public PerfProfdTest {
 protected:
  ::testing::AssertionResult SetupAndInvoke(
      const std::string& event_config,
      const std::vector<std::string>& extra_config,
      bool expect_success,
      std::string expected_log,
      bool log_match_exact) {
    //
    // Basic test to check that the event set functionality works.
    //
    // Note: this is brittle, as we do not really know which events the hardware
    //       supports. Use "cpu-cycles" and "page-faults" as something likely.
    //
    PerfProfdRunner runner(conf_dir);
    runner.addToConfig("only_debug_build=0");
    std::string ddparam("destination_directory="); ddparam += dest_dir;
    runner.addToConfig(ddparam);
    std::string cfparam("config_directory="); cfparam += conf_dir;
    runner.addToConfig(cfparam);
    runner.addToConfig("main_loop_iterations=1");
    runner.addToConfig("use_fixed_seed=12345678");
    runner.addToConfig("max_unprocessed_profiles=100");
    runner.addToConfig("collection_interval=9999");
    runner.addToConfig("sample_duration=2");
    // Avoid the symbolizer for spurious messages.
    runner.addToConfig("use_elf_symbolizer=0");

    // Disable compression.
    runner.addToConfig("compress=0");

    // Set event set.
    runner.addToConfig(event_config);

    for (const std::string& str : extra_config) {
      runner.addToConfig(str);
    }

    // Create semaphore file
    runner.create_semaphore_file();

    // Kick off daemon
    int daemon_main_return_code = runner.invoke();

    // Check return code from daemon
    if (0 != daemon_main_return_code) {
      return ::testing::AssertionFailure() << "Daemon exited with " << daemon_main_return_code;
    }

    if (expect_success) {
      // Read and decode the resulting perf.data.encoded file
      android::perfprofd::PerfprofdRecord encodedProfile;
      readEncodedProfile(dest_dir, false, encodedProfile);

      // Examine what we get back. Since it's a live profile, we can't
      // really do much in terms of verifying the contents.
      if (0 == encodedProfile.events_size()) {
        return ::testing::AssertionFailure() << "Empty encoded profile.";
      }
    }

    // Verify log contents
    return CompareLogMessages(expandVars(expected_log), log_match_exact);
  }
};

TEST_F(PerfProfdLiveEventsTest, BasicRunWithLivePerf_Events)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  const std::string expected = std::string(
        "I: starting Android Wide Profiling daemon ") +
        "I: config file path set to " + conf_dir + "/perfprofd.conf " +
        RAW_RESULT(
        I: random seed set to 12345678
        I: sleep 674 seconds
        I: initiating profile collection
        I: sleep 2 seconds
        I: profile collection complete
        I: sleep 9325 seconds
        I: finishing Android Wide Profiling daemon
                                            );
  ASSERT_TRUE(SetupAndInvoke("-e_cpu-cycles,page-faults=100000", {}, true, expected, true));
}

TEST_F(PerfProfdLiveEventsTest, BasicRunWithLivePerf_Events_Strip)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  const std::string expected = std::string(
        "I: starting Android Wide Profiling daemon ") +
        "I: config file path set to " + conf_dir + "/perfprofd.conf " +
        RAW_RESULT(
        I: random seed set to 12345678
        I: sleep 674 seconds
        I: initiating profile collection
        W: Event does:not:exist is unsupported.
        I: sleep 2 seconds
        I: profile collection complete
        I: sleep 9325 seconds
        I: finishing Android Wide Profiling daemon
                                            );
  ASSERT_TRUE(SetupAndInvoke("-e_cpu-cycles,page-faults,does:not:exist=100000",
                             { "fail_on_unsupported_events=0" },
                             true,
                             expected,
                             true));
}

TEST_F(PerfProfdLiveEventsTest, BasicRunWithLivePerf_Events_NoStrip)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  const std::string expected =
      RAW_RESULT(
      W: Event does:not:exist is unsupported.
      W: profile collection failed
                                          );
  ASSERT_TRUE(SetupAndInvoke("-e_cpu-cycles,page-faults,does:not:exist=100000",
                             { "fail_on_unsupported_events=1" },
                             false,
                             expected,
                             false));
}

TEST_F(PerfProfdLiveEventsTest, BasicRunWithLivePerf_EventsGroup)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  const std::string expected = std::string(
        "I: starting Android Wide Profiling daemon ") +
        "I: config file path set to " + conf_dir + "/perfprofd.conf " +
        RAW_RESULT(
        I: random seed set to 12345678
        I: sleep 674 seconds
        I: initiating profile collection
        I: sleep 2 seconds
        I: profile collection complete
        I: sleep 9325 seconds
        I: finishing Android Wide Profiling daemon
                                            );
  ASSERT_TRUE(SetupAndInvoke("-g_cpu-cycles,page-faults=100000", {}, true, expected, true));
}

TEST_F(PerfProfdTest, MultipleRunWithLivePerf)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  //
  // Basic test to exercise the main loop of the daemon. It includes
  // a live 'perf' run
  //
  PerfProfdRunner runner(conf_dir);
  runner.addToConfig("only_debug_build=0");
  std::string ddparam("destination_directory="); ddparam += dest_dir;
  runner.addToConfig(ddparam);
  std::string cfparam("config_directory="); cfparam += conf_dir;
  runner.addToConfig(cfparam);
  runner.addToConfig("main_loop_iterations=3");
  runner.addToConfig("use_fixed_seed=12345678");
  runner.addToConfig("collection_interval=9999");
  runner.addToConfig("sample_duration=2");
  // Avoid the symbolizer for spurious messages.
  runner.addToConfig("use_elf_symbolizer=0");

  // Disable compression.
  runner.addToConfig("compress=0");

  runner.write_processed_file(1, 2);

  // Create semaphore file
  runner.create_semaphore_file();

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  ASSERT_EQ(0, daemon_main_return_code);

  // Read and decode the resulting perf.data.encoded file
  android::perfprofd::PerfprofdRecord encodedProfile;
  readEncodedProfile(dest_dir, false, encodedProfile);

  // Examine what we get back. Since it's a live profile, we can't
  // really do much in terms of verifying the contents.
  EXPECT_LT(0, encodedProfile.events_size());

  // Examine that encoded.1 file is removed while encoded.{0|2} exists.
  EXPECT_EQ(0, access(encoded_file_path(dest_dir, 0).c_str(), F_OK));
  EXPECT_NE(0, access(encoded_file_path(dest_dir, 1).c_str(), F_OK));
  EXPECT_EQ(0, access(encoded_file_path(dest_dir, 2).c_str(), F_OK));

  // Verify log contents
  const std::string expected = std::string(
      "I: starting Android Wide Profiling daemon ") +
      "I: config file path set to " + conf_dir + "/perfprofd.conf " +
      RAW_RESULT(
      I: random seed set to 12345678
      I: sleep 674 seconds
      I: initiating profile collection
      I: sleep 2 seconds
      I: profile collection complete
      I: sleep 9325 seconds
      I: sleep 4974 seconds
      I: initiating profile collection
      I: sleep 2 seconds
      I: profile collection complete
      I: sleep 5025 seconds
      I: sleep 501 seconds
      I: initiating profile collection
      I: sleep 2 seconds
      I: profile collection complete
      I: sleep 9498 seconds
      I: finishing Android Wide Profiling daemon
                                          );
  // check to make sure log excerpt matches
  EXPECT_TRUE(CompareLogMessages(expandVars(expected), true));
}

TEST_F(PerfProfdTest, CallChainRunWithLivePerf)
{
  if (!IsPerfSupported()) {
    std::cerr << "Test not supported!" << std::endl;
    return;
  }
  //
  // Collect a callchain profile, so as to exercise the code in
  // perf_data post-processing that digests callchains.
  //
  PerfProfdRunner runner(conf_dir);
  std::string ddparam("destination_directory="); ddparam += dest_dir;
  runner.addToConfig(ddparam);
  std::string cfparam("config_directory="); cfparam += conf_dir;
  runner.addToConfig(cfparam);
  runner.addToConfig("main_loop_iterations=1");
  runner.addToConfig("use_fixed_seed=12345678");
  runner.addToConfig("max_unprocessed_profiles=100");
  runner.addToConfig("collection_interval=9999");
  runner.addToConfig("stack_profile=1");
  runner.addToConfig("sample_duration=2");
  // Avoid the symbolizer for spurious messages.
  runner.addToConfig("use_elf_symbolizer=0");

  // Disable compression.
  runner.addToConfig("compress=0");

  // Create semaphore file
  runner.create_semaphore_file();

  // Kick off daemon
  int daemon_main_return_code = runner.invoke();

  // Check return code from daemon
  ASSERT_EQ(0, daemon_main_return_code);

  // Read and decode the resulting perf.data.encoded file
  android::perfprofd::PerfprofdRecord encodedProfile;
  readEncodedProfile(dest_dir, false, encodedProfile);

  // Examine what we get back. Since it's a live profile, we can't
  // really do much in terms of verifying the contents.
  EXPECT_LT(0, encodedProfile.events_size());

  // Verify log contents
  const std::string expected = std::string(
      "I: starting Android Wide Profiling daemon ") +
      "I: config file path set to " + conf_dir + "/perfprofd.conf " +
      RAW_RESULT(
      I: random seed set to 12345678
      I: sleep 674 seconds
      I: initiating profile collection
      I: sleep 2 seconds
      I: profile collection complete
      I: sleep 9325 seconds
      I: finishing Android Wide Profiling daemon
                                          );
  // check to make sure log excerpt matches
  EXPECT_TRUE(CompareLogMessages(expandVars(expected), true));

  // Check that we have at least one SampleEvent with a callchain.
  SampleEventIterator samples(encodedProfile);
  bool found_callchain = false;
  while (!found_callchain && samples != samples.end()) {
    found_callchain = samples->sample_event().callchain_size() > 0;
  }
  EXPECT_TRUE(found_callchain) << CreateStats(encodedProfile);
}

#endif

class RangeMapTest : public testing::Test {
};

TEST_F(RangeMapTest, TestRangeMap) {
  using namespace android::perfprofd;

  RangeMap<std::string, uint64_t> map;
  auto print = [&]() {
    std::ostringstream oss;
    for (auto& aggr_sym : map) {
      oss << aggr_sym.first << "#" << aggr_sym.second.symbol;
      oss << "[";
      for (auto& x : aggr_sym.second.offsets) {
        oss << x << ",";
      }
      oss << "]";
    }
    return oss.str();
  };

  EXPECT_STREQ("", print().c_str());

  map.Insert("a", 10);
  EXPECT_STREQ("10#a[10,]", print().c_str());
  map.Insert("a", 100);
  EXPECT_STREQ("10#a[10,100,]", print().c_str());
  map.Insert("a", 1);
  EXPECT_STREQ("1#a[1,10,100,]", print().c_str());
  map.Insert("a", 1);
  EXPECT_STREQ("1#a[1,10,100,]", print().c_str());
  map.Insert("a", 2);
  EXPECT_STREQ("1#a[1,2,10,100,]", print().c_str());

  map.Insert("b", 200);
  EXPECT_STREQ("1#a[1,2,10,100,]200#b[200,]", print().c_str());
  map.Insert("b", 199);
  EXPECT_STREQ("1#a[1,2,10,100,]199#b[199,200,]", print().c_str());

  map.Insert("c", 50);
  EXPECT_STREQ("1#a[1,2,10,]50#c[50,]100#a[100,]199#b[199,200,]", print().c_str());
}

class ThreadedHandlerTest : public PerfProfdTest {
 public:
  void SetUp() override {
    PerfProfdTest::SetUp();
    threaded_handler_.reset(new android::perfprofd::ThreadedHandler());
  }

  void TearDown() override {
    threaded_handler_.reset();
    PerfProfdTest::TearDown();
  }

 protected:
  std::unique_ptr<android::perfprofd::ThreadedHandler> threaded_handler_;
};

TEST_F(ThreadedHandlerTest, Basic) {
  std::string error_msg;
  EXPECT_FALSE(threaded_handler_->StopProfiling(&error_msg));
}

#ifdef __ANDROID__
#define ThreadedHandlerTestName(x) x
#else
#define ThreadedHandlerTestName(x) DISABLED_ ## x
#endif

TEST_F(ThreadedHandlerTest, ThreadedHandlerTestName(Live)) {
  auto config_fn = [](android::perfprofd::ThreadedConfig& config) {
    // Use some values that make it likely that things don't fail quickly.
    config.main_loop_iterations = 0;
    config.collection_interval_in_s = 1000000;
  };
  std::string error_msg;
  ASSERT_TRUE(threaded_handler_->StartProfiling(config_fn, &error_msg)) << error_msg;
  EXPECT_TRUE(threaded_handler_->StopProfiling(&error_msg)) << error_msg;
}

int main(int argc, char **argv) {
  // Always log to cerr, so that device failures are visible.
  android::base::SetLogger(android::base::StderrLogger);

  CHECK(android::base::Realpath(argv[0], &gExecutableRealpath));

  // switch to / before starting testing (perfprofd
  // should be location-independent)
  chdir("/");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
