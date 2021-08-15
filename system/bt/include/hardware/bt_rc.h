/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef ANDROID_INCLUDE_BT_RC_H
#define ANDROID_INCLUDE_BT_RC_H

__BEGIN_DECLS

/* Change this macro to use multiple RC */
#define BT_RC_NUM_APP 6

/* Macros */
#define BTRC_MAX_ATTR_STR_LEN (1 << 16)
#define BTRC_UID_SIZE 8
#define BTRC_MAX_APP_SETTINGS 8
#define BTRC_MAX_FOLDER_DEPTH 4
#define BTRC_MAX_APP_ATTR_SIZE 16
#define BTRC_MAX_ELEM_ATTR_SIZE 8
#define BTRC_FEATURE_BIT_MASK_SIZE 16

/* Macros for valid scopes in get_folder_items */
#define BTRC_SCOPE_PLAYER_LIST 0x00 /* Media Player List */
#define BTRC_SCOPE_FILE_SYSTEM 0x01 /* Virtual File System */
#define BTRC_SCOPE_SEARCH 0x02      /* Search */
#define BTRC_SCOPE_NOW_PLAYING 0x03 /* Now Playing */

/* Macros for supported character encoding */
#define BTRC_CHARSET_ID_UTF8 0x006A

/* Macros for item types */
#define BTRC_ITEM_PLAYER 0x01 /* Media Player */
#define BTRC_ITEM_FOLDER 0x02 /* Folder */
#define BTRC_ITEM_MEDIA 0x03  /* Media File */

/* Macros for media attribute IDs */
#define BTRC_MEDIA_ATTR_ID_INVALID -1
#define BTRC_MEDIA_ATTR_ID_TITLE 0x00000001
#define BTRC_MEDIA_ATTR_ID_ARTIST 0x00000002
#define BTRC_MEDIA_ATTR_ID_ALBUM 0x00000003
#define BTRC_MEDIA_ATTR_ID_TRACK_NUM 0x00000004
#define BTRC_MEDIA_ATTR_ID_NUM_TRACKS 0x00000005
#define BTRC_MEDIA_ATTR_ID_GENRE 0x00000006
#define BTRC_MEDIA_ATTR_ID_PLAYING_TIME 0x00000007 /* in miliseconds */

/* Macros for folder types */
#define BTRC_FOLDER_TYPE_MIXED 0x00
#define BTRC_FOLDER_TYPE_TITLES 0x01
#define BTRC_FOLDER_TYPE_ALBUMS 0x02
#define BTRC_FOLDER_TYPE_ARTISTS 0x03
#define BTRC_FOLDER_TYPE_GENRES 0x04
#define BTRC_FOLDER_TYPE_PLAYLISTS 0x05
#define BTRC_FOLDER_TYPE_YEARS 0x06

/* Macros for media types */
#define BTRC_MEDIA_TYPE_AUDIO 0x00 /* audio */
#define BTRC_MEDIA_TYPE_VIDEO 0x01 /* video */

/* Macros for num attributes */
#define BTRC_NUM_ATTR_NONE 0xFF /* No attributes required */
#define BTRC_NUM_ATTR_ALL 0X00  /* All attributes required */

#define BTRC_HANDLE_NONE 0xFF

typedef uint8_t btrc_uid_t[BTRC_UID_SIZE];

typedef enum {
  BTRC_CONNECTION_STATE_DISCONNECTED = 0,
  BTRC_CONNECTION_STATE_CONNECTED
} btrc_connection_state_t;

typedef enum {
  BTRC_FEAT_NONE = 0x00,            /* AVRCP 1.0 */
  BTRC_FEAT_METADATA = 0x01,        /* AVRCP 1.3 */
  BTRC_FEAT_ABSOLUTE_VOLUME = 0x02, /* Supports TG role and volume sync */
  BTRC_FEAT_BROWSE = 0x04, /* AVRCP 1.4 and up, with Browsing support */
} btrc_remote_features_t;

