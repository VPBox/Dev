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

#include "common/debug.h"
#include "common/expected.h"
#include "perfetto/rx_producer.h"

#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/unique_fd.h>

#include <iostream>

#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syscall.h>
#include <fcntl.h>
#include <unistd.h>

// TODO: move to perfetto code
namespace perfetto {
namespace consumer {

std::ostream& operator<<(std::ostream& os, State state) {
  switch (state) {
    case State::kTraceFailed:
      os << "kTraceFailed";
      break;
    case State::kConnectionError:
      os << "kConnectionError";
      break;
    case State::kSessionNotFound:
      os << "kSessionNotFound";
      break;
    case State::kIdle:
      os << "kIdle";
      break;
    case State::kConnecting:
      os << "kConnecting";
      break;
    case State::kConfigured:
      os << "kConfigured";
      break;
    case State::kTracing:
      os << "kTracing";
      break;
    case State::kTraceEnded:
      os << "kTraceEnded";
      break;
    default:
      os << "(unknown)";  // did someone forget to update this code?
      break;
  }
  return os;
}

}  // namespace consumer
}  // namespace perfetto

namespace iorap::perfetto {

PerfettoDependencies::Component PerfettoDependencies::CreateComponent() {
  // TODO: read from config.
  static const uint32_t kTraceDurationMs =
      ::android::base::GetUintProperty("iorapd.perfetto.trace_duration_ms", /*default*/5000U);

  static const uint32_t kBufferSize =
      ::android::base::GetUintProperty("iorapd.perfetto.buffer_size", /*default*/4096U);

  return fruit::createComponent()
    .bind<PerfettoConsumer, PerfettoConsumerImpl>()
    .registerProvider([]() /* -> TraceConfig */ {
        return CreateConfig(kTraceDurationMs,
                            /*deferred_start*/true,
                            kBufferSize);
    });
}

::perfetto::protos::TraceConfig PerfettoDependencies::CreateConfig(uint32_t duration_ms,
                                                                   bool deferred_start,
                                                                   uint32_t buffer_size) {
  ::perfetto::protos::TraceConfig trace_config;

  trace_config.set_duration_ms(duration_ms);
  trace_config.add_buffers()->set_size_kb(buffer_size);
  trace_config.set_deferred_start(deferred_start);

  auto* ds_config = trace_config.add_data_sources()->mutable_config();
  ds_config->set_name("linux.ftrace");
  ds_config->mutable_ftrace_config()->add_ftrace_events(
      "mm_filemap_add_to_page_cache");
  ds_config->mutable_ftrace_config()->add_ftrace_events(
      "mm_filemap_delete_from_page_cache");
  ds_config->set_target_buffer(0);

  return trace_config;
}

// RAII-style wrapper around a perfetto handle that calls Destroy
// in a thread-safe manner.
struct PerfettoConsumerHandle {
 private:
  std::shared_ptr<PerfettoConsumer> consumer_;
  PerfettoConsumer::Handle handle_;

 public:
  // Takes over ownership of the 'handle'.
  //
  // Consumer must not be null.
  PerfettoConsumerHandle(std::shared_ptr<PerfettoConsumer> consumer,
                         PerfettoConsumer::Handle handle)
    : consumer_{std::move(consumer)},
      handle_{std::move(handle)} {
    DCHECK(consumer_ != nullptr);
  }

  std::shared_ptr<PerfettoConsumer> GetConsumer() const {
    return consumer_;
  }

  PerfettoConsumer::Handle GetHandle() const {
    return handle_;
  }

  ~PerfettoConsumerHandle() {
    LOG(VERBOSE) << "PerfettoConsumerHandle::Destroy(" << handle_ << ")";
    consumer_->Destroy(handle_);
  }

  bool operator==(const PerfettoConsumerHandle& other) const {
    return handle_ == other.handle_ && consumer_ == other.consumer_;
  }

