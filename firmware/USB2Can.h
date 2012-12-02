/** \file
 *
 *  Header file for USB2Can.c.
 */

#ifndef _USB_2_CAN_H_
#define _USB_2_CAN_H_

#ifdef __cplusplus
extern "C"
{
#endif

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <string.h>

		#include "Descriptors.h"

		#include <LUFA/Drivers/USB/USB.h>

	/* Function Prototypes: */
		void SetupHardware(void);
		void CheckVirtualSerialCommands(void);
		void CheckVirtualSerialCanMessages(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);

#ifdef __cplusplus
}
#endif



#endif

