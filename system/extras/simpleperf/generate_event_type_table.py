#!/usr/bin/python
#
# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


def gen_event_type_entry_str(event_type_name, event_type, event_config, description='',
                             limited_arch=''):
  """
  return string as below:
  EVENT_TYPE_TABLE_ENTRY(event_type_name, event_type, event_config, description, limited_arch)
  """
  return 'EVENT_TYPE_TABLE_ENTRY("%s", %s, %s, "%s", "%s")\n' % (
            event_type_name, event_type, event_config, description, limited_arch)

def gen_arm_event_type_entry_str(event_type_name, event_type, event_config, description):
  return gen_event_type_entry_str(event_type_name, event_type, event_config, description,
                                  "arm")


def gen_hardware_events():
  hardware_configs = ["cpu-cycles",
                      "instructions",
                      "cache-references",
                      "cache-misses",
                      "branch-instructions",
                      "branch-misses",
                      "bus-cycles",
                      "stalled-cycles-frontend",
                      "stalled-cycles-backend",
                      ]
  generated_str = ""
  for config in hardware_configs:
    event_type_name = config
    event_config = "PERF_COUNT_HW_" + config.replace('-', '_').upper()

    generated_str += gen_event_type_entry_str(
        event_type_name, "PERF_TYPE_HARDWARE", event_config)

  return generated_str


def gen_software_events():
  software_configs = ["cpu-clock",
                      "task-clock",
                      "page-faults",
                      "context-switches",
                      "cpu-migrations",
                      ["minor-faults", "PERF_COUNT_SW_PAGE_FAULTS_MIN"],
                      ["major-faults", "PERF_COUNT_SW_PAGE_FAULTS_MAJ"],
                      "alignment-faults",
                      "emulation-faults",
                      ]
  generated_str = ""
  for config in software_configs:
    if isinstance(config, list):
      event_type_name = config[0]
      event_config = config[1]
    else:
      event_type_name = config
      event_config = "PERF_COUNT_SW_" + config.replace('-', '_').upper()

    generated_str += gen_event_type_entry_str(
        event_type_name, "PERF_TYPE_SOFTWARE", event_config)

  return generated_str


def gen_hw_cache_events():
  hw_cache_types = [["L1-dcache", "PERF_COUNT_HW_CACHE_L1D"],
                    ["L1-icache", "PERF_COUNT_HW_CACHE_L1I"],
                    ["LLC", "PERF_COUNT_HW_CACHE_LL"],
                    ["dTLB", "PERF_COUNT_HW_CACHE_DTLB"],
                    ["iTLB", "PERF_COUNT_HW_CACHE_ITLB"],
                    ["branch", "PERF_COUNT_HW_CACHE_BPU"],
                    ["node", "PERF_COUNT_HW_CACHE_NODE"],
                    ]
  hw_cache_ops = [["loads", "load", "PERF_COUNT_HW_CACHE_OP_READ"],
                  ["stores", "store", "PERF_COUNT_HW_CACHE_OP_WRITE"],
                  ["prefetches", "prefetch",
                   "PERF_COUNT_HW_CACHE_OP_PREFETCH"],
                  ]
  hw_cache_op_results = [["accesses", "PERF_COUNT_HW_CACHE_RESULT_ACCESS"],
                         ["misses", "PERF_COUNT_HW_CACHE_RESULT_MISS"],
                         ]
  generated_str = ""
  for (type_name, type_config) in hw_cache_types:
    for (op_name_access, op_name_miss, op_config) in hw_cache_ops:
      for (result_name, result_config) in hw_cache_op_results:
        if result_name == "accesses":
          event_type_name = type_name + '-' + op_name_access
        else:
          event_type_name = type_name + '-' + \
              op_name_miss + '-' + result_name
        event_config = "((%s) | (%s << 8) | (%s << 16))" % (
            type_config, op_config, result_config)
        generated_str += gen_event_type_entry_str(
            event_type_name, "PERF_TYPE_HW_CACHE", event_config)

  return generated_str

def gen_user_space_events():
  generated_str = gen_event_type_entry_str("inplace-sampler",
                                           "SIMPLEPERF_TYPE_USER_SPACE_SAMPLERS",
                                           "SIMPLEPERF_CONFIG_INPLACE_SAMPLER")
  return generated_str