  bool operator!=(const PerfettoConsumerHandle& other) const {
    return !(*this == other);
  }
};


// Snapshot of a single perfetto OnStateChanged callback.
//
// Operate on the PerfettoConsumer to further change the state.
//
// The Handle is kept 'valid' until all references to the PerfettoConsumerHandle
// are dropped to 0. This ensures the Handle is not destroyed too early. All
// direct usages of 'Handle' must be scoped by the PerfettoConsumerHandle.
struct PerfettoStateChange {
 public:
  using State = ::perfetto::consumer::State;
  using Handle = ::perfetto::consumer::Handle;

  State state;                                                           // Never invalid.
  std::shared_ptr<PerfettoConsumerHandle> perfetto_consumer_and_handle;  // Never null.

  // Safety: Use only within scope of the PerfettoStateChange.
  Handle GetHandle() const {
    // TODO: it would be even safer to wrap all the calls to the handle inside a class,
    // instead of exposing this raw Handle.
    return perfetto_consumer_and_handle->GetHandle();
  }

  std::shared_ptr<PerfettoConsumer> GetConsumer() const {
    return perfetto_consumer_and_handle->GetConsumer();
  }
};

std::ostream& operator<<(std::ostream& os, const PerfettoStateChange& state_change) {
  os << "PerfettoStateChange{" << state_change.state << ","
     << state_change.GetHandle() << ","
     << state_change.GetConsumer().get() << "}";
  return os;
}

// Once created, this acts as a hot observable, emitting 'PerfettoStateChange' transition items.
// Only the 'state' will vary, the handle and perfetto_consumer are always the same value.
//
// Clients only need to handle the success states in #on_next, all failure states will go to
// #on_error.
//
// Upon reaching the appropriate terminal states, either #on_completed or #on_error is called.
// No future callbacks will then occur, so this object should be subsequently deleted.
//
// The Handle is destroyed automatically after the last item is emitted, so it must only be
// manipulated from the #on_next callbacks. Do not save the Handle and use it at other times.
class StateChangedSubject {
 public:
  using State = ::perfetto::consumer::State;
  using Handle = ::perfetto::consumer::Handle;

  StateChangedSubject(const ::perfetto::protos::TraceConfig& trace_config,
                      rxcpp::subscriber<PerfettoStateChange> destination,
                      std::shared_ptr<PerfettoConsumer> perfetto_consumer)
    : deferred_start(trace_config.deferred_start()),
      dest(std::move(destination)),
      perfetto_consumer_(std::move(perfetto_consumer)) {
    DCHECK(perfetto_consumer_ != nullptr);
  }

 private:
  struct StateChangedError : public std::runtime_error {
    explicit StateChangedError(const std::string& what_arg) : std::runtime_error(what_arg) {}
  };

  std::shared_ptr<PerfettoConsumerHandle> handle_;  // non-null after bound_ == true.
  std::atomic<bool> bound_{false};  // synchronize-with for BindHandle -> OnStateChanged.

  State last_state{State::kIdle};
  bool deferred_start{false};

  rxcpp::subscriber<PerfettoStateChange> dest;
  std::shared_ptr<PerfettoConsumer> perfetto_consumer_;  // This is never null.

  void DcheckBadStateTransition(State state, bool fail_unless = false) const {
    DCHECK(fail_unless) << "Invalid state transition to " << state << " from " << last_state;
  }

