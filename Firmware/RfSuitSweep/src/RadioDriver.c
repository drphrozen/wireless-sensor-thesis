#if defined(__AVR_ATmega128RFA1__)

#include "RadioDriver.h"
#include <Config.h>

// Transceiver state status
#define TRX_STATUS_MASK													0x1f
enum
{
	STATUS_BUSY_RX = 0x01,
	STATUS_BUSY_TX = 0x02,
	STATUS_RX_ON = 0x06,
	STATUS_TRX_OFF = 0x08,
	STATUS_PLL_ON = 0x09,
	STATUS_SLEEP = 0x0f,
	STATUS_BUSY_RX_AACK = 0x11,
	STATUS_BUSY_TX_ARET = 0x12,
	STATUS_RX_AACK_ON = 0x16,
	STATUS_TX_ARET_ON = 0x19,
	STATUS_STATE_TRANSITION_IN_PROGRESS = 0x1f
};

// Transceiver state change commands
enum
{
	STATE_NOP = 0x00,
	STATE_TX_START = 0x02,
	STATE_FORCE_TRX_OFF = 0x03,
	STATE_FORCE_PLL_ON = 0x04,
	STATE_RX_ON = 0x06,
	STATE_TRX_OFF = 0x08,
	STATE_PLL_ON = 0x09,
	STATE_RX_AACK_ON = 0x16,
	STATE_TX_ARET_ON = 0x19
};

#define PHY_RSSI_MASK														0x1f
#define PHY_RSSI_RANDOM_MASK										0x60

enum
{
	VERSION_A = 2, // A and B are both 2 according to the data sheet
	VERSION_B = 2,
	VERSION_C = 3,
	VERSION_D = 4
};

#define TRX_BUF(i)															_SFR_MEM8(0x180 + i)

#define ChangeState(s)													TRX_STATE = s;
#define WaitUntilStatus(s)											while ((TRX_STATUS & TRX_STATUS_MASK) != s);
#define WaitWhileStatus(s)											while ((TRX_STATUS & TRX_STATUS_MASK) == s);
#define ReadStatus()														(TRX_STATUS & TRX_STATUS_MASK)

static blockHandlerCallback frameHandler = NULL;
static volatile bool transmitting = false;
static volatile uint8_t rssi = 0;

void RadioDriver_Initialize(blockHandlerCallback fh)
{
	frameHandler = fh;


#if defined(RADIODRIVER_USE_CRC)
	SetBit(TRX_CTRL_1, TX_AUTO_CRC_ON); // Default value
#else
	ClearBit(TRX_CTRL_1, TX_AUTO_CRC_ON);
#endif

	IRQ_STATUS |= (1 << TX_END) | (1 << RX_END) | (1 << RX_START);
	IRQ_MASK |= (1 << TX_END_EN) | (1 << RX_END_EN) | (1 << RX_START_EN);


#if defined(RADIODRIVER_AUTO_RX)
	ChangeState(STATE_RX_ON);
#else
	ChangeState(STATE_TRX_OFF);
#endif
}

void RadioDriver_SetBitRate(uint8_t bitRate)
{
	Modify(TRX_CTRL_2, 0x03, bitRate);
}

void RadioDriver_SetTxPower(uint8_t power)
{
	Modify(PHY_TX_PWR, 0x0f, power);
}

void RadioDriver_SetChannel(uint8_t channel)
{
	Modify(PHY_CC_CCA, 0x0f, channel);
}

void RadioDriver_SetReceiverSensitivityThreshold(uint8_t level)
{
	Modify(RX_SYN, 0x0f, level);
}

uint8_t RadioDriver_GetRandomNumber()
{
	return (PHY_RSSI & PHY_RSSI_RANDOM_MASK) >> 5;
}

uint8_t RadioDriver_GetRawRssi()
{
	return rssi & PHY_RSSI_MASK;
}

int8_t RadioDriver_GetRssi()
{
	return -90 + 3 * ((rssi & PHY_RSSI_MASK) - 1);
}

