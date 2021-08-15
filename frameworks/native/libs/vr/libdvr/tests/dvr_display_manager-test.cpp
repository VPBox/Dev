#include <android-base/properties.h>
#include <base/logging.h>
#include <cutils/properties.h>
#include <gtest/gtest.h>
#include <log/log.h>
#include <poll.h>

#include <android/hardware_buffer.h>

#include <algorithm>
#include <array>
#include <set>
#include <thread>
#include <vector>

#include <dvr/dvr_configuration_data.h>
#include <dvr/dvr_deleter.h>
#include <dvr/dvr_display_manager.h>
#include <dvr/dvr_surface.h>

#include <pdx/status.h>

using android::pdx::ErrorStatus;
using android::pdx::Status;

namespace android {
namespace dvr {

namespace {

using ::testing::Test;

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key, nullptr_t) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_NONE;
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key, int32_t value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_INT32;
  attribute.value.int32_value = value;
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key, int64_t value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_INT64;
  attribute.value.int64_value = value;
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key, bool value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL;
  attribute.value.bool_value = value;
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key, float value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT;
  attribute.value.float_value = value;
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key,
                                  const std::array<float, 2>& value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT2;
  std::copy(value.begin(), value.end(), attribute.value.float2_value);
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key,
                                  const std::array<float, 3>& value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT3;
  std::copy(value.begin(), value.end(), attribute.value.float3_value);
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key,
                                  const std::array<float, 4>& value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT4;
  std::copy(value.begin(), value.end(), attribute.value.float4_value);
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key,
                                  const std::array<float, 8>& value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT8;
  std::copy(value.begin(), value.end(), attribute.value.float8_value);
  return attribute;
}

DvrSurfaceAttribute MakeAttribute(DvrSurfaceAttributeKey key,
                                  const std::array<float, 16>& value) {
  DvrSurfaceAttribute attribute;
  attribute.key = key;
  attribute.value.type = DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT16;
  std::copy(value.begin(), value.end(), attribute.value.float16_value);
  return attribute;
}

Status<UniqueDvrSurface> CreateApplicationSurface(bool visible = false,
                                                  int32_t z_order = 0) {
  DvrSurface* surface = nullptr;
  DvrSurfaceAttribute attributes[] = {
      MakeAttribute(DVR_SURFACE_ATTRIBUTE_Z_ORDER, z_order),
      MakeAttribute(DVR_SURFACE_ATTRIBUTE_VISIBLE, visible)};

  const int ret = dvrSurfaceCreate(
      attributes, std::extent<decltype(attributes)>::value, &surface);
  if (ret < 0)
    return ErrorStatus(-ret);
  else
    return {UniqueDvrSurface(surface)};
}

Status<UniqueDvrWriteBufferQueue> CreateSurfaceQueue(
    const UniqueDvrSurface& surface, uint32_t width, uint32_t height,
    uint32_t format, uint32_t layer_count, uint64_t usage, size_t capacity,
    size_t metadata_size) {
  DvrWriteBufferQueue* queue;
  const int ret = dvrSurfaceCreateWriteBufferQueue(
      surface.get(), width, height, format, layer_count, usage, capacity,
      metadata_size, &queue);
  if (ret < 0)
    return ErrorStatus(-ret);
  else
    return {UniqueDvrWriteBufferQueue(queue)};
}

Status<std::vector<uint8_t>> GetConfigData(int config_type) {
  uint8_t* data = nullptr;
  size_t data_size = 0;
  int error = dvrConfigurationDataGet(config_type, &data, &data_size);
  if (error < 0) {
    return ErrorStatus(-error);
  }

  if (!data || data_size == 0) {
    return ErrorStatus(EINVAL);
  }
  std::vector<uint8_t> data_result(data, data + data_size);
  dvrConfigurationDataDestroy(data);
  std::string s(data, data + data_size);
  return {std::move(data_result)};
}

class TestDisplayManager {
 public:
  TestDisplayManager(UniqueDvrDisplayManager display_manager,
                     UniqueDvrSurfaceState surface_state)
      : display_manager_(std::move(display_manager)),
        surface_state_(std::move(surface_state)) {
    const int fd = dvrDisplayManagerGetEventFd(display_manager_.get());
    LOG_IF(INFO, fd < 0) << "Failed to get event fd: " << strerror(-fd);
    display_manager_event_fd_ = fd;
  }

