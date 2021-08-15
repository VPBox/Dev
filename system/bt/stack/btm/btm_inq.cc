/******************************************************************************
 *
 *  Copyright 1999-2014 Broadcom Corporation
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

/******************************************************************************
 *
 *  This file contains functions that handle inquiries. These include
 *  setting discoverable mode, controlling the mode of the Baseband, and
 *  maintaining a small database of inquiry responses, with API for people
 *  to browse it.
 *
 ******************************************************************************/

#include <log/log.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/time_util.h"
#include "device/include/controller.h"
#include "osi/include/osi.h"

#include "advertise_data_parser.h"
#include "bt_common.h"
#include "bt_types.h"
#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"
#include "hcidefs.h"
#include "hcimsgs.h"

using bluetooth::Uuid;

/* 3 second timeout waiting for responses */
#define BTM_INQ_REPLY_TIMEOUT_MS (3 * 1000)

/* TRUE to enable DEBUG traces for btm_inq */
#ifndef BTM_INQ_DEBUG
#define BTM_INQ_DEBUG FALSE
#endif

/******************************************************************************/
/*               L O C A L    D A T A    D E F I N I T I O N S                */
/******************************************************************************/
static const LAP general_inq_lap = {0x9e, 0x8b, 0x33};
static const LAP limited_inq_lap = {0x9e, 0x8b, 0x00};

const uint16_t BTM_EIR_UUID_LKUP_TBL[BTM_EIR_MAX_SERVICES] = {
    UUID_SERVCLASS_SERVICE_DISCOVERY_SERVER,
    /*    UUID_SERVCLASS_BROWSE_GROUP_DESCRIPTOR,   */
    /*    UUID_SERVCLASS_PUBLIC_BROWSE_GROUP,       */
    UUID_SERVCLASS_SERIAL_PORT, UUID_SERVCLASS_LAN_ACCESS_USING_PPP,
    UUID_SERVCLASS_DIALUP_NETWORKING, UUID_SERVCLASS_IRMC_SYNC,
    UUID_SERVCLASS_OBEX_OBJECT_PUSH, UUID_SERVCLASS_OBEX_FILE_TRANSFER,
    UUID_SERVCLASS_IRMC_SYNC_COMMAND, UUID_SERVCLASS_HEADSET,
    UUID_SERVCLASS_CORDLESS_TELEPHONY, UUID_SERVCLASS_AUDIO_SOURCE,
    UUID_SERVCLASS_AUDIO_SINK, UUID_SERVCLASS_AV_REM_CTRL_TARGET,
    /*    UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION,    */
    UUID_SERVCLASS_AV_REMOTE_CONTROL,
    /*    UUID_SERVCLASS_VIDEO_CONFERENCING,        */
    UUID_SERVCLASS_INTERCOM, UUID_SERVCLASS_FAX,
    UUID_SERVCLASS_HEADSET_AUDIO_GATEWAY,
    /*    UUID_SERVCLASS_WAP,                       */
    /*    UUID_SERVCLASS_WAP_CLIENT,                */
    UUID_SERVCLASS_PANU, UUID_SERVCLASS_NAP, UUID_SERVCLASS_GN,
    UUID_SERVCLASS_DIRECT_PRINTING,
    /*    UUID_SERVCLASS_REFERENCE_PRINTING,        */
    UUID_SERVCLASS_IMAGING, UUID_SERVCLASS_IMAGING_RESPONDER,
    UUID_SERVCLASS_IMAGING_AUTO_ARCHIVE, UUID_SERVCLASS_IMAGING_REF_OBJECTS,
    UUID_SERVCLASS_HF_HANDSFREE, UUID_SERVCLASS_AG_HANDSFREE,
    UUID_SERVCLASS_DIR_PRT_REF_OBJ_SERVICE,
    /*    UUID_SERVCLASS_REFLECTED_UI,              */
    UUID_SERVCLASS_BASIC_PRINTING, UUID_SERVCLASS_PRINTING_STATUS,
    UUID_SERVCLASS_HUMAN_INTERFACE, UUID_SERVCLASS_CABLE_REPLACEMENT,
    UUID_SERVCLASS_HCRP_PRINT, UUID_SERVCLASS_HCRP_SCAN,
    /*    UUID_SERVCLASS_COMMON_ISDN_ACCESS,        */
    /*    UUID_SERVCLASS_VIDEO_CONFERENCING_GW,     */
    /*    UUID_SERVCLASS_UDI_MT,                    */
    /*    UUID_SERVCLASS_UDI_TA,                    */
    /*    UUID_SERVCLASS_VCP,                       */
    UUID_SERVCLASS_SAP, UUID_SERVCLASS_PBAP_PCE, UUID_SERVCLASS_PBAP_PSE,
    UUID_SERVCLASS_PHONE_ACCESS, UUID_SERVCLASS_HEADSET_HS,
    UUID_SERVCLASS_PNP_INFORMATION,
    /*    UUID_SERVCLASS_GENERIC_NETWORKING,        */
    /*    UUID_SERVCLASS_GENERIC_FILETRANSFER,      */
    /*    UUID_SERVCLASS_GENERIC_AUDIO,             */
    /*    UUID_SERVCLASS_GENERIC_TELEPHONY,         */
    /*    UUID_SERVCLASS_UPNP_SERVICE,              */
    /*    UUID_SERVCLASS_UPNP_IP_SERVICE,           */
    /*    UUID_SERVCLASS_ESDP_UPNP_IP_PAN,          */
    /*    UUID_SERVCLASS_ESDP_UPNP_IP_LAP,          */
    /*    UUID_SERVCLASS_ESDP_UPNP_IP_L2CAP,        */
    UUID_SERVCLASS_VIDEO_SOURCE, UUID_SERVCLASS_VIDEO_SINK,
    /*    UUID_SERVCLASS_VIDEO_DISTRIBUTION         */
    UUID_SERVCLASS_MESSAGE_ACCESS, UUID_SERVCLASS_MESSAGE_NOTIFICATION,
    UUID_SERVCLASS_HDP_SOURCE, UUID_SERVCLASS_HDP_SINK};

/******************************************************************************/
/*            L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/******************************************************************************/
static void btm_initiate_inquiry(tBTM_INQUIRY_VAR_ST* p_inq);
static tBTM_STATUS btm_set_inq_event_filter(uint8_t filter_cond_type,
                                            tBTM_INQ_FILT_COND* p_filt_cond);
static void btm_clr_inq_result_flt(void);

static uint8_t btm_convert_uuid_to_eir_service(uint16_t uuid16);
static void btm_set_eir_uuid(uint8_t* p_eir, tBTM_INQ_RESULTS* p_results);
static const uint8_t* btm_eir_get_uuid_list(uint8_t* p_eir, size_t eir_len,
                                            uint8_t uuid_size,
                                            uint8_t* p_num_uuid,
                                            uint8_t* p_uuid_list_type);

