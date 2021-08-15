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

#include "configreader.h"

#include <inttypes.h>

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>

#include "perfprofd_config.pb.h"

using android::base::StringPrintf;

//
// Config file path
//
static const char *config_file_path =
    "/data/data/com.google.android.gms/files/perfprofd.conf";

struct ConfigReader::Data {
  struct values {
    unsigned minv;
    unsigned maxv;
  };
  std::map<std::string, values> u_info;
  std::map<std::string, unsigned> u_entries;
  std::map<std::string, std::string> s_entries;

  struct events {
    std::vector<std::string> names;
    unsigned period;
    bool group;
  };
  std::vector<events> e_entries;
  bool trace_config_read;
};

ConfigReader::ConfigReader() : data_(new ConfigReader::Data())
{
  data_->trace_config_read = false;
  addDefaultEntries();
}

ConfigReader::~ConfigReader()
{
}

const char *ConfigReader::getConfigFilePath()
{
  return config_file_path;
}

void ConfigReader::setConfigFilePath(const char *path)
{
  config_file_path = strdup(path);
  LOG(INFO) << "config file path set to " << config_file_path;
}

//
// Populate the reader with the set of allowable entries
//
void ConfigReader::addDefaultEntries()
{
  struct DummyConfig : public Config {
    void Sleep(size_t seconds) override {}
    bool IsProfilingEnabled() const override { return false; }
  };
  DummyConfig config;

  // Average number of seconds between perf profile collections (if
  // set to 100, then over time we want to see a perf profile
  // collected every 100 seconds). The actual time within the interval
  // for the collection is chosen randomly.
  addUnsignedEntry("collection_interval", config.collection_interval_in_s, 0, UINT32_MAX);

  // Use the specified fixed seed for random number generation (unit
  // testing)
  addUnsignedEntry("use_fixed_seed", config.use_fixed_seed, 0, UINT32_MAX);

  // For testing purposes, number of times to iterate through main
  // loop.  Value of zero indicates that we should loop forever.
  addUnsignedEntry("main_loop_iterations", config.main_loop_iterations, 0, UINT32_MAX);

  // Destination directory (where to write profiles).
  addStringEntry("destination_directory", config.destination_directory.c_str());

  // Config directory (where to read configs).
  addStringEntry("config_directory", config.config_directory.c_str());

  // Full path to 'perf' executable.
  addStringEntry("perf_path", config.perf_path.c_str());

  // Desired sampling period (passed to perf -c option).
  addUnsignedEntry("sampling_period", config.sampling_period, 0, UINT32_MAX);
  // Desired sampling frequency (passed to perf -f option).
  addUnsignedEntry("sampling_frequency", config.sampling_frequency, 0, UINT32_MAX);

  // Length of time to collect samples (number of seconds for 'perf
  // record -a' run).
  addUnsignedEntry("sample_duration", config.sample_duration_in_s, 1, 600);

  // If this parameter is non-zero it will cause perfprofd to
  // exit immediately if the build type is not userdebug or eng.
  // Currently defaults to 1 (true).
  addUnsignedEntry("only_debug_build", config.only_debug_build ? 1 : 0, 0, 1);

  // If the "mpdecision" service is running at the point we are ready
  // to kick off a profiling run, then temporarily disable the service
  // and hard-wire all cores on prior to the collection run, provided
  // that the duration of the recording is less than or equal to the value of
  // 'hardwire_cpus_max_duration'.
  addUnsignedEntry("hardwire_cpus", config.hardwire_cpus, 0, 1);
  addUnsignedEntry("hardwire_cpus_max_duration",
                   config.hardwire_cpus_max_duration_in_s,
                   1,
                   UINT32_MAX);

  // Maximum number of unprocessed profiles we can accumulate in the
  // destination directory. Once we reach this limit, we continue
  // to collect, but we just overwrite the most recent profile.
  addUnsignedEntry("max_unprocessed_profiles", config.max_unprocessed_profiles, 1, UINT32_MAX);

  // If set to 1, pass the -g option when invoking 'perf' (requests
  // stack traces as opposed to flat profile).
  addUnsignedEntry("stack_profile", config.stack_profile ? 1 : 0, 0, 1);

  // For unit testing only: if set to 1, emit info messages on config
  // file parsing.
  addUnsignedEntry("trace_config_read", config.trace_config_read ? 1 : 0, 0, 1);

  // Control collection of various additional profile tags
  addUnsignedEntry("collect_cpu_utilization", config.collect_cpu_utilization ? 1 : 0, 0, 1);
  addUnsignedEntry("collect_charging_state", config.collect_charging_state ? 1 : 0, 0, 1);
  addUnsignedEntry("collect_booting", config.collect_booting ? 1 : 0, 0, 1);
  addUnsignedEntry("collect_camera_active", config.collect_camera_active ? 1 : 0, 0, 1);

  // If true, use an ELF symbolizer to on-device symbolize.
  addUnsignedEntry("use_elf_symbolizer", config.use_elf_symbolizer ? 1 : 0, 0, 1);
  // Whether to symbolize everything. If false, objects with build ID will be skipped.
  addUnsignedEntry("symbolize_everything", config.symbolize_everything ? 1 : 0, 0, 1);

  // If true, use libz to compress the output proto.
  addUnsignedEntry("compress", config.compress ? 1 : 0, 0, 1);

  // If true, send the proto to dropbox instead of to a file.
  addUnsignedEntry("dropbox", config.send_to_dropbox ? 1 : 0, 0, 1);

  // The pid of the process to profile. May be negative, in which case
  // the whole system will be profiled.
  addUnsignedEntry("process", static_cast<uint32_t>(-1), 0, UINT32_MAX);

  // Whether to fail or strip unsupported events.
  addUnsignedEntry("fail_on_unsupported_events", config.fail_on_unsupported_events ? 1 : 0, 0, 1);
}

