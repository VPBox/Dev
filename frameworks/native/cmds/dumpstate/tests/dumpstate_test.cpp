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

#define LOG_TAG "dumpstate"
#include <cutils/log.h>

#include "DumpstateInternal.h"
#include "DumpstateService.h"
#include "android/os/BnDumpstate.h"
#include "dumpstate.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fcntl.h>
#include <libgen.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>

#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <cutils/properties.h>

namespace android {
namespace os {
namespace dumpstate {

using ::testing::EndsWith;
using ::testing::HasSubstr;
using ::testing::IsNull;
using ::testing::IsEmpty;
using ::testing::NotNull;
using ::testing::StrEq;
using ::testing::StartsWith;
using ::testing::Test;
using ::testing::internal::CaptureStderr;
using ::testing::internal::CaptureStdout;
using ::testing::internal::GetCapturedStderr;
using ::testing::internal::GetCapturedStdout;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

class DumpstateListenerMock : public IDumpstateListener {
  public:
    MOCK_METHOD1(onProgress, binder::Status(int32_t progress));
    MOCK_METHOD1(onError, binder::Status(int32_t error_code));
    MOCK_METHOD0(onFinished, binder::Status());
    MOCK_METHOD1(onProgressUpdated, binder::Status(int32_t progress));
    MOCK_METHOD1(onMaxProgressUpdated, binder::Status(int32_t max_progress));
    MOCK_METHOD4(onSectionComplete, binder::Status(const ::std::string& name, int32_t status,
                                                   int32_t size, int32_t durationMs));

  protected:
    MOCK_METHOD0(onAsBinder, IBinder*());
};

static int calls_;

// Base class for all tests in this file
class DumpstateBaseTest : public Test {
  public:
    virtual void SetUp() override {
        calls_++;
        SetDryRun(false);
    }

    void SetDryRun(bool dry_run) const {
        PropertiesHelper::dry_run_ = dry_run;
    }

    void SetBuildType(const std::string& build_type) const {
        PropertiesHelper::build_type_ = build_type;
    }

    void SetUnroot(bool unroot) const {
        PropertiesHelper::unroot_ = unroot;
    }

    bool IsStandalone() const {
        return calls_ == 1;
    }

    void DropRoot() const {
        DropRootUser();
        uid_t uid = getuid();
        ASSERT_EQ(2000, (int)uid);
    }

  protected:
    const std::string kTestPath = dirname(android::base::GetExecutablePath().c_str());
    const std::string kFixturesPath = kTestPath + "/../dumpstate_test_fixture/";
    const std::string kTestDataPath = kFixturesPath + "tests/testdata/";
    const std::string kSimpleCommand = kFixturesPath + "dumpstate_test_fixture";
    const std::string kEchoCommand = "/system/bin/echo";

    /*
     * Copies a text file fixture to a temporary file, returning it's path.
     *
     * Useful in cases where the test case changes the content of the tile.
     */
    std::string CopyTextFileFixture(const std::string& relative_name) {
        std::string from = kTestDataPath + relative_name;
        // Not using TemporaryFile because it's deleted at the end, and it's useful to keep it
        // around for poking when the test fails.
        std::string to = kTestDataPath + relative_name + ".tmp";
        ALOGD("CopyTextFileFixture: from %s to %s\n", from.c_str(), to.c_str());
        android::base::RemoveFileIfExists(to);
        CopyTextFile(from, to);
        return to.c_str();
    }

    // Need functions that returns void to use assertions -
    // https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#assertion-placement
    void ReadFileToString(const std::string& path, std::string* content) {
        ASSERT_TRUE(android::base::ReadFileToString(path, content))
            << "could not read contents from " << path;
    }
    void WriteStringToFile(const std::string& content, const std::string& path) {
        ASSERT_TRUE(android::base::WriteStringToFile(content, path))
            << "could not write contents to " << path;
    }

  private:
    void CopyTextFile(const std::string& from, const std::string& to) {
        std::string content;
        ReadFileToString(from, &content);
        WriteStringToFile(content, to);
    }
};

class DumpOptionsTest : public Test {
  public:
    virtual ~DumpOptionsTest() {
    }
    virtual void SetUp() {
        options_ = Dumpstate::DumpOptions();
    }
    void TearDown() {
        // Reset the property
        property_set("dumpstate.options", "");
    }
    Dumpstate::DumpOptions options_;
};

TEST_F(DumpOptionsTest, InitializeNone) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("dumpstate")
    };
    // clang-format on

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);

    EXPECT_FALSE(options_.do_add_date);
    EXPECT_FALSE(options_.do_zip_file);
    EXPECT_FALSE(options_.use_socket);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.do_fb);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.do_broadcast);
}

TEST_F(DumpOptionsTest, InitializeAdbBugreport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("dumpstatez"),
        const_cast<char*>("-S"),
        const_cast<char*>("-d"),
        const_cast<char*>("-z"),
    };
    // clang-format on

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.use_control_socket);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.do_fb);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.do_broadcast);
    EXPECT_FALSE(options_.use_socket);
}

TEST_F(DumpOptionsTest, InitializeAdbShellBugreport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("dumpstate"),
        const_cast<char*>("-s"),
    };
    // clang-format on

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.use_socket);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.do_add_date);
    EXPECT_FALSE(options_.do_zip_file);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.do_fb);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.do_broadcast);
}

TEST_F(DumpOptionsTest, InitializeFullBugReport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("bugreport"),
        const_cast<char*>("-d"),
        const_cast<char*>("-p"),
        const_cast<char*>("-B"),
        const_cast<char*>("-z"),
    };
    // clang-format on
    property_set("dumpstate.options", "bugreportfull");

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_TRUE(options_.do_fb);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.do_broadcast);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.use_socket);
    EXPECT_FALSE(options_.do_start_service);
}

TEST_F(DumpOptionsTest, InitializeInteractiveBugReport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("bugreport"),
        const_cast<char*>("-d"),
        const_cast<char*>("-p"),
        const_cast<char*>("-B"),
        const_cast<char*>("-z"),
    };
    // clang-format on

    property_set("dumpstate.options", "bugreportplus");

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_TRUE(options_.do_broadcast);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.do_progress_updates);
    EXPECT_TRUE(options_.do_start_service);
    EXPECT_FALSE(options_.do_fb);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.use_socket);
}

TEST_F(DumpOptionsTest, InitializeRemoteBugReport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("bugreport"),
        const_cast<char*>("-d"),
        const_cast<char*>("-p"),
        const_cast<char*>("-B"),
        const_cast<char*>("-z"),
    };
    // clang-format on

    property_set("dumpstate.options", "bugreportremote");

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_TRUE(options_.do_broadcast);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.is_remote_mode);
    EXPECT_FALSE(options_.do_vibrate);
    EXPECT_FALSE(options_.do_fb);

    // Other options retain default values
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.use_socket);
}

