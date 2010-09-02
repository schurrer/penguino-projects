// Address of TCS3414
#define CS_SLAVE_ADDRESS (0x39<<1)

/* Command register ("address") fields */

// always set along with the CS_REG_* number, eg
// i2c_write( CS_COMMAND_CMD | CS_REG_CONTROL );
#define CS_COMMAND_CMD 0x80

// Read/write byte protocol
#define CS_COMMAND_TXN_BYTE 0x00

// Read/write word protocol
#define CS_COMMAND_TXN_WORD (1 << 5)

// Read/write block protocol
#define CS_COMMAND_TXN_BLOCK (2 << 5)

// Interrupt clear
#define CS_COMMAND_INT_CLEAR (3 << 5)


/* Register definitions */

// Control of basic functions
#define CS_REG_CONTROL 0x00

// Integration time/gain control
#define CS_REG_TIMING 0x01

// Interrupt control
#define CS_REG_INTERRUPT 0x02

// Interrupt source
#define CS_REG_INT_SOURCE 0x03

// Part number / Rev ID
#define CS_REG_ID 0x04

// ADC gain control
#define CS_REG_GAIN 0x07

// Low byte of low interrupt threshold
#define CS_REG_LOW_THRESH_LOW_BYTE 0x08

// High byte of low interrupt threshold
#define CS_REG_LOW_THRESH_HIGH_BYTE 0x09

// Low byte of high interrupt threshold
#define CS_REG_HIGH_THRESH_LOW_BYTE 0x0A

// High byte of high interrupt threshold
#define CS_REG_HIGH_THRESH_HIGH_BYTE 0x0B

// Low byte of ADC green channel
#define CS_REG_DATA1LOW 0x10
#define CS_REG_GREEN_LOW CS_REG_DATA1LOW
#define CS_REG_GREEN_WORD CS_REG_DATA1LOW

// High byte of ADC green channel
#define CS_REG_DATA1HIGH 0x11
#define CS_REG_GREEN_HIGH CS_REG_DATA1HIGH

// Low byte of ADC red channel
#define CS_REG_DATA2LOW 0x12
#define CS_REG_RED_LOW CS_REG_DATA2LOW
#define CS_REG_RED_WORD CS_REG_DATA2LOW

// High byte of ADC red channel
#define CS_REG_DATA2HIGH 0x13
#define CS_REG_RED_HIGH CS_REG_DATA2HIGH

// Low byte of ADC blue channel
#define CS_REG_DATA3LOW 0x14
#define CS_REG_BLUE_LOW CS_REG_DATA3LOW
#define CS_REG_BLUE_WORD CS_REG_DATA3LOW

// High byte of ADC blue channel
#define CS_REG_DATA3HIGH 0x15
#define CS_REG_BLUE_HIGH CS_REG_DATA3HIGH

// Low byte of ADC clear channel
#define CS_REG_DATA4LOW 0x16
#define CS_REG_CLEAR_LOW CS_REG_DATA4LOW
#define CS_REG_CLEAR_WORD CS_REG_DATA4LOW

// High byte of ADC clear channel
#define CS_REG_DATA4HIGH 0x17
#define CS_REG_CLEAR_HIGH CS_REG_DATA4HIGH


/* Register bit values */

// CONTROL register

// Power on.  Writing a 1 powers on the device, and writing a 0 turns it off.
#define CS_CONTROL_POWER 0x01

// ADC enable.  This field enables the four ADC channels to begin integration.
// Writing a 1 activates the ADC channels, and writing a 0 disables the ADCs. 
#define CS_CONTROL_ADC_EN 0x02

// ADC valid.  This read-only field indicates that the ADC channel has 
// completed an integration cycle. 
#define CS_CONTROL_ADC_VALID 0x10


// GAIN register

// GAIN[5:4]: Analog Gain Control. This field switches the common analog gain
// of the four ADC channels. Four gain modes are provided: 
#define CS_GAIN_1X (0x00<<3)
#define CS_GAIN_4X (0x01<<3)
#define CS_GAIN_16X (0x02<<3)
#define CS_GAIN_64X (0x03<<3)

// PRESCAPER[2:0]: Prescaler.  This field controls a 6-bit digital prescaler
// and divider.  The prescaler reduces the sensitivity of each ADC integrator
#define CS_PRESCALER_DIV1 (0)
#define CS_PRESCALER_DIV2 (1)
#define CS_PRESCALER_DIV4 (2)
#define CS_PRESCALER_DIV8 (3)
#define CS_PRESCALER_DIV16 (4)
#define CS_PRESCALER_DIV32 (5)
#define CS_PRESCALER_DIV64 (6)


