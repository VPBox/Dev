/*
 * Copyright 2014 Samsung System LSI
 * Copyright 2013 The Android Open Source Project
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

#include "btif_sock_l2cap.h"

#include <base/logging.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include <mutex>

#include <frameworks/base/core/proto/android/bluetooth/enums.pb.h>
#include <hardware/bt_sock.h>

#include "osi/include/allocator.h"

#include "bt_common.h"
#include "bt_target.h"
#include "bta_api.h"
#include "bta_jv_api.h"
#include "bta_jv_co.h"
#include "btif_common.h"
#include "btif_sock_sdp.h"
#include "btif_sock_thread.h"
#include "btif_sock_util.h"
#include "btif_uid.h"
#include "btif_util.h"
#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"
#include "common/metrics.h"
#include "hcimsgs.h"
#include "l2c_api.h"
#include "l2c_int.h"
#include "l2cdefs.h"
#include "port_api.h"
#include "sdp_api.h"

struct packet {
  struct packet *next, *prev;
  uint32_t len;
  uint8_t* data;
};

typedef struct l2cap_socket {
  struct l2cap_socket* prev;  // link to prev list item
  struct l2cap_socket* next;  // link to next list item
  RawAddress addr;            // other side's address
  char name[256];             // user-friendly name of the service
  uint32_t id;                // just a tag to find this struct
  int app_uid;                // The UID of the app who requested this socket
  int handle;                 // handle from lower layers
  unsigned security;          // security flags
  int channel;                // channel (fixed_chan) or PSM (!fixed_chan)
  int our_fd;                 // fd from our side
  int app_fd;                 // fd from app's side

  unsigned bytes_buffered;
  struct packet* first_packet;  // fist packet to be delivered to app
  struct packet* last_packet;   // last packet to be delivered to app

  unsigned fixed_chan : 1;        // fixed channel (or psm?)
  unsigned server : 1;            // is a server? (or connecting?)
  unsigned connected : 1;         // is connected?
  unsigned outgoing_congest : 1;  // should we hold?
  unsigned server_psm_sent : 1;   // The server shall only send PSM once.
  bool is_le_coc;                 // is le connection oriented channel?
  uint16_t rx_mtu;
  uint16_t tx_mtu;
  // Cumulative number of bytes transmitted on this socket
  int64_t tx_bytes;
  // Cumulative number of bytes received on this socket
  int64_t rx_bytes;
} l2cap_socket;

static void btsock_l2cap_server_listen(l2cap_socket* sock);

static std::mutex state_lock;

l2cap_socket* socks = NULL;
static uint32_t last_sock_id = 0;
static uid_set_t* uid_set = NULL;
static int pth = -1;

static void btsock_l2cap_cbk(tBTA_JV_EVT event, tBTA_JV* p_data,
                             uint32_t l2cap_socket_id);

/* TODO: Consider to remove this buffer, as we have a buffer in l2cap as well,
 * and we risk
 *       a buffer overflow with this implementation if the socket data is not
 * read from
 *       JAVA for a while. In such a case we should use flow control to tell the
 * sender to
 *       back off.
 *       BUT remember we need to avoid blocking the BTA task execution - hence
 * we cannot
 *       directly write to the socket.
 *       we should be able to change to store the data pointer here, and just
 * wait
 *       confirming the l2cap_ind until we have more space in the buffer. */

/* returns false if none - caller must free "data" memory when done with it */
static char packet_get_head_l(l2cap_socket* sock, uint8_t** data,
                              uint32_t* len) {
  struct packet* p = sock->first_packet;

  if (!p) return false;

  if (data) *data = sock->first_packet->data;
  if (len) *len = sock->first_packet->len;
  sock->first_packet = p->next;
  if (sock->first_packet)
    sock->first_packet->prev = NULL;
  else
    sock->last_packet = NULL;

  if (len) sock->bytes_buffered -= *len;

  osi_free(p);

  return true;
}

static struct packet* packet_alloc(const uint8_t* data, uint32_t len) {
  struct packet* p = (struct packet*)osi_calloc(sizeof(*p));
  uint8_t* buf = (uint8_t*)osi_malloc(len);

  p->data = buf;
  p->len = len;
  memcpy(p->data, data, len);
  return p;
}

/* makes a copy of the data, returns true on success */
static char packet_put_head_l(l2cap_socket* sock, const void* data,
                              uint32_t len) {
  struct packet* p = packet_alloc((const uint8_t*)data, len);

  /*
   * We do not check size limits here since this is used to undo "getting" a
   * packet that the user read incompletely. That is to say the packet was
   * already in the queue. We do check thos elimits in packet_put_tail_l() since
   * that function is used to put new data into the queue.
   */

  if (!p) return false;

  p->prev = NULL;
  p->next = sock->first_packet;
  sock->first_packet = p;
  if (p->next)
    p->next->prev = p;
  else
    sock->last_packet = p;

  sock->bytes_buffered += len;

  return true;
}

/* makes a copy of the data, returns true on success */
static char packet_put_tail_l(l2cap_socket* sock, const void* data,
                              uint32_t len) {
  if (sock->bytes_buffered >= L2CAP_MAX_RX_BUFFER) {
    LOG(ERROR) << __func__ << ": buffer overflow";
    return false;
  }

  struct packet* p = packet_alloc((const uint8_t*)data, len);
  p->next = NULL;
  p->prev = sock->last_packet;
  sock->last_packet = p;
  if (p->prev)
    p->prev->next = p;
  else
    sock->first_packet = p;

  sock->bytes_buffered += len;

  return true;
}

