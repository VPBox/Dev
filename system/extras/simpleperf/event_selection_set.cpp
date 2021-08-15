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

#include "event_selection_set.h"

#include <algorithm>
#include <atomic>
#include <thread>

#include <android-base/logging.h>

#include "environment.h"
#include "event_attr.h"
#include "event_type.h"
#include "IOEventLoop.h"
#include "perf_regs.h"
#include "utils.h"
#include "RecordReadThread.h"

bool IsBranchSamplingSupported() {
  const EventType* type = FindEventTypeByName("cpu-cycles");
  if (type == nullptr) {
    return false;
  }
  perf_event_attr attr = CreateDefaultPerfEventAttr(*type);
  attr.sample_type |= PERF_SAMPLE_BRANCH_STACK;
  attr.branch_sample_type = PERF_SAMPLE_BRANCH_ANY;
  return IsEventAttrSupported(attr);
}

bool IsDwarfCallChainSamplingSupported() {
  const EventType* type = FindEventTypeByName("cpu-cycles");
  if (type == nullptr) {
    return false;
  }
  perf_event_attr attr = CreateDefaultPerfEventAttr(*type);
  attr.sample_type |=
      PERF_SAMPLE_CALLCHAIN | PERF_SAMPLE_REGS_USER | PERF_SAMPLE_STACK_USER;
  attr.exclude_callchain_user = 1;
  attr.sample_regs_user = GetSupportedRegMask(GetBuildArch());
  attr.sample_stack_user = 8192;
  return IsEventAttrSupported(attr);
}

bool IsDumpingRegsForTracepointEventsSupported() {
  const EventType* event_type = FindEventTypeByName("sched:sched_switch", false);
  if (event_type == nullptr) {
    return false;
  }
  std::atomic<bool> done(false);
  std::atomic<pid_t> thread_id(0);
  std::thread thread([&]() {
    thread_id = gettid();
    while (!done) {
      usleep(1);
    }
    usleep(1);  // Make a sched out to generate one sample.
  });
  while (thread_id == 0) {
    usleep(1);
  }
  perf_event_attr attr = CreateDefaultPerfEventAttr(*event_type);
  attr.freq = 0;
  attr.sample_period = 1;
  std::unique_ptr<EventFd> event_fd =
      EventFd::OpenEventFile(attr, thread_id, -1, nullptr);
  if (event_fd == nullptr) {
    return false;
  }
  if (!event_fd->CreateMappedBuffer(4, true)) {
    return false;
  }
  done = true;
  thread.join();

  std::vector<char> buffer = event_fd->GetAvailableMmapData();
  std::vector<std::unique_ptr<Record>> records =
      ReadRecordsFromBuffer(attr, buffer.data(), buffer.size());
  for (auto& r : records) {
    if (r->type() == PERF_RECORD_SAMPLE) {
      auto& record = *static_cast<SampleRecord*>(r.get());
      if (record.ip_data.ip != 0) {
        return true;
      }
    }
  }
  return false;
}

bool IsSettingClockIdSupported() {
  // Do the real check only once and keep the result in a static variable.
  static int is_supported = -1;
  if (is_supported == -1) {
    const EventType* type = FindEventTypeByName("cpu-cycles");
    if (type == nullptr) {
      is_supported = 0;
    } else {
      // Check if the kernel supports setting clockid, which was added in kernel 4.0. Just check
      // with one clockid is enough. Because all needed clockids were supported before kernel 4.0.
      perf_event_attr attr = CreateDefaultPerfEventAttr(*type);
      attr.use_clockid = 1;
      attr.clockid = CLOCK_MONOTONIC;
      is_supported = IsEventAttrSupported(attr) ? 1 : 0;
    }
  }
  return is_supported;
}

bool IsMmap2Supported() {
  const EventType* type = FindEventTypeByName("cpu-cycles");
  if (type == nullptr) {
    return false;
  }
  perf_event_attr attr = CreateDefaultPerfEventAttr(*type);
  attr.mmap2 = 1;
  return IsEventAttrSupported(attr);
}

EventSelectionSet::EventSelectionSet(bool for_stat_cmd)
    : for_stat_cmd_(for_stat_cmd), loop_(new IOEventLoop) {}

EventSelectionSet::~EventSelectionSet() {}

