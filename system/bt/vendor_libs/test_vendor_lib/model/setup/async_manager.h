#ifndef TEST_VENDOR_LIB_ASYNC_MANAGER_H_
#define TEST_VENDOR_LIB_ASYNC_MANAGER_H_

#include <time.h>
#include <cstdint>
#include <map>
#include <set>
#include "errno.h"
#include "stdio.h"

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>

namespace test_vendor_lib {

using TaskCallback = std::function<void(void)>;
using ReadCallback = std::function<void(int)>;
using CriticalCallback = std::function<void(void)>;
using AsyncTaskId = uint16_t;
constexpr uint16_t kInvalidTaskId = 0;

// Manages tasks that should be done in the future. It can watch file
// descriptors to call a given callback when it is certain that a non-blocking
// read is possible or can call a callback at a specific time (aproximately) and
// (optionally) repeat the call periodically.
// The class is thread safe in the sense that all its member functions can be
// called simultaneously from different concurrent threads. The exception to
// this rule is the class destructor, which is unsafe to call concurrently with
// calls to other class member functions. This exception also has its own
// exception: it is safe to destroy the object even if some of its callbacks may
// call its member functions, because the destructor will make sure all callback
// calling threads are stopped before actually destroying anything. Callbacks
// that wait for file descriptor always run on the same thread, so there is no
// need of additional synchronization between them. The same applies to task
// callbacks since they also run on a thread of their own, however it is
// possible for a read callback and a task callback to execute at the same time
// (they are garanteed to run in different threads) so synchronization is needed
// to access common state (other than the internal state of the AsyncManager
// class). While not required, it is strongly recommended to use the
// Synchronize(const CriticalCallback&) member function to execute code inside
// critical sections. Callbacks passed to this method on the same AsyncManager
// object from different threads are granted to *NOT* run concurrently.
class AsyncManager {
 public:
  // Starts watching a file descriptor in a separate thread. The
  // on_read_fd_ready_callback() will be asynchronously called when it is
  // guaranteed that a call to read() on the FD will not block. No promise is
  // made about when in the future the callback will be called, in particular,
  // it is perfectly possible to have it called before this function returns. A
  // return of 0 means success, an error code is returned otherwise.
  int WatchFdForNonBlockingReads(int file_descriptor, const ReadCallback& on_read_fd_ready_callback);

  // If the fd was not being watched before the call will be ignored.
  void StopWatchingFileDescriptor(int file_descriptor);

  // Schedules an action to occur in the future. Even if the delay given is not
  // positive the callback will be called asynchronously.
  AsyncTaskId ExecAsync(std::chrono::milliseconds delay, const TaskCallback& callback);

  // Schedules an action to occur periodically in the future. If the delay given
  // is not positive the callback will be asynchronously called once for each
  // time in the past that it should have been called and then scheduled for
  // future times.
  AsyncTaskId ExecAsyncPeriodically(std::chrono::milliseconds delay, std::chrono::milliseconds period,
                                    const TaskCallback& callback);

  // Cancels the/every future ocurrence of the action specified by this id. It
  // is guaranteed that the asociated callback will not be called after this
  // method returns (it could be called during the execution of the method).
  // The calling thread may block until the scheduling thread acknowledges the
  // cancelation.
  bool CancelAsyncTask(AsyncTaskId async_task_id);

  // Execs the given code in a synchronized manner. It is guaranteed that code
  // given on (possibly)concurrent calls to this member function on the same
  // AsyncManager object will never be executed simultaneously. It is the
  // class's user's resposability to ensure that no calls to Synchronize are
  // made from inside a CriticalCallback, since that would cause a lock to be
  // acquired twice with unpredictable results. It is strongly recommended to
  // have very simple CriticalCallbacks, preferably using lambda expressions.
  void Synchronize(const CriticalCallback&);

  AsyncManager();

  ~AsyncManager();

 private:
  // Implementation of the FD watching part of AsyncManager, extracted to its
  // own class for clarity purposes.
  class AsyncFdWatcher;

  // Implementation of the asynchronous tasks part of AsyncManager, extracted to
  // its own class for clarity purposes.
  class AsyncTaskManager;

  AsyncManager(const AsyncManager&) = delete;
  AsyncManager& operator=(const AsyncManager&) = delete;

  // Kept as pointers because we may want to support reseting either without
  // destroying the other one
  std::unique_ptr<AsyncFdWatcher> fdWatcher_p_;
  std::unique_ptr<AsyncTaskManager> taskManager_p_;

  std::mutex synchronization_mutex_;
};
}  // namespace test_vendor_lib
#endif  // TEST_VENDOR_LIB_ASYNC_MANAGER_H_
