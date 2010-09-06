// This file is auto generated by the Message Builder.
// Don't bother making changes to this file as they will be lost when recompiling.

// Message Builder by Claus Andersen.

#include <stdint.h>

enum
{
	MESSAGEID_PINGREQUEST,
	MESSAGEID_PINGREPLY,
	MESSAGEID_NOTHINGTOKEN,
	MESSAGEID_SETTXPOWERTOKEN,
	MESSAGEID_ENABLESWEEPINGTOKEN,
	MESSAGEID_DISABLESWEEPINGTOKEN,
	MESSAGEID_REPORTTOKEN
};

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
} msgMessageBase;

#define AsMsgMessageBase(x) ((msgMessageBase*)x)

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
} pingRequestMessage;

#define AsPingRequestMessage(x) ((pingRequestMessage*)x)

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
} pingReplyMessage;

#define AsPingReplyMessage(x) ((pingReplyMessage*)x)

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
} nothingTokenMessage;

#define AsNothingTokenMessage(x) ((nothingTokenMessage*)x)

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
	uint8_t txPower;
} setTxPowerTokenMessage;

#define AsSetTxPowerTokenMessage(x) ((setTxPowerTokenMessage*)x)

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
} enableSweepingTokenMessage;

#define AsEnableSweepingTokenMessage(x) ((enableSweepingTokenMessage*)x)

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
} disableSweepingTokenMessage;

#define AsDisableSweepingTokenMessage(x) ((disableSweepingTokenMessage*)x)

typedef struct
{
	uint8_t destination;
	uint8_t source;
	uint8_t messageId;
	uint8_t rssis[16];
} reportTokenMessage;

#define AsReportTokenMessage(x) ((reportTokenMessage*)x)