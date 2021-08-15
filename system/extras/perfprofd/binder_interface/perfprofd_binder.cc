/*
**
** Copyright 2017, The Android Open Source Project
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

#include "perfprofd_binder.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <inttypes.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <binder/BinderService.h>
#include <binder/IResultReceiver.h>
#include <binder/Status.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <utils/String16.h>
#include <utils/String8.h>
#include <utils/Vector.h>

#include "android/os/BnPerfProfd.h"
#include "perfprofd_config.pb.h"
#include "perfprofd_record.pb.h"

#include "config.h"
#include "configreader.h"
#include "perfprofdcore.h"
#include "perfprofd_threaded_handler.h"

namespace android {
namespace perfprofd {
namespace binder {

namespace {

using Status = ::android::binder::Status;

class PerfProfdNativeService : public BinderService<PerfProfdNativeService>,
                               public ::android::os::BnPerfProfd,
                               public ThreadedHandler {
 public:
  static status_t start();
  static int Main();

  static char const* getServiceName() { return "perfprofd"; }

  status_t dump(int fd, const Vector<String16> &args) override;

  Status startProfiling(int32_t collectionInterval,
                        int32_t iterations,
                        int32_t process,
                        int32_t samplingPeriod,
                        int32_t samplingFrequency,
                        int32_t sampleDuration,
                        bool stackProfile,
                        bool useElfSymbolizer,
                        bool sendToDropbox) override;
  Status startProfilingString(const String16& config) override;
  Status startProfilingProtobuf(const std::vector<uint8_t>& config_proto) override;

  Status stopProfiling() override;

  // Override onTransact so we can handle shellCommand.
  status_t onTransact(uint32_t _aidl_code,
                      const Parcel& _aidl_data,
                      Parcel* _aidl_reply,
                      uint32_t _aidl_flags = 0) override;

 private:
  status_t shellCommand(int /*in*/, int out, int err, Vector<String16>& args);

  template <typename ProtoLoaderFn> Status StartProfilingProtobuf(ProtoLoaderFn fn);
  Status StartProfilingProtobufFd(int fd);
};

status_t PerfProfdNativeService::start() {
  IPCThreadState::self()->disableBackgroundScheduling(true);
  status_t ret = BinderService<PerfProfdNativeService>::publish();
  if (ret != android::OK) {
    return ret;
  }
  sp<ProcessState> ps(ProcessState::self());
  ps->startThreadPool();
  ps->giveThreadPoolName();
  return android::OK;
}

status_t PerfProfdNativeService::dump(int fd, const Vector<String16> &args) {
  auto out = std::fstream(base::StringPrintf("/proc/self/fd/%d", fd));
  auto print_config = [&out](bool is_profiling, const Config* config) {
    if (is_profiling) {
      out << "Profiling with config: " << ConfigReader::ConfigToString(*config);
    } else {
      out << "Not actively profiling.";
    }
  };
  RunOnConfig(print_config);
  out << std::endl;

  return NO_ERROR;
}

