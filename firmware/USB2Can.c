    /** \file
 *
 *  Main source file for the USB to Can converter. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "USB2Can.h"
#include "CAN_Module.h"

#define MAX_BUFFER_LENGTH 128

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the first CDC interface,
 *  which sends strings to the host for each joystick movement.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial1_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 0,
				.DataINEndpoint           =
					{
						.Address          = CDC1_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC1_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC1_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the second CDC interface,
 *  which echos back all received data from the host.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial2_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 2,
				.DataINEndpoint           =
					{
						.Address          = CDC2_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC2_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC2_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},

			},
	};


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	sei();

	//DDRB |= (1 << 3);
	//PORTB |= (1 << 3);

	can_t message;
	message.id = 40;
	message.flags.rtr = 0;
	message.flags.extended = 1;
    message.length = 2;
    message.data[0] = 0x0F;
    message.data[1] = 0xF0;

    can_t retrieved_message;

    can_set_mode(LOOPBACK_MODE);

	for (;;)
	{
	    //CheckVirtualSerialCommands();
		//CheckVirtualSerialCanMessages();

        if( can_check_free_buffer())
        {
            can_send_message(&message);
            //if(can_check_message())
            {
                //can_get_message(&retrieved_message);
                //if(retrieved_message.id == 40)
                    PORTE ^= (1 << 6);
            }
        }

		/* Discard all received data on the first CDC interface */
		CDC_Device_ReceiveByte(&VirtualSerial1_CDC_Interface);

		/* Echo all received data on the second CDC interface */
		int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial2_CDC_Interface);
		if (!(ReceivedByte < 0))
		  CDC_Device_SendByte(&VirtualSerial2_CDC_Interface, (uint8_t)ReceivedByte);

		CDC_Device_USBTask(&VirtualSerial1_CDC_Interface);
		CDC_Device_USBTask(&VirtualSerial2_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();

	CAN_Init();

	DDRE |= (1 << 6);
    PORTE &= ~(1 << 6);
}

//using VirtualSerial1 for configuration purpose
void CheckVirtualSerialCommands(void)
{


}

//Can messages are transmitted via VirtualSerial2 to computer
void CheckVirtualSerialCanMessages(void)
{
    static unsigned char buffer_pos = 0;
    static unsigned char buffer[MAX_BUFFER_LENGTH] = "";
    char ascii_message[ASCII_CAN_MESSAGE_LENGTH];
    int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial2_CDC_Interface); //return negative value while no new bytes are available
    unsigned char i,j = 0;
	while(!(ReceivedByte < 0) && buffer_pos < (MAX_BUFFER_LENGTH -1))
    {
        buffer[buffer_pos] = (unsigned char)(ReceivedByte & 0xFF);      //put byte into buffer until it is full
        buffer_pos++;                      //hopp to next buffer position
    }
    buffer[buffer_pos] = 0; //terminated by zero

    while(i < MAX_BUFFER_LENGTH) //no ascii can messages found
    {
        for(i = 0; i < MAX_BUFFER_LENGTH && buffer[i] != '\n'; i++); //search for char '\n'

        if(i <=  ASCII_CAN_MESSAGE_LENGTH)  //char '\n' found before end of buffer, i is position of '\n'
        {                                  //message has a valid length --> should not be longer then ASCII_CAN_MESSAGE_LENGTH
            for(j = 0; j < i; j++)       //copy all chars to message except '\n'
            {
                ascii_message[j] = buffer[j];  //copy
            }
        }
        for(j = 0; j <= i && (i+j+2) < MAX_BUFFER_LENGTH; j++)    //cut all chars before and including ’\n’, move following chars to front
        {
            buffer[j] = buffer[i+j+1];                  //don't copy the char '\n' (+1)
        }
        buffer[i+j+1] = 0;
    }

}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial1_CDC_Interface);
	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial2_CDC_Interface);

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial1_CDC_Interface);
	CDC_Device_ProcessControlRequest(&VirtualSerial2_CDC_Interface);
}

