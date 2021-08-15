// Copyright (C) 2018 The Android Open Source Project
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

//#undef NDEBUG // get DCHECK etc.


#include "common/debug.h"
#include "common/expected.h"
#include "perfetto/rx_producer.h"

#include <android-base/unique_fd.h>
#include <android-base/parseint.h>
#include <android-base/file.h>

#include "rxcpp/rx.hpp"
#include <iostream>
#include <optional>

#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syscall.h>
#include <fcntl.h>
#include <unistd.h>

using namespace iorap::perfetto;  // NOLINT

#if defined(IORAP_PERFETTO_MAIN)

void Usage(char** argv) {
  std::cerr << "Usage: " << argv[0] << " [--config-proto=config.pb] [--duration-ms=5000] [--output-proto=output.pb]" << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "  Request a perfetto trace, blocking until it's complete. The resulting trace proto" << std::endl;
  std::cerr << "  is output to stdout as text, or to --output-proto as a binary." << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "  Optional flags:" << std::endl;
  std::cerr << "    --help,-h                  Print this Usage." << std::endl;
  std::cerr << "    --output-proto $,-op $     Perfetto tracebuffer output file (default stdout)." << std::endl;
  std::cerr << "    --config-proto $,-cp $     Path to binary protobuf config." << std::endl;
  std::cerr << "    --duration-ms $,-dm $      How long to run trace for in milliseconds." << std::endl;
  std::cerr << "    --simple                   Simplest possible perfetto state transitions (default off)." << std::endl;
  std::cerr << "    --verbose,-v               Set verbosity (default off)." << std::endl;
  std::cerr << "    --wait,-w                  Wait for key stroke before continuing (default off)." << std::endl;
  exit(1);
}

PerfettoDependencies::Component CreateCommandLinePerfettoDependenciesComponent(
    uint32_t duration_ms) {
  // TODO: read from command line.
  static const uint32_t kBufferSize = 4096;

  // TODO: remove this hack.
  static const uint32_t kTraceDurationMs = duration_ms;

  // fruit: using 'bindInstance' causes a segfault every time.
#if 0

  // fruit: Can't use a stateful lambda, so use bindInstance instead of registerProvider.
  auto config = PerfettoDependencies::CreateConfig(duration_ms,
                                                   /*deferred_start*/true,
                                                   kBufferSize);

  .... bindInstance(config);
#endif

  return fruit::createComponent()
    .bind<PerfettoConsumer, PerfettoConsumerImpl>()
    .registerProvider([]() /* -> TraceConfig */ {
        return PerfettoDependencies::CreateConfig(kTraceDurationMs,
                                                  /*deferred_start*/true,
                                                  kBufferSize);
    });
}

static void CollectPerfettoTraceBufferViaAbstractions(
    RxProducerFactory& producer_factory,
    const std::string& arg_output_proto,
    const int arg_duration_ms) {
  LOG(VERBOSE) << "CollectPerfettoTraceBufferViaAbstractions";

  // Don't create a subscriber to emit the PerfettoStreamCommand.
  // RxCpp is "greedy" and consumes every possible item emitted (it doesn't support 'pull'). We want
  // to operate on a (command,state) iteration every time, just like in a real scenario.
  // Adding the 'interval' turns into a non-greedy version (i.e. push).

  // Immediately emit 'kStartTracing', wait and emit kStopTracing, wait and emit kShutdown.
  // In reality, there would be a delay between all these events.
  auto /*observable<PerfettoStreamCommand>*/ commands =
      rxcpp::observable<>::just(PerfettoStreamCommand::kStartTracing)
          // wait 1x
          .concat(
              // Pick a value longer than the perfetto config delay_ms, so that we send
              // 'kShutdown' after tracing has already finished.
              rxcpp::observable<>::interval(std::chrono::milliseconds(arg_duration_ms * 2))
                  .take(2)  // kStopTracing, kShutdown.
                  .map([](int value) {
                         // value is 1,2,3,...
                         return static_cast<PerfettoStreamCommand>(value);  // 1,2, ...
                       })
          );

  auto /*observable<PerfettoTraceProto>*/ trace_proto_stream =
      producer_factory.CreateTraceStream(commands);

  trace_proto_stream
    .observe_on(ObserveOnNewIoThread())  // Write data on an idle-class-priority thread.
    .as_blocking()  // Wait for observable to terminate with on_completed or on_error.
    .subscribe(/*on_next*/[arg_output_proto]
      (PerfettoTraceProto trace_proto) {
             if (!trace_proto.WriteFullyToFile(arg_output_proto)) {
               LOG(ERROR) << "Failed to save TraceBuffer to " << arg_output_proto;
             } else {
               LOG(INFO) << "TraceBuffer saved to file: " << arg_output_proto;
               LOG(INFO);
               LOG(INFO) << "To print this in a human readable form, execute these commands:";
               LOG(INFO) << "$> adb pull '" << arg_output_proto << "'";
               LOG(INFO) << "$> trace_to_text systrace <filename.pb>";
             }
      },
      /*on_error*/[](rxcpp::util::error_ptr err) {
        LOG(ERROR) << "Perfetto trace proto collection error: " << rxcpp::util::what(err);
      });
}

