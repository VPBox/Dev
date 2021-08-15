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
#include <libgen.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <android-base/logging.h>
#include <android-base/file.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <android-base/unique_fd.h>
#if defined(__ANDROID__)
#include <android-base/properties.h>
#endif

#include "CallChainJoiner.h"
#include "command.h"
#include "environment.h"
#include "event_selection_set.h"
#include "event_type.h"
#include "IOEventLoop.h"
#include "JITDebugReader.h"
#include "OfflineUnwinder.h"
#include "read_apk.h"
#include "read_elf.h"
#include "record.h"
#include "record_file.h"
#include "thread_tree.h"
#include "tracing.h"
#include "utils.h"
#include "workload.h"

using namespace simpleperf;

static std::string default_measured_event_type = "cpu-cycles";

static std::unordered_map<std::string, uint64_t> branch_sampling_type_map = {
    {"u", PERF_SAMPLE_BRANCH_USER},
    {"k", PERF_SAMPLE_BRANCH_KERNEL},
    {"any", PERF_SAMPLE_BRANCH_ANY},
    {"any_call", PERF_SAMPLE_BRANCH_ANY_CALL},
    {"any_ret", PERF_SAMPLE_BRANCH_ANY_RETURN},
    {"ind_call", PERF_SAMPLE_BRANCH_IND_CALL},
};

static std::unordered_map<std::string, int> clockid_map = {
    {"realtime", CLOCK_REALTIME},
    {"monotonic", CLOCK_MONOTONIC},
    {"monotonic_raw", CLOCK_MONOTONIC_RAW},
    {"boottime", CLOCK_BOOTTIME},
};

// The max size of records dumped by kernel is 65535, and dump stack size
// should be a multiply of 8, so MAX_DUMP_STACK_SIZE is 65528.
constexpr uint32_t MAX_DUMP_STACK_SIZE = 65528;

// The max allowed pages in mapped buffer is decided by rlimit(RLIMIT_MEMLOCK).
// Here 1024 is a desired value for pages in mapped buffer. If mapped
// successfully, the buffer size = 1024 * 4K (page size) = 4M.
constexpr size_t DESIRED_PAGES_IN_MAPPED_BUFFER = 1024;

// Cache size used by CallChainJoiner to cache call chains in memory.
constexpr size_t DEFAULT_CALL_CHAIN_JOINER_CACHE_SIZE = 8 * 1024 * 1024;

// Currently, the record buffer size in user-space is set to match the kernel buffer size on a
// 8 core system. For system-wide recording, it is 8K pages * 4K page_size * 8 cores = 256MB.
// For non system-wide recording, it is 1K pages * 4K page_size * 8 cores = 64MB.
static constexpr size_t kRecordBufferSize = 64 * 1024 * 1024;
static constexpr size_t kSystemWideRecordBufferSize = 256 * 1024 * 1024;

struct TimeStat {
  uint64_t prepare_recording_time = 0;
  uint64_t start_recording_time = 0;
  uint64_t stop_recording_time = 0;
  uint64_t finish_recording_time = 0;
  uint64_t post_process_time = 0;
};

class RecordCommand : public Command {
 public:
  RecordCommand()
      : Command(
            "record", "record sampling info in perf.data",
            // clang-format off
"Usage: simpleperf record [options] [--] [command [command-args]]\n"
"       Gather sampling information of running [command]. And -a/-p/-t option\n"
"       can be used to change target of sampling information.\n"
"       The default options are: -e cpu-cycles -f 4000 -o perf.data.\n"
"Select monitored threads:\n"
"-a     System-wide collection.\n"
#if defined(__ANDROID__)
"--app package_name    Profile the process of an Android application.\n"
"                      On non-rooted devices, the app must be debuggable,\n"
"                      because we use run-as to switch to the app's context.\n"
#endif
"-p pid1,pid2,...       Record events on existing processes. Mutually exclusive\n"
"                       with -a.\n"
"-t tid1,tid2,... Record events on existing threads. Mutually exclusive with -a.\n"
"\n"
"Select monitored event types:\n"
"-e event1[:modifier1],event2[:modifier2],...\n"
"             Select a list of events to record. An event can be:\n"
"               1) an event name listed in `simpleperf list`;\n"
"               2) a raw PMU event in rN format. N is a hex number.\n"
"                  For example, r1b selects event number 0x1b.\n"
"             Modifiers can be added to define how the event should be\n"
"             monitored. Possible modifiers are:\n"
"                u - monitor user space events only\n"
"                k - monitor kernel space events only\n"
"--group event1[:modifier],event2[:modifier2],...\n"
"             Similar to -e option. But events specified in the same --group\n"
"             option are monitored as a group, and scheduled in and out at the\n"
"             same time.\n"
"--trace-offcpu   Generate samples when threads are scheduled off cpu.\n"
"                 Similar to \"-c 1 -e sched:sched_switch\".\n"
"\n"
"Select monitoring options:\n"
"-f freq      Set event sample frequency. It means recording at most [freq]\n"
"             samples every second. For non-tracepoint events, the default\n"
"             option is -f 4000. A -f/-c option affects all event types\n"
"             following it until meeting another -f/-c option. For example,\n"
"             for \"-f 1000 cpu-cycles -c 1 -e sched:sched_switch\", cpu-cycles\n"
"             has sample freq 1000, sched:sched_switch event has sample period 1.\n"
"-c count     Set event sample period. It means recording one sample when\n"
"             [count] events happen. For tracepoint events, the default option\n"
"             is -c 1.\n"
"--call-graph fp | dwarf[,<dump_stack_size>]\n"
"             Enable call graph recording. Use frame pointer or dwarf debug\n"
"             frame as the method to parse call graph in stack.\n"
"             Default is dwarf,65528.\n"
"-g           Same as '--call-graph dwarf'.\n"
"--clockid clock_id      Generate timestamps of samples using selected clock.\n"
"                        Possible values are: realtime, monotonic,\n"
"                        monotonic_raw, boottime, perf. If supported, default\n"
"                        is monotonic, otherwise is perf.\n"
"--cpu cpu_item1,cpu_item2,...\n"
"             Collect samples only on the selected cpus. cpu_item can be cpu\n"
"             number like 1, or cpu range like 0-3.\n"
"--duration time_in_sec  Monitor for time_in_sec seconds instead of running\n"
"                        [command]. Here time_in_sec may be any positive\n"
"                        floating point number.\n"
"-j branch_filter1,branch_filter2,...\n"
"             Enable taken branch stack sampling. Each sample captures a series\n"
"             of consecutive taken branches.\n"
"             The following filters are defined:\n"
"                any: any type of branch\n"
"                any_call: any function call or system call\n"
"                any_ret: any function return or system call return\n"
"                ind_call: any indirect branch\n"
"                u: only when the branch target is at the user level\n"
"                k: only when the branch target is in the kernel\n"
"             This option requires at least one branch type among any, any_call,\n"
"             any_ret, ind_call.\n"
"-b           Enable taken branch stack sampling. Same as '-j any'.\n"
"-m mmap_pages   Set the size of the buffer used to receiving sample data from\n"
"                the kernel. It should be a power of 2. If not set, the max\n"
"                possible value <= 1024 will be used.\n"
"--no-inherit  Don't record created child threads/processes.\n"
"--cpu-percent <percent>  Set the max percent of cpu time used for recording.\n"
"                         percent is in range [1-100], default is 25.\n"
"\n"
"Dwarf unwinding options:\n"
"--post-unwind=(yes|no) If `--call-graph dwarf` option is used, then the user's\n"
"                       stack will be recorded in perf.data and unwound while\n"
"                       recording by default. Use --post-unwind=yes to switch\n"
"                       to unwind after recording.\n"
"--no-unwind   If `--call-graph dwarf` option is used, then the user's stack\n"
"              will be unwound by default. Use this option to disable the\n"
"              unwinding of the user's stack.\n"
"--no-callchain-joiner  If `--call-graph dwarf` option is used, then by default\n"
"                       callchain joiner is used to break the 64k stack limit\n"
"                       and build more complete call graphs. However, the built\n"
"                       call graphs may not be correct in all cases.\n"
"--callchain-joiner-min-matching-nodes count\n"
"               When callchain joiner is used, set the matched nodes needed to join\n"
"               callchains. The count should be >= 1. By default it is 1.\n"
"\n"
"Recording file options:\n"
"--no-dump-kernel-symbols  Don't dump kernel symbols in perf.data. By default\n"
"                          kernel symbols will be dumped when needed.\n"
"--no-dump-symbols       Don't dump symbols in perf.data. By default symbols are\n"
"                        dumped in perf.data, to support reporting in another\n"
"                        environment.\n"
"-o record_file_name    Set record file name, default is perf.data.\n"
"--size-limit SIZE[K|M|G]      Stop recording after SIZE bytes of records.\n"
"                              Default is unlimited.\n"
"--symfs <dir>    Look for files with symbols relative to this directory.\n"
"                 This option is used to provide files with symbol table and\n"
"                 debug information, which are used for unwinding and dumping symbols.\n"
"\n"
"Other options:\n"
"--exit-with-parent            Stop recording when the process starting\n"
"                              simpleperf dies.\n"
"--start_profiling_fd fd_no    After starting profiling, write \"STARTED\" to\n"
"                              <fd_no>, then close <fd_no>.\n"
"--stdio-controls-profiling    Use stdin/stdout to pause/resume profiling.\n"
#if defined(__ANDROID__)
"--in-app                      We are already running in the app's context.\n"
"--tracepoint-events file_name   Read tracepoint events from [file_name] instead of tracefs.\n"
#endif
#if 0
// Below options are only used internally and shouldn't be visible to the public.
"--out-fd <fd>    Write perf.data to a file descriptor.\n"
"--stop-signal-fd <fd>  Stop recording when fd is readable.\n"
#endif
            // clang-format on
            ),
        system_wide_collection_(false),
        branch_sampling_(0),
        fp_callchain_sampling_(false),
        dwarf_callchain_sampling_(false),
        dump_stack_size_in_dwarf_sampling_(MAX_DUMP_STACK_SIZE),
        unwind_dwarf_callchain_(true),
        post_unwind_(false),
        child_inherit_(true),
        duration_in_sec_(0),
        can_dump_kernel_symbols_(true),
        dump_symbols_(true),
        event_selection_set_(false),
        mmap_page_range_(std::make_pair(1, DESIRED_PAGES_IN_MAPPED_BUFFER)),
        record_filename_("perf.data"),
        sample_record_count_(0),
        lost_record_count_(0),
        in_app_context_(false),
        trace_offcpu_(false),
        exclude_kernel_callchain_(false),
        allow_callchain_joiner_(true),
        callchain_joiner_min_matching_nodes_(1u),
        last_record_timestamp_(0u) {
    // If we run `adb shell simpleperf record xxx` and stop profiling by ctrl-c, adb closes
    // sockets connecting simpleperf. After that, simpleperf will receive SIGPIPE when writing
    // to stdout/stderr, which is a problem when we use '--app' option. So ignore SIGPIPE to
    // finish properly.
    signal(SIGPIPE, SIG_IGN);
  }

