/*
 * Copyright 2019 The Android Open Source Project
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

#include "tuningfork/protobuf_util.h"
#include "tuningfork/tuningfork_internal.h"

#include "full/tuningfork.pb.h"
#include "full/tuningfork_clearcut_log.pb.h"
#include "full/dev_tuningfork.pb.h"

#include "gtest/gtest.h"

#include <vector>
#include <mutex>

#define LOG_TAG "TFTest"
#include "Log.h"

using namespace tuningfork;

namespace tuningfork_test {

using ::com::google::tuningfork::FidelityParams;
using ::com::google::tuningfork::Settings;
using ::com::google::tuningfork::Annotation;
using ::logs::proto::tuningfork::TuningForkLogEvent;
using ::logs::proto::tuningfork::TuningForkHistogram;

class TestBackend : public DebugBackend {
public:
    TestBackend(std::shared_ptr<std::condition_variable> cv_,
                      std::shared_ptr<std::mutex> mutex_) : cv(cv_), mutex(mutex_) {}

    bool Process(const ProtobufSerialization &evt_ser) override {
        ALOGI("Process");
        {
            std::lock_guard<std::mutex> lock(*mutex);
            TuningForkLogEvent evt;
            Deserialize(evt_ser, evt);
            result = evt.DebugString();
        }
        cv->notify_all();
        return true;
    }

    void clear() { result = ""; }

    std::string result;
    std::shared_ptr<std::condition_variable> cv;
    std::shared_ptr<std::mutex> mutex;
};

class TestParamsLoader : public ParamsLoader {
public:
    bool GetFidelityParams(ProtobufSerialization &fidelity_params, size_t timeout_ms) override {
        return false;
    }
};

// Increment time with a known tick size
class TestTimeProvider : public ITimeProvider {
public:
    TestTimeProvider(Duration tickSizeNs_ = std::chrono::milliseconds(20))
        : tickSizeNs(tickSizeNs_) {}

    TimePoint t;
    Duration tickSizeNs;

    TimePoint NowNs() override {
        t += tickSizeNs;
        return t;
    }
};

std::shared_ptr<std::condition_variable> cv = std::make_shared<std::condition_variable>();
std::shared_ptr<std::mutex> rmutex = std::make_shared<std::mutex>();
TestBackend testBackend(cv, rmutex);
TestParamsLoader paramsLoader;
TestTimeProvider timeProvider;

struct HistogramSettings {
    float start, end;
    int nBuckets;
};
Settings TestSettings(Settings::AggregationStrategy::Submission method, int n_ticks, int n_keys,
                      std::vector<int> annotation_size,
                      const std::vector<HistogramSettings>& hists = {}) {
    // Make sure we set all required fields
    Settings s;
    s.mutable_aggregation_strategy()->set_method(method);
    s.mutable_aggregation_strategy()->set_intervalms_or_count(n_ticks);
    s.mutable_aggregation_strategy()->set_max_instrumentation_keys(n_keys);
    for(int i=0;i<annotation_size.size();++i)
        s.mutable_aggregation_strategy()->add_annotation_enum_size(annotation_size[i]);
    int i=0;
    for(auto& h: hists) {
        auto sh = s.add_histograms();
        sh->set_bucket_min(h.start);
        sh->set_bucket_max(h.end);
        sh->set_n_buckets(h.nBuckets);
        sh->set_instrument_key(i++);
    }
    return s;
}
const Duration test_wait_time = std::chrono::seconds(1);
std::string TestEndToEnd() {
    const int NTICKS = 101; // note the first tick doesn't add anything to the histogram
    auto settings = TestSettings(Settings::AggregationStrategy::TICK_BASED, NTICKS - 1, 1, {});
    tuningfork::Init(Serialize(settings), &testBackend, &paramsLoader, &timeProvider);
    std::unique_lock<std::mutex> lock(*rmutex);
    for (int i = 0; i < NTICKS; ++i)
        tuningfork::FrameTick(TFTICK_SYSCPU);
    // Wait for the upload thread to complete writing the string
    EXPECT_TRUE(cv->wait_for(lock, test_wait_time)==std::cv_status::no_timeout) << "Timeout";
    return testBackend.result;
}

std::string TestEndToEndWithAnnotation() {
    testBackend.clear();
    const int NTICKS = 101; // note the first tick doesn't add anything to the histogram
    // {3} is the number of values in the Level enum in tuningfork_extensions.proto
    auto settings = TestSettings(Settings::AggregationStrategy::TICK_BASED, NTICKS - 1, 2, {3});
    tuningfork::Init(Serialize(settings), &testBackend, &paramsLoader, &timeProvider);
    Annotation ann;
    ann.set_level(com::google::tuningfork::LEVEL_1);
    tuningfork::SetCurrentAnnotation(Serialize(ann));
    std::unique_lock<std::mutex> lock(*rmutex);
    for (int i = 0; i < NTICKS; ++i)
        tuningfork::FrameTick(TFTICK_SYSGPU);
    // Wait for the upload thread to complete writing the string
    EXPECT_TRUE(cv->wait_for(lock, test_wait_time)==std::cv_status::no_timeout) << "Timeout";
    return testBackend.result;
}

std::string TestEndToEndTimeBased() {
    testBackend.clear();
    const int NTICKS = 101; // note the first tick doesn't add anything to the histogram
    TestTimeProvider timeProvider(std::chrono::milliseconds(100)); // Tick in 100ms intervals
    auto settings = TestSettings(Settings::AggregationStrategy::TIME_BASED, 10100, 1, {}, {{50,150,10}});
    tuningfork::Init(Serialize(settings), &testBackend, &paramsLoader, &timeProvider);
    std::unique_lock<std::mutex> lock(*rmutex);
    for (int i = 0; i < NTICKS; ++i)
        tuningfork::FrameTick(TFTICK_SYSCPU);
    // Wait for the upload thread to complete writing the string
    EXPECT_TRUE(cv->wait_for(lock, test_wait_time)==std::cv_status::no_timeout) << "Timeout";
    return testBackend.result;
}

std::string TestEndToEndWithStaticHistogram() {
    testBackend.clear();
    const int NTICKS = 101; // note the first tick doesn't add anything to the histogram
    TestTimeProvider timeProvider(std::chrono::milliseconds(100)); // Tick in 100ms intervals
    auto settings = TestSettings(Settings::AggregationStrategy::TIME_BASED,
                                 10100, 1, {}, {{98, 102, 10}});
    tuningfork::Init(Serialize(settings), &testBackend, &paramsLoader, &timeProvider);
    std::unique_lock<std::mutex> lock(*rmutex);
    for (int i = 0; i < NTICKS; ++i)
        tuningfork::FrameTick(TFTICK_SYSCPU);
    // Wait for the upload thread to complete writing the string
    EXPECT_TRUE(cv->wait_for(lock, test_wait_time)==std::cv_status::no_timeout) << "Timeout";
    return testBackend.result;
}

TEST(TuningForkTest, EndToEnd) {
  EXPECT_EQ(TestEndToEnd(), "histograms {\n  instrument_id: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 100\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n}\n") << "Base test failed";
}
TEST(TuningForkTest, TestEndToEndWithAnnotation) {
  EXPECT_EQ(TestEndToEndWithAnnotation(), "histograms {\n  instrument_id: 1\n  annotation: \"\\010\\001\"\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 100\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n}\n") << "Annotation test failed";
}
TEST(TuningForkTest, TestEndToEndTimeBased) {
  EXPECT_EQ(TestEndToEndTimeBased(), "histograms {\n  instrument_id: 0\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 100\n  counts: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n}\n") << "Base test failed";
}
TEST(TuningForkTest, TestEndToEndWithStaticHistogram) {
  EXPECT_EQ(TestEndToEndWithStaticHistogram(), "histograms {\n  instrument_id: 0\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n  counts: 0\n  counts: 100\n  counts: 0\n  counts: 0\n"
            "  counts: 0\n  counts: 0\n  counts: 0\n}\n") << "Base test failed";
}

} // namespace tuningfork_test
