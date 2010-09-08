/*
 * FrameTransceiver.h
 *
 *  Created on: 10/05/2010
 *      Author: coma
 */

#ifndef FRAMETRANSCEIVER_H_
#define FRAMETRANSCEIVER_H_

#include <Globals.h>

typedef struct
{
	uint8_t uart;
	uint16_t ubrr;
	blockHandlerCallback frameHandler;
} frameTransceiver;

extern void FrameTransceiver_Initialize(uint8_t uart, uint16_t ubrr, blockHandlerCallback frameReceivedHandler);
extern void FrameTransceiver_Send(uint8_t uart, void* data, uint8_t length);

#endif /* FRAMETRANSCEIVER_H_ */
