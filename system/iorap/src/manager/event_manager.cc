/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "common/debug.h"
#include "common/expected.h"
#include "manager/event_manager.h"
#include "perfetto/rx_producer.h"

#include <android-base/properties.h>
#include <rxcpp/rx.hpp>

#include <atomic>
#include <functional>

using rxcpp::observe_on_one_worker;

namespace iorap::manager {

using binder::RequestId;
using binder::AppLaunchEvent;
using perfetto::PerfettoStreamCommand;
using perfetto::PerfettoTraceProto;

struct AppComponentName {
  std::string package;
  std::string activity_name;

  static bool HasAppComponentName(const std::string& s) {
    return s.find('/') != std::string::npos;
  }

  // "com.foo.bar/.A" -> {"com.foo.bar", ".A"}
  static AppComponentName FromString(const std::string& s) {
    constexpr const char delimiter = '/';
    std::string package = s.substr(0, delimiter);

    std::string activity_name = s;
    activity_name.erase(0, s.find(delimiter) + sizeof(delimiter));

    return {std::move(package), std::move(activity_name)};
  }

  // {"com.foo.bar", ".A"} -> "com.foo.bar/.A"
  std::string ToString() const {
    return package + "/" + activity_name;
  }

  /*
   * '/' is encoded into %2F
   * '%' is encoded into %25
   *
   * This allows the component name to be be used as a file name
   * ('/' is illegal due to being a path separator) with minimal
   * munging.
   */

  // "com.foo.bar%2F.A%25" -> {"com.foo.bar", ".A%"}
  static AppComponentName FromUrlEncodedString(const std::string& s) {
    std::string cpy = s;
    Replace(cpy, "%2F", "/");
    Replace(cpy, "%25", "%");

    return FromString(cpy);
  }

  // {"com.foo.bar", ".A%"} -> "com.foo.bar%2F.A%25"
  std::string ToUrlEncodedString() const {
    std::string s = ToString();
    Replace(s, "%", "%25");
    Replace(s, "/", "%2F");
    return s;
  }

 private:
  static bool Replace(std::string& str, const std::string& from, const std::string& to) {
    // TODO: call in a loop to replace all occurrences, not just the first one.
    const size_t start_pos = str.find(from);
    if (start_pos == std::string::npos) {
      return false;
    }

    str.replace(start_pos, from.length(), to);

    return true;
}
};

std::ostream& operator<<(std::ostream& os, const AppComponentName& name) {
  os << name.ToString();
  return os;
}

// Main logic of the #OnAppLaunchEvent scan method.
//
// All functions are called from the same thread as the event manager
// functions.
//
// This is a data type, it's moved (std::move) around from one iteration
// of #scan to another.
struct AppLaunchEventState {
  std::optional<AppComponentName> component_name_;

  bool is_tracing_{false};
  std::optional<rxcpp::composite_subscription> rx_lifetime_;
  std::vector<rxcpp::composite_subscription> rx_in_flight_;

  borrowed<perfetto::RxProducerFactory*> perfetto_factory_;  // not null
  borrowed<observe_on_one_worker*> thread_;  // not null
  borrowed<observe_on_one_worker*> io_thread_;  // not null

  explicit AppLaunchEventState(borrowed<perfetto::RxProducerFactory*> perfetto_factory,
                               borrowed<observe_on_one_worker*> thread,
                               borrowed<observe_on_one_worker*> io_thread) {
    perfetto_factory_ = perfetto_factory;
    DCHECK(perfetto_factory_ != nullptr);

    thread_ = thread;
    DCHECK(thread_ != nullptr);

    io_thread_ = io_thread;
    DCHECK(io_thread_ != nullptr);
  }

