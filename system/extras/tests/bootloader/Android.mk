LOCAL_PATH := $(call my-dir)

# Build a module that has all of the python files as its LOCAL_PICKUP_FILES.
# Since no action needs to be taken to compile the python source, just
# use BUILD_PHONY_PACKAGE to give us a target to execute.
include $(CLEAR_VARS)

LOCAL_MODULE := bootloader_unit_test
LOCAL_MODULE_TAGS := tests

bootloader_py_files := $(call find-subdir-files, *.py)

bootloader_zip_prefix := $(TARGET_OUT_DATA)/py_bootloader
bootloader_zip_path := $(bootloader_zip_prefix)/nativetest/py_bootloader

GEN := $(addprefix $(bootloader_zip_path)/, $(bootloader_py_files))
$(GEN) : PRIVATE_PATH := $(LOCAL_PATH)
$(GEN) : PRIVATE_CUSTOM_TOOL = cp $< $@
$(GEN) : $(bootloader_zip_path)/% : $(LOCAL_PATH)/%
	$(transform-generated-source)

LOCAL_PICKUP_FILES := $(bootloader_zip_prefix)/nativetest
LOCAL_ADDITIONAL_DEPENDENCIES := $(GEN)

include $(BUILD_PHONY_PACKAGE)
