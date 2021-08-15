#
# External Projects Makefile
#

#
# GoogleTest/GoogleMock
#

GOOGLETEST_PATH = $(GOOGLETEST_PREFIX)/googletest
GOOGLEMOCK_PATH = $(GOOGLETEST_PREFIX)/googlemock

# Common Compiler Flags ########################################################

# Include paths.
GOOGLE_X86_GOOGLETEST_CFLAGS += -I$(GOOGLETEST_PATH)
GOOGLE_X86_GOOGLETEST_CFLAGS += -I$(GOOGLETEST_PATH)/include
GOOGLE_X86_GOOGLETEST_CFLAGS += -I$(GOOGLEMOCK_PATH)
GOOGLE_X86_GOOGLETEST_CFLAGS += -I$(GOOGLEMOCK_PATH)/include

# x86 GoogleTest Source Files ##################################################

GOOGLE_X86_GOOGLETEST_SRCS += $(GOOGLETEST_PATH)/src/gtest-all.cc
GOOGLE_X86_GOOGLETEST_SRCS += $(GOOGLETEST_PATH)/src/gtest_main.cc

# x86 GoogleMock Source Files ##################################################

GOOGLE_X86_GOOGLETEST_SRCS += $(GOOGLEMOCK_PATH)/src/gmock-all.cc

#
# Kiss FFT
#

# Common Compiler Flags ########################################################

# Include paths.
COMMON_CFLAGS += -Iexternal/kiss_fft

# Macros.
COMMON_CFLAGS += -DFIXED_POINT

# Common Source Files ##########################################################

COMMON_SRCS += external/kiss_fft/kiss_fft.c
COMMON_SRCS += external/kiss_fft/kiss_fftr.c
