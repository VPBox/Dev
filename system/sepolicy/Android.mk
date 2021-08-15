LOCAL_PATH:= $(call my-dir)

include $(LOCAL_PATH)/definitions.mk
include $(LOCAL_PATH)/policy_version.mk

include $(CLEAR_VARS)

MLS_SENS=1
MLS_CATS=1024

ifdef BOARD_SEPOLICY_UNION
$(warning BOARD_SEPOLICY_UNION is no longer required - all files found in BOARD_SEPOLICY_DIRS are implicitly unioned; please remove from your BoardConfig.mk or other .mk file.)
endif

ifdef BOARD_SEPOLICY_M4DEFS
LOCAL_ADDITIONAL_M4DEFS := $(addprefix -D, $(BOARD_SEPOLICY_M4DEFS))
else
LOCAL_ADDITIONAL_M4DEFS :=
endif

# sepolicy is now divided into multiple portions:
# public - policy exported on which non-platform policy developers may write
#   additional policy.  types and attributes are versioned and included in
#   delivered non-platform policy, which is to be combined with platform policy.
# private - platform-only policy required for platform functionality but which
#  is not exported to vendor policy developers and as such may not be assumed
#  to exist.
# vendor - vendor-only policy required for vendor functionality. This policy can
#  reference the public policy but cannot reference the private policy. This
#  policy is for components which are produced from the core/non-vendor tree and
#  placed into a vendor partition.
# mapping - This contains policy statements which map the attributes
#  exposed in the public policy of previous versions to the concrete types used
#  in this policy to ensure that policy targeting attributes from public
#  policy from an older platform version continues to work.

# build process for device:
# 1) convert policies to CIL:
#    - private + public platform policy to CIL
#    - mapping file to CIL (should already be in CIL form)
#    - non-platform public policy to CIL
#    - non-platform public + private policy to CIL
# 2) attributize policy
#    - run script which takes non-platform public and non-platform combined
#      private + public policy and produces attributized and versioned
#      non-platform policy
# 3) combine policy files
#    - combine mapping, platform and non-platform policy.
#    - compile output binary policy file

PLAT_PUBLIC_POLICY := $(LOCAL_PATH)/public
ifneq ( ,$(BOARD_PLAT_PUBLIC_SEPOLICY_DIR))
PLAT_PUBLIC_POLICY += $(BOARD_PLAT_PUBLIC_SEPOLICY_DIR)
endif
PLAT_PRIVATE_POLICY := $(LOCAL_PATH)/private
ifneq ( ,$(BOARD_PLAT_PRIVATE_SEPOLICY_DIR))
PLAT_PRIVATE_POLICY += $(BOARD_PLAT_PRIVATE_SEPOLICY_DIR)
endif
PLAT_VENDOR_POLICY := $(LOCAL_PATH)/vendor
REQD_MASK_POLICY := $(LOCAL_PATH)/reqd_mask
PRODUCT_PUBLIC_POLICY := $(PRODUCT_PUBLIC_SEPOLICY_DIRS)
PRODUCT_PRIVATE_POLICY := $(PRODUCT_PRIVATE_SEPOLICY_DIRS)

# TODO(b/119305624): Currently if the device doesn't have a product partition,
# we install product sepolicy into /system/product. We do that because bits of
# product sepolicy that's still in /system might depend on bits that have moved
# to /product. Once we finish migrating product sepolicy out of system, change
# it so that if no product partition is present, product sepolicy artifacts are
# not built and installed at all.
ifneq (,$(PRODUCT_PUBLIC_POLICY)$(PRODUCT_PRIVATE_POLICY))
HAS_PRODUCT_SEPOLICY := true
endif

# TODO: move to README when doing the README update and finalizing versioning.
# BOARD_SEPOLICY_VERS must take the format "NN.m" and contain the sepolicy
# version identifier corresponding to the sepolicy on which the non-platform
# policy is to be based. If unspecified, this will build against the current
# public platform policy in tree
ifndef BOARD_SEPOLICY_VERS
# The default platform policy version.
BOARD_SEPOLICY_VERS := $(PLATFORM_SEPOLICY_VERSION)
endif

NEVERALLOW_ARG :=
ifeq ($(SELINUX_IGNORE_NEVERALLOWS),true)
ifeq ($(TARGET_BUILD_VARIANT),user)
$(error SELINUX_IGNORE_NEVERALLOWS := true cannot be used in user builds)
endif
$(warning Be careful when using the SELINUX_IGNORE_NEVERALLOWS flag. \
          It does not work in user builds and using it will \
          not stop you from failing CTS.)
NEVERALLOW_ARG := -N
endif

# BOARD_SEPOLICY_DIRS was used for vendor/odm sepolicy customization before.
# It has been replaced by BOARD_VENDOR_SEPOLICY_DIRS (mandatory) and
# BOARD_ODM_SEPOLICY_DIRS (optional). BOARD_SEPOLICY_DIRS is still allowed for
# backward compatibility, which will be merged into BOARD_VENDOR_SEPOLICY_DIRS.
ifdef BOARD_SEPOLICY_DIRS
BOARD_VENDOR_SEPOLICY_DIRS += $(BOARD_SEPOLICY_DIRS)
endif

ifdef BOARD_ODM_SEPOLICY_DIRS
ifneq ($(PRODUCT_SEPOLICY_SPLIT),true)
$(error PRODUCT_SEPOLICY_SPLIT needs to be true when using BOARD_ODM_SEPOLICY_DIRS)
endif
endif

###########################################################
# Compute policy files to be used in policy build.
# $(1): files to include
# $(2): directories in which to find files
###########################################################

define build_policy
$(foreach type, $(1), $(foreach file, $(addsuffix /$(type), $(2)), $(sort $(wildcard $(file)))))
endef

# Builds paths for all policy files found in BOARD_VENDOR_SEPOLICY_DIRS.
# $(1): the set of policy name paths to build
build_vendor_policy = $(call build_policy, $(1), $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS))

# Builds paths for all policy files found in BOARD_ODM_SEPOLICY_DIRS.
build_odm_policy = $(call build_policy, $(1), $(BOARD_ODM_SEPOLICY_DIRS))

# Add a file containing only a newline in-between each policy configuration
# 'contexts' file. This will allow OEM policy configuration files without a
# final newline (0x0A) to be built correctly by the m4(1) macro processor.
# $(1): the set of contexts file names.
# $(2): the file containing only 0x0A.
add_nl = $(foreach entry, $(1), $(subst $(entry), $(entry) $(2), $(entry)))

sepolicy_build_files := security_classes \
                        initial_sids \
                        access_vectors \
                        global_macros \
                        neverallow_macros \
                        mls_macros \
                        mls_decl \
                        mls \
                        policy_capabilities \
                        te_macros \
                        attributes \
                        ioctl_defines \
                        ioctl_macros \
                        *.te \
                        roles_decl \
                        roles \
                        users \
                        initial_sid_contexts \
                        fs_use \
                        genfs_contexts \
                        port_contexts

# CIL files which contain workarounds for current limitation of human-readable
# module policy language. These files are appended to the CIL files produced
# from module language files.
sepolicy_build_cil_workaround_files := technical_debt.cil

my_target_arch := $(TARGET_ARCH)
ifneq (,$(filter mips mips64,$(TARGET_ARCH)))
  my_target_arch := mips
endif

intermediates := $(TARGET_OUT_INTERMEDIATES)/ETC/sepolicy_intermediates

with_asan := false
ifneq (,$(filter address,$(SANITIZE_TARGET)))
  with_asan := true
endif

with_native_coverage := false
ifeq ($(NATIVE_COVERAGE),true)
  with_native_coverage := true
endif