bool EventSelectionSet::BuildAndCheckEventSelection(const std::string& event_name, bool first_event,
                                                    EventSelection* selection) {
  std::unique_ptr<EventTypeAndModifier> event_type = ParseEventType(event_name);
  if (event_type == nullptr) {
    return false;
  }
  if (for_stat_cmd_) {
    if (event_type->event_type.name == "cpu-clock" ||
        event_type->event_type.name == "task-clock") {
      if (event_type->exclude_user || event_type->exclude_kernel) {
        LOG(ERROR) << "Modifier u and modifier k used in event type "
                   << event_type->event_type.name
                   << " are not supported by the kernel.";
        return false;
      }
    }
  }
  selection->event_type_modifier = *event_type;
  selection->event_attr = CreateDefaultPerfEventAttr(event_type->event_type);
  selection->event_attr.exclude_user = event_type->exclude_user;
  selection->event_attr.exclude_kernel = event_type->exclude_kernel;
  selection->event_attr.exclude_hv = event_type->exclude_hv;
  selection->event_attr.exclude_host = event_type->exclude_host;
  selection->event_attr.exclude_guest = event_type->exclude_guest;
  selection->event_attr.precise_ip = event_type->precise_ip;
  bool set_default_sample_freq = false;
  if (!for_stat_cmd_) {
    if (event_type->event_type.type == PERF_TYPE_TRACEPOINT) {
      selection->event_attr.freq = 0;
      selection->event_attr.sample_period = DEFAULT_SAMPLE_PERIOD_FOR_TRACEPOINT_EVENT;
    } else {
      selection->event_attr.freq = 1;
      // Set default sample freq here may print msg "Adjust sample freq to max allowed sample
      // freq". But this is misleading. Because default sample freq may not be the final sample
      // freq we use. So use minimum sample freq (1) here.
      selection->event_attr.sample_freq = 1;
      set_default_sample_freq = true;
    }
    // We only need to dump mmap and comm records for the first event type. Because all event types
    // are monitoring the same processes.
    if (first_event) {
      selection->event_attr.mmap = 1;
      selection->event_attr.comm = 1;
      if (IsMmap2Supported()) {
        selection->event_attr.mmap2 = 1;
      }
    }
  }
  if (!IsEventAttrSupported(selection->event_attr)) {
    LOG(ERROR) << "Event type '" << event_type->name
               << "' is not supported on the device";
    return false;
  }
  if (set_default_sample_freq) {
    selection->event_attr.sample_freq = DEFAULT_SAMPLE_FREQ_FOR_NONTRACEPOINT_EVENT;
  }

  selection->event_fds.clear();

  for (const auto& group : groups_) {
    for (const auto& sel : group) {
      if (sel.event_type_modifier.name == selection->event_type_modifier.name) {
        LOG(ERROR) << "Event type '" << sel.event_type_modifier.name
                   << "' appears more than once";
        return false;
      }
    }
  }
  return true;
}

bool EventSelectionSet::AddEventType(const std::string& event_name, size_t* group_id) {
  return AddEventGroup(std::vector<std::string>(1, event_name), group_id);
}

bool EventSelectionSet::AddEventGroup(
    const std::vector<std::string>& event_names, size_t* group_id) {
  EventSelectionGroup group;
  bool first_event = groups_.empty();
  for (const auto& event_name : event_names) {
    EventSelection selection;
    if (!BuildAndCheckEventSelection(event_name, first_event, &selection)) {
      return false;
    }
    first_event = false;
    group.push_back(std::move(selection));
  }
  groups_.push_back(std::move(group));
  UnionSampleType();
  if (group_id != nullptr) {
    *group_id = groups_.size() - 1;
  }
  return true;
}

std::vector<const EventType*> EventSelectionSet::GetEvents() const {
  std::vector<const EventType*> result;
  for (const auto& group : groups_) {
    for (const auto& selection : group) {
      result.push_back(&selection.event_type_modifier.event_type);
    }
  }
  return result;
}

std::vector<const EventType*> EventSelectionSet::GetTracepointEvents() const {
  std::vector<const EventType*> result;
  for (const auto& group : groups_) {
    for (const auto& selection : group) {
      if (selection.event_type_modifier.event_type.type ==
          PERF_TYPE_TRACEPOINT) {
        result.push_back(&selection.event_type_modifier.event_type);
      }
    }
  }
  return result;
}

bool EventSelectionSet::ExcludeKernel() const {
  for (const auto& group : groups_) {
    for (const auto& selection : group) {
      if (!selection.event_type_modifier.exclude_kernel) {
        return false;
      }
    }
  }
  return true;
}