def gen_arm_raw_events():
  # Refer to "Table D5-7 PMU event numbers" in ARMv8 specification.
  raw_types = [
               [0x00, "sw-incr", "software increment"],
               [0x01, "l1-icache-refill", "level 1 instruction cache refill"],
               [0x02, "l1-itlb-refill", "level 1 instruction TLB refill"],
               [0x03, "l1-dcache-refill", "level 1 data cache refill"],
               [0x04, "l1-dcache", "level 1 data cache access"],
               [0x05, "l1-dtlb-refill", "level 1 data TLB refill"],
               [0x06, "load-retired", "load (instruction architecturally executed)"],
               [0x07, "store-retired", "store (instruction architecturally executed)"],
               [0x08, "instruction-retired", "instructions (instruction architecturally executed)"],
               [0x09, "exception-taken", "exception taken"],
               [0x0a, "exception-return", "exception return (instruction architecturally executed)"],
               [0x0b, "cid-write-retired", "write to CONTEXIDR (instruction architecturally executed)"],
               [0x0c, "pc-write-retired", "software change of the PC (instruction architecturally executed)"],
               [0x0d, "br-immed-retired", "immediate branch (instruction architecturally executed)"],
               [0x0e, "br-return-retired", "procedure return (instruction architecturally executed)"],
               [0x0f, "unaligned-ldst-retired", "unaligned load or store (instruction architecturally executed)"],
               [0x10, "br-mis-pred", "mispredicted or not predicted branch speculatively executed"],
               [0x11, "cpu-cycles", "cpu cycles"],
               [0x12, "br-pred", "predictable branch speculatively executed"],
               [0x13, "mem-access", "data memory access"],
               [0x14, "l1-icache", "level 1 instruction cache access"],
               [0x15, "l1-dcache-wb", "level 1 data cache write-back"],
               [0x16, "l2-dcache", "level 2 data cache access"],
               [0x17, "l2-dcache-refill", "level 2 data cache refill"],
               [0x18, "l2-dcache-wb", "level 2 data cache write-back"],
               [0x19, "bus-access", "bus access"],
               [0x1a, "memory-error", "local memory error"],
               [0x1b, "inst-spec", "operation speculatively executed"],
               [0x1c, "ttbr-write-retired", "write to TTBR (instruction architecturally executed)"],
               [0x1d, "bus-cycles", "bus cycle"],
               # [0x1e, "chain", ""], // Not useful in user space.
               [0x1f, "l1-dcache-allocate", "level 1 data cache allocation without refill"],
               [0x20, "l2-dcache-allocate", "level 2 data cache allocation without refill"],
               [0x21, "br-retired", "branch (instruction architecturally executed)"],
               [0x22, "br-mis-pred-retired", "mispredicted branch (instruction architecturally executed)"],
               [0x23, "stall-frontend", "no operation issued due to the frontend"],
               [0x24, "stall-backend", "no operation issued due to the backend"],
               [0x25, "l1-dtlb", "level 1 data or unified TLB access"],
               [0x26, "l1-itlb", "level 1 instruction TLB access"],
               [0x27, "l2-icache", "level 2 instruction cache access"],
               [0x28, "l2-icache-refill", "level 2 instruction cache refill"],
               [0x29, "l3-dcache-allocate", "level 3 data or unified cache allocation without refill"],
               [0x2a, "l3-dcache-refill", "level 3 data or unified cache refill"],
               [0x2b, "l3-dcache", "level 3 data or unified cache access"],
               [0x2c, "l3-dcache-wb", "level 3 data or unified cache write-back"],
               [0x2d, "l2-dtlb-refill", "level 2 data or unified TLB refill"],
               [0x2e, "l2-itlb-refill", "level 2 instruction TLB refill"],
               [0x2f, "l2-dtlb", "level 2 data or unified TLB access"],
               [0x30, "l2-itlb", "level 2 instruction TLB access"],
               ]
  generated_str = ""
  for item in raw_types:
    event_type = 'PERF_TYPE_RAW'
    event_type_name = "raw-" + item[1]
    event_config = '0x%x' % item[0]
    description = item[2]
    generated_str += gen_arm_event_type_entry_str(event_type_name, event_type, event_config,
                                              description)
  return generated_str


def gen_events():
  generated_str = "// This file is auto-generated by generate-event_table.py.\n\n"
  generated_str += gen_hardware_events() + '\n'
  generated_str += gen_software_events() + '\n'
  generated_str += gen_hw_cache_events() + '\n'
  generated_str += gen_user_space_events() + '\n'
  generated_str += gen_arm_raw_events() + '\n'
  return generated_str

generated_str = gen_events()
fh = open('event_type_table.h', 'w')
fh.write(generated_str)
fh.close()