  Status<UniqueDvrReadBufferQueue> GetReadBufferQueue(int surface_id,
                                                      int queue_id) {
    DvrReadBufferQueue* queue;
    const int ret = dvrDisplayManagerGetReadBufferQueue(
        display_manager_.get(), surface_id, queue_id, &queue);
    if (ret < 0)
      return ErrorStatus(-ret);
    else
      return {UniqueDvrReadBufferQueue(queue)};
  }

  Status<void> UpdateSurfaceState() {
    const int ret = dvrDisplayManagerGetSurfaceState(display_manager_.get(),
                                                     surface_state_.get());
    if (ret < 0)
      return ErrorStatus(-ret);
    else
      return {};
  }

  enum : int { kTimeoutMs = 10000 };  // 10s

  Status<void> WaitForUpdate(int timeout_ms = kTimeoutMs) {
    if (display_manager_event_fd_ < 0)
      return ErrorStatus(-display_manager_event_fd_);

    pollfd pfd = {display_manager_event_fd_, POLLIN, 0};
    const int count = poll(&pfd, 1, timeout_ms);
    if (count < 0)
      return ErrorStatus(errno);
    else if (count == 0)
      return ErrorStatus(ETIMEDOUT);

    int events;
    const int ret = dvrDisplayManagerTranslateEpollEventMask(
        display_manager_.get(), pfd.revents, &events);
    if (ret < 0)
      return ErrorStatus(-ret);
    else if (events & POLLIN)
      return UpdateSurfaceState();
    else
      return ErrorStatus(EPROTO);
  }

  Status<size_t> GetSurfaceCount() {
    size_t count = 0;
    const int ret =
        dvrSurfaceStateGetSurfaceCount(surface_state_.get(), &count);
    if (ret < 0)
      return ErrorStatus(-ret);
    else
      return {count};
  }

  Status<DvrSurfaceUpdateFlags> GetUpdateFlags(size_t surface_index) {
    DvrSurfaceUpdateFlags update_flags;
    const int ret = dvrSurfaceStateGetUpdateFlags(surface_state_.get(),
                                                  surface_index, &update_flags);
    if (ret < 0)
      return ErrorStatus(-ret);
    else
      return {update_flags};
  }

  Status<int> GetSurfaceId(size_t surface_index) {
    int surface_id;
    const int ret = dvrSurfaceStateGetSurfaceId(surface_state_.get(),
                                                surface_index, &surface_id);
    if (ret < 0)
      return ErrorStatus(-ret);
    else
      return {surface_id};
  }

  Status<int> GetProcessId(size_t surface_index) {
    int process_id;
    const int ret = dvrSurfaceStateGetProcessId(surface_state_.get(),
                                                surface_index, &process_id);
    if (ret < 0)
      return ErrorStatus(-ret);
    else
      return {process_id};
  }

  Status<std::vector<DvrSurfaceAttribute>> GetAttributes(size_t surface_index) {
    std::vector<DvrSurfaceAttribute> attributes;
    size_t count = 0;
    const int ret = dvrSurfaceStateGetAttributeCount(surface_state_.get(),
                                                     surface_index, &count);
    if (ret < 0)
      return ErrorStatus(-ret);

    attributes.resize(count);
    const ssize_t return_count = dvrSurfaceStateGetAttributes(
        surface_state_.get(), surface_index, attributes.data(), count);
    if (return_count < 0)
      return ErrorStatus(-return_count);

    attributes.resize(return_count);
    return {std::move(attributes)};
  }

  Status<std::vector<int>> GetQueueIds(size_t surface_index) {
    std::vector<int> queue_ids;
    size_t count = 0;
    const int ret = dvrSurfaceStateGetQueueCount(surface_state_.get(),
                                                 surface_index, &count);
    if (ret < 0)
      return ErrorStatus(-ret);

    if (count > 0) {
      queue_ids.resize(count);
      const ssize_t return_count = dvrSurfaceStateGetQueueIds(
          surface_state_.get(), surface_index, queue_ids.data(), count);
      if (return_count < 0)
        return ErrorStatus(-return_count);

      queue_ids.resize(return_count);
    }

    return {std::move(queue_ids)};
  }

