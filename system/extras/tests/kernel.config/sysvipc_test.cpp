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
#include <errno.h>
#ifdef HAS_KCMP
#include <linux/kcmp.h>
#include <sys/syscall.h>
#endif
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include <gtest/gtest.h>

#ifdef HAS_KCMP
int kcmp(pid_t pid1, pid_t pid2, int type, unsigned long idx1, unsigned long idx2) {
  return syscall(SYS_kcmp, pid1, pid2, type, 0, idx1, idx2);
}
#endif

int msgctl(int id, int cmd, msqid_ds* buf) {
#if !defined(__LP64__) || defined(__mips__)
  // Annoyingly, the kernel requires this for 32-bit but rejects it for 64-bit.
  // Mips64 is an exception to this, it requires the flag.
  cmd |= IPC_64;
#endif
#if defined(SYS_msgctl)
  return syscall(SYS_msgctl, id, cmd, buf);
#else
  return syscall(SYS_ipc, MSGCTL, id, cmd, 0, buf, 0);
#endif
}

int semctl(int id, int num, int cmd, semid_ds* buf) {
#if !defined(__LP64__) || defined(__mips__)
  // Annoyingly, the kernel requires this for 32-bit but rejects it for 64-bit.
  // Mips64 is an exception to this, it requires the flag.
  cmd |= IPC_64;
#endif
#if defined(SYS_msgctl)
  return syscall(SYS_semctl, id, num, cmd, buf);
#else
  return syscall(SYS_ipc, SEMCTL, id, num, cmd, buf, 0);
#endif
}

int shmctl(int id, int cmd, shmid_ds* buf) {
#if !defined(__LP64__) || defined(__mips__)
  // Annoyingly, the kernel requires this for 32-bit but rejects it for 64-bit.
  // Mips64 is an exception to this, it requires the flag.
  cmd |= IPC_64;
#endif
#if defined(SYS_shmctl)
  return syscall(SYS_shmctl, id, cmd, buf);
#else
  return syscall(SYS_ipc, SHMCTL, id, cmd, 0, buf, 0);
#endif
}

TEST(kernel_config, NOT_CONFIG_SYSVIPC) {
#ifdef HAS_KCMP
  pid_t pid = getpid();
  int ret = kcmp(pid, pid, KCMP_SYSVSEM, 0, 0);
  int error = (ret == -1) ? (errno == ENOSYS) ? EOPNOTSUPP : errno : 0;
  EXPECT_EQ(-1, kcmp(pid, pid, KCMP_SYSVSEM, 0, 0));
  EXPECT_EQ(EOPNOTSUPP, error);
#endif

  EXPECT_EQ(-1, access("/proc/sysvipc", R_OK));

  EXPECT_EQ(-1, access("/proc/sysvipc/msg", F_OK));
  EXPECT_EQ(-1, msgctl(-1, IPC_STAT, nullptr));
  EXPECT_EQ(ENOSYS, errno);

  EXPECT_EQ(-1, access("/proc/sysvipc/sem", F_OK));
  EXPECT_EQ(-1, semctl(-1, 0, IPC_STAT, nullptr));
  EXPECT_EQ(ENOSYS, errno);

  EXPECT_EQ(-1, access("/proc/sysvipc/shm", F_OK));
  EXPECT_EQ(-1, shmctl(-1, IPC_STAT, nullptr));
  EXPECT_EQ(ENOSYS, errno);
}