void ConfigReader::addUnsignedEntry(const char *key,
                                    unsigned default_value,
                                    unsigned min_value,
                                    unsigned max_value)
{
  std::string ks(key);
  CHECK(data_->u_entries.find(ks) == data_->u_entries.end() &&
        data_->s_entries.find(ks) == data_->s_entries.end())
      << "internal error -- duplicate entry for key " << key;
  Data::values vals;
  vals.minv = min_value;
  vals.maxv = max_value;
  data_->u_info[ks] = vals;
  data_->u_entries[ks] = default_value;
}

void ConfigReader::addStringEntry(const char *key, const char *default_value)
{
  std::string ks(key);
  CHECK(data_->u_entries.find(ks) == data_->u_entries.end() &&
        data_->s_entries.find(ks) == data_->s_entries.end())
      << "internal error -- duplicate entry for key " << key;
  CHECK(default_value != nullptr) << "internal error -- bad default value for key " << key;
  data_->s_entries[ks] = std::string(default_value);
}

unsigned ConfigReader::getUnsignedValue(const char *key) const
{
  std::string ks(key);
  auto it = data_->u_entries.find(ks);
  CHECK(it != data_->u_entries.end());
  return it->second;
}

bool ConfigReader::getBoolValue(const char *key) const
{
  std::string ks(key);
  auto it = data_->u_entries.find(ks);
  CHECK(it != data_->u_entries.end());
  return it->second != 0;
}

std::string ConfigReader::getStringValue(const char *key) const
{
  std::string ks(key);
  auto it = data_->s_entries.find(ks);
  CHECK(it != data_->s_entries.end());
  return it->second;
}

void ConfigReader::overrideUnsignedEntry(const char *key, unsigned new_value)
{
  std::string ks(key);
  auto it = data_->u_entries.find(ks);
  CHECK(it != data_->u_entries.end());
  Data::values vals;
  auto iit = data_->u_info.find(key);
  CHECK(iit != data_->u_info.end());
  vals = iit->second;
  CHECK(new_value >= vals.minv && new_value <= vals.maxv);
  it->second = new_value;
  LOG(INFO) << "option " << key << " overridden to " << new_value;
}


