//
// Copyright (C) 2019 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <getopt.h>
#include <stdio.h>
#include <sysexits.h>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <android-base/parseint.h>
#include <android-base/properties.h>
#include <android-base/unique_fd.h>
#include <android/gsi/IGsiService.h>
#include <binder/IServiceManager.h>
#include <cutils/android_reboot.h>
#include <libgsi/libgsi.h>

using namespace android::gsi;
using namespace std::chrono_literals;

using android::sp;
using CommandCallback = std::function<int(sp<IGsiService>, int, char**)>;

static int Disable(sp<IGsiService> gsid, int argc, char** argv);
static int Enable(sp<IGsiService> gsid, int argc, char** argv);
static int Install(sp<IGsiService> gsid, int argc, char** argv);
static int Wipe(sp<IGsiService> gsid, int argc, char** argv);
static int WipeData(sp<IGsiService> gsid, int argc, char** argv);
static int Status(sp<IGsiService> gsid, int argc, char** argv);
static int Cancel(sp<IGsiService> gsid, int argc, char** argv);

static const std::map<std::string, CommandCallback> kCommandMap = {
        {"disable", Disable},
        {"enable", Enable},
        {"install", Install},
        {"wipe", Wipe},
        {"wipe-data", WipeData},
        {"status", Status},
        {"cancel", Cancel},
};

static sp<IGsiService> GetGsiService() {
    if (android::base::GetProperty("init.svc.gsid", "") != "running") {
        if (!android::base::SetProperty("ctl.start", "gsid") ||
            !android::base::WaitForProperty("init.svc.gsid", "running", 5s)) {
            std::cerr << "Unable to start gsid\n";
            return nullptr;
        }
    }

    static const int kSleepTimeMs = 50;
    static const int kTotalWaitTimeMs = 3000;
    for (int i = 0; i < kTotalWaitTimeMs / kSleepTimeMs; i++) {
        auto sm = android::defaultServiceManager();
        auto name = android::String16(kGsiServiceName);
        android::sp<android::IBinder> res = sm->checkService(name);
        if (res) {
            return android::interface_cast<IGsiService>(res);
        }
        usleep(kSleepTimeMs * 1000);
    }
    return nullptr;
}

static std::string ErrorMessage(const android::binder::Status& status, int error_code = IGsiService::INSTALL_ERROR_GENERIC) {
    if (!status.isOk()) {
        return status.exceptionMessage().string();
    }
    return "error code " + std::to_string(error_code);
}

class ProgressBar {
  public:
    explicit ProgressBar(sp<IGsiService> gsid) : gsid_(gsid) {}

    ~ProgressBar() { Stop(); }

    void Display() {
        Finish();
        done_ = false;
        last_update_ = {};
        worker_ = std::make_unique<std::thread>([this]() { Worker(); });
    }

    void Stop() {
        if (!worker_) {
            return;
        }
        SignalDone();
        worker_->join();
        worker_ = nullptr;
    }

    void Finish() {
        if (!worker_) {
            return;
        }
        Stop();
        FinishLastBar();
    }