  bool Run(const std::vector<std::string>& args);

 private:
  bool ParseOptions(const std::vector<std::string>& args,
                    std::vector<std::string>* non_option_args);
  bool AdjustPerfEventLimit();
  bool PrepareRecording(Workload* workload);
  bool DoRecording(Workload* workload);
  bool PostProcessRecording(const std::vector<std::string>& args);
  bool TraceOffCpu();
  bool SetEventSelectionFlags();
  bool CreateAndInitRecordFile();
  std::unique_ptr<RecordFileWriter> CreateRecordFile(
      const std::string& filename);
  bool DumpKernelSymbol();
  bool DumpTracingData();
  bool DumpKernelMaps();
  bool DumpUserSpaceMaps();
  bool DumpProcessMaps(pid_t pid, const std::unordered_set<pid_t>& tids);
  bool ProcessRecord(Record* record);
  bool ShouldOmitRecord(Record* record);
  bool DumpMapsForRecord(Record* record);
  bool SaveRecordForPostUnwinding(Record* record);
  bool SaveRecordAfterUnwinding(Record* record);
  bool SaveRecordWithoutUnwinding(Record* record);
  bool ProcessJITDebugInfo(const std::vector<JITDebugInfo>& debug_info, bool sync_kernel_records);
  bool ProcessControlCmd(IOEventLoop* loop);

  void UpdateRecord(Record* record);
  bool UnwindRecord(SampleRecord& r);
  bool PostUnwindRecords();
  bool JoinCallChains();
  bool DumpAdditionalFeatures(const std::vector<std::string>& args);
  bool DumpBuildIdFeature();
  bool DumpFileFeature();
  bool DumpMetaInfoFeature(bool kernel_symbols_available);
  void CollectHitFileInfo(const SampleRecord& r);

  std::unique_ptr<SampleSpeed> sample_speed_;
  bool system_wide_collection_;
  uint64_t branch_sampling_;
  bool fp_callchain_sampling_;
  bool dwarf_callchain_sampling_;
  uint32_t dump_stack_size_in_dwarf_sampling_;
  bool unwind_dwarf_callchain_;
  bool post_unwind_;
  std::unique_ptr<OfflineUnwinder> offline_unwinder_;
  bool child_inherit_;
  double duration_in_sec_;
  bool can_dump_kernel_symbols_;
  bool dump_symbols_;
  std::string clockid_;
  std::vector<int> cpus_;
  EventSelectionSet event_selection_set_;

  std::pair<size_t, size_t> mmap_page_range_;

  ThreadTree thread_tree_;
  std::string record_filename_;
  android::base::unique_fd out_fd_;
  std::unique_ptr<RecordFileWriter> record_file_writer_;
  android::base::unique_fd stop_signal_fd_;

  uint64_t sample_record_count_;
  uint64_t lost_record_count_;
  android::base::unique_fd start_profiling_fd_;
  bool stdio_controls_profiling_ = false;

  std::string app_package_name_;
  bool in_app_context_;
  bool trace_offcpu_;
  bool exclude_kernel_callchain_;
  uint64_t size_limit_in_bytes_ = 0;
  uint64_t max_sample_freq_ = DEFAULT_SAMPLE_FREQ_FOR_NONTRACEPOINT_EVENT;
  size_t cpu_time_max_percent_ = 25;

  // For CallChainJoiner
  bool allow_callchain_joiner_;
  size_t callchain_joiner_min_matching_nodes_;
  std::unique_ptr<CallChainJoiner> callchain_joiner_;

  std::unique_ptr<JITDebugReader> jit_debug_reader_;
  uint64_t last_record_timestamp_;  // used to insert Mmap2Records for JIT debug info
  TimeStat time_stat_;
  EventAttrWithId dumping_attr_id_;
  // In system wide recording, record if we have dumped map info for a process.
  std::unordered_set<pid_t> dumped_processes_;
};

bool RecordCommand::Run(const std::vector<std::string>& args) {
  ScopedCurrentArch scoped_arch(GetMachineArch());
  if (!CheckPerfEventLimit()) {
    return false;
  }
  AllowMoreOpenedFiles();

  std::vector<std::string> workload_args;
  if (!ParseOptions(args, &workload_args)) {
    return false;
  }
  if (!AdjustPerfEventLimit()) {
    return false;
  }
  ScopedTempFiles scoped_temp_files(android::base::Dirname(record_filename_));
  if (!app_package_name_.empty() && !in_app_context_) {
    // Some users want to profile non debuggable apps on rooted devices. If we use run-as,
    // it will be impossible when using --app. So don't switch to app's context when we are
    // root.
    if (!IsRoot()) {
      return RunInAppContext(app_package_name_, "record", args, workload_args.size(),
                             record_filename_, true);
    }
  }
  std::unique_ptr<Workload> workload;
  if (!workload_args.empty()) {
    workload = Workload::CreateWorkload(workload_args);
    if (workload == nullptr) {
      return false;
    }
  }
  time_stat_.prepare_recording_time = GetSystemClock();
  if (!PrepareRecording(workload.get())) {
    return false;
  }
  time_stat_.start_recording_time = GetSystemClock();
  if (!DoRecording(workload.get())) {
    return false;
  }
  return PostProcessRecording(args);
}