  // Updates the values in this struct only as a side effect.
  //
  // May create and fire a new rx chain on the same threads as passed
  // in by the constructors.
  void OnNewEvent(const AppLaunchEvent& event) {
    LOG(VERBOSE) << "AppLaunchEventState#OnNewEvent: " << event;

    using Type = AppLaunchEvent::Type;

    switch (event.type) {
      case Type::kIntentStarted: {
        DCHECK(!IsTracing());
        // Optimistically start tracing if we have the activity in the intent.
        if (!event.intent_proto->has_component()) {
          // Can't do anything if there is no component in the proto.
          LOG(VERBOSE) << "AppLaunchEventState#OnNewEvent: no component, can't trace";
          break;
        }

        const std::string& package_name = event.intent_proto->component().package_name();
        const std::string& class_name = event.intent_proto->component().class_name();
        AppComponentName component_name{package_name, class_name};

        component_name_ = component_name;
        rx_lifetime_ = StartTracing(std::move(component_name));

        break;
      }
      case Type::kIntentFailed:
        AbortTrace();
        break;
      case Type::kActivityLaunched: {
        // Cancel tracing for warm/hot.
        // Restart tracing if the activity was unexpected.

        AppLaunchEvent::Temperature temperature = event.temperature;
        if (temperature != AppLaunchEvent::Temperature::kCold) {
          LOG(DEBUG) << "AppLaunchEventState#OnNewEvent aborting trace due to non-cold temperature";
          AbortTrace();
        } else if (!IsTracing()) {  // and the temperature is Cold.
          // Start late trace when intent didn't have a component name
          LOG(VERBOSE) << "AppLaunchEventState#OnNewEvent need to start new trace";

          const std::string& title = event.activity_record_proto->identifier().title();
          if (!AppComponentName::HasAppComponentName(title)) {
            // Proto comment claim this is sometimes a window title.
            // We need the actual 'package/component' here, so just ignore it if it's a title.
            LOG(WARNING) << "App launched without a component name: " << event;
            break;
          }

          AppComponentName component_name = AppComponentName::FromString(title);

          component_name_ = component_name;
          rx_lifetime_ = StartTracing(std::move(component_name));
        } else {
          // FIXME: match actual component name against intent component name.
          // abort traces if they don't match.

          LOG(VERBOSE) << "AppLaunchEventState#OnNewEvent already tracing";
        }
        break;
      }
      case Type::kActivityLaunchFinished:
        // Finish tracing and collect trace buffer.
        //
        // TODO: this happens automatically when perfetto finishes its
        // trace duration.
        if (IsTracing()) {
          MarkPendingTrace();
        }
        break;
      case Type::kActivityLaunchCancelled:
        // Abort tracing.
        AbortTrace();
        break;
      default:
        DCHECK(false) << "invalid type: " << event;  // binder layer should've rejected this.
        LOG(ERROR) << "invalid type: " << event;  // binder layer should've rejected this.
    }
  }

  bool IsTracing() const {
    return is_tracing_;
  }

  rxcpp::composite_subscription StartTracing(AppComponentName component_name) {
    DCHECK(!IsTracing());

    auto /*observable<PerfettoStreamCommand>*/ perfetto_commands =
      rxcpp::observable<>::just(PerfettoStreamCommand::kStartTracing)
          // wait 1x
          .concat(
              // Pick a value longer than the perfetto config delay_ms, so that we send
              // 'kShutdown' after tracing has already finished.
              rxcpp::observable<>::interval(std::chrono::milliseconds(10000))
                  .take(2)  // kStopTracing, kShutdown.
                  .map([](int value) {
                         // value is 1,2,3,...
                         return static_cast<PerfettoStreamCommand>(value);  // 1,2, ...
                       })
          );

    auto /*observable<PerfettoTraceProto>*/ trace_proto_stream =
        perfetto_factory_->CreateTraceStream(perfetto_commands);
    // This immediately connects to perfetto asynchronously.
    //
    // TODO: create a perfetto handle earlier, to minimize perfetto startup latency.

    rxcpp::composite_subscription lifetime;

    trace_proto_stream
      .tap([](const PerfettoTraceProto& trace_proto) {
             LOG(VERBOSE) << "StartTracing -- PerfettoTraceProto received (1)";
           })
      .observe_on(*thread_)   // All work prior to 'observe_on' is handled on thread_.
      .subscribe_on(*thread_)   // All work prior to 'observe_on' is handled on thread_.
      .observe_on(*io_thread_)  // Write data on an idle-class-priority thread.
      .tap([](const PerfettoTraceProto& trace_proto) {
             LOG(VERBOSE) << "StartTracing -- PerfettoTraceProto received (2)";
           })
      .as_blocking()  // TODO: remove.
      .subscribe(/*out*/lifetime,
        /*on_next*/[component_name]
        (PerfettoTraceProto trace_proto) {
          std::string file_path = "/data/misc/iorapd/";
          file_path += component_name.ToUrlEncodedString();
          file_path += ".perfetto_trace.pb";

          // TODO: timestamp each file into a subdirectory.

          if (!trace_proto.WriteFullyToFile(file_path)) {
            LOG(ERROR) << "Failed to save TraceBuffer to " << file_path;
          } else {
            LOG(INFO) << "Perfetto TraceBuffer saved to file: " << file_path;
          }
        },
        /*on_error*/[](rxcpp::util::error_ptr err) {
          LOG(ERROR) << "Perfetto trace proto collection error: " << rxcpp::util::what(err);
        });

    is_tracing_ = true;

    return lifetime;
  }