  private:
    void Worker() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!done_) {
            if (!UpdateProgress()) {
                return;
            }
            cv_.wait_for(lock, 500ms, [this] { return done_; });
        }
    }

    bool UpdateProgress() {
        GsiProgress latest;
        auto status = gsid_->getInstallProgress(&latest);
        if (!status.isOk()) {
            std::cout << std::endl;
            return false;
        }
        if (latest.status == IGsiService::STATUS_NO_OPERATION) {
            return true;
        }
        if (last_update_.step != latest.step) {
            FinishLastBar();
        }
        Display(latest);
        return true;
    }

    void FinishLastBar() {
        // If no bar was in progress, don't do anything.
        if (last_update_.total_bytes == 0) {
            return;
        }
        // Ensure we finish the display at 100%.
        last_update_.bytes_processed = last_update_.total_bytes;
        Display(last_update_);
        std::cout << std::endl;
    }

    void Display(const GsiProgress& progress) {
        if (progress.total_bytes == 0) {
            return;
        }

        static constexpr int kColumns = 80;
        static constexpr char kRedColor[] = "\x1b[31m";
        static constexpr char kGreenColor[] = "\x1b[32m";
        static constexpr char kResetColor[] = "\x1b[0m";

        int percentage = (progress.bytes_processed * 100) / progress.total_bytes;
        int64_t bytes_per_col = progress.total_bytes / kColumns;
        uint32_t fill_count = progress.bytes_processed / bytes_per_col;
        uint32_t dash_count = kColumns - fill_count;
        std::string fills = std::string(fill_count, '=');
        std::string dashes = std::string(dash_count, '-');

        // Give the end of the bar some flare.
        if (!fills.empty() && !dashes.empty()) {
            fills[fills.size() - 1] = '>';
        }

        fprintf(stdout, "\r%-15s%6d%% ", progress.step.c_str(), percentage);
        fprintf(stdout, "%s[%s%s%s", kGreenColor, fills.c_str(), kRedColor, dashes.c_str());
        fprintf(stdout, "%s]%s", kGreenColor, kResetColor);
        fflush(stdout);

        last_update_ = progress;
    }

    void SignalDone() {
        std::lock_guard<std::mutex> guard(mutex_);
        done_ = true;
        cv_.notify_all();
    }

  private:
    sp<IGsiService> gsid_;
    std::unique_ptr<std::thread> worker_;
    std::condition_variable cv_;
    std::mutex mutex_;
    GsiProgress last_update_;
    bool done_ = false;
};

static int Install(sp<IGsiService> gsid, int argc, char** argv) {
    struct option options[] = {
            {"install-dir", required_argument, nullptr, 'i'},
            {"gsi-size", required_argument, nullptr, 's'},
            {"no-reboot", no_argument, nullptr, 'n'},
            {"userdata-size", required_argument, nullptr, 'u'},
            {"wipe", no_argument, nullptr, 'w'},
            {nullptr, 0, nullptr, 0},
    };

    GsiInstallParams params;
    params.gsiSize = 0;
    params.userdataSize = 0;
    params.wipeUserdata = false;
    bool reboot = true;

    if (getuid() != 0) {
        std::cerr << "must be root to install a GSI" << std::endl;
        return EX_NOPERM;
    }

    int rv, index;
    while ((rv = getopt_long_only(argc, argv, "", options, &index)) != -1) {
        switch (rv) {
            case 's':
                if (!android::base::ParseInt(optarg, &params.gsiSize) || params.gsiSize <= 0) {
                    std::cerr << "Could not parse image size: " << optarg << std::endl;
                    return EX_USAGE;
                }
                break;
            case 'u':
                if (!android::base::ParseInt(optarg, &params.userdataSize) ||
                    params.userdataSize < 0) {
                    std::cerr << "Could not parse image size: " << optarg << std::endl;
                    return EX_USAGE;
                }
                break;
            case 'i':
                params.installDir = optarg;
                break;
            case 'w':
                params.wipeUserdata = true;
                break;
            case 'n':
                reboot = false;
                break;
        }
    }

    if (params.gsiSize <= 0) {
        std::cerr << "Must specify --gsi-size." << std::endl;
        return EX_USAGE;
    }

    bool running_gsi = false;
    gsid->isGsiRunning(&running_gsi);
    if (running_gsi) {
        std::cerr << "Cannot install a GSI within a live GSI." << std::endl;
        std::cerr << "Use gsi_tool disable or wipe and reboot first." << std::endl;
        return EX_SOFTWARE;
    }

    android::base::unique_fd input(dup(1));
    if (input < 0) {
        std::cerr << "Error duplicating descriptor: " << strerror(errno) << std::endl;
        return EX_SOFTWARE;
    }

    // Note: the progress bar needs to be re-started in between each call.
    ProgressBar progress(gsid);
    progress.Display();

    int error;
    auto status = gsid->beginGsiInstall(params, &error);
    if (!status.isOk() || error != IGsiService::INSTALL_OK) {
        std::cerr << "Could not start live image install: " << ErrorMessage(status, error) << "\n";
        return EX_SOFTWARE;
    }

    android::os::ParcelFileDescriptor stream(std::move(input));

    bool ok = false;
    progress.Display();
    status = gsid->commitGsiChunkFromStream(stream, params.gsiSize, &ok);
    if (!ok) {
        std::cerr << "Could not commit live image data: " << ErrorMessage(status) << "\n";
        return EX_SOFTWARE;
    }

    progress.Finish();

    status = gsid->setGsiBootable(true, &error);
    if (!status.isOk() || error != IGsiService::INSTALL_OK) {
        std::cerr << "Could not make live image bootable: " << ErrorMessage(status, error) << "\n";
        return EX_SOFTWARE;
    }

    if (reboot) {
        if (!android::base::SetProperty(ANDROID_RB_PROPERTY, "reboot,adb")) {
            std::cerr << "Failed to reboot automatically" << std::endl;
            return EX_SOFTWARE;
        }
    } else {
        std::cout << "Please reboot to use the GSI." << std::endl;
    }
    return 0;
}

