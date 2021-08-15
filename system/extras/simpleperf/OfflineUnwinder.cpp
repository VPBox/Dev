/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "OfflineUnwinder.h"

#include <sys/mman.h>

#include <android-base/logging.h>
#include <unwindstack/MachineArm.h>
#include <unwindstack/MachineArm64.h>
#include <unwindstack/MachineX86.h>
#include <unwindstack/MachineX86_64.h>
#include <unwindstack/Maps.h>
#include <unwindstack/Regs.h>
#include <unwindstack/RegsArm.h>
#include <unwindstack/RegsArm64.h>
#include <unwindstack/RegsX86.h>
#include <unwindstack/RegsX86_64.h>
#include <unwindstack/Unwinder.h>
#include <unwindstack/UserArm.h>
#include <unwindstack/UserArm64.h>
#include <unwindstack/UserX86.h>
#include <unwindstack/UserX86_64.h>

#include "environment.h"
#include "perf_regs.h"
#include "read_apk.h"
#include "thread_tree.h"

static_assert(simpleperf::map_flags::PROT_JIT_SYMFILE_MAP ==
              unwindstack::MAPS_FLAGS_JIT_SYMFILE_MAP, "");

namespace simpleperf {

// Max frames seen so far is 463, in http://b/110923759.
static constexpr size_t MAX_UNWINDING_FRAMES = 512;

static unwindstack::Regs* GetBacktraceRegs(const RegSet& regs) {
  switch (regs.arch) {
    case ARCH_ARM: {
      unwindstack::arm_user_regs arm_user_regs;
      memset(&arm_user_regs, 0, sizeof(arm_user_regs));
      static_assert(
          static_cast<int>(unwindstack::ARM_REG_R0) == static_cast<int>(PERF_REG_ARM_R0), "");
      static_assert(
          static_cast<int>(unwindstack::ARM_REG_LAST) == static_cast<int>(PERF_REG_ARM_MAX), "");
      for (size_t i = unwindstack::ARM_REG_R0; i < unwindstack::ARM_REG_LAST; ++i) {
        arm_user_regs.regs[i] = static_cast<uint32_t>(regs.data[i]);
      }
      return unwindstack::RegsArm::Read(&arm_user_regs);
    }
    case ARCH_ARM64: {
      unwindstack::arm64_user_regs arm64_user_regs;
      memset(&arm64_user_regs, 0, sizeof(arm64_user_regs));
      static_assert(
          static_cast<int>(unwindstack::ARM64_REG_R0) == static_cast<int>(PERF_REG_ARM64_X0), "");
      static_assert(
          static_cast<int>(unwindstack::ARM64_REG_R30) == static_cast<int>(PERF_REG_ARM64_LR), "");
      memcpy(&arm64_user_regs.regs[unwindstack::ARM64_REG_R0], &regs.data[PERF_REG_ARM64_X0],
             sizeof(uint64_t) * (PERF_REG_ARM64_LR - PERF_REG_ARM64_X0 + 1));
      arm64_user_regs.sp = regs.data[PERF_REG_ARM64_SP];
      arm64_user_regs.pc = regs.data[PERF_REG_ARM64_PC];
      return unwindstack::RegsArm64::Read(&arm64_user_regs);
    }
    case ARCH_X86_32: {
      unwindstack::x86_user_regs x86_user_regs;
      memset(&x86_user_regs, 0, sizeof(x86_user_regs));
      x86_user_regs.eax = static_cast<uint32_t>(regs.data[PERF_REG_X86_AX]);
      x86_user_regs.ebx = static_cast<uint32_t>(regs.data[PERF_REG_X86_BX]);
      x86_user_regs.ecx = static_cast<uint32_t>(regs.data[PERF_REG_X86_CX]);
      x86_user_regs.edx = static_cast<uint32_t>(regs.data[PERF_REG_X86_DX]);
      x86_user_regs.ebp = static_cast<uint32_t>(regs.data[PERF_REG_X86_BP]);
      x86_user_regs.edi = static_cast<uint32_t>(regs.data[PERF_REG_X86_DI]);
      x86_user_regs.esi = static_cast<uint32_t>(regs.data[PERF_REG_X86_SI]);
      x86_user_regs.esp = static_cast<uint32_t>(regs.data[PERF_REG_X86_SP]);
      x86_user_regs.eip = static_cast<uint32_t>(regs.data[PERF_REG_X86_IP]);
      return unwindstack::RegsX86::Read(&x86_user_regs);
    }
    case ARCH_X86_64: {
      unwindstack::x86_64_user_regs x86_64_user_regs;
      memset(&x86_64_user_regs, 0, sizeof(x86_64_user_regs));
      x86_64_user_regs.rax = regs.data[PERF_REG_X86_AX];
      x86_64_user_regs.rbx = regs.data[PERF_REG_X86_BX];
      x86_64_user_regs.rcx = regs.data[PERF_REG_X86_CX];
      x86_64_user_regs.rdx = regs.data[PERF_REG_X86_DX];
      x86_64_user_regs.r8 = regs.data[PERF_REG_X86_R8];
      x86_64_user_regs.r9 = regs.data[PERF_REG_X86_R9];
      x86_64_user_regs.r10 = regs.data[PERF_REG_X86_R10];
      x86_64_user_regs.r11 = regs.data[PERF_REG_X86_R11];
      x86_64_user_regs.r12 = regs.data[PERF_REG_X86_R12];
      x86_64_user_regs.r13 = regs.data[PERF_REG_X86_R13];
      x86_64_user_regs.r14 = regs.data[PERF_REG_X86_R14];
      x86_64_user_regs.r15 = regs.data[PERF_REG_X86_R15];
      x86_64_user_regs.rdi = regs.data[PERF_REG_X86_DI];
      x86_64_user_regs.rsi = regs.data[PERF_REG_X86_SI];
      x86_64_user_regs.rbp = regs.data[PERF_REG_X86_BP];
      x86_64_user_regs.rsp = regs.data[PERF_REG_X86_SP];
      x86_64_user_regs.rip = regs.data[PERF_REG_X86_IP];
      return unwindstack::RegsX86_64::Read(&x86_64_user_regs);
    }
    default:
      return nullptr;
  }
}

static unwindstack::MapInfo* CreateMapInfo(const MapEntry* entry) {
  const char* name = entry->dso->GetDebugFilePath().c_str();
  uint64_t pgoff = entry->pgoff;
  if (entry->pgoff == 0) {
    auto tuple = SplitUrlInApk(entry->dso->GetDebugFilePath());
    if (std::get<0>(tuple)) {
      // The unwinder does not understand the ! format, so change back to
      // the previous format (apk, offset).
      EmbeddedElf* elf = ApkInspector::FindElfInApkByName(std::get<1>(tuple), std::get<2>(tuple));
      if (elf != nullptr) {
        name = elf->filepath().c_str();
        pgoff = elf->entry_offset();
      }
    }
  }
  return new unwindstack::MapInfo(nullptr, entry->start_addr, entry->get_end_addr(), pgoff,
                                  PROT_READ | PROT_EXEC | entry->flags, name);
}

void UnwindMaps::UpdateMaps(const MapSet& map_set) {
  if (version_ == map_set.version) {
    return;
  }
  version_ = map_set.version;
  size_t i = 0;
  size_t old_size = entries_.size();
  for (auto it = map_set.maps.begin(); it != map_set.maps.end();) {
    const MapEntry* entry = it->second;
    if (i < old_size && entry == entries_[i]) {
      i++;
      ++it;
    } else if (i == old_size || entry->start_addr <= entries_[i]->start_addr) {
      // Add an entry.
      entries_.push_back(entry);
      maps_.emplace_back(CreateMapInfo(entry));
      ++it;
    } else {
      // Remove an entry.
      entries_[i] = nullptr;
      maps_[i++] = nullptr;
    }
  }
  while (i < old_size) {
    entries_[i] = nullptr;
    maps_[i++] = nullptr;
  }
  std::sort(entries_.begin(), entries_.end(), [](const auto& e1, const auto& e2) {
    if (e1 == nullptr || e2 == nullptr) {
      return e1 != nullptr;
    }
    return e1->start_addr < e2->start_addr;
  });
  std::sort(maps_.begin(), maps_.end(),
            [](const auto& m1, const auto& m2) {
    if (m1 == nullptr || m2 == nullptr) {
      return m1 != nullptr;
    }
    return m1->start < m2->start;
  });
  entries_.resize(map_set.maps.size());
  maps_.resize(map_set.maps.size());
}

OfflineUnwinder::OfflineUnwinder(bool collect_stat) : collect_stat_(collect_stat) {
  unwindstack::Elf::SetCachingEnabled(true);
}

bool OfflineUnwinder::UnwindCallChain(const ThreadEntry& thread, const RegSet& regs,
                                      const char* stack, size_t stack_size,
                                      std::vector<uint64_t>* ips, std::vector<uint64_t>* sps) {
  uint64_t start_time;
  if (collect_stat_) {
    start_time = GetSystemClock();
  }
  is_callchain_broken_for_incomplete_jit_debug_info_ = false;
  ips->clear();
  sps->clear();
  std::vector<uint64_t> result;
  uint64_t sp_reg_value;
  if (!regs.GetSpRegValue(&sp_reg_value)) {
    LOG(ERROR) << "can't get sp reg value";
    return false;
  }
  uint64_t stack_addr = sp_reg_value;

  UnwindMaps& cached_map = cached_maps_[thread.pid];
  cached_map.UpdateMaps(*thread.maps);
  std::shared_ptr<unwindstack::MemoryOfflineBuffer> stack_memory(
      new unwindstack::MemoryOfflineBuffer(reinterpret_cast<const uint8_t*>(stack),
                                           stack_addr, stack_addr + stack_size));
  std::unique_ptr<unwindstack::Regs> unwind_regs(GetBacktraceRegs(regs));
  if (!unwind_regs) {
    return false;
  }
  unwindstack::Unwinder unwinder(MAX_UNWINDING_FRAMES, &cached_map, unwind_regs.get(),
                                 stack_memory);
  unwinder.SetResolveNames(false);
  unwinder.Unwind();
  size_t last_jit_method_frame = UINT_MAX;
  for (auto& frame : unwinder.frames()) {
    // Unwinding in arm architecture can return 0 pc address.

    // If frame.map.start == 0, this frame doesn't hit any map, it could be:
    // 1. In an executable map not backed by a file. Note that RecordCommand::ShouldOmitRecord()
    //    may omit maps only exist memory.
    // 2. An incorrectly unwound frame. Like caused by invalid stack data, as in
    //    SampleRecord::GetValidStackSize(). Or caused by incomplete JIT debug info.
    // We want to remove this frame and callchains following it in either case.
    if (frame.pc == 0 || frame.map_start == 0) {
      is_callchain_broken_for_incomplete_jit_debug_info_ = true;
      break;
    }
    if (frame.map_flags & unwindstack::MAPS_FLAGS_JIT_SYMFILE_MAP) {
      last_jit_method_frame = ips->size();
    }
    ips->push_back(frame.pc);
    sps->push_back(frame.sp);
  }
  // If the unwound frames stop near to a JITed method, it may be caused by incomplete JIT debug
  // info.
  if (last_jit_method_frame != UINT_MAX && last_jit_method_frame + 3 > ips->size()) {
    is_callchain_broken_for_incomplete_jit_debug_info_ = true;
  }

  uint64_t ip_reg_value;
  if (!regs.GetIpRegValue(&ip_reg_value)) {
    LOG(ERROR) << "can't get ip reg value";
    return false;
  }
  if (ips->empty()) {
    ips->push_back(ip_reg_value);
    sps->push_back(sp_reg_value);
  } else {
    // Check if the unwinder returns ip reg value as the first ip address in callstack.
    CHECK_EQ((*ips)[0], ip_reg_value);
  }
  if (collect_stat_) {
    unwinding_result_.used_time = GetSystemClock() - start_time;
    switch (unwinder.LastErrorCode()) {
      case unwindstack::ERROR_MAX_FRAMES_EXCEEDED:
        unwinding_result_.stop_reason = UnwindingResult::EXCEED_MAX_FRAMES_LIMIT;
        break;
      case unwindstack::ERROR_MEMORY_INVALID: {
        uint64_t addr = unwinder.LastErrorAddress();
        // Because we don't have precise stack range here, just guess an addr is in stack
        // if sp - 128K <= addr <= sp.
        if (addr <= stack_addr && addr >= stack_addr - 128 * 1024) {
          unwinding_result_.stop_reason = UnwindingResult::ACCESS_STACK_FAILED;
        } else {
          unwinding_result_.stop_reason = UnwindingResult::ACCESS_MEM_FAILED;
        }
        unwinding_result_.stop_info.addr = addr;
        break;
      }
      case unwindstack::ERROR_INVALID_MAP:
        unwinding_result_.stop_reason = UnwindingResult::MAP_MISSING;
        break;
      default:
        unwinding_result_.stop_reason = UnwindingResult::UNKNOWN_REASON;
        break;
    }
    unwinding_result_.stack_start = stack_addr;
    unwinding_result_.stack_end = stack_addr + stack_size;
  }
  return true;
}

}  // namespace simpleperf