# Library extension for host-side tests
ifeq ($(HOST_OS),darwin)
SHAREDLIB_EXT=dylib
else
SHAREDLIB_EXT=so
endif

# Convert a file_context file for a non-flattened APEX into a file for
# flattened APEX. /system/apex/<apex_name> path is prepended to the original paths
# $(1): path to the input file_contexts file for non-flattened APEX
# $(2): name of the APEX
# $(3): path to the generated file_contexs file for flattened APEX
# $(4): variable where $(3) is added to
define build_flattened_apex_file_contexts
$(4) += $(3)
$(3): PRIVATE_APEX_PATH := /system/apex/$(subst .,\\.,$(2))
$(3): $(1)
	$(hide) awk '/object_r/{printf("$$(PRIVATE_APEX_PATH)%s\n",$$$$0)}' $$< > $$@
endef

#################################

include $(CLEAR_VARS)

LOCAL_MODULE := selinux_policy
LOCAL_MODULE_TAGS := optional
LOCAL_REQUIRED_MODULES += \
    selinux_policy_nonsystem \
    selinux_policy_system \

include $(BUILD_PHONY_PACKAGE)


include $(CLEAR_VARS)
LOCAL_MODULE := selinux_policy_system
# These build targets are not used on non-Treble devices. However, we build these to avoid
# divergence between Treble and non-Treble devices.
LOCAL_REQUIRED_MODULES += \
    plat_mapping_file \
    $(addsuffix .cil,$(PLATFORM_SEPOLICY_COMPAT_VERSIONS)) \
    plat_sepolicy.cil \
    plat_sepolicy_and_mapping.sha256 \
    secilc \

LOCAL_REQUIRED_MODULES += \
    build_sepolicy \
    plat_file_contexts \
    plat_mac_permissions.xml \
    plat_property_contexts \
    plat_seapp_contexts \
    plat_service_contexts \
    plat_hwservice_contexts \
    searchpolicy \

# This conditional inclusion closely mimics the conditional logic
# inside init/init.cpp for loading SELinux policy from files.
ifneq ($(PRODUCT_SEPOLICY_SPLIT),true)
# The following files are only allowed for non-Treble devices.
LOCAL_REQUIRED_MODULES += \
    sepolicy \
    vendor_service_contexts \

endif # ($(PRODUCT_SEPOLICY_SPLIT),true)

ifneq ($(with_asan),true)
ifneq ($(SELINUX_IGNORE_NEVERALLOWS),true)
LOCAL_REQUIRED_MODULES += \
    sepolicy_tests \
    $(addprefix treble_sepolicy_tests_,$(PLATFORM_SEPOLICY_COMPAT_VERSIONS)) \

endif
endif

ifneq ($(PLATFORM_SEPOLICY_VERSION),$(TOT_SEPOLICY_VERSION))
LOCAL_REQUIRED_MODULES += \
    sepolicy_freeze_test \

endif # ($(PLATFORM_SEPOLICY_VERSION),$(TOT_SEPOLICY_VERSION))

include $(BUILD_PHONY_PACKAGE)

#################################

include $(CLEAR_VARS)

LOCAL_MODULE := selinux_policy_nonsystem
# Include precompiled policy, unless told otherwise.
ifneq ($(PRODUCT_PRECOMPILED_SEPOLICY),false)
LOCAL_REQUIRED_MODULES += \
    precompiled_sepolicy \
    precompiled_sepolicy.plat_sepolicy_and_mapping.sha256 \
    precompiled_sepolicy.product_sepolicy_and_mapping.sha256 \
    product_sepolicy_and_mapping.sha256 \

endif # ($(PRODUCT_PRECOMPILED_SEPOLICY),false)


# These build targets are not used on non-Treble devices. However, we build these to avoid
# divergence between Treble and non-Treble devices.
LOCAL_REQUIRED_MODULES += \
    plat_pub_versioned.cil \
    vendor_sepolicy.cil \
    plat_sepolicy_vers.txt \

LOCAL_REQUIRED_MODULES += \
    vendor_file_contexts \
    vendor_mac_permissions.xml \
    vendor_property_contexts \
    vendor_seapp_contexts \
    vendor_hwservice_contexts \
    vndservice_contexts \

ifdef BOARD_ODM_SEPOLICY_DIRS
LOCAL_REQUIRED_MODULES += \
    odm_sepolicy.cil \
    odm_file_contexts \
    odm_seapp_contexts \
    odm_property_contexts \
    odm_hwservice_contexts \
    odm_mac_permissions.xml
endif

ifdef HAS_PRODUCT_SEPOLICY
LOCAL_REQUIRED_MODULES += \
    product_sepolicy.cil \
    product_file_contexts \
    product_hwservice_contexts \
    product_property_contexts \
    product_seapp_contexts \
    product_service_contexts \
    product_mac_permissions.xml \
    product_mapping_file \

endif

ifneq ($(TARGET_BUILD_VARIANT), user)
LOCAL_REQUIRED_MODULES += \
    selinux_denial_metadata \

endif

# Builds an addtional userdebug sepolicy into the debug ramdisk.
LOCAL_REQUIRED_MODULES += \
    userdebug_plat_sepolicy.cil \

include $(BUILD_PHONY_PACKAGE)

#################################
include $(CLEAR_VARS)

LOCAL_MODULE := sepolicy_neverallows
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

