#
# FlatBuffers Makefile - include this and add $(FLATBUFFERS_CFLAGS) to the build
# configuration as needed
#

FLATBUFFERS_PATH = external/flatbuffers

FLATBUFFERS_CFLAGS := -I$(FLATBUFFERS_PATH)/include
FLATBUFFERS_CFLAGS += -DFLATBUFFERS_CHRE
