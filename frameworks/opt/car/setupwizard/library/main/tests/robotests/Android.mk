LOCAL_PATH := $(call my-dir)

###############################################################
# CarSetupWizardLib app just for Robolectric test target.     #
###############################################################
include $(CLEAR_VARS)

LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res

LOCAL_PACKAGE_NAME := CarSetupWizardLib
LOCAL_PRIVATE_PLATFORM_APIS := true
LOCAL_MODULE_TAGS := optional

LOCAL_USE_AAPT2 := true

LOCAL_PRIVILEGED_MODULE := true

LOCAL_JAVA_LIBRARIES := android.car

LOCAL_STATIC_ANDROID_LIBRARIES += car-setup-wizard-lib

include $(BUILD_PACKAGE)

###############################################################
# CarSetupWizardLib Robolectric test target.                  #
###############################################################
include $(CLEAR_VARS)

LOCAL_MODULE := CarSetupWizardLibRoboTests
LOCAL_MODULE_CLASS := JAVA_LIBRARIES

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_RESOURCE_DIR := $(LOCAL_PATH)/res

LOCAL_JAVA_RESOURCE_DIRS := config

LOCAL_JAVA_LIBRARIES := \
    android.car \
    robolectric_android-all-stub \
    Robolectric_all-target \
    mockito-robolectric-prebuilt \
    truth-prebuilt

LOCAL_INSTRUMENTATION_FOR := CarSetupWizardLib

LOCAL_MODULE_TAGS := optional

# Generate test_config.properties
include external/robolectric-shadows/gen_test_config.mk

include $(BUILD_STATIC_JAVA_LIBRARY)

###############################################################
# CarSetupWizardLib runner target to run the previous target. #
###############################################################
include $(CLEAR_VARS)
LOCAL_MODULE := RunCarSetupWizardLibRoboTests

LOCAL_JAVA_LIBRARIES := \
    android.car \
    CarSetupWizardLibRoboTests \
    robolectric_android-all-stub \
    Robolectric_all-target \
    mockito-robolectric-prebuilt \
    truth-prebuilt

LOCAL_TEST_PACKAGE := CarSetupWizardLib

LOCAL_ROBOTEST_FILES := $(filter-out %/BaseRobolectricTest.java,\
    $(call find-files-in-subdirs,$(LOCAL_PATH)/src,*Test.java,.))

LOCAL_INSTRUMENT_SOURCE_DIRS := $(dir $(LOCAL_PATH))../src

include external/robolectric-shadows/run_robotests.mk