/*******************************************************************************
 *
 * Function         BTM_SetDiscoverability
 *
 * Description      This function is called to set the device into or out of
 *                  discoverable mode. Discoverable mode means inquiry
 *                  scans are enabled.  If a value of '0' is entered for window
 *                  or interval, the default values are used.
 *
 * Returns          BTM_SUCCESS if successful
 *                  BTM_BUSY if a setting of the filter is already in progress
 *                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
 *                  BTM_ILLEGAL_VALUE if a bad parameter was detected
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetDiscoverability(uint16_t inq_mode, uint16_t window,
                                   uint16_t interval) {
  uint8_t scan_mode = 0;
  uint16_t service_class;
  uint8_t* p_cod;
  uint8_t major, minor;
  DEV_CLASS cod;
  LAP temp_lap[2];
  bool is_limited;
  bool cod_limited;

  BTM_TRACE_API("BTM_SetDiscoverability");
  if (controller_get_interface()->supports_ble()) {
    if (btm_ble_set_discoverability((uint16_t)(inq_mode)) == BTM_SUCCESS) {
      btm_cb.btm_inq_vars.discoverable_mode &= (~BTM_BLE_DISCOVERABLE_MASK);
      btm_cb.btm_inq_vars.discoverable_mode |=
          (inq_mode & BTM_BLE_DISCOVERABLE_MASK);
    }
  }
  inq_mode &= ~BTM_BLE_DISCOVERABLE_MASK;

  /*** Check mode parameter ***/
  if (inq_mode > BTM_MAX_DISCOVERABLE) return (BTM_ILLEGAL_VALUE);

  /* Make sure the controller is active */
  if (!controller_get_interface()->get_is_ready()) return (BTM_DEV_RESET);

  /* If the window and/or interval is '0', set to default values */
  if (!window) window = BTM_DEFAULT_DISC_WINDOW;

  if (!interval) interval = BTM_DEFAULT_DISC_INTERVAL;

  BTM_TRACE_API(
      "BTM_SetDiscoverability: mode %d [NonDisc-0, Lim-1, Gen-2], window "
      "0x%04x, interval 0x%04x",
      inq_mode, window, interval);

  /*** Check for valid window and interval parameters ***/
  /*** Only check window and duration if mode is connectable ***/
  if (inq_mode != BTM_NON_DISCOVERABLE) {
    /* window must be less than or equal to interval */
    if (window < HCI_MIN_INQUIRYSCAN_WINDOW ||
        window > HCI_MAX_INQUIRYSCAN_WINDOW ||
        interval < HCI_MIN_INQUIRYSCAN_INTERVAL ||
        interval > HCI_MAX_INQUIRYSCAN_INTERVAL || window > interval) {
      return (BTM_ILLEGAL_VALUE);
    }
  }

  /* Set the IAC if needed */
  if (inq_mode != BTM_NON_DISCOVERABLE) {
    if (inq_mode & BTM_LIMITED_DISCOVERABLE) {
      /* Use the GIAC and LIAC codes for limited discoverable mode */
      memcpy(temp_lap[0], limited_inq_lap, LAP_LEN);
      memcpy(temp_lap[1], general_inq_lap, LAP_LEN);

      btsnd_hcic_write_cur_iac_lap(2, (LAP * const)temp_lap);
    } else {
      btsnd_hcic_write_cur_iac_lap(1, (LAP * const) & general_inq_lap);
    }

    scan_mode |= HCI_INQUIRY_SCAN_ENABLED;
  }

  /* Send down the inquiry scan window and period if changed */
  if ((window != btm_cb.btm_inq_vars.inq_scan_window) ||
      (interval != btm_cb.btm_inq_vars.inq_scan_period)) {
    btsnd_hcic_write_inqscan_cfg(interval, window);
    btm_cb.btm_inq_vars.inq_scan_window = window;
    btm_cb.btm_inq_vars.inq_scan_period = interval;
  }

  if (btm_cb.btm_inq_vars.connectable_mode & BTM_CONNECTABLE_MASK)
    scan_mode |= HCI_PAGE_SCAN_ENABLED;

  btsnd_hcic_write_scan_enable(scan_mode);
  btm_cb.btm_inq_vars.discoverable_mode &= (~BTM_DISCOVERABLE_MASK);
  btm_cb.btm_inq_vars.discoverable_mode |= inq_mode;

  /* Change the service class bit if mode has changed */
  p_cod = BTM_ReadDeviceClass();
  BTM_COD_SERVICE_CLASS(service_class, p_cod);
  is_limited = (inq_mode & BTM_LIMITED_DISCOVERABLE) ? true : false;
  cod_limited = (service_class & BTM_COD_SERVICE_LMTD_DISCOVER) ? true : false;
  if (is_limited ^ cod_limited) {
    BTM_COD_MINOR_CLASS(minor, p_cod);
    BTM_COD_MAJOR_CLASS(major, p_cod);
    if (is_limited)
      service_class |= BTM_COD_SERVICE_LMTD_DISCOVER;
    else
      service_class &= ~BTM_COD_SERVICE_LMTD_DISCOVER;

    FIELDS_TO_COD(cod, minor, major, service_class);
    (void)BTM_SetDeviceClass(cod);
  }

  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_SetInquiryScanType
 *
 * Description      This function is called to set the iquiry scan-type to
 *                  standard or interlaced.
 *
 * Returns          BTM_SUCCESS if successful
 *                  BTM_MODE_UNSUPPORTED if not a 1.2 device
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetInquiryScanType(uint16_t scan_type) {
  BTM_TRACE_API("BTM_SetInquiryScanType");
  if (scan_type != BTM_SCAN_TYPE_STANDARD &&
      scan_type != BTM_SCAN_TYPE_INTERLACED)
    return (BTM_ILLEGAL_VALUE);

  /* whatever app wants if device is not 1.2 scan type should be STANDARD */
  if (!controller_get_interface()->supports_interlaced_inquiry_scan())
    return (BTM_MODE_UNSUPPORTED);

  /* Check for scan type if configuration has been changed */
  if (scan_type != btm_cb.btm_inq_vars.inq_scan_type) {
    if (BTM_IsDeviceUp()) {
      btsnd_hcic_write_inqscan_type((uint8_t)scan_type);
      btm_cb.btm_inq_vars.inq_scan_type = scan_type;
    } else
      return (BTM_WRONG_MODE);
  }
  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_SetPageScanType
 *
 * Description      This function is called to set the page scan-type to
 *                  standard or interlaced.
 *
 * Returns          BTM_SUCCESS if successful
 *                  BTM_MODE_UNSUPPORTED if not a 1.2 device
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetPageScanType(uint16_t scan_type) {
  BTM_TRACE_API("BTM_SetPageScanType");
  if (scan_type != BTM_SCAN_TYPE_STANDARD &&
      scan_type != BTM_SCAN_TYPE_INTERLACED)
    return (BTM_ILLEGAL_VALUE);

  /* whatever app wants if device is not 1.2 scan type should be STANDARD */
  if (!controller_get_interface()->supports_interlaced_inquiry_scan())
    return (BTM_MODE_UNSUPPORTED);

  /* Check for scan type if configuration has been changed */
  if (scan_type != btm_cb.btm_inq_vars.page_scan_type) {
    if (BTM_IsDeviceUp()) {
      btsnd_hcic_write_pagescan_type((uint8_t)scan_type);
      btm_cb.btm_inq_vars.page_scan_type = scan_type;
    } else
      return (BTM_WRONG_MODE);
  }
  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_SetInquiryMode
 *
 * Description      This function is called to set standard or with RSSI
 *                  mode of the inquiry for local device.
 *
 * Output Params:   mode - standard, with RSSI, extended
 *
 * Returns          BTM_SUCCESS if successful
 *                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
 *                  BTM_ILLEGAL_VALUE if a bad parameter was detected
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetInquiryMode(uint8_t mode) {
  const controller_t* controller = controller_get_interface();
  BTM_TRACE_API("BTM_SetInquiryMode");
  if (mode == BTM_INQ_RESULT_STANDARD) {
    /* mandatory mode */
  } else if (mode == BTM_INQ_RESULT_WITH_RSSI) {
    if (!controller->supports_rssi_with_inquiry_results())
      return (BTM_MODE_UNSUPPORTED);
  } else if (mode == BTM_INQ_RESULT_EXTENDED) {
    if (!controller->supports_extended_inquiry_response())
      return (BTM_MODE_UNSUPPORTED);
  } else
    return (BTM_ILLEGAL_VALUE);

  if (!BTM_IsDeviceUp()) return (BTM_WRONG_MODE);

  btsnd_hcic_write_inquiry_mode(mode);

  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_ReadDiscoverability
 *
 * Description      This function is called to read the current discoverability
 *                  mode of the device.
 *
 * Output Params:   p_window - current inquiry scan duration
 *                  p_interval - current inquiry scan interval
 *
 * Returns          BTM_NON_DISCOVERABLE, BTM_LIMITED_DISCOVERABLE, or
 *                  BTM_GENERAL_DISCOVERABLE
 *
 ******************************************************************************/
uint16_t BTM_ReadDiscoverability(uint16_t* p_window, uint16_t* p_interval) {
  BTM_TRACE_API("BTM_ReadDiscoverability");
  if (p_window) *p_window = btm_cb.btm_inq_vars.inq_scan_window;

  if (p_interval) *p_interval = btm_cb.btm_inq_vars.inq_scan_period;

  return (btm_cb.btm_inq_vars.discoverable_mode);
}

/*******************************************************************************
 *
 * Function         BTM_SetPeriodicInquiryMode
 *
 * Description      This function is called to set the device periodic inquiry
 *                  mode. If the duration is zero, the periodic inquiry mode is
 *                  cancelled.
 *
 *                  Note: We currently do not allow concurrent inquiry and
 *                  periodic inquiry.
 *
 * Parameters:      p_inqparms - pointer to the inquiry information
 *                      mode - GENERAL or LIMITED inquiry
 *                      duration - length in 1.28 sec intervals (If '0', the
 *                                 inquiry is CANCELLED)
 *                      max_resps - maximum amount of devices to search for
 *                                  before ending the inquiry
 *                      filter_cond_type - BTM_CLR_INQUIRY_FILTER,
 *                                         BTM_FILTER_COND_DEVICE_CLASS, or
 *                                         BTM_FILTER_COND_BD_ADDR
 *                      filter_cond - value for the filter (based on
 *                                                          filter_cond_type)
 *
 *                  max_delay - maximum amount of time between successive
 *                              inquiries
 *                  min_delay - minimum amount of time between successive
 *                              inquiries
 *                  p_results_cb - callback returning pointer to results
 *                              (tBTM_INQ_RESULTS)
 *
 * Returns          BTM_CMD_STARTED if successfully started
 *                  BTM_ILLEGAL_VALUE if a bad parameter is detected
 *                  BTM_NO_RESOURCES if could not allocate a message buffer
 *                  BTM_SUCCESS - if cancelling the periodic inquiry
 *                  BTM_BUSY - if an inquiry is already active
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetPeriodicInquiryMode(tBTM_INQ_PARMS* p_inqparms,
                                       uint16_t max_delay, uint16_t min_delay,
                                       tBTM_INQ_RESULTS_CB* p_results_cb) {
  tBTM_STATUS status;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  BTM_TRACE_API(
      "BTM_SetPeriodicInquiryMode: mode: %d, dur: %d, rsps: %d, flt: %d, min: "
      "%d, max: %d",
      p_inqparms->mode, p_inqparms->duration, p_inqparms->max_resps,
      p_inqparms->filter_cond_type, min_delay, max_delay);

  /*** Make sure the device is ready ***/
  if (!BTM_IsDeviceUp()) return (BTM_WRONG_MODE);

  /* Only one active inquiry is allowed in this implementation.
     Also do not allow an inquiry if the inquiry filter is being updated */
  if (p_inq->inq_active || p_inq->inqfilt_active) return (BTM_BUSY);

  /* If illegal parameters return false */
  if (p_inqparms->mode != BTM_GENERAL_INQUIRY &&
      p_inqparms->mode != BTM_LIMITED_INQUIRY)
    return (BTM_ILLEGAL_VALUE);

  /* Verify the parameters for this command */
  if (p_inqparms->duration < BTM_MIN_INQUIRY_LEN ||
      p_inqparms->duration > BTM_MAX_INQUIRY_LENGTH ||
      min_delay <= p_inqparms->duration ||
      min_delay < BTM_PER_INQ_MIN_MIN_PERIOD ||
      min_delay > BTM_PER_INQ_MAX_MIN_PERIOD || max_delay <= min_delay ||
      max_delay < BTM_PER_INQ_MIN_MAX_PERIOD)
  /*       max_delay > BTM_PER_INQ_MAX_MAX_PERIOD)*/
  /*  BTM_PER_INQ_MAX_MAX_PERIOD set to 1's in all bits. Condition resulting in
     false always*/
  {
    return (BTM_ILLEGAL_VALUE);
  }

  /* Save the inquiry parameters to be used upon the completion of
   * setting/clearing the inquiry filter */
  p_inq->inqparms = *p_inqparms;
  p_inq->per_min_delay = min_delay;
  p_inq->per_max_delay = max_delay;
  p_inq->inq_cmpl_info.num_resp = 0; /* Clear the results counter */
  p_inq->p_inq_results_cb = p_results_cb;

  p_inq->inq_active = (uint8_t)(
      (p_inqparms->mode == BTM_LIMITED_INQUIRY)
          ? (BTM_LIMITED_INQUIRY_ACTIVE | BTM_PERIODIC_INQUIRY_ACTIVE)
          : (BTM_GENERAL_INQUIRY_ACTIVE | BTM_PERIODIC_INQUIRY_ACTIVE));

  /* If a filter is specified, then save it for later and clear the current
     filter.
     The setting of the filter is done upon completion of clearing of the
     previous
     filter.
  */
  if (p_inqparms->filter_cond_type != BTM_CLR_INQUIRY_FILTER) {
    p_inq->state = BTM_INQ_CLR_FILT_STATE;
    p_inqparms->filter_cond_type = BTM_CLR_INQUIRY_FILTER;
  } else /* The filter is not being used so simply clear it; the inquiry can
            start after this operation */
    p_inq->state = BTM_INQ_SET_FILT_STATE;

  /* Before beginning the inquiry the current filter must be cleared, so
   * initiate the command */
  status = btm_set_inq_event_filter(p_inqparms->filter_cond_type,
                                    &p_inqparms->filter_cond);
  if (status != BTM_CMD_STARTED) {
    /* If set filter command is not succesful reset the state */
    p_inq->p_inq_results_cb = NULL;
    p_inq->state = BTM_INQ_INACTIVE_STATE;
  }

  return (status);
}

/*******************************************************************************
 *
 * Function         BTM_CancelPeriodicInquiry
 *
 * Description      This function cancels a periodic inquiry
 *
 * Returns
 *                  BTM_NO_RESOURCES if could not allocate a message buffer
 *                  BTM_SUCCESS - if cancelling the periodic inquiry
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_CancelPeriodicInquiry(void) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  tBTM_STATUS status = BTM_SUCCESS;
  BTM_TRACE_API("BTM_CancelPeriodicInquiry called");

  /*** Make sure the device is ready ***/
  if (!BTM_IsDeviceUp()) return (BTM_WRONG_MODE);

  /* Only cancel if one is active */
  if (btm_cb.btm_inq_vars.inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) {
    btm_cb.btm_inq_vars.inq_active = BTM_INQUIRY_INACTIVE;
    btm_cb.btm_inq_vars.p_inq_results_cb = NULL;

    btsnd_hcic_exit_per_inq();

    /* If the event filter is in progress, mark it so that the processing of the
       return
       event will be ignored */
    if (p_inq->inqfilt_active) p_inq->pending_filt_complete_event++;

    p_inq->inqfilt_active = false;
    p_inq->inq_counter++;
  }

  return (status);
}

/*******************************************************************************
 *
 * Function         BTM_SetConnectability
 *
 * Description      This function is called to set the device into or out of
 *                  connectable mode. Discoverable mode means page scans are
 *                  enabled.
 *
 * Returns          BTM_SUCCESS if successful
 *                  BTM_ILLEGAL_VALUE if a bad parameter is detected
 *                  BTM_NO_RESOURCES if could not allocate a message buffer
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_SetConnectability(uint16_t page_mode, uint16_t window,
                                  uint16_t interval) {
  uint8_t scan_mode = 0;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  BTM_TRACE_API("BTM_SetConnectability");

  if (controller_get_interface()->supports_ble()) {
    if (btm_ble_set_connectability(page_mode) != BTM_SUCCESS) {
      return BTM_NO_RESOURCES;
    }
    p_inq->connectable_mode &= (~BTM_BLE_CONNECTABLE_MASK);
    p_inq->connectable_mode |= (page_mode & BTM_BLE_CONNECTABLE_MASK);
  }
  page_mode &= ~BTM_BLE_CONNECTABLE_MASK;

  /*** Check mode parameter ***/
  if (page_mode != BTM_NON_CONNECTABLE && page_mode != BTM_CONNECTABLE)
    return (BTM_ILLEGAL_VALUE);

  /* Make sure the controller is active */
  if (!controller_get_interface()->get_is_ready()) return (BTM_DEV_RESET);

  /* If the window and/or interval is '0', set to default values */
  if (!window) window = BTM_DEFAULT_CONN_WINDOW;

  if (!interval) interval = BTM_DEFAULT_CONN_INTERVAL;

  BTM_TRACE_API(
      "BTM_SetConnectability: mode %d [NonConn-0, Conn-1], window 0x%04x, "
      "interval 0x%04x",
      page_mode, window, interval);

  /*** Check for valid window and interval parameters ***/
  /*** Only check window and duration if mode is connectable ***/
  if (page_mode == BTM_CONNECTABLE) {
    /* window must be less than or equal to interval */
    if (window < HCI_MIN_PAGESCAN_WINDOW || window > HCI_MAX_PAGESCAN_WINDOW ||
        interval < HCI_MIN_PAGESCAN_INTERVAL ||
        interval > HCI_MAX_PAGESCAN_INTERVAL || window > interval) {
      return (BTM_ILLEGAL_VALUE);
    }

    scan_mode |= HCI_PAGE_SCAN_ENABLED;
  }

  if ((window != p_inq->page_scan_window) ||
      (interval != p_inq->page_scan_period)) {
    p_inq->page_scan_window = window;
    p_inq->page_scan_period = interval;
    btsnd_hcic_write_pagescan_cfg(interval, window);
  }

  /* Keep the inquiry scan as previouosly set */
  if (p_inq->discoverable_mode & BTM_DISCOVERABLE_MASK)
    scan_mode |= HCI_INQUIRY_SCAN_ENABLED;

  btsnd_hcic_write_scan_enable(scan_mode);
  p_inq->connectable_mode &= (~BTM_CONNECTABLE_MASK);
  p_inq->connectable_mode |= page_mode;
  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_ReadConnectability
 *
 * Description      This function is called to read the current discoverability
 *                  mode of the device.
 * Output Params    p_window - current page scan duration
 *                  p_interval - current time between page scans
 *
 * Returns          BTM_NON_CONNECTABLE or BTM_CONNECTABLE
 *
 ******************************************************************************/
uint16_t BTM_ReadConnectability(uint16_t* p_window, uint16_t* p_interval) {
  BTM_TRACE_API("BTM_ReadConnectability");
  if (p_window) *p_window = btm_cb.btm_inq_vars.page_scan_window;

  if (p_interval) *p_interval = btm_cb.btm_inq_vars.page_scan_period;

  return (btm_cb.btm_inq_vars.connectable_mode);
}

/*******************************************************************************
 *
 * Function         BTM_IsInquiryActive
 *
 * Description      This function returns a bit mask of the current inquiry
 *                  state
 *
 * Returns          BTM_INQUIRY_INACTIVE if inactive (0)
 *                  BTM_LIMITED_INQUIRY_ACTIVE if a limted inquiry is active
 *                  BTM_GENERAL_INQUIRY_ACTIVE if a general inquiry is active
 *                  BTM_PERIODIC_INQUIRY_ACTIVE if a periodic inquiry is active
 *
 ******************************************************************************/
uint16_t BTM_IsInquiryActive(void) {
  BTM_TRACE_API("BTM_IsInquiryActive");

  return (btm_cb.btm_inq_vars.inq_active);
}

/*******************************************************************************
 *
 * Function         BTM_CancelInquiry
 *
 * Description      This function cancels an inquiry if active
 *
 * Returns          BTM_SUCCESS if successful
 *                  BTM_NO_RESOURCES if could not allocate a message buffer
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_CancelInquiry(void) {
  tBTM_STATUS status = BTM_SUCCESS;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  BTM_TRACE_API("BTM_CancelInquiry called");

  /*** Make sure the device is ready ***/
  if (!BTM_IsDeviceUp()) return (BTM_WRONG_MODE);

  /* Only cancel if not in periodic mode, otherwise the caller should call
   * BTM_CancelPeriodicMode */
  if ((p_inq->inq_active & BTM_INQUIRY_ACTIVE_MASK) != 0 &&
      (!(p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE))) {
    p_inq->inq_active = BTM_INQUIRY_INACTIVE;
    p_inq->state = BTM_INQ_INACTIVE_STATE;
    p_inq->p_inq_results_cb = NULL; /* Do not notify caller anymore */
    p_inq->p_inq_cmpl_cb = NULL;    /* Do not notify caller anymore */

    /* If the event filter is in progress, mark it so that the processing of the
       return
        event will be ignored */
    if (p_inq->inqfilt_active) {
      p_inq->inqfilt_active = false;
      p_inq->pending_filt_complete_event++;
    }
    /* Initiate the cancel inquiry */
    else {
      if ((p_inq->inqparms.mode & BTM_BR_INQUIRY_MASK) != 0) {
        btsnd_hcic_inq_cancel();
      }
      if ((p_inq->inqparms.mode & BTM_BLE_INQUIRY_MASK) != 0)
        btm_ble_stop_inquiry();
    }

    /* Do not send the BUSY_LEVEL event yet. Wait for the cancel_complete event
     * and then send the BUSY_LEVEL event
     * btm_acl_update_busy_level (BTM_BLI_INQ_DONE_EVT);
     */

    p_inq->inq_counter++;
    btm_clr_inq_result_flt();
  }

  return (status);
}