static char is_inited(void) {
  std::unique_lock<std::mutex> lock(state_lock);
  return pth != -1;
}

/* only call with std::mutex taken */
static l2cap_socket* btsock_l2cap_find_by_id_l(uint32_t id) {
  l2cap_socket* sock = socks;

  while (sock && sock->id != id) sock = sock->next;

  return sock;
}

static void btsock_l2cap_free_l(l2cap_socket* sock) {
  uint8_t* buf;
  l2cap_socket* t = socks;

  while (t && t != sock) t = t->next;

  if (!t) /* prever double-frees */
    return;

  // Whenever a socket is freed, the connection must be dropped
  bluetooth::common::LogSocketConnectionState(
      sock->addr, sock->id, sock->is_le_coc ? BTSOCK_L2CAP_LE : BTSOCK_L2CAP,
      android::bluetooth::SOCKET_CONNECTION_STATE_DISCONNECTED, sock->tx_bytes,
      sock->rx_bytes, sock->app_uid, sock->channel,
      sock->server ? android::bluetooth::SOCKET_ROLE_LISTEN
                   : android::bluetooth::SOCKET_ROLE_CONNECTION);

  if (sock->next) sock->next->prev = sock->prev;

  if (sock->prev)
    sock->prev->next = sock->next;
  else
    socks = sock->next;

  shutdown(sock->our_fd, SHUT_RDWR);
  close(sock->our_fd);
  if (sock->app_fd != -1) {
    close(sock->app_fd);
  } else {
    LOG(ERROR) << "SOCK_LIST: free(id = " << sock->id << ") - NO app_fd!";
  }

  while (packet_get_head_l(sock, &buf, NULL)) osi_free(buf);

  // lower-level close() should be idempotent... so let's call it and see...
  if (sock->is_le_coc) {
    // Only call if we are non server connections
    if (sock->handle >= 0 && (!sock->server)) {
      BTA_JvL2capClose(sock->handle);
    }
    if ((sock->channel >= 0) && (sock->server)) {
      BTA_JvFreeChannel(sock->channel, BTA_JV_CONN_TYPE_L2CAP_LE);
      if (!sock->fixed_chan) {
        VLOG(2) << __func__ << ": stopping L2CAP LE COC server channel "
                << sock->channel;
        BTA_JvL2capStopServer(sock->channel, sock->id);
      }
    }
  } else {
    // Only call if we are non server connections
    if ((sock->handle >= 0) && (!sock->server)) {
      if (sock->fixed_chan)
        BTA_JvL2capCloseLE(sock->handle);
      else
        BTA_JvL2capClose(sock->handle);
    }
    if ((sock->channel >= 0) && (sock->server)) {
      if (sock->fixed_chan)
        BTA_JvFreeChannel(sock->channel, BTA_JV_CONN_TYPE_L2CAP_LE);
      else
        BTA_JvFreeChannel(sock->channel, BTA_JV_CONN_TYPE_L2CAP);

      if (!sock->fixed_chan) {
        DVLOG(2) << __func__ << ": stopping L2CAP server channel "
                 << sock->channel;
        BTA_JvL2capStopServer(sock->channel, sock->id);
      }
    }
  }

  DVLOG(2) << __func__ << ": free id:" << sock->id;
  osi_free(sock);
}

static l2cap_socket* btsock_l2cap_alloc_l(const char* name,
                                          const RawAddress* addr,
                                          char is_server, int flags) {
  unsigned security = 0;
  int fds[2];
  l2cap_socket* sock = (l2cap_socket*)osi_calloc(sizeof(*sock));

  if (flags & BTSOCK_FLAG_ENCRYPT)
    security |= is_server ? BTM_SEC_IN_ENCRYPT : BTM_SEC_OUT_ENCRYPT;
  if (flags & BTSOCK_FLAG_AUTH)
    security |= is_server ? BTM_SEC_IN_AUTHENTICATE : BTM_SEC_OUT_AUTHENTICATE;
  if (flags & BTSOCK_FLAG_AUTH_MITM)
    security |= is_server ? BTM_SEC_IN_MITM : BTM_SEC_OUT_MITM;
  if (flags & BTSOCK_FLAG_AUTH_16_DIGIT)
    security |= BTM_SEC_IN_MIN_16_DIGIT_PIN;

  if (socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, fds)) {
    LOG(ERROR) << "socketpair failed, errno:" << errno;
    goto fail_sockpair;
  }

  sock->our_fd = fds[0];
  sock->app_fd = fds[1];
  sock->security = security;
  sock->server = is_server;
  sock->connected = false;
  sock->handle = 0;
  sock->server_psm_sent = false;
  sock->app_uid = -1;

  if (name) strncpy(sock->name, name, sizeof(sock->name) - 1);
  if (addr) sock->addr = *addr;

  sock->first_packet = NULL;
  sock->last_packet = NULL;

  sock->tx_mtu = L2CAP_LE_MIN_MTU;

  sock->next = socks;
  sock->prev = NULL;
  if (socks) socks->prev = sock;
  sock->id = last_sock_id + 1;
  sock->tx_bytes = 0;
  sock->rx_bytes = 0;
  socks = sock;
  /* paranoia cap on: verify no ID duplicates due to overflow and fix as needed
   */
  while (1) {
    l2cap_socket* t;
    t = socks->next;
    while (t && t->id != sock->id) {
      t = t->next;
    }
    if (!t && sock->id) /* non-zeor handle is unique -> we're done */
      break;
    /* if we're here, we found a duplicate */
    if (!++sock->id) /* no zero IDs allowed */
      sock->id++;
  }
  last_sock_id = sock->id;
  DVLOG(2) << __func__ << " SOCK_LIST: alloc id:" << sock->id;
  return sock;