//
// Parse a key=value pair read from the config file. This will issue
// warnings or errors to the system logs if the line can't be
// interpreted properly.
//
bool ConfigReader::parseLine(const std::string& key,
                             const std::string& value,
                             unsigned linecount,
                             std::string* error_msg)
{
  if (key.empty()) {
    *error_msg = StringPrintf("line %u: Key is empty", linecount);
    return false;
  }
  if (value.empty()) {
    *error_msg = StringPrintf("line %u: Value for %s is empty", linecount, key.c_str());
    return false;
  }

  auto uit = data_->u_entries.find(key);
  if (uit != data_->u_entries.end()) {
    uint64_t conv;
    if (!android::base::ParseUint(value, &conv)) {
      *error_msg = StringPrintf("line %u: value %s cannot be parsed", linecount, value.c_str());
      return false;
    }
    Data::values vals;
    auto iit = data_->u_info.find(key);
    DCHECK(iit != data_->u_info.end());
    vals = iit->second;
    if (conv < vals.minv || conv > vals.maxv) {
      *error_msg = StringPrintf("line %u: "
                                    "specified value %" PRIu64 " for '%s' "
                                    "outside permitted range [%u %u]",
                                linecount,
                                conv,
                                key.c_str(),
                                vals.minv,
                                vals.maxv);
      return false;
    } else {
      if (data_->trace_config_read) {
        LOG(INFO) << "option " << key << " set to " << conv;
      }
      uit->second = static_cast<unsigned>(conv);
    }
    data_->trace_config_read = (getUnsignedValue("trace_config_read") != 0);
    return true;
  }

  auto sit = data_->s_entries.find(key);
  if (sit != data_->s_entries.end()) {
    if (data_->trace_config_read) {
      LOG(INFO) << "option " << key << " set to " << value;
    }
    sit->second = std::string(value);
    return true;
  }

  // Check whether this follows event syntax, and create an event entry, if necessary.
  // -e_evtname(,evtname)*=period
  // -g_evtname(,evtname)*=period
  {
    bool event_key = android::base::StartsWith(key, "-e_");
    bool group_key = android::base::StartsWith(key, "-g_");
    if (event_key || group_key) {
      Data::events events;
      events.group = group_key;

      uint64_t conv;
      if (!android::base::ParseUint(value, &conv)) {
        *error_msg = StringPrintf("line %u: key %s cannot be parsed", linecount, key.c_str());
        return false;
      }
      if (conv > std::numeric_limits<unsigned>::max()) {
        *error_msg = StringPrintf("line %u: key %s: period too large", linecount, key.c_str());
        return false;
      }
      events.period = static_cast<unsigned>(conv);

      events.names = android::base::Split(key.substr(3), ",");
      data_->e_entries.push_back(events);
      return true;
    }
  }

  *error_msg = StringPrintf("line %u: unknown option '%s'", linecount, key.c_str());
  return false;
}

static bool isblank(const std::string &line)
{
  auto non_space = [](char c) { return isspace(c) == 0; };
  return std::find_if(line.begin(), line.end(), non_space) == line.end();
}



bool ConfigReader::readFile()
{
  std::string contents;
  if (! android::base::ReadFileToString(config_file_path, &contents)) {
    return false;
  }
  std::string error_msg;
  if (!Read(contents, /* fail_on_error */ false, &error_msg)) {
    LOG(ERROR) << error_msg;
    return false;
  }
  if (!error_msg.empty()) {
    LOG(WARNING) << error_msg;
  }
  return true;
}

bool ConfigReader::Read(const std::string& content, bool fail_on_error, std::string* error_msg) {
  std::stringstream ss(content);
  std::string line;

  auto append_error = [error_msg](const std::string& tmp) {
    if (!error_msg->empty()) {
      error_msg->append("\n");
      error_msg->append(tmp);
    } else {
      *error_msg = tmp;
    }
  };

  for (unsigned linecount = 1;
       std::getline(ss,line,'\n');
       linecount += 1)
  {

    // comment line?
    if (line[0] == '#') {
      continue;
    }

    // blank line?
    if (isblank(line)) {
      continue;
    }

    // look for X=Y assignment
    auto efound = line.find('=');
    if (efound == std::string::npos) {
      append_error(StringPrintf("line %u: line malformed (no '=' found)", linecount));
      if (fail_on_error) {
        return false;
      }
      continue;
    }

    std::string key(line.substr(0, efound));
    std::string value(line.substr(efound+1, std::string::npos));

    std::string local_error_msg;
    bool parse_success = parseLine(key, value, linecount, &local_error_msg);
    if (!parse_success) {
      append_error(local_error_msg);
      if (fail_on_error) {
        return false;
      }
    }
  }

  return true;
}