bool RecordCommand::PrepareRecording(Workload* workload) {
  // 1. Prepare in other modules.
  PrepareVdsoFile();

  // 2. Add default event type.
  if (event_selection_set_.empty()) {
    size_t group_id;
    if (!event_selection_set_.AddEventType(default_measured_event_type, &group_id)) {
      return false;
    }
    if (sample_speed_) {
      event_selection_set_.SetSampleSpeed(group_id, *sample_speed_);
    }
  }

  // 3. Process options before opening perf event files.
  exclude_kernel_callchain_ = event_selection_set_.ExcludeKernel();
  if (trace_offcpu_ && !TraceOffCpu()) {
    return false;
  }
  if (!SetEventSelectionFlags()) {
    return false;
  }
  if (unwind_dwarf_callchain_) {
    offline_unwinder_.reset(new OfflineUnwinder(false));
  }
  if (unwind_dwarf_callchain_ && allow_callchain_joiner_) {
    callchain_joiner_.reset(new CallChainJoiner(DEFAULT_CALL_CHAIN_JOINER_CACHE_SIZE,
                                                callchain_joiner_min_matching_nodes_,
                                                false));
  }

  // 4. Add monitored targets.
  bool need_to_check_targets = false;
  if (system_wide_collection_) {
    event_selection_set_.AddMonitoredThreads({-1});
  } else if (!event_selection_set_.HasMonitoredTarget()) {
    if (workload != nullptr) {
      event_selection_set_.AddMonitoredProcesses({workload->GetPid()});
      event_selection_set_.SetEnableOnExec(true);
      if (event_selection_set_.HasInplaceSampler()) {
        // Start worker early, because the worker process has to setup inplace-sampler server
        // before we try to connect it.
        if (!workload->Start()) {
          return false;
        }
      }
    } else if (!app_package_name_.empty()) {
      // If app process is not created, wait for it. This allows simpleperf starts before
      // app process. In this way, we can have a better support of app start-up time profiling.
      std::set<pid_t> pids = WaitForAppProcesses(app_package_name_);
      event_selection_set_.AddMonitoredProcesses(pids);
      need_to_check_targets = true;
    } else {
      LOG(ERROR)
          << "No threads to monitor. Try `simpleperf help record` for help";
      return false;
    }
  } else {
    need_to_check_targets = true;
  }
  // Profiling JITed/interpreted Java code is supported starting from Android P.
  if (GetAndroidVersion() >= kAndroidVersionP) {
    // JIT symfiles are stored in temporary files, and are deleted after recording. But if
    // `-g --no-unwind` option is used, we want to keep symfiles to support unwinding in
    // the debug-unwind cmd.
    bool keep_symfiles = dwarf_callchain_sampling_ && !unwind_dwarf_callchain_;
    bool sync_with_records = clockid_ == "monotonic";
    jit_debug_reader_.reset(new JITDebugReader(keep_symfiles, sync_with_records));
    // To profile java code, need to dump maps containing vdex files, which are not executable.
    event_selection_set_.SetRecordNotExecutableMaps(true);
  }

  // 5. Open perf event files and create mapped buffers.
  if (!event_selection_set_.OpenEventFiles(cpus_)) {
    return false;
  }
  size_t record_buffer_size = system_wide_collection_ ? kSystemWideRecordBufferSize
                                                      : kRecordBufferSize;
  if (!event_selection_set_.MmapEventFiles(mmap_page_range_.first, mmap_page_range_.second,
                                           record_buffer_size)) {
    return false;
  }
  auto callback =
      std::bind(&RecordCommand::ProcessRecord, this, std::placeholders::_1);
  if (!event_selection_set_.PrepareToReadMmapEventData(callback)) {
    return false;
  }

  // 6. Create perf.data.
  if (!CreateAndInitRecordFile()) {
    return false;
  }

  // 7. Add read/signal/periodic Events.
  if (need_to_check_targets && !event_selection_set_.StopWhenNoMoreTargets()) {
    return false;
  }
  IOEventLoop* loop = event_selection_set_.GetIOEventLoop();
  auto exit_loop_callback = [loop]() {
    return loop->ExitLoop();
  };
  if (!loop->AddSignalEvents({SIGCHLD, SIGINT, SIGTERM}, exit_loop_callback)) {
    return false;
  }

  // Only add an event for SIGHUP if we didn't inherit SIG_IGN (e.g. from nohup).
  if (!SignalIsIgnored(SIGHUP)) {
    if (!loop->AddSignalEvent(SIGHUP, exit_loop_callback)) {
      return false;
    }
  }
  if (stop_signal_fd_ != -1) {
    if (!loop->AddReadEvent(stop_signal_fd_, exit_loop_callback)) {
      return false;
    }
  }

  if (duration_in_sec_ != 0) {
    if (!loop->AddPeriodicEvent(SecondToTimeval(duration_in_sec_),
                                [loop]() { return loop->ExitLoop(); })) {
      return false;
    }
  }
  if (stdio_controls_profiling_) {
    if (!loop->AddReadEvent(0, [&]() { return ProcessControlCmd(loop); })) {
      return false;
    }
  }
  if (jit_debug_reader_) {
    auto callback = [this](const std::vector<JITDebugInfo>& debug_info, bool sync_kernel_records) {
      return ProcessJITDebugInfo(debug_info, sync_kernel_records);
    };
    if (!jit_debug_reader_->RegisterDebugInfoCallback(loop, callback)) {
      return false;
    }
    if (!app_package_name_.empty()) {
      std::set<pid_t> pids = event_selection_set_.GetMonitoredProcesses();
      for (pid_t tid : event_selection_set_.GetMonitoredThreads()) {
        pid_t pid;
        if (GetProcessForThread(tid, &pid)) {
          pids.insert(pid);
        }
      }
      for (pid_t pid : pids) {
        if (!jit_debug_reader_->MonitorProcess(pid)) {
          return false;
        }
      }
      if (!jit_debug_reader_->ReadAllProcesses()) {
        return false;
      }
    }
  }
  return true;
}

bool RecordCommand::DoRecording(Workload* workload) {
  // Write records in mapped buffers of perf_event_files to output file while workload is running.
  if (workload != nullptr && !workload->IsStarted() && !workload->Start()) {
    return false;
  }
  if (start_profiling_fd_.get() != -1) {
    if (!android::base::WriteStringToFd("STARTED", start_profiling_fd_)) {
      PLOG(ERROR) << "failed to write to start_profiling_fd_";
    }
    start_profiling_fd_.reset();
  }
  if (stdio_controls_profiling_) {
    printf("started\n");
    fflush(stdout);
  }
  if (!event_selection_set_.GetIOEventLoop()->RunLoop()) {
    return false;
  }
  time_stat_.stop_recording_time = GetSystemClock();
  if (!event_selection_set_.FinishReadMmapEventData()) {
    return false;
  }
  time_stat_.finish_recording_time = GetSystemClock();
  return true;
}

static bool WriteRecordDataToOutFd(const std::string& in_filename, android::base::unique_fd out_fd) {
  android::base::unique_fd in_fd(FileHelper::OpenReadOnly(in_filename));
  if (in_fd == -1) {
    PLOG(ERROR) << "Failed to open " << in_filename;
    return false;
  }
  char buf[8192];
  while (true) {
    ssize_t n = TEMP_FAILURE_RETRY(read(in_fd, buf, sizeof(buf)));
    if (n < 0) {
      PLOG(ERROR) << "Failed to read " << in_filename;
      return false;
    }
    if (n == 0) {
      break;
    }
    if (!android::base::WriteFully(out_fd, buf, n)) {
      PLOG(ERROR) << "Failed to write to out_fd";
      return false;
    }
  }
  unlink(in_filename.c_str());
  return true;
}