bool EventSelectionSet::HasInplaceSampler() const {
  for (const auto& group : groups_) {
    for (const auto& sel : group) {
      if (sel.event_attr.type == SIMPLEPERF_TYPE_USER_SPACE_SAMPLERS &&
          sel.event_attr.config == SIMPLEPERF_CONFIG_INPLACE_SAMPLER) {
        return true;
      }
    }
  }
  return false;
}

std::vector<EventAttrWithId> EventSelectionSet::GetEventAttrWithId() const {
  std::vector<EventAttrWithId> result;
  for (const auto& group : groups_) {
    for (const auto& selection : group) {
      EventAttrWithId attr_id;
      attr_id.attr = &selection.event_attr;
      for (const auto& fd : selection.event_fds) {
        attr_id.ids.push_back(fd->Id());
      }
      if (!selection.inplace_samplers.empty()) {
        attr_id.ids.push_back(selection.inplace_samplers[0]->Id());
      }
      result.push_back(attr_id);
    }
  }
  return result;
}

// Union the sample type of different event attrs can make reading sample
// records in perf.data easier.
void EventSelectionSet::UnionSampleType() {
  uint64_t sample_type = 0;
  for (const auto& group : groups_) {
    for (const auto& selection : group) {
      sample_type |= selection.event_attr.sample_type;
    }
  }
  for (auto& group : groups_) {
    for (auto& selection : group) {
      selection.event_attr.sample_type = sample_type;
    }
  }
}

void EventSelectionSet::SetEnableOnExec(bool enable) {
  for (auto& group : groups_) {
    for (auto& selection : group) {
      // If sampling is enabled on exec, then it is disabled at startup,
      // otherwise it should be enabled at startup. Don't use
      // ioctl(PERF_EVENT_IOC_ENABLE) to enable it after perf_event_open().
      // Because some android kernels can't handle ioctl() well when cpu-hotplug
      // happens. See http://b/25193162.
      if (enable) {
        selection.event_attr.enable_on_exec = 1;
        selection.event_attr.disabled = 1;
      } else {
        selection.event_attr.enable_on_exec = 0;
        selection.event_attr.disabled = 0;
      }
    }
  }
}

bool EventSelectionSet::GetEnableOnExec() {
  for (const auto& group : groups_) {
    for (const auto& selection : group) {
      if (selection.event_attr.enable_on_exec == 0) {
        return false;
      }
    }
  }
  return true;
}

void EventSelectionSet::SampleIdAll() {
  for (auto& group : groups_) {
    for (auto& selection : group) {
      selection.event_attr.sample_id_all = 1;
    }
  }
}

void EventSelectionSet::SetSampleSpeed(size_t group_id, const SampleSpeed& speed) {
  CHECK_LT(group_id, groups_.size());
  for (auto& selection : groups_[group_id]) {
    if (speed.UseFreq()) {
      selection.event_attr.freq = 1;
      selection.event_attr.sample_freq = speed.sample_freq;
    } else {
      selection.event_attr.freq = 0;
      selection.event_attr.sample_period = speed.sample_period;
    }
  }
}

bool EventSelectionSet::SetBranchSampling(uint64_t branch_sample_type) {
  if (branch_sample_type != 0 &&
      (branch_sample_type &
       (PERF_SAMPLE_BRANCH_ANY | PERF_SAMPLE_BRANCH_ANY_CALL |
        PERF_SAMPLE_BRANCH_ANY_RETURN | PERF_SAMPLE_BRANCH_IND_CALL)) == 0) {
    LOG(ERROR) << "Invalid branch_sample_type: 0x" << std::hex
               << branch_sample_type;
    return false;
  }
  if (branch_sample_type != 0 && !IsBranchSamplingSupported()) {
    LOG(ERROR) << "branch stack sampling is not supported on this device.";
    return false;
  }
  for (auto& group : groups_) {
    for (auto& selection : group) {
      perf_event_attr& attr = selection.event_attr;
      if (branch_sample_type != 0) {
        attr.sample_type |= PERF_SAMPLE_BRANCH_STACK;
      } else {
        attr.sample_type &= ~PERF_SAMPLE_BRANCH_STACK;
      }
      attr.branch_sample_type = branch_sample_type;
    }
  }
  return true;
}

