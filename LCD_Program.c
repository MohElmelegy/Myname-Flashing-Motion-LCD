#define F_CPU  8000000
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include <avr/delay.h>


void LCD_VidInit()
{

	DIO_VidSetPortDirection(DATA_PORT , OUTPUT_PORT);
	DIO_VidSetPortDirection(CONTROL_PORT , OUTPUT_PORT);

	_delay_ms(50);
	//u8command for Data port only
	LCD_VidSendCommand(0b00111000);
	_delay_ms(1);
	LCD_VidSendCommand(0b00001100);
	_delay_ms(1);
	LCD_VidSendCommand(0b00000001);
	_delay_ms(2);
	
}

void LCD_VidSendCommand(u8 LOC_u8Command)
{
	// RS --- register sellection (RS = 0) for write command
	DIO_VidSetPinValue(CONTROL_PORT , PIN0 , LOW);
	// R/W --- Read/Write (R/W = 0) for Write
	DIO_VidSetPinValue(CONTROL_PORT , PIN1 , LOW);
	// Write command on the LCD (it's port)
	DIO_VidSetPortValue(DATA_PORT , LOC_u8Command);
	// Enable (we make a mechanism for falling edge to can write)
	DIO_VidSetPinValue(CONTROL_PORT , PIN2 , HIGH);
	_delay_ms(1);
	DIO_VidSetPinValue(CONTROL_PORT , PIN2 , LOW);
	// instead of read the bussy flag we make delay as LCD reach write my data
	_delay_ms(1);
}

void LCD_VidWriteChar(u8 LOC_u8Data)
{
	// RS --- register sellection (RS = 1) for write data
	DIO_VidSetPinValue(CONTROL_PORT , PIN0 , HIGH);
	// R/W --- Read/Write (R/W = 0) for Write
	DIO_VidSetPinValue(CONTROL_PORT , PIN1 , LOW);
	// Write data on the LCD (it's port)
	DIO_VidSetPortValue(DATA_PORT , LOC_u8Data);
	// Enable (we make a mechanism for falling edge to can write)
	DIO_VidSetPinValue(CONTROL_PORT , PIN2 , HIGH);
	_delay_ms(1);
	DIO_VidSetPinValue(CONTROL_PORT , PIN2 , LOW);
	// instead of read the bussy flag we make delay as LCD reach write my data
	_delay_ms(1);
}

void LCD_VidWriteString(u8 LOC_u8Data[])
{
	u8 count = 0 ;

	for(count=0 ; LOC_u8Data[count] != '\0' ; count++)
	{
		LCD_VidWriteChar(LOC_u8Data[count]);
	}

}

void LCD_VidWriteNumber(u32 LOC_u8Number)
{
	// if number = 0, it can't print it using the while loop
	// as we use the number (0) as a check to reach the all numbers
	if(LOC_u8Number == 0)
	{
		LCD_VidWriteChar(LOC_u8Number + 48);
	}
	else
	{
		// if we don't reserve the number, the number will be written
		// reserved
		u32 reverse = 0;

		while(LOC_u8Number != 0)
		{
			reverse = (reverse * 10) + (LOC_u8Number % 10);
			LOC_u8Number /= 10 ;
		}
		// to write the number
		while(reverse != 0)
		{
			LCD_VidWriteChar(reverse%10 + 48);
			reverse /= 10 ;
		}

	}
}

void LCD_VidSetPosition(u8 LOC_u8Row , u8 LOC_u8Col)
{
	if (LOC_u8Row == 0)
	{
		LCD_VidSendCommand(128 + LOC_u8Col);
	}
	else if(LOC_u8Row == 1)
	{
		LCD_VidSendCommand(192 + LOC_u8Col);
	}
}