/*******************************************************************************
 *
 * Function         BTM_StartInquiry
 *
 * Description      This function is called to start an inquiry.
 *
 * Parameters:      p_inqparms - pointer to the inquiry information
 *                      mode - GENERAL or LIMITED inquiry, BR/LE bit mask
 *                             seperately
 *                      duration - length in 1.28 sec intervals (If '0', the
 *                                 inquiry is CANCELLED)
 *                      max_resps - maximum amount of devices to search for
 *                                  before ending the inquiry
 *                      filter_cond_type - BTM_CLR_INQUIRY_FILTER,
 *                                         BTM_FILTER_COND_DEVICE_CLASS, or
 *                                         BTM_FILTER_COND_BD_ADDR
 *                      filter_cond - value for the filter (based on
 *                                                          filter_cond_type)
 *
 *                  p_results_cb   - Pointer to the callback routine which gets
 *                                called upon receipt of an inquiry result. If
 *                                this field is NULL, the application is not
 *                                notified.
 *
 *                  p_cmpl_cb   - Pointer to the callback routine which gets
 *                                called upon completion.  If this field is
 *                                NULL, the application is not notified when
 *                                completed.
 * Returns          tBTM_STATUS
 *                  BTM_CMD_STARTED if successfully initiated
 *                  BTM_BUSY if already in progress
 *                  BTM_ILLEGAL_VALUE if parameter(s) are out of range
 *                  BTM_NO_RESOURCES if could not allocate resources to start
 *                                   the command
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_StartInquiry(tBTM_INQ_PARMS* p_inqparms,
                             tBTM_INQ_RESULTS_CB* p_results_cb,
                             tBTM_CMPL_CB* p_cmpl_cb) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  BTM_TRACE_API("BTM_StartInquiry: mode: %d, dur: %d, rsps: %d, flt: %d",
                p_inqparms->mode, p_inqparms->duration, p_inqparms->max_resps,
                p_inqparms->filter_cond_type);

  /* Only one active inquiry is allowed in this implementation.
     Also do not allow an inquiry if the inquiry filter is being updated */
  if (p_inq->inq_active || p_inq->inqfilt_active) {
    /*check if LE observe is already running*/
    if (p_inq->scan_type == INQ_LE_OBSERVE &&
        p_inq->p_inq_ble_results_cb != nullptr) {
      BTM_TRACE_API("BTM_StartInquiry: LE observe in progress");
      p_inq->scan_type = INQ_GENERAL;
      p_inq->inq_active = BTM_INQUIRY_INACTIVE;
      btm_cb.ble_ctr_cb.inq_var.scan_type = BTM_BLE_SCAN_MODE_NONE;
      btm_send_hci_scan_enable(BTM_BLE_SCAN_DISABLE, BTM_BLE_DUPLICATE_ENABLE);
    } else {
      LOG(ERROR) << __func__ << ": BTM_BUSY";
      return (BTM_BUSY);
    }
  } else {
    p_inq->scan_type = INQ_GENERAL;
  }

  /*** Make sure the device is ready ***/
  if (!BTM_IsDeviceUp()) {
    LOG(ERROR) << __func__ << ": adapter is not up";
    return BTM_WRONG_MODE;
  }

  if ((p_inqparms->mode & BTM_BR_INQUIRY_MASK) != BTM_GENERAL_INQUIRY &&
      (p_inqparms->mode & BTM_BR_INQUIRY_MASK) != BTM_LIMITED_INQUIRY &&
      (p_inqparms->mode & BTM_BLE_INQUIRY_MASK) != BTM_BLE_GENERAL_INQUIRY &&
      (p_inqparms->mode & BTM_BLE_INQUIRY_MASK) != BTM_BLE_LIMITED_INQUIRY) {
    LOG(ERROR) << __func__ << ": illegal inquiry mode "
               << std::to_string(p_inqparms->mode);
    return (BTM_ILLEGAL_VALUE);
  }

  /* Save the inquiry parameters to be used upon the completion of
   * setting/clearing the inquiry filter */
  p_inq->inqparms = *p_inqparms;

  /* Initialize the inquiry variables */
  p_inq->state = BTM_INQ_ACTIVE_STATE;
  p_inq->p_inq_cmpl_cb = p_cmpl_cb;
  p_inq->p_inq_results_cb = p_results_cb;
  p_inq->inq_cmpl_info.num_resp = 0; /* Clear the results counter */
  p_inq->inq_active = p_inqparms->mode;

  BTM_TRACE_DEBUG("BTM_StartInquiry: p_inq->inq_active = 0x%02x",
                  p_inq->inq_active);

  tBTM_STATUS status = BTM_CMD_STARTED;
  /* start LE inquiry here if requested */
  if ((p_inqparms->mode & BTM_BLE_INQUIRY_MASK)) {
    if (!controller_get_interface()->supports_ble()) {
      LOG(ERROR) << __func__ << ": trying to do LE scan on a non-LE adapter";
      p_inq->inqparms.mode &= ~BTM_BLE_INQUIRY_MASK;
      status = BTM_ILLEGAL_VALUE;
    } else {
      /* BLE for now does not support filter condition for inquiry */
      status = btm_ble_start_inquiry(
          (uint8_t)(p_inqparms->mode & BTM_BLE_INQUIRY_MASK),
          p_inqparms->duration);
      if (status != BTM_CMD_STARTED) {
        LOG(ERROR) << __func__ << ": Error Starting LE Inquiry";
        p_inq->inqparms.mode &= ~BTM_BLE_INQUIRY_MASK;
      }
    }
    p_inqparms->mode &= ~BTM_BLE_INQUIRY_MASK;

    BTM_TRACE_DEBUG("BTM_StartInquiry: mode = %02x", p_inqparms->mode);
  }

  /* we're done with this routine if BR/EDR inquiry is not desired. */
  if ((p_inqparms->mode & BTM_BR_INQUIRY_MASK) == BTM_INQUIRY_NONE) {
    return status;
  }

  /* BR/EDR inquiry portion */
  /* If a filter is specified, then save it for later and clear the current
     filter.
     The setting of the filter is done upon completion of clearing of the
     previous
     filter.
  */
  switch (p_inqparms->filter_cond_type) {
    case BTM_CLR_INQUIRY_FILTER:
      p_inq->state = BTM_INQ_SET_FILT_STATE;
      break;

    case BTM_FILTER_COND_DEVICE_CLASS:
    case BTM_FILTER_COND_BD_ADDR:
      /* The filter is not being used so simply clear it;
          the inquiry can start after this operation */
      p_inq->state = BTM_INQ_CLR_FILT_STATE;
      p_inqparms->filter_cond_type = BTM_CLR_INQUIRY_FILTER;
      /* =============>>>> adding LE filtering here ????? */
      break;

    default:
      LOG(ERROR) << __func__ << ": invalid filter condition type "
                 << std::to_string(p_inqparms->filter_cond_type);
      return (BTM_ILLEGAL_VALUE);
    }

    /* Before beginning the inquiry the current filter must be cleared, so
     * initiate the command */
    status = btm_set_inq_event_filter(p_inqparms->filter_cond_type,
                                      &p_inqparms->filter_cond);
    if (status != BTM_CMD_STARTED) {
      LOG(ERROR) << __func__ << ": failed to set inquiry event filter";
      p_inq->state = BTM_INQ_INACTIVE_STATE;
    }

    return (status);
}

/*******************************************************************************
 *
 * Function         BTM_ReadRemoteDeviceName
 *
 * Description      This function initiates a remote device HCI command to the
 *                  controller and calls the callback when the process has
 *                  completed.
 *
 * Input Params:    remote_bda      - device address of name to retrieve
 *                  p_cb            - callback function called when
 *                                    BTM_CMD_STARTED is returned.
 *                                    A pointer to tBTM_REMOTE_DEV_NAME is
 *                                    passed to the callback.
 *
 * Returns
 *                  BTM_CMD_STARTED is returned if the request was successfully
 *                                  sent to HCI.
 *                  BTM_BUSY if already in progress
 *                  BTM_UNKNOWN_ADDR if device address is bad
 *                  BTM_NO_RESOURCES if could not allocate resources to start
 *                                   the command
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadRemoteDeviceName(const RawAddress& remote_bda,
                                     tBTM_CMPL_CB* p_cb,
                                     tBT_TRANSPORT transport) {
  VLOG(1) << __func__ << ": bd addr " << remote_bda;
  /* Use LE transport when LE is the only available option */
  if (transport == BT_TRANSPORT_LE) {
    return btm_ble_read_remote_name(remote_bda, p_cb);
  }
  /* Use classic transport for BR/EDR and Dual Mode devices */
  return btm_initiate_rem_name(remote_bda, BTM_RMT_NAME_EXT,
                               BTM_EXT_RMT_NAME_TIMEOUT_MS, p_cb);
}