fail_sockpair:
  osi_free(sock);
  return NULL;
}

bt_status_t btsock_l2cap_init(int handle, uid_set_t* set) {
  DVLOG(2) << __func__ << ": handle: " << handle;
  std::unique_lock<std::mutex> lock(state_lock);
  pth = handle;
  socks = NULL;
  uid_set = set;
  return BT_STATUS_SUCCESS;
}

bt_status_t btsock_l2cap_cleanup() {
  std::unique_lock<std::mutex> lock(state_lock);
  pth = -1;
  while (socks) btsock_l2cap_free_l(socks);
  return BT_STATUS_SUCCESS;
}

static inline bool send_app_psm_or_chan_l(l2cap_socket* sock) {
  DVLOG(2) << __func__ << ": channel: " << sock->channel;
  return sock_send_all(sock->our_fd, (const uint8_t*)&sock->channel,
                       sizeof(sock->channel)) == sizeof(sock->channel);
}

static bool send_app_connect_signal(int fd, const RawAddress* addr, int channel,
                                    int status, int send_fd, uint16_t rx_mtu,
                                    uint16_t tx_mtu) {
  sock_connect_signal_t cs;
  cs.size = sizeof(cs);
  cs.bd_addr = *addr;
  cs.channel = channel;
  cs.status = status;
  cs.max_rx_packet_size = rx_mtu;
  cs.max_tx_packet_size = tx_mtu;
  if (send_fd != -1) {
    if (sock_send_fd(fd, (const uint8_t*)&cs, sizeof(cs), send_fd) ==
        sizeof(cs))
      return true;
    else
      LOG(ERROR) << "sock_send_fd failed, fd: " << fd
                 << ", send_fd:" << send_fd;
  } else if (sock_send_all(fd, (const uint8_t*)&cs, sizeof(cs)) == sizeof(cs)) {
    return true;
  }
  return false;
}

static void on_srv_l2cap_listen_started(tBTA_JV_L2CAP_START* p_start,
                                        uint32_t id) {
  l2cap_socket* sock;

  std::unique_lock<std::mutex> lock(state_lock);
  sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) return;

  if (p_start->status != BTA_JV_SUCCESS) {
    LOG(ERROR) << "Error starting l2cap_listen - status: "
               << loghex(p_start->status);
    btsock_l2cap_free_l(sock);
    return;
  }

  sock->handle = p_start->handle;
  DVLOG(2) << __func__ << ": sock->handle: " << sock->handle
           << ", id: " << sock->id;

  bluetooth::common::LogSocketConnectionState(
      sock->addr, sock->id, sock->is_le_coc ? BTSOCK_L2CAP_LE : BTSOCK_L2CAP,
      android::bluetooth::SocketConnectionstateEnum::
          SOCKET_CONNECTION_STATE_LISTENING,
      0, 0, sock->app_uid, sock->channel,
      sock->server ? android::bluetooth::SOCKET_ROLE_LISTEN
                   : android::bluetooth::SOCKET_ROLE_CONNECTION);

  if (!sock->server_psm_sent) {
    if (!send_app_psm_or_chan_l(sock)) {
      // closed
      DVLOG(2) << "send_app_psm() failed, close rs->id: " << sock->id;
      btsock_l2cap_free_l(sock);
    } else {
      sock->server_psm_sent = true;
    }
  }
}

static void on_cl_l2cap_init(tBTA_JV_L2CAP_CL_INIT* p_init, uint32_t id) {
  l2cap_socket* sock;

  std::unique_lock<std::mutex> lock(state_lock);
  sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) return;

  if (p_init->status != BTA_JV_SUCCESS) {
    btsock_l2cap_free_l(sock);
    return;
  }

  sock->handle = p_init->handle;
}

/**
 * Here we allocate a new sock instance to mimic the BluetoothSocket. The socket
 * will be a clone of the sock representing the BluetoothServerSocket.
 * */
