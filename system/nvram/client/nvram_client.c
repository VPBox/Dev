/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hardware/nvram.h>

#define countof(array) (sizeof(array) / sizeof((array)[0]))

// Exit status codes. These are all negative as the positive ones are used for
// the NV_RESULT_ codes.
enum StatusCode {
  kStatusInvalidArg = -1,
  kStatusHALError = -2,
  kStatusAllocationFailure = -3,
};

static struct {
  int status;
  const char* description;
} kStatusStringTable[] = {
    {kStatusInvalidArg, "Bad parameter"},
    {kStatusHALError, "NVRAM HAL initialization error"},
    {kStatusAllocationFailure, "Memory allocation error"},
    {NV_RESULT_SUCCESS, "Success"},
    {NV_RESULT_INTERNAL_ERROR, "Internal error"},
    {NV_RESULT_ACCESS_DENIED, "Access denied"},
    {NV_RESULT_INVALID_PARAMETER, "Invalid NVRAM parameter"},
    {NV_RESULT_SPACE_DOES_NOT_EXIST, "Space does not exist"},
    {NV_RESULT_SPACE_ALREADY_EXISTS, "Space already exists"},
    {NV_RESULT_OPERATION_DISABLED, "Operation disabled"},
};

// Returns a string describing |status|.
static const char* StatusToString(int status) {
  for (size_t i = 0; i < countof(kStatusStringTable); ++i) {
    if (kStatusStringTable[i].status == status) {
      return kStatusStringTable[i].description;
    }
  }

  return "unknown error";
}

// A table mapping control values to names.
static struct {
  nvram_control_t control;
  const char* name;
} kControlNameTable[] = {
    {NV_CONTROL_PERSISTENT_WRITE_LOCK, "PERSISTENT_WRITE_LOCK"},
    {NV_CONTROL_BOOT_WRITE_LOCK, "BOOT_WRITE_LOCK"},
    {NV_CONTROL_BOOT_READ_LOCK, "BOOT_READ_LOCK"},
    {NV_CONTROL_WRITE_AUTHORIZATION, "WRITE_AUTHORIZATION"},
    {NV_CONTROL_READ_AUTHORIZATION, "READ_AUTHORIZATION"},
    {NV_CONTROL_WRITE_EXTEND, "WRITE_EXTEND"},
};

// Returns the string representation of |control|, or NULL if |control| isn't a
// valid control value.
static const char* ControlToString(nvram_control_t control) {
  for (size_t i = 0; i < countof(kControlNameTable); ++i) {
    if (kControlNameTable[i].control == control) {
      return kControlNameTable[i].name;
    }
  }

  return NULL;
}

// Sets |control| to the NV_CONTROL_ value corresponding to the string control
// representation found in |name|. Returns 0 if successful, 1 if name doesn't
// match any control string.
static int StringToControl(const char* name, nvram_control_t* control) {
  for (size_t i = 0; i < countof(kControlNameTable); ++i) {
    if (strcmp(kControlNameTable[i].name, name) == 0) {
      *control = kControlNameTable[i].control;
      return 0;
    }
  }

  return 1;
}

static int HandleGetTotalSize(nvram_device_t* device, char* args[]) {
  (void)args;
  uint64_t total_size = 0;
  nvram_result_t result = device->get_total_size_in_bytes(device, &total_size);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  printf("%" PRIu64 "\n", total_size);
  return 0;
}

static int HandleGetAvailableSize(nvram_device_t* device, char* args[]) {
  (void)args;
  uint64_t available_size = 0;
  nvram_result_t result =
      device->get_available_size_in_bytes(device, &available_size);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  printf("%" PRIu64 "\n", available_size);
  return 0;
}

static int HandleGetMaxSpaceSize(nvram_device_t* device, char* args[]) {
  (void)args;
  uint64_t max_space_size = 0;
  nvram_result_t result =
      device->get_max_space_size_in_bytes(device, &max_space_size);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  printf("%" PRIu64 "\n", max_space_size);
  return 0;
}

static int HandleGetMaxSpaces(nvram_device_t* device, char* args[]) {
  (void)args;
  uint32_t max_spaces = 0;
  nvram_result_t result = device->get_max_spaces(device, &max_spaces);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  printf("%" PRIu32 "\n", max_spaces);
  return 0;
}