static int Wipe(sp<IGsiService> gsid, int argc, char** /* argv */) {
    if (argc > 1) {
        std::cerr << "Unrecognized arguments to wipe." << std::endl;
        return EX_USAGE;
    }
    bool ok;
    auto status = gsid->removeGsiInstall(&ok);
    if (!status.isOk() || !ok) {
        std::cerr << "Could not remove GSI install: " << ErrorMessage(status) << "\n";
        return EX_SOFTWARE;
    }

    bool running = false;
    if (gsid->isGsiRunning(&running).isOk() && running) {
        std::cout << "Live image install will be removed next reboot." << std::endl;
    } else {
        std::cout << "Live image install successfully removed." << std::endl;
    }
    return 0;
}

static int WipeData(sp<IGsiService> gsid, int argc, char** /* argv */) {
    if (argc > 1) {
        std::cerr << "Unrecognized arguments to wipe-data.\n";
        return EX_USAGE;
    }

    bool running;
    auto status = gsid->isGsiRunning(&running);
    if (!status.isOk()) {
        std::cerr << "error: " << status.exceptionMessage().string() << std::endl;
        return EX_SOFTWARE;
    }
    if (running) {
        std::cerr << "Cannot wipe GSI userdata while running a GSI.\n";
        return EX_USAGE;
    }

    bool installed;
    status = gsid->isGsiInstalled(&installed);
    if (!status.isOk()) {
        std::cerr << "error: " << status.exceptionMessage().string() << std::endl;
        return EX_SOFTWARE;
    }
    if (!installed) {
        std::cerr << "No GSI is installed.\n";
        return EX_USAGE;
    }

    int error;
    status = gsid->wipeGsiUserdata(&error);
    if (!status.isOk() || error) {
        std::cerr << "Could not wipe GSI userdata: " << ErrorMessage(status, error) << "\n";
        return EX_SOFTWARE;
    }
    return 0;
}

static int Status(sp<IGsiService> gsid, int argc, char** /* argv */) {
    if (argc > 1) {
        std::cerr << "Unrecognized arguments to status." << std::endl;
        return EX_USAGE;
    }
    bool running;
    auto status = gsid->isGsiRunning(&running);
    if (!status.isOk()) {
        std::cerr << "error: " << status.exceptionMessage().string() << std::endl;
        return EX_SOFTWARE;
    } else if (running) {
        std::cout << "running" << std::endl;
    }
    bool installed;
    status = gsid->isGsiInstalled(&installed);
    if (!status.isOk()) {
        std::cerr << "error: " << status.exceptionMessage().string() << std::endl;
        return EX_SOFTWARE;
    } else if (installed) {
        std::cout << "installed" << std::endl;
    }
    bool enabled;
    status = gsid->isGsiEnabled(&enabled);
    if (!status.isOk()) {
        std::cerr << status.exceptionMessage().string() << std::endl;
        return EX_SOFTWARE;
    } else if (running || installed) {
        std::cout << (enabled ? "enabled" : "disabled") << std::endl;
    } else {
        std::cout << "normal" << std::endl;
    }
    return 0;
}