typedef enum {
  BTRC_PLAYSTATE_STOPPED = 0x00,  /* Stopped */
  BTRC_PLAYSTATE_PLAYING = 0x01,  /* Playing */
  BTRC_PLAYSTATE_PAUSED = 0x02,   /* Paused  */
  BTRC_PLAYSTATE_FWD_SEEK = 0x03, /* Fwd Seek*/
  BTRC_PLAYSTATE_REV_SEEK = 0x04, /* Rev Seek*/
  BTRC_PLAYSTATE_ERROR = 0xFF,    /* Error   */
} btrc_play_status_t;

typedef enum {
  BTRC_EVT_PLAY_STATUS_CHANGED = 0x01,
  BTRC_EVT_TRACK_CHANGE = 0x02,
  BTRC_EVT_TRACK_REACHED_END = 0x03,
  BTRC_EVT_TRACK_REACHED_START = 0x04,
  BTRC_EVT_PLAY_POS_CHANGED = 0x05,
  BTRC_EVT_APP_SETTINGS_CHANGED = 0x08,
  BTRC_EVT_NOW_PLAYING_CONTENT_CHANGED = 0x09,
  BTRC_EVT_AVAL_PLAYER_CHANGE = 0x0a,
  BTRC_EVT_ADDR_PLAYER_CHANGE = 0x0b,
  BTRC_EVT_UIDS_CHANGED = 0x0c,
  BTRC_EVT_VOL_CHANGED = 0x0d,
} btrc_event_id_t;

typedef enum {
  BTRC_NOTIFICATION_TYPE_INTERIM = 0,
  BTRC_NOTIFICATION_TYPE_CHANGED = 1,
} btrc_notification_type_t;

typedef enum {
  BTRC_PLAYER_ATTR_EQUALIZER = 0x01,
  BTRC_PLAYER_ATTR_REPEAT = 0x02,
  BTRC_PLAYER_ATTR_SHUFFLE = 0x03,
  BTRC_PLAYER_ATTR_SCAN = 0x04,
} btrc_player_attr_t;

typedef enum {
  BTRC_MEDIA_ATTR_TITLE = 0x01,
  BTRC_MEDIA_ATTR_ARTIST = 0x02,
  BTRC_MEDIA_ATTR_ALBUM = 0x03,
  BTRC_MEDIA_ATTR_TRACK_NUM = 0x04,
  BTRC_MEDIA_ATTR_NUM_TRACKS = 0x05,
  BTRC_MEDIA_ATTR_GENRE = 0x06,
  BTRC_MEDIA_ATTR_PLAYING_TIME = 0x07,
} btrc_media_attr_t;

typedef enum {
  BTRC_PLAYER_VAL_OFF_REPEAT = 0x01,
  BTRC_PLAYER_VAL_SINGLE_REPEAT = 0x02,
  BTRC_PLAYER_VAL_ALL_REPEAT = 0x03,
  BTRC_PLAYER_VAL_GROUP_REPEAT = 0x04
} btrc_player_repeat_val_t;

typedef enum {
  BTRC_PLAYER_VAL_OFF_SHUFFLE = 0x01,
  BTRC_PLAYER_VAL_ALL_SHUFFLE = 0x02,
  BTRC_PLAYER_VAL_GROUP_SHUFFLE = 0x03
} btrc_player_shuffle_val_t;

