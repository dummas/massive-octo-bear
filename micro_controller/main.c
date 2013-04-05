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
#define buffer_length 25

#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/power.h>

int main( void );

void clock_scale_change( void );

/* USART Bluetooth initialization */
void USART_bluetooth_init( void );
/* USART Bluetooth send */
void USART_bluetooth_send( unsigned char );
void USART_bluetooth_send_message( char* );
/* USART Bluetooth receive */
void USART_bluetooth_recv( unsigned char );
/* USART bluetooth check */
void USART_bluetooth_at_version( void );
void USART_bluetooth_at_echo( char );
void USART_bluetooth_at_discover( char );
void USART_bluetooth_at_response( char );

/* USART debug initialization */
void USART_debug_init( void );
/* USART debug send */
void USART_debug_send( unsigned char );
/* USART debug send a buffer */
void USART_debug_send_message( char * );
/* USART debug receive */
void USART_debug_recv( unsigned char );

/* External ADC part */

/* The interrupt initiative part */
void ADC_Init( void );
/* Make start of the conversion */
void ADC_start( void );
/* Read the information from the ADC */
void ADC_read( void );
/* Stop reading the information from ADC */
void ADC_stop( void );

/* DELAY */
void delay_ms( uint8_t );
/* Debug matters */
void debug_init( void );
void debug_click( void );
void debug_interrupt( void );
void debug_data( uint8_t );

void a_set_input( uint8_t );
void a_set_output( uint8_t );
void a_output_low( uint8_t );
void a_output_high( uint8_t );

void b_set_input( uint8_t );
void b_set_output( uint8_t );
void b_output_low( uint8_t );
void b_output_high( uint8_t );

void c_set_input( uint8_t );
void c_set_output( uint8_t );
void c_output_low( uint8_t );
void c_output_high( uint8_t );

void d_set_input( uint8_t );
void d_set_output( uint8_t );
void d_output_low( uint8_t );
void d_output_high( uint8_t );

void a_set_input( uint8_t pin ) { set_input( DDRA, pin ); }
void a_set_output( uint8_t pin ) { set_output( DDRA, pin ); }
void a_output_low( uint8_t pin ) { output_low( PORTA, pin ); }
void a_output_high( uint8_t pin ) { output_high( PORTA, pin ); }

void b_set_input( uint8_t pin ) { set_input( DDRB, pin ); }
void b_set_output( uint8_t pin ) { set_output( DDRB, pin ); }
void b_output_low( uint8_t pin ) { output_low( PORTB, pin ); }
void b_output_high( uint8_t pin ) { output_high( PORTB, pin ); }

void c_set_input( uint8_t pin ) { set_input( DDRC, pin ); }
void c_set_output( uint8_t pin ) { set_output( DDRC, pin ); }
void c_output_low( uint8_t pin ) { output_low( PORTC, pin ); }
void c_output_high( uint8_t pin ) { output_high( PORTC, pin ); }

void d_set_input( uint8_t pin ) { set_input( DDRD, pin ); }
void d_set_output( uint8_t pin ) { set_output( DDRD, pin ); }
void d_output_low( uint8_t pin ) { output_low( PORTD, pin ); }
void d_output_high( uint8_t pin ) { output_high( PORTD, pin ); }

char *append(const char *, char);

/*
Some global stuff
*/
/* The buffer for receiving data from the bluetooth module */
char buffer_bluetooth[buffer_length];
/* Buffer iterator */
uint8_t buffer_i;
/* Received data char */
unsigned char recv_byte;

int states[] = {
  1, /* Initial state of the system */
  2, /* Bluetooth check */
  3, /* Error state */
  4, /* Waiting for the bluetooth connection from the host */
  5, /* Bluetooth client is connected, waiting for the command */
  6, /* Collect data and send it to the bluetooth immediatelly */
};

/* Defining the initial state of the system */
int state = 0;