# sepolicy_policy.conf - All of the policy for the device.  This is only used to
# check neverallow rules.
sepolicy_policy.conf := $(intermediates)/policy.conf
$(sepolicy_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(sepolicy_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(sepolicy_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := user
$(sepolicy_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(sepolicy_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(sepolicy_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(sepolicy_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(sepolicy_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(sepolicy_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PLAT_PRIVATE_POLICY) $(PLAT_VENDOR_POLICY) \
$(PRODUCT_PUBLIC_POLICY) $(PRODUCT_PRIVATE_POLICY) \
$(BOARD_VENDOR_SEPOLICY_DIRS) $(BOARD_ODM_SEPOLICY_DIRS))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

# sepolicy_policy_2.conf - All of the policy for the device.  This is only used to
# check neverallow rules using sepolicy-analyze, similar to CTS.
sepolicy_policy_2.conf := $(intermediates)/policy_2.conf
$(sepolicy_policy_2.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(sepolicy_policy_2.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(sepolicy_policy_2.conf): PRIVATE_TARGET_BUILD_VARIANT := user
$(sepolicy_policy_2.conf): PRIVATE_EXCLUDE_BUILD_TEST := true
$(sepolicy_policy_2.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(sepolicy_policy_2.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(sepolicy_policy_2.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(sepolicy_policy_2.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(sepolicy_policy_2.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(sepolicy_policy_2.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PLAT_PRIVATE_POLICY) $(PLAT_VENDOR_POLICY) \
$(PRODUCT_PUBLIC_POLICY) $(PRODUCT_PRIVATE_POLICY) \
$(BOARD_VENDOR_SEPOLICY_DIRS) $(BOARD_ODM_SEPOLICY_DIRS))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY_1 := $(sepolicy_policy.conf)
$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY_2 := $(sepolicy_policy_2.conf)
$(LOCAL_BUILT_MODULE): $(sepolicy_policy.conf) $(sepolicy_policy_2.conf) \
  $(HOST_OUT_EXECUTABLES)/checkpolicy $(HOST_OUT_EXECUTABLES)/sepolicy-analyze
ifneq ($(SELINUX_IGNORE_NEVERALLOWS),true)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $(HOST_OUT_EXECUTABLES)/checkpolicy -M -c \
		$(POLICYVERS) -o $@.tmp $(PRIVATE_SEPOLICY_1)
	$(hide) $(HOST_OUT_EXECUTABLES)/sepolicy-analyze $@.tmp neverallow -w -f $(PRIVATE_SEPOLICY_2) || \
	  ( echo "" 1>&2; \
	    echo "sepolicy-analyze failed. This is most likely due to the use" 1>&2; \
	    echo "of an expanded attribute in a neverallow assertion. Please fix" 1>&2; \
	    echo "the policy." 1>&2; \
	    exit 1 )
endif # ($(SELINUX_IGNORE_NEVERALLOWS),true)
	$(hide) touch $@.tmp
	$(hide) mv $@.tmp $@

sepolicy_policy.conf :=
sepolicy_policy_2.conf :=
built_sepolicy_neverallows := $(LOCAL_BUILT_MODULE)

##################################
# reqd_policy_mask - a policy.conf file which contains only the bare minimum
# policy necessary to use checkpolicy.  This bare-minimum policy needs to be
# present in all policy.conf files, but should not necessarily be exported as
# part of the public policy.  The rules generated by reqd_policy_mask will allow
# the compilation of public policy and subsequent removal of CIL policy that
# should not be exported.

reqd_policy_mask.conf := $(intermediates)/reqd_policy_mask.conf
$(reqd_policy_mask.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(reqd_policy_mask.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(reqd_policy_mask.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(reqd_policy_mask.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(reqd_policy_mask.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(reqd_policy_mask.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(reqd_policy_mask.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(reqd_policy_mask.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(reqd_policy_mask.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(reqd_policy_mask.conf): $(call build_policy, $(sepolicy_build_files), $(REQD_MASK_POLICY))
	$(transform-policy-to-conf)
# b/37755687
CHECKPOLICY_ASAN_OPTIONS := ASAN_OPTIONS=detect_leaks=0

reqd_policy_mask.cil := $(intermediates)/reqd_policy_mask.cil
$(reqd_policy_mask.cil): $(reqd_policy_mask.conf) $(HOST_OUT_EXECUTABLES)/checkpolicy
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $(HOST_OUT_EXECUTABLES)/checkpolicy -C -M -c \
		$(POLICYVERS) -o $@ $<

reqd_policy_mask.conf :=

##################################
# pub_policy - policy that will be exported to be a part of non-platform
# policy corresponding to this platform version.  This is a limited subset of
# policy that would not compile in checkpolicy on its own.  To get around this
# limitation, add only the required files from private policy, which will
# generate CIL policy that will then be filtered out by the reqd_policy_mask.
pub_policy.conf := $(intermediates)/pub_policy.conf
$(pub_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(pub_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(pub_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(pub_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(pub_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(pub_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(pub_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(pub_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(pub_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(pub_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PRODUCT_PUBLIC_POLICY) $(REQD_MASK_POLICY))
	$(transform-policy-to-conf)
pub_policy.cil := $(intermediates)/pub_policy.cil
$(pub_policy.cil): PRIVATE_POL_CONF := $(pub_policy.conf)
$(pub_policy.cil): PRIVATE_REQD_MASK := $(reqd_policy_mask.cil)
$(pub_policy.cil): $(HOST_OUT_EXECUTABLES)/checkpolicy \
$(HOST_OUT_EXECUTABLES)/build_sepolicy $(pub_policy.conf) $(reqd_policy_mask.cil)
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $< -C -M -c $(POLICYVERS) -o $@ $(PRIVATE_POL_CONF)
	$(hide) $(HOST_OUT_EXECUTABLES)/build_sepolicy -a $(HOST_OUT_EXECUTABLES) filter_out \
		-f $(PRIVATE_REQD_MASK) -t $@

pub_policy.conf :=

##################################
plat_pub_policy.conf := $(intermediates)/plat_pub_policy.conf
$(plat_pub_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(plat_pub_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(plat_pub_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(plat_pub_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(plat_pub_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(plat_pub_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(plat_pub_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(plat_pub_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(plat_pub_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(plat_pub_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(REQD_MASK_POLICY))
	$(transform-policy-to-conf)

plat_pub_policy.cil := $(intermediates)/plat_pub_policy.cil
$(plat_pub_policy.cil): PRIVATE_POL_CONF := $(plat_pub_policy.conf)
$(plat_pub_policy.cil): PRIVATE_REQD_MASK := $(reqd_policy_mask.cil)
$(plat_pub_policy.cil): $(HOST_OUT_EXECUTABLES)/checkpolicy \
$(HOST_OUT_EXECUTABLES)/build_sepolicy $(plat_pub_policy.conf) $(reqd_policy_mask.cil)
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $< -C -M -c $(POLICYVERS) -o $@ $(PRIVATE_POL_CONF)
	$(hide) $(HOST_OUT_EXECUTABLES)/build_sepolicy -a $(HOST_OUT_EXECUTABLES) filter_out \
		-f $(PRIVATE_REQD_MASK) -t $@

plat_pub_policy.conf :=

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := sectxfile_nl
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional

# Create a file containing newline only to add between context config files
include $(BUILD_SYSTEM)/base_rules.mk
$(LOCAL_BUILT_MODULE):
	@mkdir -p $(dir $@)
	$(hide) echo > $@

built_nl := $(LOCAL_BUILT_MODULE)

#################################
include $(CLEAR_VARS)

LOCAL_MODULE := plat_sepolicy.cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

# plat_policy.conf - A combination of the private and public platform policy
# which will ship with the device.  The platform will always reflect the most
# recent platform version and is not currently being attributized.
plat_policy.conf := $(intermediates)/plat_policy.conf
$(plat_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(plat_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(plat_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(plat_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(plat_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(plat_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(plat_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(plat_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(plat_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(plat_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PLAT_PRIVATE_POLICY))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

$(LOCAL_BUILT_MODULE): PRIVATE_ADDITIONAL_CIL_FILES := \
  $(call build_policy, $(sepolicy_build_cil_workaround_files), $(PLAT_PRIVATE_POLICY))
$(LOCAL_BUILT_MODULE): PRIVATE_NEVERALLOW_ARG := $(NEVERALLOW_ARG)
$(LOCAL_BUILT_MODULE): $(plat_policy.conf) $(HOST_OUT_EXECUTABLES)/checkpolicy \
  $(HOST_OUT_EXECUTABLES)/secilc \
  $(call build_policy, $(sepolicy_build_cil_workaround_files), $(PLAT_PRIVATE_POLICY)) \
  $(built_sepolicy_neverallows)
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $(HOST_OUT_EXECUTABLES)/checkpolicy -M -C -c \
		$(POLICYVERS) -o $@.tmp $<
	$(hide) cat $(PRIVATE_ADDITIONAL_CIL_FILES) >> $@.tmp
	$(hide) $(HOST_OUT_EXECUTABLES)/secilc -m -M true -G -c $(POLICYVERS) $(PRIVATE_NEVERALLOW_ARG) $@.tmp -o /dev/null -f /dev/null
	$(hide) mv $@.tmp $@

built_plat_cil := $(LOCAL_BUILT_MODULE)
plat_policy.conf :=

#################################
include $(CLEAR_VARS)

LOCAL_MODULE := userdebug_plat_sepolicy.cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_DEBUG_RAMDISK_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

# userdebug_plat_policy.conf - the userdebug version plat_sepolicy.cil
userdebug_plat_policy.conf := $(intermediates)/userdebug_plat_policy.conf
$(userdebug_plat_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(userdebug_plat_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(userdebug_plat_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := userdebug
$(userdebug_plat_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(userdebug_plat_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(userdebug_plat_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(userdebug_plat_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(userdebug_plat_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(userdebug_plat_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(userdebug_plat_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PLAT_PRIVATE_POLICY))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

$(LOCAL_BUILT_MODULE): PRIVATE_ADDITIONAL_CIL_FILES := \
  $(call build_policy, $(sepolicy_build_cil_workaround_files), $(PLAT_PRIVATE_POLICY))
$(LOCAL_BUILT_MODULE): PRIVATE_NEVERALLOW_ARG := $(NEVERALLOW_ARG)
$(LOCAL_BUILT_MODULE): $(userdebug_plat_policy.conf) $(HOST_OUT_EXECUTABLES)/checkpolicy \
  $(HOST_OUT_EXECUTABLES)/secilc \
  $(call build_policy, $(sepolicy_build_cil_workaround_files), $(PLAT_PRIVATE_POLICY)) \
  $(built_sepolicy_neverallows)
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $(HOST_OUT_EXECUTABLES)/checkpolicy -M -C -c \
		$(POLICYVERS) -o $@.tmp $<
	$(hide) cat $(PRIVATE_ADDITIONAL_CIL_FILES) >> $@.tmp
	$(hide) $(HOST_OUT_EXECUTABLES)/secilc -m -M true -G -c $(POLICYVERS) $(PRIVATE_NEVERALLOW_ARG) $@.tmp -o /dev/null -f /dev/null
	$(hide) mv $@.tmp $@

userdebug_plat_policy.conf :=

#################################
include $(CLEAR_VARS)

ifdef HAS_PRODUCT_SEPOLICY
LOCAL_MODULE := product_sepolicy.cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

# product_policy.conf - A combination of the private and public product policy
# which will ship with the device. Product policy is not attributized.
product_policy.conf := $(intermediates)/product_policy.conf
$(product_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(product_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(product_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(product_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(product_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(product_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(product_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(product_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(product_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(product_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PLAT_PRIVATE_POLICY) \
$(PRODUCT_PUBLIC_POLICY) $(PRODUCT_PRIVATE_POLICY))
	$(transform-policy-to-conf)
	$(hide) sed '/dontaudit/d' $@ > $@.dontaudit

$(LOCAL_BUILT_MODULE): PRIVATE_NEVERALLOW_ARG := $(NEVERALLOW_ARG)
$(LOCAL_BUILT_MODULE): PRIVATE_PLAT_CIL := $(built_plat_cil)
$(LOCAL_BUILT_MODULE): $(product_policy.conf) $(HOST_OUT_EXECUTABLES)/checkpolicy \
$(HOST_OUT_EXECUTABLES)/build_sepolicy $(HOST_OUT_EXECUTABLES)/secilc $(built_plat_cil)
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $(HOST_OUT_EXECUTABLES)/checkpolicy -M -C -c \
	$(POLICYVERS) -o $@ $<
	$(hide) $(HOST_OUT_EXECUTABLES)/build_sepolicy -a $(HOST_OUT_EXECUTABLES) filter_out \
		-f $(PRIVATE_PLAT_CIL) -t $@
	# Line markers (denoted by ;;) are malformed after above cmd. They are only
	# used for debugging, so we remove them.
	$(hide) grep -v ';;' $@ > $@.tmp
	$(hide) mv $@.tmp $@
	# Combine plat_sepolicy.cil and product_sepolicy.cil to make sure that the
	# latter doesn't accidentally depend on vendor/odm policies.
	$(hide) $(HOST_OUT_EXECUTABLES)/secilc -m -M true -G -c $(POLICYVERS) \
		$(PRIVATE_NEVERALLOW_ARG) $(PRIVATE_PLAT_CIL) $@ -o /dev/null -f /dev/null


built_product_cil := $(LOCAL_BUILT_MODULE)
product_policy.conf :=
endif # ifdef HAS_PRODUCT_SEPOLICY

#################################
include $(CLEAR_VARS)

LOCAL_MODULE := plat_sepolicy_vers.txt
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE) : PRIVATE_PLAT_SEPOL_VERS := $(BOARD_SEPOLICY_VERS)
$(LOCAL_BUILT_MODULE) :
	mkdir -p $(dir $@)
	echo $(PRIVATE_PLAT_SEPOL_VERS) > $@

#################################
include $(CLEAR_VARS)

LOCAL_MODULE := plat_mapping_file
LOCAL_MODULE_STEM := $(PLATFORM_SEPOLICY_VERSION).cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/selinux/mapping

include $(BUILD_SYSTEM)/base_rules.mk

# auto-generate the mapping file for current platform policy, since it needs to
# track platform policy development
$(LOCAL_BUILT_MODULE) : PRIVATE_VERS := $(PLATFORM_SEPOLICY_VERSION)
$(LOCAL_BUILT_MODULE) : $(plat_pub_policy.cil) $(HOST_OUT_EXECUTABLES)/version_policy
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/version_policy -b $< -m -n $(PRIVATE_VERS) -o $@

built_plat_mapping_cil := $(LOCAL_BUILT_MODULE)

#################################
include $(CLEAR_VARS)

ifdef HAS_PRODUCT_SEPOLICY
LOCAL_MODULE := product_mapping_file
LOCAL_MODULE_STEM := $(PLATFORM_SEPOLICY_VERSION).cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_PRODUCT)/etc/selinux/mapping

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE) : PRIVATE_VERS := $(PLATFORM_SEPOLICY_VERSION)
$(LOCAL_BUILT_MODULE) : PRIVATE_PLAT_MAPPING_CIL := $(built_plat_mapping_cil)
$(LOCAL_BUILT_MODULE) : $(pub_policy.cil) $(HOST_OUT_EXECUTABLES)/version_policy \
$(built_plat_mapping_cil)
	@mkdir -p $(dir $@)
	# Generate product mapping file as mapping file of all public sepolicy minus
	# plat_mapping_file.
	$(hide) $(HOST_OUT_EXECUTABLES)/version_policy -b $< -m -n $(PRIVATE_VERS) -o $@
	$(hide) $(HOST_OUT_EXECUTABLES)/build_sepolicy -a $(HOST_OUT_EXECUTABLES) filter_out \
		-f $(PRIVATE_PLAT_MAPPING_CIL) -t $@

built_product_mapping_cil := $(LOCAL_BUILT_MODULE)
endif # HAS_PRODUCT_SEPOLICY

#################################
include $(CLEAR_VARS)

# plat_pub_versioned.cil - the exported platform policy associated with the version
# that non-platform policy targets.
LOCAL_MODULE := plat_pub_versioned.cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE) : PRIVATE_VERS := $(BOARD_SEPOLICY_VERS)
$(LOCAL_BUILT_MODULE) : PRIVATE_TGT_POL := $(pub_policy.cil)
$(LOCAL_BUILT_MODULE) : PRIVATE_DEP_CIL_FILES := $(built_plat_cil) $(built_product_cil)\
$(built_plat_mapping_cil) $(built_product_mapping_cil)
$(LOCAL_BUILT_MODULE) : $(pub_policy.cil) $(HOST_OUT_EXECUTABLES)/version_policy \
  $(HOST_OUT_EXECUTABLES)/secilc $(built_plat_cil) $(built_product_cil) \
  $(built_plat_mapping_cil) $(built_product_mapping_cil)
	@mkdir -p $(dir $@)
	$(HOST_OUT_EXECUTABLES)/version_policy -b $< -t $(PRIVATE_TGT_POL) -n $(PRIVATE_VERS) -o $@
	$(hide) $(HOST_OUT_EXECUTABLES)/secilc -m -M true -G -N -c $(POLICYVERS) \
		$(PRIVATE_DEP_CIL_FILES) $@ -o /dev/null -f /dev/null

built_pub_vers_cil := $(LOCAL_BUILT_MODULE)

#################################
include $(CLEAR_VARS)

# vendor_policy.cil - the vendor sepolicy. This needs attributization and to be combined
# with the platform-provided policy.  It makes use of the reqd_policy_mask files from private
# policy and the platform public policy files in order to use checkpolicy.
LOCAL_MODULE := vendor_sepolicy.cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

vendor_policy.conf := $(intermediates)/vendor_policy.conf
$(vendor_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(vendor_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(vendor_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(vendor_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(vendor_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(vendor_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(vendor_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(vendor_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(vendor_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(vendor_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PRODUCT_PUBLIC_POLICY) $(REQD_MASK_POLICY) $(PLAT_VENDOR_POLICY) \
$(BOARD_VENDOR_SEPOLICY_DIRS))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

$(LOCAL_BUILT_MODULE): PRIVATE_POL_CONF := $(vendor_policy.conf)
$(LOCAL_BUILT_MODULE): PRIVATE_REQD_MASK := $(reqd_policy_mask.cil)
$(LOCAL_BUILT_MODULE): PRIVATE_BASE_CIL := $(pub_policy.cil)
$(LOCAL_BUILT_MODULE): PRIVATE_VERS := $(BOARD_SEPOLICY_VERS)
$(LOCAL_BUILT_MODULE): PRIVATE_DEP_CIL_FILES := $(built_plat_cil) $(built_product_cil)\
$(built_pub_vers_cil) $(built_plat_mapping_cil) $(built_product_mapping_cil)
$(LOCAL_BUILT_MODULE): PRIVATE_FILTER_CIL := $(built_pub_vers_cil)
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/build_sepolicy \
  $(vendor_policy.conf) $(reqd_policy_mask.cil) $(pub_policy.cil) \
  $(built_plat_cil) $(built_product_cil) $(built_pub_vers_cil) \
  $(built_plat_mapping_cil) $(built_product_mapping_cil)
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/build_sepolicy -a $(HOST_OUT_EXECUTABLES) build_cil \
		-i $(PRIVATE_POL_CONF) -m $(PRIVATE_REQD_MASK) -c $(CHECKPOLICY_ASAN_OPTIONS) \
		-b $(PRIVATE_BASE_CIL) -d $(PRIVATE_DEP_CIL_FILES) -f $(PRIVATE_FILTER_CIL) \
		-t $(PRIVATE_VERS) -p $(POLICYVERS) -o $@

built_vendor_cil := $(LOCAL_BUILT_MODULE)
vendor_policy.conf :=

#################################
include $(CLEAR_VARS)

ifdef BOARD_ODM_SEPOLICY_DIRS
# odm_policy.cil - the odm sepolicy. This needs attributization and to be combined
# with the platform-provided policy.  It makes use of the reqd_policy_mask files from private
# policy and the platform public policy files in order to use checkpolicy.
LOCAL_MODULE := odm_sepolicy.cil
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

odm_policy.conf := $(intermediates)/odm_policy.conf
$(odm_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(odm_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(odm_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(odm_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(odm_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(odm_policy.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(odm_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(odm_policy.conf): PRIVATE_SEPOLICY_SPLIT := $(PRODUCT_SEPOLICY_SPLIT)
$(odm_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(odm_policy.conf): $(call build_policy, $(sepolicy_build_files), \
  $(PLAT_PUBLIC_POLICY) $(PRODUCT_PUBLIC_POLICY) $(REQD_MASK_POLICY) $(PLAT_VENDOR_POLICY) \
  $(BOARD_VENDOR_SEPOLICY_DIRS) $(BOARD_ODM_SEPOLICY_DIRS))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

$(LOCAL_BUILT_MODULE): PRIVATE_POL_CONF := $(odm_policy.conf)
$(LOCAL_BUILT_MODULE): PRIVATE_REQD_MASK := $(reqd_policy_mask.cil)
$(LOCAL_BUILT_MODULE): PRIVATE_BASE_CIL := $(pub_policy.cil)
$(LOCAL_BUILT_MODULE): PRIVATE_VERS := $(BOARD_SEPOLICY_VERS)
$(LOCAL_BUILT_MODULE): PRIVATE_DEP_CIL_FILES := $(built_plat_cil) $(built_product_cil) \
  $(built_pub_vers_cil) $(built_plat_mapping_cil) $(built_product_mapping_cil)\
  $(built_vendor_cil)
$(LOCAL_BUILT_MODULE) : PRIVATE_FILTER_CIL_FILES := $(built_pub_vers_cil) $(built_vendor_cil)
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/build_sepolicy \
  $(odm_policy.conf) $(reqd_policy_mask.cil) $(pub_policy.cil) \
  $(built_plat_cil) $(built_product_cil) $(built_pub_vers_cil) \
  $(built_plat_mapping_cil) $(built_product_mapping_cil) $(built_vendor_cil)
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/build_sepolicy -a $(HOST_OUT_EXECUTABLES) build_cil \
		-i $(PRIVATE_POL_CONF) -m $(PRIVATE_REQD_MASK) -c $(CHECKPOLICY_ASAN_OPTIONS) \
		-b $(PRIVATE_BASE_CIL) -d $(PRIVATE_DEP_CIL_FILES) -f $(PRIVATE_FILTER_CIL_FILES) \
		-t $(PRIVATE_VERS) -p $(POLICYVERS) -o $@

built_odm_cil := $(LOCAL_BUILT_MODULE)
odm_policy.conf :=
odm_policy_raw :=
endif

#################################
include $(CLEAR_VARS)

LOCAL_MODULE := precompiled_sepolicy
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true

ifeq ($(BOARD_USES_ODMIMAGE),true)
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux
else
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux
endif

include $(BUILD_SYSTEM)/base_rules.mk

all_cil_files := \
    $(built_plat_cil) \
    $(built_plat_mapping_cil) \
    $(built_pub_vers_cil) \
    $(built_vendor_cil)

ifdef HAS_PRODUCT_SEPOLICY
all_cil_files += \
    $(built_product_cil) \
    $(built_product_mapping_cil) \

endif

ifdef BOARD_ODM_SEPOLICY_DIRS
all_cil_files += $(built_odm_cil)
endif

$(LOCAL_BUILT_MODULE): PRIVATE_CIL_FILES := $(all_cil_files)
$(LOCAL_BUILT_MODULE): PRIVATE_NEVERALLOW_ARG := $(NEVERALLOW_ARG)
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/secilc $(all_cil_files) $(built_sepolicy_neverallows)
	$(hide) $(HOST_OUT_EXECUTABLES)/secilc -m -M true -G -c $(POLICYVERS) $(PRIVATE_NEVERALLOW_ARG) \
		$(PRIVATE_CIL_FILES) -o $@ -f /dev/null

built_precompiled_sepolicy := $(LOCAL_BUILT_MODULE)
all_cil_files :=

#################################
# Precompiled sepolicy is loaded if and only if:
# - plat_sepolicy_and_mapping.sha256 equals
#   precompiled_sepolicy.plat_sepolicy_and_mapping.sha256
# AND
# - product_sepolicy_and_mapping.sha256 equals
#   precompiled_sepolicy.product_sepolicy_and_mapping.sha256
# See system/core/init/selinux.cpp for details.
#################################
include $(CLEAR_VARS)

LOCAL_MODULE := plat_sepolicy_and_mapping.sha256
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH = $(TARGET_OUT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_plat_cil) $(built_plat_mapping_cil)
	cat $^ | sha256sum | cut -d' ' -f1 > $@

#################################
include $(CLEAR_VARS)

LOCAL_MODULE := product_sepolicy_and_mapping.sha256
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH = $(TARGET_OUT_PRODUCT)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(built_product_cil) $(built_product_mapping_cil)
	cat $^ | sha256sum | cut -d' ' -f1 > $@

#################################
# SHA-256 digest of the plat_sepolicy.cil and plat_mapping_file against
# which precompiled_policy was built.
#################################
include $(CLEAR_VARS)
LOCAL_MODULE := precompiled_sepolicy.plat_sepolicy_and_mapping.sha256
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional

ifeq ($(BOARD_USES_ODMIMAGE),true)
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux
else
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux
endif

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): PRIVATE_CIL_FILES := $(built_plat_cil) $(built_plat_mapping_cil)
$(LOCAL_BUILT_MODULE): $(built_precompiled_sepolicy) $(built_plat_cil) $(built_plat_mapping_cil)
	cat $(PRIVATE_CIL_FILES) | sha256sum | cut -d' ' -f1 > $@

#################################
# SHA-256 digest of the product_sepolicy.cil and product_mapping_file against
# which precompiled_policy was built.
#################################
include $(CLEAR_VARS)
LOCAL_MODULE := precompiled_sepolicy.product_sepolicy_and_mapping.sha256
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional

ifeq ($(BOARD_USES_ODMIMAGE),true)
LOCAL_MODULE_PATH := $(TARGET_OUT_ODM)/etc/selinux
else
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux
endif

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): PRIVATE_CIL_FILES := $(built_product_cil) $(built_product_mapping_cil)
$(LOCAL_BUILT_MODULE): $(built_precompiled_sepolicy) $(built_product_cil) $(built_product_mapping_cil)
	cat $(PRIVATE_CIL_FILES) | sha256sum | cut -d' ' -f1 > $@

#################################
include $(CLEAR_VARS)
# build this target so that we can still perform neverallow checks

LOCAL_MODULE := sepolicy
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

all_cil_files := \
    $(built_plat_cil) \
    $(built_plat_mapping_cil) \
    $(built_pub_vers_cil) \
    $(built_vendor_cil)

ifdef HAS_PRODUCT_SEPOLICY
all_cil_files += \
    $(built_product_cil) \
    $(built_product_mapping_cil) \

endif

ifdef BOARD_ODM_SEPOLICY_DIRS
all_cil_files += $(built_odm_cil)
endif

$(LOCAL_BUILT_MODULE): PRIVATE_CIL_FILES := $(all_cil_files)
$(LOCAL_BUILT_MODULE): PRIVATE_NEVERALLOW_ARG := $(NEVERALLOW_ARG)
$(LOCAL_BUILT_MODULE): $(HOST_OUT_EXECUTABLES)/secilc $(HOST_OUT_EXECUTABLES)/sepolicy-analyze $(all_cil_files) \
$(built_sepolicy_neverallows)
	@mkdir -p $(dir $@)
	$(hide) $< -m -M true -G -c $(POLICYVERS) $(PRIVATE_NEVERALLOW_ARG) $(PRIVATE_CIL_FILES) -o $@.tmp -f /dev/null
	$(hide) $(HOST_OUT_EXECUTABLES)/sepolicy-analyze $@.tmp permissive > $@.permissivedomains
	$(hide) if [ "$(TARGET_BUILD_VARIANT)" = "user" -a -s $@.permissivedomains ]; then \
		echo "==========" 1>&2; \
		echo "ERROR: permissive domains not allowed in user builds" 1>&2; \
		echo "List of invalid domains:" 1>&2; \
		cat $@.permissivedomains 1>&2; \
		exit 1; \
		fi
	$(hide) mv $@.tmp $@

built_sepolicy := $(LOCAL_BUILT_MODULE)
all_cil_files :=

#################################
include $(CLEAR_VARS)

# keep concrete sepolicy for neverallow checks
# If SELINUX_IGNORE_NEVERALLOWS is set, we use sed to remove the neverallow lines before compiling.

LOCAL_MODULE := sepolicy.recovery
LOCAL_MODULE_STEM := sepolicy
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

sepolicy.recovery.conf := $(intermediates)/sepolicy.recovery.conf
$(sepolicy.recovery.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(sepolicy.recovery.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(sepolicy.recovery.conf): PRIVATE_TARGET_BUILD_VARIANT := $(TARGET_BUILD_VARIANT)
$(sepolicy.recovery.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(sepolicy.recovery.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(sepolicy.recovery.conf): PRIVATE_TGT_WITH_NATIVE_COVERAGE := $(with_native_coverage)
$(sepolicy.recovery.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(sepolicy.recovery.conf): PRIVATE_TGT_RECOVERY := -D target_recovery=true
$(sepolicy.recovery.conf): $(call build_policy, $(sepolicy_build_files), \
                           $(PLAT_PUBLIC_POLICY) $(PLAT_PRIVATE_POLICY) \
                           $(PRODUCT_PUBLIC_POLICY) $(PRODUCT_PRIVATE_POLICY) \
                           $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) \
                           $(BOARD_ODM_SEPOLICY_DIRS))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

ifeq ($(SELINUX_IGNORE_NEVERALLOWS),true)
	$(hide) sed -z 's/\n\s*neverallow[^;]*;/\n/g' $@ > $@.neverallow
	$(hide) mv $@.neverallow $@
endif

$(LOCAL_BUILT_MODULE): $(sepolicy.recovery.conf) $(HOST_OUT_EXECUTABLES)/checkpolicy \
                       $(HOST_OUT_EXECUTABLES)/sepolicy-analyze
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $(HOST_OUT_EXECUTABLES)/checkpolicy -M -c \
		$(POLICYVERS) -o $@.tmp $<
	$(hide) $(HOST_OUT_EXECUTABLES)/sepolicy-analyze $@.tmp permissive > $@.permissivedomains
	$(hide) if [ "$(TARGET_BUILD_VARIANT)" = "user" -a -s $@.permissivedomains ]; then \
		echo "==========" 1>&2; \
		echo "ERROR: permissive domains not allowed in user builds" 1>&2; \
		echo "List of invalid domains:" 1>&2; \
		cat $@.permissivedomains 1>&2; \
		exit 1; \
		fi
	$(hide) mv $@.tmp $@

sepolicy.recovery.conf :=

##################################
# SELinux policy embedded into CTS.
# CTS checks neverallow rules of this policy against the policy of the device under test.
##################################
include $(CLEAR_VARS)

LOCAL_MODULE := general_sepolicy.conf
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := tests

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): PRIVATE_MLS_SENS := $(MLS_SENS)
$(LOCAL_BUILT_MODULE): PRIVATE_MLS_CATS := $(MLS_CATS)
$(LOCAL_BUILT_MODULE): PRIVATE_TARGET_BUILD_VARIANT := user
$(LOCAL_BUILT_MODULE): PRIVATE_TGT_ARCH := $(my_target_arch)
$(LOCAL_BUILT_MODULE): PRIVATE_WITH_ASAN := false
$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY_SPLIT := cts
$(LOCAL_BUILT_MODULE): PRIVATE_COMPATIBLE_PROPERTY := cts
$(LOCAL_BUILT_MODULE): PRIVATE_EXCLUDE_BUILD_TEST := true
$(LOCAL_BUILT_MODULE): $(call build_policy, $(sepolicy_build_files), \
$(PLAT_PUBLIC_POLICY) $(PLAT_PRIVATE_POLICY))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

##################################
# TODO - remove this.   Keep around until we get the filesystem creation stuff taken care of.
#
include $(CLEAR_VARS)

LOCAL_MODULE := file_contexts.bin
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT)

include $(BUILD_SYSTEM)/base_rules.mk

# The file_contexts.bin is built in the following way:
# 1. Collect all file_contexts files in THIS repository and process them with
#    m4 into a tmp file called file_contexts.local.tmp.
# 2. Collect all device specific file_contexts files and process them with m4
#    into a tmp file called file_contexts.device.tmp.
# 3. Run checkfc -e (allow no device fc entries ie empty) and fc_sort on
#    file_contexts.device.tmp and output to file_contexts.device.sorted.tmp.
# 4. Concatenate file_contexts.local.tmp and file_contexts.device.tmp into
#    file_contexts.concat.tmp.
# 5. Run checkfc and sefcontext_compile on file_contexts.concat.tmp to produce
#    file_contexts.bin.
#
#  Note: That a newline file is placed between each file_context file found to
#        ensure a proper build when an fc file is missing an ending newline.

local_fc_files := $(call build_policy, file_contexts, $(PLAT_PRIVATE_POLICY))

ifdef HAS_PRODUCT_SEPOLICY
local_fc_files += $(call build_policy, file_contexts, $(PRODUCT_PRIVATE_POLICY))
endif

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

file_contexts.local.tmp := $(intermediates)/file_contexts.local.tmp
$(file_contexts.local.tmp): $(local_fcfiles_with_nl)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $^ > $@

device_fc_files := $(call build_vendor_policy, file_contexts)

ifdef BOARD_ODM_SEPOLICY_DIRS
device_fc_files += $(call build_odm_policy, file_contexts)
endif

device_fcfiles_with_nl := $(call add_nl, $(device_fc_files), $(built_nl))

file_contexts.device.tmp := $(intermediates)/file_contexts.device.tmp
$(file_contexts.device.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(file_contexts.device.tmp): $(device_fcfiles_with_nl)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $^ > $@

file_contexts.device.sorted.tmp := $(intermediates)/file_contexts.device.sorted.tmp
$(file_contexts.device.sorted.tmp): PRIVATE_SEPOLICY := $(built_sepolicy)
$(file_contexts.device.sorted.tmp): $(file_contexts.device.tmp) $(built_sepolicy) \
  $(HOST_OUT_EXECUTABLES)/fc_sort $(HOST_OUT_EXECUTABLES)/checkfc
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/checkfc -e $(PRIVATE_SEPOLICY) $<
	$(hide) $(HOST_OUT_EXECUTABLES)/fc_sort $< $@

file_contexts.concat.tmp := $(intermediates)/file_contexts.concat.tmp
$(file_contexts.concat.tmp): $(file_contexts.local.tmp) $(file_contexts.device.sorted.tmp)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $^ > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(file_contexts.concat.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/sefcontext_compile $(HOST_OUT_EXECUTABLES)/checkfc
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/checkfc $(PRIVATE_SEPOLICY) $<
	$(hide) $(HOST_OUT_EXECUTABLES)/sefcontext_compile -o $@ $<

built_fc := $(LOCAL_BUILT_MODULE)
local_fc_files :=
local_fcfiles_with_nl :=
device_fc_files :=
device_fcfiles_with_nl :=
file_contexts.concat.tmp :=
file_contexts.device.sorted.tmp :=
file_contexts.device.tmp :=
file_contexts.local.tmp :=

##################################
ifneq ($(TARGET_BUILD_VARIANT), user)
include $(CLEAR_VARS)

LOCAL_MODULE := selinux_denial_metadata
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

bug_files := $(call build_policy, bug_map, $(LOCAL_PATH) $(PLAT_PRIVATE_POLICY) $(PLAT_VENDOR_POLICY) $(BOARD_SEPOLICY_DIRS) $(PLAT_PUBLIC_POLICY))

$(LOCAL_BUILT_MODULE) : $(bug_files)
	@mkdir -p $(dir $@)
	cat $^ > $@

bug_files :=
endif

##################################
include $(LOCAL_PATH)/file_contexts.mk

##################################
include $(LOCAL_PATH)/seapp_contexts.mk

##################################
include $(LOCAL_PATH)/property_contexts.mk

##################################
include $(LOCAL_PATH)/service_contexts.mk

##################################
include $(LOCAL_PATH)/hwservice_contexts.mk

##################################
include $(CLEAR_VARS)

LOCAL_MODULE := vndservice_contexts
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/selinux

include $(BUILD_SYSTEM)/base_rules.mk

vnd_svcfiles := $(call build_policy, vndservice_contexts, $(PLAT_VENDOR_POLICY) $(BOARD_VENDOR_SEPOLICY_DIRS) $(REQD_MASK_POLICY))

vndservice_contexts.tmp := $(intermediates)/vndservice_contexts.tmp
$(vndservice_contexts.tmp): PRIVATE_SVC_FILES := $(vnd_svcfiles)
$(vndservice_contexts.tmp): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(vndservice_contexts.tmp): $(vnd_svcfiles)
	@mkdir -p $(dir $@)
	$(hide) m4 --fatal-warnings -s $(PRIVATE_ADDITIONAL_M4DEFS) $(PRIVATE_SVC_FILES) > $@

$(LOCAL_BUILT_MODULE): PRIVATE_SEPOLICY := $(built_sepolicy)
$(LOCAL_BUILT_MODULE): $(vndservice_contexts.tmp) $(built_sepolicy) $(HOST_OUT_EXECUTABLES)/checkfc $(ACP)
	@mkdir -p $(dir $@)
	sed -e 's/#.*$$//' -e '/^$$/d' $< > $@
	$(hide) $(HOST_OUT_EXECUTABLES)/checkfc -e -v $(PRIVATE_SEPOLICY) $@

vnd_svcfiles :=
vndservice_contexts.tmp :=

##################################
include $(LOCAL_PATH)/mac_permissions.mk

#################################
include $(CLEAR_VARS)
LOCAL_MODULE := sepolicy_tests
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := tests

include $(BUILD_SYSTEM)/base_rules.mk

all_fc_files := $(built_plat_fc) $(built_vendor_fc)
ifdef HAS_PRODUCT_SEPOLICY
all_fc_args += $(built_product_fc)
endif
ifdef BOARD_ODM_SEPOLICY_DIRS
all_fc_files += $(built_odm_fc)
endif
all_fc_args := $(foreach file, $(all_fc_files), -f $(file))

sepolicy_tests := $(intermediates)/sepolicy_tests
$(sepolicy_tests): ALL_FC_ARGS := $(all_fc_args)
$(sepolicy_tests): PRIVATE_SEPOLICY := $(built_sepolicy)
$(sepolicy_tests): $(HOST_OUT_EXECUTABLES)/sepolicy_tests $(all_fc_files) $(built_sepolicy)
	@mkdir -p $(dir $@)
	$(hide) $(HOST_OUT_EXECUTABLES)/sepolicy_tests -l $(HOST_OUT)/lib64/libsepolwrap.$(SHAREDLIB_EXT) \
		$(ALL_FC_ARGS)  -p $(PRIVATE_SEPOLICY)
	$(hide) touch $@

##################################
intermediates := $(call intermediates-dir-for,ETC,built_plat_sepolicy,,,,)

# plat_sepolicy - the current platform policy only, built into a policy binary.
# TODO - this currently excludes partner extensions, but support should be added
# to enable partners to add their own compatibility mapping
BASE_PLAT_PUBLIC_POLICY := $(filter-out $(BOARD_PLAT_PUBLIC_SEPOLICY_DIR), $(PLAT_PUBLIC_POLICY))
BASE_PLAT_PRIVATE_POLICY := $(filter-out $(BOARD_PLAT_PRIVATE_SEPOLICY_DIR), $(PLAT_PRIVATE_POLICY))
base_plat_policy.conf := $(intermediates)/base_plat_policy.conf
$(base_plat_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(base_plat_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(base_plat_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := user
$(base_plat_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(base_plat_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(base_plat_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(base_plat_policy.conf): PRIVATE_SEPOLICY_SPLIT := true
$(base_plat_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(base_plat_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(BASE_PLAT_PUBLIC_POLICY) $(BASE_PLAT_PRIVATE_POLICY))
	$(transform-policy-to-conf)
	$(hide) sed '/^\s*dontaudit.*;/d' $@ | sed '/^\s*dontaudit/,/;/d' > $@.dontaudit

built_plat_sepolicy := $(intermediates)/built_plat_sepolicy
$(built_plat_sepolicy): PRIVATE_ADDITIONAL_CIL_FILES := \
  $(call build_policy, $(sepolicy_build_cil_workaround_files), $(BASE_PLAT_PRIVATE_POLICY))
$(built_plat_sepolicy): PRIVATE_NEVERALLOW_ARG := $(NEVERALLOW_ARG)
$(built_plat_sepolicy): $(base_plat_policy.conf) $(HOST_OUT_EXECUTABLES)/checkpolicy \
$(HOST_OUT_EXECUTABLES)/secilc \
$(call build_policy, $(sepolicy_build_cil_workaround_files), $(BASE_PLAT_PRIVATE_POLICY)) \
$(built_sepolicy_neverallows)
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $(HOST_OUT_EXECUTABLES)/checkpolicy -M -C -c \
		$(POLICYVERS) -o $@ $<
	$(hide) cat $(PRIVATE_ADDITIONAL_CIL_FILES) >> $@
	$(hide) $(HOST_OUT_EXECUTABLES)/secilc -m -M true -G -c $(POLICYVERS) $(PRIVATE_NEVERALLOW_ARG) $@ -o $@ -f /dev/null

base_plat_pub_policy.conf := $(intermediates)/base_plat_pub_policy.conf
$(base_plat_pub_policy.conf): PRIVATE_MLS_SENS := $(MLS_SENS)
$(base_plat_pub_policy.conf): PRIVATE_MLS_CATS := $(MLS_CATS)
$(base_plat_pub_policy.conf): PRIVATE_TARGET_BUILD_VARIANT := user
$(base_plat_pub_policy.conf): PRIVATE_TGT_ARCH := $(my_target_arch)
$(base_plat_pub_policy.conf): PRIVATE_TGT_WITH_ASAN := $(with_asan)
$(base_plat_pub_policy.conf): PRIVATE_ADDITIONAL_M4DEFS := $(LOCAL_ADDITIONAL_M4DEFS)
$(base_plat_pub_policy.conf): PRIVATE_SEPOLICY_SPLIT := true
$(base_plat_pub_policy.conf): PRIVATE_COMPATIBLE_PROPERTY := $(PRODUCT_COMPATIBLE_PROPERTY)
$(base_plat_pub_policy.conf): $(call build_policy, $(sepolicy_build_files), \
$(BASE_PLAT_PUBLIC_POLICY) $(REQD_MASK_POLICY))
	$(transform-policy-to-conf)

base_plat_pub_policy.cil := $(intermediates)/base_plat_pub_policy.cil
$(base_plat_pub_policy.cil): PRIVATE_POL_CONF := $(base_plat_pub_policy.conf)
$(base_plat_pub_policy.cil): PRIVATE_REQD_MASK := $(reqd_policy_mask.cil)
$(base_plat_pub_policy.cil): $(HOST_OUT_EXECUTABLES)/checkpolicy \
$(HOST_OUT_EXECUTABLES)/build_sepolicy $(base_plat_pub_policy.conf) $(reqd_policy_mask.cil)
	@mkdir -p $(dir $@)
	$(hide) $(CHECKPOLICY_ASAN_OPTIONS) $< -C -M -c $(POLICYVERS) -o $@ $(PRIVATE_POL_CONF)
	$(hide) $(HOST_OUT_EXECUTABLES)/build_sepolicy -a $(HOST_OUT_EXECUTABLES) filter_out \
		-f $(PRIVATE_REQD_MASK) -t $@

all_fc_files := $(built_plat_fc) $(built_vendor_fc)
ifdef HAS_PRODUCT_SEPOLICY
all_fc_files += $(built_product_fc)
endif
ifdef BOARD_ODM_SEPOLICY_DIRS
all_fc_files += $(built_odm_fc)
endif
all_fc_args := $(foreach file, $(all_fc_files), -f $(file))

# Tests for Treble compatibility of current platform policy and vendor policy of
# given release version.
version_under_treble_tests := 26.0
include $(LOCAL_PATH)/treble_sepolicy_tests_for_release.mk

version_under_treble_tests := 27.0
include $(LOCAL_PATH)/treble_sepolicy_tests_for_release.mk

version_under_treble_tests := 28.0
include $(LOCAL_PATH)/treble_sepolicy_tests_for_release.mk

BASE_PLAT_PUBLIC_POLICY :=
BASE_PLAT_PRIVATE_POLICY :=
base_plat_policy.conf :=
base_plat_pub_policy.conf :=
plat_sepolicy :=

#################################
include $(CLEAR_VARS)
LOCAL_MODULE := sepolicy_freeze_test
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := tests

include $(BUILD_SYSTEM)/base_rules.mk

base_plat_public := $(LOCAL_PATH)/public
base_plat_private := $(LOCAL_PATH)/private
base_plat_public_prebuilt := \
  $(LOCAL_PATH)/prebuilts/api/$(PLATFORM_SEPOLICY_VERSION)/public
base_plat_private_prebuilt := \
  $(LOCAL_PATH)/prebuilts/api/$(PLATFORM_SEPOLICY_VERSION)/private

all_frozen_files := $(call build_policy,$(sepolicy_build_files), \
$(base_plat_public) $(base_plat_private) $(base_plat_public_prebuilt) $(base_plat_private_prebuilt))

$(LOCAL_BUILT_MODULE): PRIVATE_BASE_PLAT_PUBLIC := $(base_plat_public)
$(LOCAL_BUILT_MODULE): PRIVATE_BASE_PLAT_PRIVATE := $(base_plat_private)
$(LOCAL_BUILT_MODULE): PRIVATE_BASE_PLAT_PUBLIC_PREBUILT := $(base_plat_public_prebuilt)
$(LOCAL_BUILT_MODULE): PRIVATE_BASE_PLAT_PRIVATE_PREBUILT := $(base_plat_private_prebuilt)
$(LOCAL_BUILT_MODULE): $(all_frozen_files)
ifneq ($(PLATFORM_SEPOLICY_VERSION),$(TOT_SEPOLICY_VERSION))
	@diff -rq -x bug_map $(PRIVATE_BASE_PLAT_PUBLIC_PREBUILT) $(PRIVATE_BASE_PLAT_PUBLIC)
	@diff -rq -x bug_map $(PRIVATE_BASE_PLAT_PRIVATE_PREBUILT) $(PRIVATE_BASE_PLAT_PRIVATE)
endif # ($(PLATFORM_SEPOLICY_VERSION),$(TOT_SEPOLICY_VERSION))
	$(hide) touch $@

base_plat_public :=
base_plat_private :=
base_plat_public_prebuilt :=
base_plat_private_prebuilt :=
all_frozen_files :=

#################################


add_nl :=
build_vendor_policy :=
build_odm_policy :=
build_policy :=
built_plat_fc :=
built_product_fc :=
built_vendor_fc :=
built_odm_fc :=
built_nl :=
built_plat_cil :=
built_pub_vers_cil :=
built_plat_mapping_cil :=
built_product_mapping_cil :=
built_plat_pc :=
built_product_pc :=
built_vendor_cil :=
built_vendor_pc :=
built_vendor_sc :=
built_odm_cil :=
built_odm_pc :=
built_odm_sc :=
built_plat_sc :=
built_precompiled_sepolicy :=
built_sepolicy :=
built_sepolicy_neverallows :=
built_plat_svc :=
built_vendor_svc :=
built_plat_sepolicy :=
mapping_policy :=
my_target_arch :=
pub_policy.cil :=
reqd_policy_mask.cil :=
sepolicy_build_files :=
sepolicy_build_cil_workaround_files :=
with_asan :=

include $(call all-makefiles-under,$(LOCAL_PATH))
