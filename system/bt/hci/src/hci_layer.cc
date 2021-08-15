/******************************************************************************
 *
 *  Copyright 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#define LOG_TAG "bt_hci"

#include "hci_layer.h"

#include <base/bind.h>
#include <base/logging.h>
#include <base/run_loop.h>
#include <base/sequenced_task_runner.h>
#include <base/threading/thread.h>
#include <frameworks/base/core/proto/android/bluetooth/hci/enums.pb.h>

#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <mutex>

#include "btcore/include/module.h"
#include "btif/include/btif_bqr.h"
#include "btsnoop.h"
#include "buffer_allocator.h"
#include "common/message_loop_thread.h"
#include "common/metrics.h"
#include "common/once_timer.h"
#include "hci_inject.h"
#include "hci_internals.h"
#include "hcidefs.h"
#include "hcimsgs.h"
#include "osi/include/alarm.h"
#include "osi/include/list.h"
#include "osi/include/log.h"
#include "osi/include/properties.h"
#include "osi/include/reactor.h"
#include "packet_fragmenter.h"

#define BT_HCI_TIMEOUT_TAG_NUM 1010000

using bluetooth::common::MessageLoopThread;
using bluetooth::common::OnceTimer;

extern void hci_initialize();
extern void hci_transmit(BT_HDR* packet);
extern void hci_close();
extern int hci_open_firmware_log_file();
extern void hci_close_firmware_log_file(int fd);
extern void hci_log_firmware_debug_packet(int fd, BT_HDR* packet);

static int hci_firmware_log_fd = INVALID_FD;

typedef struct {
  uint16_t opcode;
  future_t* complete_future;
  command_complete_cb complete_callback;
  command_status_cb status_callback;
  void* context;
  BT_HDR* command;
  std::chrono::time_point<std::chrono::steady_clock> timestamp;
} waiting_command_t;

// Using a define here, because it can be stringified for the property lookup
// Default timeout should be less than BLE_START_TIMEOUT and
// having less than 3 sec would hold the wakelock for init
#define DEFAULT_STARTUP_TIMEOUT_MS 2900
#define STRING_VALUE_OF(x) #x

// Abort if there is no response to an HCI command.
static const uint32_t COMMAND_PENDING_TIMEOUT_MS = 2000;
static const uint32_t COMMAND_PENDING_MUTEX_ACQUIRE_TIMEOUT_MS = 500;
static const uint32_t COMMAND_TIMEOUT_RESTART_MS = 5000;
static const int HCI_UNKNOWN_COMMAND_TIMED_OUT = 0x00ffffff;
static const int HCI_STARTUP_TIMED_OUT = 0x00eeeeee;

// Our interface
static bool interface_created;
static hci_t interface;

// Modules we import and callbacks we export
static const allocator_t* buffer_allocator;
static const btsnoop_t* btsnoop;
static const packet_fragmenter_t* packet_fragmenter;

static future_t* startup_future;
static MessageLoopThread hci_thread("bt_hci_thread");

static alarm_t* startup_timer;

// Outbound-related
static int command_credits = 1;
static std::mutex command_credits_mutex;
static std::queue<base::Closure> command_queue;

// Inbound-related
static alarm_t* command_response_timer;
static list_t* commands_pending_response;
static std::recursive_timed_mutex commands_pending_response_mutex;
static OnceTimer abort_timer;

// The hand-off point for data going to a higher layer, set by the higher layer
static base::Callback<void(const base::Location&, BT_HDR*)> send_data_upwards;

static bool filter_incoming_event(BT_HDR* packet);
static waiting_command_t* get_waiting_command(command_opcode_t opcode);
static int get_num_waiting_commands();

static void event_finish_startup(void* context);
static void startup_timer_expired(void* context);

static void enqueue_command(waiting_command_t* wait_entry);
static void event_command_ready(waiting_command_t* wait_entry);
static void enqueue_packet(void* packet);
static void event_packet_ready(void* packet);
static void command_timed_out(void* context);

static void update_command_response_timer(void);

static void transmit_fragment(BT_HDR* packet, bool send_transmit_finished);
static void dispatch_reassembled(BT_HDR* packet);
static void fragmenter_transmit_finished(BT_HDR* packet,
                                         bool all_fragments_sent);

static const packet_fragmenter_callbacks_t packet_fragmenter_callbacks = {
    transmit_fragment, dispatch_reassembled, fragmenter_transmit_finished};

void initialization_complete() {
  hci_thread.DoInThread(FROM_HERE, base::Bind(&event_finish_startup, nullptr));
}

void hci_event_received(const base::Location& from_here, BT_HDR* packet) {
  btsnoop->capture(packet, true);

  if (!filter_incoming_event(packet)) {
    send_data_upwards.Run(from_here, packet);
  }
}

void acl_event_received(BT_HDR* packet) {
  btsnoop->capture(packet, true);
  packet_fragmenter->reassemble_and_dispatch(packet);
}

void sco_data_received(BT_HDR* packet) {
  btsnoop->capture(packet, true);
  packet_fragmenter->reassemble_and_dispatch(packet);
}

void hal_service_died() {
  if (abort_timer.IsScheduled()) {
    LOG(ERROR) << "abort_timer is scheduled, wait for timeout";
    return;
  }
  abort();
}

// Module lifecycle functions

static future_t* hci_module_shut_down();

static future_t* hci_module_start_up(void) {
  LOG_INFO(LOG_TAG, "%s", __func__);

  // The host is only allowed to send at most one command initially,
  // as per the Bluetooth spec, Volume 2, Part E, 4.4 (Command Flow Control)
  // This value can change when you get a command complete or command status
  // event.
  command_credits = 1;

  // For now, always use the default timeout on non-Android builds.
  uint64_t startup_timeout_ms = DEFAULT_STARTUP_TIMEOUT_MS;

  // Grab the override startup timeout ms, if present.
  char timeout_prop[PROPERTY_VALUE_MAX];
  if (!osi_property_get("bluetooth.enable_timeout_ms", timeout_prop,
                        STRING_VALUE_OF(DEFAULT_STARTUP_TIMEOUT_MS)) ||
      (startup_timeout_ms = atoi(timeout_prop)) < 100)
    startup_timeout_ms = DEFAULT_STARTUP_TIMEOUT_MS;

  startup_timer = alarm_new("hci.startup_timer");
  if (!startup_timer) {
    LOG_ERROR(LOG_TAG, "%s unable to create startup timer.", __func__);
    goto error;
  }

  command_response_timer = alarm_new("hci.command_response_timer");
  if (!command_response_timer) {
    LOG_ERROR(LOG_TAG, "%s unable to create command response timer.", __func__);
    goto error;
  }

  hci_thread.StartUp();
  if (!hci_thread.IsRunning()) {
    LOG_ERROR(LOG_TAG, "%s unable to start thread.", __func__);
    goto error;
  }
  if (!hci_thread.EnableRealTimeScheduling()) {
    LOG_ERROR(LOG_TAG, "%s unable to make thread RT.", __func__);
    goto error;
  }

  commands_pending_response = list_new(NULL);
  if (!commands_pending_response) {
    LOG_ERROR(LOG_TAG,
              "%s unable to create list for commands pending response.",
              __func__);
    goto error;
  }

  // Make sure we run in a bounded amount of time
  future_t* local_startup_future;
  local_startup_future = future_new();
  startup_future = local_startup_future;
  alarm_set(startup_timer, startup_timeout_ms, startup_timer_expired, NULL);

  packet_fragmenter->init(&packet_fragmenter_callbacks);

  hci_thread.DoInThread(FROM_HERE, base::Bind(&hci_initialize));

  LOG_DEBUG(LOG_TAG, "%s starting async portion", __func__);
  return local_startup_future;

error:
  hci_module_shut_down();  // returns NULL so no need to wait for it
  return future_new_immediate(FUTURE_FAIL);
}

static future_t* hci_module_shut_down() {
  LOG_INFO(LOG_TAG, "%s", __func__);

  // Free the timers
  {
    std::lock_guard<std::recursive_timed_mutex> lock(
        commands_pending_response_mutex);
    alarm_free(command_response_timer);
    command_response_timer = NULL;
    alarm_free(startup_timer);
    startup_timer = NULL;
  }

  hci_thread.ShutDown();

  // Close HCI to prevent callbacks.
  hci_close();

  {
    std::lock_guard<std::recursive_timed_mutex> lock(
        commands_pending_response_mutex);
    list_free(commands_pending_response);
    commands_pending_response = NULL;
  }

  packet_fragmenter->cleanup();

  if (hci_firmware_log_fd != INVALID_FD) {
    hci_close_firmware_log_file(hci_firmware_log_fd);
    hci_firmware_log_fd = INVALID_FD;
  }

  return NULL;
}

EXPORT_SYMBOL extern const module_t hci_module = {
    .name = HCI_MODULE,
    .init = NULL,
    .start_up = hci_module_start_up,
    .shut_down = hci_module_shut_down,
    .clean_up = NULL,
    .dependencies = {BTSNOOP_MODULE, NULL}};

// Interface functions

static void set_data_cb(
    base::Callback<void(const base::Location&, BT_HDR*)> send_data_cb) {
  send_data_upwards = std::move(send_data_cb);
}

static void transmit_command(BT_HDR* command,
                             command_complete_cb complete_callback,
                             command_status_cb status_callback, void* context) {
  waiting_command_t* wait_entry = reinterpret_cast<waiting_command_t*>(
      osi_calloc(sizeof(waiting_command_t)));

  uint8_t* stream = command->data + command->offset;
  STREAM_TO_UINT16(wait_entry->opcode, stream);
  wait_entry->complete_callback = complete_callback;
  wait_entry->status_callback = status_callback;
  wait_entry->command = command;
  wait_entry->context = context;

  // Store the command message type in the event field
  // in case the upper layer didn't already
  command->event = MSG_STACK_TO_HC_HCI_CMD;

  enqueue_command(wait_entry);
}

static future_t* transmit_command_futured(BT_HDR* command) {
  waiting_command_t* wait_entry = reinterpret_cast<waiting_command_t*>(
      osi_calloc(sizeof(waiting_command_t)));
  future_t* future = future_new();

  uint8_t* stream = command->data + command->offset;
  STREAM_TO_UINT16(wait_entry->opcode, stream);
  wait_entry->complete_future = future;
  wait_entry->command = command;

  // Store the command message type in the event field
  // in case the upper layer didn't already
  command->event = MSG_STACK_TO_HC_HCI_CMD;

  enqueue_command(wait_entry);
  return future;
}

static void transmit_downward(uint16_t type, void* data) {
  if (type == MSG_STACK_TO_HC_HCI_CMD) {
    // TODO(zachoverflow): eliminate this call
    transmit_command((BT_HDR*)data, NULL, NULL, NULL);
    LOG_WARN(LOG_TAG,
             "%s legacy transmit of command. Use transmit_command instead.",
             __func__);
  } else {
    enqueue_packet(data);
  }
}

// Start up functions

static void event_finish_startup(UNUSED_ATTR void* context) {
  LOG_INFO(LOG_TAG, "%s", __func__);
  std::lock_guard<std::recursive_timed_mutex> lock(
      commands_pending_response_mutex);
  alarm_cancel(startup_timer);
  if (!startup_future) {
    return;
  }
  future_ready(startup_future, FUTURE_SUCCESS);
  startup_future = NULL;
}

static void startup_timer_expired(UNUSED_ATTR void* context) {
  LOG_ERROR(LOG_TAG, "%s", __func__);

  LOG_EVENT_INT(BT_HCI_TIMEOUT_TAG_NUM, HCI_STARTUP_TIMED_OUT);
  abort();
}

// Command/packet transmitting functions
static void enqueue_command(waiting_command_t* wait_entry) {
  base::Closure callback = base::Bind(&event_command_ready, wait_entry);

  std::lock_guard<std::mutex> command_credits_lock(command_credits_mutex);
  if (command_credits > 0) {
    if (!hci_thread.DoInThread(FROM_HERE, std::move(callback))) {
      // HCI Layer was shut down or not running
      buffer_allocator->free(wait_entry->command);
      osi_free(wait_entry);
      return;
    }
    command_credits--;
  } else {
    command_queue.push(std::move(callback));
  }
}

static void event_command_ready(waiting_command_t* wait_entry) {
  {
    /// Move it to the list of commands awaiting response
    std::lock_guard<std::recursive_timed_mutex> lock(
        commands_pending_response_mutex);
    wait_entry->timestamp = std::chrono::steady_clock::now();
    list_append(commands_pending_response, wait_entry);
  }
  // Send it off
  packet_fragmenter->fragment_and_dispatch(wait_entry->command);

  update_command_response_timer();
}

static void enqueue_packet(void* packet) {
  if (!hci_thread.DoInThread(FROM_HERE,
                             base::Bind(&event_packet_ready, packet))) {
    // HCI Layer was shut down or not running
    buffer_allocator->free(packet);
    return;
  }
}

static void event_packet_ready(void* pkt) {
  // The queue may be the command queue or the packet queue, we don't care
  BT_HDR* packet = (BT_HDR*)pkt;
  packet_fragmenter->fragment_and_dispatch(packet);
}

// Callback for the fragmenter to send a fragment
static void transmit_fragment(BT_HDR* packet, bool send_transmit_finished) {
  btsnoop->capture(packet, false);

  // HCI command packets are freed on a different thread when the matching
  // event is received. Check packet->event before sending to avoid a race.
  bool free_after_transmit =
      (packet->event & MSG_EVT_MASK) != MSG_STACK_TO_HC_HCI_CMD &&
      send_transmit_finished;

  hci_transmit(packet);

  if (free_after_transmit) {
    buffer_allocator->free(packet);
  }
}

static void fragmenter_transmit_finished(BT_HDR* packet,
                                         bool all_fragments_sent) {
  if (all_fragments_sent) {
    buffer_allocator->free(packet);
  } else {
    // This is kind of a weird case, since we're dispatching a partially sent
    // packet up to a higher layer.
    // TODO(zachoverflow): rework upper layer so this isn't necessary.

    send_data_upwards.Run(FROM_HERE, packet);
  }
}

// Abort.  The chip has had time to write any debugging information.
static void hci_timeout_abort(void) {
  LOG_ERROR(LOG_TAG, "%s restarting the Bluetooth process.", __func__);
  hci_close_firmware_log_file(hci_firmware_log_fd);

  // We shouldn't try to recover the stack from this command timeout.
  // If it's caused by a software bug, fix it. If it's a hardware bug, fix it.
  abort();
}

static void hci_root_inflamed_abort(uint8_t error_code,
                                    uint8_t vendor_error_code) {
  LOG(FATAL) << __func__ << ": error_code = " << std::to_string(error_code)
             << ", vendor_error_code = " << std::to_string(vendor_error_code);
}

static void command_timed_out_log_info(void* original_wait_entry) {
  LOG_ERROR(LOG_TAG, "%s: %d commands pending response", __func__,
            get_num_waiting_commands());

  for (const list_node_t* node = list_begin(commands_pending_response);
       node != list_end(commands_pending_response); node = list_next(node)) {
    waiting_command_t* wait_entry =
        reinterpret_cast<waiting_command_t*>(list_node(node));

    int wait_time_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - wait_entry->timestamp)
            .count();
    LOG_ERROR(LOG_TAG, "%s: Waited %d ms for a response to opcode: 0x%x %s",
              __func__, wait_time_ms, wait_entry->opcode,
              (wait_entry == original_wait_entry) ? "*matches timer*" : "");

    // Dump the length field and the first byte of the payload, if present.
    uint8_t* command = wait_entry->command->data + wait_entry->command->offset;
    if (wait_entry->command->len > 3) {
      LOG_ERROR(LOG_TAG, "%s: Size %d Hex %02x %02x %02x %02x", __func__,
                wait_entry->command->len, command[0], command[1], command[2],
                command[3]);
    } else {
      LOG_ERROR(LOG_TAG, "%s: Size %d Hex %02x %02x %02x", __func__,
                wait_entry->command->len, command[0], command[1], command[2]);
    }

    LOG_EVENT_INT(BT_HCI_TIMEOUT_TAG_NUM, wait_entry->opcode);
    bluetooth::common::LogHciTimeoutEvent(wait_entry->opcode);
  }
}

// Print debugging information and quit. Don't dereference original_wait_entry.
static void command_timed_out(void* original_wait_entry) {
  LOG_ERROR(LOG_TAG, "%s", __func__);
  std::unique_lock<std::recursive_timed_mutex> lock(
      commands_pending_response_mutex, std::defer_lock);
  if (!lock.try_lock_for(std::chrono::milliseconds(
          COMMAND_PENDING_MUTEX_ACQUIRE_TIMEOUT_MS))) {
    LOG_ERROR(LOG_TAG, "%s: Cannot obtain the mutex", __func__);
    LOG_EVENT_INT(BT_HCI_TIMEOUT_TAG_NUM, HCI_UNKNOWN_COMMAND_TIMED_OUT);
    bluetooth::common::LogHciTimeoutEvent(android::bluetooth::hci::CMD_UNKNOWN);
  } else {
    command_timed_out_log_info(original_wait_entry);
    lock.unlock();
  }

  // Don't request a firmware dump for multiple hci timeouts
  if (hci_firmware_log_fd != INVALID_FD) {
    return;
  }

  LOG_ERROR(LOG_TAG, "%s: requesting a firmware dump.", __func__);

  /* Allocate a buffer to hold the HCI command. */
  BT_HDR* bt_hdr =
      static_cast<BT_HDR*>(osi_malloc(sizeof(BT_HDR) + HCIC_PREAMBLE_SIZE));

  bt_hdr->len = HCIC_PREAMBLE_SIZE;
  bt_hdr->event = MSG_STACK_TO_HC_HCI_CMD;
  bt_hdr->offset = 0;

  uint8_t* hci_packet = reinterpret_cast<uint8_t*>(bt_hdr + 1);

  UINT16_TO_STREAM(hci_packet,
                   HCI_GRP_VENDOR_SPECIFIC | HCI_CONTROLLER_DEBUG_INFO_OCF);
  UINT8_TO_STREAM(hci_packet, 0);  // No parameters

  hci_firmware_log_fd = hci_open_firmware_log_file();

  transmit_fragment(bt_hdr, true);

  osi_free(bt_hdr);
  LOG_ERROR(LOG_TAG, "%s: Setting a timer to restart.", __func__);

  // alarm_default_callbacks thread post to hci_thread.
  if (!abort_timer.Schedule(
          hci_thread.GetWeakPtr(), FROM_HERE, base::Bind(hci_timeout_abort),
          base::TimeDelta::FromMilliseconds(COMMAND_TIMEOUT_RESTART_MS))) {
    LOG_ERROR(LOG_TAG, "%s unable to create an abort timer.", __func__);
    abort();
  }
}