Status PerfProfdNativeService::startProfiling(int32_t collectionInterval,
                                              int32_t iterations,
                                              int32_t process,
                                              int32_t samplingPeriod,
                                              int32_t samplingFrequency,
                                              int32_t sampleDuration,
                                              bool stackProfile,
                                              bool useElfSymbolizer,
                                              bool sendToDropbox) {
  auto config_fn = [&](ThreadedConfig& config) {
    config = ThreadedConfig();  // Reset to a default config.

    if (collectionInterval >= 0) {
      config.collection_interval_in_s = collectionInterval;
    }
    if (iterations >= 0) {
      config.main_loop_iterations = iterations;
    }
    if (process >= 0) {
      config.process = process;
    }
    if (samplingPeriod > 0) {
      config.sampling_period = samplingPeriod;
    }
    if (samplingFrequency > 0) {
      config.sampling_frequency = samplingFrequency;
    }
    if (sampleDuration > 0) {
      config.sample_duration_in_s = sampleDuration;
    }
    config.stack_profile = stackProfile;
    config.use_elf_symbolizer = useElfSymbolizer;
    config.send_to_dropbox = sendToDropbox;
  };
  std::string error_msg;
  if (!StartProfiling(config_fn, &error_msg)) {
    return Status::fromExceptionCode(1, error_msg.c_str());
  }
  return Status::ok();
}
Status PerfProfdNativeService::startProfilingString(const String16& config) {
  ConfigReader reader;
  std::string error_msg;
  // Split configuration along colon.
  std::vector<std::string> args = base::Split(String8(config).string(), ":");
  for (auto& arg : args) {
    if (!reader.Read(arg, /* fail_on_error */ true, &error_msg)) {
      std::string tmp = base::StringPrintf("Could not parse %s: %s",
                                           arg.c_str(),
                                           error_msg.c_str());
      return Status::fromExceptionCode(1, tmp.c_str());
    }
  }
  auto config_fn = [&](ThreadedConfig& config) {
    config = ThreadedConfig();  // Reset to a default config.
    reader.FillConfig(&config);
  };
  if (!StartProfiling(config_fn, &error_msg)) {
    return Status::fromExceptionCode(1, error_msg.c_str());
  }
  return Status::ok();
}
Status PerfProfdNativeService::startProfilingProtobuf(const std::vector<uint8_t>& config_proto) {
  auto proto_loader_fn = [&config_proto](ProfilingConfig& proto_config) {
    return proto_config.ParseFromArray(config_proto.data(), config_proto.size());
  };
  return StartProfilingProtobuf(proto_loader_fn);
}

template <typename ProtoLoaderFn>
Status PerfProfdNativeService::StartProfilingProtobuf(ProtoLoaderFn fn) {
  ProfilingConfig proto_config;
  if (!fn(proto_config)) {
    return binder::Status::fromExceptionCode(2, "Could not read protobuf");
  }
  auto config_fn = [&proto_config](ThreadedConfig& config) {
    config = ThreadedConfig();  // Reset to a default config.
    ConfigReader::ProtoToConfig(proto_config, &config);
  };
  std::string error_msg;
  if (!StartProfiling(config_fn, &error_msg)) {
    return Status::fromExceptionCode(1, error_msg.c_str());
  }
  return Status::ok();
}

Status PerfProfdNativeService::StartProfilingProtobufFd(int fd) {
  auto proto_loader_fn = [fd](ProfilingConfig& proto_config) {
    struct IstreamCopyingInputStream : public google::protobuf::io::CopyingInputStream {
      IstreamCopyingInputStream(int fd_in)
                : stream(base::StringPrintf("/proc/self/fd/%d", fd_in),
                         std::ios::binary | std::ios::in) {
      }

      int Read(void* buffer, int size) override {
        stream.read(reinterpret_cast<char*>(buffer), size);
        size_t count = stream.gcount();
        if (count > 0) {
          return count;
        }
        return -1;
      }

      std::ifstream stream;
    };
    std::unique_ptr<IstreamCopyingInputStream> is(new IstreamCopyingInputStream(fd));
    std::unique_ptr<google::protobuf::io::CopyingInputStreamAdaptor> is_adaptor(
        new google::protobuf::io::CopyingInputStreamAdaptor(is.get()));
    return proto_config.ParseFromZeroCopyStream(is_adaptor.get());
  };
  return StartProfilingProtobuf(proto_loader_fn);
}

Status PerfProfdNativeService::stopProfiling() {
  std::string error_msg;
  if (!StopProfiling(&error_msg)) {
    Status::fromExceptionCode(1, error_msg.c_str());
  }
  return Status::ok();
}