 private:
  UniqueDvrDisplayManager display_manager_;
  UniqueDvrSurfaceState surface_state_;

  // Owned by object in display_manager_, do not explicitly close.
  int display_manager_event_fd_;

  TestDisplayManager(const TestDisplayManager&) = delete;
  void operator=(const TestDisplayManager&) = delete;
};

class DvrDisplayManagerTest : public Test {
 protected:
  void SetUp() override {
    // dvr display manager test doesn't apply to standalone vr devices because
    // tests cannot create display manager client on these devices.
    if (property_get_bool("ro.boot.vr", false)) {
      GTEST_SKIP()
          << "All tests in DvrDisplayManagerTest test case are skipped "
             "because the device boot to VR.";
    }

    int ret;
    DvrDisplayManager* display_manager;
    DvrSurfaceState* surface_state;

    ret = dvrDisplayManagerCreate(&display_manager);
    ASSERT_EQ(0, ret) << "Failed to create display manager client";
    ASSERT_NE(nullptr, display_manager);

    ret = dvrSurfaceStateCreate(&surface_state);
    ASSERT_EQ(0, ret) << "Failed to create surface state object";
    ASSERT_NE(nullptr, surface_state);

    manager_.reset(
        new TestDisplayManager(UniqueDvrDisplayManager(display_manager),
                               UniqueDvrSurfaceState(surface_state)));
  }
  void TearDown() override {}

