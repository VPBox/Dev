#=====================================================================
# Include Subdirectories
#=====================================================================

# Not building RSoV tests in PDK builds.
ifneq ($(TARGET_BUILD_PDK), true)

LOCAL_PATH:=$(call my-dir)

include $(call all-makefiles-under,$(LOCAL_PATH))

endif # TARGET_BUILD_PDK