TEST_F(DumpOptionsTest, InitializeWearBugReport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("bugreport"),
        const_cast<char*>("-d"),
        const_cast<char*>("-p"),
        const_cast<char*>("-B"),
        const_cast<char*>("-z"),
    };
    // clang-format on

    property_set("dumpstate.options", "bugreportwear");

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_TRUE(options_.do_fb);
    EXPECT_TRUE(options_.do_broadcast);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.do_progress_updates);
    EXPECT_TRUE(options_.do_start_service);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.use_socket);
}

TEST_F(DumpOptionsTest, InitializeTelephonyBugReport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("bugreport"),
        const_cast<char*>("-d"),
        const_cast<char*>("-p"),
        const_cast<char*>("-B"),
        const_cast<char*>("-z"),
    };
    // clang-format on

    property_set("dumpstate.options", "bugreporttelephony");

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_FALSE(options_.do_fb);
    EXPECT_TRUE(options_.do_broadcast);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.telephony_only);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.use_socket);
}

TEST_F(DumpOptionsTest, InitializeWifiBugReport) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("bugreport"),
        const_cast<char*>("-d"),
        const_cast<char*>("-p"),
        const_cast<char*>("-B"),
        const_cast<char*>("-z"),
    };
    // clang-format on

    property_set("dumpstate.options", "bugreportwifi");

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_FALSE(options_.do_fb);
    EXPECT_TRUE(options_.do_broadcast);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.wifi_only);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.use_socket);
}

TEST_F(DumpOptionsTest, InitializeDefaultBugReport) {
    // default: commandline options are not overridden
    // clang-format off
    char* argv[] = {
        const_cast<char*>("bugreport"),
        const_cast<char*>("-d"),
        const_cast<char*>("-p"),
        const_cast<char*>("-B"),
        const_cast<char*>("-z"),
    };
    // clang-format on

    property_set("dumpstate.options", "");

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_TRUE(options_.do_fb);
    EXPECT_TRUE(options_.do_zip_file);
    EXPECT_TRUE(options_.do_broadcast);

    // Other options retain default values
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.use_control_socket);
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.use_socket);
    EXPECT_FALSE(options_.wifi_only);
}

TEST_F(DumpOptionsTest, InitializePartial1) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("dumpstate"),
        const_cast<char*>("-d"),
        const_cast<char*>("-z"),
        const_cast<char*>("-s"),
        const_cast<char*>("-S"),

    };
    // clang-format on

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.do_add_date);
    EXPECT_TRUE(options_.do_zip_file);
    // TODO: Maybe we should trim the filename
    EXPECT_TRUE(options_.use_socket);
    EXPECT_TRUE(options_.use_control_socket);

    // Other options retain default values
    EXPECT_FALSE(options_.show_header_only);
    EXPECT_TRUE(options_.do_vibrate);
    EXPECT_FALSE(options_.do_fb);
    EXPECT_FALSE(options_.do_progress_updates);
    EXPECT_FALSE(options_.is_remote_mode);
    EXPECT_FALSE(options_.do_broadcast);
}

TEST_F(DumpOptionsTest, InitializePartial2) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("dumpstate"),
        const_cast<char*>("-v"),
        const_cast<char*>("-q"),
        const_cast<char*>("-p"),
        const_cast<char*>("-P"),
        const_cast<char*>("-R"),
        const_cast<char*>("-B"),
    };
    // clang-format on

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    EXPECT_EQ(status, Dumpstate::RunStatus::OK);
    EXPECT_TRUE(options_.show_header_only);
    EXPECT_FALSE(options_.do_vibrate);
    EXPECT_TRUE(options_.do_fb);
    EXPECT_TRUE(options_.do_progress_updates);
    EXPECT_TRUE(options_.is_remote_mode);
    EXPECT_TRUE(options_.do_broadcast);

    // Other options retain default values
    EXPECT_FALSE(options_.do_add_date);
    EXPECT_FALSE(options_.do_zip_file);
    EXPECT_FALSE(options_.use_socket);
    EXPECT_FALSE(options_.use_control_socket);
}

TEST_F(DumpOptionsTest, InitializeHelp) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("dumpstate"),
        const_cast<char*>("-h")
    };
    // clang-format on

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    // -h is for help.
    EXPECT_EQ(status, Dumpstate::RunStatus::HELP);
}

TEST_F(DumpOptionsTest, InitializeUnknown) {
    // clang-format off
    char* argv[] = {
        const_cast<char*>("dumpstate"),
        const_cast<char*>("-u")  // unknown flag
    };
    // clang-format on

    Dumpstate::RunStatus status = options_.Initialize(ARRAY_SIZE(argv), argv);

    // -u is unknown.
    EXPECT_EQ(status, Dumpstate::RunStatus::INVALID_INPUT);
}

TEST_F(DumpOptionsTest, ValidateOptionsNeedOutfile1) {
    options_.do_zip_file = true;
    // Writing to socket = !writing to file.
    options_.use_socket = true;
    EXPECT_FALSE(options_.ValidateOptions());

    options_.use_socket = false;
    EXPECT_TRUE(options_.ValidateOptions());
}

TEST_F(DumpOptionsTest, ValidateOptionsNeedOutfile2) {
    options_.do_broadcast = true;
    // Writing to socket = !writing to file.
    options_.use_socket = true;
    EXPECT_FALSE(options_.ValidateOptions());

    options_.use_socket = false;
    EXPECT_TRUE(options_.ValidateOptions());
}

TEST_F(DumpOptionsTest, ValidateOptionsNeedZipfile) {
    options_.use_control_socket = true;
    EXPECT_FALSE(options_.ValidateOptions());

    options_.do_zip_file = true;
    EXPECT_TRUE(options_.ValidateOptions());
}

TEST_F(DumpOptionsTest, ValidateOptionsUpdateProgressNeedsBroadcast) {
    options_.do_progress_updates = true;
    EXPECT_FALSE(options_.ValidateOptions());

    options_.do_broadcast = true;
    EXPECT_TRUE(options_.ValidateOptions());
}

TEST_F(DumpOptionsTest, ValidateOptionsRemoteMode) {
    options_.is_remote_mode = true;
    EXPECT_FALSE(options_.ValidateOptions());

    options_.do_broadcast = true;
    options_.do_zip_file = true;
    options_.do_add_date = true;
    EXPECT_TRUE(options_.ValidateOptions());
}

class DumpstateTest : public DumpstateBaseTest {
  public:
    void SetUp() {
        DumpstateBaseTest::SetUp();
        SetDryRun(false);
        SetBuildType(android::base::GetProperty("ro.build.type", "(unknown)"));
        ds.progress_.reset(new Progress());
        ds.update_progress_threshold_ = 0;
        ds.options_.reset(new Dumpstate::DumpOptions());
    }

    // Runs a command and capture `stdout` and `stderr`.
    int RunCommand(const std::string& title, const std::vector<std::string>& full_command,
                   const CommandOptions& options = CommandOptions::DEFAULT) {
        CaptureStdout();
        CaptureStderr();
        int status = ds.RunCommand(title, full_command, options);
        out = GetCapturedStdout();
        err = GetCapturedStderr();
        return status;
    }

    // Dumps a file and capture `stdout` and `stderr`.
    int DumpFile(const std::string& title, const std::string& path) {
        CaptureStdout();
        CaptureStderr();
        int status = ds.DumpFile(title, path);
        out = GetCapturedStdout();
        err = GetCapturedStderr();
        return status;
    }