// Event/packet receiving functions
void process_command_credits(int credits) {
  std::lock_guard<std::mutex> command_credits_lock(command_credits_mutex);

  if (!hci_thread.IsRunning()) {
    // HCI Layer was shut down or not running
    return;
  }

  // Subtract commands in flight.
  command_credits = credits - get_num_waiting_commands();

  while (command_credits > 0 && !command_queue.empty()) {
    if (!hci_thread.DoInThread(FROM_HERE, std::move(command_queue.front()))) {
      LOG(ERROR) << __func__ << ": failed to enqueue command";
    }
    command_queue.pop();
    command_credits--;
  }
}

// Returns true if the event was intercepted and should not proceed to
// higher layers. Also inspects an incoming event for interesting
// information, like how many commands are now able to be sent.
static bool filter_incoming_event(BT_HDR* packet) {
  waiting_command_t* wait_entry = NULL;
  uint8_t* stream = packet->data;
  uint8_t event_code;
  int credits = 0;
  command_opcode_t opcode;

  STREAM_TO_UINT8(event_code, stream);
  STREAM_SKIP_UINT8(stream);  // Skip the parameter total length field

  if (event_code == HCI_COMMAND_COMPLETE_EVT) {
    STREAM_TO_UINT8(credits, stream);
    STREAM_TO_UINT16(opcode, stream);

    wait_entry = get_waiting_command(opcode);

    process_command_credits(credits);

    if (!wait_entry) {
      if (opcode != HCI_COMMAND_NONE) {
        LOG_WARN(LOG_TAG,
                 "%s command complete event with no matching command (opcode: "
                 "0x%04x).",
                 __func__, opcode);
      }
    } else {
      update_command_response_timer();
      if (wait_entry->complete_callback) {
        wait_entry->complete_callback(packet, wait_entry->context);
      } else if (wait_entry->complete_future) {
        future_ready(wait_entry->complete_future, packet);
      }
    }

    goto intercepted;
  } else if (event_code == HCI_COMMAND_STATUS_EVT) {
    uint8_t status;
    STREAM_TO_UINT8(status, stream);
    STREAM_TO_UINT8(credits, stream);
    STREAM_TO_UINT16(opcode, stream);

    // If a command generates a command status event, it won't be getting a
    // command complete event
    wait_entry = get_waiting_command(opcode);

    process_command_credits(credits);

    if (!wait_entry) {
      LOG_WARN(
          LOG_TAG,
          "%s command status event with no matching command. opcode: 0x%04x",
          __func__, opcode);
    } else {
      update_command_response_timer();
      if (wait_entry->status_callback)
        wait_entry->status_callback(status, wait_entry->command,
                                    wait_entry->context);
    }

    goto intercepted;
  } else if (event_code == HCI_VENDOR_SPECIFIC_EVT) {
    uint8_t sub_event_code;
    STREAM_TO_UINT8(sub_event_code, stream);

    if (sub_event_code == HCI_VSE_SUBCODE_DEBUG_INFO_SUB_EVT) {
      if (hci_firmware_log_fd == INVALID_FD)
        hci_firmware_log_fd = hci_open_firmware_log_file();

      if (hci_firmware_log_fd != INVALID_FD)
        hci_log_firmware_debug_packet(hci_firmware_log_fd, packet);

      buffer_allocator->free(packet);
      return true;
    } else if (sub_event_code == HCI_VSE_SUBCODE_BQR_SUB_EVT) {
      uint8_t bqr_report_id;
      STREAM_TO_UINT8(bqr_report_id, stream);

      if (bqr_report_id ==
              bluetooth::bqr::QUALITY_REPORT_ID_ROOT_INFLAMMATION &&
          packet->len >= bluetooth::bqr::kRootInflammationPacketMinSize) {
        uint8_t error_code;
        uint8_t vendor_error_code;
        STREAM_TO_UINT8(error_code, stream);
        STREAM_TO_UINT8(vendor_error_code, stream);
        // TODO(ugoyu) Report to bluetooth metrics here

        LOG(ERROR) << __func__
                   << ": Root inflammation event! setting timer to restart.";
        {
          // Try to stop hci command and startup timers
          std::unique_lock<std::recursive_timed_mutex> lock(
              commands_pending_response_mutex, std::defer_lock);
          if (lock.try_lock_for(std::chrono::milliseconds(
                  COMMAND_PENDING_MUTEX_ACQUIRE_TIMEOUT_MS))) {
            if (alarm_is_scheduled(startup_timer)) {
              alarm_cancel(startup_timer);
            }
            if (alarm_is_scheduled(command_response_timer)) {
              alarm_cancel(command_response_timer);
            }
          } else {
            LOG(ERROR) << __func__ << ": Failed to obtain mutex";
          }
        }

        // HwBinder thread post to hci_thread
        if (!abort_timer.Schedule(hci_thread.GetWeakPtr(), FROM_HERE,
                                  base::Bind(hci_root_inflamed_abort,
                                             error_code, vendor_error_code),
                                  base::TimeDelta::FromMilliseconds(
                                      COMMAND_TIMEOUT_RESTART_MS))) {
          LOG(ERROR) << "Failed to schedule abort_timer!";
          hci_root_inflamed_abort(error_code, vendor_error_code);
        }
      }
    }
  }

  return false;

intercepted:
  if (wait_entry) {
    // If it has a callback, it's responsible for freeing the packet
    if (event_code == HCI_COMMAND_STATUS_EVT ||
        (!wait_entry->complete_callback && !wait_entry->complete_future))
      buffer_allocator->free(packet);

    // If it has a callback, it's responsible for freeing the command
    if (event_code == HCI_COMMAND_COMPLETE_EVT || !wait_entry->status_callback)
      buffer_allocator->free(wait_entry->command);

    osi_free(wait_entry);
  } else {
    buffer_allocator->free(packet);
  }

  return true;
}