bool RecordCommand::PostProcessRecording(const std::vector<std::string>& args) {
  // 1. Post unwind dwarf callchain.
  if (unwind_dwarf_callchain_ && post_unwind_) {
    if (!PostUnwindRecords()) {
      return false;
    }
  }

  // 2. Optionally join Callchains.
  if (callchain_joiner_) {
    JoinCallChains();
  }

  // 3. Dump additional features, and close record file.
  if (!DumpAdditionalFeatures(args)) {
    return false;
  }
  if (!record_file_writer_->Close()) {
    return false;
  }
  if (out_fd_ != -1 && !WriteRecordDataToOutFd(record_filename_, std::move(out_fd_))) {
    return false;
  }
  time_stat_.post_process_time = GetSystemClock();

  // 4. Show brief record result.
  size_t lost_samples;
  size_t lost_non_samples;
  size_t cut_stack_samples;
  event_selection_set_.GetLostRecords(&lost_samples, &lost_non_samples, &cut_stack_samples);
  std::string cut_samples;
  if (cut_stack_samples > 0) {
    cut_samples = android::base::StringPrintf(" (cut %zu)", cut_stack_samples);
  }
  lost_record_count_ += lost_samples + lost_non_samples;
  LOG(INFO) << "Samples recorded: " << sample_record_count_ << cut_samples
            << ". Samples lost: " << lost_record_count_ << ".";
  LOG(DEBUG) << "In user space, dropped " << lost_samples << " samples, " << lost_non_samples
             << " non samples, cut stack of " << cut_stack_samples << " samples.";
  if (sample_record_count_ + lost_record_count_ != 0) {
    double lost_percent = static_cast<double>(lost_record_count_) /
                          (lost_record_count_ + sample_record_count_);
    constexpr double LOST_PERCENT_WARNING_BAR = 0.1;
    if (lost_percent >= LOST_PERCENT_WARNING_BAR) {
      LOG(WARNING) << "Lost " << (lost_percent * 100) << "% of samples, "
                   << "consider increasing mmap_pages(-m), "
                   << "or decreasing sample frequency(-f), "
                   << "or increasing sample period(-c).";
    }
  }
  if (callchain_joiner_) {
    callchain_joiner_->DumpStat();
  }
  LOG(DEBUG) << "Prepare recording time "
      << (time_stat_.start_recording_time - time_stat_.prepare_recording_time) / 1e6
      << " ms, recording time "
      << (time_stat_.stop_recording_time - time_stat_.start_recording_time) / 1e6
      << " ms, stop recording time "
      << (time_stat_.finish_recording_time - time_stat_.stop_recording_time) / 1e6
      << " ms, post process time "
      << (time_stat_.post_process_time - time_stat_.finish_recording_time) / 1e6 << " ms.";
  return true;
}

bool RecordCommand::ParseOptions(const std::vector<std::string>& args,
                                 std::vector<std::string>* non_option_args) {
  std::vector<size_t> wait_setting_speed_event_groups_;
  size_t i;
  for (i = 0; i < args.size() && !args[i].empty() && args[i][0] == '-'; ++i) {
    if (args[i] == "-a") {
      system_wide_collection_ = true;
    } else if (args[i] == "--app") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      app_package_name_ = args[i];
    } else if (args[i] == "-b") {
      branch_sampling_ = branch_sampling_type_map["any"];
    } else if (args[i] == "-c" || args[i] == "-f") {
      uint64_t value;
      if (!GetUintOption(args, &i, &value, 1)) {
        return false;
      }
      if (args[i-1] == "-c") {
        sample_speed_.reset(new SampleSpeed(0, value));
      } else {
        if (value >= INT_MAX) {
          LOG(ERROR) << "sample freq can't be bigger than INT_MAX.";
          return false;
        }
        sample_speed_.reset(new SampleSpeed(value, 0));
        max_sample_freq_ = std::max(max_sample_freq_, value);
      }
      for (auto group_id : wait_setting_speed_event_groups_) {
        event_selection_set_.SetSampleSpeed(group_id, *sample_speed_);
      }
      wait_setting_speed_event_groups_.clear();

    } else if (args[i] == "--call-graph") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      std::vector<std::string> strs = android::base::Split(args[i], ",");
      if (strs[0] == "fp") {
        fp_callchain_sampling_ = true;
        dwarf_callchain_sampling_ = false;
      } else if (strs[0] == "dwarf") {
        fp_callchain_sampling_ = false;
        dwarf_callchain_sampling_ = true;
        if (strs.size() > 1) {
          uint64_t size;
          if (!android::base::ParseUint(strs[1], &size)) {
            LOG(ERROR) << "invalid dump stack size in --call-graph option: " << strs[1];
            return false;
          }
          if ((size & 7) != 0) {
            LOG(ERROR) << "dump stack size " << size
                       << " is not 8-byte aligned.";
            return false;
          }
          if (size >= MAX_DUMP_STACK_SIZE) {
            LOG(ERROR) << "dump stack size " << size
                       << " is bigger than max allowed size "
                       << MAX_DUMP_STACK_SIZE << ".";
            return false;
          }
          dump_stack_size_in_dwarf_sampling_ = static_cast<uint32_t>(size);
        }
      } else {
        LOG(ERROR) << "unexpected argument for --call-graph option: "
                   << args[i];
        return false;
      }
    } else if (args[i] == "--clockid") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      if (args[i] != "perf") {
        if (!IsSettingClockIdSupported()) {
          LOG(ERROR) << "Setting clockid is not supported by the kernel.";
          return false;
        }
        if (clockid_map.find(args[i]) == clockid_map.end()) {
          LOG(ERROR) << "Invalid clockid: " << args[i];
          return false;
        }
      }
      clockid_ = args[i];
    } else if (args[i] == "--cpu") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      cpus_ = GetCpusFromString(args[i]);
    } else if (args[i] == "--cpu-percent") {
      if (!GetUintOption(args, &i, &cpu_time_max_percent_, 1, 100)) {
        return false;
      }
    } else if (args[i] == "--duration") {
      if (!GetDoubleOption(args, &i, &duration_in_sec_, 1e-9)) {
        return false;
      }
    } else if (args[i] == "-e") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      std::vector<std::string> event_types = android::base::Split(args[i], ",");
      for (auto& event_type : event_types) {
        size_t group_id;
        if (!event_selection_set_.AddEventType(event_type, &group_id)) {
          return false;
        }
        if (sample_speed_) {
          event_selection_set_.SetSampleSpeed(group_id, *sample_speed_);
        } else {
          wait_setting_speed_event_groups_.push_back(group_id);
        }
      }
    } else if (args[i] == "--exit-with-parent") {
      prctl(PR_SET_PDEATHSIG, SIGHUP, 0, 0, 0);
    } else if (args[i] == "-g") {
      fp_callchain_sampling_ = false;
      dwarf_callchain_sampling_ = true;
    } else if (args[i] == "--group") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      std::vector<std::string> event_types = android::base::Split(args[i], ",");
      size_t group_id;
      if (!event_selection_set_.AddEventGroup(event_types, &group_id)) {
        return false;
      }
      if (sample_speed_) {
        event_selection_set_.SetSampleSpeed(group_id, *sample_speed_);
      } else {
        wait_setting_speed_event_groups_.push_back(group_id);
      }
    } else if (args[i] == "--in-app") {
      in_app_context_ = true;
    } else if (args[i] == "-j") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      std::vector<std::string> branch_sampling_types =
          android::base::Split(args[i], ",");
      for (auto& type : branch_sampling_types) {
        auto it = branch_sampling_type_map.find(type);
        if (it == branch_sampling_type_map.end()) {
          LOG(ERROR) << "unrecognized branch sampling filter: " << type;
          return false;
        }
        branch_sampling_ |= it->second;
      }
    } else if (args[i] == "-m") {
      uint64_t pages;
      if (!GetUintOption(args, &i, &pages)) {
        return false;
      }
      if (!IsPowerOfTwo(pages)) {
        LOG(ERROR) << "Invalid mmap_pages: '" << args[i] << "'";
        return false;
      }
      mmap_page_range_.first = mmap_page_range_.second = pages;
    } else if (args[i] == "--no-dump-kernel-symbols") {
      can_dump_kernel_symbols_ = false;
    } else if (args[i] == "--no-dump-symbols") {
      dump_symbols_ = false;
    } else if (args[i] == "--no-inherit") {
      child_inherit_ = false;
    } else if (args[i] == "--no-unwind") {
      unwind_dwarf_callchain_ = false;
    } else if (args[i] == "--no-callchain-joiner") {
      allow_callchain_joiner_ = false;
    } else if (args[i] == "--callchain-joiner-min-matching-nodes") {
      if (!GetUintOption(args, &i, &callchain_joiner_min_matching_nodes_, 1)) {
        return false;
      }
    } else if (args[i] == "-o") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      record_filename_ = args[i];
    } else if (args[i] == "--out-fd") {
      int fd;
      if (!GetUintOption(args, &i, &fd)) {
        return false;
      }
      out_fd_.reset(fd);
    } else if (args[i] == "-p") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      std::set<pid_t> pids;
      if (!GetValidThreadsFromThreadString(args[i], &pids)) {
        return false;
      }
      event_selection_set_.AddMonitoredProcesses(pids);
    } else if (android::base::StartsWith(args[i], "--post-unwind")) {
      if (args[i] == "--post-unwind" || args[i] == "--post-unwind=yes") {
        post_unwind_ = true;
      } else if (args[i] == "--post-unwind=no") {
        post_unwind_ = false;
      } else {
        LOG(ERROR) << "unexpected option " << args[i];
        return false;
      }
    } else if (args[i] == "--size-limit") {
      if (!GetUintOption(args, &i, &size_limit_in_bytes_, 1, std::numeric_limits<uint64_t>::max(),
                         true)) {
        return false;
      }
    } else if (args[i] == "--start_profiling_fd") {
      int fd;
      if (!GetUintOption(args, &i, &fd)) {
        return false;
      }
      start_profiling_fd_.reset(fd);
    } else if (args[i] == "--stdio-controls-profiling") {
      stdio_controls_profiling_ = true;
    } else if (args[i] == "--stop-signal-fd") {
      int fd;
      if (!GetUintOption(args, &i, &fd)) {
        return false;
      }
      stop_signal_fd_.reset(fd);
    } else if (args[i] == "--symfs") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      if (!Dso::SetSymFsDir(args[i])) {
        return false;
      }
    } else if (args[i] == "-t") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      std::set<pid_t> tids;
      if (!GetValidThreadsFromThreadString(args[i], &tids)) {
        return false;
      }
      event_selection_set_.AddMonitoredThreads(tids);
    } else if (args[i] == "--trace-offcpu") {
      trace_offcpu_ = true;
    } else if (args[i] == "--tracepoint-events") {
      if (!NextArgumentOrError(args, &i)) {
        return false;
      }
      if (!SetTracepointEventsFilePath(args[i])) {
        return false;
      }
    } else if (args[i] == "--") {
      i++;
      break;
    } else {
      ReportUnknownOption(args, i);
      return false;
    }
  }

  if (!dwarf_callchain_sampling_) {
    if (!unwind_dwarf_callchain_) {
      LOG(ERROR)
          << "--no-unwind is only used with `--call-graph dwarf` option.";
      return false;
    }
    unwind_dwarf_callchain_ = false;
  }
  if (post_unwind_) {
    if (!dwarf_callchain_sampling_ || !unwind_dwarf_callchain_) {
      post_unwind_ = false;
    }
  }

  if (fp_callchain_sampling_) {
    if (GetBuildArch() == ARCH_ARM) {
      LOG(WARNING) << "`--callgraph fp` option doesn't work well on arm architecture, "
                   << "consider using `-g` option or profiling on aarch64 architecture.";
    }
  }

  if (system_wide_collection_ && event_selection_set_.HasMonitoredTarget()) {
    LOG(ERROR) << "Record system wide and existing processes/threads can't be "
                  "used at the same time.";
    return false;
  }

  if (system_wide_collection_ && !IsRoot()) {
    LOG(ERROR) << "System wide profiling needs root privilege.";
    return false;
  }

  if (dump_symbols_ && can_dump_kernel_symbols_) {
    // No need to dump kernel symbols as we will dump all required symbols.
    can_dump_kernel_symbols_ = false;
  }
  if (clockid_.empty()) {
    clockid_ = IsSettingClockIdSupported() ? "monotonic" : "perf";
  }

  non_option_args->clear();
  for (; i < args.size(); ++i) {
    non_option_args->push_back(args[i]);
  }
  return true;
}

