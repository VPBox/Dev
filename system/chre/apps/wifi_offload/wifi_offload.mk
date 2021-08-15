#
# Wifi Offload Makefile
#

WIFI_OFFLOAD_TYPES_PREFIX = $(CHRE_PREFIX)/apps/wifi_offload
FLAT_BUFFERS_PREFIX = $(CHRE_PREFIX)/external/flatbuffers

# Common Compiler Flags ########################################################

# Include paths.
COMMON_CFLAGS += -I$(WIFI_OFFLOAD_TYPES_PREFIX)/include
COMMON_CFLAGS += -I$(WIFI_OFFLOAD_TYPES_PREFIX)/test/include
COMMON_CFLAGS += -I$(FLAT_BUFFERS_PREFIX)/include

# Common Source Files ##########################################################

COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/channel_histogram.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/chre_scan_params_safe.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/flatbuffers_serialization.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/preferred_network.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/rpc_log_record.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/scan_config.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/scan_filter.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/scan_params.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/scan_record.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/scan_result.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/scan_result_message.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/scan_stats.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/ssid.cc
COMMON_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/utility.cc

# GoogleTest Source Files ######################################################

GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/channelhistogram_test.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/chrescanparamssafe_test.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/flatbuffersserialization_test.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/offloadtypes_test.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/random_generator.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/randomgenerator_test.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/scanresult_test.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/utility.cc
GOOGLETEST_SRCS += $(WIFI_OFFLOAD_TYPES_PREFIX)/test/wifioffloadutility_test.cc