/*******************************************************************************
 *
 * Function         BTM_CancelRemoteDeviceName
 *
 * Description      This function initiates the cancel request for the specified
 *                  remote device.
 *
 * Input Params:    None
 *
 * Returns
 *                  BTM_CMD_STARTED is returned if the request was successfully
 *                                  sent to HCI.
 *                  BTM_NO_RESOURCES if could not allocate resources to start
 *                                   the command
 *                  BTM_WRONG_MODE if there is not an active remote name
 *                                 request.
 *
 ******************************************************************************/
tBTM_STATUS BTM_CancelRemoteDeviceName(void) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  BTM_TRACE_API("BTM_CancelRemoteDeviceName()");

  /* Make sure there is not already one in progress */
  if (p_inq->remname_active) {
    if (BTM_UseLeLink(p_inq->remname_bda)) {
      if (btm_ble_cancel_remote_name(p_inq->remname_bda))
        return (BTM_CMD_STARTED);
      else
        return (BTM_UNKNOWN_ADDR);
    } else
      btsnd_hcic_rmt_name_req_cancel(p_inq->remname_bda);
    return (BTM_CMD_STARTED);
  } else
    return (BTM_WRONG_MODE);
}

/*******************************************************************************
 *
 * Function         BTM_InqDbRead
 *
 * Description      This function looks through the inquiry database for a match
 *                  based on Bluetooth Device Address. This is the application's
 *                  interface to get the inquiry details of a specific BD
 *                  address.
 *
 * Returns          pointer to entry, or NULL if not found
 *
 ******************************************************************************/
tBTM_INQ_INFO* BTM_InqDbRead(const RawAddress& p_bda) {
  VLOG(1) << __func__ << ": bd addr " << p_bda;

  tINQ_DB_ENT* p_ent = btm_inq_db_find(p_bda);
  if (!p_ent) return NULL;

  return &p_ent->inq_info;
}

/*******************************************************************************
 *
 * Function         BTM_InqDbFirst
 *
 * Description      This function looks through the inquiry database for the
 *                  first used entry, and returns that. This is used in
 *                  conjunction with
 *                  BTM_InqDbNext by applications as a way to walk through the
 *                  inquiry database.
 *
 * Returns          pointer to first in-use entry, or NULL if DB is empty
 *
 ******************************************************************************/
tBTM_INQ_INFO* BTM_InqDbFirst(void) {
  uint16_t xx;
  tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;

  for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
    if (p_ent->in_use) return (&p_ent->inq_info);
  }

  /* If here, no used entry found */
  return ((tBTM_INQ_INFO*)NULL);
}

/*******************************************************************************
 *
 * Function         BTM_InqDbNext
 *
 * Description      This function looks through the inquiry database for the
 *                  next used entry, and returns that.  If the input parameter
 *                  is NULL, the first entry is returned.
 *
 * Returns          pointer to next in-use entry, or NULL if no more found.
 *
 ******************************************************************************/
tBTM_INQ_INFO* BTM_InqDbNext(tBTM_INQ_INFO* p_cur) {
  tINQ_DB_ENT* p_ent;
  uint16_t inx;

  if (p_cur) {
    p_ent = (tINQ_DB_ENT*)((uint8_t*)p_cur - offsetof(tINQ_DB_ENT, inq_info));
    inx = (uint16_t)((p_ent - btm_cb.btm_inq_vars.inq_db) + 1);

    for (p_ent = &btm_cb.btm_inq_vars.inq_db[inx]; inx < BTM_INQ_DB_SIZE;
         inx++, p_ent++) {
      if (p_ent->in_use) return (&p_ent->inq_info);
    }

    /* If here, more entries found */
    return ((tBTM_INQ_INFO*)NULL);
  } else
    return (BTM_InqDbFirst());
}

/*******************************************************************************
 *
 * Function         BTM_ClearInqDb
 *
 * Description      This function is called to clear out a device or all devices
 *                  from the inquiry database.
 *
 * Parameter        p_bda - (input) BD_ADDR ->  Address of device to clear
 *                                              (NULL clears all entries)
 *
 * Returns          BTM_BUSY if an inquiry, get remote name, or event filter
 *                          is active, otherwise BTM_SUCCESS
 *
 ******************************************************************************/
tBTM_STATUS BTM_ClearInqDb(const RawAddress* p_bda) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  /* If an inquiry or remote name is in progress return busy */
  if (p_inq->inq_active != BTM_INQUIRY_INACTIVE || p_inq->inqfilt_active)
    return (BTM_BUSY);

  btm_clr_inq_db(p_bda);

  return (BTM_SUCCESS);
}

/*******************************************************************************
 *
 * Function         BTM_ReadInquiryRspTxPower
 *
 * Description      This command will read the inquiry Transmit Power level used
 *                  to transmit the FHS and EIR data packets. This can be used
 *                  directly in the Tx Power Level EIR data type.
 *
 * Returns          BTM_SUCCESS if successful
 *
 ******************************************************************************/
tBTM_STATUS BTM_ReadInquiryRspTxPower(tBTM_CMPL_CB* p_cb) {
  if (btm_cb.devcb.p_inq_tx_power_cmpl_cb) return (BTM_BUSY);

  btm_cb.devcb.p_inq_tx_power_cmpl_cb = p_cb;
  alarm_set_on_mloop(btm_cb.devcb.read_inq_tx_power_timer,
                     BTM_INQ_REPLY_TIMEOUT_MS, btm_read_inq_tx_power_timeout,
                     NULL);

  btsnd_hcic_read_inq_tx_power();
  return (BTM_CMD_STARTED);
}

/*******************************************************************************
 *******************************************************************************
 *                                                                            **
 *                    BTM Internal Inquiry Functions                          **
 *                                                                            **
 *******************************************************************************
 ******************************************************************************/
/*******************************************************************************
 *
 * Function         btm_inq_db_reset
 *
 * Description      This function is called at at reset to clear the inquiry
 *                  database & pending callback.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_inq_db_reset(void) {
  tBTM_REMOTE_DEV_NAME rem_name;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  uint8_t num_responses;
  uint8_t temp_inq_active;
  tBTM_STATUS status;

  /* If an inquiry or periodic inquiry is active, reset the mode to inactive */
  if (p_inq->inq_active != BTM_INQUIRY_INACTIVE) {
    temp_inq_active = p_inq->inq_active; /* Save so state can change BEFORE
                                                callback is called */
    p_inq->inq_active = BTM_INQUIRY_INACTIVE;

    /* If not a periodic inquiry, the complete callback must be called to notify
     * caller */
    if (temp_inq_active == BTM_LIMITED_INQUIRY_ACTIVE ||
        temp_inq_active == BTM_GENERAL_INQUIRY_ACTIVE) {
      if (p_inq->p_inq_cmpl_cb) {
        num_responses = 0;
        (*p_inq->p_inq_cmpl_cb)(&num_responses);
      }
    }
  }

  /* Cancel a remote name request if active, and notify the caller (if waiting)
   */
  if (p_inq->remname_active) {
    alarm_cancel(p_inq->remote_name_timer);
    p_inq->remname_active = false;
    p_inq->remname_bda = RawAddress::kEmpty;

    if (p_inq->p_remname_cmpl_cb) {
      rem_name.status = BTM_DEV_RESET;

      (*p_inq->p_remname_cmpl_cb)(&rem_name);
      p_inq->p_remname_cmpl_cb = NULL;
    }
  }

  /* Cancel an inquiry filter request if active, and notify the caller (if
   * waiting) */
  if (p_inq->inqfilt_active) {
    p_inq->inqfilt_active = false;

    if (p_inq->p_inqfilter_cmpl_cb) {
      status = BTM_DEV_RESET;
      (*p_inq->p_inqfilter_cmpl_cb)(&status);
    }
  }

  p_inq->state = BTM_INQ_INACTIVE_STATE;
  p_inq->pending_filt_complete_event = 0;
  p_inq->p_inq_results_cb = NULL;
  btm_clr_inq_db(NULL); /* Clear out all the entries in the database */
  btm_clr_inq_result_flt();

  p_inq->discoverable_mode = BTM_NON_DISCOVERABLE;
  p_inq->connectable_mode = BTM_NON_CONNECTABLE;
  p_inq->page_scan_type = BTM_SCAN_TYPE_STANDARD;
  p_inq->inq_scan_type = BTM_SCAN_TYPE_STANDARD;

  p_inq->discoverable_mode |= BTM_BLE_NON_DISCOVERABLE;
  p_inq->connectable_mode |= BTM_BLE_NON_CONNECTABLE;
  return;
}

/*******************************************************************************
 *
 * Function         btm_inq_db_init
 *
 * Description      This function is called at startup to initialize the inquiry
 *                  database.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_inq_db_init(void) {
  alarm_free(btm_cb.btm_inq_vars.remote_name_timer);
  btm_cb.btm_inq_vars.remote_name_timer =
      alarm_new("btm_inq.remote_name_timer");
  btm_cb.btm_inq_vars.no_inc_ssp = BTM_NO_SSP_ON_INQUIRY;
}

/*******************************************************************************
 *
 * Function         btm_inq_stop_on_ssp
 *
 * Description      This function is called on incoming SSP
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_inq_stop_on_ssp(void) {
  uint8_t normal_active =
      (BTM_GENERAL_INQUIRY_ACTIVE | BTM_LIMITED_INQUIRY_ACTIVE);

#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG(
      "btm_inq_stop_on_ssp: no_inc_ssp=%d inq_active:0x%x state:%d "
      "inqfilt_active:%d",
      btm_cb.btm_inq_vars.no_inc_ssp, btm_cb.btm_inq_vars.inq_active,
      btm_cb.btm_inq_vars.state, btm_cb.btm_inq_vars.inqfilt_active);
#endif
  if (btm_cb.btm_inq_vars.no_inc_ssp) {
    if (btm_cb.btm_inq_vars.state == BTM_INQ_ACTIVE_STATE) {
      if (btm_cb.btm_inq_vars.inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) {
        BTM_CancelPeriodicInquiry();
      } else if (btm_cb.btm_inq_vars.inq_active & normal_active) {
        /* can not call BTM_CancelInquiry() here. We need to report inquiry
         * complete evt */
        btsnd_hcic_inq_cancel();
      }
    }
    /* do not allow inquiry to start */
    btm_cb.btm_inq_vars.inq_active |= BTM_SSP_INQUIRY_ACTIVE;
  }
}

/*******************************************************************************
 *
 * Function         btm_inq_clear_ssp
 *
 * Description      This function is called when pairing_state becomes idle
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_inq_clear_ssp(void) {
  btm_cb.btm_inq_vars.inq_active &= ~BTM_SSP_INQUIRY_ACTIVE;
}

/*******************************************************************************
 *
 * Function         btm_clr_inq_db
 *
 * Description      This function is called to clear out a device or all devices
 *                  from the inquiry database.
 *
 * Parameter        p_bda - (input) BD_ADDR ->  Address of device to clear
 *                                              (NULL clears all entries)
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_clr_inq_db(const RawAddress* p_bda) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  tINQ_DB_ENT* p_ent = p_inq->inq_db;
  uint16_t xx;

#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG("btm_clr_inq_db: inq_active:0x%x state:%d",
                  btm_cb.btm_inq_vars.inq_active, btm_cb.btm_inq_vars.state);
#endif
  for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
    if (p_ent->in_use) {
      /* If this is the specified BD_ADDR or clearing all devices */
      if (p_bda == NULL || (p_ent->inq_info.results.remote_bd_addr == *p_bda)) {
        p_ent->in_use = false;
      }
    }
  }
#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG("inq_active:0x%x state:%d", btm_cb.btm_inq_vars.inq_active,
                  btm_cb.btm_inq_vars.state);
#endif
}

/*******************************************************************************
 *
 * Function         btm_clr_inq_result_flt
 *
 * Description      This function looks through the bdaddr database for a match
 *                  based on Bluetooth Device Address
 *
 * Returns          true if found, else false (new entry)
 *
 ******************************************************************************/
static void btm_clr_inq_result_flt(void) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  osi_free_and_reset((void**)&p_inq->p_bd_db);
  p_inq->num_bd_entries = 0;
  p_inq->max_bd_entries = 0;
}

/*******************************************************************************
 *
 * Function         btm_inq_find_bdaddr
 *
 * Description      This function looks through the bdaddr database for a match
 *                  based on Bluetooth Device Address
 *
 * Returns          true if found, else false (new entry)
 *
 ******************************************************************************/