void EventSelectionSet::EnableFpCallChainSampling() {
  for (auto& group : groups_) {
    for (auto& selection : group) {
      selection.event_attr.sample_type |= PERF_SAMPLE_CALLCHAIN;
    }
  }
}

bool EventSelectionSet::EnableDwarfCallChainSampling(uint32_t dump_stack_size) {
  if (!IsDwarfCallChainSamplingSupported()) {
    LOG(ERROR) << "dwarf callchain sampling is not supported on this device.";
    return false;
  }
  for (auto& group : groups_) {
    for (auto& selection : group) {
      selection.event_attr.sample_type |= PERF_SAMPLE_CALLCHAIN |
                                          PERF_SAMPLE_REGS_USER |
                                          PERF_SAMPLE_STACK_USER;
      selection.event_attr.exclude_callchain_user = 1;
      selection.event_attr.sample_regs_user =
          GetSupportedRegMask(GetMachineArch());
      selection.event_attr.sample_stack_user = dump_stack_size;
    }
  }
  return true;
}

void EventSelectionSet::SetInherit(bool enable) {
  for (auto& group : groups_) {
    for (auto& selection : group) {
      selection.event_attr.inherit = (enable ? 1 : 0);
    }
  }
}

void EventSelectionSet::SetClockId(int clock_id) {
  for (auto& group : groups_) {
    for (auto& selection : group) {
      selection.event_attr.use_clockid = 1;
      selection.event_attr.clockid = clock_id;
    }
  }
}

bool EventSelectionSet::NeedKernelSymbol() const {
  for (const auto& group : groups_) {
    for (const auto& selection : group) {
      if (!selection.event_type_modifier.exclude_kernel) {
        return true;
      }
    }
  }
  return false;
}

void EventSelectionSet::SetRecordNotExecutableMaps(bool record) {
  // We only need to dump non-executable mmap records for the first event type.
  groups_[0][0].event_attr.mmap_data = record ? 1 : 0;
}

bool EventSelectionSet::RecordNotExecutableMaps() const {
  return groups_[0][0].event_attr.mmap_data == 1;
}

static bool CheckIfCpusOnline(const std::vector<int>& cpus) {
  std::vector<int> online_cpus = GetOnlineCpus();
  for (const auto& cpu : cpus) {
    if (std::find(online_cpus.begin(), online_cpus.end(), cpu) ==
        online_cpus.end()) {
      LOG(ERROR) << "cpu " << cpu << " is not online.";
      return false;
    }
  }
  return true;
}

bool EventSelectionSet::OpenEventFilesOnGroup(EventSelectionGroup& group,
                                              pid_t tid, int cpu,
                                              std::string* failed_event_type) {
  std::vector<std::unique_ptr<EventFd>> event_fds;
  // Given a tid and cpu, events on the same group should be all opened
  // successfully or all failed to open.
  EventFd* group_fd = nullptr;
  for (auto& selection : group) {
    std::unique_ptr<EventFd> event_fd =
        EventFd::OpenEventFile(selection.event_attr, tid, cpu, group_fd, false);
    if (!event_fd) {
        *failed_event_type = selection.event_type_modifier.name;
        return false;
    }
    LOG(VERBOSE) << "OpenEventFile for " << event_fd->Name();
    event_fds.push_back(std::move(event_fd));
    if (group_fd == nullptr) {
      group_fd = event_fds.back().get();
    }
  }
  for (size_t i = 0; i < group.size(); ++i) {
    group[i].event_fds.push_back(std::move(event_fds[i]));
  }
  return true;
}

static std::map<pid_t, std::set<pid_t>> PrepareThreads(const std::set<pid_t>& processes,
                                                       const std::set<pid_t>& threads) {
  std::map<pid_t, std::set<pid_t>> result;
  for (auto& pid : processes) {
    std::vector<pid_t> tids = GetThreadsInProcess(pid);
    std::set<pid_t>& threads_in_process = result[pid];
    threads_in_process.insert(tids.begin(), tids.end());
  }
  for (auto& tid : threads) {
    // tid = -1 means monitoring all threads.
    if (tid == -1) {
      result[-1].insert(-1);
    } else {
      pid_t pid;
      if (GetProcessForThread(tid, &pid)) {
        result[pid].insert(tid);
      }
    }
  }
  return result;
}