typedef enum {
  BTRC_STS_BAD_CMD = 0x00,       /* Invalid command */
  BTRC_STS_BAD_PARAM = 0x01,     /* Invalid parameter */
  BTRC_STS_NOT_FOUND = 0x02,     /* Specified parameter is wrong or not found */
  BTRC_STS_INTERNAL_ERR = 0x03,  /* Internal Error */
  BTRC_STS_NO_ERROR = 0x04,      /* Operation Success */
  BTRC_STS_UID_CHANGED = 0x05,   /* UIDs changed */
  BTRC_STS_RESERVED = 0x06,      /* Reserved */
  BTRC_STS_INV_DIRN = 0x07,      /* Invalid direction */
  BTRC_STS_INV_DIRECTORY = 0x08, /* Invalid directory */
  BTRC_STS_INV_ITEM = 0x09,      /* Invalid Item */
  BTRC_STS_INV_SCOPE = 0x0a,     /* Invalid scope */
  BTRC_STS_INV_RANGE = 0x0b,     /* Invalid range */
  BTRC_STS_DIRECTORY = 0x0c,     /* UID is a directory */
  BTRC_STS_MEDIA_IN_USE = 0x0d,  /* Media in use */
  BTRC_STS_PLAY_LIST_FULL = 0x0e, /* Playing list full */
  BTRC_STS_SRCH_NOT_SPRTD = 0x0f, /* Search not supported */
  BTRC_STS_SRCH_IN_PROG = 0x10,   /* Search in progress */
  BTRC_STS_INV_PLAYER = 0x11,     /* Invalid player */
  BTRC_STS_PLAY_NOT_BROW = 0x12,  /* Player not browsable */
  BTRC_STS_PLAY_NOT_ADDR = 0x13,  /* Player not addressed */
  BTRC_STS_INV_RESULTS = 0x14,    /* Invalid results */
  BTRC_STS_NO_AVBL_PLAY = 0x15,   /* No available players */
  BTRC_STS_ADDR_PLAY_CHGD = 0x16, /* Addressed player changed */
} btrc_status_t;

typedef struct {
  uint16_t player_id;
  uint16_t uid_counter;
} btrc_addr_player_changed_t;

typedef struct {
  uint8_t num_attr;
  uint8_t attr_ids[BTRC_MAX_APP_SETTINGS];
  uint8_t attr_values[BTRC_MAX_APP_SETTINGS];
} btrc_player_settings_t;

typedef struct {
  uint8_t val;
  uint16_t charset_id;
  uint16_t str_len;
  uint8_t* p_str;
} btrc_player_app_ext_attr_val_t;

typedef struct {
  uint8_t attr_id;
  uint16_t charset_id;
  uint16_t str_len;
  uint8_t* p_str;
  uint8_t num_val;
  btrc_player_app_ext_attr_val_t ext_attr_val[BTRC_MAX_APP_ATTR_SIZE];
} btrc_player_app_ext_attr_t;

typedef struct {
  uint8_t attr_id;
  uint8_t num_val;
  uint8_t attr_val[BTRC_MAX_APP_ATTR_SIZE];
} btrc_player_app_attr_t;

typedef struct {
  uint32_t start_item;
  uint32_t end_item;
  uint32_t size;
  uint32_t attrs[BTRC_MAX_ELEM_ATTR_SIZE];
  uint8_t attr_count;
} btrc_getfolderitem_t;

typedef struct {
  uint16_t type;
  uint16_t uid_counter;
} btrc_uids_changed_t;

typedef struct { uint16_t type; } btrc_now_playing_changed_t;

typedef union {
  btrc_play_status_t play_status;
  btrc_uid_t track; /* queue position in NowPlaying */
  uint32_t song_pos;
  uint16_t uid_counter;
  btrc_player_settings_t player_setting;
  btrc_addr_player_changed_t addr_player_changed;
  btrc_uids_changed_t uids_changed;
  btrc_now_playing_changed_t now_playing_changed;
} btrc_register_notification_t;

typedef struct {
  uint8_t id; /* can be attr_id or value_id */
  uint8_t text[BTRC_MAX_ATTR_STR_LEN];
} btrc_player_setting_text_t;

typedef struct {
  uint32_t attr_id;
  uint8_t text[BTRC_MAX_ATTR_STR_LEN];
} btrc_element_attr_val_t;

typedef struct {
  uint16_t player_id;
  uint8_t major_type;
  uint32_t sub_type;
  uint8_t play_status;
  uint8_t features[BTRC_FEATURE_BIT_MASK_SIZE];
  uint16_t charset_id;
  uint8_t name[BTRC_MAX_ATTR_STR_LEN];
} btrc_item_player_t;

typedef struct {
  uint8_t uid[BTRC_UID_SIZE];
  uint8_t type;
  uint8_t playable;
  uint16_t charset_id;
  uint8_t name[BTRC_MAX_ATTR_STR_LEN];
} btrc_item_folder_t;

