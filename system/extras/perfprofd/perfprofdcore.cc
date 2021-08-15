/*
**
** Copyright 2015, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <memory>
#include <sstream>
#include <string>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/macros.h>
#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>

#ifdef __BIONIC__
#include <android-base/properties.h>
#endif

#ifdef __ANDROID__
#include <healthhalutils/HealthHalUtils.h>
#endif

#include "perfprofd_record.pb.h"

#include "config.h"
#include "cpuconfig.h"
#include "perf_data_converter.h"
#include "perfprofdcore.h"
#include "perfprofd_io.h"
#include "perfprofd_perf.h"
#include "symbolizer.h"

//
// Perf profiling daemon -- collects system-wide profiles using
//
//       simpleperf record -a
//
// and encodes them so that they can be uploaded by a separate service.
//

//......................................................................

using ProtoUniquePtr = std::unique_ptr<android::perfprofd::PerfprofdRecord>;

//
// Output file from 'perf record'.
//
#define PERF_OUTPUT "perf.data"

//
// This enum holds the results of the "should we profile" configuration check.
//
typedef enum {

  // All systems go for profile collection.
  DO_COLLECT_PROFILE,

  // The selected configuration directory doesn't exist.
  DONT_PROFILE_MISSING_CONFIG_DIR,

  // Destination directory does not contain the semaphore file that
  // the perf profile uploading service creates when it determines
  // that the user has opted "in" for usage data collection. No
  // semaphore -> no user approval -> no profiling.
  DONT_PROFILE_MISSING_SEMAPHORE,

  // No perf executable present
  DONT_PROFILE_MISSING_PERF_EXECUTABLE,

  // We're running in the emulator, perf won't be able to do much
  DONT_PROFILE_RUNNING_IN_EMULATOR

} CKPROFILE_RESULT;

static bool common_initialized = false;

//
// Are we running in the emulator? If so, stub out profile collection
// Starts as uninitialized (-1), then set to 1 or 0 at init time.
//
static int running_in_emulator = -1;

//
// Is this a debug build ('userdebug' or 'eng')?
//
static bool is_debug_build = false;

//
// Random number generator seed (set at startup time).
//
static unsigned short random_seed[3];

//
// Convert a CKPROFILE_RESULT to a string
//
static const char *ckprofile_result_to_string(CKPROFILE_RESULT result)
{
  switch (result) {
    case DO_COLLECT_PROFILE:
      return "DO_COLLECT_PROFILE";
    case DONT_PROFILE_MISSING_CONFIG_DIR:
      return "missing config directory";
    case DONT_PROFILE_MISSING_SEMAPHORE:
      return "missing semaphore file";
    case DONT_PROFILE_MISSING_PERF_EXECUTABLE:
      return "missing 'perf' executable";
    case DONT_PROFILE_RUNNING_IN_EMULATOR:
      return "running in emulator";
    default:
      return "unknown";
  }
}

//
// Check to see whether we should perform a profile collection
//
static CKPROFILE_RESULT check_profiling_enabled(const Config& config)
{
  //
  // Profile collection in the emulator doesn't make sense
  //
  assert(running_in_emulator != -1);
  if (running_in_emulator) {
    return DONT_PROFILE_RUNNING_IN_EMULATOR;
  }

  if (!config.IsProfilingEnabled()) {
    return DONT_PROFILE_MISSING_CONFIG_DIR;
  }

  // Check for existence of simpleperf/perf executable
  std::string pp = config.perf_path;
  if (access(pp.c_str(), R_OK|X_OK) == -1) {
    LOG(WARNING) << "unable to access/execute " << pp;
    return DONT_PROFILE_MISSING_PERF_EXECUTABLE;
  }

  //
  // We are good to go
  //
  return DO_COLLECT_PROFILE;
}

bool get_booting()
{
#ifdef __BIONIC__
  return android::base::GetBoolProperty("sys.boot_completed", false) != true;
#else
  return false;
#endif
}

//
// Constructor takes a timeout (in seconds) and a child pid; If an
// alarm set for the specified number of seconds triggers, then a
// SIGKILL is sent to the child. Destructor resets alarm. Example:
//
//       pid_t child_pid = ...;
//       { AlarmHelper h(10, child_pid);
//         ... = read_from_child(child_pid, ...);
//       }
//
// NB: this helper is not re-entrant-- avoid nested use or
// use by multiple threads
//
class AlarmHelper {
 public:
  AlarmHelper(unsigned num_seconds, pid_t child)
  {
    struct sigaction sigact;
    assert(child);
    assert(child_ == 0);
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_sigaction = handler;
    sigaction(SIGALRM, &sigact, &oldsigact_);
    child_ = child;
    alarm(num_seconds);
  }
  ~AlarmHelper()
  {
    alarm(0);
    child_ = 0;
    sigaction(SIGALRM, &oldsigact_, NULL);
  }
  static void handler(int, siginfo_t *, void *);

 private:
  struct sigaction oldsigact_;
  static pid_t child_;
};

pid_t AlarmHelper::child_;

void AlarmHelper::handler(int, siginfo_t *, void *)
{
  LOG(WARNING) << "SIGALRM timeout";
  kill(child_, SIGKILL);
}

//
// This implementation invokes "dumpsys media.camera" and inspects the
// output to determine if any camera clients are active. NB: this is
// currently disable (via config option) until the selinux issues can
// be sorted out. Another possible implementation (not yet attempted)
// would be to use the binder to call into the native camera service
// via "ICameraService".
//
bool get_camera_active()
{
  int pipefds[2];
  if (pipe2(pipefds, O_CLOEXEC) != 0) {
    PLOG(ERROR) << "pipe2() failed";
    return false;
  }
  pid_t pid = fork();
  if (pid == -1) {
    PLOG(ERROR) << "fork() failed";
    close(pipefds[0]);
    close(pipefds[1]);
    return false;
  } else if (pid == 0) {
    // child
    close(pipefds[0]);
    dup2(pipefds[1], fileno(stderr));
    dup2(pipefds[1], fileno(stdout));
    const char *argv[10];
    unsigned slot = 0;
    argv[slot++] = "/system/bin/dumpsys";
    argv[slot++] = "media.camera";
    argv[slot++] = nullptr;
    execvp(argv[0], (char * const *)argv);
    PLOG(ERROR) << "execvp() failed";
    return false;
  }
  // parent
  AlarmHelper helper(10, pid);
  close(pipefds[1]);

  // read output
  bool have_cam = false;
  bool have_clients = true;
  std::string dump_output;
  bool result = android::base::ReadFdToString(pipefds[0], &dump_output);
  close(pipefds[0]);
  if (result) {
    std::stringstream ss(dump_output);
    std::string line;
    while (std::getline(ss,line,'\n')) {
      if (line.find("Camera module API version:") !=
          std::string::npos) {
        have_cam = true;
      }
      if (line.find("No camera module available") !=
          std::string::npos ||
          line.find("No active camera clients yet") !=
          std::string::npos) {
        have_clients = false;
      }
    }
  }

  // reap child (no zombies please)
  int st = 0;
  TEMP_FAILURE_RETRY(waitpid(pid, &st, 0));
  return have_cam && have_clients;
}

bool get_charging()
{
#ifdef __ANDROID__
  using android::sp;
  using android::hardware::Return;
  using android::hardware::health::V2_0::get_health_service;
  using android::hardware::health::V2_0::HealthInfo;
  using android::hardware::health::V2_0::IHealth;
  using android::hardware::health::V2_0::Result;

  sp<IHealth> service = get_health_service();
  if (service == nullptr) {
    LOG(ERROR) << "Failed to get health HAL";
    return false;
  }
  Result res = Result::UNKNOWN;
  HealthInfo val;
  Return<void> ret =
      service->getHealthInfo([&](Result out_res, HealthInfo out_val) {
        res = out_res;
        val = out_val;
      });
  if (!ret.isOk()) {
    LOG(ERROR) << "Failed to call getChargeStatus on health HAL: " << ret.description();
    return false;
  }
  if (res != Result::SUCCESS) {
    LOG(ERROR) << "Failed to retrieve charge status from health HAL: result = "
               << toString(res);
    return false;
  }
  return val.legacy.chargerAcOnline || val.legacy.chargerUsbOnline ||
         val.legacy.chargerWirelessOnline;
#else
  return false;
#endif
}

static bool postprocess_proc_stat_contents(const std::string &pscontents,
                                           long unsigned *idleticks,
                                           long unsigned *remainingticks)
{
  long unsigned usertime, nicetime, systime, idletime, iowaittime;
  long unsigned irqtime, softirqtime;

  int rc = sscanf(pscontents.c_str(), "cpu  %lu %lu %lu %lu %lu %lu %lu",
                  &usertime, &nicetime, &systime, &idletime,
                  &iowaittime, &irqtime, &softirqtime);
  if (rc != 7) {
    return false;
  }
  *idleticks = idletime;
  *remainingticks = usertime + nicetime + systime + iowaittime + irqtime + softirqtime;
  return true;
}

unsigned collect_cpu_utilization()
{
  std::string contents;
  long unsigned idle[2];
  long unsigned busy[2];
  for (unsigned iter = 0; iter < 2; ++iter) {
    if (!android::base::ReadFileToString("/proc/stat", &contents)) {
      return 0;
    }
    if (!postprocess_proc_stat_contents(contents, &idle[iter], &busy[iter])) {
      return 0;
    }
    if (iter == 0) {
      sleep(1);
    }
  }
  long unsigned total_delta = (idle[1] + busy[1]) - (idle[0] + busy[0]);
  long unsigned busy_delta = busy[1] - busy[0];
  return busy_delta * 100 / total_delta;
}

static void annotate_encoded_perf_profile(android::perfprofd::PerfprofdRecord* profile,
                                          const Config& config,
                                          unsigned cpu_utilization)
{
  //
  // Incorporate cpu utilization (collected prior to perf run)
  //
  if (config.collect_cpu_utilization) {
    profile->SetExtension(quipper::cpu_utilization, cpu_utilization);
  }

  //
  // Load average as reported by the kernel
  //
  std::string load;
  double fload = 0.0;
  if (android::base::ReadFileToString("/proc/loadavg", &load) &&
      sscanf(load.c_str(), "%lf", &fload) == 1) {
    int iload = static_cast<int>(fload * 100.0);
    profile->SetExtension(quipper::sys_load_average, iload);
  } else {
    PLOG(ERROR) << "Failed to read or scan /proc/loadavg";
  }

  //
  // Device still booting? Camera in use? Plugged into charger?
  //
  bool is_booting = get_booting();
  if (config.collect_booting) {
    profile->SetExtension(quipper::booting, is_booting);
  }
  if (config.collect_camera_active) {
    profile->SetExtension(quipper::camera_active, is_booting ? false : get_camera_active());
  }
  if (config.collect_charging_state) {
    profile->SetExtension(quipper::on_charger, get_charging());
  }

  //
  // Examine the contents of wake_unlock to determine whether the
  // device display is on or off. NB: is this really the only way to
  // determine this info?
  //
  std::string disp;
  if (android::base::ReadFileToString("/sys/power/wake_unlock", &disp)) {
    bool ison = (strstr(disp.c_str(), "PowerManagerService.Display") == 0);
    profile->SetExtension(quipper::display_on, ison);
  } else {
    PLOG(ERROR) << "Failed to read /sys/power/wake_unlock";
  }
}

static ProtoUniquePtr encode_to_proto(const std::string &data_file_path,
                                      const Config& config,
                                      unsigned cpu_utilization,
                                      perfprofd::Symbolizer* symbolizer) {
  //
  // Open and read perf.data file
  //
  ProtoUniquePtr encodedProfile(
      android::perfprofd::RawPerfDataToAndroidPerfProfile(data_file_path,
                                                          symbolizer,
                                                          config.symbolize_everything));
  if (encodedProfile == nullptr) {
    return nullptr;
  }

  // All of the info in 'encodedProfile' is derived from the perf.data file;
  // here we tack display status, cpu utilization, system load, etc.
  annotate_encoded_perf_profile(encodedProfile.get(), config, cpu_utilization);

  return encodedProfile;
}

PROFILE_RESULT encode_to_proto(const std::string &data_file_path,
                               const char *encoded_file_path,
                               const Config& config,
                               unsigned cpu_utilization,
                               perfprofd::Symbolizer* symbolizer)
{
  ProtoUniquePtr encodedProfile = encode_to_proto(data_file_path,
                                                  config,
                                                  cpu_utilization,
                                                  symbolizer);

  //
  // Issue error if no samples
  //
  if (encodedProfile == nullptr || encodedProfile->events_size() == 0) {
    return ERR_PERF_ENCODE_FAILED;
  }

  return android::perfprofd::SerializeProtobuf(encodedProfile.get(),
                                               encoded_file_path,
                                               config.compress)
      ? OK_PROFILE_COLLECTION
      : ERR_WRITE_ENCODED_FILE_FAILED;
}

//
// Remove all files in the destination directory during initialization
//
static void cleanup_destination_dir(const std::string& dest_dir)
{
  DIR* dir = opendir(dest_dir.c_str());
  if (dir != NULL) {
    struct dirent* e;
    while ((e = readdir(dir)) != 0) {
      if (e->d_name[0] != '.') {
        std::string file_path = dest_dir + "/" + e->d_name;
        remove(file_path.c_str());
      }
    }
    closedir(dir);
  } else {
    PLOG(WARNING) << "unable to open destination dir " << dest_dir << " for cleanup";
  }
}

//
// Collect a perf profile. Steps for this operation are:
// - kick off 'perf record'
// - read perf.data, convert to protocol buf
//
static ProtoUniquePtr collect_profile(Config& config)
{
  //
  // Collect cpu utilization if enabled
  //
  unsigned cpu_utilization = 0;
  if (config.collect_cpu_utilization) {
    cpu_utilization = collect_cpu_utilization();
  }

  //
  // Form perf.data file name, perf error output file name
  //
  const std::string& destdir = config.destination_directory;
  std::string data_file_path(destdir);
  data_file_path += "/";
  data_file_path += PERF_OUTPUT;
  std::string perf_stderr_path(destdir);
  perf_stderr_path += "/perferr.txt";

  //
  // Remove any existing perf.data file -- if we don't do this, perf
  // will rename the old file and we'll have extra cruft lying around.
  //
  struct stat statb;
  if (stat(data_file_path.c_str(), &statb) == 0) { // if file exists...
    if (unlink(data_file_path.c_str())) {          // then try to remove
      PLOG(WARNING) << "unable to unlink previous perf.data file";
    }
  }

  //
  // The "mpdecision" daemon can cause problems for profile
  // collection: if it decides to online a CPU partway through the
  // 'perf record' run, the activity on that CPU will be invisible to
  // perf, and if it offlines a CPU during the recording this can
  // sometimes leave the PMU in an unusable state (dmesg errors of the
  // form "perfevents: unable to request IRQXXX for ...").  To avoid
  // these issues, if "mpdecision" is running the helper below will
  // stop the service and then online all available CPUs. The object
  // destructor (invoked when this routine terminates) will then
  // restart the service again when needed.
  //
  uint32_t duration = config.sample_duration_in_s;
  bool hardwire = config.hardwire_cpus;
  uint32_t max_duration = config.hardwire_cpus_max_duration_in_s;
  bool take_action = (hardwire && duration <= max_duration);
  HardwireCpuHelper helper(take_action);

  auto scope_guard = android::base::make_scope_guard(
      [&data_file_path]() { unlink(data_file_path.c_str()); });

  //
  // Invoke perf
  //
  const char *stack_profile_opt =
      (config.stack_profile ? "-g" : nullptr);
  const std::string& perf_path = config.perf_path;

  android::perfprofd::PerfResult invoke_res =
      android::perfprofd::InvokePerf(config,
                                     perf_path,
                                     stack_profile_opt,
                                     duration,
                                     data_file_path,
                                     perf_stderr_path);
  if (invoke_res != android::perfprofd::PerfResult::kOK) {
    return nullptr;
  }

  //
  // Read the resulting perf.data file, encode into protocol buffer, then write
  // the result to the file perf.data.encoded
  //
  std::unique_ptr<perfprofd::Symbolizer> symbolizer;
  if (config.use_elf_symbolizer) {
    symbolizer = perfprofd::CreateELFSymbolizer();
  }
  return encode_to_proto(data_file_path, config, cpu_utilization, symbolizer.get());
}

//
// Assuming that we want to collect a profile every N seconds,
// randomly partition N into two sub-intervals.
//
static void determine_before_after(unsigned &sleep_before_collect,
                                   unsigned &sleep_after_collect,
                                   unsigned collection_interval)
{
  double frac = erand48(random_seed);
  sleep_before_collect = (unsigned) (((double)collection_interval) * frac);
  assert(sleep_before_collect <= collection_interval);
  sleep_after_collect = collection_interval - sleep_before_collect;
}

//
// Set random number generator seed
//
static void set_seed(uint32_t use_fixed_seed)
{
  unsigned seed = 0;
  if (use_fixed_seed) {
    //
    // Use fixed user-specified seed
    //
    seed = use_fixed_seed;
  } else {
    //
    // Randomized seed
    //
#ifdef __BIONIC__
    seed = arc4random();
#else
    seed = 12345678u;
#endif
  }
  LOG(INFO) << "random seed set to " << seed;
  // Distribute the 32-bit seed into the three 16-bit array
  // elements. The specific values being written do not especially
  // matter as long as we are setting them to something based on the seed.
  random_seed[0] = seed & 0xffff;
  random_seed[1] = (seed >> 16);
  random_seed[2] = (random_seed[0] ^ random_seed[1]);
}

void CommonInit(uint32_t use_fixed_seed, const char* dest_dir) {
  // Children of init inherit an artificially low OOM score -- this is not
  // desirable for perfprofd (its OOM score should be on par with
  // other user processes).
  std::stringstream oomscore_path;
  oomscore_path << "/proc/" << getpid() << "/oom_score_adj";
  if (!android::base::WriteStringToFile("0", oomscore_path.str())) {
    LOG(ERROR) << "unable to write to " << oomscore_path.str();
  }

  set_seed(use_fixed_seed);
  if (dest_dir != nullptr) {
    cleanup_destination_dir(dest_dir);
  }

#ifdef __BIONIC__
  running_in_emulator = android::base::GetBoolProperty("ro.kernel.qemu", false);
  is_debug_build = android::base::GetBoolProperty("ro.debuggable", false);
#else
  running_in_emulator = false;
  is_debug_build = true;
#endif

  common_initialized = true;
}

void GlobalInit(const std::string& perf_path) {
  if (!android::perfprofd::FindSupportedPerfCounters(perf_path)) {
    LOG(WARNING) << "Could not read supported perf counters.";
  }
}

bool IsDebugBuild() {
  CHECK(common_initialized);
  return is_debug_build;
}

template <typename ConfigFn, typename UpdateFn>
static void ProfilingLoopImpl(ConfigFn config, UpdateFn update, HandlerFn handler) {
  unsigned iterations = 0;
  while(config()->main_loop_iterations == 0 ||
      iterations < config()->main_loop_iterations) {
    if (config()->ShouldStopProfiling()) {
      return;
    }

    // Figure out where in the collection interval we're going to actually
    // run perf
    unsigned sleep_before_collect = 0;
    unsigned sleep_after_collect = 0;
    determine_before_after(sleep_before_collect,
                           sleep_after_collect,
                           config()->collection_interval_in_s);
    if (sleep_before_collect > 0) {
      config()->Sleep(sleep_before_collect);
    }

    if (config()->ShouldStopProfiling()) {
      return;
    }

    // Run any necessary updates.
    update();

    // Check for profiling enabled...
    CKPROFILE_RESULT ckresult = check_profiling_enabled(*config());
    if (ckresult != DO_COLLECT_PROFILE) {
      LOG(INFO) << "profile collection skipped (" << ckprofile_result_to_string(ckresult) << ")";
    } else {
      // Kick off the profiling run...
      LOG(INFO) << "initiating profile collection";
      ProtoUniquePtr proto = collect_profile(*config());
      if (proto == nullptr) {
        LOG(WARNING) << "profile collection failed";
      }

      // Always report, even a null result.
      bool handle_result = handler(proto.get(), config());
      if (handle_result) {
        LOG(INFO) << "profile collection complete";
      } else if (proto != nullptr) {
        LOG(WARNING) << "profile handling failed";
      }
    }

    if (config()->ShouldStopProfiling()) {
      return;
    }

    if (sleep_after_collect > 0) {
      config()->Sleep(sleep_after_collect);
    }
    iterations += 1;
  }
}

void ProfilingLoop(Config& config, HandlerFn handler) {
  CommonInit(config.use_fixed_seed, nullptr);

  auto config_fn = [&config]() {
    return &config;;
  };
  auto do_nothing = []() {
  };
  ProfilingLoopImpl(config_fn, do_nothing, handler);
}

void ProfilingLoop(std::function<Config*()> config_fn,
                   std::function<void()> update_fn,
                   HandlerFn handler) {
  ProfilingLoopImpl(config_fn, update_fn, handler);
}