void RadioDriver_Calibrate()
{
	// Filter tuning trigger - should be run at least every 5 minutes. Bit auto clears when completed after ~25us.
	FTN_CTRL |= (1 << FTN_START);
	// Transceiver center frequency calibration. 5 minutes. Automatically performed when switching to PLL_ON or RX_ON. Typical duration is 35 us.
	PLL_CF |= (1 << PLL_CF_START);
	// Transceiver Delay cell calibration. 5 minutes. Automatically performed when switching to PLL_ON or RX_ON. Takes at most 6 us.
	PLL_DCU |= (1 << PLL_DCU_START);
}

void RadioDriver_Send(void* data, uint8_t length)
{
#if defined(RADIODRIVER_USE_CRC)
	if (length > 125)
	{
		return;
	}
#else
	if (length > 127)
	{
		return;
	}
#endif


	// Wait for a reception to complete in case one is in progress. This is done prior to checking and setting
	// the transmitting flag to enable calling the RadioDriver_Send function from within a receive handler
	// function (thus blocking the previous non-interrupt call to Send).
	// This is not tested and may cause the interrupt handler to clock while sending the low priority message
	// due to latency from leaving STATUS_BUSY_RX to the actual interrupt handler firing.
	WaitWhileStatus(STATUS_BUSY_RX);

	// Wait for any current transmission to complete.
	while (transmitting)
		;

	// Signal that a transmission is in progress thus blocking any further transmission.
	transmitting = true;


	// Switch to PLL_ON state so a transmission can be initiated.
	ChangeState(STATE_PLL_ON);


#if defined(RADIODRIVER_USE_CRC)
	// If automatic CRC calculation is used two additional bytes must be sent.
	TRX_BUF(0) = length + 2;
#else
	// Set number of bytes to be sent.
	TRX_BUF(0) = length;
#endif

	uint8_t* d = (uint8_t*) data;


	// Fill transmission buffer.
	for (uint8_t i = 0; i < length; ++i)
	{
		TRX_BUF(i + 1) = d[i];
	}


	// Wait until the transceiver is ready to switch to transmit mode.
	WaitUntilStatus(STATUS_PLL_ON);


	// Start transmission.
	ChangeState(STATE_TX_START);
}

void RadioDriver_EnableReceiveMode()
{
	if (ReadStatus() != STATUS_RX_ON)
	{
		ChangeState(STATE_RX_ON);
	}
}

void RadioDriver_DisableReceiveMode()
{
	if (ReadStatus() == STATUS_BUSY_RX || ReadStatus() == STATUS_RX_ON)
	{
		ChangeState(STATE_TRX_OFF);
	}
}

//ISR(TRX24_AWAKE_vect)
//{
//}

ISR(TRX24_TX_END_vect)
{
	// Maybe this can be done immediately after starting the transmission (data sheet page 40).
#if defined(RADIODRIVER_AUTO_RX)
	ChangeState(STATE_RX_ON);
#else
	ChangeState(STATE_TRX_OFF);
#endif

	transmitting = false;
}

//ISR(TRX24_XAH_AMI_vect)
//{
//}

//ISR(TRX24_CCA_ED_DONE_vect)
//{
//}

ISR(TRX24_RX_END_vect)
{
#if defined(RADIODRIVER_USE_CRC)
	if ((PHY_RSSI & RX_CRC_VALID) == 0) // CRC error in frame => dump it

	{
		return;
	}
#endif

	uint8_t length = TST_RX_LENGTH;
	uint8_t buffer[length];

	for (uint8_t i = 0; i < length; ++i)
	{
		buffer[i] = TRX_BUF(i);
	}

	if (frameHandler != NULL)
	{
		frameHandler(buffer, length);
	}
}

ISR(TRX24_RX_START_vect)
{
	rssi = PHY_RSSI;
}

//ISR(TRX24_PLL_UNLOCK_vect)
//{
//}

//ISR(TRX24_PLL_LOCK_vect)
//{
//}

#endif