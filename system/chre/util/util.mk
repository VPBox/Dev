#
# Util Makefile
#

# Common Compiler Flags ########################################################

# Include paths.
COMMON_CFLAGS += -Iutil/include

# Common Source Files ##########################################################

COMMON_SRCS += util/buffer_base.cc
COMMON_SRCS += util/dynamic_vector_base.cc
COMMON_SRCS += util/nanoapp/audio.cc
COMMON_SRCS += util/nanoapp/debug.cc
COMMON_SRCS += util/nanoapp/sensor.cc
COMMON_SRCS += util/nanoapp/wifi.cc
COMMON_SRCS += util/system/debug_dump.cc

# GoogleTest Source Files ######################################################

GOOGLETEST_SRCS += util/tests/array_queue_test.cc
GOOGLETEST_SRCS += util/tests/blocking_queue_test.cc
GOOGLETEST_SRCS += util/tests/buffer_test.cc
GOOGLETEST_SRCS += util/tests/dynamic_vector_test.cc
GOOGLETEST_SRCS += util/tests/fixed_size_vector_test.cc
GOOGLETEST_SRCS += util/tests/heap_test.cc
GOOGLETEST_SRCS += util/tests/lock_guard_test.cc
GOOGLETEST_SRCS += util/tests/memory_pool_test.cc
GOOGLETEST_SRCS += util/tests/optional_test.cc
GOOGLETEST_SRCS += util/tests/priority_queue_test.cc
GOOGLETEST_SRCS += util/tests/singleton_test.cc
GOOGLETEST_SRCS += util/tests/time_test.cc
GOOGLETEST_SRCS += util/tests/unique_ptr_test.cc