/* Main function */
int main() {

  /* 
  Make 8Mhz from 16Mhz to work on 3.3V,
  Scale 1
  */
  clock_prescale_set(1);

  /*
  Initialization of USART debug unit
  */
  USART_debug_init();

  /*
  Debugging units
  */
  debug_init();

  /* 
  Initialize the interrupts 
  */
  ADC_Init();

  /*
  Initialize the USART for communication with Bluetooth module
  */
  USART_bluetooth_init();

  /*
  Enable global interrupts
  */
  sei();

  // delay_ms(32000);
  // USART_debug_send(0x48);

  /* Send enable to the ADC */
  // ADC_start();

  // USART_bluetooth_send(0x48);
  
  // USART_debug_send('H');

  // USART_bluetooth_send_message("AT+NAME=patric");

  // MASTER switch
  // USART_bluetooth_send_message("AT+ROLES");

  // QUERY
  // USART_bluetooth_send_message("AT+ENQ");
  // USART_bluetooth_send_message("AT+ROLEM");
  // Modem enable
  // USART_bluetooth_send_message("AT+MODEM-");
  // USART_bluetooth_send(0x48);

  // USART_bluetooth_check_alive();

  // debug_click();
  // b_output_high( DEBUG_OUTPUT_PIN );

  USART_bluetooth_check_alive();

  /* Forever alone loop */
  for (;;) {
  }

  return 1;
}

/* ADC intialization with interrupt */
void ADC_Init() {

  /* Interrupt on failing edge */
  MCUCR = (1 << ISC01 ) | (0 << ISC00);

  /* Turn on interrupts */
  GICR |= (1 << INT0 );
  /* Set PD as output */
  c_set_output( PD );

  /* SET RD as output */
  c_set_output( RD );

  /* Set CR as output */
  c_set_output( CS );

  /* Set CONVST as output */
  c_set_output( CONVST );

  /* Set PORTA as input */
  DDRA = 0x00;

}

/* Make the ADC readout */ 
void ADC_read() {

  // uint8_t adc_value;

  /* Chip select as low */
  c_output_low( CS );
  /* Read data as low */
  c_output_low( RD );
  
  /* Read the PORTA */
  // adc_value = PINA;

  /* Make Read Data high */
  c_output_high( RD );
  /* Make Chip select high */
  c_output_high( CS );

  /* Stop ADC */
  ADC_stop();

  // delay_ms(100);

  // /* Start conversion again */
  // ADC_start();
  // debug_data(adc_value);
  
}

/* Make the ADC readout start */
void ADC_start() {

  /* MODE 2 Operation */
  c_output_low( CONVST );

  /* Pre-start configuration */
  /* Wake up ADC - high */
  c_output_high( PD );
  /* Chip select - high */
  c_output_high( CS );
  /* Read data - high */
  c_output_high( RD );
  /* Start of conversation - low */
  c_output_low( CONVST );

  /* Start of conversation - high */
  c_output_high( CONVST );
  delay_ms(10); /* t_power-up */
  /* Start of conversation - low */
  c_output_low( CONVST );

}

void ADC_stop() {

  output_low( PORTC, CS );

}

/**
* Communication with Bluetooth USART initialization
* Configuration: 19200/8-N-1
* TODO: do a better initialization set-up
***/
void USART_bluetooth_init() {

  /* Interrupt on falling edge */
  // MCUCR = (1 << ISC11 ) | (1 << ISC10);

  /* Turn on interrupts */
  // GICR |= (1 << INT1);

  /* Enable received and transmitter */
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);

  /* Enable receive interrupt */
  UCSR0B |= (1 << RXCIE0);
  /* 8 bit configuration */
  UCSR0B |= (0 << UCSZ02);
  /* No double speed option */
  UCSR0A |= (0 << U2X0);

  /* Use 8 bit configuration sizes */
  UCSR0C |= (0 << URSEL0);
  UCSR0C |= (0 << UMSEL0) | (0<<UPM00) | (0<<UPM01) | (1<<UCSZ00) | (1<<UCSZ01);

  // UBRR0H |= (1 << URSEL0);

  /* Set baud rate */
  /* 19200 -- 16Mhz */
  UBRR0H = (25) >> 8;
  UBRR0L = (25);

  d_set_output(BLUETOOTH_CTS_PIN);
  /* Set low -- make the data transfer happen */
  d_output_low(BLUETOOTH_CTS_PIN);

}

/* USART send byte */ 
void USART_bluetooth_send(unsigned char send_byte) {

  /* Wait until the register is ready */
  while ((UCSR0A & (1<<UDRE0)) == 0) {};

  /* Send the byte */
  UDR0 = send_byte;

}