  std::unique_ptr<TestDisplayManager> manager_;
};

// TODO(eieio): Consider moving these somewhere more central because they are
// broadly useful.

template <typename T>
testing::AssertionResult StatusOk(const char* status_expression,
                                  const Status<T>& status) {
  if (!status.ok()) {
    return testing::AssertionFailure()
           << "(" << status_expression
           << ") expected to indicate success but actually contains error ("
           << status.error() << ")";
  } else {
    return testing::AssertionSuccess();
  }
}

template <typename T>
testing::AssertionResult StatusError(const char* status_expression,
                                     const Status<T>& status) {
  if (status.ok()) {
    return testing::AssertionFailure()
           << "(" << status_expression
           << ") expected to indicate error but instead indicates success.";
  } else {
    return testing::AssertionSuccess();
  }
}

template <typename T>
testing::AssertionResult StatusHasError(const char* status_expression,
                                        const char* /*error_code_expression*/,
                                        const Status<T>& status,
                                        int error_code) {
  if (status.ok()) {
    return StatusError(status_expression, status);
  } else if (status.error() != error_code) {
    return testing::AssertionFailure()
           << "(" << status_expression << ") expected to indicate error ("
           << error_code << ") but actually indicates error (" << status.error()
           << ")";
  } else {
    return testing::AssertionSuccess();
  }
}

template <typename T, typename U>
testing::AssertionResult StatusHasValue(const char* status_expression,
                                        const char* /*value_expression*/,
                                        const Status<T>& status,
                                        const U& value) {
  if (!status.ok()) {
    return StatusOk(status_expression, status);
  } else if (status.get() != value) {
    return testing::AssertionFailure()
           << "(" << status_expression << ") expected to contain value ("
           << testing::PrintToString(value) << ") but actually contains value ("
           << testing::PrintToString(status.get()) << ")";
  } else {
    return testing::AssertionSuccess();
  }
}

template <typename T, typename Op>
testing::AssertionResult StatusPred(const char* status_expression,
                                    const char* pred_expression,
                                    const Status<T>& status, Op pred) {
  if (!status.ok()) {
    return StatusOk(status_expression, status);
  } else if (!pred(status.get())) {
    return testing::AssertionFailure()
           << status_expression << " value ("
           << testing::PrintToString(status.get())
           << ") failed to pass predicate " << pred_expression;
  } else {
    return testing::AssertionSuccess();
  }
}

#define ASSERT_STATUS_OK(status) ASSERT_PRED_FORMAT1(StatusOk, status)
#define ASSERT_STATUS_ERROR(status) ASSERT_PRED_FORMAT1(StatusError, status)

#define ASSERT_STATUS_ERROR_VALUE(value, status) \
  ASSERT_PRED_FORMAT2(StatusHasError, status, value)

#define ASSERT_STATUS_EQ(value, status) \
  ASSERT_PRED_FORMAT2(StatusHasValue, status, value)

#define EXPECT_STATUS_OK(status) EXPECT_PRED_FORMAT1(StatusOk, status)
#define EXPECT_STATUS_ERROR(status) EXPECT_PRED_FORMAT1(StatusError, status)

#define EXPECT_STATUS_ERROR_VALUE(value, status) \
  EXPECT_PRED_FORMAT2(StatusHasError, status, value)

#define EXPECT_STATUS_EQ(value, status) \
  EXPECT_PRED_FORMAT2(StatusHasValue, status, value)

#define EXPECT_STATUS_PRED(pred, status) \
  EXPECT_PRED_FORMAT2(StatusPred, status, pred)

#if 0
// Verify utility predicate/macro functionality. This section is commented out
// because it is designed to fail in some cases to validate the helpers.
TEST_F(Test, ExpectVoid) {
  Status<void> status_error{ErrorStatus{EINVAL}};
  Status<void> status_ok{};

  EXPECT_STATUS_ERROR(status_error);
  EXPECT_STATUS_ERROR(status_ok);
  EXPECT_STATUS_OK(status_error);
  EXPECT_STATUS_OK(status_ok);

  EXPECT_STATUS_ERROR_VALUE(EINVAL, status_error);
  EXPECT_STATUS_ERROR_VALUE(ENOMEM, status_error);
  EXPECT_STATUS_ERROR_VALUE(EINVAL, status_ok);
  EXPECT_STATUS_ERROR_VALUE(ENOMEM, status_ok);
}

TEST_F(Test, ExpectInt) {
  Status<int> status_error{ErrorStatus{EINVAL}};
  Status<int> status_ok{10};

  EXPECT_STATUS_ERROR(status_error);
  EXPECT_STATUS_ERROR(status_ok);
  EXPECT_STATUS_OK(status_error);
  EXPECT_STATUS_OK(status_ok);

  EXPECT_STATUS_ERROR_VALUE(EINVAL, status_error);
  EXPECT_STATUS_ERROR_VALUE(ENOMEM, status_error);
  EXPECT_STATUS_ERROR_VALUE(EINVAL, status_ok);
  EXPECT_STATUS_ERROR_VALUE(ENOMEM, status_ok);

  EXPECT_STATUS_EQ(10, status_error);
  EXPECT_STATUS_EQ(20, status_error);
  EXPECT_STATUS_EQ(10, status_ok);
  EXPECT_STATUS_EQ(20, status_ok);

  auto pred1 = [](const auto& value) { return value < 15; };
  auto pred2 = [](const auto& value) { return value > 5; };
  auto pred3 = [](const auto& value) { return value > 15; };
  auto pred4 = [](const auto& value) { return value < 5; };

  EXPECT_STATUS_PRED(pred1, status_error);
  EXPECT_STATUS_PRED(pred2, status_error);
  EXPECT_STATUS_PRED(pred3, status_error);
  EXPECT_STATUS_PRED(pred4, status_error);
  EXPECT_STATUS_PRED(pred1, status_ok);
  EXPECT_STATUS_PRED(pred2, status_ok);
  EXPECT_STATUS_PRED(pred3, status_ok);
  EXPECT_STATUS_PRED(pred4, status_ok);
}
#endif

TEST_F(DvrDisplayManagerTest, SurfaceCreateEvent) {
  // Get surface state and verify there are no surfaces.
  ASSERT_STATUS_OK(manager_->UpdateSurfaceState());
  ASSERT_STATUS_EQ(0u, manager_->GetSurfaceCount());

  // Get flags for invalid surface index.
  EXPECT_STATUS_ERROR_VALUE(EINVAL, manager_->GetUpdateFlags(0));

  // Create an application surface.
  auto surface_status = CreateApplicationSurface();
  ASSERT_STATUS_OK(surface_status);
  UniqueDvrSurface surface = surface_status.take();
  ASSERT_NE(nullptr, surface.get());

  const int surface_id = dvrSurfaceGetId(surface.get());
  ASSERT_GE(surface_id, 0);

  // Now there should be one new surface.
  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  EXPECT_STATUS_EQ(1u, manager_->GetSurfaceCount());

  // Verify the new surface flag is set.
  auto check_flags = [](const auto& value) {
    return value & DVR_SURFACE_UPDATE_FLAGS_NEW_SURFACE;
  };
  EXPECT_STATUS_PRED(check_flags, manager_->GetUpdateFlags(0));

  // Verify the surface id matches.
  EXPECT_STATUS_EQ(surface_id, manager_->GetSurfaceId(0));

  // Verify the owning process of the surface.
  EXPECT_STATUS_EQ(getpid(), manager_->GetProcessId(0));

  surface.reset();

  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  EXPECT_STATUS_EQ(0u, manager_->GetSurfaceCount());
}

TEST_F(DvrDisplayManagerTest, SurfaceAttributeEvent) {
  // Get surface state and verify there are no surfaces.
  ASSERT_STATUS_OK(manager_->UpdateSurfaceState());
  ASSERT_STATUS_EQ(0u, manager_->GetSurfaceCount());

  // Get attributes for an invalid surface index.
  EXPECT_STATUS_ERROR_VALUE(EINVAL, manager_->GetAttributes(0));

  const bool kInitialVisibility = true;
  const int32_t kInitialZOrder = 10;
  auto surface_status =
      CreateApplicationSurface(kInitialVisibility, kInitialZOrder);
  ASSERT_STATUS_OK(surface_status);
  auto surface = surface_status.take();
  ASSERT_NE(nullptr, surface.get());

  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  ASSERT_STATUS_EQ(1u, manager_->GetSurfaceCount());

  // Check the initial attribute values.
  auto attribute_status = manager_->GetAttributes(0);
  ASSERT_STATUS_OK(attribute_status);
  auto attributes = attribute_status.take();
  EXPECT_GE(attributes.size(), 2u);

  std::set<int32_t> actual_keys;
  std::set<int32_t> expected_keys = {DVR_SURFACE_ATTRIBUTE_Z_ORDER,
                                     DVR_SURFACE_ATTRIBUTE_VISIBLE};

  // Collect all the keys in attributes that match the expected keys.
  auto compare_keys = [](const auto& attributes, const auto& expected_keys) {
    std::set<int32_t> keys;
    for (const auto& attribute : attributes) {
      if (expected_keys.find(attribute.key) != expected_keys.end())
        keys.emplace(attribute.key);
    }
    return keys;
  };

  // If the sets match then attributes contained at least the expected keys,
  // even if other keys were also present.
  actual_keys = compare_keys(attributes, expected_keys);
  EXPECT_EQ(expected_keys, actual_keys);

  std::vector<DvrSurfaceAttribute> attributes_to_set = {
      MakeAttribute(DVR_SURFACE_ATTRIBUTE_Z_ORDER, 0)};

  // Test invalid args.
  EXPECT_EQ(-EINVAL, dvrSurfaceSetAttributes(nullptr, attributes_to_set.data(),
                                             attributes_to_set.size()));
  EXPECT_EQ(-EINVAL, dvrSurfaceSetAttributes(surface.get(), nullptr,
                                             attributes_to_set.size()));

  // Test attribute change events.
  ASSERT_EQ(0, dvrSurfaceSetAttributes(surface.get(), attributes_to_set.data(),
                                       attributes_to_set.size()));
  ASSERT_STATUS_OK(manager_->WaitForUpdate());

  // Verify the attributes changed flag is set.
  auto check_flags = [](const auto& value) {
    return value & DVR_SURFACE_UPDATE_FLAGS_ATTRIBUTES_CHANGED;
  };
  EXPECT_STATUS_PRED(check_flags, manager_->GetUpdateFlags(0));

  attribute_status = manager_->GetAttributes(0);
  ASSERT_STATUS_OK(attribute_status);
  attributes = attribute_status.take();
  EXPECT_GE(attributes.size(), 2u);

  expected_keys = {DVR_SURFACE_ATTRIBUTE_Z_ORDER,
                   DVR_SURFACE_ATTRIBUTE_VISIBLE};

  actual_keys.clear();
  actual_keys = compare_keys(attributes, expected_keys);
  EXPECT_EQ(expected_keys, actual_keys);

  // Test setting and then deleting an attribute.
  const DvrSurfaceAttributeKey kUserKey = 1;
  attributes_to_set = {MakeAttribute(kUserKey, 1024)};

  ASSERT_EQ(0, dvrSurfaceSetAttributes(surface.get(), attributes_to_set.data(),
                                       attributes_to_set.size()));
  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  EXPECT_STATUS_PRED(check_flags, manager_->GetUpdateFlags(0));

  attribute_status = manager_->GetAttributes(0);
  ASSERT_STATUS_OK(attribute_status);
  attributes = attribute_status.take();
  EXPECT_GE(attributes.size(), 2u);

  expected_keys = {DVR_SURFACE_ATTRIBUTE_Z_ORDER, DVR_SURFACE_ATTRIBUTE_VISIBLE,
                   kUserKey};

  actual_keys.clear();
  actual_keys = compare_keys(attributes, expected_keys);
  EXPECT_EQ(expected_keys, actual_keys);

  // Delete the attribute.
  attributes_to_set = {MakeAttribute(kUserKey, nullptr)};

  ASSERT_EQ(0, dvrSurfaceSetAttributes(surface.get(), attributes_to_set.data(),
                                       attributes_to_set.size()));
  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  EXPECT_STATUS_PRED(check_flags, manager_->GetUpdateFlags(0));

  attribute_status = manager_->GetAttributes(0);
  ASSERT_STATUS_OK(attribute_status);
  attributes = attribute_status.take();
  EXPECT_GE(attributes.size(), 2u);

  expected_keys = {DVR_SURFACE_ATTRIBUTE_Z_ORDER, DVR_SURFACE_ATTRIBUTE_VISIBLE,
                   kUserKey};

  actual_keys.clear();
  actual_keys = compare_keys(attributes, expected_keys);
  EXPECT_NE(expected_keys, actual_keys);

  // Test deleting a reserved attribute.
  attributes_to_set = {MakeAttribute(DVR_SURFACE_ATTRIBUTE_VISIBLE, nullptr)};

  EXPECT_EQ(0, dvrSurfaceSetAttributes(surface.get(), attributes_to_set.data(),
                                       attributes_to_set.size()));

  // Failed attribute operations should not trigger update events.
  const int kTimeoutMs = 100;  // 0.1s
  EXPECT_STATUS_ERROR_VALUE(ETIMEDOUT, manager_->WaitForUpdate(kTimeoutMs));

  attribute_status = manager_->GetAttributes(0);
  ASSERT_STATUS_OK(attribute_status);
  attributes = attribute_status.take();
  EXPECT_GE(attributes.size(), 2u);

  expected_keys = {DVR_SURFACE_ATTRIBUTE_Z_ORDER,
                   DVR_SURFACE_ATTRIBUTE_VISIBLE};

  actual_keys.clear();
  actual_keys = compare_keys(attributes, expected_keys);
  EXPECT_EQ(expected_keys, actual_keys);
}

TEST_F(DvrDisplayManagerTest, SurfaceAttributeTypes) {
  // Create an application surface.
  auto surface_status = CreateApplicationSurface();
  ASSERT_STATUS_OK(surface_status);
  UniqueDvrSurface surface = surface_status.take();
  ASSERT_NE(nullptr, surface.get());

  enum : std::int32_t {
    kInt32Key = 1,
    kInt64Key,
    kBoolKey,
    kFloatKey,
    kFloat2Key,
    kFloat3Key,
    kFloat4Key,
    kFloat8Key,
    kFloat16Key,
  };

  const std::vector<DvrSurfaceAttribute> attributes_to_set = {
      MakeAttribute(kInt32Key, int32_t{0}),
      MakeAttribute(kInt64Key, int64_t{0}),
      MakeAttribute(kBoolKey, false),
      MakeAttribute(kFloatKey, 0.0f),
      MakeAttribute(kFloat2Key, std::array<float, 2>{{1.0f, 2.0f}}),
      MakeAttribute(kFloat3Key, std::array<float, 3>{{3.0f, 4.0f, 5.0f}}),
      MakeAttribute(kFloat4Key, std::array<float, 4>{{6.0f, 7.0f, 8.0f, 9.0f}}),
      MakeAttribute(kFloat8Key,
                    std::array<float, 8>{{10.0f, 11.0f, 12.0f, 13.0f, 14.0f,
                                          15.0f, 16.0f, 17.0f}}),
      MakeAttribute(kFloat16Key, std::array<float, 16>{
                                     {18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f,
                                      24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f,
                                      30.0f, 31.0f, 32.0f, 33.0f}})};

  EXPECT_EQ(0, dvrSurfaceSetAttributes(surface.get(), attributes_to_set.data(),
                                       attributes_to_set.size()));

  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  auto attribute_status = manager_->GetAttributes(0);
  ASSERT_STATUS_OK(attribute_status);
  auto attributes = attribute_status.take();
  EXPECT_GE(attributes.size(), attributes_to_set.size());

  auto HasAttribute = [](const auto& attributes,
                         DvrSurfaceAttributeKey key) -> bool {
    for (const auto& attribute : attributes) {
      if (attribute.key == key)
        return true;
    }
    return false;
  };
  auto AttributeType =
      [](const auto& attributes,
         DvrSurfaceAttributeKey key) -> DvrSurfaceAttributeType {
    for (const auto& attribute : attributes) {
      if (attribute.key == key)
        return attribute.value.type;
    }
    return DVR_SURFACE_ATTRIBUTE_TYPE_NONE;
  };

  ASSERT_TRUE(HasAttribute(attributes, kInt32Key));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_INT32,
            AttributeType(attributes, kInt32Key));