void ConfigReader::FillConfig(Config* config) {
  config->collection_interval_in_s = getUnsignedValue("collection_interval");

  config->use_fixed_seed = getUnsignedValue("use_fixed_seed");

  config->main_loop_iterations = getUnsignedValue("main_loop_iterations");

  config->destination_directory = getStringValue("destination_directory");

  config->config_directory = getStringValue("config_directory");

  config->perf_path = getStringValue("perf_path");

  config->sampling_period = getUnsignedValue("sampling_period");
  config->sampling_frequency = getUnsignedValue("sampling_frequency");

  config->sample_duration_in_s = getUnsignedValue("sample_duration");

  config->only_debug_build = getBoolValue("only_debug_build");

  config->hardwire_cpus = getBoolValue("hardwire_cpus");
  config->hardwire_cpus_max_duration_in_s = getUnsignedValue("hardwire_cpus_max_duration");

  config->max_unprocessed_profiles = getUnsignedValue("max_unprocessed_profiles");

  config->stack_profile = getBoolValue("stack_profile");

  config->trace_config_read = getBoolValue("trace_config_read");

  config->collect_cpu_utilization = getBoolValue("collect_cpu_utilization");
  config->collect_charging_state = getBoolValue("collect_charging_state");
  config->collect_booting = getBoolValue("collect_booting");
  config->collect_camera_active = getBoolValue("collect_camera_active");

  config->process = static_cast<int32_t>(getUnsignedValue("process"));
  config->use_elf_symbolizer = getBoolValue("use_elf_symbolizer");
  config->symbolize_everything = getBoolValue("symbolize_everything");
  config->compress = getBoolValue("compress");
  config->send_to_dropbox = getBoolValue("dropbox");
  config->fail_on_unsupported_events = getBoolValue("fail_on_unsupported_events");

  config->event_config.clear();
  for (const auto& event : data_->e_entries) {
    Config::PerfCounterConfigElem elem;
    elem.events = event.names;
    elem.group = event.group;
    elem.sampling_period = event.period;
    config->event_config.push_back(std::move(elem));
  }
}

namespace {

template <typename T>
struct OssFormatter {
};

template <>
struct OssFormatter<std::string> {
  void Add(std::ostream& os, const std::string& val) {
    os << val;
  }
};

template <>
struct OssFormatter<uint32_t> {
  void Add(std::ostream& os, const uint32_t& val) {
    os << val;
  }
};

template <>
struct OssFormatter<int32_t> {
  void Add(std::ostream& os, const int32_t& val) {
    os << val;
  }
};

template <>
struct OssFormatter<bool> {
  void Add(std::ostream& os, const bool& val) {
    os << (val ? 1 : 0);
  }
};


}  // namespace