bool RecordCommand::AdjustPerfEventLimit() {
  bool set_prop = false;
  // 1. Adjust max_sample_rate.
  uint64_t cur_max_freq;
  if (GetMaxSampleFrequency(&cur_max_freq) && cur_max_freq < max_sample_freq_ &&
      !SetMaxSampleFrequency(max_sample_freq_)) {
    set_prop = true;
  }
  // 2. Adjust perf_cpu_time_max_percent.
  size_t cur_percent;
  if (GetCpuTimeMaxPercent(&cur_percent) && cur_percent != cpu_time_max_percent_ &&
      !SetCpuTimeMaxPercent(cpu_time_max_percent_)) {
    set_prop = true;
  }
  // 3. Adjust perf_event_mlock_kb.
  uint64_t mlock_kb = sysconf(_SC_NPROCESSORS_CONF) * (mmap_page_range_.second + 1) * 4;
  uint64_t cur_mlock_kb;
  if (GetPerfEventMlockKb(&cur_mlock_kb) && cur_mlock_kb < mlock_kb &&
      !SetPerfEventMlockKb(mlock_kb)) {
    set_prop = true;
  }

  if (GetAndroidVersion() >= kAndroidVersionP + 1 && set_prop && !in_app_context_) {
    return SetPerfEventLimits(std::max(max_sample_freq_, cur_max_freq), cpu_time_max_percent_,
                              std::max(mlock_kb, cur_mlock_kb));
  }
  return true;
}

bool RecordCommand::TraceOffCpu() {
  if (FindEventTypeByName("sched:sched_switch") == nullptr) {
    LOG(ERROR) << "Can't trace off cpu because sched:sched_switch event is not available";
    return false;
  }
  for (auto& event_type : event_selection_set_.GetTracepointEvents()) {
    if (event_type->name == "sched:sched_switch") {
      LOG(ERROR) << "Trace offcpu can't be used together with sched:sched_switch event";
      return false;
    }
  }
  if (!IsDumpingRegsForTracepointEventsSupported()) {
    LOG(ERROR) << "Dumping regs for tracepoint events is not supported by the kernel";
    return false;
  }
  return event_selection_set_.AddEventType("sched:sched_switch");
}

bool RecordCommand::SetEventSelectionFlags() {
  event_selection_set_.SampleIdAll();
  if (!event_selection_set_.SetBranchSampling(branch_sampling_)) {
    return false;
  }
  if (fp_callchain_sampling_) {
    event_selection_set_.EnableFpCallChainSampling();
  } else if (dwarf_callchain_sampling_) {
    if (!event_selection_set_.EnableDwarfCallChainSampling(
            dump_stack_size_in_dwarf_sampling_)) {
      return false;
    }
  }
  event_selection_set_.SetInherit(child_inherit_);
  if (clockid_ != "perf") {
    event_selection_set_.SetClockId(clockid_map[clockid_]);
  }
  return true;
}

bool RecordCommand::CreateAndInitRecordFile() {
  record_file_writer_ = CreateRecordFile(record_filename_);
  if (record_file_writer_ == nullptr) {
    return false;
  }
  // Use first perf_event_attr and first event id to dump mmap and comm records.
  dumping_attr_id_ = event_selection_set_.GetEventAttrWithId()[0];
  return DumpKernelSymbol() && DumpTracingData() && DumpKernelMaps() && DumpUserSpaceMaps();
}

std::unique_ptr<RecordFileWriter> RecordCommand::CreateRecordFile(
    const std::string& filename) {
  std::unique_ptr<RecordFileWriter> writer =
      RecordFileWriter::CreateInstance(filename);
  if (writer == nullptr) {
    return nullptr;
  }

  if (!writer->WriteAttrSection(event_selection_set_.GetEventAttrWithId())) {
    return nullptr;
  }
  return writer;
}

bool RecordCommand::DumpKernelSymbol() {
  if (can_dump_kernel_symbols_) {
    std::string kallsyms;
    if (event_selection_set_.NeedKernelSymbol() &&
        CheckKernelSymbolAddresses()) {
      if (!android::base::ReadFileToString("/proc/kallsyms", &kallsyms)) {
        PLOG(ERROR) << "failed to read /proc/kallsyms";
        return false;
      }
      KernelSymbolRecord r(kallsyms);
      if (!ProcessRecord(&r)) {
        return false;
      }
    }
  }
  return true;
}

bool RecordCommand::DumpTracingData() {
  std::vector<const EventType*> tracepoint_event_types =
      event_selection_set_.GetTracepointEvents();
  if (tracepoint_event_types.empty() || !CanRecordRawData() || in_app_context_) {
    return true;  // No need to dump tracing data, or can't do it.
  }
  std::vector<char> tracing_data;
  if (!GetTracingData(tracepoint_event_types, &tracing_data)) {
    return false;
  }
  TracingDataRecord record(tracing_data);
  if (!ProcessRecord(&record)) {
    return false;
  }
  return true;
}