static void on_srv_l2cap_psm_connect_l(tBTA_JV_L2CAP_OPEN* p_open,
                                       l2cap_socket* sock) {
  // std::mutex locked by caller
  l2cap_socket* accept_rs =
      btsock_l2cap_alloc_l(sock->name, &p_open->rem_bda, false, 0);
  accept_rs->connected = true;
  accept_rs->security = sock->security;
  accept_rs->fixed_chan = sock->fixed_chan;
  accept_rs->channel = sock->channel;
  accept_rs->handle = sock->handle;
  accept_rs->app_uid = sock->app_uid;
  sock->handle =
      -1; /* We should no longer associate this handle with the server socket */
  accept_rs->is_le_coc = sock->is_le_coc;
  accept_rs->tx_mtu = sock->tx_mtu = p_open->tx_mtu;

  /* Swap IDs to hand over the GAP connection to the accepted socket, and start
     a new server on the newly create socket ID. */
  uint32_t new_listen_id = accept_rs->id;
  accept_rs->id = sock->id;
  sock->id = new_listen_id;

  bluetooth::common::LogSocketConnectionState(
      accept_rs->addr, accept_rs->id,
      accept_rs->is_le_coc ? BTSOCK_L2CAP_LE : BTSOCK_L2CAP,
      android::bluetooth::SOCKET_CONNECTION_STATE_CONNECTED, 0, 0,
      accept_rs->app_uid, accept_rs->channel,
      accept_rs->server ? android::bluetooth::SOCKET_ROLE_LISTEN
                        : android::bluetooth::SOCKET_ROLE_CONNECTION);

  // start monitor the socket
  btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP,
                       SOCK_THREAD_FD_EXCEPTION, sock->id);
  btsock_thread_add_fd(pth, accept_rs->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD,
                       accept_rs->id);
  DVLOG(2) << "sending connect signal & app fd: " << accept_rs->app_fd
           << " to app server to accept() the connection";
  DVLOG(2) << "server fd: << " << sock->our_fd << ", scn:" << sock->channel;
  send_app_connect_signal(sock->our_fd, &accept_rs->addr, sock->channel, 0,
                          accept_rs->app_fd, sock->rx_mtu, p_open->tx_mtu);
  accept_rs->app_fd =
      -1;  // The fd is closed after sent to app in send_app_connect_signal()
  // But for some reason we still leak a FD - either the server socket
  // one or the accept socket one.
  btsock_l2cap_server_listen(sock);
}

static void on_srv_l2cap_le_connect_l(tBTA_JV_L2CAP_LE_OPEN* p_open,
                                      l2cap_socket* sock) {
  // std::mutex locked by caller
  l2cap_socket* accept_rs =
      btsock_l2cap_alloc_l(sock->name, &p_open->rem_bda, false, 0);
  if (!accept_rs) return;

  // swap IDs
  uint32_t new_listen_id = accept_rs->id;
  accept_rs->id = sock->id;
  sock->id = new_listen_id;

  accept_rs->handle = p_open->handle;
  accept_rs->connected = true;
  accept_rs->security = sock->security;
  accept_rs->fixed_chan = sock->fixed_chan;
  accept_rs->channel = sock->channel;
  accept_rs->app_uid = sock->app_uid;
  accept_rs->tx_mtu = sock->tx_mtu = p_open->tx_mtu;

  // if we do not set a callback, this socket will be dropped */
  *(p_open->p_p_cback) = (void*)btsock_l2cap_cbk;
  *(p_open->p_user_data) = UINT_TO_PTR(accept_rs->id);

  bluetooth::common::LogSocketConnectionState(
      accept_rs->addr, accept_rs->id,
      accept_rs->is_le_coc ? BTSOCK_L2CAP_LE : BTSOCK_L2CAP,
      android::bluetooth::SOCKET_CONNECTION_STATE_CONNECTED, 0, 0,
      accept_rs->app_uid, accept_rs->channel,
      accept_rs->server ? android::bluetooth::SOCKET_ROLE_LISTEN
                        : android::bluetooth::SOCKET_ROLE_CONNECTION);

  // start monitor the socket
  btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP,
                       SOCK_THREAD_FD_EXCEPTION, sock->id);
  btsock_thread_add_fd(pth, accept_rs->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD,
                       accept_rs->id);
  DVLOG(2) << "sending connect signal & app fd: " << accept_rs->app_fd
           << " to app server to accept() the connection";
  DVLOG(2) << "server fd: << " << sock->our_fd << ", scn:" << sock->channel;
  send_app_connect_signal(sock->our_fd, &accept_rs->addr, sock->channel, 0,
                          accept_rs->app_fd, sock->rx_mtu, p_open->tx_mtu);
  accept_rs->app_fd = -1;  // the fd is closed after sent to app
}

static void on_cl_l2cap_psm_connect_l(tBTA_JV_L2CAP_OPEN* p_open,
                                      l2cap_socket* sock) {
  sock->addr = p_open->rem_bda;
  sock->tx_mtu = p_open->tx_mtu;

  if (!send_app_psm_or_chan_l(sock)) {
    LOG(ERROR) << "send_app_psm_or_chan_l failed";
    return;
  }

  if (!send_app_connect_signal(sock->our_fd, &sock->addr, sock->channel, 0, -1,
                               sock->rx_mtu, p_open->tx_mtu)) {
    LOG(ERROR) << "send_app_connect_signal failed";
    return;
  }

  bluetooth::common::LogSocketConnectionState(
      sock->addr, sock->id, sock->is_le_coc ? BTSOCK_L2CAP_LE : BTSOCK_L2CAP,
      android::bluetooth::SOCKET_CONNECTION_STATE_CONNECTED, 0, 0,
      sock->app_uid, sock->channel,
      sock->server ? android::bluetooth::SOCKET_ROLE_LISTEN
                   : android::bluetooth::SOCKET_ROLE_CONNECTION);

  // start monitoring the socketpair to get call back when app writing data
  DVLOG(2) << " connect signal sent, slot id: " << sock->id
           << ", chan: " << sock->channel << ", server: " << sock->server;
  btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD,
                       sock->id);
  sock->connected = true;
}

