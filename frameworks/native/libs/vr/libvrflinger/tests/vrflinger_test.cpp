#include <android/hardware/configstore/1.0/ISurfaceFlingerConfigs.h>
#include <android/hardware/configstore/1.1/types.h>
#include <android/hardware_buffer.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <configstore/Utils.h>
#include <cutils/properties.h>
#include <gtest/gtest.h>
#include <gui/ISurfaceComposer.h>
#include <log/log.h>
#include <utils/StrongPointer.h>

#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>

#include <private/dvr/display_client.h>

using namespace android::hardware::configstore;
using namespace android::hardware::configstore::V1_0;
using android::dvr::display::DisplayClient;
using android::dvr::display::Surface;
using android::dvr::display::SurfaceAttribute;
using android::dvr::display::SurfaceAttributeValue;

namespace android {
namespace dvr {

// The transaction code for asking surface flinger if vr flinger is active. This
// is done as a hidden api since it's only used for tests. See the "case 1028"
// block in SurfaceFlinger::onTransact() in SurfaceFlinger.cpp.
constexpr uint32_t kIsVrFlingerActiveTransactionCode = 1028;

// The maximum amount of time to give vr flinger to activate/deactivate. If the
// switch hasn't completed in this amount of time, the test will fail.
constexpr auto kVrFlingerSwitchMaxTime = std::chrono::seconds(1);

// How long to wait between each check to see if the vr flinger switch
// completed.
constexpr auto kVrFlingerSwitchPollInterval = std::chrono::milliseconds(50);

// How long to wait for a device that boots to VR to have vr flinger ready.
constexpr auto kBootVrFlingerWaitTimeout = std::chrono::seconds(30);

// A Binder connection to surface flinger.
class SurfaceFlingerConnection {
 public:
  static std::unique_ptr<SurfaceFlingerConnection> Create() {
    sp<ISurfaceComposer> surface_flinger = interface_cast<ISurfaceComposer>(
        defaultServiceManager()->getService(String16("SurfaceFlinger")));
    if (surface_flinger == nullptr) {
      return nullptr;
    }

    return std::unique_ptr<SurfaceFlingerConnection>(
        new SurfaceFlingerConnection(surface_flinger));
  }

  // Returns true if the surface flinger process is still running. We use this
  // to detect if surface flinger has crashed.
  bool IsAlive() {
    IInterface::asBinder(surface_flinger_)->pingBinder();
    return IInterface::asBinder(surface_flinger_)->isBinderAlive();
  }

  // Return true if vr flinger is currently active, false otherwise. If there's
  // an error communicating with surface flinger, std::nullopt is returned.
  std::optional<bool> IsVrFlingerActive() {
    Parcel data, reply;
    status_t result =
        data.writeInterfaceToken(surface_flinger_->getInterfaceDescriptor());
    if (result != OK) {
      return std::nullopt;
    }
    result = IInterface::asBinder(surface_flinger_)
                 ->transact(kIsVrFlingerActiveTransactionCode, data, &reply);
    if (result != OK) {
      return std::nullopt;
    }
    bool vr_flinger_active;
    result = reply.readBool(&vr_flinger_active);
    if (result != OK) {
      return std::nullopt;
    }
    return vr_flinger_active;
  }

  enum class VrFlingerSwitchResult : int8_t {
    kSuccess,
    kTimedOut,
    kCommunicationError,
    kSurfaceFlingerDied
  };

  // Wait for vr flinger to become active or inactive.
  VrFlingerSwitchResult WaitForVrFlinger(bool wait_active) {
    return WaitForVrFlingerTimed(wait_active, kVrFlingerSwitchPollInterval,
        kVrFlingerSwitchMaxTime);
  }

  // Wait for vr flinger to become active or inactive, specifying custom timeouts.
  VrFlingerSwitchResult WaitForVrFlingerTimed(bool wait_active,
      std::chrono::milliseconds pollInterval, std::chrono::seconds timeout) {
    auto start_time = std::chrono::steady_clock::now();
    while (1) {
      std::this_thread::sleep_for(pollInterval);
      if (!IsAlive()) {
        return VrFlingerSwitchResult::kSurfaceFlingerDied;
      }
      std::optional<bool> vr_flinger_active = IsVrFlingerActive();
      if (!vr_flinger_active.has_value()) {
        return VrFlingerSwitchResult::kCommunicationError;
      }
      if (vr_flinger_active.value() == wait_active) {
        return VrFlingerSwitchResult::kSuccess;
      } else if (std::chrono::steady_clock::now() - start_time > timeout) {
        return VrFlingerSwitchResult::kTimedOut;
      }
    }
  }

 private:
  SurfaceFlingerConnection(sp<ISurfaceComposer> surface_flinger)
      : surface_flinger_(surface_flinger) {}