  ASSERT_TRUE(HasAttribute(attributes, kInt64Key));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_INT64,
            AttributeType(attributes, kInt64Key));

  ASSERT_TRUE(HasAttribute(attributes, kBoolKey));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
            AttributeType(attributes, kBoolKey));

  ASSERT_TRUE(HasAttribute(attributes, kFloatKey));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT,
            AttributeType(attributes, kFloatKey));

  ASSERT_TRUE(HasAttribute(attributes, kFloat2Key));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT2,
            AttributeType(attributes, kFloat2Key));

  ASSERT_TRUE(HasAttribute(attributes, kFloat3Key));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT3,
            AttributeType(attributes, kFloat3Key));

  ASSERT_TRUE(HasAttribute(attributes, kFloat4Key));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT4,
            AttributeType(attributes, kFloat4Key));

  ASSERT_TRUE(HasAttribute(attributes, kFloat8Key));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT8,
            AttributeType(attributes, kFloat8Key));

  ASSERT_TRUE(HasAttribute(attributes, kFloat16Key));
  EXPECT_EQ(DVR_SURFACE_ATTRIBUTE_TYPE_FLOAT16,
            AttributeType(attributes, kFloat16Key));
}

TEST_F(DvrDisplayManagerTest, SurfaceQueueEvent) {
  // Create an application surface.
  auto surface_status = CreateApplicationSurface();
  ASSERT_STATUS_OK(surface_status);
  UniqueDvrSurface surface = surface_status.take();
  ASSERT_NE(nullptr, surface.get());

  const int surface_id = dvrSurfaceGetId(surface.get());
  ASSERT_GE(surface_id, 0);
  // Get surface state and verify there is one surface.
  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  ASSERT_STATUS_EQ(1u, manager_->GetSurfaceCount());

  // Verify there are no queues for the surface recorded in the state
  // snapshot.
  EXPECT_STATUS_EQ(std::vector<int>{}, manager_->GetQueueIds(0));

  // Create a new queue in the surface.
  auto write_queue_status = CreateSurfaceQueue(
      surface, 320, 240, AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM, 1,
      AHARDWAREBUFFER_USAGE_CPU_READ_RARELY, 1, 0);
  ASSERT_STATUS_OK(write_queue_status);
  UniqueDvrWriteBufferQueue write_queue = write_queue_status.take();
  ASSERT_NE(nullptr, write_queue.get());

  const int queue_id = dvrWriteBufferQueueGetId(write_queue.get());
  ASSERT_GE(queue_id, 0);

  // Update surface state.
  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  ASSERT_STATUS_EQ(1u, manager_->GetSurfaceCount());

  // Verify the buffers changed flag is set.
  auto check_flags = [](const auto& value) {
    return value & DVR_SURFACE_UPDATE_FLAGS_BUFFERS_CHANGED;
  };
  EXPECT_STATUS_PRED(check_flags, manager_->GetUpdateFlags(0));

  auto queue_ids_status = manager_->GetQueueIds(0);
  ASSERT_STATUS_OK(queue_ids_status);

  auto queue_ids = queue_ids_status.take();
  ASSERT_EQ(1u, queue_ids.size());
  EXPECT_EQ(queue_id, queue_ids[0]);

  auto read_queue_status = manager_->GetReadBufferQueue(surface_id, queue_id);
  ASSERT_STATUS_OK(read_queue_status);
  UniqueDvrReadBufferQueue read_queue = read_queue_status.take();
  ASSERT_NE(nullptr, read_queue.get());
  EXPECT_EQ(queue_id, dvrReadBufferQueueGetId(read_queue.get()));

  write_queue.reset();

  // Verify that destroying the queue generates a surface update event.
  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  ASSERT_STATUS_EQ(1u, manager_->GetSurfaceCount());

  // Verify that the buffers changed flag is set.
  EXPECT_STATUS_PRED(check_flags, manager_->GetUpdateFlags(0));

  // Verify that the queue ids reflect the change.
  queue_ids_status = manager_->GetQueueIds(0);
  ASSERT_STATUS_OK(queue_ids_status);

  queue_ids = queue_ids_status.take();
  ASSERT_EQ(0u, queue_ids.size());
}

