include $(CLEAR_VARS)

LOCAL_MODULE := plat_mac_permissions.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

# Build keys.conf
plat_mac_perms_keys.tmp := $(intermediates)/plat_keys.tmp
$(plat_mac_perms_keys.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(plat_mac_perms_keys.tmp): $(call build_policy, keys.conf, $(PLAT_PRIVATE_POLICY) $(PRODUCT_PRIVATE_POLICY))
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $^ > $@

all_plat_mac_perms_files := $(call build_policy, mac_permissions.xml, $(PLAT_PRIVATE_POLICY))

# Should be synced with keys.conf.
all_plat_keys := platform media shared testkey
all_plat_keys := $(all_keys:%=$(dir $(DEFAULT_SYSTEM_DEV_CERTIFICATE))/%.x509.pem)

$(LOCAL_BUILT_MODULE): PRIVATE_MAC_PERMS_FILES := $(all_plat_mac_perms_files)
$(LOCAL_BUILT_MODULE): $(plat_mac_perms_keys.tmp) $(HOST_OUT_EXECUTABLES)/insertkeys.py \
$(all_plat_mac_perms_files) $(all_plat_keys)
	@mkdir -p $(dir $@)
	$(hide) DEFAULT_SYSTEM_DEV_CERTIFICATE="$(dir $(DEFAULT_SYSTEM_DEV_CERTIFICATE))" \
		$(HOST_OUT_EXECUTABLES)/insertkeys.py -t $(TARGET_BUILD_VARIANT) -c $(TOP) $< -o $@ $(PRIVATE_MAC_PERMS_FILES)

all_mac_perms_files :=
all_plat_keys :=
plat_mac_perms_keys.tmp :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := product_mac_permissions.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

# Build keys.conf
product_mac_perms_keys.tmp := $(intermediates)/product_keys.tmp
$(product_mac_perms_keys.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(product_mac_perms_keys.tmp): $(call build_policy, keys.conf, $(PRODUCT_PRIVATE_POLICY) $(REQD_MASK_POLICY))
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $^ > $@

all_product_mac_perms_files := $(call build_policy, mac_permissions.xml, $(PRODUCT_PRIVATE_POLICY) $(REQD_MASK_POLICY))

$(LOCAL_BUILT_MODULE): PRIVATE_MAC_PERMS_FILES := $(all_product_mac_perms_files)
$(LOCAL_BUILT_MODULE): $(product_mac_perms_keys.tmp) $(HOST_OUT_EXECUTABLES)/insertkeys.py \
$(all_product_mac_perms_files)
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/insertkeys.py -t $(TARGET_BUILD_VARIANT) -c $(TOP) $< -o $@ $(PRIVATE_MAC_PERMS_FILES)

product_mac_perms_keys.tmp :=
all_product_mac_perms_files :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := vendor_mac_permissions.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

# Build keys.conf
vendor_mac_perms_keys.tmp := $(intermediates)/vendor_keys.tmp
$(vendor_mac_perms_keys.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(vendor_mac_perms_keys.tmp): $(call build_policy, keys.conf, $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) $(REQD_MASK_POLICY))
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $^ > $@

all_vendor_mac_perms_files := $(call build_policy, mac_permissions.xml, $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) $(REQD_MASK_POLICY))

$(LOCAL_BUILT_MODULE): PRIVATE_MAC_PERMS_FILES := $(all_vendor_mac_perms_files)
$(LOCAL_BUILT_MODULE): $(vendor_mac_perms_keys.tmp) $(HOST_OUT_EXECUTABLES)/insertkeys.py \
$(all_vendor_mac_perms_files)
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/insertkeys.py -t $(TARGET_BUILD_VARIANT) -c $(TOP) $< -o $@ $(PRIVATE_MAC_PERMS_FILES)

vendor_mac_perms_keys.tmp :=
all_vendor_mac_perms_files :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := odm_mac_permissions.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

# Build keys.conf
odm_mac_perms_keys.tmp := $(intermediates)/odm_keys.tmp
$(odm_mac_perms_keys.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(odm_mac_perms_keys.tmp): $(call build_policy, keys.conf, $(BOARD_ODM_SEPOLICY_DIRS) $(REQD_MASK_POLICY))
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $^ > $@

all_odm_mac_perms_files := $(call build_policy, mac_permissions.xml, $(BOARD_ODM_SEPOLICY_DIRS) $(REQD_MASK_POLICY))

$(LOCAL_BUILT_MODULE): PRIVATE_MAC_PERMS_FILES := $(all_odm_mac_perms_files)
$(LOCAL_BUILT_MODULE): $(odm_mac_perms_keys.tmp) $(HOST_OUT_EXECUTABLES)/insertkeys.py \
$(all_odm_mac_perms_files)
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/insertkeys.py -t $(TARGET_BUILD_VARIANT) -c $(TOP) $< -o $@ $(PRIVATE_MAC_PERMS_FILES)

odm_mac_perms_keys.tmp :=
all_odm_mac_perms_files :=