/**
* Bluetooth receive event handler
**/
void USART_bluetooth_recv(unsigned char recv_byte) {
  /* Echo to debug */
  // USART_debug_send( recv_byte );

  /* Append receiving byte */
  buffer_bluetooth[buffer_i] = recv_byte;
  if (buffer_i < buffer_length) {
    /*
    We still have the space for more bytes
    */
    buffer_i = buffer_i + 1;
  } else {
    /*
    No more spaces are available, need to
    do something about it
    */
    /* Resetting the buffer */
    buffer_i = 0;
    memset(buffer_bluetooth, 0, buffer_length);
  }
  if (buffer_i > 0) {
    /* Check if the last received messages where cr,lf */
    if (buffer_bluetooth[buffer_i] == 0x0a && buffer_bluetooth[buffer_i-1] == 0x0d) {
      /* Make a message, that the buffer is fully received */
      USART_debug_send_message(buffer_bluetooth);
      /* Reseting the buffer */
      buffer_i = 0;
      memset(buffer_bluetooth, 0, buffer_length);
    }
  }
}

/**
* Check if bluetooth is alive
**/
void USART_bluetooth_at_version() {
  /* 
  * AT+VER is a basic query to check
  * if the device is alive 
  */
  USART_bluetooth_send_message("AT+VER");
}

/**
* Echo settings
* Possible options:
* - `+` -- makes echo active
* - `-` -- makes echo not active
**/
void USART_bluetooth_at_echo(char option) {
  char *message;
  /* Check the comming options */
  if (option == '+' || option == '-') {
    message = append("AT+ECHO", option);
    USART_bluetooth_send_message(message);
    free(message);
  }
}

/**
* Discover settings
* Possible options:
* - `+` -- makes discoverable
* - `-` -- makes not discoverable
**/
void USART_bluetooth_at_discover(char option) {
  char *message;
  /* Check the comming options */
  if (option == '+' || option == '-') {
    message = append("AT+DCOV", option);
    USART_bluetooth_send_message(message);
    free(message);
  }
}

/**
* Command response settings
* Possible options:
* - `+` -- makes responses active
* - `-` -- makes responses not active
**/
void USART_bluetooth_at_response(char option) {
  char *message;
  /* Check the comming options */
  if (option == '+' || option == '-') {
    message = append("AT+RESP", option);
    USART_bluetooth_send_message(message);
    free(message);
  }
}

/**
* Sending and receiving packages from the Bluetooth
**/
void USART_bluetooth_send_message(char *message) {
  
  uint8_t i = 0;
  while(message[i] != '\0') {
    /* Send the require characters */
    USART_bluetooth_send(message[i]);
    i = i + 1;
    delay_ms(1);
  }
  /* End the message with cr */
  USART_bluetooth_send(0x0d);

}


/**
* Communication with debug USART initialization
* Configuration: 19200/8-N-1
* TODO: do a better configuration set-up
***/
void USART_debug_init() {
  
  /* Enable received and transmitter */
  UCSR1B = (1<<RXEN1)|(1<<TXEN1);

  /* Enable receive interrupt */
  UCSR1B |= (1 << RXCIE1);
  UCSR1B |= (0 << UCSZ12);

  /* Normal asynchronous mode */
  UCSR1A |= (0 << U2X1);
  /* Select */
  UCSR1C = (1 << URSEL1) | (0 << UMSEL1) | (0<<UPM10) | (0<<UPM11) | (1<<UCSZ10) | (1<<UCSZ11);

  /* Asynchronous mode */
  // UCSR1C |= (0 << UMSEL1);
  /* Parity mode -- disabled */
  // UCSR1C |= (0 << UPM10);
  // UCSR1C |= (0 << UPM11);
  /* Stop bit -- 1 bit */
  // UCSR1C |= (1 << USBS1);
  /* Use 8bit character sizes */
  // UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
  // UBRR1H |= (1 << URSEL1);

  /* Set baud rate */
  /* 19200 -- 8Mhz */
  UBRR1H = (25) >> 8;
  UBRR1L = (25);

}

