include $(CLEAR_VARS)

LOCAL_MODULE := plat_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

local_fc_files := $(call build_policy, file_contexts, $(PLAT_PRIVATE_POLICY))
ifneq ($(filter address,$(SANITIZE_TARGET)),)
  local_fc_files += $(wildcard $(addsuffix /file_contexts_asan, $(PLAT_PRIVATE_POLICY)))
endif
ifneq (,$(filter userdebug eng,$(TARGET_BUILD_VARIANT)))
  local_fc_files += $(wildcard $(addsuffix /file_contexts_overlayfs, $(PLAT_PRIVATE_POLICY)))
endif
ifeq ($(TARGET_FLATTEN_APEX),true)
  apex_fc_files := $(wildcard $(LOCAL_PATH)/apex/*-file_contexts)
  $(foreach _input,$(apex_fc_files),\
    $(eval _output := $(intermediates)/$(notdir $(_input))-flattened)\
    $(eval _apex_name := $(patsubst %-file_contexts,%,$(notdir $(_input))))\
    $(eval $(call build_flattened_apex_file_contexts,$(_input),$(_apex_name),$(_output),local_fc_files))\
   )
endif
local_fcfiles_with_nl := $(call add_nl, $(local_fc_files), $(built_nl))

$(LOCAL_BUILT_MODULE): PRIVATE_FC_FILES := $(local_fcfiles_with_nl)
$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_FC_SORT := $(HOST_OUT_EXECUTABLES)/fc_sort
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/checkfc $(HOST_OUT_EXECUTABLES)/fc_sort \
$(local_fcfiles_with_nl) $(built_sepolicy)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_FC_FILES) > $@.tmp
	$(hide) $< $(PRIVATE_SEPOLICY) $@.tmp
	$(hide) $(PRIVATE_FC_SORT) $@.tmp $@

built_plat_fc := $(LOCAL_BUILT_MODULE)
local_fc_files :=
local_fcfiles_with_nl :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := product_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

product_fc_files := $(call build_policy, file_contexts, $(PRODUCT_PRIVATE_POLICY))
product_fcfiles_with_nl := $(call add_nl, $(product_fc_files), $(built_nl))

$(LOCAL_BUILT_MODULE): PRIVATE_FC_FILES := $(product_fcfiles_with_nl)
$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(LOCAL_BUILT_MODULE): PRIVATE_FC_SORT := $(HOST_OUT_EXECUTABLES)/fc_sort
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/checkfc $(HOST_OUT_EXECUTABLES)/fc_sort \
$(product_fcfiles_with_nl) $(built_sepolicy)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_FC_FILES) > $@.tmp
	$(hide) $< $(PRIVATE_SEPOLICY) $@.tmp
	$(hide) $(PRIVATE_FC_SORT) $@.tmp $@

built_product_fc := $(LOCAL_BUILT_MODULE)
product_fc_files :=
product_fcfiles_with_nl :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := vendor_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

vendor_fc_files := $(call build_vendor_policy, file_contexts)
vendor_fcfiles_with_nl := $(call add_nl, $(vendor_fc_files), $(built_nl))

$(LOCAL_BUILT_MODULE): PRIVATE_FC_FILES := $(vendor_fcfiles_with_nl)
$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(LOCAL_BUILT_MODULE): PRIVATE_FC_SORT := $(HOST_OUT_EXECUTABLES)/fc_sort
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/checkfc $(HOST_OUT_EXECUTABLES)/fc_sort \
$(vendor_fcfiles_with_nl) $(built_sepolicy)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_FC_FILES) > $@.tmp
	$(hide) $< $(PRIVATE_SEPOLICY) $@.tmp
	$(hide) $(PRIVATE_FC_SORT) $@.tmp $@

built_vendor_fc := $(LOCAL_BUILT_MODULE)
vendor_fc_files :=
vendor_fcfiles_with_nl :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := odm_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

odm_fc_files := $(call build_odm_policy, file_contexts)
odm_fcfiles_with_nl := $(call add_nl, $(odm_fc_files), $(built_nl))

$(LOCAL_BUILT_MODULE): PRIVATE_FC_FILES := $(odm_fcfiles_with_nl)
$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(LOCAL_BUILT_MODULE): PRIVATE_FC_SORT := $(HOST_OUT_EXECUTABLES)/fc_sort
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/checkfc $(HOST_OUT_EXECUTABLES)/fc_sort \
$(odm_fcfiles_with_nl) $(built_sepolicy)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_FC_FILES) > $@.tmp
	$(hide) $< $(PRIVATE_SEPOLICY) $@.tmp
	$(hide) $(PRIVATE_FC_SORT) $@.tmp $@

built_odm_fc := $(LOCAL_BUILT_MODULE)
odm_fc_files :=
odm_fcfiles_with_nl :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := plat_file_contexts.recovery
LOCAL_MODULE_STEM := plat_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_plat_fc)
	$(hide) cp -f $< $@

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := product_file_contexts.recovery
LOCAL_MODULE_STEM := product_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_product_fc)
	$(hide) cp -f $< $@

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := vendor_file_contexts.recovery
LOCAL_MODULE_STEM := vendor_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_vendor_fc)
	$(hide) cp -f $< $@

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := odm_file_contexts.recovery
LOCAL_MODULE_STEM := odm_file_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_odm_fc)
	$(hide) cp -f $< $@