static int Cancel(sp<IGsiService> gsid, int /* argc */, char** /* argv */) {
    bool cancelled = false;
    auto status = gsid->cancelGsiInstall(&cancelled);
    if (!status.isOk()) {
        std::cerr << status.exceptionMessage().string() << std::endl;
        return EX_SOFTWARE;
    }
    if (!cancelled) {
        std::cout << "Fail to cancel the installation." << std::endl;
        return EX_SOFTWARE;
    }
    return 0;
}

static int Enable(sp<IGsiService> gsid, int argc, char** argv) {
    bool one_shot = false;

    struct option options[] = {
            {"single-boot", no_argument, nullptr, 's'},
            {nullptr, 0, nullptr, 0},
    };
    int rv, index;
    while ((rv = getopt_long_only(argc, argv, "", options, &index)) != -1) {
        switch (rv) {
            case 's':
                one_shot = true;
                break;
            default:
                std::cerr << "Unrecognized argument to enable\n";
                return EX_USAGE;
        }
    }

    bool installed = false;
    gsid->isGsiInstalled(&installed);
    if (!installed) {
        std::cerr << "Could not find GSI install to re-enable" << std::endl;
        return EX_SOFTWARE;
    }

    bool installing = false;
    gsid->isGsiInstallInProgress(&installing);
    if (installing) {
        std::cerr << "Cannot enable or disable while an installation is in progress." << std::endl;
        return EX_SOFTWARE;
    }

    int error;
    auto status = gsid->setGsiBootable(one_shot, &error);
    if (!status.isOk() || error != IGsiService::INSTALL_OK) {
        std::cerr << "Error re-enabling GSI: " << ErrorMessage(status, error) << "\n";
        return EX_SOFTWARE;
    }
    std::cout << "Live image install successfully enabled." << std::endl;
    return 0;
}

static int Disable(sp<IGsiService> gsid, int argc, char** /* argv */) {
    if (argc > 1) {
        std::cerr << "Unrecognized arguments to disable." << std::endl;
        return EX_USAGE;
    }

    bool installing = false;
    gsid->isGsiInstallInProgress(&installing);
    if (installing) {
        std::cerr << "Cannot enable or disable while an installation is in progress." << std::endl;
        return EX_SOFTWARE;
    }

    bool ok = false;
    gsid->disableGsiInstall(&ok);
    if (!ok) {
        std::cerr << "Error disabling GSI" << std::endl;
        return EX_SOFTWARE;
    }
    std::cout << "Live image install successfully disabled." << std::endl;
    return 0;
}

static int usage(int /* argc */, char* argv[]) {
    fprintf(stderr,
            "%s - command-line tool for installing GSI images.\n"
            "\n"
            "Usage:\n"
            "  %s <disable|install|wipe|status> [options]\n"
            "\n"
            "  disable      Disable the currently installed GSI.\n"
            "  enable [-s, --single-boot]\n"
            "               Enable a previously disabled GSI.\n"
            "  install      Install a new GSI. Specify the image size with\n"
            "               --gsi-size and the desired userdata size with\n"
            "               --userdata-size (the latter defaults to 8GiB)\n"
            "               --wipe (remove old gsi userdata first)\n"
            "  wipe         Completely remove a GSI and its associated data\n"
            "  wipe-data    Ensure the GSI's userdata will be formatted\n"
            "  cancel       Cancel the installation\n"
            "  status       Show status\n",
            argv[0], argv[0]);
    return EX_USAGE;
}

int main(int argc, char** argv) {
    auto gsid = GetGsiService();
    if (!gsid) {
        std::cerr << "Could not connect to the gsid service." << std::endl;
        return EX_NOPERM;
    }

    if (1 >= argc) {
        std::cerr << "Expected command." << std::endl;
        return EX_USAGE;
    }

    std::string command = argv[1];

    auto iter = kCommandMap.find(command);
    if (iter == kCommandMap.end()) {
        std::cerr << "Unrecognized command: " << command << std::endl;
        return usage(argc, argv);
    }

    int rc = iter->second(gsid, argc - 1, argv + 1);
    return rc;
}
