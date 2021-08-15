#
# Build Template
#
# Invoke this template with a set of variables in order to make build targets
# for a build variant that targets a specific CPU architecture.
#

################################################################################
#
# Build Template
#
# Invoke this to instantiate a set of build targets. Two build targets are
# produced by this template that can be either used directly or depended on to
# perform post processing (ie: during a nanoapp build).
#
# TARGET_NAME_ar - An archive of the code compiled by this template.
# TARGET_NAME_so - A shared object of the code compiled by this template.
# TARGET_NAME    - A convenience target that depends on the above archive and
#                  shared object targets.
#
# Argument List:
#     $1  - TARGET_NAME          - The name of the target being built.
#     $2  - TARGET_CFLAGS        - The compiler flags to use for this target.
#     $3  - TARGET_CC            - The C/C++ compiler for the target variant.
#     $4  - TARGET_SO_LDFLAGS    - The linker flags to use for this target.
#     $5  - TARGET_LD            - The linker for the target variant.
#     $6  - TARGET_ARFLAGS       - The archival flags to use for this target.
#     $7  - TARGET_AR            - The archival tool for the targer variant.
#     $8  - TARGET_VARIANT_SRCS  - Source files specific to this variant.
#     $9  - TARGET_BUILD_BIN     - Build a binary. Typically this means that the
#                                  source files provided include an entry point.
#     $10 - TARGET_BIN_LDFLAGS   - Linker flags that are passed to the linker
#                                  when building an executable binary.
#     $11 - TARGET_SO_EARLY_LIBS - Link against a set of libraries when building
#                                  a shared object or binary. These are placed
#                                  before the objects produced by this build.
#     $12 - TARGET_SO_LATE_LIBS  - Link against a set of libraries when building
#                                  a shared object or binary. These are placed
#                                  after the objects produced by this build.
#     $13 - TARGET_PLATFORM_ID   - The ID of the platform that this nanoapp
#                                  build targets.
#
################################################################################

ifndef BUILD_TEMPLATE
define BUILD_TEMPLATE

# Target Objects ###############################################################

# Source files.
$$(1)_CC_SRCS = $$(filter %.cc, $(COMMON_SRCS) $(8))
$$(1)_CPP_SRCS = $$(filter %.cpp, $(COMMON_SRCS) $(8))
$$(1)_C_SRCS = $$(filter %.c, $(COMMON_SRCS) $(8))
$$(1)_S_SRCS = $$(filter %.S, $(COMMON_SRCS) $(8))

# Object files.
$$(1)_OBJS_DIR = $(1)_objs
$$(1)_CC_OBJS = $$(patsubst %.cc, $(OUT)/$$($$(1)_OBJS_DIR)/%.o, \
                            $$($$(1)_CC_SRCS))
$$(1)_CPP_OBJS = $$(patsubst %.cpp, $(OUT)/$$($$(1)_OBJS_DIR)/%.o, \
                             $$($$(1)_CPP_SRCS))
$$(1)_C_OBJS = $$(patsubst %.c, $(OUT)/$$($$(1)_OBJS_DIR)/%.o, \
                           $$($$(1)_C_SRCS))
$$(1)_S_OBJS = $$(patsubst %.S, $(OUT)/$$($$(1)_OBJS_DIR)/%.o, \
                           $$($$(1)_S_SRCS))

# Automatic dependency resolution Makefiles.
$$(1)_CC_DEPS = $$(patsubst %.cc, $(OUT)/$$($$(1)_OBJS_DIR)/%.d, \
                            $$($$(1)_CC_SRCS))
$$(1)_CPP_DEPS = $$(patsubst %.cpp, $(OUT)/$$($$(1)_OBJS_DIR)/%.d, \
                             $$($$(1)_CPP_SRCS))
$$(1)_C_DEPS = $$(patsubst %.c, $(OUT)/$$($$(1)_OBJS_DIR)/%.d, \
                           $$($$(1)_C_SRCS))
$$(1)_S_DEPS = $$(patsubst %.S, $(OUT)/$$($$(1)_OBJS_DIR)/%.d, \
                           $$($$(1)_S_SRCS))

# Add object file directories.
$$$(1)_DIRS = $$(sort $$(dir $$($$(1)_CC_OBJS) \
                             $$($$(1)_CPP_OBJS) \
                             $$($$(1)_C_OBJS) \
                             $$($$(1)_S_OBJS)))

# Outputs ######################################################################

# Shared Object
$$(1)_SO = $(OUT)/$$$(1)/$(OUTPUT_NAME).so

# Static Archive
$$(1)_AR = $(OUT)/$$$(1)/$(OUTPUT_NAME).a

# Nanoapp Header
$$(1)_HEADER = $$(if $(IS_NANOAPP_BUILD), $(OUT)/$$$(1)/$(OUTPUT_NAME).napp_header, )

