#include <android/input.h>
#include <gtest/gtest.h>
#include <linux/input.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "EvdevInjector.h"
#include "VirtualTouchpadEvdev.h"

namespace android {
namespace dvr {

namespace {

class UInputForTesting : public EvdevInjector::UInput {
 public:
  ~UInputForTesting() override {}
  void WriteInputEvent(uint16_t type, uint16_t code, int32_t value) {
    struct input_event event;
    memset(&event, 0, sizeof(event));
    event.type = type;
    event.code = code;
    event.value = value;
    Write(&event, sizeof(event));
  }
};

// Recording test implementation of UInput.
//
class UInputRecorder : public UInputForTesting {
 public:
  UInputRecorder() {}
  ~UInputRecorder() override {}

  const std::string& GetString() const { return s_; }
  void Reset() { s_.clear(); }

  // UInput overrides:

  int Open() override {
    s_ += "o;";
    return 0;
  }

  int Close() override {
    s_ += "c;";
    return 0;
  }

  int Write(const void* buf, size_t count) override {
    s_ += "w(";
    s_ += Encode(&count, sizeof(count));
    s_ += ",";
    s_ += Encode(buf, count);
    s_ += ");";
    return 0;
  }

  int IoctlVoid(int request) override {
    s_ += "i(";
    s_ += Encode(&request, sizeof(request));
    s_ += ");";
    return 0;
  }

  int IoctlSetInt(int request, int value) override {
    s_ += "i(";
    s_ += Encode(&request, sizeof(request));
    s_ += ",";
    s_ += Encode(&value, sizeof(value));
    s_ += ");";
    return 0;
  }

 private:
  std::string s_;

