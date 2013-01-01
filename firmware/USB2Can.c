    /** \file
 *
 *  Main source file for the USB to Can converter. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "USB2Can.h"
#include "CAN_Module.h"
#include <string.h>

#ifdef DEBUG

#define BAUD 9600UL      // Baudrate

// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif

#endif




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

	sei();

	//DDRB |= (1 << 3);
	//PORTB |= (1 << 3);

	can_t message;
	message.id = 41;
	message.flags.rtr = 0;
	message.flags.extended = 1;
    message.length = 2;
    message.data[0] = 0x0F;
    message.data[1] = 0xF0;

    can_t retrieved_message;

    can_send_message(&message);
    can_set_mode(LOOPBACK_MODE);

	for (;;)
	{
	    CheckVirtualSerialCommands();
		CheckVirtualSerialCanMessages();

      //  if( can_check_free_buffer())
        //{


            //if(can_check_message())
            //{
                //can_get_message(&retrieved_message);
                //if(retrieved_message.id == 40)

            //}
       //    }


		/* Discard all received data on the first CDC interface */
		//CDC_Device_ReceiveByte(&VirtualSerial1_CDC_Interface);

		/* Echo all received data on the second CDC interface */
		//int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial2_CDC_Interface);
		//if (!(ReceivedByte < 0))
		//  CDC_Device_SendByte(&VirtualSerial2_CDC_Interface, (uint8_t)ReceivedByte);

        //PORTE &= ~(1 << 6);

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
	USB_Init();

	CAN_Init();

	DDRE |= (1 << 6);
    PORTE &= ~(1 << 6);

    PORTB &= ~(1 << 5);    //B5 is input
    PORTB |= (1 << 5);     //B5 has pull-up

#ifdef DEBUG
    //Debug UART Port:
    UBRR1 = UBRR_VAL;

    UCSR1B |= (1<<TXEN1);                           // UART TX einschalten
    UCSR1C = (1<<UCSZ11)|(3<<UCSZ10); // Asynchron 8N1
#endif
}


void CheckVirtualSerialCommands(void)   //channel 0: using VirtualSerial1 for configuration purpose and slow can message interface
{
    if(CDC_Device_ReceiveByte(&VirtualSerial1_CDC_Interface) >= 0)
    {
        can_t message;
        message.id = 42;
        message.flags.rtr = 0;
        message.flags.extended = 1;
        message.length = 2;
        message.data[0] = 0x0F;
        message.data[1] = 0xF0;

        can_t message2;
        message2.id = 0;
        message2.flags.rtr = 0;
        message2.flags.extended = 0;
        message2.length = 0;
        message2.data[0] = 0;

        char ascii_message[ASCII_CAN_MESSAGE_LENGTH];



        can2ascii(ascii_message, &message);       //convert current can message to ascii code
        if(ascii2can(ascii_message, &message2))
            CDC_Device_SendString(&VirtualSerial1_CDC_Interface, "Conversation successfull\n");   //testing message
        else
            CDC_Device_SendString(&VirtualSerial1_CDC_Interface, "Conversation not successfull\n");

        can2ascii(ascii_message, &message2);

        CDC_Device_SendString(&VirtualSerial1_CDC_Interface, ascii_message);   //and send it over usb
    }


}

