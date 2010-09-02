/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#warning "CPU frequency not defined, defaulting to 16MHz"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <penguino/uart/uart.h>
#include <penguino/uart/uart-stdio.h>
#include <penguino/io.h>
#include <penguino/time.h>

#include "i2c/i2cmaster.h"

#include "cs/tcs3414.h"

#define UART_BAUD_RATE 115200

uint16_t cs_read_word( uint8_t reg_address ) {
	uint16_t word = 0;
	
	i2c_start_wait( CS_SLAVE_ADDRESS + I2C_WRITE );
	i2c_write( CS_COMMAND_CMD | CS_COMMAND_TXN_WORD | reg_address );
	i2c_rep_start( CS_SLAVE_ADDRESS + I2C_READ );
	word |= i2c_readAck();
	word |= (i2c_readNak() << 8);
	i2c_stop();
	
	return word;
}

int main( ) {
	statusLed_init( );
	
	// flash status LED for a bit
	statusLed_orange( );
	delay_ms( 500 );
	statusLed_green( );
	
	// initalise UART
	uart_init( UART_BAUD_RATE );
	
	// enable UART as stdio
	uart_stdio_init( );
	
	// enable echo on the UART stdin so the user can see what they are typing
    uart_stdio_echo( true );

	printf( "\r\n" );
	printf( "I2C test for TCS3414\r\n" );
	printf( "\r\n" );
	
	// initialize I2C library
	i2c_init();
	
	printf( "i2c initialised...\r\n" );
	
	// power on the device and enable the ADC
	i2c_start_wait( CS_SLAVE_ADDRESS + I2C_WRITE );
	i2c_write( CS_COMMAND_CMD | CS_REG_CONTROL );
	i2c_write( CS_CONTROL_POWER | CS_CONTROL_ADC_EN );
	i2c_stop();
	
	printf( "device powered up...\r\n" );
	
	// set the gain control to high
	i2c_start_wait( CS_SLAVE_ADDRESS + I2C_WRITE );
	i2c_write( CS_COMMAND_CMD | CS_REG_GAIN );
	// "The most sensitive gain setting of the device would be when GAIN
	// is set to 11b (64×), and PRESCALER is set to 000b (divide by 1)."
	i2c_write( CS_GAIN_64X | CS_PRESCALER_DIV1 );
	i2c_stop();
	
	printf( "gain set to super-sensitive...\r\n" );
	
	// read the ID Register (04h)
	i2c_start_wait( CS_SLAVE_ADDRESS + I2C_WRITE );
	i2c_write( CS_COMMAND_CMD | CS_REG_ID );
	i2c_rep_start( CS_SLAVE_ADDRESS + I2C_READ );
	unsigned char ret = i2c_readNak();
	i2c_stop();
	
	printf( "CS_REG_ID = 0x%x\r\n", ret );
	
	printf( "\r\n" );

	for ( ; ; ) {
        
        printf( "CS_REG_(RED,GREEN,BLUE,CLR) = 0x%x, 0x%x, 0x%x, 0x%x\r\n", 
    			cs_read_word( CS_REG_RED_WORD ), cs_read_word( CS_REG_GREEN_WORD ), 
    			cs_read_word( CS_REG_BLUE_WORD ), cs_read_word( CS_REG_CLEAR_WORD ) );
		
		delay_ms( 500 );

	}

	return 0;
}

