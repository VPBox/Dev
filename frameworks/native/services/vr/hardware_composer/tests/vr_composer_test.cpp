#include <android/dvr/BnVrComposerCallback.h>
#include <binder/IServiceManager.h>
#include <gtest/gtest.h>
#include <sys/eventfd.h>
#include <vr_composer.h>

namespace android {
namespace dvr {
namespace {

const char kVrDisplayName[] = "VrDisplay_Test";

class TestComposerView : public ComposerView {
 public:
  TestComposerView() {}
  ~TestComposerView() override = default;

  size_t display_refresh_count() const { return display_refresh_count_; }

  void ForceDisplaysRefresh() override { display_refresh_count_++; }
  void RegisterObserver(Observer* observer) override {}
  void UnregisterObserver(Observer* observer) override {}

  TestComposerView(const TestComposerView&) = delete;
  void operator=(const TestComposerView&) = delete;

 private:
  size_t display_refresh_count_ = 0;
};

class TestComposerCallback : public BnVrComposerCallback {
 public:
  TestComposerCallback() {}
  ~TestComposerCallback() override = default;

  ComposerView::Frame last_frame() const { return last_frame_; }

  binder::Status onNewFrame(
      const ParcelableComposerFrame& frame,
      ParcelableUniqueFd* /* fence */) override {
    last_frame_ = frame.frame();
    return binder::Status::ok();
  }

 private:
  ComposerView::Frame last_frame_;

  TestComposerCallback(const TestComposerCallback&) = delete;
  void operator=(const TestComposerCallback&) = delete;
};

class TestComposerCallbackWithFence : public TestComposerCallback {
 public:
  ~TestComposerCallbackWithFence() override = default;

  binder::Status onNewFrame(
      const ParcelableComposerFrame& frame,
      ParcelableUniqueFd* fence) override {
    binder::Status status = TestComposerCallback::onNewFrame(frame, fence);

    base::unique_fd fd(eventfd(0, 0));
    EXPECT_LE(0, fd.get());
    fence->set_fence(fd);

    return status;
  }
};

sp<GraphicBuffer> CreateBuffer() {
  return new GraphicBuffer(600, 400, PIXEL_FORMAT_RGBA_8888,
                           GraphicBuffer::USAGE_HW_TEXTURE);
}

}  // namespace

class VrComposerTest : public testing::Test {
 public:
  VrComposerTest() : composer_(new VrComposer(&composer_view_)) {}
  ~VrComposerTest() override = default;

  sp<IVrComposer> GetComposerProxy() const {
    sp<IServiceManager> sm(defaultServiceManager());
    return interface_cast<IVrComposer>(sm->getService(String16(kVrDisplayName)));
  }

  void SetUp() override {
    sp<IServiceManager> sm(defaultServiceManager());
    EXPECT_EQ(OK,
              sm->addService(String16(kVrDisplayName), composer_, false));
  }

 protected:
  TestComposerView composer_view_;
  sp<VrComposer> composer_;

  VrComposerTest(const VrComposerTest&) = delete;
  void operator=(const VrComposerTest&) = delete;
};

TEST_F(VrComposerTest, TestWithoutObserver) {
  sp<IVrComposer> composer = GetComposerProxy();
  ComposerView::Frame frame;

  base::unique_fd fence = composer_->OnNewFrame(frame);
  ASSERT_EQ(-1, fence.get());
}

TEST_F(VrComposerTest, TestWithObserver) {
  sp<IVrComposer> composer = GetComposerProxy();
  sp<TestComposerCallback> callback = new TestComposerCallback();
  ASSERT_EQ(0, composer_view_.display_refresh_count());
  ASSERT_TRUE(composer->registerObserver(callback).isOk());
  ASSERT_EQ(1, composer_view_.display_refresh_count());

  ComposerView::Frame frame;
  base::unique_fd fence = composer_->OnNewFrame(frame);
  ASSERT_EQ(-1, fence.get());
}

TEST_F(VrComposerTest, TestWithOneLayer) {
  sp<IVrComposer> composer = GetComposerProxy();
  sp<TestComposerCallback> callback = new TestComposerCallbackWithFence();
  ASSERT_TRUE(composer->registerObserver(callback).isOk());

  ComposerView::Frame frame;
  frame.display_id = 1;
  frame.removed = false;
  frame.display_width = 600;
  frame.display_height = 400;
  frame.layers.push_back(ComposerView::ComposerLayer{
    .id = 1,
    .buffer = CreateBuffer(),
    .fence = new Fence(eventfd(0, 0)),
    .display_frame = {0, 0, 600, 400},
    .crop = {0.0f, 0.0f, 600.0f, 400.0f},
    .blend_mode = IComposerClient::BlendMode::NONE,
    .alpha = 1.0f,
    .type = 1,
    .app_id = 1,
  });
  base::unique_fd fence = composer_->OnNewFrame(frame);
  ASSERT_LE(0, fence.get());

  ComposerView::Frame received_frame = callback->last_frame();
  ASSERT_EQ(frame.display_id, received_frame.display_id);
  ASSERT_EQ(frame.display_width, received_frame.display_width);
  ASSERT_EQ(frame.display_height, received_frame.display_height);
  ASSERT_EQ(frame.removed, received_frame.removed);
  ASSERT_EQ(1u, received_frame.layers.size());
  ASSERT_EQ(frame.layers[0].id, received_frame.layers[0].id);
  ASSERT_NE(nullptr, received_frame.layers[0].buffer.get());
  ASSERT_TRUE(received_frame.layers[0].fence->isValid());
  ASSERT_EQ(frame.layers[0].display_frame.left,
            received_frame.layers[0].display_frame.left);
  ASSERT_EQ(frame.layers[0].display_frame.top,
            received_frame.layers[0].display_frame.top);
  ASSERT_EQ(frame.layers[0].display_frame.right,
            received_frame.layers[0].display_frame.right);
  ASSERT_EQ(frame.layers[0].display_frame.bottom,
            received_frame.layers[0].display_frame.bottom);
  ASSERT_EQ(frame.layers[0].crop.left, received_frame.layers[0].crop.left);
  ASSERT_EQ(frame.layers[0].crop.top, received_frame.layers[0].crop.top);
  ASSERT_EQ(frame.layers[0].crop.right, received_frame.layers[0].crop.right);
  ASSERT_EQ(frame.layers[0].crop.bottom, received_frame.layers[0].crop.bottom);
  ASSERT_EQ(frame.layers[0].blend_mode, received_frame.layers[0].blend_mode);
  ASSERT_EQ(frame.layers[0].alpha, received_frame.layers[0].alpha);
  ASSERT_EQ(frame.layers[0].type, received_frame.layers[0].type);
  ASSERT_EQ(frame.layers[0].app_id, received_frame.layers[0].app_id);
}

}  // namespace dvr
}  // namespace android