    void SetProgress(long progress, long initial_max, long threshold = 0) {
        ds.options_->do_progress_updates = true;
        ds.update_progress_threshold_ = threshold;
        ds.last_updated_progress_ = 0;
        ds.progress_.reset(new Progress(initial_max, progress, 1.2));
    }

    std::string GetProgressMessage(const std::string& listener_name, int progress, int max,
                                   int old_max = 0, bool update_progress = true) {
        EXPECT_EQ(progress, ds.progress_->Get()) << "invalid progress";
        EXPECT_EQ(max, ds.progress_->GetMax()) << "invalid max";

        bool max_increased = old_max > 0;

        std::string message = "";
        if (max_increased) {
            message =
                android::base::StringPrintf("Adjusting max progress from %d to %d\n", old_max, max);
        }

        if (update_progress) {
            message += android::base::StringPrintf("Setting progress (%s): %d/%d (%d%%)\n",
                                                   listener_name.c_str(), progress, max,
                                                   (100 * progress / max));
        }

        return message;
    }

    // `stdout` and `stderr` from the last command ran.
    std::string out, err;

    Dumpstate& ds = Dumpstate::GetInstance();
};

TEST_F(DumpstateTest, RunCommandNoArgs) {
    EXPECT_EQ(-1, RunCommand("", {}));
}

TEST_F(DumpstateTest, RunCommandNoTitle) {
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}));
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateTest, RunCommandWithTitle) {
    EXPECT_EQ(0, RunCommand("I AM GROOT", {kSimpleCommand}));
    EXPECT_THAT(err, StrEq("stderr\n"));
    // We don't know the exact duration, so we check the prefix and suffix
    EXPECT_THAT(out,
                StartsWith("------ I AM GROOT (" + kSimpleCommand + ") ------\nstdout\n"));
}

TEST_F(DumpstateTest, RunCommandWithLoggingMessage) {
    EXPECT_EQ(
        0, RunCommand("", {kSimpleCommand},
                      CommandOptions::WithTimeout(10).Log("COMMAND, Y U NO LOG FIRST?").Build()));
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("COMMAND, Y U NO LOG FIRST?stderr\n"));
}

TEST_F(DumpstateTest, RunCommandRedirectStderr) {
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand},
                            CommandOptions::WithTimeout(10).RedirectStderr().Build()));
    EXPECT_THAT(out, IsEmpty());
    EXPECT_THAT(err, StrEq("stdout\nstderr\n"));
}

TEST_F(DumpstateTest, RunCommandWithOneArg) {
    EXPECT_EQ(0, RunCommand("", {kEchoCommand, "one"}));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("one\n"));
}

TEST_F(DumpstateTest, RunCommandWithMultipleArgs) {
    EXPECT_EQ(0, RunCommand("", {kEchoCommand, "one", "is", "the", "loniest", "number"}));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("one is the loniest number\n"));
}

TEST_F(DumpstateTest, RunCommandDryRun) {
    SetDryRun(true);
    EXPECT_EQ(0, RunCommand("I AM GROOT", {kSimpleCommand}));
    // We don't know the exact duration, so we check the prefix and suffix
    EXPECT_THAT(out, StartsWith("------ I AM GROOT (" + kSimpleCommand +
                                ") ------\n\t(skipped on dry run)\n"));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateTest, RunCommandDryRunNoTitle) {
    SetDryRun(true);
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}));
    EXPECT_THAT(out, IsEmpty());
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateTest, RunCommandDryRunAlways) {
    SetDryRun(true);
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(10).Always().Build()));
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateTest, RunCommandNotFound) {
    EXPECT_NE(0, RunCommand("", {"/there/cannot/be/such/command"}));
    EXPECT_THAT(out, StartsWith("*** command '/there/cannot/be/such/command' failed: exit code"));
    EXPECT_THAT(err, StartsWith("execvp on command '/there/cannot/be/such/command' failed"));
}

TEST_F(DumpstateTest, RunCommandFails) {
    EXPECT_EQ(42, RunCommand("", {kSimpleCommand, "--exit", "42"}));
    EXPECT_THAT(out, StrEq("stdout\n*** command '" + kSimpleCommand +
                           " --exit 42' failed: exit code 42\n"));
    EXPECT_THAT(err, StrEq("stderr\n*** command '" + kSimpleCommand +
                           " --exit 42' failed: exit code 42\n"));
}

TEST_F(DumpstateTest, RunCommandCrashes) {
    EXPECT_NE(0, RunCommand("", {kSimpleCommand, "--crash"}));
    // We don't know the exit code, so check just the prefix.
    EXPECT_THAT(
        out, StartsWith("stdout\n*** command '" + kSimpleCommand + " --crash' failed: exit code"));
    EXPECT_THAT(
        err, StartsWith("stderr\n*** command '" + kSimpleCommand + " --crash' failed: exit code"));
}

TEST_F(DumpstateTest, RunCommandTimesout) {
    EXPECT_EQ(-1, RunCommand("", {kSimpleCommand, "--sleep", "2"},
                             CommandOptions::WithTimeout(1).Build()));
    EXPECT_THAT(out, StartsWith("stdout line1\n*** command '" + kSimpleCommand +
                                " --sleep 2' timed out after 1"));
    EXPECT_THAT(err, StartsWith("sleeping for 2s\n*** command '" + kSimpleCommand +
                                " --sleep 2' timed out after 1"));
}

TEST_F(DumpstateTest, RunCommandIsKilled) {
    CaptureStdout();
    CaptureStderr();

    std::thread t([=]() {
        EXPECT_EQ(SIGTERM, ds.RunCommand("", {kSimpleCommand, "--pid", "--sleep", "20"},
                                         CommandOptions::WithTimeout(100).Always().Build()));
    });

    // Capture pid and pre-sleep output.
    sleep(1);  // Wait a little bit to make sure pid and 1st line were printed.
    std::string err = GetCapturedStderr();
    EXPECT_THAT(err, StrEq("sleeping for 20s\n"));

    std::string out = GetCapturedStdout();
    std::vector<std::string> lines = android::base::Split(out, "\n");
    ASSERT_EQ(3, (int)lines.size()) << "Invalid lines before sleep: " << out;

    int pid = atoi(lines[0].c_str());
    EXPECT_THAT(lines[1], StrEq("stdout line1"));
    EXPECT_THAT(lines[2], IsEmpty());  // \n

    // Then kill the process.
    CaptureStdout();
    CaptureStderr();
    ASSERT_EQ(0, kill(pid, SIGTERM)) << "failed to kill pid " << pid;
    t.join();

    // Finally, check output after murder.
    out = GetCapturedStdout();
    err = GetCapturedStderr();

    EXPECT_THAT(out, StrEq("*** command '" + kSimpleCommand +
                           " --pid --sleep 20' failed: killed by signal 15\n"));
    EXPECT_THAT(err, StrEq("*** command '" + kSimpleCommand +
                           " --pid --sleep 20' failed: killed by signal 15\n"));
}