typedef struct {
  uint8_t uid[BTRC_UID_SIZE];
  uint8_t type;
  uint16_t charset_id;
  uint8_t name[BTRC_MAX_ATTR_STR_LEN];
  int num_attrs;
  btrc_element_attr_val_t* p_attrs;
} btrc_item_media_t;

typedef struct {
  uint8_t item_type;
  union {
    btrc_item_player_t player;
    btrc_item_folder_t folder;
    btrc_item_media_t media;
  };
} btrc_folder_items_t;

typedef struct {
  uint16_t str_len;
  uint8_t p_str[BTRC_MAX_ATTR_STR_LEN];
} btrc_br_folder_name_t;

/** Callback for the controller's supported feautres */
typedef void (*btrc_remote_features_callback)(const RawAddress& bd_addr,
                                              btrc_remote_features_t features);

/** Callback for play status request */
typedef void (*btrc_get_play_status_callback)(const RawAddress& bd_addr);

/** Callback for list player application attributes (Shuffle, Repeat,...) */
typedef void (*btrc_list_player_app_attr_callback)(const RawAddress& bd_addr);

/** Callback for list player application attributes (Shuffle, Repeat,...) */
typedef void (*btrc_list_player_app_values_callback)(btrc_player_attr_t attr_id,
                                                     const RawAddress& bd_addr);

/** Callback for getting the current player application settings value
**  num_attr: specifies the number of attribute ids contained in p_attrs
*/
typedef void (*btrc_get_player_app_value_callback)(uint8_t num_attr,
                                                   btrc_player_attr_t* p_attrs,
                                                   const RawAddress& bd_addr);

/** Callback for getting the player application settings attributes' text
**  num_attr: specifies the number of attribute ids contained in p_attrs
*/
typedef void (*btrc_get_player_app_attrs_text_callback)(
    uint8_t num_attr, btrc_player_attr_t* p_attrs, const RawAddress& bd_addr);

/** Callback for getting the player application settings values' text
**  num_attr: specifies the number of value ids contained in p_vals
*/
typedef void (*btrc_get_player_app_values_text_callback)(
    uint8_t attr_id, uint8_t num_val, uint8_t* p_vals,
    const RawAddress& bd_addr);

/** Callback for setting the player application settings values */
typedef void (*btrc_set_player_app_value_callback)(
    btrc_player_settings_t* p_vals, const RawAddress& bd_addr);

/** Callback to fetch the get element attributes of the current song
**  num_attr: specifies the number of attributes requested in p_attrs
*/
typedef void (*btrc_get_element_attr_callback)(uint8_t num_attr,
                                               btrc_media_attr_t* p_attrs,
                                               const RawAddress& bd_addr);

/** Callback for register notification (Play state change/track change/...)
**  param: Is only valid if event_id is BTRC_EVT_PLAY_POS_CHANGED
*/
typedef void (*btrc_register_notification_callback)(btrc_event_id_t event_id,
                                                    uint32_t param,
                                                    const RawAddress& bd_addr);

/* AVRCP 1.4 Enhancements */
/** Callback for volume change on CT
**  volume: Current volume setting on the CT (0-127)
*/
typedef void (*btrc_volume_change_callback)(uint8_t volume, uint8_t ctype,
                                            const RawAddress& bd_addr);

/** Callback for passthrough commands */
typedef void (*btrc_passthrough_cmd_callback)(int id, int key_state,
                                              const RawAddress& bd_addr);

/** Callback for set addressed player response on TG **/
typedef void (*btrc_set_addressed_player_callback)(uint16_t player_id,
                                                   const RawAddress& bd_addr);

/** Callback for set browsed player response on TG **/
typedef void (*btrc_set_browsed_player_callback)(uint16_t player_id,
                                                 const RawAddress& bd_addr);

/** Callback for get folder items on TG
**  num_attr: specifies the number of attributes requested in p_attr_ids
*/
typedef void (*btrc_get_folder_items_callback)(
    uint8_t scope, uint32_t start_item, uint32_t end_item, uint8_t num_attr,
    uint32_t* p_attr_ids, const RawAddress& bd_addr);

