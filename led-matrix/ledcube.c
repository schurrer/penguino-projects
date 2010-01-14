#include <stdio.h>
#include <string.h>

#include "penguino/uart/uart.h"
#include "penguino/uart/uart-stdio.h"
#include "penguino/io.h"
#include "penguino/time.h"

#define UART_BAUD_RATE 115200
#define NUM_LAYERS 4

#define MAX_FRAMES 10

typedef uint16_t frame_t[NUM_LAYERS];

typedef struct {
	int frameDelay;
	
	int numFrames;
	
	frame_t frames[MAX_FRAMES];
} effect_t;

void output_cube( frame_t *frame ) {
	for ( int layer = 0; layer < NUM_LAYERS; ++layer ) {
		uint8_t layerPin = D2 + layer;
		drivePin( layerPin, High );
		
		drivePort( B, (*frame)[layer] & 0xFF );
		drivePort( A, (*frame)[layer] >> 8 );
		
		delay_ms( 1 );
		drivePin( layerPin, Low );
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
	
	// init ground layers
	output_init( D2 );
	output_init( D3 );
	output_init( D4 );
	output_init( D5 );
	
	drivePin( D2, Low );
	drivePin( D3, Low );
	drivePin( D4, Low );
	drivePin( D5, Low );
	
	//uint8_t layer = D2;
	
	DDRB  = 0xFF;
	DDRA  = 0xFF;
	//PORTB = 0x10;
	//PORTA = 0x00;
	
	//uint16_t ledId = 0;
	
	//uint16_t cube[4] = {0x000F, 0x00F0, 0x0F00, 0xF000};
	frame_t *currentDisplay = NULL;
	
	
	
	effect_t x_spin = {
		100, // 100ms per frame
		6, // number of frames
		{
			{0x000F, 0x00F0, 0x0F00, 0xF000},
			{0x00F0, 0x00F0, 0x0F00, 0x0F00},
			{0x0F00, 0x0F00, 0x00F0, 0x00F0},
			{0xF000, 0x0F00, 0x00F0, 0x000F},
			{0x0000, 0xFF00, 0x00FF, 0x0000},
			{0x0000, 0x00FF, 0xFF00, 0x0000},
		}
	};
	
	effect_t y_spin = {
		100, // 100ms per frame
		6, // number of frames
		{
			{0x1248, 0x1248, 0x1248, 0x1248},
			{0x03C0, 0x03C0, 0x03C0, 0x03C0},
			{0x0C30, 0x0C30, 0x0C30, 0x0C30},
			{0x8421, 0x8421, 0x8421, 0x8421},
			{0x4422, 0x4422, 0x4422, 0x4422},
			{0x2244, 0x2244, 0x2244, 0x2244},
		}
	};
	
	effect_t z_spin = {
		100, // 100ms per frame
		6, // number of frames
		{
			{0x1111, 0x2222, 0x4444, 0x8888},
			{0x0000, 0x3333, 0xCCCC, 0x0000},
			{0x0000, 0xCCCC, 0x3333, 0x0000},
			{0x8888, 0x4444, 0x2222, 0x1111},
			{0x4444, 0x4444, 0x2222, 0x2222},
			{0x2222, 0x2222, 0x4444, 0x4444},
		}
	};
	
	#define NUM_EFFECTS 3
	
	effect_t *effects[NUM_EFFECTS] = {
		&x_spin,
		&y_spin,
		&z_spin
	};
	
	int currentEffectNum = 0;
	effect_t *currentEffect = &z_spin;
	
	uint16_t counter = 0;
	uint8_t frame = 0;
	
	uint8_t effect_iterations = 0;
	
	while ( 1 ) {
		
		counter++;
		if ( currentDisplay == NULL || counter >= currentEffect->frameDelay/NUM_LAYERS ) { // 400ms
			//memcpy( cube, currentEffect->frames[frame], sizeof(uint16_t)*NUM_LAYERS );
			currentDisplay = &currentEffect->frames[frame];
			
			frame++;
			frame %= currentEffect->numFrames;
			
			if ( frame == 0 ) {
				effect_iterations++;
				
				if ( effect_iterations == 4 ) {
					currentEffectNum++;
					currentEffectNum %= NUM_EFFECTS;
					
					currentEffect = effects[currentEffectNum];
					
					effect_iterations = 0;
				}
			}
			
			counter = 0;
		}
		
		output_cube( currentDisplay );
		
		/*
		if ( ledId == 0 ) {
			ledId = 1;
		
			drivePin( layer, Low );
			layer++;
			if ( layer > D5 ) layer = D2;
			drivePin( layer, High );
		}
		
		PORTB = ledId & 0xFF;
		PORTA = ledId >> 8;
		
		ledId <<= 1;
		
		delay_ms( 200 );
		*/
	}

	return 0;
}
