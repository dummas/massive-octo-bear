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

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int main( void );

/* USART Bluetooth initialization */
void USART_bluetooth_init( void );
/* USART Bluetooth send */
void USART_bluetooth_send( unsigned char );
/* USART Bluetooth receive */
void USART_bluetooth_recv( unsigned char );
/* USART bluetooth check */
void USART_bluetooth_check( void );

/* USART debug initialization */
void USART_debug_init( void );
/* USART debug send */
void USART_debug_send( unsigned char );
/* USART debug receive */
void USART_debug_recv( unsigned char );

/* External ADC part */

/* The interrupt intitiative part */
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
void da_output_low( uint8_t pin ) { output_low( PORTD, pin ); }
void d_output_high( uint8_t pin ) { output_high( PORTD, pin ); }

/* Main function */
int main() {

  /* Initialize the USART for communication with Bluetooth module */
  USART_bluetooth_init( );

  USART_debug_init( );

  debug_init();

  /* Initialize the interrupts */
  ADC_Init();

  /* Enable global interrupts */
  sei();

  /* Send enable to the ADC */
  // ADC_start();

  USART_bluetooth_check();

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
  // ADC_stop();

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

  // output_low( PORTC, CS );

}


/* USART initialization */
void USART_bluetooth_init() {

  /* Enable received and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);

  /* Enable receive interrupt */
  UCSR0B |= (1 << RXCIE0);

  UCSR0A = (1 << U2X0);

  UCSR0C |= (1<< URSEL0);

  /* Use 8bit chacter sizes */
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

  /* Set baud rate */
  /* 9600 -- 16Mhz */
  UBRR0H = (68) >> 8;
  UBRR0L = (68);
  // UBRR0H = ((F_CPU / 16 + BAUD / 2) / BAUD - 1) >> 8;
  // UBRR0L = ((F_CPU / 16 + BAUD / 2) / BAUD - 1);

  d_set_output( PD0 );
  d_set_output( PD1 );

}

/* USART send byte */ 
void USART_bluetooth_send( unsigned char send_byte ) {

  /* Wait until USART is ready */
  while (( UCSR0A & (1 << UDRE0) ) == 0) {};

  /* Send the byte */
  UDR0 = send_byte;

  // USART_debug_send( send_byte );

}

void USART_bluetooth_recv( unsigned char recv_byte ) {

  /* Echo to Arduino */
  USART_debug_recv( recv_byte );
}

void USART_bluetooth_check() {

  debug_click();

  /* Send A */
  // USART_bluetooth_send(0x41);
  /* Send T */
  // USART_bluetooth_send(0x54);
  /* Send cr */
  // USART_debug_send(0x01);
  // USART_debug_send(0x10);

}


/**
* USART DEBUG
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
  /* 38400 -- 16Mhz */
  UBRR1H = (51) >> 8;
  UBRR1L = (51);

  /* Set output */
  // b_set_output( PB2 );
  // b_set_output( PB3 );

  // b_output_high( PB2 );
  // b_output_high( PB3 );

}

void USART_debug_send( unsigned char send_byte ) {

  /* Wait until USART is ready */
  while (( UCSR1A & (1<<UDRE1)) == 0) {};

  /* Send the byte */
  // UDR1 = send_byte;
  UDR1 = 0x55;

  /* Wait until USART is ready */
  // while (( UCSR1A & (1<<UDRE1)) == 0) {};

  // /* Flush the send */
  // UDR1 = 0x00;
}

void USART_debug_recv( unsigned char recv_byte ) {

  /* Echo to the Arduino */
  USART_debug_send( recv_byte );
}

/* Events */

/* USART bluetooth receive event */
ISR ( USART0_RXC_vect ) {

  unsigned char recv_byte;

  while ( !(UCSR0A & (1<<RXC0))) {};

  // Fetch the received byte value into the variable
  recv_byte = UDR0;

  // Echo back
  USART_bluetooth_recv( recv_byte );
}

/* USART debug receive event */
ISR ( USART1_RXC_vect ) {

  unsigned char recv_byte;

  while ( !(UCSR1A & (1<<RXC1))) {};

  recv_byte = UDR1;

  // delay_ms(100);

  // while (( UCSR1A & (1<<UDRE1)) == 0) {};

  /* Send the byte */
  // UDR1 = recv_byte;

  USART_debug_recv( recv_byte );
}

/* Interrupts on ADC finish */
ISR ( ADC_INTERRUPT_INT_VECTOR ) {

  ADC_read();

  // unsigned char received_byte;
  // received_byte = 0xFF;
  // USART_Send(received_byte); 
}

/**
* Delay utility
***/
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
***/
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
***/
void debug_interrupt() {
  b_output_low( DEBUG_INTERRUPT_PIN );
  b_output_high( DEBUG_INTERRUPT_PIN );
  delay_ms(100);
  b_output_low( DEBUG_INTERRUPT_PIN );
}

/**
* Sets the debug pin to low - high - low
* Indication of the step was passed
***/
void debug_click() {
  b_output_low( DEBUG_OUTPUT_PIN );
  b_output_high( DEBUG_OUTPUT_PIN );
  delay_ms(100);
  b_output_low( DEBUG_OUTPUT_PIN );
}

/**
* Debug data, coming from the ADC
***/
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