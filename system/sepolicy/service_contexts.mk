include $(CLEAR_VARS)

LOCAL_MODULE := plat_service_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

plat_svcfiles := $(call build_policy, service_contexts, $(PLAT_PRIVATE_POLICY))

plat_service_contexts.tmp := $(intermediates)/plat_service_contexts.tmp
$(plat_service_contexts.tmp): PRIVATE_SVC_FILES := $(plat_svcfiles)
$(plat_service_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(plat_service_contexts.tmp): $(plat_svcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(plat_service_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc $(ACP)
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(HOST_OUT_EXECUTABLES)/checkfc -s $(PRIVATE_SEPOLICY) $@

built_plat_svc := $(LOCAL_BUILT_MODULE)
plat_svcfiles :=
plat_service_contexts.tmp :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := product_service_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

product_svcfiles := $(call build_policy, service_contexts, $(PRODUCT_PRIVATE_POLICY))

product_service_contexts.tmp := $(intermediates)/product_service_contexts.tmp
$(product_service_contexts.tmp): PRIVATE_SVC_FILES := $(product_svcfiles)
$(product_service_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(product_service_contexts.tmp): $(product_svcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(product_service_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(HOST_OUT_EXECUTABLES)/checkfc -s $(PRIVATE_SEPOLICY) $@

product_svcfiles :=
product_service_contexts.tmp :=

##################################
# nonplat_service_contexts is only allowed on non-full-treble devices
ifneq ($(PRODUCT_SEPOLICY_SPLIT),true)

include $(CLEAR_VARS)

LOCAL_MODULE := vendor_service_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

vendor_svcfiles := $(call build_policy, service_contexts, $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) $(REQD_MASK_POLICY))

vendor_service_contexts.tmp := $(intermediates)/vendor_service_contexts.tmp
$(vendor_service_contexts.tmp): PRIVATE_SVC_FILES := $(vendor_svcfiles)
$(vendor_service_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(vendor_service_contexts.tmp): $(vendor_svcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(vendor_service_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc $(ACP)
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(hide) $(HOST_OUT_EXECUTABLES)/checkfc -s $(PRIVATE_SEPOLICY) $@

built_vendor_svc := $(LOCAL_BUILT_MODULE)
vendor_svcfiles :=
vendor_service_contexts.tmp :=

endif
