#include <stdio.h>

#include "penguino/uart/uart.h"
#include "penguino/uart/uart-stdio.h"
#include "penguino/spi/spi-master.h"
#include "penguino/io.h"
#include "penguino/time.h"

#define UART_BAUD_RATE 115200

// http://www.merl.com/reports/docs/TR2003-35.pdf

uint32_t readLed( uint8_t led ) {
	uint8_t led_anode = 1 << (led << 1);
	uint8_t led_cathode = 2 << (led << 1);
	
	// configure both pins as output
	DDRA |= led_anode | led_cathode;
	
	// LED off, prepare
	PORTA &= ~( led_anode | led_cathode );
	
	// power LED in reverse bias mode
	PORTA &= ~led_anode; // turn off anode (to -)
	PORTA |= led_cathode; // turn on cathode (to +)
	
	// give it some time
	//delay_ms( 1 );
	//_delay_us( 10 );
	
	uint32_t clocks = 0;
	
	// turn off the cathode, make it input
	DDRA &= ~(led_cathode); // input
	PORTA &= ~(led_cathode); // no pull-up
	
	// watch the pin, counting the number of clocks it stays high for
	while ( (PINA & led_cathode) != 0 ) {
		clocks++;
		_delay_us( 1 );
	}
	
	// LED on, as per normal
	DDRA |= led_anode | led_cathode;
	PORTA |= led_anode;
	PORTA &= ~led_cathode;
	
	return clocks;
}

int main( void ) {
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

	// initialise by reading once from each LED
	/*readLed( 0 );
	readLed( 1 );
	readLed( 2 );*/
	//readLed( 3 );
	
	while ( 1 ) {
		// main loop of the programmer
		
		/*if ( uart_hasData( ) ) {
			unsigned char cmd = uart_getc( );
			
			if ( cmd >= '1' && cmd <= '4' ) {
				printf( "LED %c value: %d\r\n", cmd, readLed( cmd - '1' ) );
			}
		}*/
		
		printf( "\r\n" );
		printf( "--\r\n" );
		printf( "\r\n" );
		printf( "\r\n" );
		//printf( "LED 3 discharge time: %luus\r\n", readLed( 3 ) );
		//printf( "LED 2 discharge time: %luus\r\n", readLed( 2 ) );
		
		uint32_t led0 = readLed( 0 ) >> 8;
		uint32_t led1 = readLed( 1 ) >> 8;
		uint32_t led2 = readLed( 2 ) >> 8;
		uint32_t led3 = readLed( 3 ) >> 8;
		
		//printf( "LEDs: %lu %lu\r\n", led2, led3 );
		
		printf( "0: " );
		for ( int i = 0; i < led0; i++ ) {
			printf( "#" );
		}
		printf( "\r\n" );
		
		printf( "1: " );
		for ( int i = 0; i < led1; i++ ) {
			printf( "#" );
		}
		printf( "\r\n" );
		
		printf( "2: " );
		for ( int i = 0; i < led2; i++ ) {
			printf( "#" );
		}
		printf( "\r\n" );
		
		printf( "3: " );
		for ( int i = 0; i < led3; i++ ) {
			printf( "#" );
		}
		printf( "\r\n" );
		
		delay_ms( 100 );
	}

	return 0;
}
