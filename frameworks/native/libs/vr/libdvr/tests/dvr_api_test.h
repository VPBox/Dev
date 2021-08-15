#include <dlfcn.h>
#include <dvr/dvr_api.h>

#include <gtest/gtest.h>

/** DvrTestBase loads the libdvr.so at runtime and get the Dvr API version 1. */
class DvrApiTest : public ::testing::Test {
 protected:
  void SetUp() override {
    int flags = RTLD_NOW | RTLD_LOCAL;

    // Here we need to ensure that libdvr is loaded with RTLD_NODELETE flag set
    // (so that calls to `dlclose` don't actually unload the library). This is a
    // workaround for an Android NDK bug. See more detail:
    // https://github.com/android-ndk/ndk/issues/360
    flags |= RTLD_NODELETE;
    platform_handle_ = dlopen("libdvr.google.so", flags);
    ASSERT_NE(nullptr, platform_handle_) << "Dvr shared library missing.";

    auto dvr_get_api = reinterpret_cast<decltype(&dvrGetApi)>(
        dlsym(platform_handle_, "dvrGetApi"));
    ASSERT_NE(nullptr, dvr_get_api) << "Platform library missing dvrGetApi.";

    ASSERT_EQ(dvr_get_api(&api_, sizeof(api_), /*version=*/1), 0)
        << "Unable to find compatible Dvr API.";
  }

  void TearDown() override {
    if (platform_handle_ != nullptr) {
      dlclose(platform_handle_);
    }
  }

  void* platform_handle_ = nullptr;
  DvrApi_v1 api_;
};