static void on_cl_l2cap_le_connect_l(tBTA_JV_L2CAP_LE_OPEN* p_open,
                                     l2cap_socket* sock) {
  sock->addr = p_open->rem_bda;
  sock->tx_mtu = p_open->tx_mtu;

  if (!send_app_psm_or_chan_l(sock)) {
    LOG(ERROR) << "send_app_psm_or_chan_l failed";
    return;
  }

  if (!send_app_connect_signal(sock->our_fd, &sock->addr, sock->channel, 0, -1,
                               sock->rx_mtu, p_open->tx_mtu)) {
    LOG(ERROR) << "send_app_connect_signal failed";
    return;
  }

  bluetooth::common::LogSocketConnectionState(
      sock->addr, sock->id, sock->is_le_coc ? BTSOCK_L2CAP_LE : BTSOCK_L2CAP,
      android::bluetooth::SOCKET_CONNECTION_STATE_CONNECTED, 0, 0,
      sock->app_uid, sock->channel,
      sock->server ? android::bluetooth::SOCKET_ROLE_LISTEN
                   : android::bluetooth::SOCKET_ROLE_CONNECTION);

  // start monitoring the socketpair to get call back when app writing data
  DVLOG(2) << " connect signal sent, slot id: " << sock->id
           << ", chan: " << sock->channel << ", server: " << sock->server;
  btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD,
                       sock->id);
  sock->connected = true;
}

static void on_l2cap_connect(tBTA_JV* p_data, uint32_t id) {
  l2cap_socket* sock;
  tBTA_JV_L2CAP_OPEN* psm_open = &p_data->l2c_open;
  tBTA_JV_L2CAP_LE_OPEN* le_open = &p_data->l2c_le_open;

  std::unique_lock<std::mutex> lock(state_lock);
  sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) {
    LOG(ERROR) << __func__ << ": unknown socket";
    return;
  }

  sock->tx_mtu = le_open->tx_mtu;
  if (sock->fixed_chan && le_open->status == BTA_JV_SUCCESS) {
    if (!sock->server) {
      on_cl_l2cap_le_connect_l(le_open, sock);
    } else {
      on_srv_l2cap_le_connect_l(le_open, sock);
    }
  } else if (!sock->fixed_chan && psm_open->status == BTA_JV_SUCCESS) {
    if (!sock->server) {
      on_cl_l2cap_psm_connect_l(psm_open, sock);
    } else {
      on_srv_l2cap_psm_connect_l(psm_open, sock);
    }
  } else {
    btsock_l2cap_free_l(sock);
  }
}

static void on_l2cap_close(tBTA_JV_L2CAP_CLOSE* p_close, uint32_t id) {
  l2cap_socket* sock;

  std::unique_lock<std::mutex> lock(state_lock);
  sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) return;

  bluetooth::common::LogSocketConnectionState(
      sock->addr, sock->id, sock->is_le_coc ? BTSOCK_L2CAP_LE : BTSOCK_L2CAP,
      android::bluetooth::SOCKET_CONNECTION_STATE_DISCONNECTING, 0, 0,
      sock->app_uid, sock->channel,
      sock->server ? android::bluetooth::SOCKET_ROLE_LISTEN
                   : android::bluetooth::SOCKET_ROLE_CONNECTION);

  DVLOG(2) << __func__ << ": slot id: " << sock->id << ", fd: " << sock->our_fd
           << (sock->fixed_chan ? ", fixed_chan:" : ", PSM: ") << sock->channel
           << ", server:" << sock->server;
  // TODO: This does not seem to be called...
  // I'm not sure if this will be called for non-server sockets?
  if (!sock->fixed_chan && (sock->server)) {
    BTA_JvFreeChannel(sock->channel, BTA_JV_CONN_TYPE_L2CAP);
  }
  btsock_l2cap_free_l(sock);
}

static void on_l2cap_outgoing_congest(tBTA_JV_L2CAP_CONG* p, uint32_t id) {
  l2cap_socket* sock;

  std::unique_lock<std::mutex> lock(state_lock);
  sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) return;

  sock->outgoing_congest = p->cong ? 1 : 0;
  // mointer the fd for any outgoing data
  if (!sock->outgoing_congest) {
    DVLOG(2) << __func__ << ": adding fd to btsock_thread...";
    btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD,
                         sock->id);
  }
}

static void on_l2cap_write_done(uint16_t len, uint32_t id) {
  std::unique_lock<std::mutex> lock(state_lock);
  l2cap_socket* sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) return;

  int app_uid = sock->app_uid;
  if (!sock->outgoing_congest) {
    // monitor the fd for any outgoing data
    DVLOG(2) << __func__ << ": adding fd to btsock_thread...";
    btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_RD,
                         sock->id);
  }

  sock->tx_bytes += len;
  uid_set_add_tx(uid_set, app_uid, len);
}