bool RecordCommand::DumpKernelMaps() {
  KernelMmap kernel_mmap;
  std::vector<KernelMmap> module_mmaps;
  GetKernelAndModuleMmaps(&kernel_mmap, &module_mmaps);

  MmapRecord mmap_record(*dumping_attr_id_.attr, true, UINT_MAX, 0, kernel_mmap.start_addr,
                         kernel_mmap.len, 0, kernel_mmap.filepath, dumping_attr_id_.ids[0]);
  if (!ProcessRecord(&mmap_record)) {
    return false;
  }
  for (auto& module_mmap : module_mmaps) {
    MmapRecord mmap_record(*dumping_attr_id_.attr, true, UINT_MAX, 0, module_mmap.start_addr,
                           module_mmap.len, 0, module_mmap.filepath, dumping_attr_id_.ids[0]);
    if (!ProcessRecord(&mmap_record)) {
      return false;
    }
  }
  return true;
}

bool RecordCommand::DumpUserSpaceMaps() {
  // For system_wide profiling, maps of a process is dumped when needed (first time a sample hits
  // that process).
  if (system_wide_collection_) {
    return true;
  }
  // Map from process id to a set of thread ids in that process.
  std::unordered_map<pid_t, std::unordered_set<pid_t>> process_map;
  for (pid_t pid : event_selection_set_.GetMonitoredProcesses()) {
    std::vector<pid_t> tids = GetThreadsInProcess(pid);
    process_map[pid].insert(tids.begin(), tids.end());
  }
  for (pid_t tid : event_selection_set_.GetMonitoredThreads()) {
    pid_t pid;
    if (GetProcessForThread(tid, &pid)) {
      process_map[pid].insert(tid);
    }
  }

  // Dump each process.
  for (auto& pair : process_map) {
    if (!DumpProcessMaps(pair.first, pair.second)) {
      return false;
    }
  }
  return true;
}

bool RecordCommand::DumpProcessMaps(pid_t pid, const std::unordered_set<pid_t>& tids) {
  // Dump mmap records.
  std::vector<ThreadMmap> thread_mmaps;
  if (!GetThreadMmapsInProcess(pid, &thread_mmaps)) {
    // The process may exit before we get its info.
    return true;
  }
  const perf_event_attr& attr = *dumping_attr_id_.attr;
  uint64_t event_id = dumping_attr_id_.ids[0];
  for (const auto& map : thread_mmaps) {
    if (!(map.prot & PROT_EXEC) && !event_selection_set_.RecordNotExecutableMaps()) {
      continue;
    }
    Mmap2Record record(attr, false, pid, pid, map.start_addr, map.len,
                      map.pgoff, map.prot, map.name, event_id, last_record_timestamp_);
    if (!ProcessRecord(&record)) {
      return false;
    }
  }
  // Dump process name.
  std::string name = GetCompleteProcessName(pid);
  if (!name.empty()) {
    CommRecord record(attr, pid, pid, name, event_id, last_record_timestamp_);
    if (!ProcessRecord(&record)) {
      return false;
    }
  }
  // Dump thread info.
  for (const auto& tid : tids) {
    if (tid != pid && GetThreadName(tid, &name)) {
      CommRecord comm_record(attr, pid, tid, name, event_id, last_record_timestamp_);
      if (!ProcessRecord(&comm_record)) {
        return false;
      }
    }
  }
  return true;
}

bool RecordCommand::ProcessRecord(Record* record) {
  UpdateRecord(record);
  if (ShouldOmitRecord(record)) {
    return true;
  }
  if (size_limit_in_bytes_ > 0u) {
    if (size_limit_in_bytes_ < record_file_writer_->GetDataSectionSize()) {
      return event_selection_set_.GetIOEventLoop()->ExitLoop();
    }
  }
  if (jit_debug_reader_ && !jit_debug_reader_->UpdateRecord(record)) {
    return false;
  }
  last_record_timestamp_ = std::max(last_record_timestamp_, record->Timestamp());
  // In system wide recording, maps are dumped when they are needed by records.
  if (system_wide_collection_ && !DumpMapsForRecord(record)) {
    return false;
  }
  if (unwind_dwarf_callchain_) {
    if (post_unwind_) {
      return SaveRecordForPostUnwinding(record);
    }
    return SaveRecordAfterUnwinding(record);
  }
  return SaveRecordWithoutUnwinding(record);
}

template <typename MmapRecordType>
bool MapOnlyExistInMemory(MmapRecordType* record) {
  return !record->InKernel() && MappedFileOnlyExistInMemory(record->filename);
}

bool RecordCommand::ShouldOmitRecord(Record* record) {
  if (jit_debug_reader_) {
    // To profile jitted Java code, we need PROT_JIT_SYMFILE_MAP maps not overlapped by maps for
    // [anon:dalvik-jit-code-cache]. To profile interpreted Java code, we record maps that
    // are not executable. Some non-exec maps (like those for stack, heap) provide misleading map
    // entries for unwinding, as in http://b/77236599. So it is better to remove
    // dalvik-jit-code-cache and other maps that only exist in memory.
    switch (record->type()) {
      case PERF_RECORD_MMAP:
        return MapOnlyExistInMemory(static_cast<MmapRecord*>(record));
      case PERF_RECORD_MMAP2:
        return MapOnlyExistInMemory(static_cast<Mmap2Record*>(record));
    }
  }
  return false;
}

bool RecordCommand::DumpMapsForRecord(Record* record) {
  if (record->type() == PERF_RECORD_SAMPLE) {
    pid_t pid = static_cast<SampleRecord*>(record)->tid_data.pid;
    if (dumped_processes_.find(pid) == dumped_processes_.end()) {
      // Dump map info and all thread names for that process.
      std::vector<pid_t> tids = GetThreadsInProcess(pid);
      if (!tids.empty() &&
          !DumpProcessMaps(pid, std::unordered_set<pid_t>(tids.begin(), tids.end()))) {
        return false;
      }
      dumped_processes_.insert(pid);
    }
  }
  return true;
}

bool RecordCommand::SaveRecordForPostUnwinding(Record* record) {
  if (!record_file_writer_->WriteRecord(*record)) {
    LOG(ERROR) << "If there isn't enough space for storing profiling data, consider using "
               << "--no-post-unwind option.";
    return false;
  }
  return true;
}

bool RecordCommand::SaveRecordAfterUnwinding(Record* record) {
  if (record->type() == PERF_RECORD_SAMPLE) {
    auto& r = *static_cast<SampleRecord*>(record);
    // AdjustCallChainGeneratedByKernel() should go before UnwindRecord(). Because we don't want
    // to adjust callchains generated by dwarf unwinder.
    r.AdjustCallChainGeneratedByKernel();
    if (!UnwindRecord(r)) {
      return false;
    }
    // ExcludeKernelCallChain() should go after UnwindRecord() to notice the generated user call
    // chain.
    if (r.InKernel() && exclude_kernel_callchain_ && !r.ExcludeKernelCallChain()) {
      // If current record contains no user callchain, skip it.
      return true;
    }
    sample_record_count_++;
  } else if (record->type() == PERF_RECORD_LOST) {
    lost_record_count_ += static_cast<LostRecord*>(record)->lost;
  } else {
    thread_tree_.Update(*record);
  }
  return record_file_writer_->WriteRecord(*record);
}

bool RecordCommand::SaveRecordWithoutUnwinding(Record* record) {
  if (record->type() == PERF_RECORD_SAMPLE) {
    auto& r = *static_cast<SampleRecord*>(record);
    if (fp_callchain_sampling_ || dwarf_callchain_sampling_) {
      r.AdjustCallChainGeneratedByKernel();
    }
    if (r.InKernel() && exclude_kernel_callchain_ && !r.ExcludeKernelCallChain()) {
      // If current record contains no user callchain, skip it.
      return true;
    }
    sample_record_count_++;
  } else if (record->type() == PERF_RECORD_LOST) {
    lost_record_count_ += static_cast<LostRecord*>(record)->lost;
  }
  return record_file_writer_->WriteRecord(*record);
}

