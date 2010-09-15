/*
 * Configuration.h
 *
 *  Created on: 13/09/2010
 *      Author: coma
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define FRAMETRANSCEIVER_TX_BUFFER_SIZE					500
#define FRAMETRANSCEIVER_RX_BUFFER_SIZE					500
#define FRAMETRANSCEIVER_MAXIMUM_FRAME_SIZE			64

// Configuration of RadioDriver
#define RADIODRIVER_AUTO_RX
#define RADIODRIVER_USE_CRC

#undef NETWORK_CAN_ROUTE
#define NETWORK_CAN_ROUTE												0

#endif /* CONFIGURATION_H_ */