  void DcheckValidStateTransition(State state) {
    // State must not be out of range.
    DCHECK_GE(state, State::kTraceFailed);
    DCHECK_LE(state, State::kTraceEnded);

    // Internal state that should never leak out into public perfetto API:
    DCHECK_NE(state, State::kIdle);
    // These can only be returned by PollState:
    DCHECK_NE(state, State::kSessionNotFound);

    // Validate state transitions as per the perfetto API contract.
    // See the 'state diagram' in consumer_api.h
    switch (last_state) {
      case State::kTraceFailed:  // Final and unrecoverable.
        // b/122548195: this can transition to 'kConnectionError' if selinux is disabled.
        if (state == State::kConnectionError) {
          LOG(WARNING) << "b/122548195: kTraceFailed is non-terminal, ignoring.";
          // This is a bit awkward: rxcpp will drop the #on_error calls if its more than once.
          break;
        }
        DcheckBadStateTransition(state);
        break;
      case State::kConnectionError:  // Final and unrecoverable.
        DcheckBadStateTransition(state);
        break;
      case State::kSessionNotFound:
        DcheckBadStateTransition(state);
        break;
      case State::kIdle:
        // OK: we initialized our own state to idle prior to the first callback.
        break;
      case State::kConnecting:
        switch (state) {
          case State::kConfigured:
            // kConfigured, if |deferred_start| == true in the trace config.
            DcheckBadStateTransition(state, deferred_start);
            break;
          case State::kTracing:
            // kTracing, if |deferred_start| == false.
            DcheckBadStateTransition(state, !deferred_start);
            break;
          case State::kConnectionError:
            // An error state, e.g. if cannot reach the traced daemon.
            break;
          default:
            // Unconditionally invalid state transitions from kConnecting to anything else.
            DcheckBadStateTransition(state);
        }
        break;
      case State::kConfigured:
        DCHECK(deferred_start);
        if (state != State::kTracing  // OK: this is documented.
            && state != State::kTraceFailed) {  // Undocumented selinux failure.
            // Undocumented, but it appears to go directly from Configured->TraceEnded
            // it can also go to kTraceFailed if e.g. there's an selinux violation
            // however this appears to be underdocumented.
            // b/122607276 #2

          if (state != State::kTraceEnded) {  // b/122607276 #1
            DcheckBadStateTransition(state);
          }
        }
        break;
      case State::kTracing:
        switch (state) {
          case State::kTraceEnded:
            break;
          case State::kTraceFailed:
            break;
          default:
            DcheckBadStateTransition(state);
        }
        break;
      case State::kTraceEnded:
        // Cannot transition from terminal state to another state.
        DcheckBadStateTransition(state);
        break;

      // default: This list is exhaustive
    }
  }

  constexpr bool IsTerminalState() const {
    switch (last_state) {
      case State::kTraceFailed:
      case State::kConnectionError:
      case State::kTraceEnded:
        return true;
      default:
        return false;
    }
  }

  // Returns true for non-terminal states (i.e. this callback will be invoked again).
  // Returns false otherwise.
  bool OnStateChanged(Handle handle, State state) {
    using namespace ::perfetto::consumer;

    // Block until 'BoundHandle' is called by the other thread.
    while (!bound_.load()) {}  // seq_cst acquire.

    std::shared_ptr<PerfettoConsumerHandle> handle_ptr = handle_;
    DCHECK(handle_ptr != nullptr);

    DCHECK_EQ(handle_ptr->GetHandle(), handle);
    DcheckValidStateTransition(state);

    switch (state) {
      // Error states (terminal).
      case State::kTraceFailed:
        EmitError("kTraceFailed");
        break;
      case State::kConnectionError:
        EmitError("kConnectionError");
        break;

      // Regular transitions (non-terminal).
      case State::kConnecting:
      case State::kConfigured:
      case State::kTracing:
        EmitNext(state);
        break;
      // Regular transitions (terminal).
      case State::kTraceEnded:  // XX: do we even need to emit the 'TraceEnded' state?
        EmitNext(state);
        dest.on_completed();
        break;
      default:
        DcheckBadStateTransition(state);
    }

    bool force_non_terminal = false;

    if (last_state == State::kConfigured &&  state == State::kConnectionError) {
      // b/122548195: this can transition to 'kConnectionError' if selinux is disabled.
      force_non_terminal = true;
      // This function must 'return true' in this buggy case, otherwise we will
      // call the destructor too early and subsequent callbacks will crash.
    }

    // Remember the state to validate prior state transitions.
    last_state = state;

    // The owner of this class should avoid leaking memory once we reach a terminal state.
    return !IsTerminalState() || force_non_terminal;
  }

 public:
  // Thread safety: Called by main thread, terminates the rx stream.
  // When this function is invoked, no calls to this class from other threads can occur.
  void OnCreateFailed() {
    // returned when an invalid handle is passed to PollState().
    last_state = State::kSessionNotFound;
    EmitError("Create returned kInvalidHandle");
  }