status_t PerfProfdNativeService::shellCommand(int in,
                                              int out,
                                              int err_fd,
                                              Vector<String16>& args) {
  if (android::base::kEnableDChecks) {
    LOG(VERBOSE) << "Perfprofd::shellCommand";

    for (size_t i = 0, n = args.size(); i < n; i++) {
      LOG(VERBOSE) << "  arg[" << i << "]: '" << String8(args[i]).string() << "'";
    }
  }

  auto err_str = std::fstream(base::StringPrintf("/proc/self/fd/%d", err_fd));

  if (args.size() >= 1) {
    if (args[0] == String16("dump")) {
      dump(out, args);
      return OK;
    } else if (args[0] == String16("startProfiling")) {
      ConfigReader reader;
      for (size_t i = 1; i < args.size(); ++i) {
        std::string error_msg;
        if (!reader.Read(String8(args[i]).string(), /* fail_on_error */ true, &error_msg)) {
          err_str << "Could not parse '" << String8(args[i]).string() << "': " << error_msg
                  << std::endl;
          return BAD_VALUE;
        }
      }
      auto config_fn = [&](ThreadedConfig& config) {
        config = ThreadedConfig();  // Reset to a default config.
        reader.FillConfig(&config);
      };
      std::string error_msg;
      if (!StartProfiling(config_fn, &error_msg)) {
        err_str << error_msg << std::endl;
        return UNKNOWN_ERROR;
      }
      return OK;
    } else if (args[0] == String16("startProfilingProto")) {
      if (args.size() < 2) {
        return BAD_VALUE;
      }
      int fd = -1;
      if (args[1] == String16("-")) {
        fd = in;
      } else {
        // TODO: Implement reading from disk?
      }
      if (fd < 0) {
        err_str << "Bad file descriptor " << args[1] << std::endl;
        return BAD_VALUE;
      }
      binder::Status status = StartProfilingProtobufFd(fd);
      if (status.isOk()) {
        return OK;
      } else {
        err_str << status.toString8() << std::endl;
        return UNKNOWN_ERROR;
      }
    } else if (args[0] == String16("stopProfiling")) {
      Status status = stopProfiling();
      if (status.isOk()) {
        return OK;
      } else {
        err_str << status.toString8() << std::endl;
        return UNKNOWN_ERROR;
      }
    }
  }
  return BAD_VALUE;
}

status_t PerfProfdNativeService::onTransact(uint32_t _aidl_code,
                                            const Parcel& _aidl_data,
                                            Parcel* _aidl_reply,
                                            uint32_t _aidl_flags) {
  switch (_aidl_code) {
    case IBinder::SHELL_COMMAND_TRANSACTION: {
      int in = _aidl_data.readFileDescriptor();
      int out = _aidl_data.readFileDescriptor();
      int err = _aidl_data.readFileDescriptor();
      int argc = _aidl_data.readInt32();
      Vector<String16> args;
      for (int i = 0; i < argc && _aidl_data.dataAvail() > 0; i++) {
        args.add(_aidl_data.readString16());
      }
      sp<IBinder> unusedCallback;
      sp<IResultReceiver> resultReceiver;
      status_t status;
      if ((status = _aidl_data.readNullableStrongBinder(&unusedCallback)) != OK)
        return status;
      if ((status = _aidl_data.readNullableStrongBinder(&resultReceiver)) != OK)
        return status;
      status = shellCommand(in, out, err, args);
      if (resultReceiver != nullptr) {
        resultReceiver->send(status);
      }
      return OK;
    }

    default:
      return ::android::os::BnPerfProfd::onTransact(
          _aidl_code, _aidl_data, _aidl_reply, _aidl_flags);
  }
}

}  // namespace

int Main() {
  {
    struct DummyConfig : public Config {
      void Sleep(size_t seconds) override {}
      bool IsProfilingEnabled() const override { return false; }
    };
    DummyConfig config;
    GlobalInit(config.perf_path);
  }

  android::status_t ret;
  if ((ret = PerfProfdNativeService::start()) != android::OK) {
    LOG(ERROR) << "Unable to start InstalldNativeService: %d" << ret;
    exit(1);
  }

  android::IPCThreadState::self()->joinThreadPool();

  LOG(INFO) << "Exiting perfprofd";
  return 0;
}

}  // namespace binder
}  // namespace perfprofd
}  // namespace android
