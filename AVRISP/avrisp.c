#include <stdio.h>

#include "penguino/uart/uart.h"
#include "penguino/uart/uart-stdio.h"
#include "penguino/spi/spi-master.h"
#include "penguino/io.h"
#include "penguino/time.h"

#define UART_BAUD_RATE 19200

ubyte supportedDevices[] = {
	0x10, // AT90S1200 rev. A     ;From 0x00-0x0f unused yet
	0x11, // AT90S1200 rev. B
	0x12, // AT90S1200 rev. C
	0x13, // AT90S1200
	0x20, // AT90S2313
	0x28, // AT90S4414
	0x30, // AT90S4433
	0x34, // AT90S2333
	0x38, // AT90S8515
	0x3A, // ATmega8515
	0x3B, // ATmega8515 BOOT
	0x41, // ATmega103
	0x42, // ATmega603
	0x43, // ATmega128
	0x44, // ATmega128 BOOT
	0x45, // ATmega64
	0x46, // ATmega64 BOOT

	0x48, // AT90S2323
	0x4C, // AT90S2343
	
	0x55, // ATtiny12
	0x56, // ATtiny15
	0x58, // ATtiny19
	0x5C, // ATtiny28
	0x5E, // ATtiny26
	0x61, // ATmega161 BOOT
	0x64, // ATmega163
	0x65, // ATmega83
	0x66, // ATmega163 BOOT
	0x67, // ATmega83 BOOT
	0x68, // AT90S8535
	0x69, // ATmega8535
	0x6c, // AT90S4434
	0x70, // AT90C8534
	0x71, // AT90C8544
	0x72, // ATmega32
	0x73, // ATmega32 BOOT
	0x74, // ATmega16
	0x75, // ATmega16 BOOT
	0x76, // ATmega8
	0x77, // ATmega8 BOOT
	0x78, // ATmega169
	0x79, // ATmega169 BOOT
};

// a string containing all supported devices
#define AVRISP_SUPPORTED_DEVICES supportedDevices

// software/hardware versions and identifiers
#define AVRISP_IDENTIFIER "AVR ISP"
#define AVRISP_SWVER "33"
#define AVRISP_HWVER "10"

// pins used
#define AVRISP_PIN_RESET 4
#define AVRISP_PIN_MOSI 5
#define AVRISP_PIN_MISO 6
#define AVRISP_PIN_SCK  7

// standard responses
#define AVRISP_PROGRAMMER_TYPE 'S'
#define AVRISP_RESP_YES 'Y'
#define AVRISP_RESP_NO 'N'
#define AVRISP_RESP_UNKNOWN '?'

// commands that don't require a device
#define CMD_SELECT_DEVICE 'T'
#define CMD_SHOW_ID 'S'
#define CMD_SOFTWARE_VERSION 'V'
#define CMD_HARDWARE_VERSION 'v'
#define CMD_LIST_SUPPORTED_DEVICES 't'
#define CMD_PROGRAMMER_TYPE 'p'
#define CMD_SUPPORTS_AUTOINCREMENT 'a'
#define CMD_SET_LED 'x'
#define CMD_CLEAR_LED 'y'

// commands that require a device
#define CMD_ENTER_PROGRAM_MODE 'P'
#define CMD_LEAVE_PROGRAM_MODE 'L'
#define CMD_WRITE_PROG_H 'C'
#define CMD_WRITE_PROG_L 'c'
#define CMD_READ_PROG 'R'
#define CMD_LOAD_ADDRESS 'A'
#define CMD_WRITE_DATA 'D'
#define CMD_READ_DATA 'd'
#define CMD_CHIP_ERASE 'e'
#define CMD_WRITE_LOCK_BITS 'l'
#define CMD_READ_SIGNATURE 's'
#define CMD_WRITE_PROGRAM_PAGE 'm'
#define CMD_UNIVERSAL_3 ':'
#define CMD_UNIVERSAL_4 '.'
#define CMD_LOAD_ADDRESS_0 '0'

// delays between commands
#define WD_EEPROM 5
#define WD_FLASH 5
#define WD_ERASE 5

// LED defs
#define LED_on statusLed_orange
#define LED_off statusLed_green

#define BYTE_CR '\r'

static volatile unsigned char selectedDevice = 0;
static volatile uint16_t currentAddress = 0;

#define currentAddressH ((currentAddress >> 8) & 0xff)
#define currentAddressL (currentAddress & 0xff)

void processCommand( unsigned char cmd );