static int HandleGetSpaceList(nvram_device_t* device, char* args[]) {
  (void)args;
  uint32_t list_size = 0;
  nvram_result_t result = device->get_space_list(device, 0, NULL, &list_size);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  uint32_t* space_index_list = calloc(list_size, sizeof(uint32_t));
  if (!space_index_list) {
    return kStatusAllocationFailure;
  }

  result =
      device->get_space_list(device, list_size, space_index_list, &list_size);
  if (result != NV_RESULT_SUCCESS) {
    free(space_index_list);
    return result;
  }

  for (uint32_t i = 0; i < list_size; ++i) {
    if (i != 0) {
      fputs(",", stdout);
    }
    printf("%" PRIu32, space_index_list[i]);
  }
  fputs("\n", stdout);

  free(space_index_list);
  return 0;
}

static int HandleGetSpaceSize(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  uint64_t space_size = 0;
  nvram_result_t result = device->get_space_size(device, index, &space_size);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  printf("%" PRIu64 "\n", space_size);
  return 0;
}

static int HandleGetSpaceControls(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  uint32_t list_size = 0;
  nvram_result_t result =
      device->get_space_controls(device, index, 0, NULL, &list_size);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  uint32_t* controls_list = calloc(list_size, sizeof(nvram_control_t));
  if (!controls_list) {
    return kStatusAllocationFailure;
  }

  result = device->get_space_controls(device, index, list_size, controls_list,
                                      &list_size);
  if (result != NV_RESULT_SUCCESS) {
    free(controls_list);
    return result;
  }

  for (uint32_t i = 0; i < list_size; ++i) {
    if (i != 0) {
      fputs(",", stdout);
    }
    const char* name = ControlToString(controls_list[i]);
    if (name) {
      fputs(name, stdout);
    } else {
      printf("<unknown_control_%" PRIu32 ">", controls_list[i]);
    }
  }
  fputs("", stdout);

  free(controls_list);
  return 0;
}

static int HandleIsSpaceReadLocked(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  int read_locked = 0;
  int write_locked = 0;
  nvram_result_t result =
      device->is_space_locked(device, index, &read_locked, &write_locked);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  printf("%d\n", read_locked);
  return 0;
}

static int HandleIsSpaceWriteLocked(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  int read_locked = 0;
  int write_locked = 0;
  nvram_result_t result =
      device->is_space_locked(device, index, &read_locked, &write_locked);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  printf("%d\n", write_locked);
  return 0;
}

static int HandleCreateSpace(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  uint64_t size = strtoull(args[1], NULL, 0);
  uint32_t list_size = 0;
  nvram_control_t* controls_list = NULL;
  char* tail = args[2];
  while (tail) {
    ++list_size;
    nvram_control_t* new_controls_list =
        realloc(controls_list, sizeof(nvram_control_t) * list_size);
    if (new_controls_list) {
      controls_list = new_controls_list;
    } else {
      free(controls_list);
      return kStatusAllocationFailure;
    }

    if (StringToControl(strsep(&tail, ","), &(controls_list[list_size - 1]))) {
      free(controls_list);
      return kStatusInvalidArg;
    }
  }

  return device->create_space(device, index, size, controls_list, list_size,
                              (uint8_t*)args[3], strlen(args[3]));
}

static int HandleDeleteSpace(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  return device->delete_space(device, index, (uint8_t*)args[3],
                              strlen(args[3]));
}

static int HandleDisableCreate(nvram_device_t* device, char* args[]) {
  (void)args;
  return device->disable_create(device);
}

static int HandleWriteSpace(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  return device->write_space(device, index, (uint8_t*)args[1], strlen(args[1]),
                             (uint8_t*)args[2], strlen(args[2]));
}

static int HandleReadSpace(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  uint64_t size = 0;
  nvram_result_t result = device->get_space_size(device, index, &size);
  if (result != NV_RESULT_SUCCESS) {
    return result;
  }

  uint8_t* buffer = calloc(sizeof(uint8_t), size);
  if (!buffer) {
    return kStatusAllocationFailure;
  }

  result = device->read_space(device, index, size, (uint8_t*)args[1],
                              strlen(args[1]), buffer, &size);
  if (result != NV_RESULT_SUCCESS) {
    free(buffer);
    return result;
  }

  fwrite(buffer, sizeof(uint8_t), size, stdout);
  fputs("\n", stdout);
  free(buffer);
  return 0;
}