  // Thread safety: Called by main thread, this could be concurrent to
  // 'CallbackOnStateChanged'.
  void BindHandle(const std::shared_ptr<PerfettoConsumerHandle>& handle) {
    handle_ = handle;

    // Unblock OnStateChanged.
    bound_.store(true);  // seq_cst release.
  }

  // Thread safety: Called by libperfetto background thread (same one every time).
  static void CallbackOnStateChanged(Handle handle, State state, void* callback_arg) {
    LOG(VERBOSE) << "CallbackOnStateChanged(handle=" << handle << ",state=" << state
                 << ",callback_arg=" << callback_arg << ")";

    // Validate OnStateChanged callback invariants, guaranteed by libperfetto.
    DCHECK_NE(handle, ::perfetto::consumer::kInvalidHandle);

    // Note: Perfetto guarantees this callback always occurs on the same thread,
    // so we don't need to do any extra thread synchronization here since we are only mutating
    // StateChangedSubject from within this function.

    // TODO: the memory ordering guarantees should be explicitly specified in consumer_api.h:
    // This isn't specific enough:
    // "The callback will be invoked on an internal thread and must not block."
    // However looking at the implementation it posts onto a single-thread task runner,
    // so this must be the case.

    StateChangedSubject* state_subject = reinterpret_cast<StateChangedSubject*>(callback_arg);
    // This current thread owns 'StateChangedSubject', no other threads must access it.
    // Explicit synchronization is not necessary.

    if (!state_subject->OnStateChanged(handle, state)) {
      // Clean up the state tracker when we reach a terminal state.
      // This means that no future callbacks will occur anymore.
      delete state_subject;
    }
  }

 private:
  void EmitError(const std::string& msg) {
    // Sidenote: Exact error class does not matter, rxcpp only lets us access the error
    // as a string (rxcpp::util::what).
    //
    // Either way, the recovery strategy is identical (log then try and restart).
    dest.on_error(rxcpp::util::make_error_ptr(StateChangedError{msg}));
  }

  void EmitNext(State state) {
    if (WOULD_LOG(VERBOSE) && !dest.is_subscribed()) {
      // This is purely for logging: #on_next already filters out items after unsubscription.
      LOG(VERBOSE) << "StateChangedSubject#EmitNext(" << state << ") - drop due to unsubscribe";
    }

    auto handle_ptr = handle_;
    DCHECK(handle_ptr != nullptr);

    // Non-null guarantee for the items emitted into this stream.
    PerfettoStateChange state_change{state, handle_ptr};
    dest.on_next(std::move(state_change));
  }