bool EventSelectionSet::OpenEventFiles(const std::vector<int>& on_cpus) {
  std::vector<int> cpus = on_cpus;
  if (!cpus.empty()) {
    // cpus = {-1} means open an event file for all cpus.
    if (!(cpus.size() == 1 && cpus[0] == -1) && !CheckIfCpusOnline(cpus)) {
      return false;
    }
  } else {
    cpus = GetOnlineCpus();
  }
  std::map<pid_t, std::set<pid_t>> process_map = PrepareThreads(processes_, threads_);
  for (auto& group : groups_) {
    if (IsUserSpaceSamplerGroup(group)) {
      if (!OpenUserSpaceSamplersOnGroup(group, process_map)) {
        return false;
      }
    } else {
      for (const auto& pair : process_map) {
        size_t success_count = 0;
        std::string failed_event_type;
        for (const auto& tid : pair.second) {
          for (const auto& cpu : cpus) {
            if (OpenEventFilesOnGroup(group, tid, cpu, &failed_event_type)) {
              success_count++;
            }
          }
        }
        // We can't guarantee to open perf event file successfully for each thread on each cpu.
        // Because threads may exit between PrepareThreads() and OpenEventFilesOnGroup(), and
        // cpus may be offlined between GetOnlineCpus() and OpenEventFilesOnGroup().
        // So we only check that we can at least monitor one thread for each process.
        if (success_count == 0) {
          PLOG(ERROR) << "failed to open perf event file for event_type "
                      << failed_event_type << " for "
                      << (pair.first == -1 ? "all threads"
                                           : "threads in process " + std::to_string(pair.first));
          return false;
        }
      }
    }
  }
  return true;
}

bool EventSelectionSet::IsUserSpaceSamplerGroup(EventSelectionGroup& group) {
  return group.size() == 1 && group[0].event_attr.type == SIMPLEPERF_TYPE_USER_SPACE_SAMPLERS;
}

bool EventSelectionSet::OpenUserSpaceSamplersOnGroup(EventSelectionGroup& group,
    const std::map<pid_t, std::set<pid_t>>& process_map) {
  CHECK_EQ(group.size(), 1u);
  for (auto& selection : group) {
    if (selection.event_attr.type == SIMPLEPERF_TYPE_USER_SPACE_SAMPLERS &&
        selection.event_attr.config == SIMPLEPERF_CONFIG_INPLACE_SAMPLER) {
      for (auto& pair : process_map) {
        std::unique_ptr<InplaceSamplerClient> sampler = InplaceSamplerClient::Create(
            selection.event_attr, pair.first, pair.second);
        if (sampler == nullptr) {
          return false;
        }
        selection.inplace_samplers.push_back(std::move(sampler));
      }
    }
  }
  return true;
}

static bool ReadCounter(EventFd* event_fd, CounterInfo* counter) {
  if (!event_fd->ReadCounter(&counter->counter)) {
    return false;
  }
  counter->tid = event_fd->ThreadId();
  counter->cpu = event_fd->Cpu();
  return true;
}

bool EventSelectionSet::ReadCounters(std::vector<CountersInfo>* counters) {
  counters->clear();
  for (size_t i = 0; i < groups_.size(); ++i) {
    for (auto& selection : groups_[i]) {
      CountersInfo counters_info;
      counters_info.group_id = i;
      counters_info.event_name = selection.event_type_modifier.event_type.name;
      counters_info.event_modifier = selection.event_type_modifier.modifier;
      counters_info.counters = selection.hotplugged_counters;
      for (auto& event_fd : selection.event_fds) {
        CounterInfo counter;
        if (!ReadCounter(event_fd.get(), &counter)) {
          return false;
        }
        counters_info.counters.push_back(counter);
      }
      counters->push_back(counters_info);
    }
  }
  return true;
}

bool EventSelectionSet::MmapEventFiles(size_t min_mmap_pages, size_t max_mmap_pages,
                                       size_t record_buffer_size) {
  record_read_thread_.reset(new simpleperf::RecordReadThread(
      record_buffer_size, groups_[0][0].event_attr, min_mmap_pages, max_mmap_pages));
  return true;
}

bool EventSelectionSet::PrepareToReadMmapEventData(const std::function<bool(Record*)>& callback) {
  // Prepare record callback function.
  record_callback_ = callback;
  if (!record_read_thread_->RegisterDataCallback(*loop_,
                                                 [this]() { return ReadMmapEventData(true); })) {
    return false;
  }
  std::vector<EventFd*> event_fds;
  for (auto& group : groups_) {
    for (auto& selection : group) {
      for (auto& event_fd : selection.event_fds) {
        event_fds.push_back(event_fd.get());
      }
    }
  }
  return record_read_thread_->AddEventFds(event_fds);
}