// Callback for the fragmenter to dispatch up a completely reassembled packet
static void dispatch_reassembled(BT_HDR* packet) {
  // Events should already have been dispatched before this point
  CHECK((packet->event & MSG_EVT_MASK) != MSG_HC_TO_STACK_HCI_EVT);
  CHECK(!send_data_upwards.is_null());

  send_data_upwards.Run(FROM_HERE, packet);
}

// Misc internal functions

static waiting_command_t* get_waiting_command(command_opcode_t opcode) {
  std::lock_guard<std::recursive_timed_mutex> lock(
      commands_pending_response_mutex);

  for (const list_node_t* node = list_begin(commands_pending_response);
       node != list_end(commands_pending_response); node = list_next(node)) {
    waiting_command_t* wait_entry =
        reinterpret_cast<waiting_command_t*>(list_node(node));

    if (!wait_entry || wait_entry->opcode != opcode) continue;

    list_remove(commands_pending_response, wait_entry);

    return wait_entry;
  }

  return NULL;
}

static int get_num_waiting_commands() {
  std::lock_guard<std::recursive_timed_mutex> lock(
      commands_pending_response_mutex);
  return list_length(commands_pending_response);
}

static void update_command_response_timer(void) {
  std::lock_guard<std::recursive_timed_mutex> lock(
      commands_pending_response_mutex);

  if (command_response_timer == NULL) return;
  if (list_is_empty(commands_pending_response)) {
    alarm_cancel(command_response_timer);
  } else {
    alarm_set(command_response_timer, COMMAND_PENDING_TIMEOUT_MS,
              command_timed_out, list_front(commands_pending_response));
  }
}