  // TODO: inherit from rx subject and handle #unsubscribe explicitly, instead
  // of just being subject-like?
};

// Note: The states will be emitted on a separate thread, so e.g. #as_blocking()
// needs to be used to avoid dropping everything on the floor.
//
// Important: The #on_error case must be handled explicitly by the observable,
// because the default behavior is to 'throw' which will cause an std::terminate with -fno-except.
static auto /*[observable<State>, shared_ptr<PerfettoConsumerHandle>]*/
    CreatePerfettoStateStream(::perfetto::protos::TraceConfig perfetto_config,
                              std::shared_ptr<PerfettoConsumer> perfetto_consumer) {
  auto obs = rxcpp::observable<>::create<PerfettoStateChange>(
    [perfetto_config = std::move(perfetto_config), perfetto_consumer = std::move(perfetto_consumer)]
        (rxcpp::subscriber<PerfettoStateChange> subscriber) {
      std::unique_ptr<StateChangedSubject> state_subject{
          new StateChangedSubject{perfetto_config, subscriber, perfetto_consumer}};

      // Perfetto API requires a pointer to a serialized protobuf, it doesn't accept
      // the code-generated object.
      std::string perfetto_config_str = perfetto_config.SerializeAsString();

      ::perfetto::consumer::Handle handle =
          perfetto_consumer->Create(perfetto_config_str.data(),
                                    perfetto_config_str.size(),
                                    // executes on the same background thread repeatedly.
                                    &StateChangedSubject::CallbackOnStateChanged,
                                    // inter-thread-move
                                    reinterpret_cast<void*>(state_subject.get()));
      // perfetto::consumer::Create synchronizes-with OnStateChanged callback, this means
      // we don't need to explicitly synchronize state_subject here so long as we don't access
      // it on this thread again.
      LOG(DEBUG) << "Create Perfetto handle " << handle;

      if (handle == ::perfetto::consumer::kInvalidHandle) {
        LOG(ERROR) << "Failed to create Perfetto handle";
        // No callbacks will occur, so our thread still owns the state subject.
        state_subject->OnCreateFailed();
        return;
      }

      std::shared_ptr<PerfettoConsumerHandle> safe_handle{
          new PerfettoConsumerHandle{perfetto_consumer, handle}};

      // Share ownership of the Handle with the StateSubject.
      // This way we defer calling 'Destroy' until the callback reaches a terminal state
      // *and* all users of the stream are done with the handle.
      state_subject->BindHandle(safe_handle);

      // state_subject ownership is taken over by OnStateChanged.
      // It will also be touched in a separate thread, so we must never access it here again.
      state_subject.release();

      // 'subscriber#add' is actually a call to register an on_unsubscribe listener.
      subscriber.add([safe_handle]() {
        LOG(VERBOSE) << "PerfettoStateChange#unsubscribe";

        // Release our ref-count to the handle.
        // safe_handle.reset();  // This happens implicitly.

        // TODO: I think this won't handle the case where we need to shut down early.
        // Need to use the explicit kShutdown for that?
      });

      // TODO: this would be an excellent place to shuffle the perfetto config protobuf
      // into a global debug state for dumpsys.
    });

  return obs;
}

template <typename T>
bool BinaryWireProtobuf<T>::WriteFullyToFile(const std::string& path,
                                             bool follow_symlinks) const {
  // TODO: it would be great if android::base had a string_view overload to avoid copying
  // data into an std::string.

  // u  g  o
  // rw-rw----
  //
  // Protobufs can be read/written but not executed.
  static constexpr const mode_t kMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

  int flags =
      O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC | O_BINARY | (follow_symlinks ? 0 : O_NOFOLLOW);
  android::base::unique_fd fd(TEMP_FAILURE_RETRY(open(path.c_str(), flags, kMode)));

  if (fd == -1) {
    PLOG(ERROR) << "BinaryWireProtobuf::WriteFullyToFile open failed";
    return false;
  }

  if (!::android::base::WriteFully(fd, data_.data(), size())) {
    PLOG(ERROR) << "BinaryWireProtobuf::WriteFullyToFile write failed";
    return CleanUpAfterFailedWrite(path);
  }

  return true;
}

template <typename T>
bool BinaryWireProtobuf<T>::CleanUpAfterFailedWrite(const std::string& path) {
  // Something went wrong. Let's not leave a corrupt file lying around.
  int saved_errno = errno;
  unlink(path.c_str());
  errno = saved_errno;
  return false;
}

template <typename T>
bool BinaryWireProtobuf<T>::WriteStringToFd(int fd) const {
  const char* p = reinterpret_cast<const char*>(data_.data());
  size_t left = size();
  while (left > 0) {
    ssize_t n = TEMP_FAILURE_RETRY(write(fd, p, left));
    if (n == -1) {
      return false;
    }
    p += n;
    left -= n;
  }
  return true;
}

// explicit template instantiation.
template struct BinaryWireProtobuf<::google::protobuf::MessageLite>;
// TODO: refactor this not to need the template instantiation.

#if defined(__ANDROID__)
// Copy of the 2.6.18 kernel header (linux/ioprio.h)

#define IOPRIO_WHO_PROCESS (1)
#define IOPRIO_CLASS_IDLE (3)

#define IOPRIO_BITS		(16)
#define IOPRIO_CLASS_SHIFT	(13)
#define IOPRIO_PRIO_MASK	((1UL << IOPRIO_CLASS_SHIFT) - 1)

#define IOPRIO_PRIO_CLASS(mask)	((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask)	((mask) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class, data)	(((class) << IOPRIO_CLASS_SHIFT) | data)
#endif

static int ioprio_get(int which, int who) {
  return syscall(SYS_ioprio_get, which, who);
}

static int ioprio_set(int which, int who, int ioprio) {
  return syscall(SYS_ioprio_set, which, who, ioprio);
}

// An rx Coordination, which will cause a new thread to spawn for each new Worker.
//
// Idle-class priority is set for the CPU and IO priorities on the new thread.
rxcpp::observe_on_one_worker ObserveOnNewIoThread() {
  // IO thread factory for idle-priority threads.
  // Both the CPU scheduler and the IO scheduler are set to idle.
  //
  // Use this when needing to schedule disk access from a normal-priority thread onto a
  // very low priority thread, but not so low that we need to use a BackgroundJobScheduler.
  struct io_thread_factory {
    std::thread operator()(std::function<void()> start) const {
      return std::thread{
        [start=std::move(start)]() {
          // Set IO priority to idle.
          do {
            int value = ioprio_get(IOPRIO_WHO_PROCESS, /*pid*/0);
            if (value == -1) {
              PLOG(ERROR) << "io_thread_factory failed ioprio_get";
              break;  // Can't set the ioprio, we don't know what data to use.
            }

            int data = IOPRIO_PRIO_DATA(value); // priority level
            // This appears to be '4' in practice. We may want to raise to
            // be the highest-priority within the idle class.

            // idle scheduling class. only access disk when nobody else needs disk.
            int res = ioprio_set(IOPRIO_WHO_PROCESS,
                                 /*pid*/0,
                                 IOPRIO_PRIO_VALUE(IOPRIO_CLASS_IDLE, data));
            if (res < 0) {
              PLOG(ERROR) << "io_thread_factory failed ioprio_set";
              break;
            }

            // Changing the IO priority only has any effect with cfq scheduler:
            // $> cat /sys/block/sda/queue/scheduler
            LOG(VERBOSE) << "ioprio_set(WHO_PROCESS, class=IDLE, data=" << data << ")";
          } while (false);

          // Set CPU priority to idle.
          do {
            struct sched_param param{};
            param.sched_priority = 0;  // Required to be statically 0 when used with SCHED_IDLE.

            if (sched_setscheduler(/*pid*/0,  // current thread,
                                   SCHED_IDLE,
                                   /*in*/&param) != 0) {
              PLOG(ERROR) << "io_thread_factory failed sched_setscheduler";
              break;
            }

            LOG(VERBOSE) << "sched_setscheduler(self, IDLE)";
          } while (false);

          // XX: if changing the scheduling is too aggressive (i.e. it causes starvation),
          // we may want to stick with the default class and change the nice (priority) levels
          // to the minimum.

          // TODO: future work, maybe use cgroups configuration file instead?

          // Call the rxcpp-supplied code.
          start();
        }
      };
    }
  };

  static rxcpp::schedulers::scheduler thread_scheduler =
      rxcpp::schedulers::make_new_thread(io_thread_factory{});

  static rxcpp::observe_on_one_worker observe_on_io_thread{thread_scheduler};

  return observe_on_io_thread;
}

static auto/*observable<PerfettoTraceProto>*/
    CreatePerfettoStream(rxcpp::observable<PerfettoStreamCommand> input,
                         std::shared_ptr<PerfettoConsumer> perfetto_consumer,
                         const ::perfetto::protos::TraceConfig& trace_config) {
        // XX: should I also take a scheduler for input here???

  auto /*observable<PerfettoStateChange>*/ perfetto_states =
    CreatePerfettoStateStream(trace_config, perfetto_consumer);

  using State = ::perfetto::consumer::State;

  auto/*coordinator*/ serialize_coordinator = rxcpp::observe_on_new_thread();
  // Rx note:
  // The optimal thing to do would be to have a lock/unlock for an entire subset of a chain.
  // This would avoid creating new threads, and could also be used to intentionally block
  // the regular C-callback perfetto thread.
  //
  // It seems possible to create a coordinator to lock a single operator in a chain, but this
  // appears to be unsound. In particular, it doesn't even make life any simpler below because
  // it would only apply the synchronization to 'zip' but not 'flat_map' which is unsound.
  //
  // There is also the built-in 'serialize_new_thread' which seems to create a new thread but
  // then never actually uses it, that seems unfortunate and wasteful.
  //
  // Instead, do the simple thing which is create a new thread and always queue on there.
  // Execution an action on that worker is itself unsynchronized, but this doesn't matter since
  // the worker is only backed by 1 thread (no 2 schedulables can be executed concurrently
  // on the 'observe_new_thread' worker).
  return input
    .tap([](PerfettoStreamCommand command) {
           LOG(VERBOSE) << "CreatePerfettoStreamCommand#tap(command=" << command << ")";
         })
    // Input A, thread tA. Input B, thread tB. Continue execution with (A,B) on thread tC.
    .zip(serialize_coordinator,  // rest of chain is also executed on the same thread.
         perfetto_states)
    // Note: zip terminates when either of the streams complete.
    .flat_map(
         [](std::tuple<PerfettoStreamCommand, PerfettoStateChange> p) {
           auto& [command, state_change] = p;
           LOG(VERBOSE) << "CreatePerfettoStream#combine("
                        << command << "," << state_change << ")";
           if (command == PerfettoStreamCommand::kShutdown) {
             // Perfetto: Always safe to call ::perfetto::consumer::Destroy
             // at any time.
             //
             // XX: How do we clean up the StateChangedSubject without racing
             // against the callback? It strikes me that we may need a 'kDestroyed'
             // state that perfetto can transition to from kConfigured.
             LOG(VERBOSE) << "Call Perfetto_Consumer->Destroy";
             state_change.GetConsumer()->Destroy(state_change.GetHandle());

             // XX: Do we even have any guarantees about not getting more callbacks?
             // We could just say 'there can still be spurious output after Shutdown'
             // and just ignore it (e.g. Shutdown and immediately unsubscribe).
           } else if (command == PerfettoStreamCommand::kStartTracing
                          && state_change.state == State::kConfigured) {
             LOG(VERBOSE) << "Call Perfetto_Consumer->StartTracing";
             state_change.GetConsumer()->StartTracing(state_change.GetHandle());
           } else if (command == PerfettoStreamCommand::kStopTracing &&
                          state_change.state == State::kTraceEnded) {
             // TODO: if perfetto actually had a 'StopTracing' we could call that here.
             // right now we just pretend it exists, but rely on the config timer instead.
             ::perfetto::consumer::TraceBuffer trace_buffer =
                 state_change.GetConsumer()->ReadTrace(state_change.GetHandle());

             LOG(VERBOSE) << "Perfetto Trace ended"
                          << ", addr=" << reinterpret_cast<void*>(trace_buffer.begin)
                          << ",size= " << trace_buffer.size;

             PerfettoTraceProto wire_proto{trace_buffer.begin, trace_buffer.size};
             return rxcpp::observable<>::just(std::move(wire_proto)).as_dynamic();
           }
           return rxcpp::observable<>::empty<PerfettoTraceProto>().as_dynamic();
         }
    );
}

std::ostream& operator<<(std::ostream& os, PerfettoStreamCommand c) {
  switch (c) {
    case PerfettoStreamCommand::kStartTracing:
      os << "kStartTracing";
      break;
    case PerfettoStreamCommand::kStopTracing:
      os << "kStopTracing";
      break;
    case PerfettoStreamCommand::kShutdown:
      os << "kShutdown";
      break;
    default:
      os << "(unknown)";
      break;
  }
  return os;
}

RxProducerFactory::RxProducerFactory(PerfettoDependencies::Injector& injector)
  : injector_(injector) {
}

// TODO: (fruit) maybe this could be streamlined further by avoiding this boilerplate?
rxcpp::observable<PerfettoTraceProto> RxProducerFactory::CreateTraceStream(
    rxcpp::observable<PerfettoStreamCommand> commands) {
  std::shared_ptr<PerfettoConsumer> perfetto_consumer =
      injector_.get<std::shared_ptr<PerfettoConsumer>>();
  const ::perfetto::protos::TraceConfig& trace_config =
      injector_.get<::perfetto::protos::TraceConfig>();

  DCHECK(perfetto_consumer != nullptr);
  DCHECK(reinterpret_cast<volatile const void*>(&trace_config) != nullptr);

  return CreatePerfettoStream(commands,
                              perfetto_consumer,
                              trace_config);
}

// For testing/debugging only.
//
// Saves protobuf results in file name specified by 'arg_output_proto'.
void CollectPerfettoTraceBufferImmediately(
    RxProducerFactory& producer_factory,
    const std::string& arg_output_proto) {
  LOG(VERBOSE) << "CollectPerfettoTraceBufferImmediately";

  std::shared_ptr<PerfettoConsumer> perfetto_consumer =
      producer_factory.injector_.get<std::shared_ptr<PerfettoConsumer>>();
  const ::perfetto::protos::TraceConfig& trace_config =
      producer_factory.injector_.get<const ::perfetto::protos::TraceConfig&>();

  auto /*observable<PerfettoStateChange>*/ perfetto_states =
    CreatePerfettoStateStream(trace_config, perfetto_consumer);

  perfetto_states
    .as_blocking()  // Wait for observable to terminate with on_completed or on_error.
    .subscribe(/*on_next*/[&](auto state_change) {
       LOG(VERBOSE) << "Perfetto post-processed State change: " << state_change;

       using State = ::perfetto::consumer::State;
       switch (state_change.state) {
         case State::kConnecting:
           LOG(VERBOSE) << "Perfetto Tracing is Connecting";
           // Transitional state. No-op.
           break;
         case State::kConfigured:
           state_change.GetConsumer()->StartTracing(state_change.GetHandle());
           break;
         case State::kTracing:
           LOG(VERBOSE) << "Perfetto Tracing started";
           // Transitional state. No-op.
           break;
         case State::kTraceEnded: {
           ::perfetto::consumer::TraceBuffer trace_buffer =
             state_change.GetConsumer()->ReadTrace(state_change.GetHandle());

           LOG(VERBOSE) << "Perfetto Trace ended"
                        << ", addr=" << reinterpret_cast<void*>(trace_buffer.begin)
                        << ",size= " << trace_buffer.size;

           if (!arg_output_proto.empty()) {
             std::string trace_buffer_str;
             trace_buffer_str.resize(trace_buffer.size);
             std::copy(trace_buffer.begin,
                       trace_buffer.begin + trace_buffer.size,
                       trace_buffer_str.data());
             if (!android::base::WriteStringToFile(trace_buffer_str, arg_output_proto)) {
               LOG(ERROR) << "Failed to save TraceBuffer to " << arg_output_proto;
             } else {
               LOG(INFO) << "TraceBuffer saved to file: " << arg_output_proto;
               LOG(INFO);
               LOG(INFO) << "To print this in a human readable form, execute these commands:";
               LOG(INFO) << "$> adb pull '" << arg_output_proto << "'";
               LOG(INFO) << "$> trace_to_text systrace <filename.pb>";
             }
           }

           // TODO: something more useful with this TraceBuffer, such as saving it to a file
           // and printing the output.
           break;
         }
         default:
           // No other states are possible, because they go to #on_error or cause a dcheck.
           DCHECK(false) << "Invalid state: " << state_change;
       }

       //INTENTIONAL_COMPILER_ERROR_HERE // lets make sure this code actually does a trace.

     }, /*on_error*/[](rxcpp::util::error_ptr err) {
       LOG(ERROR) << "Perfetto post-processed state change failed: " << rxcpp::util::what(err);
     }, /*on_completed*/[]() {
       LOG(VERBOSE) << "Perfetto post-processed State #on_completed";
     });
}


}  // namespace iorap::perfetto
