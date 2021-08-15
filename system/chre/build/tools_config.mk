#
# Common global compiler configuration
#

# Common Compiler Flags ########################################################

# CHRE requires C++11 and C99 support.
COMMON_CXX_CFLAGS += -std=c++11
COMMON_C_CFLAGS += -x c
COMMON_C_CFLAGS += -std=c99

# Configure 'all' and 'extra' warnings and promote warnings to errors.
COMMON_CFLAGS += -Wall
COMMON_CFLAGS += -Wextra
COMMON_CFLAGS += -Werror

# Disabled warnings. You better have a good reason to add more here.
COMMON_CFLAGS += -Wno-unused-parameter

# Additional warnings. Even more! :]
COMMON_CFLAGS += -Wshadow
COMMON_CFLAGS += -Wdouble-promotion

# Disable exceptions and RTTI.
COMMON_CXX_CFLAGS += -fno-exceptions
COMMON_CXX_CFLAGS += -fno-rtti

# Enable the linker to garbage collect unused code and variables.
COMMON_CFLAGS += -fdata-sections
COMMON_CFLAGS += -ffunction-sections

# Enable debugging symbols for debug builds.
COMMON_DEBUG_CFLAGS += -g

# Dependency Resolution
DEP_CFLAGS = -MM -MG -MP -MF $$(basename $$@).d

# Compile with hidden visibility by default.
COMMON_CFLAGS += -fvisibility=hidden

# Common Archive Flags #########################################################

COMMON_ARFLAGS += rsc