bool btm_inq_find_bdaddr(const RawAddress& p_bda) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  tINQ_BDADDR* p_db = &p_inq->p_bd_db[0];
  uint16_t xx;

  /* Don't bother searching, database doesn't exist or periodic mode */
  if ((p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) || !p_db)
    return (false);

  for (xx = 0; xx < p_inq->num_bd_entries; xx++, p_db++) {
    if (p_db->bd_addr == p_bda && p_db->inq_count == p_inq->inq_counter)
      return (true);
  }

  if (xx < p_inq->max_bd_entries) {
    p_db->inq_count = p_inq->inq_counter;
    p_db->bd_addr = p_bda;
    p_inq->num_bd_entries++;
  }

  /* If here, New Entry */
  return (false);
}

/*******************************************************************************
 *
 * Function         btm_inq_db_find
 *
 * Description      This function looks through the inquiry database for a match
 *                  based on Bluetooth Device Address
 *
 * Returns          pointer to entry, or NULL if not found
 *
 ******************************************************************************/
tINQ_DB_ENT* btm_inq_db_find(const RawAddress& p_bda) {
  uint16_t xx;
  tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;

  for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
    if (p_ent->in_use && p_ent->inq_info.results.remote_bd_addr == p_bda)
      return (p_ent);
  }

  /* If here, not found */
  return (NULL);
}

/*******************************************************************************
 *
 * Function         btm_inq_db_new
 *
 * Description      This function looks through the inquiry database for an
 *                  unused entry. If no entry is free, it allocates the oldest
 *                  entry.
 *
 * Returns          pointer to entry
 *
 ******************************************************************************/
tINQ_DB_ENT* btm_inq_db_new(const RawAddress& p_bda) {
  uint16_t xx;
  tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;
  tINQ_DB_ENT* p_old = btm_cb.btm_inq_vars.inq_db;
  uint64_t ot = UINT64_MAX;

  for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
    if (!p_ent->in_use) {
      memset(p_ent, 0, sizeof(tINQ_DB_ENT));
      p_ent->inq_info.results.remote_bd_addr = p_bda;
      p_ent->in_use = true;

      return (p_ent);
    }

    if (p_ent->time_of_resp < ot) {
      p_old = p_ent;
      ot = p_ent->time_of_resp;
    }
  }

  /* If here, no free entry found. Return the oldest. */

  memset(p_old, 0, sizeof(tINQ_DB_ENT));
  p_old->inq_info.results.remote_bd_addr = p_bda;
  p_old->in_use = true;

  return (p_old);
}

/*******************************************************************************
 *
 * Function         btm_set_inq_event_filter
 *
 * Description      This function is called to set the inquiry event filter.
 *                  It is called by either internally, or by the external API
 *                  function (BTM_SetInqEventFilter).  It is used internally as
 *                  part of the inquiry processing.
 *
 * Input Params:
 *                  filter_cond_type - this is the type of inquiry filter to
 *                                     apply:
 *                          BTM_FILTER_COND_DEVICE_CLASS,
 *                          BTM_FILTER_COND_BD_ADDR, or
 *                          BTM_CLR_INQUIRY_FILTER
 *
 *                  p_filt_cond - this is either a BD_ADDR or DEV_CLASS
 *                                depending on the filter_cond_type
 *                                (See section 4.7.3 of Core Spec 1.0b).
 *
 * Returns          BTM_CMD_STARTED if successfully initiated
 *                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
 *                  BTM_ILLEGAL_VALUE if a bad parameter was detected
 *
 ******************************************************************************/
static tBTM_STATUS btm_set_inq_event_filter(uint8_t filter_cond_type,
                                            tBTM_INQ_FILT_COND* p_filt_cond) {
  uint8_t condition_length = DEV_CLASS_LEN * 2;
  uint8_t condition_buf[DEV_CLASS_LEN * 2];
  uint8_t* p_cond = condition_buf; /* points to the condition to pass to HCI */

#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG(
      "btm_set_inq_event_filter: filter type %d [Clear-0, COD-1, BDADDR-2]",
      filter_cond_type);
  VLOG(2) << "condition " << p_filt_cond->bdaddr_cond;
#endif

  /* Load the correct filter condition to pass to the lower layer */
  switch (filter_cond_type) {
    case BTM_FILTER_COND_DEVICE_CLASS:
      /* copy the device class and device class fields into contiguous memory to
       * send to HCI */
      memcpy(condition_buf, p_filt_cond->cod_cond.dev_class, DEV_CLASS_LEN);
      memcpy(&condition_buf[DEV_CLASS_LEN],
             p_filt_cond->cod_cond.dev_class_mask, DEV_CLASS_LEN);

      /* condition length should already be set as the default */
      break;

    case BTM_FILTER_COND_BD_ADDR:
      p_cond = (uint8_t*)&p_filt_cond->bdaddr_cond;

      /* condition length should already be set as the default */
      break;

    case BTM_CLR_INQUIRY_FILTER:
      condition_length = 0;
      break;

    default:
      return (BTM_ILLEGAL_VALUE); /* Bad parameter was passed in */
  }

  btm_cb.btm_inq_vars.inqfilt_active = true;

  /* Filter the inquiry results for the specified condition type and value */
  btsnd_hcic_set_event_filter(HCI_FILTER_INQUIRY_RESULT, filter_cond_type,
                              p_cond, condition_length);
  return (BTM_CMD_STARTED);
}