# Optional Binary
$$(1)_BIN = $$(if $(9), $(OUT)/$$$(1)/$(OUTPUT_NAME), )

# Top-level Build Rule #########################################################

# Define the phony target.
.PHONY: $(1)_ar
$(1)_ar: $$($$(1)_AR)

.PHONY: $(1)_so
$(1)_so: $$($$(1)_SO)

.PHONY: $(1)_bin
$(1)_bin: $$($$(1)_BIN)

.PHONY: $(1)_header
$(1)_header: $$($$(1)_HEADER)

.PHONY: $(1)
$(1): $(1)_ar $(1)_so $(1)_bin $(1)_header

# If building the runtime, simply add the archive and shared object to the all
# target. When building CHRE, it is expected that this runtime just be linked
# into another build system (or the entire runtime is built using another build
# system).
ifeq ($(IS_NANOAPP_BUILD),)
all: $(1)
endif

# Nanoapp Header Generation ####################################################

#
# Whoa there... what have we here? Some binary file generation ala bash? ಠ_ಠ
#
# The following build rule generates a nanoapp header. A nanoapp header is a
# small binary blob that is prepended to a nanoapp. Android can parse this
# blob to determine some attributes about the nanoapp, such as version and
# target hub. The layout is as follows:
#
# struct NanoAppBinaryHeader {
#   uint32_t headerVersion;        // 0x1 for this version
#   uint32_t magic;                // "NANO"
#   uint64_t appId;                // App Id, contains vendor id
#   uint32_t appVersion;           // Version of the app
#   uint32_t flags;                // Signed, encrypted
#   uint64_t hwHubType;            // Which hub type is this compiled for
#   uint8_t targetChreApiMajorVersion; // CHRE API version
#   uint8_t targetChreApiMinorVersion;
#   uint8_t reserved[6];
# } __attribute__((packed));
#
# The basic idea here is to generate a hexdump formatted file and then reverse
# that hexdump into binary form. The way that is accomplished is as follows.
#
# ... Why tho?
#
# The build system has a lot of knowledge of what it is building: the name of
# the nanoapp, the version and the app ID. Marshalling this data from the
# Makefile environment into something like python or even a small C program
# is an unnecessary step.

$$($$(1)_HEADER):
	printf "00000000  %.8x " `$(BE_TO_LE_SCRIPT) 0x00000001` > $$@
	printf "%.8x " `$(BE_TO_LE_SCRIPT) 0x4f4e414e` >> $$@
	printf "%.16x\n" `$(BE_TO_LE_SCRIPT) $(NANOAPP_ID)` >> $$@
	printf "00000010  %.8x " `$(BE_TO_LE_SCRIPT) $(NANOAPP_VERSION)` >> $$@
	printf "%.8x " `$(BE_TO_LE_SCRIPT) 0x00000001` >> $$@
	printf "%.16x\n" `$(BE_TO_LE_SCRIPT) $(13)` >> $$@
	printf "00000020  %.2x " \
	    `$(BE_TO_LE_SCRIPT) $(TARGET_CHRE_API_VERSION_MAJOR)` >> $$@
	printf "%.2x " \
	    `$(BE_TO_LE_SCRIPT) $(TARGET_CHRE_API_VERSION_MINOR)` >> $$@
	printf "%.12x \n" `$(BE_TO_LE_SCRIPT) 0x000000` >> $$@
	cp $$@ $$@_ascii
	xxd -r $$@_ascii > $$@
	rm $$@_ascii

# Compile ######################################################################

$$($$(1)_CPP_OBJS): $(OUT)/$$($$(1)_OBJS_DIR)/%.o: %.cpp
	$(3) $(COMMON_CXX_CFLAGS) -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< \
	    -o $$@

$$($$(1)_CC_OBJS): $(OUT)/$$($$(1)_OBJS_DIR)/%.o: %.cc
	$(3) $(COMMON_CXX_CFLAGS) -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< \
	    -o $$@

$$($$(1)_C_OBJS): $(OUT)/$$($$(1)_OBJS_DIR)/%.o: %.c
	$(3) $(COMMON_C_CFLAGS) -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< \
	    -o $$@

$$($$(1)_S_OBJS): $(OUT)/$$($$(1)_OBJS_DIR)/%.o: %.S
	$(3) -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< \
	    -o $$@

# Archive ######################################################################

# Add common and target-specific archive flags.
$$$(1)_ARFLAGS = $(COMMON_ARFLAGS) \
    $(6)

$$($$(1)_AR): $$(OUT)/$$$(1) $$($$$(1)_DIRS) $$($$(1)_CC_OBJS) \
              $$($$(1)_CPP_OBJS) $$($$(1)_C_OBJS) $$($$(1)_S_OBJS)
	$(7) $$($$$(1)_ARFLAGS) $$@ $$(filter %.o, $$^)

# Link #########################################################################