bool EventSelectionSet::SyncKernelBuffer() {
  return record_read_thread_->SyncKernelBuffer();
}

// Read records from the RecordBuffer. If with_time_limit is false, read until the RecordBuffer is
// empty, otherwise stop after 100 ms or when the record buffer is empty.
bool EventSelectionSet::ReadMmapEventData(bool with_time_limit) {
  uint64_t start_time_in_ns;
  if (with_time_limit) {
    start_time_in_ns = GetSystemClock();
  }
  std::unique_ptr<Record> r;
  while ((r = record_read_thread_->GetRecord()) != nullptr) {
    if (!record_callback_(r.get())) {
      return false;
    }
    if (with_time_limit && (GetSystemClock() - start_time_in_ns) >= 1e8) {
      break;
    }
  }
  return true;
}

bool EventSelectionSet::FinishReadMmapEventData() {
  // Stop the read thread, so we don't get more records beyond current time.
  if (!SyncKernelBuffer() || !record_read_thread_->StopReadThread()) {
    return false;
  }
  if (!ReadMmapEventData(false)) {
    return false;
  }
  if (!HasInplaceSampler()) {
    return true;
  }
  // Inplace sampler server uses a buffer to cache samples before sending them, so we need to
  // explicitly ask it to send the cached samples.
  loop_.reset(new IOEventLoop);
  size_t inplace_sampler_count = 0;
  auto close_callback = [&]() {
    if (--inplace_sampler_count == 0) {
      return loop_->ExitLoop();
    }
    return true;
  };
  for (auto& group : groups_) {
    for (auto& sel : group) {
      for (auto& sampler : sel.inplace_samplers) {
        if (!sampler->IsClosed()) {
          if (!sampler->StopProfiling(*loop_, close_callback)) {
            return false;
          }
          inplace_sampler_count++;
        }
      }
    }
  }
  if (inplace_sampler_count == 0) {
    return true;
  }

  // Set a timeout to exit the loop.
  timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if (!loop_->AddPeriodicEvent(tv, [&]() { return loop_->ExitLoop(); })) {
    return false;
  }
  return loop_->RunLoop();
}

void EventSelectionSet::GetLostRecords(size_t* lost_samples, size_t* lost_non_samples,
                                       size_t* cut_stack_samples) {
  record_read_thread_->GetLostRecords(lost_samples, lost_non_samples, cut_stack_samples);
}

bool EventSelectionSet::HandleCpuHotplugEvents(const std::vector<int>& monitored_cpus,
                                               double check_interval_in_sec) {
  monitored_cpus_.insert(monitored_cpus.begin(), monitored_cpus.end());
  online_cpus_ = GetOnlineCpus();
  if (!loop_->AddPeriodicEvent(SecondToTimeval(check_interval_in_sec),
                               [&]() { return DetectCpuHotplugEvents(); })) {
    return false;
  }
  return true;
}

bool EventSelectionSet::DetectCpuHotplugEvents() {
  std::vector<int> new_cpus = GetOnlineCpus();
  for (const auto& cpu : online_cpus_) {
    if (std::find(new_cpus.begin(), new_cpus.end(), cpu) == new_cpus.end()) {
      if (monitored_cpus_.empty() ||
          monitored_cpus_.find(cpu) != monitored_cpus_.end()) {
        LOG(INFO) << "Cpu " << cpu << " is offlined";
        if (!HandleCpuOfflineEvent(cpu)) {
          return false;
        }
      }
    }
  }
  for (const auto& cpu : new_cpus) {
    if (std::find(online_cpus_.begin(), online_cpus_.end(), cpu) ==
        online_cpus_.end()) {
      if (monitored_cpus_.empty() ||
          monitored_cpus_.find(cpu) != monitored_cpus_.end()) {
        LOG(INFO) << "Cpu " << cpu << " is onlined";
        if (!HandleCpuOnlineEvent(cpu)) {
          return false;
        }
      }
    }
  }
  online_cpus_ = new_cpus;
  return true;
}

