#
# Nanoapp NanoPB Makefile
#
# Include this file in your nanoapp Makefile to produce pb.c and pb.h (or
# $NANOPB_EXTENSION.c and $NANOPB_EXTENSION.h if $NANOPB_EXTENSION is defined)
# for .proto files specified in the NANOPB_SRCS variable. The produced pb.c or
# $NANOPB_EXTENSION.c files are automatically added to the COMMON_SRCS variable
# for the nanoapp build.
#
# NANOPB_FLAGS can be used to supply additional command line arguments to the
# nanopb compiler. Note that this is global and applies to all protobuf
# generated source.

# Environment Checks ###########################################################

ifneq ($(NANOPB_SRCS),)
ifeq ($(NANOPB_PREFIX),)
$(error "NANOPB_SRCS is non-empty. You must supply a NANOPB_PREFIX environment \
         variable containing a path to the nanopb project. Example: \
         export NANOPB_PREFIX=$$HOME/path/to/nanopb/nanopb-c")
endif
endif

ifeq ($(PROTOC),)
PROTOC=protoc
endif

# Generated Source Files #######################################################

NANOPB_GEN_PATH = $(OUT)/nanopb_gen

ifeq ($(NANOPB_EXTENSION),)
NANOPB_EXTENSION = pb
else
NANOPB_GENERATOR_FLAGS = --extension=.$(NANOPB_EXTENSION)
endif

NANOPB_GEN_SRCS += $(patsubst %.proto, \
                              $(NANOPB_GEN_PATH)/%.$(NANOPB_EXTENSION).c, \
                              $(NANOPB_SRCS))

ifneq ($(NANOPB_GEN_SRCS),)
COMMON_CFLAGS += -I$(NANOPB_PREFIX)
COMMON_CFLAGS += -I$(NANOPB_GEN_PATH)
COMMON_CFLAGS += $(addprefix -I$(NANOPB_GEN_PATH)/, $(NANOPB_INCLUDES))

ifneq ($(NANOPB_INCLUDE_LIBRARY),false)
COMMON_SRCS += $(NANOPB_PREFIX)/pb_common.c
COMMON_SRCS += $(NANOPB_PREFIX)/pb_decode.c
COMMON_SRCS += $(NANOPB_PREFIX)/pb_encode.c
endif

endif

# NanoPB Compiler Flags ########################################################

ifneq ($(NANOPB_GEN_SRCS),)
ifneq ($(NANOPB_INCLUDE_LIBRARY),false)
COMMON_CFLAGS += -DPB_NO_PACKED_STRUCTS=1
endif
endif

# NanoPB Generator Setup #######################################################

NANOPB_GENERATOR_SRCS = $(NANOPB_PREFIX)/generator/proto/nanopb_pb2.py
NANOPB_GENERATOR_SRCS += $(NANOPB_PREFIX)/generator/proto/plugin_pb2.py

$(NANOPB_GENERATOR_SRCS):
	cd $(NANOPB_PREFIX)/generator/proto && make

# Generate NanoPB Sources ######################################################

COMMON_SRCS += $(NANOPB_GEN_SRCS)

NANOPB_PROTOC = $(NANOPB_PREFIX)/generator/protoc-gen-nanopb

$(NANOPB_GEN_PATH)/%.$(NANOPB_EXTENSION).c \
        $(NANOPB_GEN_PATH)/%.$(NANOPB_EXTENSION).h: %.proto \
                                                    %.options \
                                                    $(NANOPB_GENERATOR_SRCS)
	mkdir -p $(dir $@)
	$(PROTOC) --plugin=protoc-gen-nanopb=$(NANOPB_PROTOC) $(NANOPB_FLAGS) \
	  --nanopb_out="$(NANOPB_GENERATOR_FLAGS) --options-file=$(basename $<).options:$(NANOPB_GEN_PATH)/$(NANOPB_PROTO_PATH)" \
	  $<

$(NANOPB_GEN_PATH)/%.$(NANOPB_EXTENSION).c \
        $(NANOPB_GEN_PATH)/%.$(NANOPB_EXTENSION).h: %.proto \
                                                    $(NANOPB_OPTIONS) \
                                                    $(NANOPB_GENERATOR_SRCS)
	mkdir -p $(dir $@)
	$(PROTOC) --plugin=protoc-gen-nanopb=$(NANOPB_PROTOC) $(NANOPB_FLAGS) \
	  --nanopb_out="$(NANOPB_GENERATOR_FLAGS) --options-file=$(NANOPB_OPTIONS):$(NANOPB_GEN_PATH)/$(NANOPB_PROTO_PATH)" \
	  $<
