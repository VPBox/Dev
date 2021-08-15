include $(CLEAR_VARS)
LOCAL_MODULE := plat_seapp_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

plat_sc_files := $(call build_policy, seapp_contexts, $(PLAT_PRIVATE_POLICY))

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_SC_FILES := $(plat_sc_files)
$(LOCAL_BUILT_MODULE): $(built_sepolicy) $(plat_sc_files) $(HOST_OUT_EXECUTABLES)/checkseapp
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/checkseapp -p $(PRIVATE_SEPOLICY) -o $@ $(PRIVATE_SC_FILES)

built_plat_sc := $(LOCAL_BUILT_MODULE)
plat_sc_files :=

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := product_seapp_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

product_sc_files := $(call build_policy, seapp_contexts, $(PRODUCT_PRIVATE_POLICY))
plat_sc_neverallow_files := $(call build_policy, seapp_contexts, $(PLAT_PRIVATE_POLICY))

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_SC_FILES := $(product_sc_files)
$(LOCAL_BUILT_MODULE): PRIVATE_SC_NEVERALLOW_FILES := $(plat_sc_neverallow_files)
$(LOCAL_BUILT_MODULE): $(built_sepolicy) $(product_sc_files) $(HOST_OUT_EXECUTABLES)/checkseapp $(plat_sc_neverallow_files)
	@mkdir -p $(dir $@)
	$(hide) grep -ihe '^neverallow' $(PRIVATE_SC_NEVERALLOW_FILES) > $@.tmp
	$(hide) $(HOST_OUT_EXECUTABLES)/checkseapp -p $(PRIVATE_SEPOLICY) -o $@ $(PRIVATE_SC_FILES) $@.tmp

product_sc_files :=
plat_sc_neverallow_files :=

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := vendor_seapp_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

vendor_sc_files := $(call build_policy, seapp_contexts, $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) $(REQD_MASK_POLICY))
plat_sc_neverallow_files := $(call build_policy, seapp_contexts, $(PLAT_PRIVATE_POLICY) $(PRODUCT_PRIVATE_POLICY))

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_SC_FILES := $(vendor_sc_files)
$(LOCAL_BUILT_MODULE): PRIVATE_SC_NEVERALLOW_FILES := $(plat_sc_neverallow_files)
$(LOCAL_BUILT_MODULE): $(built_sepolicy) $(vendor_sc_files) $(HOST_OUT_EXECUTABLES)/checkseapp $(plat_sc_neverallow_files)
	@mkdir -p $(dir $@)
	$(hide) grep -ihe '^neverallow' $(PRIVATE_SC_NEVERALLOW_FILES) > $@.tmp
	$(hide) $(HOST_OUT_EXECUTABLES)/checkseapp -p $(PRIVATE_SEPOLICY) -o $@ $(PRIVATE_SC_FILES) $@.tmp

built_vendor_sc := $(LOCAL_BUILT_MODULE)
vendor_sc_files :=

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := odm_seapp_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

odm_sc_files := $(call build_policy, seapp_contexts, $(BOARD_ODM_SEPOLICY_DIRS))
plat_sc_neverallow_files := $(call build_policy, seapp_contexts, $(PLAT_PRIVATE_POLICY) $(PRODUCT_PRIVATE_POLICY))

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_SC_FILES := $(odm_sc_files)
$(LOCAL_BUILT_MODULE): PRIVATE_SC_NEVERALLOW_FILES := $(plat_sc_neverallow_files)
$(LOCAL_BUILT_MODULE): $(built_sepolicy) $(odm_sc_files) $(HOST_OUT_EXECUTABLES)/checkseapp $(plat_sc_neverallow_files)
	@mkdir -p $(dir $@)
	$(hide) grep -ihe '^neverallow' $(PRIVATE_SC_NEVERALLOW_FILES) > $@.tmp
	$(hide) $(HOST_OUT_EXECUTABLES)/checkseapp -p $(PRIVATE_SEPOLICY) -o $@ $(PRIVATE_SC_FILES) $@.tmp

built_odm_sc := $(LOCAL_BUILT_MODULE)
odm_sc_files :=

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := plat_seapp_neverallows
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := tests

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(plat_sc_neverallow_files)
	@mkdir -p $(dir $@)
	- $(hide) grep -ihe '^neverallow' $< > $@

plat_sc_neverallow_files :=
