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
#include <fcntl.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include <android-base/unique_fd.h>

static int hwtime(int flag, int request, struct rtc_time *tm) {
  static const char rtc[] = "/dev/rtc0";

  int ret = access(rtc, flag & O_WRONLY);
  if (ret < 0) {
    return -errno;
  }

  if (flag & O_WRONLY) {
    struct stat st;
    ret = TEMP_FAILURE_RETRY(stat(rtc, &st));
    if (ret < 0) {
      return -errno;
    } else if (!(st.st_mode & (S_IWUSR|S_IWGRP|S_IWOTH))) {
      return -EACCES;
    }
  }

  for (int count = 0; count < 10; count++) {
    ret = TEMP_FAILURE_RETRY(open(rtc, flag));
    if (ret < 0) {
      if (errno == EBUSY) {
        sleep(1);
        continue;
      }
      return -errno;
    }
    break;
  }
  android::base::unique_fd fd(ret);

  for (int count = 0; count < 10; count++) {
    ret = TEMP_FAILURE_RETRY(ioctl(fd.get(), request, tm));
    if (ret < 0) {
      if (errno == EBUSY) {
        sleep(1);
        continue;
      }
      return -errno;
    }
    return ret;
  };
  return -EBUSY;
}

static int rd_hwtime(struct rtc_time *tm) {
  return hwtime(O_RDONLY, RTC_RD_TIME, tm);
}

static int set_hwtime(struct rtc_time *tm) {
  return hwtime(O_WRONLY, RTC_SET_TIME, tm);
}

static void rtc_rollover(int start, int end) {
  struct rtc_time roll;
  memset(&roll, 0, sizeof(roll));
  ASSERT_LE(0, rd_hwtime(&roll));
  int mday[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  mday[1] = (roll.tm_year % 4) ? 28 : 29;
  ASSERT_LE(0, roll.tm_sec);
  ASSERT_GT(60, roll.tm_sec);
  ASSERT_LE(0, roll.tm_min);
  ASSERT_GT(60, roll.tm_min);
  ASSERT_LE(0, roll.tm_hour);
  ASSERT_GT(24, roll.tm_hour);
  ASSERT_LE(0, roll.tm_mday);
  ASSERT_GE(mday[roll.tm_mon], roll.tm_mday);
  ASSERT_LE(0, roll.tm_mon);
  ASSERT_GT(12, roll.tm_mon);
  ASSERT_LE(0, roll.tm_year);
  ASSERT_GT(138, roll.tm_year);

  // Wait for granular clock
  struct rtc_time save = roll;
  static const useconds_t timeout_sleep = 10000;
  static const int timeout_num = 2000000 / timeout_sleep;
  int timeout;
  for (timeout = timeout_num; timeout && (roll.tm_year == save.tm_year); --timeout) {
    ASSERT_LE(0, rd_hwtime(&save));
    usleep(timeout_sleep);
  }

  memset(&roll, 0, sizeof(roll));
  roll.tm_sec = 59;
  roll.tm_min = 59;
  roll.tm_hour = 23;
  roll.tm_mday = 31;
  roll.tm_mon = 11;
  roll.tm_year = 70;
  roll.tm_wday = 0;
  roll.tm_yday = 0;
  roll.tm_isdst = 0;

  bool eacces = true;
  for (roll.tm_year = start; roll.tm_year < end; ++roll.tm_year) {
    struct rtc_time tm = roll;
    int __set_hwtime = set_hwtime(&tm);
    // Allowed to be 100% denied for writing
    if ((__set_hwtime == -EACCES) && (eacces == true)) {
      continue;
    }
    eacces = false;
    // below 2016, permitted to error out.
    if ((__set_hwtime == -EINVAL) && (roll.tm_year < 116)) {
      continue;
    }
    ASSERT_LE(0, __set_hwtime);
    ASSERT_LE(0, rd_hwtime(&tm));
    ASSERT_EQ(roll.tm_sec, tm.tm_sec);
    ASSERT_EQ(roll.tm_min, tm.tm_min);
    ASSERT_EQ(roll.tm_hour, tm.tm_hour);
    ASSERT_EQ(roll.tm_mday, tm.tm_mday);
    ASSERT_EQ(roll.tm_mon, tm.tm_mon);
    ASSERT_EQ(roll.tm_year, tm.tm_year);
    for (timeout = timeout_num; timeout && (roll.tm_year == tm.tm_year); --timeout) {
      ASSERT_LE(0, rd_hwtime(&tm));
      usleep(timeout_sleep);
    }
    ASSERT_EQ(roll.tm_year + 1, tm.tm_year);
    EXPECT_LT(timeout_num * 5 / 100, timeout);
    EXPECT_GT(timeout_num * 95 / 100, timeout);

    // correct saved time to compensate for rollover check
    if (++save.tm_sec >= 60) {
      save.tm_sec = 0;
      if (++save.tm_min >= 60) {
        save.tm_min = 0;
        if (++save.tm_hour >= 24) {
          save.tm_hour = 0;
          mday[1] = (save.tm_year % 4) ? 28 : 29;
          if (++save.tm_mday >= mday[save.tm_mon]) {
            save.tm_mday = 1;
            if (++save.tm_mon >= 12) {
              save.tm_mon = 0;
              ++save.tm_year;
            }
          }
        }
      }
    }
  }

  if (!eacces) {
    ASSERT_LE(0, set_hwtime(&save));
  }
  ASSERT_LE(0, rd_hwtime(&roll));

  if (!eacces) {
    ASSERT_EQ(save.tm_sec, roll.tm_sec);
    ASSERT_EQ(save.tm_min, roll.tm_min);
    ASSERT_EQ(save.tm_hour, roll.tm_hour);
    ASSERT_EQ(save.tm_mday, roll.tm_mday);
    ASSERT_EQ(save.tm_mon, roll.tm_mon);
    ASSERT_EQ(save.tm_year, roll.tm_year);
  }
}

TEST(time, rtc_rollover_1970_1990) {
  rtc_rollover(70, 90);
}

TEST(time, rtc_rollover_1990_2010) {
  rtc_rollover(90, 110);
}

TEST(time, rtc_rollover_2010_2030) {
  rtc_rollover(110, 130);
}

TEST(time, rtc_rollover_2030_2037) {
  rtc_rollover(130, 137);
}