/*******************************************************************************
 *
 * Function         btm_event_filter_complete
 *
 * Description      This function is called when a set event filter has
 *                  completed.
 *                  Note: This routine currently only handles inquiry filters.
 *                      Connection filters are ignored for now.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_event_filter_complete(uint8_t* p) {
  uint8_t hci_status;
  tBTM_STATUS status;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  tBTM_CMPL_CB* p_cb = p_inq->p_inqfilter_cmpl_cb;

#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG(
      "btm_event_filter_complete: inq_active:0x%x state:%d inqfilt_active:%d",
      btm_cb.btm_inq_vars.inq_active, btm_cb.btm_inq_vars.state,
      btm_cb.btm_inq_vars.inqfilt_active);
#endif
  /* If the filter complete event is from an old or cancelled request, ignore it
   */
  if (p_inq->pending_filt_complete_event) {
    p_inq->pending_filt_complete_event--;
    return;
  }

  /* Only process the inquiry filter; Ignore the connection filter until it
     is used by the upper layers */
  if (p_inq->inqfilt_active) {
    /* Extract the returned status from the buffer */
    STREAM_TO_UINT8(hci_status, p);
    if (hci_status != HCI_SUCCESS) {
      /* If standalone operation, return the error status; if embedded in the
       * inquiry, continue the inquiry */
      BTM_TRACE_WARNING(
          "BTM Warning: Set Event Filter Failed (HCI returned 0x%x)",
          hci_status);
      status = BTM_ERR_PROCESSING;
    } else
      status = BTM_SUCCESS;

    /* If the set filter was initiated externally (via BTM_SetInqEventFilter),
       call the
       callback function to notify the initiator that it has completed */
    if (p_inq->state == BTM_INQ_INACTIVE_STATE) {
      p_inq->inqfilt_active = false;
      if (p_cb) (*p_cb)(&status);
    } else /* An inquiry is active (the set filter command was internally
              generated),
              process the next state of the process (Set a new filter or start
              the inquiry). */
    {
      if (status != BTM_SUCCESS) {
        /* Process the inquiry complete (Error Status) */
        btm_process_inq_complete(
            BTM_ERR_PROCESSING,
            (uint8_t)(p_inq->inqparms.mode & BTM_BR_INQUIRY_MASK));

        /* btm_process_inq_complete() does not restore the following settings on
         * periodic inquiry */
        p_inq->inqfilt_active = false;
        p_inq->inq_active = BTM_INQUIRY_INACTIVE;
        p_inq->state = BTM_INQ_INACTIVE_STATE;

        return;
      }

      /* Check to see if a new filter needs to be set up */
      if (p_inq->state == BTM_INQ_CLR_FILT_STATE) {
        status = btm_set_inq_event_filter(p_inq->inqparms.filter_cond_type,
                                          &p_inq->inqparms.filter_cond);
        if (status == BTM_CMD_STARTED) {
          p_inq->state = BTM_INQ_SET_FILT_STATE;
        } else /* Error setting the filter: Call the initiator's callback
                  function to indicate a failure */
        {
          p_inq->inqfilt_active = false;

          /* Process the inquiry complete (Error Status) */
          btm_process_inq_complete(
              BTM_ERR_PROCESSING,
              (uint8_t)(p_inq->inqparms.mode & BTM_BR_INQUIRY_MASK));
        }
      } else /* Initiate the Inquiry or Periodic Inquiry */
      {
        p_inq->state = BTM_INQ_ACTIVE_STATE;
        p_inq->inqfilt_active = false;
        btm_initiate_inquiry(p_inq);
      }
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_initiate_inquiry
 *
 * Description      This function is called to start an inquiry or periodic
 *                  inquiry upon completion of the setting and/or clearing of
 *                  the inquiry filter.
 *
 * Inputs:          p_inq (btm_cb.btm_inq_vars) - pointer to saved inquiry
 *                                                information
 *                      mode - GENERAL or LIMITED inquiry
 *                      duration - length in 1.28 sec intervals
 *                                 (If '0', the inquiry is CANCELLED)
 *                      max_resps - maximum amount of devices to search for
 *                                  before ending the inquiry
 *                      filter_cond_type - BTM_CLR_INQUIRY_FILTER,
 *                                         BTM_FILTER_COND_DEVICE_CLASS, or
 *                                         BTM_FILTER_COND_BD_ADDR
 *                      filter_cond - value for the filter
 *                                   (based on filter_cond_type)
 *
 * Returns          If an error occurs the initiator's callback is called with
 *                  the error status.
 *
 ******************************************************************************/
static void btm_initiate_inquiry(tBTM_INQUIRY_VAR_ST* p_inq) {
  const LAP* lap;
  tBTM_INQ_PARMS* p_inqparms = &p_inq->inqparms;

#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG(
      "btm_initiate_inquiry: inq_active:0x%x state:%d inqfilt_active:%d",
      btm_cb.btm_inq_vars.inq_active, btm_cb.btm_inq_vars.state,
      btm_cb.btm_inq_vars.inqfilt_active);
#endif
  btm_acl_update_busy_level(BTM_BLI_INQ_EVT);

  if (p_inq->inq_active & BTM_SSP_INQUIRY_ACTIVE) {
    btm_process_inq_complete(BTM_NO_RESOURCES,
                             (uint8_t)(p_inqparms->mode & BTM_BR_INQUIRY_MASK));
    return;
  }

  /* Make sure the number of responses doesn't overflow the database
   * configuration */
  p_inqparms->max_resps = (uint8_t)((p_inqparms->max_resps <= BTM_INQ_DB_SIZE)
                                        ? p_inqparms->max_resps
                                        : BTM_INQ_DB_SIZE);

  lap = (p_inq->inq_active & BTM_LIMITED_INQUIRY_ACTIVE) ? &limited_inq_lap
                                                         : &general_inq_lap;

  if (p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) {
    btsnd_hcic_per_inq_mode(p_inq->per_max_delay, p_inq->per_min_delay, *lap,
                            p_inqparms->duration, p_inqparms->max_resps);
  } else {
    btm_clr_inq_result_flt();

    /* Allocate memory to hold bd_addrs responding */
    p_inq->p_bd_db = (tINQ_BDADDR*)osi_calloc(BT_DEFAULT_BUFFER_SIZE);
    p_inq->max_bd_entries =
        (uint16_t)(BT_DEFAULT_BUFFER_SIZE / sizeof(tINQ_BDADDR));

    btsnd_hcic_inquiry(*lap, p_inqparms->duration, 0);
  }
}

/*******************************************************************************
 *
 * Function         btm_process_inq_results
 *
 * Description      This function is called when inquiry results are received
 *                  from the device. It updates the inquiry database. If the
 *                  inquiry database is full, the oldest entry is discarded.
 *
 * Parameters       inq_res_mode - BTM_INQ_RESULT_STANDARD
 *                                 BTM_INQ_RESULT_WITH_RSSI
 *                                 BTM_INQ_RESULT_EXTENDED
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_process_inq_results(uint8_t* p, uint8_t hci_evt_len,
                             uint8_t inq_res_mode) {
  uint8_t num_resp, xx;
  RawAddress bda;
  tINQ_DB_ENT* p_i;
  tBTM_INQ_RESULTS* p_cur = NULL;
  bool is_new = true;
  bool update = false;
  int8_t i_rssi;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  tBTM_INQ_RESULTS_CB* p_inq_results_cb = p_inq->p_inq_results_cb;
  uint8_t page_scan_rep_mode = 0;
  uint8_t page_scan_per_mode = 0;
  uint8_t page_scan_mode = 0;
  uint8_t rssi = 0;
  DEV_CLASS dc;
  uint16_t clock_offset;
  uint8_t* p_eir_data = NULL;

#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG(
      "btm_process_inq_results inq_active:0x%x state:%d inqfilt_active:%d",
      btm_cb.btm_inq_vars.inq_active, btm_cb.btm_inq_vars.state,
      btm_cb.btm_inq_vars.inqfilt_active);
#endif
  /* Only process the results if the BR inquiry is still active */
  if (!(p_inq->inq_active & BTM_BR_INQ_ACTIVE_MASK)) return;

  STREAM_TO_UINT8(num_resp, p);

  if (inq_res_mode == BTM_INQ_RESULT_EXTENDED) {
    if (num_resp > 1) {
      BTM_TRACE_ERROR("btm_process_inq_results() extended results (%d) > 1",
                      num_resp);
      return;
    }

    constexpr uint16_t extended_inquiry_result_size = 254;
    if (hci_evt_len - 1 != extended_inquiry_result_size) {
      android_errorWriteLog(0x534e4554, "141620271");
      BTM_TRACE_ERROR("%s: can't fit %d results in %d bytes", __func__,
                      num_resp, hci_evt_len);
      return;
    }
  } else if (inq_res_mode == BTM_INQ_RESULT_STANDARD ||
             inq_res_mode == BTM_INQ_RESULT_WITH_RSSI) {
    constexpr uint16_t inquiry_result_size = 14;
    if (hci_evt_len < num_resp * inquiry_result_size) {
      android_errorWriteLog(0x534e4554, "141620271");
      BTM_TRACE_ERROR("%s: can't fit %d results in %d bytes", __func__,
                      num_resp, hci_evt_len);
      return;
    }
  }

  for (xx = 0; xx < num_resp; xx++) {
    update = false;
    /* Extract inquiry results */
    STREAM_TO_BDADDR(bda, p);
    STREAM_TO_UINT8(page_scan_rep_mode, p);
    STREAM_TO_UINT8(page_scan_per_mode, p);

    if (inq_res_mode == BTM_INQ_RESULT_STANDARD) {
      STREAM_TO_UINT8(page_scan_mode, p);
    }

    STREAM_TO_DEVCLASS(dc, p);
    STREAM_TO_UINT16(clock_offset, p);
    if (inq_res_mode != BTM_INQ_RESULT_STANDARD) {
      STREAM_TO_UINT8(rssi, p);
    }

    p_i = btm_inq_db_find(bda);

    /* Only process the num_resp is smaller than max_resps.
       If results are queued to BTU task while canceling inquiry,
       or when more than one result is in this response, > max_resp
       responses could be processed which can confuse some apps
    */
    if (p_inq->inqparms.max_resps &&
        p_inq->inq_cmpl_info.num_resp >= p_inq->inqparms.max_resps
        /* new device response */
        &&
        (p_i == NULL ||
         /* exisiting device with BR/EDR info */
         (p_i &&
          (p_i->inq_info.results.device_type & BT_DEVICE_TYPE_BREDR) != 0))) {
      /* BTM_TRACE_WARNING("INQ RES: Extra Response Received...ignoring"); */
      return;
    }

    /* Check if this address has already been processed for this inquiry */
    if (btm_inq_find_bdaddr(bda)) {
      /* BTM_TRACE_DEBUG("BDA seen before [%02x%02x %02x%02x %02x%02x]",
                      bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);*/
      /* By default suppose no update needed */
      i_rssi = (int8_t)rssi;

      /* If this new RSSI is higher than the last one */
      if (p_inq->inqparms.report_dup && (rssi != 0) && p_i &&
          (i_rssi > p_i->inq_info.results.rssi ||
           p_i->inq_info.results.rssi == 0
           /* BR/EDR inquiry information update */
           ||
           (p_i->inq_info.results.device_type & BT_DEVICE_TYPE_BREDR) != 0)) {
        p_cur = &p_i->inq_info.results;
        BTM_TRACE_DEBUG("update RSSI new:%d, old:%d", i_rssi, p_cur->rssi);
        p_cur->rssi = i_rssi;
        update = true;
      }
      /* If we received a second Extended Inq Event for an already */
      /* discovered device, this is because for the first one EIR was not
         received */
      else if ((inq_res_mode == BTM_INQ_RESULT_EXTENDED) && (p_i)) {
        p_cur = &p_i->inq_info.results;
        update = true;
      }
      /* If no update needed continue with next response (if any) */
      else
        continue;
    }

    /* If existing entry, use that, else get a new one (possibly reusing the
     * oldest) */
    if (p_i == NULL) {
      p_i = btm_inq_db_new(bda);
      is_new = true;
    }

    /* If an entry for the device already exists, overwrite it ONLY if it is
       from
       a previous inquiry. (Ignore it if it is a duplicate response from the
       same
       inquiry.
    */
    else if (p_i->inq_count == p_inq->inq_counter &&
             (p_i->inq_info.results.device_type == BT_DEVICE_TYPE_BREDR))
      is_new = false;

    /* keep updating RSSI to have latest value */
    if (inq_res_mode != BTM_INQ_RESULT_STANDARD)
      p_i->inq_info.results.rssi = (int8_t)rssi;
    else
      p_i->inq_info.results.rssi = BTM_INQ_RES_IGNORE_RSSI;

    if (is_new) {
      /* Save the info */
      p_cur = &p_i->inq_info.results;
      p_cur->page_scan_rep_mode = page_scan_rep_mode;
      p_cur->page_scan_per_mode = page_scan_per_mode;
      p_cur->page_scan_mode = page_scan_mode;
      p_cur->dev_class[0] = dc[0];
      p_cur->dev_class[1] = dc[1];
      p_cur->dev_class[2] = dc[2];
      p_cur->clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;

      p_i->time_of_resp = bluetooth::common::time_get_os_boottime_ms();

      if (p_i->inq_count != p_inq->inq_counter)
        p_inq->inq_cmpl_info.num_resp++; /* A new response was found */

      p_cur->inq_result_type = BTM_INQ_RESULT_BR;
      if (p_i->inq_count != p_inq->inq_counter) {
        p_cur->device_type = BT_DEVICE_TYPE_BREDR;
        p_i->scan_rsp = false;
      } else
        p_cur->device_type |= BT_DEVICE_TYPE_BREDR;
      p_i->inq_count = p_inq->inq_counter; /* Mark entry for current inquiry */

      /* If the number of responses found and not unlimited, issue a cancel
       * inquiry */
      if (!(p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) &&
          p_inq->inqparms.max_resps &&
          p_inq->inq_cmpl_info.num_resp == p_inq->inqparms.max_resps &&
          /* BLE scanning is active and received adv */
          ((((p_inq->inqparms.mode & BTM_BLE_INQUIRY_MASK) != 0) &&
            p_cur->device_type == BT_DEVICE_TYPE_DUMO && p_i->scan_rsp) ||
           (p_inq->inqparms.mode & BTM_BLE_INQUIRY_MASK) == 0)) {
        /*                BTM_TRACE_DEBUG("BTMINQ: Found devices, cancelling
         * inquiry..."); */
        btsnd_hcic_inq_cancel();

        if ((p_inq->inqparms.mode & BTM_BLE_INQUIRY_MASK) != 0)
          btm_ble_stop_inquiry();
        btm_acl_update_busy_level(BTM_BLI_INQ_DONE_EVT);
      }
      /* Initialize flag to false. This flag is set/used by application */
      p_i->inq_info.appl_knows_rem_name = false;
    }

    if (is_new || update) {
      if (inq_res_mode == BTM_INQ_RESULT_EXTENDED) {
        memset(p_cur->eir_uuid, 0,
               BTM_EIR_SERVICE_ARRAY_SIZE * (BTM_EIR_ARRAY_BITS / 8));
        /* set bit map of UUID list from received EIR */
        btm_set_eir_uuid(p, p_cur);
        p_eir_data = p;
      } else
        p_eir_data = NULL;

      /* If a callback is registered, call it with the results */
      if (p_inq_results_cb)
        (p_inq_results_cb)((tBTM_INQ_RESULTS*)p_cur, p_eir_data,
                           HCI_EXT_INQ_RESPONSE_LEN);
    }
  }
}

/*******************************************************************************
 *
 * Function         btm_sort_inq_result
 *
 * Description      This function is called when inquiry complete is received
 *                  from the device to sort inquiry results based on rssi.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_sort_inq_result(void) {
  uint8_t xx, yy, num_resp;
  tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;
  tINQ_DB_ENT* p_next = btm_cb.btm_inq_vars.inq_db + 1;
  int size;
  tINQ_DB_ENT* p_tmp = (tINQ_DB_ENT*)osi_malloc(sizeof(tINQ_DB_ENT));

  num_resp = (btm_cb.btm_inq_vars.inq_cmpl_info.num_resp < BTM_INQ_DB_SIZE)
                 ? btm_cb.btm_inq_vars.inq_cmpl_info.num_resp
                 : BTM_INQ_DB_SIZE;

  size = sizeof(tINQ_DB_ENT);
  for (xx = 0; xx < num_resp - 1; xx++, p_ent++) {
    for (yy = xx + 1, p_next = p_ent + 1; yy < num_resp; yy++, p_next++) {
      if (p_ent->inq_info.results.rssi < p_next->inq_info.results.rssi) {
        memcpy(p_tmp, p_next, size);
        memcpy(p_next, p_ent, size);
        memcpy(p_ent, p_tmp, size);
      }
    }
  }

  osi_free(p_tmp);
}

/*******************************************************************************
 *
 * Function         btm_process_inq_complete
 *
 * Description      This function is called when inquiry complete is received
 *                  from the device.  Call the callback if not in periodic
 *                  inquiry mode AND it is not NULL
 *                  (The caller wants the event).
 *
 *                  The callback pass back the status and the number of
 *                  responses
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_process_inq_complete(uint8_t status, uint8_t mode) {
  tBTM_CMPL_CB* p_inq_cb = btm_cb.btm_inq_vars.p_inq_cmpl_cb;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  p_inq->inqparms.mode &= ~(mode);

  if (p_inq->scan_type == INQ_LE_OBSERVE && !p_inq->inq_active) {
    /*end of LE observe*/
    p_inq->p_inq_ble_results_cb = NULL;
    p_inq->p_inq_ble_cmpl_cb = NULL;
    p_inq->scan_type = INQ_NONE;
  }

#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG(
      "btm_process_inq_complete inq_active:0x%x state:%d inqfilt_active:%d",
      btm_cb.btm_inq_vars.inq_active, btm_cb.btm_inq_vars.state,
      btm_cb.btm_inq_vars.inqfilt_active);
#endif
  btm_acl_update_busy_level(BTM_BLI_INQ_DONE_EVT);
  /* Ignore any stray or late complete messages if the inquiry is not active */
  if (p_inq->inq_active) {
    p_inq->inq_cmpl_info.status = (tBTM_STATUS)(
        (status == HCI_SUCCESS) ? BTM_SUCCESS : BTM_ERR_PROCESSING);

    /* Notify caller that the inquiry has completed; (periodic inquiries do not
     * send completion events */
    if (!(p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) &&
        p_inq->inqparms.mode == 0) {
      btm_clear_all_pending_le_entry();
      p_inq->state = BTM_INQ_INACTIVE_STATE;

      /* Increment so the start of a next inquiry has a new count */
      p_inq->inq_counter++;

      btm_clr_inq_result_flt();

      if ((p_inq->inq_cmpl_info.status == BTM_SUCCESS) &&
          controller_get_interface()->supports_rssi_with_inquiry_results()) {
        btm_sort_inq_result();
      }

      /* Clear the results callback if set */
      p_inq->p_inq_results_cb = NULL;
      p_inq->inq_active = BTM_INQUIRY_INACTIVE;
      p_inq->p_inq_cmpl_cb = NULL;

      /* If we have a callback registered for inquiry complete, call it */
      BTM_TRACE_DEBUG("BTM Inq Compl Callback: status 0x%02x, num results %d",
                      p_inq->inq_cmpl_info.status,
                      p_inq->inq_cmpl_info.num_resp);

      if (p_inq_cb) (p_inq_cb)((tBTM_INQUIRY_CMPL*)&p_inq->inq_cmpl_info);
    }
  }
  if (p_inq->inqparms.mode == 0 &&
      p_inq->scan_type == INQ_GENERAL)  // this inquiry is complete
  {
    p_inq->scan_type = INQ_NONE;
    /* check if the LE observe is pending */
    if (p_inq->p_inq_ble_results_cb != NULL) {
      BTM_TRACE_DEBUG("BTM Inq Compl: resuming a pending LE scan");
      BTM_BleObserve(1, 0, p_inq->p_inq_ble_results_cb,
                     p_inq->p_inq_ble_cmpl_cb);
    }
  }