/** Callback for changing browsed path on TG **/
typedef void (*btrc_change_path_callback)(uint8_t direction,
                                          uint8_t* folder_uid,
                                          const RawAddress& bd_addr);

/** Callback to fetch the get item attributes of the media item
**  num_attr: specifies the number of attributes requested in p_attrs
*/
typedef void (*btrc_get_item_attr_callback)(uint8_t scope, uint8_t* uid,
                                            uint16_t uid_counter,
                                            uint8_t num_attr,
                                            btrc_media_attr_t* p_attrs,
                                            const RawAddress& bd_addr);

/** Callback for play request for the media item indicated by an identifier */
typedef void (*btrc_play_item_callback)(uint8_t scope, uint16_t uid_counter,
                                        uint8_t* uid,
                                        const RawAddress& bd_addr);

/** Callback to fetch total number of items from a folder **/
typedef void (*btrc_get_total_num_of_items_callback)(uint8_t scope,
                                                     const RawAddress& bd_addr);

/** Callback for conducting recursive search on a current browsed path for a
 * specified string */
typedef void (*btrc_search_callback)(uint16_t charset_id, uint16_t str_len,
                                     uint8_t* p_str, const RawAddress& bd_addr);

/** Callback to add a specified media item indicated by an identifier to now
 * playing queue. */
typedef void (*btrc_add_to_now_playing_callback)(uint8_t scope, uint8_t* uid,
                                                 uint16_t uid_counter,
                                                 const RawAddress& bd_addr);

/** BT-RC Target callback structure. */
typedef struct {
  /** set to sizeof(BtRcCallbacks) */
  size_t size;
  btrc_remote_features_callback remote_features_cb;
  btrc_get_play_status_callback get_play_status_cb;
  btrc_list_player_app_attr_callback list_player_app_attr_cb;
  btrc_list_player_app_values_callback list_player_app_values_cb;
  btrc_get_player_app_value_callback get_player_app_value_cb;
  btrc_get_player_app_attrs_text_callback get_player_app_attrs_text_cb;
  btrc_get_player_app_values_text_callback get_player_app_values_text_cb;
  btrc_set_player_app_value_callback set_player_app_value_cb;
  btrc_get_element_attr_callback get_element_attr_cb;
  btrc_register_notification_callback register_notification_cb;
  btrc_volume_change_callback volume_change_cb;
  btrc_passthrough_cmd_callback passthrough_cmd_cb;
  btrc_set_addressed_player_callback set_addressed_player_cb;
  btrc_set_browsed_player_callback set_browsed_player_cb;
  btrc_get_folder_items_callback get_folder_items_cb;
  btrc_change_path_callback change_path_cb;
  btrc_get_item_attr_callback get_item_attr_cb;
  btrc_play_item_callback play_item_cb;
  btrc_get_total_num_of_items_callback get_total_num_of_items_cb;
  btrc_search_callback search_cb;
  btrc_add_to_now_playing_callback add_to_now_playing_cb;
} btrc_callbacks_t;