bool RecordCommand::ProcessJITDebugInfo(const std::vector<JITDebugInfo>& debug_info,
                                        bool sync_kernel_records) {
  EventAttrWithId attr_id = event_selection_set_.GetEventAttrWithId()[0];
  for (auto& info : debug_info) {
    if (info.type == JITDebugInfo::JIT_DEBUG_JIT_CODE) {
      uint64_t timestamp = jit_debug_reader_->SyncWithRecords() ? info.timestamp
                                                                : last_record_timestamp_;
      Mmap2Record record(*attr_id.attr, false, info.pid, info.pid,
                         info.jit_code_addr, info.jit_code_len, 0, map_flags::PROT_JIT_SYMFILE_MAP,
                         info.file_path, attr_id.ids[0], timestamp);
      if (!ProcessRecord(&record)) {
        return false;
      }
    } else {
      thread_tree_.AddDexFileOffset(info.file_path, info.dex_file_offset);
    }
  }
  // We want to let samples see the most recent JIT maps generated before them, but no JIT maps
  // generated after them. So process existing samples each time generating new JIT maps. We prefer
  // to process samples after processing JIT maps. Because some of the samples may hit the new JIT
  // maps, and we want to report them properly.
  if (sync_kernel_records && !event_selection_set_.SyncKernelBuffer()) {
    return false;
  }
  return true;
}

bool RecordCommand::ProcessControlCmd(IOEventLoop* loop) {
  char* line = nullptr;
  size_t line_length = 0;
  if (getline(&line, &line_length, stdin) == -1) {
    free(line);
    // When the simpleperf Java API destroys the simpleperf process, it also closes the stdin pipe.
    // So we may see EOF of stdin.
    return loop->ExitLoop();
  }
  std::string cmd = android::base::Trim(line);
  free(line);
  LOG(DEBUG) << "process control cmd: " << cmd;
  bool result = false;
  if (cmd == "pause") {
    result = event_selection_set_.SetEnableEvents(false);
  } else if (cmd == "resume") {
    result = event_selection_set_.SetEnableEvents(true);
  } else {
    LOG(ERROR) << "unknown control cmd: " << cmd;
  }
  printf("%s\n", result ? "ok" : "error");
  fflush(stdout);
  return result;
}

template <class RecordType>
void UpdateMmapRecordForEmbeddedPath(RecordType& r, bool has_prot, uint32_t prot) {
  if (r.InKernel()) {
    return;
  }
  std::string filename = r.filename;
  bool name_changed = false;
  // Some vdex files in map files are marked with deleted flag, but they exist in the file system.
  // It may be because a new file is used to replace the old one, but still worth to try.
  if (android::base::EndsWith(filename, " (deleted)")) {
    filename.resize(filename.size() - 10);
    name_changed = true;
  }
  if (r.data->pgoff != 0 && (!has_prot || (prot & PROT_EXEC))) {
    // For the case of a shared library "foobar.so" embedded
    // inside an APK, we rewrite the original MMAP from
    // ["path.apk" offset=X] to ["path.apk!/foobar.so" offset=W]
    // so as to make the library name explicit. This update is
    // done here (as part of the record operation) as opposed to
    // on the host during the report, since we want to report
    // the correct library name even if the the APK in question
    // is not present on the host. The new offset W is
    // calculated to be with respect to the start of foobar.so,
    // not to the start of path.apk.
    EmbeddedElf* ee = ApkInspector::FindElfInApkByOffset(filename, r.data->pgoff);
    if (ee != nullptr) {
      // Compute new offset relative to start of elf in APK.
      auto data = *r.data;
      data.pgoff -= ee->entry_offset();
      r.SetDataAndFilename(data, GetUrlInApk(filename, ee->entry_name()));
      return;
    }
  }
  std::string zip_path;
  std::string entry_name;
  if (ParseExtractedInMemoryPath(filename, &zip_path, &entry_name)) {
    filename = GetUrlInApk(zip_path, entry_name);
    name_changed = true;
  }
  if (name_changed) {
    auto data = *r.data;
    r.SetDataAndFilename(data, filename);
  }
}

void RecordCommand::UpdateRecord(Record* record) {
  if (record->type() == PERF_RECORD_MMAP) {
    UpdateMmapRecordForEmbeddedPath(*static_cast<MmapRecord*>(record), false, 0);
  } else if (record->type() == PERF_RECORD_MMAP2) {
    auto r = static_cast<Mmap2Record*>(record);
    UpdateMmapRecordForEmbeddedPath(*r, true, r->data->prot);
  } else if (record->type() == PERF_RECORD_COMM) {
    auto r = static_cast<CommRecord*>(record);
    if (r->data->pid == r->data->tid) {
      std::string s = GetCompleteProcessName(r->data->pid);
      if (!s.empty()) {
        r->SetCommandName(s);
      }
    }
  }
}

bool RecordCommand::UnwindRecord(SampleRecord& r) {
  if ((r.sample_type & PERF_SAMPLE_CALLCHAIN) &&
      (r.sample_type & PERF_SAMPLE_REGS_USER) &&
      (r.regs_user_data.reg_mask != 0) &&
      (r.sample_type & PERF_SAMPLE_STACK_USER) &&
      (r.GetValidStackSize() > 0)) {
    ThreadEntry* thread =
        thread_tree_.FindThreadOrNew(r.tid_data.pid, r.tid_data.tid);
    RegSet regs(r.regs_user_data.abi, r.regs_user_data.reg_mask, r.regs_user_data.regs);
    std::vector<uint64_t> ips;
    std::vector<uint64_t> sps;
    if (!offline_unwinder_->UnwindCallChain(*thread, regs, r.stack_user_data.data,
                                            r.GetValidStackSize(), &ips, &sps)) {
      return false;
    }
    // The unwinding may fail if JIT debug info isn't the latest. In this case, read JIT debug info
    // from the process and retry unwinding.
    if (jit_debug_reader_ && !post_unwind_ &&
        offline_unwinder_->IsCallChainBrokenForIncompleteJITDebugInfo()) {
      jit_debug_reader_->ReadProcess(r.tid_data.pid);
      jit_debug_reader_->FlushDebugInfo(r.Timestamp());
      if (!offline_unwinder_->UnwindCallChain(*thread, regs, r.stack_user_data.data,
                                              r.GetValidStackSize(), &ips, &sps)) {
        return false;
      }
    }
    r.ReplaceRegAndStackWithCallChain(ips);
    if (callchain_joiner_) {
      return callchain_joiner_->AddCallChain(r.tid_data.pid, r.tid_data.tid,
                                             CallChainJoiner::ORIGINAL_OFFLINE, ips, sps);
    }
  }
  return true;
}

bool RecordCommand::PostUnwindRecords() {
  // 1. Move records from record_filename_ to a temporary file.
  if (!record_file_writer_->Close()) {
    return false;
  }
  record_file_writer_.reset();
  std::unique_ptr<TemporaryFile> tmp_file = ScopedTempFiles::CreateTempFile();
  if (!Workload::RunCmd({"mv", record_filename_, tmp_file->path})) {
    return false;
  }
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmp_file->path);
  if (!reader) {
    return false;
  }

  // 2. Read records from the temporary file, and write unwound records back to record_filename_.
  record_file_writer_ = CreateRecordFile(record_filename_);
  if (!record_file_writer_) {
    return false;
  }
  sample_record_count_ = 0;
  lost_record_count_ = 0;
  auto callback = [this](std::unique_ptr<Record> record) {
    return SaveRecordAfterUnwinding(record.get());
  };
  return reader->ReadDataSection(callback);
}

bool RecordCommand::JoinCallChains() {
  // 1. Prepare joined callchains.
  if (!callchain_joiner_->JoinCallChains()) {
    return false;
  }
  // 2. Move records from record_filename_ to a temporary file.
  if (!record_file_writer_->Close()) {
    return false;
  }
  record_file_writer_.reset();
  std::unique_ptr<TemporaryFile> tmp_file = ScopedTempFiles::CreateTempFile();
  if (!Workload::RunCmd({"mv", record_filename_, tmp_file->path})) {
    return false;
  }

  // 3. Read records from the temporary file, and write record with joined call chains back
  // to record_filename_.
  std::unique_ptr<RecordFileReader> reader = RecordFileReader::CreateInstance(tmp_file->path);
  record_file_writer_ = CreateRecordFile(record_filename_);
  if (!reader || !record_file_writer_) {
    return false;
  }

  auto record_callback = [&](std::unique_ptr<Record> r) {
    if (r->type() != PERF_RECORD_SAMPLE) {
      return record_file_writer_->WriteRecord(*r);
    }
    SampleRecord& sr = *static_cast<SampleRecord*>(r.get());
    if (!sr.HasUserCallChain()) {
      return record_file_writer_->WriteRecord(sr);
    }
    pid_t pid;
    pid_t tid;
    CallChainJoiner::ChainType type;
    std::vector<uint64_t> ips;
    std::vector<uint64_t> sps;
    if (!callchain_joiner_->GetNextCallChain(pid, tid, type, ips, sps)) {
      return false;
    }
    CHECK_EQ(type, CallChainJoiner::JOINED_OFFLINE);
    CHECK_EQ(pid, static_cast<pid_t>(sr.tid_data.pid));
    CHECK_EQ(tid, static_cast<pid_t>(sr.tid_data.tid));
    sr.UpdateUserCallChain(ips);
    return record_file_writer_->WriteRecord(sr);
  };
  return reader->ReadDataSection(record_callback);
}