namespace iorap::perfetto {
// Reach inside rx_producer.cc
// Not part of any headers because it's internal.
void CollectPerfettoTraceBufferImmediately(
    RxProducerFactory& producer_factory,
    const std::string& arg_output_proto);
}

int main(int argc, char** argv) {
  android::base::InitLogging(argv);
  android::base::SetLogger(android::base::StderrLogger);

  bool wait_for_keystroke = false;
  bool enable_verbose = false;

  std::string arg_output_proto;
  std::string arg_config_proto;
  uint32_t arg_duration_ms = 1000;
  bool arg_simple = false;

  if (argc == 1) {
    Usage(argv);
  }

  for (int arg = 1; arg < argc; ++arg) {
    std::string argstr = argv[arg];
    bool has_arg_next = (arg+1)<argc;
    std::string arg_next = has_arg_next ? argv[arg+1] : "";

    if (argstr == "--help" || argstr == "-h") {
      Usage(argv);
    } else if (argstr == "--output-proto" || argstr == "-op") {
      if (!has_arg_next) {
        std::cerr << "Missing --output-proto <value>" << std::endl;
        return 1;
      }
      arg_output_proto = arg_next;
      ++arg;
    } else if (argstr == "--config-proto" || argstr == "-cp") {
      if (!has_arg_next) {
        std::cerr << "Missing --config-proto <value>" << std::endl;
        return 1;
      }
      arg_config_proto = arg_next;
      LOG(WARNING) << "TODO: parse configs from a file, not implemented yet.";
      ++arg;
    } else if (argstr == "--duration-ms" || argstr == "-dm") {
      if (!has_arg_next) {
        std::cerr << "Missing --duration-ms <value>" << std::endl;
        return 1;
      }
      if (!android::base::ParseUint(arg_next.c_str(), /*out*/&arg_duration_ms)) {
        std::cerr << "Invalid --duration-ms " << arg_next << ", reason: " << strerror(errno);
        return 1;
      }
      ++arg;
    } else if (argstr == "--simple") {
      arg_simple = true;
    } else if (argstr == "--verbose" || argstr == "-v") {
      enable_verbose = true;
    } else if (argstr == "--wait" || argstr == "-w") {
      wait_for_keystroke = true;
    }
  }

  if (enable_verbose) {
    android::base::SetMinimumLogSeverity(android::base::VERBOSE);

    LOG(VERBOSE) << "Verbose check";
    LOG(VERBOSE) << "Debug check: " << ::iorap::kIsDebugBuild;
  }

  // Useful to attach a debugger...
  // 1) $> iorap-cmd-perfetto -w <args>
  // 2) $> gdbclient <pid>
  if (wait_for_keystroke) {
    LOG(INFO) << "Self pid: " << getpid();
    LOG(INFO) << "Press any key to continue...";
    std::cin >> wait_for_keystroke;
  }

  int return_code = 0;
  // TODO: convert #on-error into a non-0 return code.

  PerfettoDependencies::Injector injector{
      CreateCommandLinePerfettoDependenciesComponent,
      arg_duration_ms
  };
  RxProducerFactory rx_producer_factory{/*borrow*/injector};

  if (arg_simple) {
    // To debug any kind of low-level perfetto issues.
    CollectPerfettoTraceBufferImmediately(/*inout*/rx_producer_factory, arg_output_proto);
  } else {
    // To debug our own iorap internal abstractions.
    CollectPerfettoTraceBufferViaAbstractions(/*inout*/rx_producer_factory,
                                              arg_output_proto,
                                              arg_duration_ms);
  }

  // Uncomment this if we want to leave the process around to inspect it from adb shell.
  // sleep(100000);

  // 0 -> successfully wrote the TraceProto out to file.
  // 1 -> failed along the way (#on_error and also see the error logs).
  return return_code;
}

#endif