  void AbortTrace() {
    LOG(VERBOSE) << "AppLaunchEventState - AbortTrace";
    is_tracing_ = false;
    if (rx_lifetime_) {
      // TODO: it would be good to call perfetto Destroy.

      LOG(VERBOSE) << "AppLaunchEventState - AbortTrace - Unsubscribe";
      rx_lifetime_->unsubscribe();
      rx_lifetime_.reset();
    }
  }

  void MarkPendingTrace() {
    LOG(VERBOSE) << "AppLaunchEventState - MarkPendingTrace";
    DCHECK(is_tracing_);
    DCHECK(rx_lifetime_.has_value());

    if (rx_lifetime_) {
      LOG(VERBOSE) << "AppLaunchEventState - MarkPendingTrace - lifetime moved";
      // Don't unsubscribe because that would cause the perfetto TraceBuffer
      // to get dropped on the floor.
      //
      // Instead, we want to let it finish and write it out to a file.
      rx_in_flight_.push_back(*std::move(rx_lifetime_));
      rx_lifetime_.reset();
    } else {
      LOG(VERBOSE) << "AppLaunchEventState - MarkPendingTrace - lifetime was empty";
    }

    // FIXME: how do we clear this vector?
  }
};

// Convert callback pattern into reactive pattern.
struct AppLaunchEventSubject {
  using RefWrapper =
    std::reference_wrapper<const AppLaunchEvent>;

  AppLaunchEventSubject() {}

  void Subscribe(rxcpp::subscriber<RefWrapper> subscriber) {
    DCHECK(ready_ != true) << "Cannot Subscribe twice";

    subscriber_ = std::move(subscriber);

    // Release edge of synchronizes-with AcquireIsReady.
    ready_.store(true);
  }

  void OnNext(const AppLaunchEvent& e) {
    if (!AcquireIsReady()) {
      return;
    }

    if (!subscriber_->is_subscribed()) {
      return;
    }

    /*
     * TODO: fix upstream.
     *
     * Rx workaround: this fails to compile when
     * the observable is a reference type:
     *
     * external/Reactive-Extensions/RxCpp/Rx/v2/src/rxcpp/rx-observer.hpp:354:18: error: multiple overloads of 'on_next' instantiate to the same signature 'void (const iorap::binder::AppLaunchEvent &) const'
     *   virtual void on_next(T&&) const {};
     *
     * external/Reactive-Extensions/RxCpp/Rx/v2/src/rxcpp/rx-observer.hpp:353:18: note: previous declaration is here
     *   virtual void on_next(T&) const {};
     *
     * (The workaround is to use reference_wrapper instead
     *  of const AppLaunchEvent&)
     */
    subscriber_->on_next(std::cref(e));

  }

  void OnCompleted() {
    if (!AcquireIsReady()) {
      return;
    }

    subscriber_->on_completed();
  }

 private:
  bool AcquireIsReady() {
    // Synchronizes-with the release-edge in Subscribe.
    // This can happen much later, only once the subscription actually happens.

    // However, as far as I know, 'rxcpp::subscriber' is not thread safe,
    // (but the observable chain itself can be made thread-safe via #observe_on, etc).
    // so we must avoid reading it until it has been fully synchronized.
    //
    // TODO: investigate rxcpp subscribers and see if we can get rid of this atomics,
    // to make it simpler.
    return ready_.load();
  }

  // TODO: also track the RequestId ?

  std::atomic<bool> ready_{false};