/** Represents the standard BT-RC AVRCP Target interface. */
typedef struct {
  /** set to sizeof(BtRcInterface) */
  size_t size;
  /**
   * Register the BtRc callbacks
   */
  bt_status_t (*init)(btrc_callbacks_t* callbacks);

  /** Respose to GetPlayStatus request. Contains the current
  **  1. Play status
  **  2. Song duration/length
  **  3. Song position
  */
  bt_status_t (*get_play_status_rsp)(const RawAddress& bd_addr,
                                     btrc_play_status_t play_status,
                                     uint32_t song_len, uint32_t song_pos);

  /** Lists the support player application attributes (Shuffle/Repeat/...)
  **  num_attr: Specifies the number of attributes contained in the pointer
  *p_attrs
  */
  bt_status_t (*list_player_app_attr_rsp)(const RawAddress& bd_addr,
                                          int num_attr,
                                          btrc_player_attr_t* p_attrs);

  /** Lists the support player application attributes (Shuffle Off/On/Group)
  **  num_val: Specifies the number of values contained in the pointer p_vals
  */
  bt_status_t (*list_player_app_value_rsp)(const RawAddress& bd_addr,
                                           int num_val, uint8_t* p_vals);

  /** Returns the current application attribute values for each of the specified
   * attr_id */
  bt_status_t (*get_player_app_value_rsp)(const RawAddress& bd_addr,
                                          btrc_player_settings_t* p_vals);

  /** Returns the application attributes text ("Shuffle"/"Repeat"/...)
  **  num_attr: Specifies the number of attributes' text contained in the
  *pointer p_attrs
  */
  bt_status_t (*get_player_app_attr_text_rsp)(
      const RawAddress& bd_addr, int num_attr,
      btrc_player_setting_text_t* p_attrs);

  /** Returns the application attributes text ("Shuffle"/"Repeat"/...)
  **  num_attr: Specifies the number of attribute values' text contained in the
  *pointer p_vals
  */
  bt_status_t (*get_player_app_value_text_rsp)(
      const RawAddress& bd_addr, int num_val,
      btrc_player_setting_text_t* p_vals);

  /** Returns the current songs' element attributes text
   *("Title"/"Album"/"Artist")
   **  num_attr: Specifies the number of attributes' text contained in the
   *pointer p_attrs
   */
  bt_status_t (*get_element_attr_rsp)(const RawAddress& bd_addr,
                                      uint8_t num_attr,
                                      btrc_element_attr_val_t* p_attrs);

  /** Response to set player attribute request ("Shuffle"/"Repeat")
  **  rsp_status: Status of setting the player attributes for the current media
  *player
  */
  bt_status_t (*set_player_app_value_rsp)(const RawAddress& bd_addr,
                                          btrc_status_t rsp_status);

  /* Response to the register notification request (Play state change/track
   *change/...).
   ** event_id: Refers to the event_id this notification change corresponds too
   ** type: Response type - interim/changed
   ** p_params: Based on the event_id, this parameter should be populated
   */
  bt_status_t (*register_notification_rsp)(
      btrc_event_id_t event_id, btrc_notification_type_t type,
      btrc_register_notification_t* p_param);

  /* AVRCP 1.4 enhancements */

  /**Send current volume setting to remote side. Support limited to
   *SetAbsoluteVolume
   ** This can be enhanced to support Relative Volume (AVRCP 1.0).
   ** With RelateVolume, we will send VOLUME_UP/VOLUME_DOWN opposed to absolute
   *volume level
   ** volume: Should be in the range 0-127. bit7 is reseved and cannot be set
   */
  bt_status_t (*set_volume)(uint8_t volume);

  /* Set addressed player response from TG to CT */
  bt_status_t (*set_addressed_player_rsp)(const RawAddress& bd_addr,
                                          btrc_status_t rsp_status);

  /* Set browsed player response from TG to CT */
  bt_status_t (*set_browsed_player_rsp)(const RawAddress& bd_addr,
                                        btrc_status_t rsp_status,
                                        uint32_t num_items, uint16_t charset_id,
                                        uint8_t folder_depth,
                                        btrc_br_folder_name_t* p_folders);

  /* Get folder item list response from TG to CT */
  bt_status_t (*get_folder_items_list_rsp)(const RawAddress& bd_addr,
                                           btrc_status_t rsp_status,
                                           uint16_t uid_counter,
                                           uint8_t num_items,
                                           btrc_folder_items_t* p_items);

  /* Change path response from TG to CT */
  bt_status_t (*change_path_rsp)(const RawAddress& bd_addr,
                                 btrc_status_t rsp_status, uint32_t num_items);

  /** Returns the element's attributes num_attr: Specifies the number of
   * attributes' text contained in the pointer p_attrs
   */
  bt_status_t (*get_item_attr_rsp)(const RawAddress& bd_addr,
                                   btrc_status_t rsp_status, uint8_t num_attr,
                                   btrc_element_attr_val_t* p_attrs);

  /* play media item response from TG to CT */
  bt_status_t (*play_item_rsp)(const RawAddress& bd_addr,
                               btrc_status_t rsp_status);

  /* get total number of items response from TG to CT*/
  bt_status_t (*get_total_num_of_items_rsp)(const RawAddress& bd_addr,
                                            btrc_status_t rsp_status,
                                            uint32_t uid_counter,
                                            uint32_t num_items);

  /* Search VFS response from TG to CT */
  bt_status_t (*search_rsp)(const RawAddress& bd_addr, btrc_status_t rsp_status,
                            uint32_t uid_counter, uint32_t num_items);

  /* add_to_now playing list response from TG to CT */
  bt_status_t (*add_to_now_playing_rsp)(const RawAddress& bd_addr,
                                        btrc_status_t rsp_status);

  /** Closes the interface. */
  void (*cleanup)(void);
} btrc_interface_t;