TEST_F(DumpstateTest, RunCommandProgress) {
    sp<DumpstateListenerMock> listener(new DumpstateListenerMock());
    ds.listener_ = listener;
    ds.listener_name_ = "FoxMulder";
    SetProgress(0, 30);

    EXPECT_CALL(*listener, onProgressUpdated(20));
    EXPECT_CALL(*listener, onProgress(66));  // 20/30 %
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(20).Build()));
    std::string progress_message = GetProgressMessage(ds.listener_name_, 20, 30);
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n" + progress_message));

    EXPECT_CALL(*listener, onProgressUpdated(30));
    EXPECT_CALL(*listener, onProgress(100));  // 35/35 %
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(10).Build()));
    progress_message = GetProgressMessage(ds.listener_name_, 30, 30);
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n" + progress_message));

    // Run a command that will increase maximum timeout.
    EXPECT_CALL(*listener, onProgressUpdated(31));
    EXPECT_CALL(*listener, onMaxProgressUpdated(37));
    EXPECT_CALL(*listener, onProgress(83));  // 31/37 %
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(1).Build()));
    progress_message = GetProgressMessage(ds.listener_name_, 31, 37, 30);  // 20% increase
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n" + progress_message));

    // Make sure command ran while in dry_run is counted.
    SetDryRun(true);
    EXPECT_CALL(*listener, onProgressUpdated(35));
    EXPECT_CALL(*listener, onProgress(94));  // 35/37 %
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(4).Build()));
    progress_message = GetProgressMessage(ds.listener_name_, 35, 37);
    EXPECT_THAT(out, IsEmpty());
    EXPECT_THAT(err, StrEq(progress_message));

    ds.listener_.clear();
}

TEST_F(DumpstateTest, RunCommandProgressIgnoreThreshold) {
    sp<DumpstateListenerMock> listener(new DumpstateListenerMock());
    ds.listener_ = listener;
    ds.listener_name_ = "FoxMulder";
    SetProgress(0, 8, 5);  // 8 max, 5 threshold

    // First update should always be sent.
    EXPECT_CALL(*listener, onProgressUpdated(1));
    EXPECT_CALL(*listener, onProgress(12));  // 1/12 %
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(1).Build()));
    std::string progress_message = GetProgressMessage(ds.listener_name_, 1, 8);
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n" + progress_message));

    // Fourth update should be ignored because it's between the threshold (5 -1 = 4 < 5).
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(4).Build()));
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));

    // Third update should be sent because it reaches threshold (6 - 1 = 5).
    EXPECT_CALL(*listener, onProgressUpdated(6));
    EXPECT_CALL(*listener, onProgress(75));  // 6/8 %
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(1).Build()));
    progress_message = GetProgressMessage(ds.listener_name_, 6, 8);
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n" + progress_message));

    // Fourth update should be ignored because it's between the threshold (9 - 6 = 3 < 5).
    // But max update should be sent.
    EXPECT_CALL(*listener, onMaxProgressUpdated(10));  // 9 * 120% = 10.8 = 10
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(3).Build()));
    progress_message = GetProgressMessage(ds.listener_name_, 9, 10, 8, false);
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n" + progress_message));

    ds.listener_.clear();
}

TEST_F(DumpstateTest, RunCommandDropRoot) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateTest.RunCommandDropRoot() on test suite\n")
        return;
    }
    // First check root case - only available when running with 'adb root'.
    uid_t uid = getuid();
    if (uid == 0) {
        EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"}));
        EXPECT_THAT(out, StrEq("0\nstdout\n"));
        EXPECT_THAT(err, StrEq("stderr\n"));
        return;
    }
    // Then run dropping root.
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).DropRoot().Build()));
    EXPECT_THAT(out, StrEq("2000\nstdout\n"));
    EXPECT_THAT(err, StrEq("drop_root_user(): already running as Shell\nstderr\n"));
}

TEST_F(DumpstateTest, RunCommandAsRootUserBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateTest.RunCommandAsRootUserBuild() on test suite\n")
        return;
    }
    if (!PropertiesHelper::IsUserBuild()) {
        // Emulates user build if necessarily.
        SetBuildType("user");
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(1).AsRoot().Build()));

    // We don't know the exact path of su, so we just check for the 'root ...' commands
    EXPECT_THAT(out, StartsWith("Skipping"));
    EXPECT_THAT(out, EndsWith("root " + kSimpleCommand + "' on user build.\n"));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateTest, RunCommandAsRootNonUserBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateTest.RunCommandAsRootNonUserBuild() on test suite\n")
        return;
    }
    if (PropertiesHelper::IsUserBuild()) {
        ALOGI("Skipping RunCommandAsRootNonUserBuild on user builds\n");
        return;
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRoot().Build()));

    EXPECT_THAT(out, StrEq("0\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateTest, RunCommandAsRootNonUserBuild_withUnroot) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE(
            "Skipping DumpstateTest.RunCommandAsRootNonUserBuild_withUnroot() "
            "on test suite\n")
        return;
    }
    if (PropertiesHelper::IsUserBuild()) {
        ALOGI("Skipping RunCommandAsRootNonUserBuild_withUnroot on user builds\n");
        return;
    }

    // Same test as above, but with unroot property set, which will override su availability.
    SetUnroot(true);
    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRoot().Build()));

    // AsRoot is ineffective.
    EXPECT_THAT(out, StrEq("2000\nstdout\n"));
    EXPECT_THAT(err, StrEq("drop_root_user(): already running as Shell\nstderr\n"));
}

TEST_F(DumpstateTest, RunCommandAsRootIfAvailableOnUserBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateTest.RunCommandAsRootIfAvailableOnUserBuild() on test suite\n")
        return;
    }
    if (!PropertiesHelper::IsUserBuild()) {
        // Emulates user build if necessarily.
        SetBuildType("user");
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRootIfAvailable().Build()));

    EXPECT_THAT(out, StrEq("2000\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateTest, RunCommandAsRootIfAvailableOnDebugBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateTest.RunCommandAsRootIfAvailableOnDebugBuild() on test suite\n")
        return;
    }
    if (PropertiesHelper::IsUserBuild()) {
        ALOGI("Skipping RunCommandAsRootNonUserBuild on user builds\n");
        return;
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRootIfAvailable().Build()));

    EXPECT_THAT(out, StrEq("0\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateTest, RunCommandAsRootIfAvailableOnDebugBuild_withUnroot) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE(
            "Skipping DumpstateTest.RunCommandAsRootIfAvailableOnDebugBuild_withUnroot() "
            "on test suite\n")
        return;
    }
    if (PropertiesHelper::IsUserBuild()) {
        ALOGI("Skipping RunCommandAsRootIfAvailableOnDebugBuild_withUnroot on user builds\n");
        return;
    }
    // Same test as above, but with unroot property set, which will override su availability.
    SetUnroot(true);

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRootIfAvailable().Build()));

    // It's a userdebug build, so "su root" should be available, but unroot=true overrides it.
    EXPECT_THAT(out, StrEq("2000\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateTest, DumpFileNotFoundNoTitle) {
    EXPECT_EQ(-1, DumpFile("", "/I/cant/believe/I/exist"));
    EXPECT_THAT(out,
                StrEq("*** Error dumping /I/cant/believe/I/exist: No such file or directory\n"));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateTest, DumpFileNotFoundWithTitle) {
    EXPECT_EQ(-1, DumpFile("Y U NO EXIST?", "/I/cant/believe/I/exist"));
    EXPECT_THAT(err, IsEmpty());
    // We don't know the exact duration, so we check the prefix and suffix
    EXPECT_THAT(out, StartsWith("*** Error dumping /I/cant/believe/I/exist (Y U NO EXIST?): No "
                                "such file or directory\n"));
}

TEST_F(DumpstateTest, DumpFileSingleLine) {
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "single-line.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\n"));  // dumpstate adds missing newline
}