static int HandleEnableWriteLock(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  return device->enable_write_lock(device, index, (uint8_t*)args[1],
                                   strlen(args[1]));
}

static int HandleEnableReadLock(nvram_device_t* device, char* args[]) {
  uint32_t index = strtoul(args[0], NULL, 0);
  return device->enable_read_lock(device, index, (uint8_t*)args[1],
                                  strlen(args[1]));
}

struct CommandHandler {
  const char* name;
  const char* params_desc;
  int nparams;
  int (*run)(nvram_device_t*, char* args[]);
};

struct CommandHandler kCommandHandlers[] = {
    {"get_total_size", "", 0, &HandleGetTotalSize},
    {"get_available_size", "", 0, &HandleGetAvailableSize},
    {"get_max_space_size", "", 0, &HandleGetMaxSpaceSize},
    {"get_max_spaces", "", 0, &HandleGetMaxSpaces},
    {"get_space_list", "", 0, &HandleGetSpaceList},
    {"get_space_size", "<index>", 1, &HandleGetSpaceSize},
    {"get_space_controls", "<index>", 1, &HandleGetSpaceControls},
    {"is_space_read_locked", "<index>", 1, &HandleIsSpaceReadLocked},
    {"is_space_write_locked", "<index>", 1, &HandleIsSpaceWriteLocked},
    {"create_space", "<index> <size> <controls> <auth>", 4, &HandleCreateSpace},
    {"delete_space", "<index> <auth>", 2, &HandleDeleteSpace},
    {"disable_create", "", 0, &HandleDisableCreate},
    {"write_space", "<index> <data> <auth>", 3, &HandleWriteSpace},
    {"read_space", "<index> <auth>", 2, &HandleReadSpace},
    {"enable_write_lock", "<index> <auth>", 2, &HandleEnableWriteLock},
    {"enable_read_lock", "<index> <auth>", 2, &HandleEnableReadLock},
};

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <command> <command-args>\n", argv[0]);
    fprintf(stderr, "Valid commands are:\n");
    for (size_t i = 0; i < countof(kCommandHandlers); ++i) {
      fprintf(stderr, "  %s %s\n", kCommandHandlers[i].name,
              kCommandHandlers[i].params_desc);
    }
    return kStatusInvalidArg;
  }

  const struct CommandHandler* cmd = NULL;
  for (size_t i = 0; i < countof(kCommandHandlers); ++i) {
    if (strcmp(kCommandHandlers[i].name, argv[1]) == 0) {
      cmd = &kCommandHandlers[i];
    }
  }

  if (!cmd) {
    fprintf(stderr, "Bad command: %s\n", argv[1]);
    return kStatusInvalidArg;
  }

  if (argc - 2 != cmd->nparams) {
    fprintf(stderr, "Command %s takes %d parameters, %d given.\n", argv[1],
            cmd->nparams, argc - 2);
    return kStatusInvalidArg;
  }

  const hw_module_t* module = NULL;
  nvram_device_t* nvram_device = NULL;
  if (hw_get_module(NVRAM_HARDWARE_MODULE_ID, &module) != 0 ||
      module->methods->open(module, NVRAM_HARDWARE_DEVICE_ID,
                            (hw_device_t**)&nvram_device) != 0) {
    fprintf(stderr, "Failed to open NVRAM HAL.\n");
    return kStatusHALError;
  }

  if (nvram_device->common.version != NVRAM_DEVICE_API_VERSION_1_1) {
    fprintf(stderr, "Unsupported NVRAM HAL version.\n");
    nvram_device->common.close(&nvram_device->common);
    return kStatusHALError;
  }

  int ret = cmd->run(nvram_device, argv + 2);
  if (ret != 0) {
    fprintf(stderr, "Command execution failure: %s (%d).\n",
            StatusToString(ret), ret);
  }

  nvram_device->common.close(&nvram_device->common);
  return ret;
}
