include $(CLEAR_VARS)

LOCAL_MODULE := plat_hwservice_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

plat_hwsvcfiles := $(call build_policy, hwservice_contexts, $(PLAT_PRIVATE_POLICY))

plat_hwservice_contexts.tmp := $(intermediates)/plat_hwservice_contexts.tmp
$(plat_hwservice_contexts.tmp): PRIVATE_SVC_FILES := $(plat_hwsvcfiles)
$(plat_hwservice_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(plat_hwservice_contexts.tmp): $(plat_hwsvcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(plat_hwservice_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc $(ACP)
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(HOST_OUT_EXECUTABLES)/checkfc -e -l $(PRIVATE_SEPOLICY) $@

plat_hwsvcfiles :=
plat_hwservice_contexts.tmp :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := product_hwservice_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

product_hwsvcfiles := $(call build_policy, hwservice_contexts, $(PRODUCT_PRIVATE_POLICY))

product_hwservice_contexts.tmp := $(intermediates)/product_hwservice_contexts.tmp
$(product_hwservice_contexts.tmp): PRIVATE_SVC_FILES := $(product_hwsvcfiles)
$(product_hwservice_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(product_hwservice_contexts.tmp): $(product_hwsvcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(product_hwservice_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(hide) $(HOST_OUT_EXECUTABLES)/checkfc -e -l $(PRIVATE_SEPOLICY) $@

product_hwsvcfiles :=
product_hwservice_contexts.tmp :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := vendor_hwservice_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

vendor_hwsvcfiles := $(call build_policy, hwservice_contexts, $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) $(REQD_MASK_POLICY))

vendor_hwservice_contexts.tmp := $(intermediates)/vendor_hwservice_contexts.tmp
$(vendor_hwservice_contexts.tmp): PRIVATE_SVC_FILES := $(vendor_hwsvcfiles)
$(vendor_hwservice_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(vendor_hwservice_contexts.tmp): $(vendor_hwsvcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(vendor_hwservice_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc $(ACP)
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(hide) $(HOST_OUT_EXECUTABLES)/checkfc -e -l $(PRIVATE_SEPOLICY) $@

vendor_hwsvcfiles :=
vendor_hwservice_contexts.tmp :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := odm_hwservice_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

odm_hwsvcfiles := $(call build_policy, hwservice_contexts, $(BOARD_ODM_SEPOLICY_DIRS))

odm_hwservice_contexts.tmp := $(intermediates)/odm_hwservice_contexts.tmp
$(odm_hwservice_contexts.tmp): PRIVATE_SVC_FILES := $(odm_hwsvcfiles)
$(odm_hwservice_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(odm_hwservice_contexts.tmp): $(odm_hwsvcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(odm_hwservice_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc $(ACP)
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(hide) $(HOST_OUT_EXECUTABLES)/checkfc -e -l $(PRIVATE_SEPOLICY) $@

odm_hwsvcfiles :=
odm_hwservice_contexts.tmp :=
