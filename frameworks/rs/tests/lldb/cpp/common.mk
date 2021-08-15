LOCAL_MODULE_TAGS := tests

LOCAL_RENDERSCRIPT_FLAGS += -g -O0 -target-api 0
LOCAL_CFLAGS := -Werror -Wall -Wextra -std=c++11
LOCAL_LDFLAGS += -llog

LOCAL_STATIC_LIBRARIES += libRScpp_static

intermediates += $(call intermediates-dir-for,STATIC_LIBRARIES,libRS,TARGET,)

LOCAL_C_INCLUDES += $(intermediates)