TEST_F(DumpstateTest, DumpFileSingleLineWithNewLine) {
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "single-line-with-newline.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\n"));
}

TEST_F(DumpstateTest, DumpFileMultipleLines) {
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "multiple-lines.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\nI AM LINE2\nI AM LINE3\n"));
}

TEST_F(DumpstateTest, DumpFileMultipleLinesWithNewLine) {
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "multiple-lines-with-newline.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\nI AM LINE2\nI AM LINE3\n"));
}

TEST_F(DumpstateTest, DumpFileOnDryRunNoTitle) {
    SetDryRun(true);
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "single-line.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, IsEmpty());
}

TEST_F(DumpstateTest, DumpFileOnDryRun) {
    SetDryRun(true);
    EXPECT_EQ(0, DumpFile("Might as well dump. Dump!", kTestDataPath + "single-line.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(
        out, StartsWith("------ Might as well dump. Dump! (" + kTestDataPath + "single-line.txt:"));
    EXPECT_THAT(out, HasSubstr("\n\t(skipped on dry run)\n"));
}

TEST_F(DumpstateTest, DumpFileUpdateProgress) {
    sp<DumpstateListenerMock> listener(new DumpstateListenerMock());
    ds.listener_ = listener;
    ds.listener_name_ = "FoxMulder";
    SetProgress(0, 30);

    EXPECT_CALL(*listener, onProgressUpdated(5));
    EXPECT_CALL(*listener, onProgress(16));  // 5/30 %
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "single-line.txt"));

    std::string progress_message =
        GetProgressMessage(ds.listener_name_, 5, 30);  // TODO: unhardcode WEIGHT_FILE (5)?
    EXPECT_THAT(err, StrEq(progress_message));
    EXPECT_THAT(out, StrEq("I AM LINE1\n"));  // dumpstate adds missing newline

    ds.listener_.clear();
}

class DumpstateServiceTest : public DumpstateBaseTest {
  public:
    DumpstateService dss;
};

TEST_F(DumpstateServiceTest, SetListenerNoName) {
    sp<DumpstateListenerMock> listener(new DumpstateListenerMock());
    sp<IDumpstateToken> token;
    EXPECT_TRUE(dss.setListener("", listener, /* getSectionDetails = */ false, &token).isOk());
    ASSERT_THAT(token, IsNull());
}

TEST_F(DumpstateServiceTest, SetListenerNoPointer) {
    sp<IDumpstateToken> token;
    EXPECT_TRUE(
        dss.setListener("whatever", nullptr, /* getSectionDetails = */ false, &token).isOk());
    ASSERT_THAT(token, IsNull());
}

TEST_F(DumpstateServiceTest, SetListenerTwice) {
    sp<DumpstateListenerMock> listener(new DumpstateListenerMock());
    sp<IDumpstateToken> token;
    EXPECT_TRUE(
        dss.setListener("whatever", listener, /* getSectionDetails = */ false, &token).isOk());
    ASSERT_THAT(token, NotNull());
    EXPECT_THAT(Dumpstate::GetInstance().listener_name_, StrEq("whatever"));
    EXPECT_FALSE(Dumpstate::GetInstance().report_section_);

    token.clear();
    EXPECT_TRUE(
        dss.setListener("whatsoever", listener, /* getSectionDetails = */ false, &token).isOk());
    ASSERT_THAT(token, IsNull());
    EXPECT_THAT(Dumpstate::GetInstance().listener_name_, StrEq("whatever"));
    EXPECT_FALSE(Dumpstate::GetInstance().report_section_);
}

TEST_F(DumpstateServiceTest, SetListenerWithSectionDetails) {
    sp<DumpstateListenerMock> listener(new DumpstateListenerMock());
    sp<IDumpstateToken> token;
    Dumpstate::GetInstance().listener_ = nullptr;
    EXPECT_TRUE(
        dss.setListener("whatever", listener, /* getSectionDetails = */ true, &token).isOk());
    ASSERT_THAT(token, NotNull());
    EXPECT_THAT(Dumpstate::GetInstance().listener_name_, StrEq("whatever"));
    EXPECT_TRUE(Dumpstate::GetInstance().report_section_);
}

class ProgressTest : public DumpstateBaseTest {
  public:
    Progress GetInstance(int32_t max, double growth_factor, const std::string& path = "") {
        return Progress(max, growth_factor, path);
    }

    void AssertStats(const std::string& path, int32_t expected_runs, int32_t expected_average) {
        std::string expected_content =
            android::base::StringPrintf("%d %d\n", expected_runs, expected_average);
        std::string actual_content;
        ReadFileToString(path, &actual_content);
        ASSERT_THAT(actual_content, StrEq(expected_content)) << "invalid stats on " << path;
    }
};

TEST_F(ProgressTest, SimpleTest) {
    Progress progress;
    EXPECT_EQ(0, progress.Get());
    EXPECT_EQ(Progress::kDefaultMax, progress.GetInitialMax());
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());

    bool max_increased = progress.Inc(1);
    EXPECT_EQ(1, progress.Get());
    EXPECT_EQ(Progress::kDefaultMax, progress.GetInitialMax());
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
    EXPECT_FALSE(max_increased);

    // Ignore negative increase.
    max_increased = progress.Inc(-1);
    EXPECT_EQ(1, progress.Get());
    EXPECT_EQ(Progress::kDefaultMax, progress.GetInitialMax());
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
    EXPECT_FALSE(max_increased);
}

TEST_F(ProgressTest, MaxGrowsInsideNewRange) {
    Progress progress = GetInstance(10, 1.2);  // 20% growth factor
    EXPECT_EQ(0, progress.Get());
    EXPECT_EQ(10, progress.GetInitialMax());
    EXPECT_EQ(10, progress.GetMax());

    // No increase
    bool max_increased = progress.Inc(10);
    EXPECT_EQ(10, progress.Get());
    EXPECT_EQ(10, progress.GetMax());
    EXPECT_FALSE(max_increased);

    // Increase, with new value < max*20%
    max_increased = progress.Inc(1);
    EXPECT_EQ(11, progress.Get());
    EXPECT_EQ(13, progress.GetMax());  // 11 average * 20% growth = 13.2 = 13
    EXPECT_TRUE(max_increased);
}