typedef void (*btrc_passthrough_rsp_callback)(const RawAddress& bd_addr, int id,
                                              int key_state);

typedef void (*btrc_groupnavigation_rsp_callback)(int id, int key_state);

typedef void (*btrc_connection_state_callback)(bool rc_connect, bool bt_connect,
                                               const RawAddress& bd_addr);

typedef void (*btrc_ctrl_getrcfeatures_callback)(const RawAddress& bd_addr,
                                                 int features);

typedef void (*btrc_ctrl_setabsvol_cmd_callback)(const RawAddress& bd_addr,
                                                 uint8_t abs_vol,
                                                 uint8_t label);

typedef void (*btrc_ctrl_registernotification_abs_vol_callback)(
    const RawAddress& bd_addr, uint8_t label);

typedef void (*btrc_ctrl_setplayerapplicationsetting_rsp_callback)(
    const RawAddress& bd_addr, uint8_t accepted);

typedef void (*btrc_ctrl_playerapplicationsetting_callback)(
    const RawAddress& bd_addr, uint8_t num_attr,
    btrc_player_app_attr_t* app_attrs, uint8_t num_ext_attr,
    btrc_player_app_ext_attr_t* ext_attrs);

typedef void (*btrc_ctrl_playerapplicationsetting_changed_callback)(
    const RawAddress& bd_addr, const btrc_player_settings_t& vals);

typedef void (*btrc_ctrl_track_changed_callback)(
    const RawAddress& bd_addr, uint8_t num_attr,
    btrc_element_attr_val_t* p_attrs);

typedef void (*btrc_ctrl_play_position_changed_callback)(
    const RawAddress& bd_addr, uint32_t song_len, uint32_t song_pos);

typedef void (*btrc_ctrl_play_status_changed_callback)(
    const RawAddress& bd_addr, btrc_play_status_t play_status);

typedef void (*btrc_ctrl_get_folder_items_callback)(
    const RawAddress& bd_addr, btrc_status_t status,
    const btrc_folder_items_t* folder_items, uint8_t count);

typedef void (*btrc_ctrl_change_path_callback)(const RawAddress& bd_addr,
                                               uint32_t count);

typedef void (*btrc_ctrl_set_browsed_player_callback)(const RawAddress& bd_addr,
                                                      uint8_t num_items,
                                                      uint8_t depth);
typedef void (*btrc_ctrl_set_addressed_player_callback)(
    const RawAddress& bd_addr, uint8_t status);
typedef void (*btrc_ctrl_addressed_player_changed_callback)(
    const RawAddress& bd_addr, uint16_t id);
typedef void (*btrc_ctrl_now_playing_contents_changed_callback)(
    const RawAddress& bd_addr);