  std::string Encode(const void* buf, size_t count) {
    const char* in = static_cast<const char*>(buf);
    char out[2 * count + 1];
    for (size_t i = 0; i < count; ++i) {
      snprintf(&out[2 * i], 3, "%02X", in[i]);
    }
    return out;
  }
};

class EvdevInjectorForTesting : public EvdevInjector {
 public:
  EvdevInjectorForTesting() { SetUInputForTesting(&record); }
  const uinput_user_dev* GetUiDev() const { return GetUiDevForTesting(); }
  UInputRecorder record;
};

class VirtualTouchpadForTesting : public VirtualTouchpadEvdev {
 public:
  static std::unique_ptr<VirtualTouchpad> Create() {
    return std::unique_ptr<VirtualTouchpad>(New());
  }
  static VirtualTouchpadForTesting* New() {
    VirtualTouchpadForTesting* const touchpad = new VirtualTouchpadForTesting();
    touchpad->Reset();
    for (int t = 0; t < kTouchpads; ++t) {
      touchpad->SetEvdevInjectorForTesting(t, &touchpad->injector[t]);
    }
    return touchpad;
  }
  int GetTouchpadCount() const { return kTouchpads; }
  EvdevInjectorForTesting injector[kTouchpads];
};

}  // anonymous namespace

class VirtualTouchpadTest : public testing::Test {};

TEST_F(VirtualTouchpadTest, Goodness) {
  std::unique_ptr<VirtualTouchpadForTesting> touchpad(
      VirtualTouchpadForTesting::New());
  UInputRecorder expect;

  status_t touch_status = touchpad->Attach();
  EXPECT_EQ(0, touch_status);

  // Check some aspects of uinput_user_dev.
  const uinput_user_dev* uidev;
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    uidev = touchpad->injector[t].GetUiDev();
    String8 name;
    name.appendFormat("vr-virtual-touchpad-%d", t);
    EXPECT_EQ(name, uidev->name);
    for (int i = 0; i < ABS_CNT; ++i) {
      EXPECT_EQ(0, uidev->absmin[i]);
      EXPECT_EQ(0, uidev->absfuzz[i]);
      EXPECT_EQ(0, uidev->absflat[i]);
      if (i != ABS_MT_POSITION_X && i != ABS_MT_POSITION_Y &&
          i != ABS_MT_SLOT) {
        EXPECT_EQ(0, uidev->absmax[i]);
      }
    }
  }
  const int32_t width = 1 + uidev->absmax[ABS_MT_POSITION_X];
  const int32_t height = 1 + uidev->absmax[ABS_MT_POSITION_Y];

  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    // Check the system calls performed by initialization.
    expect.Reset();
    // From ConfigureBegin():
    expect.Open();
    // From ConfigureInputProperty(INPUT_PROP_DIRECT):
    expect.IoctlSetInt(UI_SET_PROPBIT, INPUT_PROP_DIRECT);
    // From ConfigureMultiTouchXY(0, 0, kWidth - 1, kHeight - 1):
    expect.IoctlSetInt(UI_SET_EVBIT, EV_ABS);
    expect.IoctlSetInt(UI_SET_ABSBIT, ABS_MT_POSITION_X);
    expect.IoctlSetInt(UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    // From ConfigureAbsSlots(kSlots):
    expect.IoctlSetInt(UI_SET_ABSBIT, ABS_MT_SLOT);
    // From ConfigureRel(REL_WHEEL):
    expect.IoctlSetInt(UI_SET_EVBIT, EV_REL);
    expect.IoctlSetInt(UI_SET_RELBIT, REL_WHEEL);
    // From ConfigureRel(REL_HWHEEL):
    expect.IoctlSetInt(UI_SET_RELBIT, REL_HWHEEL);
    // From ConfigureKey(BTN_TOUCH):
    expect.IoctlSetInt(UI_SET_EVBIT, EV_KEY);
    expect.IoctlSetInt(UI_SET_KEYBIT, BTN_TOUCH);
    expect.IoctlSetInt(UI_SET_KEYBIT, BTN_BACK);
    // From ConfigureEnd():
    expect.Write(touchpad->injector[t].GetUiDev(), sizeof(uinput_user_dev));
    expect.IoctlVoid(UI_DEV_CREATE);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  expect.WriteInputEvent(EV_ABS, ABS_MT_SLOT, 0);
  expect.WriteInputEvent(EV_ABS, ABS_MT_TRACKING_ID, 0);
  expect.WriteInputEvent(EV_ABS, ABS_MT_POSITION_X, 0);
  expect.WriteInputEvent(EV_ABS, ABS_MT_POSITION_Y, 0);
  expect.WriteInputEvent(EV_SYN, SYN_REPORT, 0);
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->Touch(t, 0, 0, 0);
    EXPECT_EQ(0, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  expect.WriteInputEvent(EV_ABS, ABS_MT_TRACKING_ID, 0);
  expect.WriteInputEvent(EV_ABS, ABS_MT_POSITION_X, 0.25f * width);
  expect.WriteInputEvent(EV_ABS, ABS_MT_POSITION_Y, 0.75f * height);
  expect.WriteInputEvent(EV_KEY, BTN_TOUCH, EvdevInjector::KEY_PRESS);
  expect.WriteInputEvent(EV_SYN, SYN_REPORT, 0);
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->Touch(t, 0.25f, 0.75f, 0.5f);
    EXPECT_EQ(0, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  expect.WriteInputEvent(EV_ABS, ABS_MT_TRACKING_ID, 0);
  expect.WriteInputEvent(EV_ABS, ABS_MT_POSITION_X, 0.99f * width);
  expect.WriteInputEvent(EV_ABS, ABS_MT_POSITION_Y, 0.99f * height);
  expect.WriteInputEvent(EV_SYN, SYN_REPORT, 0);
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->Touch(t, 0.99f, 0.99f, 0.99f);
    EXPECT_EQ(0, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->Touch(t, 1.0f, 1.0f, 1.0f);
    EXPECT_EQ(EINVAL, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  expect.WriteInputEvent(EV_KEY, BTN_TOUCH, EvdevInjector::KEY_RELEASE);
  expect.WriteInputEvent(EV_ABS, ABS_MT_TRACKING_ID, -1);
  expect.WriteInputEvent(EV_SYN, SYN_REPORT, 0);
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->Touch(t, 0.25f, 0.75f, -0.01f);
    EXPECT_EQ(0, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  expect.WriteInputEvent(EV_KEY, BTN_BACK, EvdevInjector::KEY_PRESS);
  expect.WriteInputEvent(EV_SYN, SYN_REPORT, 0);
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->ButtonState(t, AMOTION_EVENT_BUTTON_BACK);
    EXPECT_EQ(0, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->ButtonState(t, AMOTION_EVENT_BUTTON_BACK);
    EXPECT_EQ(0, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  expect.WriteInputEvent(EV_KEY, BTN_BACK, EvdevInjector::KEY_RELEASE);
  expect.WriteInputEvent(EV_SYN, SYN_REPORT, 0);
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
    touch_status = touchpad->ButtonState(t, 0);
    EXPECT_EQ(0, touch_status);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }

  expect.Reset();
  expect.Close();
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    touchpad->injector[t].record.Reset();
  }
  touch_status = touchpad->Detach();
  EXPECT_EQ(0, touch_status);
  for (int t = 0; t < touchpad->GetTouchpadCount(); ++t) {
    SCOPED_TRACE(t);
    EXPECT_EQ(expect.GetString(), touchpad->injector[t].record.GetString());
  }
}

TEST_F(VirtualTouchpadTest, Badness) {
  std::unique_ptr<VirtualTouchpadForTesting> touchpad(
      VirtualTouchpadForTesting::New());
  UInputRecorder expect;
  UInputRecorder& record = touchpad->injector[VirtualTouchpad::PRIMARY].record;

  status_t touch_status = touchpad->Attach();
  EXPECT_EQ(0, touch_status);

  // Touch off-screen should return an error,
  // and should not result in any system calls.
  expect.Reset();
  record.Reset();
  touch_status = touchpad->Touch(VirtualTouchpad::PRIMARY, -0.25f, 0.75f, 1.0f);
  EXPECT_NE(OK, touch_status);
  touch_status = touchpad->Touch(VirtualTouchpad::PRIMARY, 0.25f, -0.75f, 1.0f);
  EXPECT_NE(OK, touch_status);
  touch_status = touchpad->Touch(VirtualTouchpad::PRIMARY, 1.25f, 0.75f, 1.0f);
  EXPECT_NE(OK, touch_status);
  touch_status = touchpad->Touch(VirtualTouchpad::PRIMARY, 0.25f, 1.75f, 1.0f);
  EXPECT_NE(OK, touch_status);
  EXPECT_EQ(expect.GetString(), record.GetString());

  // Unsupported button should return an error,
  // and should not result in any system calls.
  expect.Reset();
  record.Reset();
  touch_status = touchpad->ButtonState(VirtualTouchpad::PRIMARY,
                                       AMOTION_EVENT_BUTTON_FORWARD);
  EXPECT_NE(OK, touch_status);
  EXPECT_EQ(expect.GetString(), record.GetString());

  // Repeated attach is an error.
  touch_status = touchpad->Attach();
  EXPECT_NE(0, touch_status);
}

}  // namespace dvr
}  // namespace android