TEST_F(ProgressTest, MaxGrowsOutsideNewRange) {
    Progress progress = GetInstance(10, 1.2);  // 20% growth factor
    EXPECT_EQ(0, progress.Get());
    EXPECT_EQ(10, progress.GetInitialMax());
    EXPECT_EQ(10, progress.GetMax());

    // No increase
    bool max_increased = progress.Inc(10);
    EXPECT_EQ(10, progress.Get());
    EXPECT_EQ(10, progress.GetMax());
    EXPECT_FALSE(max_increased);

    // Increase, with new value > max*20%
    max_increased = progress.Inc(5);
    EXPECT_EQ(15, progress.Get());
    EXPECT_EQ(18, progress.GetMax());  // 15 average * 20% growth = 18
    EXPECT_TRUE(max_increased);
}

TEST_F(ProgressTest, InvalidPath) {
    Progress progress("/devil/null");
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, EmptyFile) {
    Progress progress(CopyTextFileFixture("empty-file.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, InvalidLine1stEntryNAN) {
    Progress progress(CopyTextFileFixture("stats-invalid-1st-NAN.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, InvalidLine2ndEntryNAN) {
    Progress progress(CopyTextFileFixture("stats-invalid-2nd-NAN.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, InvalidLineBothNAN) {
    Progress progress(CopyTextFileFixture("stats-invalid-both-NAN.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, InvalidLine1stEntryNegative) {
    Progress progress(CopyTextFileFixture("stats-invalid-1st-negative.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, InvalidLine2ndEntryNegative) {
    Progress progress(CopyTextFileFixture("stats-invalid-2nd-negative.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, InvalidLine1stEntryTooBig) {
    Progress progress(CopyTextFileFixture("stats-invalid-1st-too-big.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

TEST_F(ProgressTest, InvalidLine2ndEntryTooBig) {
    Progress progress(CopyTextFileFixture("stats-invalid-2nd-too-big.txt"));
    EXPECT_EQ(Progress::kDefaultMax, progress.GetMax());
}

// Tests stats are properly saved when the file does not exists.
TEST_F(ProgressTest, FirstTime) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it's failing when running as suite
        MYLOGE("Skipping ProgressTest.FirstTime() on test suite\n")
        return;
    }

    std::string path = kTestDataPath + "FirstTime.txt";
    android::base::RemoveFileIfExists(path);

    Progress run1(path);
    EXPECT_EQ(0, run1.Get());
    EXPECT_EQ(Progress::kDefaultMax, run1.GetInitialMax());
    EXPECT_EQ(Progress::kDefaultMax, run1.GetMax());

    bool max_increased = run1.Inc(20);
    EXPECT_EQ(20, run1.Get());
    EXPECT_EQ(Progress::kDefaultMax, run1.GetMax());
    EXPECT_FALSE(max_increased);

    run1.Save();
    AssertStats(path, 1, 20);
}

// Tests what happens when the persistent settings contains the average duration of 1 run.
// Data on file is 1 run and 109 average.
TEST_F(ProgressTest, SecondTime) {
    std::string path = CopyTextFileFixture("stats-one-run-no-newline.txt");

    Progress run1 = GetInstance(-42, 1.2, path);
    EXPECT_EQ(0, run1.Get());
    EXPECT_EQ(10, run1.GetInitialMax());
    EXPECT_EQ(10, run1.GetMax());

    bool max_increased = run1.Inc(20);
    EXPECT_EQ(20, run1.Get());
    EXPECT_EQ(24, run1.GetMax());
    EXPECT_TRUE(max_increased);

    // Average now is 2 runs and (10 + 20)/ 2 = 15
    run1.Save();
    AssertStats(path, 2, 15);

    Progress run2 = GetInstance(-42, 1.2, path);
    EXPECT_EQ(0, run2.Get());
    EXPECT_EQ(15, run2.GetInitialMax());
    EXPECT_EQ(15, run2.GetMax());

    max_increased = run2.Inc(25);
    EXPECT_EQ(25, run2.Get());
    EXPECT_EQ(30, run2.GetMax());
    EXPECT_TRUE(max_increased);

    // Average now is 3 runs and (15 * 2 + 25)/ 3 = 18.33 = 18
    run2.Save();
    AssertStats(path, 3, 18);

    Progress run3 = GetInstance(-42, 1.2, path);
    EXPECT_EQ(0, run3.Get());
    EXPECT_EQ(18, run3.GetInitialMax());
    EXPECT_EQ(18, run3.GetMax());

    // Make sure average decreases as well
    max_increased = run3.Inc(5);
    EXPECT_EQ(5, run3.Get());
    EXPECT_EQ(18, run3.GetMax());
    EXPECT_FALSE(max_increased);

    // Average now is 4 runs and (18 * 3 + 5)/ 4 = 14.75 = 14
    run3.Save();
    AssertStats(path, 4, 14);
}

// Tests what happens when the persistent settings contains the average duration of 2 runs.
// Data on file is 2 runs and 15 average.
TEST_F(ProgressTest, ThirdTime) {
    std::string path = CopyTextFileFixture("stats-two-runs.txt");
    AssertStats(path, 2, 15);  // Sanity check

    Progress run1 = GetInstance(-42, 1.2, path);
    EXPECT_EQ(0, run1.Get());
    EXPECT_EQ(15, run1.GetInitialMax());
    EXPECT_EQ(15, run1.GetMax());

    bool max_increased = run1.Inc(20);
    EXPECT_EQ(20, run1.Get());
    EXPECT_EQ(24, run1.GetMax());
    EXPECT_TRUE(max_increased);

    // Average now is 3 runs and (15 * 2 + 20)/ 3 = 16.66 = 16
    run1.Save();
    AssertStats(path, 3, 16);
}

class DumpstateUtilTest : public DumpstateBaseTest {
  public:
    void SetUp() {
        DumpstateBaseTest::SetUp();
        SetDryRun(false);
    }

    void CaptureFdOut() {
        ReadFileToString(path_, &out);
    }

    void CreateFd(const std::string& name) {
        path_ = kTestDataPath + name;
        MYLOGD("Creating fd for file %s\n", path_.c_str());

        fd = TEMP_FAILURE_RETRY(open(path_.c_str(),
                                     O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_NOFOLLOW,
                                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
        ASSERT_GE(fd, 0) << "could not create FD for path " << path_;
    }

    // Runs a command into the `fd` and capture `stderr`.
    int RunCommand(const std::string& title, const std::vector<std::string>& full_command,
                   const CommandOptions& options = CommandOptions::DEFAULT) {
        CaptureStderr();
        int status = RunCommandToFd(fd, title, full_command, options);
        close(fd);

        CaptureFdOut();
        err = GetCapturedStderr();
        return status;
    }

    // Dumps a file and into the `fd` and `stderr`.
    int DumpFile(const std::string& title, const std::string& path) {
        CaptureStderr();
        int status = DumpFileToFd(fd, title, path);
        close(fd);

        CaptureFdOut();
        err = GetCapturedStderr();
        return status;
    }

    // Find out the pid of the process_name
    int FindPidOfProcess(const std::string& process_name) {
        CaptureStderr();
        int status = GetPidByName(process_name);
        err = GetCapturedStderr();
        return status;
    }

    int fd;

    // 'fd` output and `stderr` from the last command ran.
    std::string out, err;

  private:
    std::string path_;
};

TEST_F(DumpstateUtilTest, RunCommandNoArgs) {
    CreateFd("RunCommandNoArgs.txt");
    EXPECT_EQ(-1, RunCommand("", {}));
}

TEST_F(DumpstateUtilTest, RunCommandNoTitle) {
    CreateFd("RunCommandWithNoArgs.txt");
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}));
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateUtilTest, RunCommandWithTitle) {
    CreateFd("RunCommandWithNoArgs.txt");
    EXPECT_EQ(0, RunCommand("I AM GROOT", {kSimpleCommand}));
    EXPECT_THAT(out, StrEq("------ I AM GROOT (" + kSimpleCommand + ") ------\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateUtilTest, RunCommandWithOneArg) {
    CreateFd("RunCommandWithOneArg.txt");
    EXPECT_EQ(0, RunCommand("", {kEchoCommand, "one"}));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("one\n"));
}

TEST_F(DumpstateUtilTest, RunCommandWithMultipleArgs) {
    CreateFd("RunCommandWithMultipleArgs.txt");
    EXPECT_EQ(0, RunCommand("", {kEchoCommand, "one", "is", "the", "loniest", "number"}));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("one is the loniest number\n"));
}

TEST_F(DumpstateUtilTest, RunCommandWithLoggingMessage) {
    CreateFd("RunCommandWithLoggingMessage.txt");
    EXPECT_EQ(
        0, RunCommand("", {kSimpleCommand},
                      CommandOptions::WithTimeout(10).Log("COMMAND, Y U NO LOG FIRST?").Build()));
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("COMMAND, Y U NO LOG FIRST?stderr\n"));
}

TEST_F(DumpstateUtilTest, RunCommandRedirectStderr) {
    CreateFd("RunCommandRedirectStderr.txt");
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand},
                            CommandOptions::WithTimeout(10).RedirectStderr().Build()));
    EXPECT_THAT(out, IsEmpty());
    EXPECT_THAT(err, StrEq("stdout\nstderr\n"));
}

TEST_F(DumpstateUtilTest, RunCommandDryRun) {
    CreateFd("RunCommandDryRun.txt");
    SetDryRun(true);
    EXPECT_EQ(0, RunCommand("I AM GROOT", {kSimpleCommand}));
    EXPECT_THAT(out, StrEq(android::base::StringPrintf(
                         "------ I AM GROOT (%s) ------\n\t(skipped on dry run)\n",
                         kSimpleCommand.c_str())));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateUtilTest, RunCommandDryRunNoTitle) {
    CreateFd("RunCommandDryRun.txt");
    SetDryRun(true);
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}));
    EXPECT_THAT(
        out, StrEq(android::base::StringPrintf("%s: skipped on dry run\n", kSimpleCommand.c_str())));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateUtilTest, RunCommandDryRunAlways) {
    CreateFd("RunCommandDryRunAlways.txt");
    SetDryRun(true);
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(10).Always().Build()));
    EXPECT_THAT(out, StrEq("stdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateUtilTest, RunCommandNotFound) {
    CreateFd("RunCommandNotFound.txt");
    EXPECT_NE(0, RunCommand("", {"/there/cannot/be/such/command"}));
    EXPECT_THAT(out, StartsWith("*** command '/there/cannot/be/such/command' failed: exit code"));
    EXPECT_THAT(err, StartsWith("execvp on command '/there/cannot/be/such/command' failed"));
}

TEST_F(DumpstateUtilTest, RunCommandFails) {
    CreateFd("RunCommandFails.txt");
    EXPECT_EQ(42, RunCommand("", {kSimpleCommand, "--exit", "42"}));
    EXPECT_THAT(out, StrEq("stdout\n*** command '" + kSimpleCommand +
                           " --exit 42' failed: exit code 42\n"));
    EXPECT_THAT(err, StrEq("stderr\n*** command '" + kSimpleCommand +
                           " --exit 42' failed: exit code 42\n"));
}

TEST_F(DumpstateUtilTest, RunCommandCrashes) {
    CreateFd("RunCommandCrashes.txt");
    EXPECT_NE(0, RunCommand("", {kSimpleCommand, "--crash"}));
    // We don't know the exit code, so check just the prefix.
    EXPECT_THAT(
        out, StartsWith("stdout\n*** command '" + kSimpleCommand + " --crash' failed: exit code"));
    EXPECT_THAT(
        err, StartsWith("stderr\n*** command '" + kSimpleCommand + " --crash' failed: exit code"));
}

TEST_F(DumpstateUtilTest, RunCommandTimesoutWithSec) {
    CreateFd("RunCommandTimesout.txt");
    EXPECT_EQ(-1, RunCommand("", {kSimpleCommand, "--sleep", "2"},
                             CommandOptions::WithTimeout(1).Build()));
    EXPECT_THAT(out, StartsWith("stdout line1\n*** command '" + kSimpleCommand +
                                " --sleep 2' timed out after 1"));
    EXPECT_THAT(err, StartsWith("sleeping for 2s\n*** command '" + kSimpleCommand +
                                " --sleep 2' timed out after 1"));
}

TEST_F(DumpstateUtilTest, RunCommandTimesoutWithMsec) {
    CreateFd("RunCommandTimesout.txt");
    EXPECT_EQ(-1, RunCommand("", {kSimpleCommand, "--sleep", "2"},
                             CommandOptions::WithTimeoutInMs(1000).Build()));
    EXPECT_THAT(out, StartsWith("stdout line1\n*** command '" + kSimpleCommand +
                                " --sleep 2' timed out after 1"));
    EXPECT_THAT(err, StartsWith("sleeping for 2s\n*** command '" + kSimpleCommand +
                                " --sleep 2' timed out after 1"));
}


TEST_F(DumpstateUtilTest, RunCommandIsKilled) {
    CreateFd("RunCommandIsKilled.txt");
    CaptureStderr();

    std::thread t([=]() {
        EXPECT_EQ(SIGTERM, RunCommandToFd(fd, "", {kSimpleCommand, "--pid", "--sleep", "20"},
                                          CommandOptions::WithTimeout(100).Always().Build()));
    });

    // Capture pid and pre-sleep output.
    sleep(1);  // Wait a little bit to make sure pid and 1st line were printed.
    std::string err = GetCapturedStderr();
    EXPECT_THAT(err, StrEq("sleeping for 20s\n"));

    CaptureFdOut();
    std::vector<std::string> lines = android::base::Split(out, "\n");
    ASSERT_EQ(3, (int)lines.size()) << "Invalid lines before sleep: " << out;

    int pid = atoi(lines[0].c_str());
    EXPECT_THAT(lines[1], StrEq("stdout line1"));
    EXPECT_THAT(lines[2], IsEmpty());  // \n

    // Then kill the process.
    CaptureFdOut();
    CaptureStderr();
    ASSERT_EQ(0, kill(pid, SIGTERM)) << "failed to kill pid " << pid;
    t.join();

    // Finally, check output after murder.
    CaptureFdOut();
    err = GetCapturedStderr();

    // out starts with the pid, which is an unknown
    EXPECT_THAT(out, EndsWith("stdout line1\n*** command '" + kSimpleCommand +
                              " --pid --sleep 20' failed: killed by signal 15\n"));
    EXPECT_THAT(err, StrEq("*** command '" + kSimpleCommand +
                           " --pid --sleep 20' failed: killed by signal 15\n"));
}

TEST_F(DumpstateUtilTest, RunCommandAsRootUserBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateUtilTest.RunCommandAsRootUserBuild() on test suite\n")
        return;
    }
    CreateFd("RunCommandAsRootUserBuild.txt");
    if (!PropertiesHelper::IsUserBuild()) {
        // Emulates user build if necessarily.
        SetBuildType("user");
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand}, CommandOptions::WithTimeout(1).AsRoot().Build()));

    // We don't know the exact path of su, so we just check for the 'root ...' commands
    EXPECT_THAT(out, StartsWith("Skipping"));
    EXPECT_THAT(out, EndsWith("root " + kSimpleCommand + "' on user build.\n"));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateUtilTest, RunCommandAsRootNonUserBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateUtilTest.RunCommandAsRootNonUserBuild() on test suite\n")
        return;
    }
    CreateFd("RunCommandAsRootNonUserBuild.txt");
    if (PropertiesHelper::IsUserBuild()) {
        ALOGI("Skipping RunCommandAsRootNonUserBuild on user builds\n");
        return;
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRoot().Build()));

    EXPECT_THAT(out, StrEq("0\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}


TEST_F(DumpstateUtilTest, RunCommandAsRootIfAvailableOnUserBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateUtilTest.RunCommandAsRootIfAvailableOnUserBuild() on test suite\n")
        return;
    }
    CreateFd("RunCommandAsRootIfAvailableOnUserBuild.txt");
    if (!PropertiesHelper::IsUserBuild()) {
        // Emulates user build if necessarily.
        SetBuildType("user");
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRootIfAvailable().Build()));

    EXPECT_THAT(out, StrEq("2000\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateUtilTest, RunCommandAsRootIfAvailableOnDebugBuild) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateUtilTest.RunCommandAsRootIfAvailableOnDebugBuild() on test suite\n")
        return;
    }
    CreateFd("RunCommandAsRootIfAvailableOnDebugBuild.txt");
    if (PropertiesHelper::IsUserBuild()) {
        ALOGI("Skipping RunCommandAsRootNonUserBuild on user builds\n");
        return;
    }

    DropRoot();

    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).AsRootIfAvailable().Build()));

    EXPECT_THAT(out, StrEq("0\nstdout\n"));
    EXPECT_THAT(err, StrEq("stderr\n"));
}