bool RecordCommand::DumpAdditionalFeatures(
    const std::vector<std::string>& args) {
  // Read data section of perf.data to collect hit file information.
  thread_tree_.ClearThreadAndMap();
  bool kernel_symbols_available = false;
  if (CheckKernelSymbolAddresses()) {
    Dso::ReadKernelSymbolsFromProc();
    kernel_symbols_available = true;
  }
  auto callback = [&](const Record* r) {
    thread_tree_.Update(*r);
    if (r->type() == PERF_RECORD_SAMPLE) {
      CollectHitFileInfo(*reinterpret_cast<const SampleRecord*>(r));
    }
  };
  if (!record_file_writer_->ReadDataSection(callback)) {
    return false;
  }

  size_t feature_count = 6;
  if (branch_sampling_) {
    feature_count++;
  }
  if (!record_file_writer_->BeginWriteFeatures(feature_count)) {
    return false;
  }
  if (!DumpBuildIdFeature()) {
    return false;
  }
  if (!DumpFileFeature()) {
    return false;
  }
  utsname uname_buf;
  if (TEMP_FAILURE_RETRY(uname(&uname_buf)) != 0) {
    PLOG(ERROR) << "uname() failed";
    return false;
  }
  if (!record_file_writer_->WriteFeatureString(PerfFileFormat::FEAT_OSRELEASE,
                                               uname_buf.release)) {
    return false;
  }
  if (!record_file_writer_->WriteFeatureString(PerfFileFormat::FEAT_ARCH,
                                               uname_buf.machine)) {
    return false;
  }

  std::string exec_path = android::base::GetExecutablePath();
  if (exec_path.empty()) exec_path = "simpleperf";
  std::vector<std::string> cmdline;
  cmdline.push_back(exec_path);
  cmdline.push_back("record");
  cmdline.insert(cmdline.end(), args.begin(), args.end());
  if (!record_file_writer_->WriteCmdlineFeature(cmdline)) {
    return false;
  }
  if (branch_sampling_ != 0 &&
      !record_file_writer_->WriteBranchStackFeature()) {
    return false;
  }
  if (!DumpMetaInfoFeature(kernel_symbols_available)) {
    return false;
  }

  if (!record_file_writer_->EndWriteFeatures()) {
    return false;
  }
  return true;
}

bool RecordCommand::DumpBuildIdFeature() {
  std::vector<BuildIdRecord> build_id_records;
  BuildId build_id;
  std::vector<Dso*> dso_v = thread_tree_.GetAllDsos();
  for (Dso* dso : dso_v) {
    if (!dso->HasDumpId()) {
      continue;
    }
    if (dso->type() == DSO_KERNEL) {
      if (!GetKernelBuildId(&build_id)) {
        continue;
      }
      build_id_records.push_back(
          BuildIdRecord(true, UINT_MAX, build_id, dso->Path()));
    } else if (dso->type() == DSO_KERNEL_MODULE) {
      std::string path = dso->Path();
      std::string module_name = basename(&path[0]);
      if (android::base::EndsWith(module_name, ".ko")) {
        module_name = module_name.substr(0, module_name.size() - 3);
      }
      if (!GetModuleBuildId(module_name, &build_id)) {
        LOG(DEBUG) << "can't read build_id for module " << module_name;
        continue;
      }
      build_id_records.push_back(BuildIdRecord(true, UINT_MAX, build_id, path));
    } else if (dso->type() == DSO_ELF_FILE) {
      if (dso->Path() == DEFAULT_EXECNAME_FOR_THREAD_MMAP) {
        continue;
      }
      if (!GetBuildIdFromDsoPath(dso->Path(), &build_id)) {
        LOG(DEBUG) << "Can't read build_id from file " << dso->Path();
        continue;
      }
      build_id_records.push_back(
          BuildIdRecord(false, UINT_MAX, build_id, dso->Path()));
    }
  }
  if (!record_file_writer_->WriteBuildIdFeature(build_id_records)) {
    return false;
  }
  return true;
}

bool RecordCommand::DumpFileFeature() {
  std::vector<Dso*> dso_v = thread_tree_.GetAllDsos();
  return record_file_writer_->WriteFileFeatures(thread_tree_.GetAllDsos());
}

bool RecordCommand::DumpMetaInfoFeature(bool kernel_symbols_available) {
  std::unordered_map<std::string, std::string> info_map;
  info_map["simpleperf_version"] = GetSimpleperfVersion();
  info_map["system_wide_collection"] = system_wide_collection_ ? "true" : "false";
  info_map["trace_offcpu"] = trace_offcpu_ ? "true" : "false";
  // By storing event types information in perf.data, the readers of perf.data have the same
  // understanding of event types, even if they are on another machine.
  info_map["event_type_info"] = ScopedEventTypes::BuildString(event_selection_set_.GetEvents());
#if defined(__ANDROID__)
  info_map["product_props"] = android::base::StringPrintf("%s:%s:%s",
                                  android::base::GetProperty("ro.product.manufacturer", "").c_str(),
                                  android::base::GetProperty("ro.product.model", "").c_str(),
                                  android::base::GetProperty("ro.product.name", "").c_str());
  info_map["android_version"] = android::base::GetProperty("ro.build.version.release", "");
  if (!app_package_name_.empty()) {
    info_map["app_package_name"] = app_package_name_;
  }
#endif
  info_map["clockid"] = clockid_;
  info_map["timestamp"] = std::to_string(time(nullptr));
  info_map["kernel_symbols_available"] = kernel_symbols_available ? "true" : "false";
  return record_file_writer_->WriteMetaInfoFeature(info_map);
}

void RecordCommand::CollectHitFileInfo(const SampleRecord& r) {
  const ThreadEntry* thread =
      thread_tree_.FindThreadOrNew(r.tid_data.pid, r.tid_data.tid);
  const MapEntry* map =
      thread_tree_.FindMap(thread, r.ip_data.ip, r.InKernel());
  Dso* dso = map->dso;
  const Symbol* symbol;
  if (dump_symbols_) {
    symbol = thread_tree_.FindSymbol(map, r.ip_data.ip, nullptr, &dso);
    if (!symbol->HasDumpId()) {
      dso->CreateSymbolDumpId(symbol);
    }
  }
  if (!dso->HasDumpId() && dso->type() != DSO_UNKNOWN_FILE) {
    dso->CreateDumpId();
  }
  if (r.sample_type & PERF_SAMPLE_CALLCHAIN) {
    bool in_kernel = r.InKernel();
    bool first_ip = true;
    for (uint64_t i = 0; i < r.callchain_data.ip_nr; ++i) {
      uint64_t ip = r.callchain_data.ips[i];
      if (ip >= PERF_CONTEXT_MAX) {
        switch (ip) {
          case PERF_CONTEXT_KERNEL:
            in_kernel = true;
            break;
          case PERF_CONTEXT_USER:
            in_kernel = false;
            break;
          default:
            LOG(DEBUG) << "Unexpected perf_context in callchain: " << std::hex
                       << ip;
        }
      } else {
        if (first_ip) {
          first_ip = false;
          // Remove duplication with sample ip.
          if (ip == r.ip_data.ip) {
            continue;
          }
        }
        map = thread_tree_.FindMap(thread, ip, in_kernel);
        dso = map->dso;
        if (dump_symbols_) {
          symbol = thread_tree_.FindSymbol(map, ip, nullptr, &dso);
          if (!symbol->HasDumpId()) {
            dso->CreateSymbolDumpId(symbol);
          }
        }
        if (!dso->HasDumpId() && dso->type() != DSO_UNKNOWN_FILE) {
          dso->CreateDumpId();
        }
      }
    }
  }
}

void RegisterRecordCommand() {
  RegisterCommand("record",
                  [] { return std::unique_ptr<Command>(new RecordCommand()); });
}