void CheckVirtualSerialCanMessages(void)  //cannel 1: high speed can message interface
{
    static unsigned char buffer_pos = 0;
    static char buffer[MAX_BUFFER_LENGTH];

    int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial2_CDC_Interface); //returns a negative number while no new data is available

    while((ReceivedByte >= 0) && (buffer_pos < (MAX_BUFFER_LENGTH -1)))
    {
        #ifdef DEBUG
        UART_Transmit(ReceivedByte);
        #endif
        CDC_Device_SendByte(&VirtualSerial2_CDC_Interface, ReceivedByte); //echo received byte, debug!
        buffer[buffer_pos] = (unsigned char)(ReceivedByte & 0xFF);      //put byte into buffer until it is full
        buffer_pos++;                      //hopp to next buffer position
        ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial2_CDC_Interface); //get next char - returns negative value while no new bytes are available
    }
    buffer[buffer_pos] = 0; //terminated by zero

    if(buffer_pos >= ASCII_CAN_MESSAGE_LENGTH - 16) //length of buffer contains at a full can message (with at least zero data bytes)
    {
        int end_of_ascii_message =  ascii_message_exists(buffer);

        if(end_of_ascii_message > 0)
        {
            char temp[MAX_BUFFER_LENGTH];

            get_substring(buffer, temp, 0, end_of_ascii_message);

            can_t can_message;

            if(ascii2can(temp, &can_message))    //if conversation is successfull --> send message
            {
                can_send_message(&can_message);
                CDC_Device_SendString(&VirtualSerial2_CDC_Interface, "OK");   //ACK
                CDC_Device_SendString(&VirtualSerial2_CDC_Interface, "\r\n");   //new line after message
            }
            else
            {
                CDC_Device_SendString(&VirtualSerial2_CDC_Interface, "NOK");   //ACK
                CDC_Device_SendString(&VirtualSerial2_CDC_Interface, "\r\n");   //new line after message
            }

            get_substring(buffer, temp, end_of_ascii_message, strlen(buffer));

            memcpy(buffer, temp, strlen(temp)+1);

        }
        else if(buffer_pos > ASCII_CAN_MESSAGE_LENGTH)
        {   //when buffer is longer than one ascii can message than snip front of buffer ---> buffer gets emptied
            //PORTE |= (1 << 6);
            char temp[MAX_BUFFER_LENGTH];
            get_substring(buffer, temp, 1, strlen(buffer)+1);
            memcpy(buffer, temp, strlen(temp)+1);
            buffer_pos--;
        }

    }

    //PORTE |= (1 << 6);
    can_t can_message;
    if(can_check_message())
    {
        if(can_get_message(&can_message))   //true if message could be recieved
        {
            if(PORTE & (1 << 6))
                PORTE &= ~(1 << 6);
            else
                PORTE |= (1 << 6);
            char ascii_message[ASCII_CAN_MESSAGE_LENGTH];
            can2ascii(ascii_message, &can_message);       //convert current can message to ascii code
            CDC_Device_SendString(&VirtualSerial2_CDC_Interface, ascii_message);   //and send it over usb
            CDC_Device_SendString(&VirtualSerial2_CDC_Interface, "\n");   //new line after message
        }
    }
}

/*
//Can messages are transmitted via VirtualSerial2 to computer
void CheckVirtualSerialCanMessages(void)
{
    static unsigned char buffer_pos = 0;
    static unsigned char buffer[MAX_BUFFER_LENGTH] = "";

    int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial2_CDC_Interface); //returns negative value while no new bytes are available
    if(ReceivedByte < 0)
        return; //nothing to do! no new messages

    //begin interpret new messsages ==>
    can_t can_message;
    unsigned char i,j = 0;
    char ascii_message[ASCII_CAN_MESSAGE_LENGTH];
	while(!(ReceivedByte < 0) && buffer_pos < (MAX_BUFFER_LENGTH -1))
    {
        CDC_Device_SendByte(&VirtualSerial2_CDC_Interface, ReceivedByte); //echo received byte, debug!
        buffer[buffer_pos] = (unsigned char)(ReceivedByte & 0xFF);      //put byte into buffer until it is full
        buffer_pos++;                      //hopp to next buffer position
        ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial2_CDC_Interface); //get next char - returns negative value while no new bytes are available
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
                if(ascii2can(ascii_message, &can_message))
                {
                    can_send_message(&can_message);
                    PORTE |= (1 << 6);
                }
            }
        }
        for(j = 0; j <= i && (i+j) < (MAX_BUFFER_LENGTH - 2); j++)    //cut all chars before and including ’\n’, move following chars to front
        {
            buffer[j] = buffer[i+j+1];                  //don't copy the char '\n' (+1)
        }
        buffer[i+j+1] = 0;
    }

    if(can_get_message(&can_message))   //true if message could be recieved
    {
        can2ascii(ascii_message, &can_message);       //convert current can message to ascii code
        CDC_Device_SendString(&VirtualSerial2_CDC_Interface, ascii_message);   //and send it over usb
    }
}  */

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    //PORTE |= (1 << 6);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    //PORTE &= ~(1 << 6);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial1_CDC_Interface);
	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial2_CDC_Interface);

}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial1_CDC_Interface);
	CDC_Device_ProcessControlRequest(&VirtualSerial2_CDC_Interface);
}


void send_command_string(char* str)
{
    CDC_Device_SendString(&VirtualSerial1_CDC_Interface, str);
}

void send_can_string(char* str)
{
    CDC_Device_SendString(&VirtualSerial2_CDC_Interface, str);
}


#ifdef DEBUG

void UART_Transmit( unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR1A & (1<<UDRE1)));

    /* Put data into buffer, sends the data */
    UDR1 = data;
}

#endif