TEST_F(DumpstateUtilTest, RunCommandDropRoot) {
    if (!IsStandalone()) {
        // TODO: temporarily disabled because it might cause other tests to fail after dropping
        // to Shell - need to refactor tests to avoid this problem)
        MYLOGE("Skipping DumpstateUtilTest.RunCommandDropRoot() on test suite\n")
        return;
    }
    CreateFd("RunCommandDropRoot.txt");
    // First check root case - only available when running with 'adb root'.
    uid_t uid = getuid();
    if (uid == 0) {
        EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"}));
        EXPECT_THAT(out, StrEq("0\nstdout\n"));
        EXPECT_THAT(err, StrEq("stderr\n"));
        return;
    }
    // Then run dropping root.
    EXPECT_EQ(0, RunCommand("", {kSimpleCommand, "--uid"},
                            CommandOptions::WithTimeout(1).DropRoot().Build()));
    EXPECT_THAT(out, StrEq("2000\nstdout\n"));
    EXPECT_THAT(err, StrEq("drop_root_user(): already running as Shell\nstderr\n"));
}

TEST_F(DumpstateUtilTest, DumpFileNotFoundNoTitle) {
    CreateFd("DumpFileNotFound.txt");
    EXPECT_EQ(-1, DumpFile("", "/I/cant/believe/I/exist"));
    EXPECT_THAT(out,
                StrEq("*** Error dumping /I/cant/believe/I/exist: No such file or directory\n"));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateUtilTest, DumpFileNotFoundWithTitle) {
    CreateFd("DumpFileNotFound.txt");
    EXPECT_EQ(-1, DumpFile("Y U NO EXIST?", "/I/cant/believe/I/exist"));
    EXPECT_THAT(out, StrEq("*** Error dumping /I/cant/believe/I/exist (Y U NO EXIST?): No such "
                           "file or directory\n"));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateUtilTest, DumpFileSingleLine) {
    CreateFd("DumpFileSingleLine.txt");
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "single-line.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\n"));  // dumpstate adds missing newline
}