TEST_F(DvrDisplayManagerTest, MultiLayerBufferQueue) {
  // Create an application surface.
  auto surface_status = CreateApplicationSurface();
  ASSERT_STATUS_OK(surface_status);
  UniqueDvrSurface surface = surface_status.take();
  ASSERT_NE(nullptr, surface.get());

  // Get surface state and verify there is one surface.
  ASSERT_STATUS_OK(manager_->WaitForUpdate());
  ASSERT_STATUS_EQ(1u, manager_->GetSurfaceCount());

  // Create a new queue in the surface.
  const uint32_t kLayerCount = 3;
  auto write_queue_status = CreateSurfaceQueue(
      surface, 320, 240, AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM, kLayerCount,
      AHARDWAREBUFFER_USAGE_CPU_READ_RARELY, 1, 0);
  ASSERT_STATUS_OK(write_queue_status);
  UniqueDvrWriteBufferQueue write_queue = write_queue_status.take();
  ASSERT_NE(nullptr, write_queue.get());

  DvrWriteBuffer* buffer = nullptr;
  DvrNativeBufferMetadata metadata;
  int fence_fd = -1;
  int error = dvrWriteBufferQueueGainBuffer(write_queue.get(), /*timeout=*/1000,
                                            &buffer, &metadata, &fence_fd);
  ASSERT_EQ(0, error);

  AHardwareBuffer* hardware_buffer = nullptr;
  error = dvrWriteBufferGetAHardwareBuffer(buffer, &hardware_buffer);
  ASSERT_EQ(0, error);

  AHardwareBuffer_Desc desc = {};
  AHardwareBuffer_describe(hardware_buffer, &desc);
  ASSERT_EQ(kLayerCount, desc.layers);

  AHardwareBuffer_release(hardware_buffer);
  dvrWriteBufferDestroy(buffer);
}

TEST_F(Test, ConfigurationData) {
  // TODO(hendrikw): Move this test and GetConfigData helper function out of the
  // display manager tests.
  auto data1 = GetConfigData(-1);
  ASSERT_STATUS_ERROR(data1);

  const char kDvrLensMetricsProperty[] = "ro.dvr.lens_metrics";

  // This should be run on devices with and without built in metrics.
  bool has_metric = !base::GetProperty(kDvrLensMetricsProperty, "").empty();
  auto data2 = GetConfigData(DVR_CONFIGURATION_DATA_LENS_METRICS);
  if (has_metric) {
    ASSERT_STATUS_OK(data2);
    ASSERT_NE(0u, data2.get().size());
  } else {
    ASSERT_STATUS_ERROR(data2);
  }
}

}  // namespace

}  // namespace dvr
}  // namespace android