#if (BTM_INQ_DEBUG == TRUE)
  BTM_TRACE_DEBUG("inq_active:0x%x state:%d inqfilt_active:%d",
                  btm_cb.btm_inq_vars.inq_active, btm_cb.btm_inq_vars.state,
                  btm_cb.btm_inq_vars.inqfilt_active);
#endif
}

/*******************************************************************************
 *
 * Function         btm_process_cancel_complete
 *
 * Description      This function is called when inquiry cancel complete is
 *                  received from the device. This function will also call the
 *                  btm_process_inq_complete. This function is needed to
 *                  differentiate a cancel_cmpl_evt from the inq_cmpl_evt.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_process_cancel_complete(uint8_t status, uint8_t mode) {
  btm_acl_update_busy_level(BTM_BLI_INQ_CANCEL_EVT);
  btm_process_inq_complete(status, mode);
}
/*******************************************************************************
 *
 * Function         btm_initiate_rem_name
 *
 * Description      This function looks initiates a remote name request.  It is
 *                  called either by GAP or by the API call
 *                  BTM_ReadRemoteDeviceName.
 *
 * Input Params:    p_cb            - callback function called when
 *                                    BTM_CMD_STARTED is returned.
 *                                    A pointer to tBTM_REMOTE_DEV_NAME is
 *                                    passed to the callback.
 *
 * Returns
 *                  BTM_CMD_STARTED is returned if the request was sent to HCI.
 *                  BTM_BUSY if already in progress
 *                  BTM_NO_RESOURCES if could not allocate resources to start
 *                                   the command
 *                  BTM_WRONG_MODE if the device is not up.
 *
 ******************************************************************************/
tBTM_STATUS btm_initiate_rem_name(const RawAddress& remote_bda, uint8_t origin,
                                  uint64_t timeout_ms, tBTM_CMPL_CB* p_cb) {
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

  /*** Make sure the device is ready ***/
  if (!BTM_IsDeviceUp()) return (BTM_WRONG_MODE);

  if (origin == BTM_RMT_NAME_SEC) {
    btsnd_hcic_rmt_name_req(remote_bda, HCI_PAGE_SCAN_REP_MODE_R1,
                            HCI_MANDATARY_PAGE_SCAN_MODE, 0);
    return BTM_CMD_STARTED;
  }
  /* Make sure there are no two remote name requests from external API in
     progress */
  else if (origin == BTM_RMT_NAME_EXT) {
    if (p_inq->remname_active) {
      return (BTM_BUSY);
    } else {
      /* If there is no remote name request running,call the callback function
       * and start timer */
      p_inq->p_remname_cmpl_cb = p_cb;
      p_inq->remname_bda = remote_bda;

      alarm_set_on_mloop(p_inq->remote_name_timer, timeout_ms,
                         btm_inq_remote_name_timer_timeout, NULL);

      /* If the database entry exists for the device, use its clock offset */
      tINQ_DB_ENT* p_i = btm_inq_db_find(remote_bda);
      if (p_i) {
        tBTM_INQ_INFO* p_cur = &p_i->inq_info;
        btsnd_hcic_rmt_name_req(
            remote_bda, p_cur->results.page_scan_rep_mode,
            p_cur->results.page_scan_mode,
            (uint16_t)(p_cur->results.clock_offset | BTM_CLOCK_OFFSET_VALID));
      } else {
        /* Otherwise use defaults and mark the clock offset as invalid */
        btsnd_hcic_rmt_name_req(remote_bda, HCI_PAGE_SCAN_REP_MODE_R1,
                                HCI_MANDATARY_PAGE_SCAN_MODE, 0);
      }

      p_inq->remname_active = true;
      return BTM_CMD_STARTED;
    }
  } else {
    return BTM_ILLEGAL_VALUE;
  }
}

/*******************************************************************************
 *
 * Function         btm_process_remote_name
 *
 * Description      This function is called when a remote name is received from
 *                  the device. If remote names are cached, it updates the
 *                  inquiry database.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_process_remote_name(const RawAddress* bda, BD_NAME bdn,
                             uint16_t evt_len, uint8_t hci_status) {
  tBTM_REMOTE_DEV_NAME rem_name;
  tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
  tBTM_CMPL_CB* p_cb = p_inq->p_remname_cmpl_cb;
  uint8_t* p_n1;

  uint16_t temp_evt_len;

  if (bda) {
    VLOG(2) << "BDA " << *bda;
  }

  VLOG(2) << "Inquire BDA " << p_inq->remname_bda;

  /* If the inquire BDA and remote DBA are the same, then stop the timer and set
   * the active to false */
  if ((p_inq->remname_active) && (!bda || (*bda == p_inq->remname_bda))) {
    if (BTM_UseLeLink(p_inq->remname_bda)) {
      if (hci_status == HCI_ERR_UNSPECIFIED)
        btm_ble_cancel_remote_name(p_inq->remname_bda);
    }
    alarm_cancel(p_inq->remote_name_timer);
    p_inq->remname_active = false;
    /* Clean up and return the status if the command was not successful */
    /* Note: If part of the inquiry, the name is not stored, and the    */
    /*       inquiry complete callback is called.                       */

    if (hci_status == HCI_SUCCESS) {
      /* Copy the name from the data stream into the return structure */
      /* Note that even if it is not being returned, it is used as a  */
      /*      temporary buffer.                                       */
      p_n1 = (uint8_t*)rem_name.remote_bd_name;
      rem_name.length = (evt_len < BD_NAME_LEN) ? evt_len : BD_NAME_LEN;
      rem_name.remote_bd_name[rem_name.length] = 0;
      rem_name.status = BTM_SUCCESS;
      temp_evt_len = rem_name.length;

      while (temp_evt_len > 0) {
        *p_n1++ = *bdn++;
        temp_evt_len--;
      }
      rem_name.remote_bd_name[rem_name.length] = 0;
    }

    /* If processing a stand alone remote name then report the error in the
       callback */
    else {
      rem_name.status = BTM_BAD_VALUE_RET;
      rem_name.length = 0;
      rem_name.remote_bd_name[0] = 0;
    }
    /* Reset the remote BAD to zero and call callback if possible */
    p_inq->remname_bda = RawAddress::kEmpty;

    p_inq->p_remname_cmpl_cb = NULL;
    if (p_cb) (p_cb)(&rem_name);
  }
}

void btm_inq_remote_name_timer_timeout(UNUSED_ATTR void* data) {
  btm_inq_rmt_name_failed();
}

/*******************************************************************************
 *
 * Function         btm_inq_rmt_name_failed
 *
 * Description      This function is if timeout expires while getting remote
 *                  name.  This is done for devices that incorrectly do not
 *                  report operation failure
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_inq_rmt_name_failed(void) {
  BTM_TRACE_ERROR("btm_inq_rmt_name_failed()  remname_active=%d",
                  btm_cb.btm_inq_vars.remname_active);

  if (btm_cb.btm_inq_vars.remname_active)
    btm_process_remote_name(&btm_cb.btm_inq_vars.remname_bda, NULL, 0,
                            HCI_ERR_UNSPECIFIED);
  else
    btm_process_remote_name(NULL, NULL, 0, HCI_ERR_UNSPECIFIED);

  btm_sec_rmt_name_request_complete(NULL, NULL, HCI_ERR_UNSPECIFIED);
}

/*******************************************************************************
 *
 * Function         btm_read_inq_tx_power_timeout
 *
 * Description      Callback when reading the inquiry tx power times out.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_inq_tx_power_timeout(UNUSED_ATTR void* data) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_inq_tx_power_cmpl_cb;
  btm_cb.devcb.p_inq_tx_power_cmpl_cb = NULL;
  if (p_cb) (*p_cb)((void*)NULL);
}

/*******************************************************************************
 *
 * Function         btm_read_inq_tx_power_complete
 *
 * Description      read inquiry tx power level complete callback function.
 *
 * Returns          void
 *
 ******************************************************************************/
void btm_read_inq_tx_power_complete(uint8_t* p) {
  tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_inq_tx_power_cmpl_cb;
  tBTM_INQ_TXPWR_RESULT result;

  BTM_TRACE_DEBUG("%s", __func__);
  alarm_cancel(btm_cb.devcb.read_inq_tx_power_timer);
  btm_cb.devcb.p_inq_tx_power_cmpl_cb = NULL;

  /* If there was a registered callback, call it */
  if (p_cb) {
    STREAM_TO_UINT8(result.hci_status, p);

    if (result.hci_status == HCI_SUCCESS) {
      result.status = BTM_SUCCESS;

      STREAM_TO_UINT8(result.tx_power, p);
      BTM_TRACE_EVENT(
          "BTM INQ TX POWER Complete: tx_power %d, hci status 0x%02x",
          result.tx_power, result.hci_status);
    } else {
      result.status = BTM_ERR_PROCESSING;
    }

    (*p_cb)(&result);
  }
}
/*******************************************************************************
 *
 * Function         BTM_WriteEIR
 *
 * Description      This function is called to write EIR data to controller.
 *
 * Parameters       p_buff - allocated HCI command buffer including extended
 *                           inquriry response
 *
 * Returns          BTM_SUCCESS  - if successful
 *                  BTM_MODE_UNSUPPORTED - if local device cannot support it
 *
 ******************************************************************************/
tBTM_STATUS BTM_WriteEIR(BT_HDR* p_buff) {
  if (controller_get_interface()->supports_extended_inquiry_response()) {
    BTM_TRACE_API("Write Extended Inquiry Response to controller");
    btsnd_hcic_write_ext_inquiry_response(p_buff, BTM_EIR_DEFAULT_FEC_REQUIRED);
    return BTM_SUCCESS;
  } else {
    osi_free(p_buff);
    return BTM_MODE_UNSUPPORTED;
  }
}

/*******************************************************************************
 *
 * Function         btm_convert_uuid_to_eir_service
 *
 * Description      This function is called to get the bit position of UUID.
 *
 * Parameters       uuid16 - UUID 16-bit
 *
 * Returns          BTM EIR service ID if found
 *                  BTM_EIR_MAX_SERVICES - if not found
 *
 ******************************************************************************/
static uint8_t btm_convert_uuid_to_eir_service(uint16_t uuid16) {
  uint8_t xx;

  for (xx = 0; xx < BTM_EIR_MAX_SERVICES; xx++) {
    if (uuid16 == BTM_EIR_UUID_LKUP_TBL[xx]) {
      return xx;
    }
  }
  return BTM_EIR_MAX_SERVICES;
}

/*******************************************************************************
 *
 * Function         BTM_HasEirService
 *
 * Description      This function is called to know if UUID in bit map of UUID.
 *
 * Parameters       p_eir_uuid - bit map of UUID list
 *                  uuid16 - UUID 16-bit
 *
 * Returns          true - if found
 *                  false - if not found
 *
 ******************************************************************************/
bool BTM_HasEirService(const uint32_t* p_eir_uuid, uint16_t uuid16) {
  uint8_t service_id;

  service_id = btm_convert_uuid_to_eir_service(uuid16);
  if (service_id < BTM_EIR_MAX_SERVICES)
    return (BTM_EIR_HAS_SERVICE(p_eir_uuid, service_id));
  else
    return (false);
}

/*******************************************************************************
 *
 * Function         BTM_HasInquiryEirService
 *
 * Description      This function is called to know if UUID in bit map of UUID
 *                  list.
 *
 * Parameters       p_results - inquiry results
 *                  uuid16 - UUID 16-bit
 *
 * Returns          BTM_EIR_FOUND - if found
 *                  BTM_EIR_NOT_FOUND - if not found and it is complete list
 *                  BTM_EIR_UNKNOWN - if not found and it is not complete list
 *
 ******************************************************************************/
tBTM_EIR_SEARCH_RESULT BTM_HasInquiryEirService(tBTM_INQ_RESULTS* p_results,
                                                uint16_t uuid16) {
  if (BTM_HasEirService(p_results->eir_uuid, uuid16)) {
    return BTM_EIR_FOUND;
  } else if (p_results->eir_complete_list) {
    return BTM_EIR_NOT_FOUND;
  } else
    return BTM_EIR_UNKNOWN;
}

/*******************************************************************************
 *
 * Function         BTM_AddEirService
 *
 * Description      This function is called to add a service in bit map of UUID
 *                  list.
 *
 * Parameters       p_eir_uuid - bit mask of UUID list for EIR
 *                  uuid16 - UUID 16-bit
 *
 * Returns          None
 *
 ******************************************************************************/
