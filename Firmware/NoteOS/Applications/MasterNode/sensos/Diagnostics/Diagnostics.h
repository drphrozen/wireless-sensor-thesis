/*
 * Diagnostics.h
 *
 *  Created on: 05/11/2010
 *      Author: Coma
 */

#ifndef DIAGNOSTICS_H_
#define DIAGNOSTICS_H_

#include "../Globals.h"

enum
{
	DIAGNOSTICS_SYSTEM_INITIALIZED,
	DIAGNOSTICS_ALLOCATED_NETWORK_BLOCK,
	DIAGNOSTICS_FAILED_TO_ALLOCATE_NETWORK_BLOCK,
	DIAGNOSTICS_RELEASED_NETWORK_BLOCK,
	DIAGNOSTICS_FAILED_TO_RELEASE_NETWORK_BLOCK,
	DIAGNOSTICS_ALLOCATED_SENSOR_BLOCK,
	DIAGNOSTICS_FAILED_TO_ALLOCATE_SENSOR_BLOCK,
	DIAGNOSTICS_RELEASE_SENSOR_BLOCK,
	DIAGNOSTICS_FAILED_TO_RELEASE_SENSOR_BLOCK,
	DIAGNOSTICS_NOTIFY_QUEUED,
	DIAGNOSTICS_NOTIFY_EXECUTED,
	DIAGNOSTICS_PROCESS_QUEUED,
	DIAGNOSTICS_PROCESS_EXECUTED,
	DIAGNOSTICS_PUBLISH_QUEUED,
	DIAGNOSTICS_PUBLISH_EXECUTED,
	DIAGNOSTICS_RX_RTS,
	DIAGNOSTICS_RX_CTS,
	DIAGNOSTICS_RX_DATA,
	DIAGNOSTICS_RX_ACK,
	DIAGNOSTICS_TX_RTS,
	DIAGNOSTICS_TX_CTS,
	DIAGNOSTICS_TX_DATA,
	DIAGNOSTICS_TX_ACK,
	DIAGNOSTICS_DATA_QUEUED,
	DIAGNOSTICS_DATA_NOT_QUEUED,
	DIAGNOSTICS_NETWORK_TICK,
	DIAGNOSTICS_NETWORK_FORWARDING
};

extern void Diagnostics_Initialize(uint32_t serialNumber);
extern void Diagnostics_SendEvent(uint8_t eventId);
extern void Diagnostics_SendMessage(uint8_t messageId, void* message, uint8_t messageLength);
extern void Diagnostics_SendState(uint8_t stateMachineId, uint8_t newState);

#endif /* DIAGNOSTICS_H_ */