  std::optional<rxcpp::subscriber<RefWrapper>> subscriber_;
};

class EventManager::Impl {
 public:
  Impl(/*borrow*/perfetto::RxProducerFactory& perfetto_factory)
    : perfetto_factory_(perfetto_factory),
      worker_thread_(rxcpp::observe_on_new_thread()),
      worker_thread2_(rxcpp::observe_on_new_thread()),
      io_thread_(perfetto::ObserveOnNewIoThread()) {

    // TODO: read all properties from one config class.
    tracing_allowed_ = ::android::base::GetBoolProperty("iorapd.perfetto.enable", /*default*/false);

    if (tracing_allowed_) {
      rx_lifetime_ = InitializeRxGraph();
    } else {
      LOG(WARNING) << "Tracing disabled by iorapd.perfetto.enable=false";
    }
  }

  bool OnAppLaunchEvent(RequestId request_id,
                        const AppLaunchEvent& event) {
    LOG(VERBOSE) << "EventManager::OnAppLaunchEvent("
                 << "request_id=" << request_id.request_id << ","
                 << event;

    app_launch_event_subject_.OnNext(event);

    return true;
  }

  rxcpp::composite_subscription InitializeRxGraph() {
    LOG(VERBOSE) << "EventManager::InitializeRxGraph";

    app_launch_events_ = rxcpp::observable<>::create<AppLaunchEventRefWrapper>(
      [&](rxcpp::subscriber<AppLaunchEventRefWrapper> subscriber) {
        app_launch_event_subject_.Subscribe(std::move(subscriber));
      });

    rxcpp::composite_subscription lifetime;

    AppLaunchEventState initial_state{&perfetto_factory_, &worker_thread2_, &io_thread_};
    app_launch_events_
      .subscribe_on(worker_thread_)
      .scan(std::move(initial_state),
            [](AppLaunchEventState state, AppLaunchEventRefWrapper event) {
              state.OnNewEvent(event.get());
              return state;
            })
      .subscribe(/*out*/lifetime, [](const AppLaunchEventState& state) {
                   // Intentionally left blank.
                   (void)state;
                 });

    return lifetime;
  }

  perfetto::RxProducerFactory& perfetto_factory_;
  bool tracing_allowed_{true};

  using AppLaunchEventRefWrapper = AppLaunchEventSubject::RefWrapper;
  rxcpp::observable<AppLaunchEventRefWrapper> app_launch_events_;
  AppLaunchEventSubject app_launch_event_subject_;

  rxcpp::observable<RequestId> completed_requests_;

  // regular-priority thread to handle binder callbacks.
  observe_on_one_worker worker_thread_;
  observe_on_one_worker worker_thread2_;
  // low priority idle-class thread for IO operations.
  observe_on_one_worker io_thread_;

  rxcpp::composite_subscription rx_lifetime_;

//INTENTIONAL_COMPILER_ERROR_HERE:
  // FIXME:
  // ok so we want to expose a 'BlockingSubscribe' or a 'Subscribe' or some kind of function
  // that the main thread can call. This would subscribe on all the observables we internally
  // have here (probably on an event-manager-dedicated thread for simplicity).
  //
  // ideally we'd just reuse the binder thread to handle the events but I'm not super sure,
  // maybe this already works with the identity_current_thread coordination?
};
using Impl = EventManager::Impl;

EventManager::EventManager(perfetto::RxProducerFactory& perfetto_factory)
    : impl_(new Impl(perfetto_factory)) {}

std::shared_ptr<EventManager> EventManager::Create() {
  static perfetto::PerfettoDependencies::Injector injector{
    perfetto::PerfettoDependencies::CreateComponent
  };
  static perfetto::RxProducerFactory producer_factory{
    /*borrow*/injector
  };
  return EventManager::Create(/*borrow*/producer_factory);
}

std::shared_ptr<EventManager> EventManager::Create(perfetto::RxProducerFactory& perfetto_factory) {
  std::shared_ptr<EventManager> p{new EventManager{/*borrow*/perfetto_factory}};
  return p;
}

bool EventManager::OnAppLaunchEvent(RequestId request_id,
                                    const AppLaunchEvent& event) {
  return impl_->OnAppLaunchEvent(request_id, event);
}

}  // namespace iorap::manager