/** BT-RC Controller callback structure. */
typedef struct {
  /** set to sizeof(BtRcCallbacks) */
  size_t size;
  btrc_passthrough_rsp_callback passthrough_rsp_cb;
  btrc_groupnavigation_rsp_callback groupnavigation_rsp_cb;
  btrc_connection_state_callback connection_state_cb;
  btrc_ctrl_getrcfeatures_callback getrcfeatures_cb;
  btrc_ctrl_setplayerapplicationsetting_rsp_callback setplayerappsetting_rsp_cb;
  btrc_ctrl_playerapplicationsetting_callback playerapplicationsetting_cb;
  btrc_ctrl_playerapplicationsetting_changed_callback
      playerapplicationsetting_changed_cb;
  btrc_ctrl_setabsvol_cmd_callback setabsvol_cmd_cb;
  btrc_ctrl_registernotification_abs_vol_callback
      registernotification_absvol_cb;
  btrc_ctrl_track_changed_callback track_changed_cb;
  btrc_ctrl_play_position_changed_callback play_position_changed_cb;
  btrc_ctrl_play_status_changed_callback play_status_changed_cb;
  btrc_ctrl_get_folder_items_callback get_folder_items_cb;
  btrc_ctrl_change_path_callback change_folder_path_cb;
  btrc_ctrl_set_browsed_player_callback set_browsed_player_cb;
  btrc_ctrl_set_addressed_player_callback set_addressed_player_cb;
  btrc_ctrl_addressed_player_changed_callback addressed_player_changed_cb;
  btrc_ctrl_now_playing_contents_changed_callback
      now_playing_contents_changed_cb;
} btrc_ctrl_callbacks_t;

/** Represents the standard BT-RC AVRCP Controller interface. */
typedef struct {
  /** set to sizeof(BtRcInterface) */
  size_t size;
  /**
   * Register the BtRc callbacks
   */
  bt_status_t (*init)(btrc_ctrl_callbacks_t* callbacks);

  /** send pass through command to target */
  bt_status_t (*send_pass_through_cmd)(const RawAddress& bd_addr,
                                       uint8_t key_code, uint8_t key_state);

  /** send group navigation command to target */
  bt_status_t (*send_group_navigation_cmd)(const RawAddress& bd_addr,
                                           uint8_t key_code, uint8_t key_state);

  /** send command to set player applicaiton setting attributes to target */
  bt_status_t (*set_player_app_setting_cmd)(const RawAddress& bd_addr,
                                            uint8_t num_attrib,
                                            uint8_t* attrib_ids,
                                            uint8_t* attrib_vals);

  /** send command to play a particular item */
  bt_status_t (*play_item_cmd)(const RawAddress& bd_addr, uint8_t scope,
                               uint8_t* uid, uint16_t uid_counter);

  /** get the playback state */
  bt_status_t (*get_playback_state_cmd)(const RawAddress& bd_addr);

  /** get the now playing list */
  bt_status_t (*get_now_playing_list_cmd)(const RawAddress& bd_addr,
                                          uint32_t start, uint32_t end);

  /** get the folder list */
  bt_status_t (*get_folder_list_cmd)(const RawAddress& bd_addr, uint32_t start,
                                     uint32_t end);

  /** get the player list */
  bt_status_t (*get_player_list_cmd)(const RawAddress& bd_addr, uint32_t start,
                                     uint32_t end);

  /** change the folder path */
  bt_status_t (*change_folder_path_cmd)(const RawAddress& bd_addr,
                                        uint8_t direction, uint8_t* uid);

  /** set browsed player */
  bt_status_t (*set_browsed_player_cmd)(const RawAddress& bd_addr,
                                        uint16_t player_id);

  /** set addressed player */
  bt_status_t (*set_addressed_player_cmd)(const RawAddress& bd_addr,
                                          uint16_t player_id);

  /** send rsp to set_abs_vol received from target */
  bt_status_t (*set_volume_rsp)(const RawAddress& bd_addr, uint8_t abs_vol,
                                uint8_t label);

  /** send notificaiton rsp for abs vol to target */
  bt_status_t (*register_abs_vol_rsp)(const RawAddress& bd_addr,
                                      btrc_notification_type_t rsp_type,
                                      uint8_t abs_vol, uint8_t label);

  /** Closes the interface. */
  void (*cleanup)(void);
} btrc_ctrl_interface_t;

__END_DECLS

#endif /* ANDROID_INCLUDE_BT_RC_H */