$$($$(1)_SO): $$(OUT)/$$$(1) $$($$$(1)_DIRS) $$($$(1)_CC_DEPS) \
              $$($$(1)_CPP_DEPS) $$($$(1)_C_DEPS) $$($$(1)_S_DEPS) \
              $$($$(1)_CC_OBJS) $$($$(1)_CPP_OBJS) $$($$(1)_C_OBJS) \
              $$($$(1)_S_OBJS)
	$(5) $(4) -o $$@ $(11) $$(filter %.o, $$^) $(12)

$$($$(1)_BIN): $$(OUT)/$$$(1) $$($$$(1)_DIRS) $$($$(1)_CC_DEPS) \
               $$($$(1)_CPP_DEPS) $$($$(1)_C_DEPS) $$($$(1)_S_DEPS) \
               $$($$(1)_CC_OBJS) $$($$(1)_CPP_OBJS) $$($$(1)_C_OBJS) \
               $$($$(1)_S_OBJS)
	$(3) -o $$@ $(11) $$(filter %.o, $$^) $(12) $(10)

# Output Directories ###########################################################

$$($$$(1)_DIRS):
	mkdir -p $$@

$$(OUT)/$$$(1):
	mkdir -p $$@

# Automatic Dependency Resolution ##############################################

$$($$(1)_CC_DEPS): $(OUT)/$$($$(1)_OBJS_DIR)/%.d: %.cc
	mkdir -p $$(dir $$@)
	$(3) $(DEP_CFLAGS) $(COMMON_CXX_CFLAGS) \
	    -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< -o $$@

$$($$(1)_CPP_DEPS): $(OUT)/$$($$(1)_OBJS_DIR)/%.d: %.cpp
	mkdir -p $$(dir $$@)
	$(3) $(DEP_CFLAGS) $(COMMON_CXX_CFLAGS) \
	    -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< -o $$@

$$($$(1)_C_DEPS): $(OUT)/$$($$(1)_OBJS_DIR)/%.d: %.c
	mkdir -p $$(dir $$@)
	$(3) $(DEP_CFLAGS) $(COMMON_C_CFLAGS) \
	    -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< -o $$@

$$($$(1)_S_DEPS): $(OUT)/$$($$(1)_OBJS_DIR)/%.d: %.S
	mkdir -p $$(dir $$@)
	$(3) $(DEP_CFLAGS) \
	    -DCHRE_FILENAME=\"$$(notdir $$<)\" $(2) -c $$< -o $$@

# Include generated dependency files if they are in the requested build target.
# This avoids dependency generation from occuring for a debug target when a
# non-debug target is requested.
ifneq ($(filter $(1) all, $(MAKECMDGOALS)),)
-include $$(patsubst %.o, %.d, $$($$(1)_CC_DEPS))
-include $$(patsubst %.o, %.d, $$($$(1)_CPP_DEPS))
-include $$(patsubst %.o, %.d, $$($$(1)_C_DEPS))
-include $$(patsubst %.o, %.d, $$($$(1)_S_DEPS))
endif

endef
endif

# Template Invocation ##########################################################

TARGET_CFLAGS_LOCAL = $(TARGET_CFLAGS)
TARGET_CFLAGS_LOCAL += -DCHRE_PLATFORM_ID=$(TARGET_PLATFORM_ID)
$(eval $(call BUILD_TEMPLATE, $(TARGET_NAME), \
                              $(COMMON_CFLAGS) $(TARGET_CFLAGS_LOCAL), \
                              $(TARGET_CC), \
                              $(TARGET_SO_LDFLAGS), \
                              $(TARGET_LD), \
                              $(TARGET_ARFLAGS), \
                              $(TARGET_AR), \
                              $(TARGET_VARIANT_SRCS), \
                              $(TARGET_BUILD_BIN), \
                              $(TARGET_BIN_LDFLAGS), \
                              $(TARGET_SO_EARLY_LIBS), \
                              $(TARGET_SO_LATE_LIBS), \
                              $(TARGET_PLATFORM_ID)))

# Debug Template Invocation ####################################################

$(eval $(call BUILD_TEMPLATE, $(TARGET_NAME)_debug, \
                              $(COMMON_CFLAGS) $(COMMON_DEBUG_CFLAGS) \
                                  $(TARGET_CFLAGS_LOCAL) $(TARGET_DEBUG_CFLAGS), \
                              $(TARGET_CC), \
                              $(TARGET_SO_LDFLAGS), \
                              $(TARGET_LD), \
                              $(TARGET_ARFLAGS), \
                              $(TARGET_AR), \
                              $(TARGET_VARIANT_SRCS), \
                              $(TARGET_BUILD_BIN), \
                              $(TARGET_BIN_LDFLAGS), \
                              $(TARGET_SO_EARLY_LIBS), \
                              $(TARGET_SO_LATE_LIBS), \
                              $(TARGET_PLATFORM_ID)))