static void on_l2cap_data_ind(tBTA_JV* evt, uint32_t id) {
  l2cap_socket* sock;

  int app_uid = -1;
  uint32_t bytes_read = 0;

  std::unique_lock<std::mutex> lock(state_lock);
  sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) return;

  app_uid = sock->app_uid;

  if (sock->fixed_chan) { /* we do these differently */

    tBTA_JV_LE_DATA_IND* p_le_data_ind = &evt->le_data_ind;
    BT_HDR* p_buf = p_le_data_ind->p_buf;
    uint8_t* data = (uint8_t*)(p_buf + 1) + p_buf->offset;

    if (packet_put_tail_l(sock, data, p_buf->len)) {
      bytes_read = p_buf->len;
      btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_WR,
                           sock->id);
    } else {  // connection must be dropped
      DVLOG(2) << __func__
               << ": unable to push data to socket - closing  fixed channel";
      BTA_JvL2capCloseLE(sock->handle);
      btsock_l2cap_free_l(sock);
    }

  } else {
    uint32_t count;

    if (BTA_JvL2capReady(sock->handle, &count) == BTA_JV_SUCCESS) {
      std::vector<uint8_t> buffer(count);
      if (BTA_JvL2capRead(sock->handle, sock->id, buffer.data(), count) ==
          BTA_JV_SUCCESS) {
        if (packet_put_tail_l(sock, buffer.data(), count)) {
          bytes_read = count;
          btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP,
                               SOCK_THREAD_FD_WR, sock->id);
        } else {  // connection must be dropped
          DVLOG(2) << __func__
                   << ": unable to push data to socket - closing channel";
          BTA_JvL2capClose(sock->handle);
          btsock_l2cap_free_l(sock);
        }
      }
    }
  }

  sock->rx_bytes += bytes_read;
  uid_set_add_rx(uid_set, app_uid, bytes_read);
}

static void btsock_l2cap_cbk(tBTA_JV_EVT event, tBTA_JV* p_data,
                             uint32_t l2cap_socket_id) {
  switch (event) {
    case BTA_JV_L2CAP_START_EVT:
      on_srv_l2cap_listen_started(&p_data->l2c_start, l2cap_socket_id);
      break;

    case BTA_JV_L2CAP_CL_INIT_EVT:
      on_cl_l2cap_init(&p_data->l2c_cl_init, l2cap_socket_id);
      break;

    case BTA_JV_L2CAP_OPEN_EVT:
      on_l2cap_connect(p_data, l2cap_socket_id);
      BTA_JvSetPmProfile(p_data->l2c_open.handle, BTA_JV_PM_ID_1,
                         BTA_JV_CONN_OPEN);
      break;

    case BTA_JV_L2CAP_CLOSE_EVT:
      DVLOG(2) << "BTA_JV_L2CAP_CLOSE_EVT: id: " << l2cap_socket_id;
      on_l2cap_close(&p_data->l2c_close, l2cap_socket_id);
      break;

    case BTA_JV_L2CAP_DATA_IND_EVT:
      on_l2cap_data_ind(p_data, l2cap_socket_id);
      DVLOG(2) << "BTA_JV_L2CAP_DATA_IND_EVT";
      break;

    case BTA_JV_L2CAP_READ_EVT:
      DVLOG(2) << "BTA_JV_L2CAP_READ_EVT not used";
      break;

    case BTA_JV_L2CAP_WRITE_EVT:
      DVLOG(2) << "BTA_JV_L2CAP_WRITE_EVT: id: " << l2cap_socket_id;
      on_l2cap_write_done(p_data->l2c_write.len, l2cap_socket_id);
      break;

    case BTA_JV_L2CAP_WRITE_FIXED_EVT:
      DVLOG(2) << "BTA_JV_L2CAP_WRITE_FIXED_EVT: id: " << l2cap_socket_id;
      on_l2cap_write_done(p_data->l2c_write.len, l2cap_socket_id);
      break;

    case BTA_JV_L2CAP_CONG_EVT:
      on_l2cap_outgoing_congest(&p_data->l2c_cong, l2cap_socket_id);
      break;

    default:
      LOG(ERROR) << "unhandled event: " << event
                 << ", slot id: " << l2cap_socket_id;
      break;
  }
}

/* L2CAP default options for OBEX socket connections */
const tL2CAP_FCR_OPTS obex_l2c_fcr_opts_def = {
    L2CAP_FCR_ERTM_MODE,               /* Mandatory for OBEX over l2cap */
    OBX_FCR_OPT_TX_WINDOW_SIZE_BR_EDR, /* Tx window size */
    OBX_FCR_OPT_MAX_TX_B4_DISCNT,      /* Maximum transmissions before
                                          disconnecting */
    OBX_FCR_OPT_RETX_TOUT,             /* Retransmission timeout (2 secs) */
    OBX_FCR_OPT_MONITOR_TOUT,          /* Monitor timeout (12 secs) */
    OBX_FCR_OPT_MAX_PDU_SIZE           /* MPS segment size */
};
const tL2CAP_ERTM_INFO obex_l2c_etm_opt = {
    L2CAP_FCR_ERTM_MODE,     /* Mandatory for OBEX over l2cap */
    L2CAP_FCR_CHAN_OPT_ERTM, /* Mandatory for OBEX over l2cap */
    OBX_USER_RX_BUF_SIZE,    OBX_USER_TX_BUF_SIZE,
    OBX_FCR_RX_BUF_SIZE,     OBX_FCR_TX_BUF_SIZE};