void enableProgramMode( void );
void leaveProgramMode( void );
void writeProgH( void );
void writeProgL( void );
void readProg( void );
void loadAddress( void );
void writeData( void );
void readData( void );
void chipErase( void );
void writeLockBits( void );
void readSignature( void );
void writeProgramPage( void );
void universalWrite3( void );
void universalWrite4( void );

ubyte spi_avrTransaction( ubyte a, ubyte b, ubyte c, ubyte d );

int main( void ) {
	// initalise UART
	uart_init( UART_BAUD_RATE );
	
	statusLed_init( );
	
	// flash status LED for a bit
	statusLed_orange( );
	delay_ms( 500 );
	statusLed_green( );
	
	// uart_getc should block until data received
	uart_setBlocking( true );
	
	while ( 1 ) {
		// main loop of the programmer
		
		if ( uart_hasData( ) ) {
			unsigned char cmd = uart_getc( );
			
			processCommand( cmd );
		}
	}

	return 0;
}

void processCommand( unsigned char cmd ) {
	
	if ( cmd == CMD_SELECT_DEVICE ) {
		selectedDevice = uart_getc( );
		uart_putc( BYTE_CR );
	} else if ( cmd == CMD_SHOW_ID ) {
		uart_puts( AVRISP_IDENTIFIER );
	} else if ( cmd == CMD_SOFTWARE_VERSION ) {
		uart_puts( AVRISP_SWVER );
	} else if ( cmd == CMD_HARDWARE_VERSION ) {
		uart_puts( AVRISP_HWVER );
	} else if ( cmd == CMD_LIST_SUPPORTED_DEVICES ) {
		uart_puts( AVRISP_SUPPORTED_DEVICES );
		uart_putc( 0 ); // terminating null character needs to be sent
	} else if ( cmd == CMD_PROGRAMMER_TYPE ) {
		uart_putc( AVRISP_PROGRAMMER_TYPE );
	} else if ( cmd == CMD_SUPPORTS_AUTOINCREMENT ) {
		uart_putc( AVRISP_RESP_YES );
	} else if ( cmd == CMD_SET_LED ) {
		LED_on( );
		uart_putc( BYTE_CR );
	} else if ( cmd == CMD_CLEAR_LED ) {
		LED_off( );
		uart_putc( BYTE_CR );
	} else if ( selectedDevice == 0 ) {
		// require a device for the rest of the commands
		uart_putc( AVRISP_RESP_UNKNOWN );
		return;
	}
	
	// command not specified above
	
	if ( cmd == CMD_ENTER_PROGRAM_MODE ) {
		enableProgramMode( );
	} else if ( cmd == CMD_LEAVE_PROGRAM_MODE ) {
		leaveProgramMode( );
	} else if ( cmd == CMD_WRITE_PROG_H ) {
		writeProgH( );
	} else if ( cmd == CMD_WRITE_PROG_L ) {
		writeProgL( );
	} else if ( cmd == CMD_READ_PROG ) {
		readProg( );
	} else if ( cmd == CMD_LOAD_ADDRESS ) {
		loadAddress( );
	} else if ( cmd == CMD_WRITE_DATA ) {
		writeData( );
	} else if ( cmd == CMD_READ_DATA ) {
		readData( );
	} else if ( cmd == CMD_CHIP_ERASE ) {
		chipErase( );
	} else if ( cmd == CMD_WRITE_LOCK_BITS ) {
		writeLockBits( );
	} else if ( cmd == CMD_READ_SIGNATURE ) {
		readSignature( );
	} else if ( cmd == CMD_WRITE_PROGRAM_PAGE ) {
		writeProgramPage( );
	} else if ( cmd == CMD_UNIVERSAL_3 ) {
		universalWrite3( );
	} else if ( cmd == CMD_UNIVERSAL_4 ) {
		universalWrite4( );
	} else if ( cmd == CMD_LOAD_ADDRESS_0 ) {
		currentAddress = 0;
		uart_putc( BYTE_CR );
	}
}

void enableProgramMode( ) {
	// drive reset high initially to stop flickering
	drivePin( B, AVRISP_PIN_RESET, high );
	output_init( B, AVRISP_PIN_RESET );
	
	// initialize SPI
	spiMaster_init( spiMaster_clockDiv64 );
	
	// sink device into reset state
	delay_ms( 50 );
	drivePin( B, AVRISP_PIN_RESET, low );
	
	// enter prog mode (manually to check)
	spiMaster_transmit( 0xAC );
	spiMaster_transmit( 0x53 );
	spiMaster_transmit( 0x00 );
	ubyte verify = spiMaster_receive( );
	spiMaster_transmit( 0x00 );
	if ( verify != 0x53 ) {
		statusLed_red( );
		uart_putc( AVRISP_RESP_UNKNOWN );
		return;
	}
	
	LED_on( );
	
	uart_putc( BYTE_CR ); // done!
}