void USART_debug_send( unsigned char send_byte ) {

  /* Wait until USART1 is ready */
  while (( UCSR1A & (1<<UDRE1)) == 0) {};

  /* Send the byte */
  UDR1 = send_byte;

}

/**
* Sending the received package from the debug
***/
void USART_debug_send_message(char *message) {
  /* Request the line */
  d_output_low( BLUETOOTH_CTS_PIN );
  uint8_t i = 0;
  while (message[i] != '\0') {
    USART_debug_send(message[i]);
    i = i + 1;
  }
  /* END message with cr,lf */
  USART_debug_send(0x0d);
  USART_debug_send(0x0a);
}

void USART_debug_recv( unsigned char recv_byte ) {

  /* Echo to the debug port */
  USART_debug_send( recv_byte );
}

/* Events */

/* USART bluetooth receive event */
ISR ( USART0_RXC_vect ) {
  recv_byte = UDR0;
  USART_bluetooth_recv(recv_byte);
}

/* USART debug receive event */
ISR ( USART1_RXC_vect ) {
  recv_byte = UDR1;
  USART_debug_recv( recv_byte );
}

/**
* Interrupts on ADC finish 
**/
ISR ( ADC_INTERRUPT_INT_VECTOR ) {
  ADC_read();
}

/**
* RTS line request
**/
ISR( BLUETOOTH_RTS_INTERRUPT_VECTOR ) {
  d_output_low( BLUETOOTH_CTS_PIN );
}

/**
* Delay utility
**/
void delay_ms( uint8_t ms ) {
  uint16_t delay_count = F_CPU / 14500;

  volatile uint16_t i;

  while ( ms != 0 ) {
    for (i=0; i != delay_count; i++ );
    ms--;
  }
}

/**
* Initialization of debug pins
**/
void debug_init() {
  
  /* Set debug output pin as output */
  b_set_output( DEBUG_OUTPUT_PIN );
  /* Set debug interrupt pin as output */
  b_set_output( DEBUG_INTERRUPT_PIN );
  /* Set debug data pin as output */
  b_set_output( DEBUG_DATA_PIN );

  /* Set debug output pin as low */
  b_output_low( DEBUG_OUTPUT_PIN );
  /* Set debug interrupt pin as low */
  b_output_low( DEBUG_INTERRUPT_PIN );
  /* Set debug data pin as low */
  b_output_low( DEBUG_DATA_PIN );
}

/**
* Makes debug pin `interrupt`
**/
void debug_interrupt() {
  b_output_low( DEBUG_INTERRUPT_PIN );
  b_output_high( DEBUG_INTERRUPT_PIN );
  delay_ms(10);
  b_output_low( DEBUG_INTERRUPT_PIN );
}

/**
* Sets the debug pin to low - high - low
* Indication of the step was passed
**/
void debug_click() {
  b_output_low( DEBUG_OUTPUT_PIN );
  b_output_high( DEBUG_OUTPUT_PIN );
}

/**
* Debug data, coming from the ADC
**/
void debug_data( uint8_t data ) {
  
  /* Mask */
  int8_t mask = 0;
  /* Masked data */
  int8_t masked_data = 0;
  /* Loop variable */
  uint8_t i = 0;

  for (i=0; i < 8; i++) {
    mask = 1 << i;
    masked_data = data & mask;
    if (masked_data >> i == 1) {
      b_output_high( DEBUG_DATA_PIN );
    } else {
      b_output_low( DEBUG_DATA_PIN );
    }
    delay_ms(100);
    b_output_low( DEBUG_DATA_PIN );
    delay_ms(100);
  }

  b_output_low( DEBUG_DATA_PIN );
  delay_ms(100);
  b_output_high( DEBUG_DATA_PIN );
  delay_ms(100);
  b_output_low( DEBUG_DATA_PIN );
}

/**
* Append the character to the `string`
**/
char *append(const char *o, char s) {
  /* Get the length */
  int len = strlen(o);
  /* Create new buffer +2 spaces */
  char buf[len+2];
  /* Copy the original read-only `string` */
  strcpy(buf, o);
  /* Append the character */
  buf[len] = s;
  /* Add the end to the character */
  buf[len+1] = 0;
  /* Return the constructed character */
  return strdup(buf);

}