void BTM_AddEirService(uint32_t* p_eir_uuid, uint16_t uuid16) {
  uint8_t service_id;

  service_id = btm_convert_uuid_to_eir_service(uuid16);
  if (service_id < BTM_EIR_MAX_SERVICES)
    BTM_EIR_SET_SERVICE(p_eir_uuid, service_id);
}

/*******************************************************************************
 *
 * Function         BTM_RemoveEirService
 *
 * Description      This function is called to remove a service in bit map of
 *                  UUID list.
 *
 * Parameters       p_eir_uuid - bit mask of UUID list for EIR
 *                  uuid16 - UUID 16-bit
 *
 * Returns          None
 *
 ******************************************************************************/
void BTM_RemoveEirService(uint32_t* p_eir_uuid, uint16_t uuid16) {
  uint8_t service_id;

  service_id = btm_convert_uuid_to_eir_service(uuid16);
  if (service_id < BTM_EIR_MAX_SERVICES)
    BTM_EIR_CLR_SERVICE(p_eir_uuid, service_id);
}

/*******************************************************************************
 *
 * Function         BTM_GetEirSupportedServices
 *
 * Description      This function is called to get UUID list from bit map of
 *                  UUID list.
 *
 * Parameters       p_eir_uuid - bit mask of UUID list for EIR
 *                  p - reference of current pointer of EIR
 *                  max_num_uuid16 - max number of UUID can be written in EIR
 *                  num_uuid16 - number of UUID have been written in EIR
 *
 * Returns          BTM_EIR_MORE_16BITS_UUID_TYPE, if it has more than max
 *                  BTM_EIR_COMPLETE_16BITS_UUID_TYPE, otherwise
 *
 ******************************************************************************/
uint8_t BTM_GetEirSupportedServices(uint32_t* p_eir_uuid, uint8_t** p,
                                    uint8_t max_num_uuid16,
                                    uint8_t* p_num_uuid16) {
  uint8_t service_index;

  *p_num_uuid16 = 0;

  for (service_index = 0; service_index < BTM_EIR_MAX_SERVICES;
       service_index++) {
    if (BTM_EIR_HAS_SERVICE(p_eir_uuid, service_index)) {
      if (*p_num_uuid16 < max_num_uuid16) {
        UINT16_TO_STREAM(*p, BTM_EIR_UUID_LKUP_TBL[service_index]);
        (*p_num_uuid16)++;
      }
      /* if max number of UUIDs are stored and found one more */
      else {
        return BTM_EIR_MORE_16BITS_UUID_TYPE;
      }
    }
  }
  return BTM_EIR_COMPLETE_16BITS_UUID_TYPE;
}

/*******************************************************************************
 *
 * Function         BTM_GetEirUuidList
 *
 * Description      This function parses EIR and returns UUID list.
 *
 * Parameters       p_eir - EIR
 *                  eir_len - EIR len
 *                  uuid_size - Uuid::kNumBytes16, Uuid::kNumBytes32,
 *                              Uuid::kNumBytes128
 *                  p_num_uuid - return number of UUID in found list
 *                  p_uuid_list - return UUID list
 *                  max_num_uuid - maximum number of UUID to be returned
 *
 * Returns          0 - if not found
 *                  BTM_EIR_COMPLETE_16BITS_UUID_TYPE
 *                  BTM_EIR_MORE_16BITS_UUID_TYPE
 *                  BTM_EIR_COMPLETE_32BITS_UUID_TYPE
 *                  BTM_EIR_MORE_32BITS_UUID_TYPE
 *                  BTM_EIR_COMPLETE_128BITS_UUID_TYPE
 *                  BTM_EIR_MORE_128BITS_UUID_TYPE
 *
 ******************************************************************************/
uint8_t BTM_GetEirUuidList(uint8_t* p_eir, size_t eir_len, uint8_t uuid_size,
                           uint8_t* p_num_uuid, uint8_t* p_uuid_list,
                           uint8_t max_num_uuid) {
  const uint8_t* p_uuid_data;
  uint8_t type;
  uint8_t yy, xx;
  uint16_t* p_uuid16 = (uint16_t*)p_uuid_list;
  uint32_t* p_uuid32 = (uint32_t*)p_uuid_list;
  char buff[Uuid::kNumBytes128 * 2 + 1];

  p_uuid_data =
      btm_eir_get_uuid_list(p_eir, eir_len, uuid_size, p_num_uuid, &type);
  if (p_uuid_data == NULL) {
    return 0x00;
  }

  if (*p_num_uuid > max_num_uuid) {
    BTM_TRACE_WARNING("%s: number of uuid in EIR = %d, size of uuid list = %d",
                      __func__, *p_num_uuid, max_num_uuid);
    *p_num_uuid = max_num_uuid;
  }

  BTM_TRACE_DEBUG("%s: type = %02X, number of uuid = %d", __func__, type,
                  *p_num_uuid);

  if (uuid_size == Uuid::kNumBytes16) {
    for (yy = 0; yy < *p_num_uuid; yy++) {
      STREAM_TO_UINT16(*(p_uuid16 + yy), p_uuid_data);
      BTM_TRACE_DEBUG("                     0x%04X", *(p_uuid16 + yy));
    }
  } else if (uuid_size == Uuid::kNumBytes32) {
    for (yy = 0; yy < *p_num_uuid; yy++) {
      STREAM_TO_UINT32(*(p_uuid32 + yy), p_uuid_data);
      BTM_TRACE_DEBUG("                     0x%08X", *(p_uuid32 + yy));
    }
  } else if (uuid_size == Uuid::kNumBytes128) {
    for (yy = 0; yy < *p_num_uuid; yy++) {
      STREAM_TO_ARRAY16(p_uuid_list + yy * Uuid::kNumBytes128, p_uuid_data);
      for (xx = 0; xx < Uuid::kNumBytes128; xx++)
        snprintf(buff + xx * 2, sizeof(buff) - xx * 2, "%02X",
                 *(p_uuid_list + yy * Uuid::kNumBytes128 + xx));
      BTM_TRACE_DEBUG("                     0x%s", buff);
    }
  }

  return type;
}

/*******************************************************************************
 *
 * Function         btm_eir_get_uuid_list
 *
 * Description      This function searches UUID list in EIR.
 *
 * Parameters       p_eir - address of EIR
 *                  eir_len - EIR length
 *                  uuid_size - size of UUID to find
 *                  p_num_uuid - number of UUIDs found
 *                  p_uuid_list_type - EIR data type
 *
 * Returns          NULL - if UUID list with uuid_size is not found
 *                  beginning of UUID list in EIR - otherwise
 *
 ******************************************************************************/
static const uint8_t* btm_eir_get_uuid_list(uint8_t* p_eir, size_t eir_len,
                                            uint8_t uuid_size,
                                            uint8_t* p_num_uuid,
                                            uint8_t* p_uuid_list_type) {
  const uint8_t* p_uuid_data;
  uint8_t complete_type, more_type;
  uint8_t uuid_len;

  switch (uuid_size) {
    case Uuid::kNumBytes16:
      complete_type = BTM_EIR_COMPLETE_16BITS_UUID_TYPE;
      more_type = BTM_EIR_MORE_16BITS_UUID_TYPE;
      break;
    case Uuid::kNumBytes32:
      complete_type = BTM_EIR_COMPLETE_32BITS_UUID_TYPE;
      more_type = BTM_EIR_MORE_32BITS_UUID_TYPE;
      break;
    case Uuid::kNumBytes128:
      complete_type = BTM_EIR_COMPLETE_128BITS_UUID_TYPE;
      more_type = BTM_EIR_MORE_128BITS_UUID_TYPE;
      break;
    default:
      *p_num_uuid = 0;
      return NULL;
      break;
  }

  p_uuid_data = AdvertiseDataParser::GetFieldByType(p_eir, eir_len,
                                                    complete_type, &uuid_len);
  if (p_uuid_data == NULL) {
    p_uuid_data = AdvertiseDataParser::GetFieldByType(p_eir, eir_len, more_type,
                                                      &uuid_len);
    *p_uuid_list_type = more_type;
  } else {
    *p_uuid_list_type = complete_type;
  }

  *p_num_uuid = uuid_len / uuid_size;
  return p_uuid_data;
}

/*******************************************************************************
 *
 * Function         btm_convert_uuid_to_uuid16
 *
 * Description      This function converts UUID to UUID 16-bit.
 *
 * Parameters       p_uuid - address of UUID
 *                  uuid_size - size of UUID
 *
 * Returns          0 - if UUID cannot be converted to UUID 16-bit
 *                  UUID 16-bit - otherwise
 *
 ******************************************************************************/
static uint16_t btm_convert_uuid_to_uuid16(const uint8_t* p_uuid,
                                           uint8_t uuid_size) {
  static const uint8_t base_uuid[Uuid::kNumBytes128] = {
      0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
      0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint16_t uuid16 = 0;
  uint32_t uuid32;
  bool is_base_uuid;
  uint8_t xx;

  switch (uuid_size) {
    case Uuid::kNumBytes16:
      STREAM_TO_UINT16(uuid16, p_uuid);
      break;
    case Uuid::kNumBytes32:
      STREAM_TO_UINT32(uuid32, p_uuid);
      if (uuid32 < 0x10000) uuid16 = (uint16_t)uuid32;
      break;
    case Uuid::kNumBytes128:
      /* See if we can compress his UUID down to 16 or 32bit UUIDs */
      is_base_uuid = true;
      for (xx = 0; xx < Uuid::kNumBytes128 - 4; xx++) {
        if (p_uuid[xx] != base_uuid[xx]) {
          is_base_uuid = false;
          break;
        }
      }
      if (is_base_uuid) {
        if ((p_uuid[Uuid::kNumBytes128 - 1] == 0) &&
            (p_uuid[Uuid::kNumBytes128 - 2] == 0)) {
          p_uuid += (Uuid::kNumBytes128 - 4);
          STREAM_TO_UINT16(uuid16, p_uuid);
        }
      }
      break;
    default:
      BTM_TRACE_WARNING("btm_convert_uuid_to_uuid16 invalid uuid size");
      break;
  }

  return (uuid16);
}

/*******************************************************************************
 *
 * Function         btm_set_eir_uuid
 *
 * Description      This function is called to store received UUID into inquiry
 *                  result.
 *
 * Parameters       p_eir - pointer of EIR significant part
 *                  p_results - pointer of inquiry result
 *
 * Returns          None
 *
 ******************************************************************************/
void btm_set_eir_uuid(uint8_t* p_eir, tBTM_INQ_RESULTS* p_results) {
  const uint8_t* p_uuid_data;
  uint8_t num_uuid;
  uint16_t uuid16;
  uint8_t yy;
  uint8_t type = BTM_EIR_MORE_16BITS_UUID_TYPE;

  p_uuid_data = btm_eir_get_uuid_list(p_eir, HCI_EXT_INQ_RESPONSE_LEN,
                                      Uuid::kNumBytes16, &num_uuid, &type);

  if (type == BTM_EIR_COMPLETE_16BITS_UUID_TYPE) {
    p_results->eir_complete_list = true;
  } else {
    p_results->eir_complete_list = false;
  }

  BTM_TRACE_API("btm_set_eir_uuid eir_complete_list=0x%02X",
                p_results->eir_complete_list);

  if (p_uuid_data) {
    for (yy = 0; yy < num_uuid; yy++) {
      STREAM_TO_UINT16(uuid16, p_uuid_data);
      BTM_AddEirService(p_results->eir_uuid, uuid16);
    }
  }

  p_uuid_data = btm_eir_get_uuid_list(p_eir, HCI_EXT_INQ_RESPONSE_LEN,
                                      Uuid::kNumBytes32, &num_uuid, &type);
  if (p_uuid_data) {
    for (yy = 0; yy < num_uuid; yy++) {
      uuid16 = btm_convert_uuid_to_uuid16(p_uuid_data, Uuid::kNumBytes32);
      p_uuid_data += Uuid::kNumBytes32;
      if (uuid16) BTM_AddEirService(p_results->eir_uuid, uuid16);
    }
  }

  p_uuid_data = btm_eir_get_uuid_list(p_eir, HCI_EXT_INQ_RESPONSE_LEN,
                                      Uuid::kNumBytes128, &num_uuid, &type);
  if (p_uuid_data) {
    for (yy = 0; yy < num_uuid; yy++) {
      uuid16 = btm_convert_uuid_to_uuid16(p_uuid_data, Uuid::kNumBytes128);
      p_uuid_data += Uuid::kNumBytes128;
      if (uuid16) BTM_AddEirService(p_results->eir_uuid, uuid16);
    }
  }
}
