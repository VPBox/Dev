include $(CLEAR_VARS)

LOCAL_MODULE := plat_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

plat_pcfiles := $(call build_policy, property_contexts, $(PLAT_PRIVATE_POLICY))
ifeq ($(PRODUCT_COMPATIBLE_PROPERTY),true)
plat_pcfiles += $(LOCAL_PATH)/public/property_contexts
endif

plat_property_contexts.tmp := $(intermediates)/plat_property_contexts.tmp
$(plat_property_contexts.tmp): PRIVATE_PC_FILES := $(plat_pcfiles)
$(plat_property_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(plat_property_contexts.tmp): $(plat_pcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_PC_FILES) > $@
$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(plat_property_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/property_info_checker
	@mkdir -p $(dir $@)
	$(hide) cp -f $< $@
	$(hide) $(HOST_OUT_EXECUTABLES)/property_info_checker $(PRIVATE_SEPOLICY) $@

built_plat_pc := $(LOCAL_BUILT_MODULE)
plat_pcfiles :=
plat_property_contexts.tmp :=

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := product_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

product_pcfiles := $(call build_policy, property_contexts, $(PRODUCT_PRIVATE_POLICY))

product_property_contexts.tmp := $(intermediates)/product_property_contexts.tmp
$(product_property_contexts.tmp): PRIVATE_PC_FILES := $(product_pcfiles)
$(product_property_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(product_property_contexts.tmp): $(product_pcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_PC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(product_property_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/property_info_checker
	@mkdir -p $(dir $@)
	$(hide) cp -f $< $@
	$(hide) $(HOST_OUT_EXECUTABLES)/property_info_checker $(PRIVATE_SEPOLICY) $@

built_product_pc := $(LOCAL_BUILT_MODULE)
product_pcfiles :=
product_property_contexts.tmp :=

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := vendor_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

vendor_pcfiles := $(call build_policy, property_contexts, $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) $(REQD_MASK_POLICY))

vendor_property_contexts.tmp := $(intermediates)/vendor_property_contexts.tmp
$(vendor_property_contexts.tmp): PRIVATE_PC_FILES := $(vendor_pcfiles)
$(vendor_property_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(vendor_property_contexts.tmp): $(vendor_pcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_PC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_BUILT_PLAT_PC := $(built_plat_pc)
$(LOCAL_BUILT_MODULE): $(vendor_property_contexts.tmp) $(built_sepolicy) $(built_plat_pc) $(HOST_OUT_EXECUTABLES)/property_info_checker
	@mkdir -p $(dir $@)
	$(hide) cp -f $< $@
	$(hide) $(HOST_OUT_EXECUTABLES)/property_info_checker $(PRIVATE_SEPOLICY) $(PRIVATE_BUILT_PLAT_PC) $@

built_vendor_pc := $(LOCAL_BUILT_MODULE)
vendor_pcfiles :=
vendor_property_contexts.tmp :=

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := odm_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

odm_pcfiles := $(call build_policy, property_contexts, $(BOARD_ODM_SEPOLICY_DIRS))

odm_property_contexts.tmp := $(intermediates)/odm_property_contexts.tmp
$(odm_property_contexts.tmp): PRIVATE_PC_FILES := $(odm_pcfiles)
$(odm_property_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(odm_property_contexts.tmp): $(odm_pcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_PC_FILES) > $@


$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): PRIVATE_BUILT_PLAT_PC := $(built_plat_pc)
$(LOCAL_BUILT_MODULE): PRIVATE_BUILT_VENDOR_PC := $(built_vendor_pc)
$(LOCAL_BUILT_MODULE): $(odm_property_contexts.tmp) $(built_sepolicy) $(built_plat_pc) $(built_vendor_pc) $(HOST_OUT_EXECUTABLES)/property_info_checker
	@mkdir -p $(dir $@)
	$(hide) cp -f $< $@
	$(hide) $(HOST_OUT_EXECUTABLES)/property_info_checker $(PRIVATE_SEPOLICY) $(PRIVATE_BUILT_PLAT_PC) $(PRIVATE_BUILT_VENDOR_PC) $@

built_odm_pc := $(LOCAL_BUILT_MODULE)
odm_pcfiles :=
odm_property_contexts.tmp :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := plat_property_contexts.recovery
LOCAL_MODULE_STEM := plat_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_plat_pc)
	$(hide) cp -f $< $@

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := product_property_contexts.recovery
LOCAL_MODULE_STEM := product_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_product_pc)
	$(hide) cp -f $< $@

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := vendor_property_contexts.recovery
LOCAL_MODULE_STEM := vendor_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_vendor_pc)
	$(hide) cp -f $< $@

##################################
include $(CLEAR_VARS)
LOCAL_MODULE := odm_property_contexts.recovery
LOCAL_MODULE_STEM := odm_property_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_odm_pc)
	$(hide) cp -f $< $@