std::string ConfigReader::ConfigToString(const Config& config) {
  std::ostringstream oss;

  auto add = [&oss](const char* str, auto val) {
    if (oss.tellp() != 0) {
      oss << ' ';
    }
    oss << str << '=';
    OssFormatter<decltype(val)> fmt;
    fmt.Add(oss, val);
  };

  add("collection_interval", config.collection_interval_in_s);
  add("use_fixed_seed", config.use_fixed_seed);
  add("main_loop_iterations", config.main_loop_iterations);

  add("destination_directory", config.destination_directory);  // TODO: Escape.
  add("config_directory", config.config_directory);            // TODO: Escape.
  add("perf_path", config.perf_path);                          // TODO: Escape.

  add("sampling_period", config.sampling_period);
  add("sampling_frequency", config.sampling_frequency);

  add("sample_duration", config.sample_duration_in_s);

  add("only_debug_build", config.only_debug_build);

  add("hardwire_cpus", config.hardwire_cpus);

  add("hardwire_cpus_max_duration", config.hardwire_cpus_max_duration_in_s);

  add("max_unprocessed_profiles", config.max_unprocessed_profiles);

  add("stack_profile", config.stack_profile);

  add("trace_config_read", config.trace_config_read);

  add("collect_cpu_utilization", config.collect_cpu_utilization);
  add("collect_charging_state", config.collect_charging_state);
  add("collect_booting", config.collect_booting);
  add("collect_camera_active", config.collect_camera_active);

  add("process", config.process);
  add("use_elf_symbolizer", config.use_elf_symbolizer);
  add("symbolize_everything", config.symbolize_everything);
  add("compress", config.compress);
  add("dropbox", config.send_to_dropbox);
  add("fail_on_unsupported_events", config.fail_on_unsupported_events);

  for (const auto& elem : config.event_config) {
    std::ostringstream oss_elem;
    oss_elem << '-' << (elem.group ? 'g' : 'e') << '_';
    bool first = true;
    for (const auto& event : elem.events) {
      if (!first) {
        oss_elem << ',';
      }
      oss_elem << event;
      first = false;
    }
    add(oss_elem.str().c_str(), elem.sampling_period);
  }

  return oss.str();
}

void ConfigReader::ProtoToConfig(const android::perfprofd::ProfilingConfig& in, Config* out) {
  // Copy base proto values.
#define CHECK_AND_COPY_FROM_PROTO(name)      \
  if (in.has_ ## name()) {      \
    out->name = in.name();  \
  }
  CHECK_AND_COPY_FROM_PROTO(collection_interval_in_s)
  CHECK_AND_COPY_FROM_PROTO(use_fixed_seed)
  CHECK_AND_COPY_FROM_PROTO(main_loop_iterations)
  CHECK_AND_COPY_FROM_PROTO(destination_directory)
  CHECK_AND_COPY_FROM_PROTO(config_directory)
  CHECK_AND_COPY_FROM_PROTO(perf_path)
  CHECK_AND_COPY_FROM_PROTO(sampling_period)
  CHECK_AND_COPY_FROM_PROTO(sampling_frequency)
  CHECK_AND_COPY_FROM_PROTO(sample_duration_in_s)
  CHECK_AND_COPY_FROM_PROTO(only_debug_build)
  CHECK_AND_COPY_FROM_PROTO(hardwire_cpus)
  CHECK_AND_COPY_FROM_PROTO(hardwire_cpus_max_duration_in_s)
  CHECK_AND_COPY_FROM_PROTO(max_unprocessed_profiles)
  CHECK_AND_COPY_FROM_PROTO(stack_profile)
  CHECK_AND_COPY_FROM_PROTO(collect_cpu_utilization)
  CHECK_AND_COPY_FROM_PROTO(collect_charging_state)
  CHECK_AND_COPY_FROM_PROTO(collect_booting)
  CHECK_AND_COPY_FROM_PROTO(collect_camera_active)
  CHECK_AND_COPY_FROM_PROTO(process)
  CHECK_AND_COPY_FROM_PROTO(use_elf_symbolizer)
  CHECK_AND_COPY_FROM_PROTO(symbolize_everything)
  CHECK_AND_COPY_FROM_PROTO(send_to_dropbox)
  CHECK_AND_COPY_FROM_PROTO(compress)
  CHECK_AND_COPY_FROM_PROTO(fail_on_unsupported_events)
#undef CHECK_AND_COPY_FROM_PROTO

  // Convert counters.
  for (const auto& event_config : in.event_config()) {
    Config::PerfCounterConfigElem config_elem;

    if (event_config.counters_size() == 0) {
      LOG(WARNING) << "Missing counters.";
      continue;
    }
    config_elem.events.reserve(event_config.counters_size());
    for (const std::string& str : event_config.counters()) {
      config_elem.events.push_back(str);
    }
    config_elem.group = event_config.has_as_group() ? event_config.as_group() : false;
    config_elem.sampling_period = event_config.has_sampling_period()
                                      ? event_config.sampling_period()
                                      : 0;
    out->event_config.push_back(std::move(config_elem));
  }
}