void leaveProgramMode( ) {
	spiMaster_disable( );
	drivePin( B, AVRISP_PIN_RESET, low );
	floatingInput_init( B, AVRISP_PIN_RESET );
	
	LED_off( );
	
	uart_putc( BYTE_CR ); // done!
}

void writeProgH( void ) {
	ubyte data = uart_getc( );
	
	spi_avrTransaction( 0x48, currentAddressH, currentAddressL, data );
	
	currentAddress++;
	
	//delay_ms( WD_FLASH );
	
	uart_putc( BYTE_CR ); // done!
}

void writeProgL( void ) {
	ubyte data = uart_getc( );
	
	spi_avrTransaction( 0x40, currentAddressH, currentAddressL, data );
	
	//delay_ms( WD_FLASH );
	
	uart_putc( BYTE_CR ); // done!
}

void readProg( void ) {
	ubyte data;
	
	// high byte
	data = spi_avrTransaction( 0x28, currentAddressH, currentAddressL, 0 );
	uart_putc( data );
	
	// low byte
	data = spi_avrTransaction( 0x20, currentAddressH, currentAddressL, 0 );
	uart_putc( data );
	
	currentAddress++;
}

void loadAddress( void ) {
	// read high first, then low
	ubyte addrH = uart_getc( );
	ubyte addrL = uart_getc( );
	
	currentAddress = addrH;
	currentAddress <<= 8;
	currentAddress |= addrL;
	
	uart_putc( BYTE_CR ); // done!
}

void writeData( void ) {
	ubyte data = uart_getc( );
	
	spi_avrTransaction( 0xC0, currentAddressH, currentAddressL, data );
	
	currentAddress++;
	
	delay_ms( WD_EEPROM );

	uart_putc( BYTE_CR ); // done!
}

void readData( void ) {
	ubyte data;
	
	data = spi_avrTransaction( 0xA0, currentAddressH, currentAddressL, 0 );
	uart_putc( data );
	
	delay_ms( WD_EEPROM );
	
	currentAddress++;
}

void chipErase( void ) {
	spi_avrTransaction( 0xac, 0x80, 0x04, 0x00 );
	
	delay_ms( WD_ERASE );
	
	uart_putc( BYTE_CR ); // done!
}

void writeLockBits( void ) {
	uart_putc( BYTE_CR ); // done!
}

void readSignature( void ) {
	ubyte data;
	
	data = spi_avrTransaction( 0x30, 0x00, 0x02, 0x00 );
	uart_putc( data );
	
	data = spi_avrTransaction( 0x30, 0x00, 0x01, 0x00 );
	uart_putc( data );
	
	data = spi_avrTransaction( 0x30, 0x00, 0x00, 0x00 );
	uart_putc( data );
}

void writeProgramPage( void ) {
	spi_avrTransaction( 0x4C, currentAddressH, currentAddressL, 0 );
	
	delay_ms( WD_FLASH );
	
	uart_putc( BYTE_CR ); // done!
}

void universalWrite3( void ) {
	ubyte reply;
	
	// in this order exactly -- DONT rely on order of execution in function parameters!
	ubyte a = uart_getc( );
	ubyte b = uart_getc( );
	ubyte c = uart_getc( );
	
	reply = spi_avrTransaction( a, b, c, 0 );
	uart_putc( reply );
	
	delay_ms( WD_FLASH );
	
	uart_putc( BYTE_CR ); // done!
}

void universalWrite4( void ) {
	ubyte reply;
	
	// in this order exactly -- DONT rely on order of execution in function parameters!
	ubyte a = uart_getc( );
	ubyte b = uart_getc( );
	ubyte c = uart_getc( );
	ubyte d = uart_getc( );
	
	reply = spi_avrTransaction( a, b, c, d );
	uart_putc( reply );
	
	delay_ms( WD_FLASH );
	
	uart_putc( BYTE_CR ); // done!
}


ubyte spi_avrTransaction( ubyte a, ubyte b, ubyte c, ubyte d ) {
	spiMaster_transmit( a );
	spiMaster_transmit( b );
	spiMaster_transmit( c );
	spiMaster_transmit( d );
	
	return spiMaster_receive( );
}