  sp<ISurfaceComposer> surface_flinger_ = nullptr;
};

// This test activates vr flinger by creating a vr flinger surface, then
// deactivates vr flinger by destroying the surface. We verify that vr flinger
// is activated and deactivated as expected, and that surface flinger doesn't
// crash.
//
// If the device doesn't support vr flinger (as repoted by ConfigStore), the
// test does nothing.
//
// If the device is a standalone vr device, the test also does nothing, since
// this test verifies the behavior of display handoff from surface flinger to vr
// flinger and back, and standalone devices never hand control of the display
// back to surface flinger.
TEST(VrFlingerTest, ActivateDeactivate) {
  android::ProcessState::self()->startThreadPool();

  // Exit immediately if the device doesn't support vr flinger. This ConfigStore
  // check is the same mechanism used by surface flinger to decide if it should
  // initialize vr flinger.
  bool vr_flinger_enabled =
      getBool<ISurfaceFlingerConfigs, &ISurfaceFlingerConfigs::useVrFlinger>(
          false);
  if (!vr_flinger_enabled) {
    return;
  }

  // This test doesn't apply to standalone vr devices.
  if (property_get_bool("ro.boot.vr", false)) {
    return;
  }

  auto surface_flinger_connection = SurfaceFlingerConnection::Create();
  ASSERT_NE(surface_flinger_connection, nullptr);

  // Verify we start off with vr flinger disabled.
  ASSERT_TRUE(surface_flinger_connection->IsAlive());
  auto vr_flinger_active = surface_flinger_connection->IsVrFlingerActive();
  ASSERT_TRUE(vr_flinger_active.has_value());
  ASSERT_FALSE(vr_flinger_active.value());

  // Create a vr flinger surface, and verify vr flinger becomes active.
  // Introduce a scope so that, at the end of the scope, the vr flinger surface
  // is destroyed, and vr flinger deactivates.
  {
    auto display_client = DisplayClient::Create();
    ASSERT_NE(display_client, nullptr);
    auto metrics = display_client->GetDisplayMetrics();
    ASSERT_TRUE(metrics.ok());

    auto surface = Surface::CreateSurface({
        {SurfaceAttribute::Direct, SurfaceAttributeValue(true)},
        {SurfaceAttribute::Visible, SurfaceAttributeValue(true)},
    });
    ASSERT_TRUE(surface.ok());
    ASSERT_TRUE(surface.get() != nullptr);

    auto queue = surface.get()->CreateQueue(
        metrics.get().display_width, metrics.get().display_height,
        /*layer_count=*/1, AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM,
        AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE |
            AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT |
            AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
        /*capacity=*/1,
        /*metadata_size=*/0);
    ASSERT_TRUE(queue.ok());
    ASSERT_TRUE(queue.get() != nullptr);

    size_t slot;
    pdx::LocalHandle release_fence;
    auto buffer = queue.get()->Dequeue(/*timeout=*/0, &slot, &release_fence);
    ASSERT_TRUE(buffer.ok());
    ASSERT_TRUE(buffer.get() != nullptr);

    ASSERT_EQ(buffer.get()->width(), metrics.get().display_width);
    ASSERT_EQ(buffer.get()->height(), metrics.get().display_height);

    void* raw_buf = nullptr;
    ASSERT_GE(buffer.get()->buffer()->Lock(
                  AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN, /*x=*/0, /*y=*/0,
                  buffer.get()->width(), buffer.get()->height(), &raw_buf),
              0);
    ASSERT_NE(raw_buf, nullptr);
    uint32_t* pixels = static_cast<uint32_t*>(raw_buf);

    for (int i = 0; i < buffer.get()->stride() * buffer.get()->height(); ++i) {
      pixels[i] = 0x0000ff00;
    }

    ASSERT_GE(buffer.get()->buffer()->Unlock(), 0);

    ASSERT_GE(buffer.get()->Post(/*ready_fence=*/pdx::LocalHandle()), 0);

    ASSERT_EQ(
        surface_flinger_connection->WaitForVrFlinger(/*wait_active=*/true),
        SurfaceFlingerConnection::VrFlingerSwitchResult::kSuccess);
  }

  // Now that the vr flinger surface is destroyed, vr flinger should deactivate.
  ASSERT_EQ(
      surface_flinger_connection->WaitForVrFlinger(/*wait_active=*/false),
      SurfaceFlingerConnection::VrFlingerSwitchResult::kSuccess);
}

// This test runs only on devices that boot to vr. Such a device should boot to
// a state where vr flinger is running, and the test verifies this after a
// delay.
TEST(BootVrFlingerTest, BootsToVrFlinger) {
  // Exit if we are not running on a device that boots to vr.
  if (!property_get_bool("ro.boot.vr", false)) {
    return;
  }

  auto surface_flinger_connection = SurfaceFlingerConnection::Create();
  ASSERT_NE(surface_flinger_connection, nullptr);

  // Verify that vr flinger is enabled.
  ASSERT_TRUE(surface_flinger_connection->IsAlive());
  auto vr_flinger_active = surface_flinger_connection->IsVrFlingerActive();
  ASSERT_TRUE(vr_flinger_active.has_value());

  bool active_value = vr_flinger_active.value();
  if (!active_value) {
    // Try again, but delay up to 30 seconds.
    ASSERT_EQ(surface_flinger_connection->WaitForVrFlingerTimed(true,
        kVrFlingerSwitchPollInterval, kBootVrFlingerWaitTimeout),
        SurfaceFlingerConnection::VrFlingerSwitchResult::kSuccess);
  }
}

}  // namespace dvr
}  // namespace android
