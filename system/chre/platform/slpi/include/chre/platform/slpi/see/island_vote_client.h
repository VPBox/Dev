/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SLPI_SEE_ISLAND_VOTE_CLIENT_H_
#define CHRE_PLATFORM_SLPI_SEE_ISLAND_VOTE_CLIENT_H_

#include "chre/platform/mutex.h"
#include "chre/util/singleton.h"
#include "chre/util/time.h"

extern "C" {

#include "sns_island_util.h"

} // extern "C"

namespace chre {

class IslandVoteClient : public NonCopyable {
 public:
  /**
   * Constructor for the IslandVoteClient object.
   *
   * @param clientName the client name to use when creating an island client
   */
  IslandVoteClient(const char *clientName);

  ~IslandVoteClient();

  /**
   * Makes a power mode request. An actual vote to the SLPI power manager may
   * not be cast depending on current power mode and mBigImageRefCount.
   *
   * @param bigImage Whether to request bigImage or not.
   *
   * @return true if the vote returned success.
   */
  bool voteBigImage(bool bigImage);

#ifdef CHRE_SLPI_UIMG_ENABLED
  /**
   * Increment the big image reference count when client needs to perform some
   * big image activity and holds the system in big image. A big image vote is
   * cast when the count increments from 0.
   */
  void incrementBigImageRefCount();

  /**
   * Decrement the big image reference count when client finishes some activity
   * that has to be performed in big image. A big image vote may be cast or
   * removed when the count decrements to 0, depending on the system's intended
   * power state.
   */
  void decrementBigImageRefCount();

 private:
  //! The maximum allowed duration to be voted into big image by
  //! incrementBigImageRefCount before a FATAL_ERROR is triggered.
  static constexpr Seconds kSeeMaxBigImageDuration = Seconds(300);

  //! Last big image request made through voteBigImage().
  bool mLastBigImageRequest = false;

  //! Last big image vote cast to sns_island_aggregator.
  bool mLastBigImageVote = false;

  //! Client handle for the island aggregator registration.
  sns_island_client_handle mClientHandle = nullptr;

  //! The system time mBigImageRefCount increments from 0.
  Milliseconds mRefCountStart = Milliseconds(0);

  //! The count of big image activities.
  uint32_t mBigImageRefCount = 0;

  //! Used to protect access to member variables from other threads.
  Mutex mMutex;

  /**
   * Cast a vote to sns_island_aggregator.
   *
   * @param bigImage Whether to vote for bigImage or not.
   *
   * @return true if the vote returned success.
   */
  bool voteSnsPowerMode(bool bigImage);

  /**
   * Check how long the system has been voted into big image due to
   * incrementBigImageRefCount. If longer than kSeeMaxBigImageDuration, trigger
   * a crash.
   *
   * @return the duration in milliseconds since the system has been voted into
   *         big image due to incrementBigImageRefCount.
   */
  uint64_t checkBigImageDuration() const;
#endif  // CHRE_SLPI_UIMG_ENABLED
};

//! Provides an alias to the IslandVoteClient singleton
typedef Singleton<IslandVoteClient> IslandVoteClientSingleton;

extern template class Singleton<IslandVoteClient>;

} // namespace chre

#endif // CHRE_PLATFORM_SLPI_SEE_ISLAND_VOTE_CLIENT_H_
