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

#ifndef CHRE_HOST_SOCKET_CLIENT_H_
#define CHRE_HOST_SOCKET_CLIENT_H_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <cutils/sockets.h>
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>

namespace android {
namespace chre {

class SocketClient {
 public:
  SocketClient();
  ~SocketClient();

  /**
   * Represents the callback interface used for handling events that occur on
   * the receive thread. Note that it is *not* safe to call connect(),
   * connectInBackground(), or disconnect() from the context of these callbacks.
   */
  class ICallbacks : public VirtualLightRefBase {
   public:
    /**
     * Invoked from within the context of the read thread when a message is
     * received on the socket.
     *
     * @param data Buffer containing received message data
     * @param length Size of the message in bytes
     */
    virtual void onMessageReceived(const void *data, size_t length) = 0;

    /**
     * Called when the socket is successfully (re-)connected.
     */
    virtual void onConnected() {};

    /**
     * Called when we have failed to (re-)connect the socket after many attempts
     * and are giving up.
     */
    virtual void onConnectionAborted() {};

    /**
     * Invoked when the socket is disconnected, and this connection loss was not
     * the result of an explicit call to disconnect(), i.e. the connection was
     * terminated on the remote end.
     */
    virtual void onDisconnected() {};
  };

  /**
   * Synchronously attempts to connect to the Android reserved namespace socket
   * with the given name. If this connection attempt is successful, starts a
   * receive thread to handle messages received on the socket, and uses this
   * thread to automatically reconnect if disconnected by the remote end.
   *
   * @param socketName Name of the Android domain socket to connect to
   * @param callbacks
   *
   * @return true if the connection was successful
   */
  bool connect(const char *socketName,
               const ::android::sp<ICallbacks>& callbacks);

  /**
   * Starts up the receive thread and attempts to connect to the socket in the
   * background. The onConnected() callback will be invoked when the socket is
   * connected successfully, or onConnectionAborted() will be invoked if the
   * connection could not be made after many retries and the client is giving
   * up.
   *
   * @param socketName Name of the Android domain socket to connect to
   * @param callbacks
   *
   * @return true if the receive thread was started and will attempt to connect
   *         the socket asynchronously
   */
  bool connectInBackground(const char *socketName,
                           const ::android::sp<ICallbacks>& callbacks);

  /**
   * Performs graceful teardown of the socket. After this function returns, this
   * object will no longer invoke any callbacks or hold a reference to the
   * callbacks object provided to connect().
   */
  void disconnect();

  /**
   * @return true if the socket is currently connected
   */
  bool isConnected() const;

  /**
   * Send a message on the connected socket. Safe to call from any thread.
   *
   * @param data Buffer containing message data
   * @param length Size of the message to send in bytes
   *
   * @return true if the message was successfully sent
   */
  bool sendMessage(const void *data, size_t length);

 private:
  static constexpr size_t kMaxSocketNameLen = 64;
  char mSocketName[kMaxSocketNameLen];
  sp<ICallbacks> mCallbacks;

  std::atomic<int> mSockFd;
  std::thread mRxThread;

  //! Set to true when we initiate the graceful socket shutdown procedure, so we
  //! know not to invoke onSocketDisconnectedByRemote()
  std::atomic<bool> mGracefulShutdown;

  //! Condition variable used as the method to wake the RX thread when we want
  //! to disconnect, but it's trying to reconnect automatically
  std::condition_variable mShutdownCond;
  std::mutex mShutdownMutex;

  bool doConnect(const char *socketName,
                 const ::android::sp<ICallbacks>& callbacks,
                 bool connectInBackground);
  bool inReceiveThread() const;
  void receiveThread();
  bool receiveThreadRunning() const;
  bool reconnect();
  void startReceiveThread();
  bool tryConnect(bool suppressErrorLogs = false);
};

}  // namespace chre
}  // namespace android

#endif  // CHRE_HOST_SOCKET_CLIENT_H_