/**
 * When using a dynamic PSM, a PSM allocation is requested from
 * btsock_l2cap_listen_or_connect().
 * The PSM allocation event is refeived in the JV-callback - currently located
 * in RFC-code -
 * and this function is called with the newly allocated PSM.
 */
void on_l2cap_psm_assigned(int id, int psm) {
  /* Setup ETM settings:
   *  mtu will be set below */
  std::unique_lock<std::mutex> lock(state_lock);
  l2cap_socket* sock = btsock_l2cap_find_by_id_l(id);
  if (!sock) {
    LOG(ERROR) << __func__ << ": sock is null";
    return;
  }

  sock->channel = psm;

  btsock_l2cap_server_listen(sock);
}

static void btsock_l2cap_server_listen(l2cap_socket* sock) {
  DVLOG(2) << __func__ << ": fixed_chan: " << sock->fixed_chan
           << ", channel: " << sock->channel
           << ", is_le_coc: " << sock->is_le_coc;

  if (sock->fixed_chan) {
    BTA_JvL2capStartServerLE(sock->channel, btsock_l2cap_cbk, sock->id);
    return;
  }

  int connection_type =
      sock->is_le_coc ? BTA_JV_CONN_TYPE_L2CAP_LE : BTA_JV_CONN_TYPE_L2CAP;

  /* If we have a channel specified in the request, just start the server,
   * else we request a PSM and start the server after we receive a PSM. */
  if (sock->channel <= 0) {
    BTA_JvGetChannelId(connection_type, sock->id, 0);
    return;
  }

  /* Setup ETM settings: mtu will be set below */
  std::unique_ptr<tL2CAP_CFG_INFO> cfg = std::make_unique<tL2CAP_CFG_INFO>(
      tL2CAP_CFG_INFO{.fcr_present = true, .fcr = obex_l2c_fcr_opts_def});

  std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info;
  if (!sock->is_le_coc) {
    ertm_info.reset(new tL2CAP_ERTM_INFO(obex_l2c_etm_opt));
  }

  BTA_JvL2capStartServer(connection_type, sock->security, 0,
                         std::move(ertm_info), sock->channel, sock->rx_mtu,
                         std::move(cfg), btsock_l2cap_cbk, sock->id);
}

static bt_status_t btsock_l2cap_listen_or_connect(const char* name,
                                                  const RawAddress* addr,
                                                  int channel, int* sock_fd,
                                                  int flags, char listen,
                                                  int app_uid) {
  int fixed_chan = 1;
  bool is_le_coc = false;

  if (!sock_fd) return BT_STATUS_PARM_INVALID;

  if (channel < 0) {
    // We need to auto assign a PSM
    fixed_chan = 0;
  } else {
    is_le_coc = (flags & BTSOCK_FLAG_LE_COC) != 0;
    fixed_chan = (channel & L2CAP_MASK_FIXED_CHANNEL) != 0;
    channel &= ~L2CAP_MASK_FIXED_CHANNEL;
  }

  if (!is_inited()) return BT_STATUS_NOT_READY;

  // TODO: This is kind of bad to lock here, but it is needed for the current
  // design.
  std::unique_lock<std::mutex> lock(state_lock);
  l2cap_socket* sock = btsock_l2cap_alloc_l(name, addr, listen, flags);
  if (!sock) {
    return BT_STATUS_NOMEM;
  }

  sock->fixed_chan = fixed_chan;
  sock->channel = channel;
  sock->app_uid = app_uid;
  sock->is_le_coc = is_le_coc;
  sock->rx_mtu = is_le_coc ? L2CAP_SDU_LENGTH_LE_MAX : L2CAP_SDU_LENGTH_MAX;

  /* "role" is never initialized in rfcomm code */
  if (listen) {
    btsock_l2cap_server_listen(sock);
  } else {
    if (fixed_chan) {
      BTA_JvL2capConnectLE(channel, sock->addr, btsock_l2cap_cbk, sock->id);
    } else {
      int connection_type =
          sock->is_le_coc ? BTA_JV_CONN_TYPE_L2CAP_LE : BTA_JV_CONN_TYPE_L2CAP;

      /* Setup ETM settings: mtu will be set below */
      std::unique_ptr<tL2CAP_CFG_INFO> cfg = std::make_unique<tL2CAP_CFG_INFO>(
          tL2CAP_CFG_INFO{.fcr_present = true, .fcr = obex_l2c_fcr_opts_def});

      std::unique_ptr<tL2CAP_ERTM_INFO> ertm_info;
      if (!sock->is_le_coc) {
        ertm_info.reset(new tL2CAP_ERTM_INFO(obex_l2c_etm_opt));
      }

      BTA_JvL2capConnect(
          connection_type, sock->security, 0, std::move(ertm_info), channel,
          sock->rx_mtu, std::move(cfg), sock->addr, btsock_l2cap_cbk, sock->id);
    }
  }

  *sock_fd = sock->app_fd;
  /* We pass the FD to JAVA, but since it runs in another process, we need to
   * also close it in native, either straight away, as done when accepting an
   * incoming connection, or when doing cleanup after this socket */
  sock->app_fd = -1;
  /*This leaks the file descriptor. The FD should be closed in JAVA but it
   * apparently do not work */
  btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP,
                       SOCK_THREAD_FD_EXCEPTION, sock->id);

  return BT_STATUS_SUCCESS;
}

