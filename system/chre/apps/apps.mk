#
# Apps Makefile
#

# Common Compiler Flags ########################################################

COMMON_CFLAGS += -Iapps/include

# App makefiles ################################################################

include apps/ash_world/ash_world.mk
include apps/audio_world/audio_world.mk
include apps/gnss_world/gnss_world.mk
include apps/hello_world/hello_world.mk
include apps/host_awake_world/host_awake_world.mk
include apps/message_world/message_world.mk
include apps/sensor_world/sensor_world.mk
include apps/spammer/spammer.mk
include apps/timer_world/timer_world.mk
include apps/unload_tester/unload_tester.mk
include apps/wifi_world/wifi_world.mk
include apps/wwan_world/wwan_world.mk
