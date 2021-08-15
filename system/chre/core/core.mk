#
# Core Makefile
#

# Common Compiler Flags ########################################################

# Include paths.
COMMON_CFLAGS += -Icore/include

# Common Source Files ##########################################################

COMMON_SRCS += core/event.cc
COMMON_SRCS += core/event_loop.cc
COMMON_SRCS += core/event_loop_manager.cc
COMMON_SRCS += core/event_ref_queue.cc
COMMON_SRCS += core/host_comms_manager.cc
COMMON_SRCS += core/init.cc
COMMON_SRCS += core/nanoapp.cc
COMMON_SRCS += core/sensor.cc
COMMON_SRCS += core/sensor_request.cc
COMMON_SRCS += core/sensor_request_manager.cc
COMMON_SRCS += core/sensor_type.cc
COMMON_SRCS += core/static_nanoapps.cc
COMMON_SRCS += core/timer_pool.cc

# Optional audio support.
ifeq ($(CHRE_AUDIO_SUPPORT_ENABLED), true)
COMMON_SRCS += core/audio_request_manager.cc
endif

# Optional GNSS support.
ifeq ($(CHRE_GNSS_SUPPORT_ENABLED), true)
COMMON_SRCS += core/gnss_manager.cc
endif

# Optional Wi-Fi support.
ifeq ($(CHRE_WIFI_SUPPORT_ENABLED), true)
COMMON_SRCS += core/wifi_request_manager.cc
COMMON_SRCS += core/wifi_scan_request.cc
endif

# Optional WWAN support.
ifeq ($(CHRE_WWAN_SUPPORT_ENABLED), true)
COMMON_SRCS += core/wwan_request_manager.cc
endif

# GoogleTest Source Files ######################################################

GOOGLETEST_SRCS += core/tests/audio_request_manager_test.cc
GOOGLETEST_SRCS += core/tests/memory_manager_test.cc
GOOGLETEST_SRCS += core/tests/request_multiplexer_test.cc
GOOGLETEST_SRCS += core/tests/sensor_request_test.cc
GOOGLETEST_SRCS += core/tests/wifi_scan_request_test.cc