bool EventSelectionSet::HandleCpuOfflineEvent(int cpu) {
  if (!for_stat_cmd_) {
    // Read mmap data here, so we won't lose the existing records of the
    // offlined cpu.
    if (!ReadMmapEventData(true)) {
      return false;
    }
  }
  if (record_read_thread_) {
    std::vector<EventFd*> remove_fds;
    for (auto& group : groups_) {
      for (auto& selection : group) {
        for (auto& fd : selection.event_fds) {
          if (fd->Cpu() == cpu) {
            remove_fds.push_back(fd.get());
          }
        }
      }
    }
    if (!record_read_thread_->RemoveEventFds(remove_fds)) {
      return false;
    }
  }
  for (auto& group : groups_) {
    for (auto& selection : group) {
      for (auto it = selection.event_fds.begin(); it != selection.event_fds.end();) {
        if ((*it)->Cpu() == cpu) {
          if (for_stat_cmd_) {
            CounterInfo counter;
            if (!ReadCounter(it->get(), &counter)) {
              return false;
            }
            selection.hotplugged_counters.push_back(counter);
          }
          it = selection.event_fds.erase(it);
        } else {
          ++it;
        }
      }
    }
  }
  return true;
}

bool EventSelectionSet::HandleCpuOnlineEvent(int cpu) {
  // We need to start profiling when opening new event files.
  SetEnableOnExec(false);
  std::map<pid_t, std::set<pid_t>> process_map = PrepareThreads(processes_, threads_);
  for (auto& group : groups_) {
    if (IsUserSpaceSamplerGroup(group)) {
      continue;
    }
    for (const auto& pair : process_map) {
      for (const auto& tid : pair.second) {
        std::string failed_event_type;
        if (!OpenEventFilesOnGroup(group, tid, cpu, &failed_event_type)) {
          // If failed to open event files, maybe the cpu has been offlined.
          PLOG(WARNING) << "failed to open perf event file for event_type "
                        << failed_event_type << " for "
                        << (tid == -1 ? "all threads" : "thread " + std::to_string(tid))
                        << " on cpu " << cpu;
        }
      }
    }
  }
  if (record_read_thread_) {
    // Prepare mapped buffer.
    if (!CreateMappedBufferForCpu(cpu)) {
      return false;
    }
    // Send a EventIdRecord.
    std::vector<uint64_t> event_id_data;
    uint64_t attr_id = 0;
    for (const auto& group : groups_) {
      for (const auto& selection : group) {
        for (const auto& event_fd : selection.event_fds) {
          if (event_fd->Cpu() == cpu) {
            event_id_data.push_back(attr_id);
            event_id_data.push_back(event_fd->Id());
          }
        }
        ++attr_id;
      }
    }
    EventIdRecord r(event_id_data);
    if (!record_callback_(&r)) {
      return false;
    }
  }
  return true;
}

bool EventSelectionSet::CreateMappedBufferForCpu(int cpu) {
  std::vector<EventFd*> event_fds;
  for (auto& group : groups_) {
    for (auto& selection : group) {
      for (auto& fd : selection.event_fds) {
        if (fd->Cpu() == cpu) {
          event_fds.push_back(fd.get());
        }
      }
    }
  }
  return record_read_thread_->AddEventFds(event_fds);
}

bool EventSelectionSet::StopWhenNoMoreTargets(double check_interval_in_sec) {
  return loop_->AddPeriodicEvent(SecondToTimeval(check_interval_in_sec),
                                 [&]() { return CheckMonitoredTargets(); });
}

bool EventSelectionSet::CheckMonitoredTargets() {
  if (!HasSampler()) {
    return loop_->ExitLoop();
  }
  for (const auto& tid : threads_) {
    if (IsThreadAlive(tid)) {
      return true;
    }
  }
  for (const auto& pid : processes_) {
    if (IsThreadAlive(pid)) {
      return true;
    }
  }
  return loop_->ExitLoop();
}

bool EventSelectionSet::HasSampler() {
  for (auto& group : groups_) {
    for (auto& sel : group) {
      if (!sel.event_fds.empty()) {
        return true;
      }
      for (auto& sampler : sel.inplace_samplers) {
        if (!sampler->IsClosed()) {
          return true;
        }
      }
    }
  }
  return false;
}

bool EventSelectionSet::SetEnableEvents(bool enable) {
  for (auto& group : groups_) {
    for (auto& sel : group) {
      for (auto& fd : sel.event_fds) {
        if (!fd->SetEnableEvent(enable)) {
          return false;
        }
      }
    }
  }
  return true;
}