bt_status_t btsock_l2cap_listen(const char* name, int channel, int* sock_fd,
                                int flags, int app_uid) {
  return btsock_l2cap_listen_or_connect(name, NULL, channel, sock_fd, flags, 1,
                                        app_uid);
}

bt_status_t btsock_l2cap_connect(const RawAddress* bd_addr, int channel,
                                 int* sock_fd, int flags, int app_uid) {
  return btsock_l2cap_listen_or_connect(NULL, bd_addr, channel, sock_fd, flags,
                                        0, app_uid);
}

/* return true if we have more to send and should wait for user readiness, false
 * else
 * (for example: unrecoverable error or no data)
 */
static bool flush_incoming_que_on_wr_signal_l(l2cap_socket* sock) {
  uint8_t* buf;
  uint32_t len;

  while (packet_get_head_l(sock, &buf, &len)) {
    ssize_t sent;
    OSI_NO_INTR(sent = send(sock->our_fd, buf, len, MSG_DONTWAIT));
    int saved_errno = errno;

    if (sent == (signed)len)
      osi_free(buf);
    else if (sent >= 0) {
      packet_put_head_l(sock, buf + sent, len - sent);
      osi_free(buf);
      if (!sent) /* special case if other end not keeping up */
        return true;
    } else {
      packet_put_head_l(sock, buf, len);
      osi_free(buf);
      return saved_errno == EWOULDBLOCK || saved_errno == EAGAIN;
    }
  }

  return false;
}

inline BT_HDR* malloc_l2cap_buf(uint16_t len) {
  // We need FCS only for L2CAP_FCR_ERTM_MODE, but it's just 2 bytes so it's ok
  BT_HDR* msg = (BT_HDR*)osi_malloc(BT_HDR_SIZE + L2CAP_MIN_OFFSET + len +
                                    L2CAP_FCS_LENGTH);
  msg->offset = L2CAP_MIN_OFFSET;
  msg->len = len;
  return msg;
}

inline uint8_t* get_l2cap_sdu_start_ptr(BT_HDR* msg) {
  return (uint8_t*)(msg) + BT_HDR_SIZE + msg->offset;
}

void btsock_l2cap_signaled(int fd, int flags, uint32_t user_id) {
  char drop_it = false;

  /* We use MSG_DONTWAIT when sending data to JAVA, hence it can be accepted to
   * hold the lock. */
  std::unique_lock<std::mutex> lock(state_lock);
  l2cap_socket* sock = btsock_l2cap_find_by_id_l(user_id);
  if (!sock) return;

  if ((flags & SOCK_THREAD_FD_RD) && !sock->server) {
    // app sending data
    if (sock->connected) {
      int size = 0;
      bool ioctl_success = ioctl(sock->our_fd, FIONREAD, &size) == 0;
      if (!(flags & SOCK_THREAD_FD_EXCEPTION) || (ioctl_success && size)) {
        /* FIONREAD return number of bytes that are immediately available for
           reading, might be bigger than awaiting packet.

           BluetoothSocket.write(...) guarantees that any packet send to this
           socket is broken into pieces no bigger than MTU bytes (as requested
           by BT spec). */
        size = std::min(size, (int)sock->tx_mtu);

        BT_HDR* buffer = malloc_l2cap_buf(size);
        /* The socket is created with SOCK_SEQPACKET, hence we read one message
         * at the time. */
        ssize_t count;
        OSI_NO_INTR(count = recv(fd, get_l2cap_sdu_start_ptr(buffer), size,
                                 MSG_NOSIGNAL | MSG_DONTWAIT | MSG_TRUNC));
        if (count > sock->tx_mtu) {
          /* This can't happen thanks to check in BluetoothSocket.java but leave
           * this in case this socket is ever used anywhere else*/
          LOG(ERROR) << "recv more than MTU. Data will be lost: " << count;
          count = sock->tx_mtu;
        }

        /* When multiple packets smaller than MTU are flushed to the socket, the
           size of the single packet read could be smaller than the ioctl
           reported total size of awaiting packets. Hence, we adjust the buffer
           length. */
        buffer->len = count;
        DVLOG(2) << __func__ << ": bytes received from socket: " << count;

        if (sock->fixed_chan) {
          // will take care of freeing buffer
          BTA_JvL2capWriteFixed(sock->channel, sock->addr, PTR_TO_UINT(buffer),
                                btsock_l2cap_cbk, buffer, user_id);
        } else {
          // will take care of freeing buffer
          BTA_JvL2capWrite(sock->handle, PTR_TO_UINT(buffer), buffer, user_id);
        }
      }
    } else
      drop_it = true;
  }
  if (flags & SOCK_THREAD_FD_WR) {
    // app is ready to receive more data, tell stack to enable the data flow
    if (flush_incoming_que_on_wr_signal_l(sock) && sock->connected)
      btsock_thread_add_fd(pth, sock->our_fd, BTSOCK_L2CAP, SOCK_THREAD_FD_WR,
                           sock->id);
  }
  if (drop_it || (flags & SOCK_THREAD_FD_EXCEPTION)) {
    int size = 0;
    if (drop_it || ioctl(sock->our_fd, FIONREAD, &size) != 0 || size == 0)
      btsock_l2cap_free_l(sock);
  }
}
