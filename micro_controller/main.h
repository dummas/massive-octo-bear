#define F_CPU 1600000 // Clock speed
#define BAUD 19200

/* Some macros */

/* ADC PD */
#define PD PC0
/* ADC RD */
#define RD PC1
/* ADC CS */
#define CS PC2
/* ADC CONVST */
#define CONVST PC3
/* ADC interrupt */
#define ADC_INTERRUPT_PIN PD2
#define ADC_INTERRUPT_INT_VECTOR INT0_vect

/* Bluetooth control */
#define BLUETOOTH_RTS_PIN PD3
#define BLUETOOTH_CTS_PIN PD4
#define BLUETOOTH_RTS_INTERRUPT_VECTOR INT1_vect
#define BLUETOOTH_RESET_PIN PD5

/* DEBUG */
#define DEBUG_OUTPUT_PIN PB0
#define DEBUG_INTERRUPT_PIN PB1
#define DEBUG_DATA_PIN PB4

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m))
#define bit_clear(p,m) ((p) &= ~(m))
#define bit_flip(p,m) ((p) ^= (m))
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))

/* Configuration matters */
#define buffer_length 126

/* SYSTEM STATES */
#define INITIALIZATION_STATE 1
#define BLUETOOTH_CHECK_STATE 2
#define BLUETOOTH_CONFIGURE_STATE 3
#define ERROR_STATE 4
#define WAIT_FOR_CONNECTION 5
#define WAIT_FOR_COMMAND 6
#define DEBUG_STATE 7
#define BLUETOOTH_RESET_STATE 8
#define DATA_START_COLLECTION_STATE 9
#define DATA_END_COLLECTION_STATE 10

#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/power.h>

/* DELAY */
void delay_ms( uint8_t );