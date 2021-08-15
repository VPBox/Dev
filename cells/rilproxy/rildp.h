
#ifndef RILPROXYD
#define RILPROXYD

enum RIL_UNSOL_COMMAND {
	RIL_UNSOL_RESPONSE_BASE = 1000,
	RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED = 1001,
	RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED = 1002,
	RIL_UNSOL_RESPONSE_NEW_SMS = 1003,
	RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT = 1004,
	RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM = 1005,
	RIL_UNSOL_SIGNAL_STRENGTH = 1009,
	RIL_UNSOL_CALL_RING = 1018,
	RIL_UNSOL_OEM_HOOK_RAW = 1028,
	RIL_UNSOL_RIL_CONNECTED = 1034,
	RIL_UNSOL_VOICE_RADIO_TECH_CHANGED = 1035,
};

enum RIL_SOL_COMMAND {
	RIL_REQUEST_GET_CURRENT_CALLS = 9,
	RIL_REQUEST_DIAL = 10,
	RIL_REQUEST_HANGUP = 12,
	RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND = 13,
	RIL_REQUEST_LAST_CALL_FAIL_CAUSE = 18,
	RIL_REQUEST_VOICE_REGISTRATION_STATE = 20,
	RIL_REQUEST_DATA_REGISTRATION_STATE = 21,
	RIL_REQUEST_OPERATOR = 22,
	RIL_REQUEST_SIM_IO = 28,
	RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE = 45,
	RIL_REQUEST_SET_MUTE = 53,
	RIL_REQUEST_SCREEN_STATE = 61,
	RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION = 62,
	RIL_REQUEST_ALLOW_DATA = 123,
	RIL_REQUEST_CALLS_DISCONNECT = 132,
	RIL_REQUEST_CAN_REPORT_CALL = 133,
};

typedef enum {
    RIL_CALL_ACTIVE = 0,
    RIL_CALL_HOLDING = 1,
    RIL_CALL_DIALING = 2,    /* MO call only */
    RIL_CALL_ALERTING = 3,   /* MO call only */
    RIL_CALL_INCOMING = 4,   /* MT call only */
    RIL_CALL_WAITING = 5     /* MT call only */
} RIL_CallState;

typedef struct {
    RIL_CallState   state;
    int             index;      /* Connection Index for use with, eg, AT+CHLD */
    int             toa;        /* type of address, eg 145 = intl */
    char            isMpty;     /* nonzero if is mpty call */
    char            isMT;       /* nonzero if call is mobile terminated */
    char            als;        /* ALS line indicator if available
                                   (0 = line 1) */
    char            isVoice;    /* nonzero if this is is a voice call */
    char            isVoicePrivacy;     /* nonzero if CDMA voice privacy mode is active */
    char *          number;     /* Remote party number */
    int             numberPresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown 3=Payphone */
    char *          name;       /* Remote party name */
    int             namePresentation; /* 0=Allowed, 1=Restricted, 2=Not Specified/Unknown 3=Payphone */
    void *  uusInfo;    /* NULL or Pointer to User-User Signaling Information */
} RIL_Call;

#endif
