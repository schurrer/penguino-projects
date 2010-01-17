#include <stdio.h>

#include "penguino/uart/uart.h"
#include "penguino/uart/uart-stdio.h"
#include "penguino/spi/spi-master.h"
#include "penguino/io.h"
#include "penguino/time.h"

#define UART_BAUD_RATE 115200

// http://www.merl.com/reports/docs/TR2003-35.pdf

uint32_t clocks[8] = {0};

void readLeds( ) {
	// configure both pins as output
	DDRA = 0xff;
	DDRB = 0xff;
	
	// LED off, prepare
	PORTA = 0x00;
	PORTB = 0x00;
	
	// power LED in reverse bias mode
	PORTB = 0x00; // turn off anode (to -)
	PORTA = 0xff; // turn on cathode (to +)
	
	// give it some time
	//delay_ms( 1 );
	//_delay_us( 10 );
	
	clocks[0] = 0;
	clocks[1] = 0;
	clocks[2] = 0;
	clocks[3] = 0;
	clocks[4] = 0;
	clocks[5] = 0;
	clocks[6] = 0;
	clocks[7] = 0;
	
	// turn off the cathode, make it input
	DDRA = 0x00; // input
	PORTA = 0x00; // no pull-up
	
	// watch the pin, counting the number of clocks it stays high for
	while ( 1 ) {
		uint8_t reading = PINA;
		
		// if all LEDs read 0, done!
		if ( reading == 0 ) {
			break;
		}
		
		// otherwise, increment the clock for any which are set
		uint8_t pin = 0;
		for ( pin = 0; pin < 8; pin++ ) {
			if ( (reading & (1<<pin)) != 0 ) {
				clocks[pin]++;
			}
		}
		
		_delay_us( 1 );
	}
	
	// LED on, as per normal
	DDRA = 0xff; // cathode = output
	PORTA = 0x00; // cathode = low
	PORTB = 0xff; // anode = high
}

void displayBargraph( int val ) {
	for ( int i = 0; i < val; i++ ) {
		printf( "#" );
	}
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
	
	readLeds( );
	readLeds( );
	
	uint32_t bases[8];
	
	for ( int i = 0; i < 8; i++ ) {
		bases[i] = clocks[i] >> 1;
	}
	
	while ( 1 ) {
		// main loop of the programmer
		
		/*if ( uart_hasData( ) ) {
			unsigned char cmd = uart_getc( );
			
			if ( cmd >= '1' && cmd <= '4' ) {
				printf( "LED %c value: %d\r\n", cmd, readLed( cmd - '1' ) );
			}
		}*/
		
		if ( uart_hasData( ) ) {
			unsigned char cmd = uart_getc( );
			
			readLeds( );
		
			fwrite( clocks, sizeof(uint32_t), 8, stdout );
		}
		
		/*
		
		int i;
		for ( i = 0; i < 8; i++ ) {
			printf( "%d: ", i );
			
			displayBargraph( (clocks[i] - bases[i]) >> 8 );
			
			printf( "\r\n" );
			
			delay_ms( 10 );
		}
	
		
		
		delay_ms( 100 );*/
	}

	return 0;
}
