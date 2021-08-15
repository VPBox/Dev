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

#ifndef IORAP_SRC_PERFETTO_PERFETTO_CONSUMER_H_
#define IORAP_SRC_PERFETTO_PERFETTO_CONSUMER_H_

#include <fruit/fruit.h>
#include <perfetto/public/consumer_api.h>  // libperfetto

namespace iorap::perfetto {

// Abstract out the Perfetto C API behind a virtual interface:
// This enables us to use dependency injection to provide mock implementations
// during tests.
struct PerfettoConsumer {
  // 1:1 aliasing of type definitions and constants in perfetto/public/consumer_api.h
  // Refer to the documentation there.
  using State = ::perfetto::consumer::State;
  using Handle = ::perfetto::consumer::Handle;
  static constexpr Handle kInvalidHandle = ::perfetto::consumer::kInvalidHandle;
  using OnStateChangedCb = ::perfetto::consumer::OnStateChangedCb;
  using TraceBuffer = ::perfetto::consumer::TraceBuffer;

  // 1:1 forwarding of C-style functions in perfetto/public/consumer_api.h
  // Refer to the documentation there.

  virtual Handle Create(const void* config_proto,
                        size_t config_len,
                        OnStateChangedCb callback,
                        void* callback_arg) = 0;
  virtual void StartTracing(Handle) = 0;
  virtual TraceBuffer ReadTrace(Handle) = 0;
  virtual void Destroy(Handle) = 0;
  virtual State PollState(Handle) = 0;

  virtual ~PerfettoConsumer() {}
};

// "Live" implementation that calls down to libperfetto.
struct PerfettoConsumerImpl : public PerfettoConsumer {
  // Marks this constructor as the one to use for injection.
  INJECT(PerfettoConsumerImpl()) = default;

  virtual Handle Create(const void* config_proto,
                        size_t config_len,
                        OnStateChangedCb callback,
                        void* callback_arg) override {
    return ::perfetto::consumer::Create(config_proto,
                                      config_len,
                                      callback,
                                      callback_arg);
  }

  virtual void StartTracing(Handle handle) override {
    ::perfetto::consumer::StartTracing(handle);
  }

  virtual TraceBuffer ReadTrace(Handle handle) override {
    return ::perfetto::consumer::ReadTrace(handle);
  }

  virtual void Destroy(Handle handle) override {
    ::perfetto::consumer::Destroy(handle);
  }
  virtual State PollState(Handle handle) override {
    return ::perfetto::consumer::PollState(handle);
  }

  virtual ~PerfettoConsumerImpl() {}
};

}  // namespace iorap::perfetto

#endif  // IORAP_SRC_PERFETTO_PERFETTO_CONSUMER_H_

