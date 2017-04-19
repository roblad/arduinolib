/**
 * OBDII CAN BUS Implementation which is layered on top of CANBUS
 *
 * Copyright (c) 
*/

#include <string.h>
#include "Arduino.h"
#include "OBDIICanbus.h"

OBDIICanbusClass OBDIICanbus;

uint8_t OBDIICanbusClass::init(uint8_t speed)
{
#ifdef SERIAL_DEBUG
 	Serial.println("OBDIICanbusClass::init:Enter");
#endif

	// Initialize the MCP2515 chip
	uint8_t retVal = MCP2515.init(speed);
	
	if (!retVal)
	{
		return retVal;
	}

	// Setup the masks and filter to only pick up
	// OBDII Can bus responses.
	retVal = MCP2515.setMaskAndFilter(0x7FF, PID_REPLY);

//	MCP2515.setConfigMode(CONFIG_MODE);

#ifdef SERIAL_DEBUG
 	Serial.println("OBDIICanbusClass::init:Exit");
#endif
	return retVal;
}

/*------------------------------------------------------------*/
uint8_t OBDIICanbusClass::request(uint8_t pid,  uint8_t *buffer) 
{
	return request_byId(PID_REQUEST, pid,  buffer);
}

/*------------------------------------------------------------*/
uint8_t OBDIICanbusClass::request_byId(uint16_t reqid, uint8_t pid,  uint8_t *buffer) 
{
	return request_byIdnMode(MODE_CURR_DATA, reqid, pid, buffer);
}

/*------------------------------------------------------------*/
uint8_t OBDIICanbusClass::request_byIdnMode(uint8_t mode, uint16_t reqid, uint8_t pid,  uint8_t *buffer) 
{
#ifdef SERIAL_DEBUG  
	Serial.println("OBDIICanbusClass::request_byIdnMode:Enter");
#endif
	tCANMsg message;
	int timeout = 0;

	// Build the  message
	message.id = reqid;
	message.header.rtr = 0;
	message.header.length = 8;
	message.data[0] = 0x02;
	message.data[1] = mode;
	message.data[2] = pid;
	message.data[3] = DATA_PAD;
	message.data[4] = DATA_PAD;
	message.data[5] = DATA_PAD;
	message.data[6] = DATA_PAD;
	message.data[7] = DATA_PAD;						

	// Reset timeout
	timeout = 0;

	// Send the message
	if (MCP2515.send_message(&message))
	{
		// Wait for response
		while (timeout < NUM_READ_RETRY)
		{
			timeout++;

#ifdef SERIAL_DEBUG
// See if any error in the rec and transmit registers
uint8_t rxErr = MCP2515.read_register(REC);
uint8_t txErr = MCP2515.read_register(TEC);
uint8_t chkMsg = MCP2515.check_message();
  
//    char out[40];
//    sprintf(out, "txErr=0x%X,rxErr=0x%X,rxInt=0x%X", txErr, rxErr, chkMsg);
//    Serial.println(out);
#endif

#ifdef SERIAL_DEBUG
//    uint8_t merrf= MCP2515.read_register(CANINTF) & 0x80;
//    sprintf(out, "merrf=0x%X", merrf);
//    Serial.println(out);
#endif
			if (MCP2515.check_message()) 
			{
				if (MCP2515.get_message(&message)) 
				{
					// Check message is the reply and its the right PID
					if((message.id == PID_REPLY) && (message.data[2] == pid))	
					{
						// Got a response so transfer the data to the buffer for return
						memcpy(buffer, &message.data[0], message.header.length);
#ifdef SERIAL_DEBUG  
  Serial.println("OBDIICanbusClass::request:Exit STATUS_OK");
#endif
						return STATUS_OK;
					}
				}
			}
		}
#ifdef SERIAL_DEBUG  
  Serial.println("OBDIICanbusClass::request:Exit STATUS_TIMEOUT");
#endif
		return STATUS_TIMEOUT;
	}
#ifdef SERIAL_DEBUG  
  Serial.println("OBDIICanbusClass::request:Exit STATUS_FAILURE");
#endif
	return STATUS_FAILURE;
}

/*------------------------------------------------------------*/
uint8_t OBDIICanbusClass::get_message(tCANMsg *message) 
{
#ifdef SERIAL_DEBUG  
	Serial.println("OBDIICanbusClass::get_message:Enter");
#endif

	// Set normal mode	
	MCP2515.bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);

      // See if any error in the rec and transmit registers
      uint8_t rxErr = MCP2515.read_register(REC);
      uint8_t txErr = MCP2515.read_register(TEC);
      uint8_t chkMsg = MCP2515.check_message();
  
#ifdef SERIAL_DEBUG
    char out[132];
    sprintf(out, "Before=txErr=0x%X,rxErr=0x%X,rxInt=0x%X", txErr, rxErr, chkMsg);
    Serial.println(out);
#endif

	if (MCP2515.check_message()) 
	{
		if (MCP2515.get_message(message)) 
		{
#ifdef SERIAL_DEBUG  
  Serial.println("OBDIICanbusClass::get_message:Exit STATUS_OK");
#endif
			return STATUS_OK;
		}
	}
#ifdef SERIAL_DEBUG  
  Serial.println("OBDIICanbusClass::get_message:Exit STATUS_FAILURE");
#endif
	return STATUS_FAILURE;
}

