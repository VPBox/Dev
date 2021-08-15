/*
 * Copyright (C) 2018 The Android Open Source Project
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

package android.net.wifi;

// A callback to notify the results of sending a management frame.
interface ISendMgmtFrameEvent {
  // Error codes for OnFailure():

  // Unknown error.
  const int SEND_MGMT_FRAME_ERROR_UNKNOWN = 1;

  // Specifying the MCS rate is not supported by this device.
  const int SEND_MGMT_FRAME_ERROR_MCS_UNSUPPORTED = 2;

  // Driver reported that no ACK was received for the transmitted frame.
  const int SEND_MGMT_FRAME_ERROR_NO_ACK = 3;

  // Timed out while waiting for a response from the driver about the status
  // of the transmitted frame.
  const int SEND_MGMT_FRAME_ERROR_TIMEOUT = 4;

  // An existing transmission is in progress. Another frame cannot be sent until
  // the first transmission completes.
  const int SEND_MGMT_FRAME_ERROR_ALREADY_STARTED = 5;

  // Called when the management frame was successfully sent and ACKed by the
  // recipient.
  // @param elapsedTimeMs The elapsed time between when the management frame was
  //     sent and when the ACK was processed, in milliseconds, as measured by
  //     wificond. This includes the time that the send frame spent queuing
  //     before it was sent, any firmware retries, and the time the received
  //     ACK spent queuing before it was processed.
  oneway void OnAck(int elapsedTimeMs);

  // Called when the send failed.
  // @param reason The error code for the failure.
  oneway void OnFailure(int reason);
}