TEST_F(DumpstateUtilTest, DumpFileSingleLineWithNewLine) {
    CreateFd("DumpFileSingleLineWithNewLine.txt");
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "single-line-with-newline.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\n"));
}

TEST_F(DumpstateUtilTest, DumpFileMultipleLines) {
    CreateFd("DumpFileMultipleLines.txt");
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "multiple-lines.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\nI AM LINE2\nI AM LINE3\n"));
}

TEST_F(DumpstateUtilTest, DumpFileMultipleLinesWithNewLine) {
    CreateFd("DumpFileMultipleLinesWithNewLine.txt");
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "multiple-lines-with-newline.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq("I AM LINE1\nI AM LINE2\nI AM LINE3\n"));
}

TEST_F(DumpstateUtilTest, DumpFileOnDryRunNoTitle) {
    CreateFd("DumpFileOnDryRun.txt");
    SetDryRun(true);
    std::string path = kTestDataPath + "single-line.txt";
    EXPECT_EQ(0, DumpFile("", kTestDataPath + "single-line.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(out, StrEq(path + ": skipped on dry run\n"));
}

TEST_F(DumpstateUtilTest, DumpFileOnDryRun) {
    CreateFd("DumpFileOnDryRun.txt");
    SetDryRun(true);
    std::string path = kTestDataPath + "single-line.txt";
    EXPECT_EQ(0, DumpFile("Might as well dump. Dump!", kTestDataPath + "single-line.txt"));
    EXPECT_THAT(err, IsEmpty());
    EXPECT_THAT(
        out, StartsWith("------ Might as well dump. Dump! (" + kTestDataPath + "single-line.txt:"));
    EXPECT_THAT(out, EndsWith("skipped on dry run\n"));
}

TEST_F(DumpstateUtilTest, FindingPidWithExistingProcess) {
    // init process always has pid 1.
    EXPECT_EQ(1, FindPidOfProcess("init"));
    EXPECT_THAT(err, IsEmpty());
}

TEST_F(DumpstateUtilTest, FindingPidWithNotExistingProcess) {
    // find the process with abnormal name.
    EXPECT_EQ(-1, FindPidOfProcess("abcdef12345-543"));
    EXPECT_THAT(err, StrEq("can't find the pid\n"));
}

}  // namespace dumpstate
}  // namespace os
}  // namespace android