static void init_layer_interface() {
  if (!interface_created) {
    // It's probably ok for this to live forever. It's small and
    // there's only one instance of the hci interface.

    interface.set_data_cb = set_data_cb;
    interface.transmit_command = transmit_command;
    interface.transmit_command_futured = transmit_command_futured;
    interface.transmit_downward = transmit_downward;
    interface_created = true;
  }
}

void hci_layer_cleanup_interface() {
  if (interface_created) {
    send_data_upwards.Reset();

    interface.set_data_cb = NULL;
    interface.transmit_command = NULL;
    interface.transmit_command_futured = NULL;
    interface.transmit_downward = NULL;
    interface_created = false;
  }
}

const hci_t* hci_layer_get_interface() {
  buffer_allocator = buffer_allocator_get_interface();
  btsnoop = btsnoop_get_interface();
  packet_fragmenter = packet_fragmenter_get_interface();

  init_layer_interface();

  return &interface;
}

const hci_t* hci_layer_get_test_interface(
    const allocator_t* buffer_allocator_interface,
    const btsnoop_t* btsnoop_interface,
    const packet_fragmenter_t* packet_fragmenter_interface) {
  buffer_allocator = buffer_allocator_interface;
  btsnoop = btsnoop_interface;
  packet_fragmenter = packet_fragmenter_interface;

  init_layer_interface();
  return &interface;
}
