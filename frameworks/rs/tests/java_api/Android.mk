LOCAL_PATH := $(call my-dir)

ifneq (true,$(TARGET_BUILD_PDK))

# Set RSTESTFORWARD_TARGETS
include $(LOCAL_PATH)/RSTestForward/Targets.mk

include $(call all-makefiles-under,$(LOCAL_PATH))


# RSUnbundledTests
# Tests that run on older devices

my_modules := \
    RSTestBackward \
    RSTest_Compat \

my_package_name := RSUnbundledTests

include $(BUILD_SYSTEM)/tasks/tools/package-modules.mk

.PHONY: RSUnbundledTests
RSUnbundledTests: $(my_package_zip)

$(call dist-for-goals,RSUnbundledTests,$(my_package_zip):$(my_package_name).zip)


# RSUnbundledTests19
# Tests that run on older devices (API 19 and below)

my_modules := \
    RSTestBackward19 \
    RSTest_Compat19 \

my_package_name := RSUnbundledTests19

include $(BUILD_SYSTEM)/tasks/tools/package-modules.mk

$(call dist-for-goals,RSUnbundledTests,$(my_package_zip):$(my_package_name).zip)


# RSTests
# Tests that run on latest platform build

ifneq ($(ENABLE_RSTESTS),)

my_modules := \
    RSTest \
    RSTest_Compat \
    $(RSTESTFORWARD_TARGETS) \

my_package_name := RSTests

include $(BUILD_SYSTEM)/tasks/tools/package-modules.mk

.PHONY: RSTests
RSTests: $(my_package_zip)

$(call dist-for-goals,RSTests,$(my_package_zip):$(my_package_name).zip)

endif

RSTESTFORWARD_TARGETS :=

endif
