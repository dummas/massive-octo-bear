#define FOSC 800000 // Clock speed
#define BAUD 9600
#define MUBRR FOSC/16/BAUD-1

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
#define ADC_INTERRUPT_INT INT0
#define ADC_INTERRUPT_INT_VECTOR INT0_vect

/* DEBUG */
#define DEBUG_OUTPUT_PIN 1
#define DEBUG_INTERRUPT_PIN 2

#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>


/* Main function */
int main ( void );

/* USART */

/* Usart initialization */
void USART_Init ( unsigned int );
/* Usart send */
void USART_Send ( unsigned char );

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
  // USART_Init( MUBRR );

  // debug_init();
  b_set_output(1);
  DDRB = 0xFF;/* Set everything input */
  PORTB = (1<<0); /* set high */

  // debug_click();

  /* Initialize the interrupts */
  // ADC_Init();

  /* Enable global interrupts */
  // sei();

  /* Send enable to the ADC */
  // ADC_start();

  /* Forever alone loop */
  for (;;) {
  }

  return 1;
}

/* ADC intialization with interrupt */
void ADC_Init() {
  /* Status register - SREG, enable global interrupts */
  // SREG |= (1 << 7);
  /* General Interrupt Control Register - GICR, enable PCIE1 interrupt GROUP */
  // GICR |= (1 << ADC_INTERRUPT_INT);
  /* Enable interrupt on Port C, pin 4 (pcint12) */
  // set_output( DDRD, ADC_INTERRUPT_PIN );

  // PCMSK0 |= (1<<PIND2);
  // PCMSK1 |= (1 << PCINT12);

  /* Interrupt on failing edge */
  // MCUCR = (1 << ISC01 ) | (1 << ISC00);

  /* Turn on interrupts */
  //GIMSK |= (1 << INT0 );
  set_output( DDRC, PD );

  /* SET RD as output */
  //DDRC |= (1 << 2);
  set_output( DDRC, RD );

  /* Set CR as output */
  set_output( DDRC, CS );

  /* Set CONVST as output */
  set_output( DDRC, CONVST );

  /* Set PORTA as input */
  DDRA = 0x00;

}

/* Make the ADC readout */ 
void ADC_read() {

  // unsigned char adc_value;

  // output_low( PORTC, RD );
  // output_low( PORTC, CS );

  // /* Read the PORTA */
  // adc_value = PORTA;

  // /* Make RD low */
  // output_high( DDRC, RD );
  // output_high( DDRC, CS );

  // /* Stop ADC */
  // ADC_stop();

  // delay_ms(100);

  // /* Start conversion again */
  // ADC_start();
  
}

/* Make the ADC readout start */
void ADC_start() {

  /* MODE 2 Operation */
  output_low( PORTC, CONVST );

  /* Start of conversion */
  /* Set ADC to power up */
  output_high( PORTC, PD );
  output_high( PORTC, CS );
  output_high( PORTC, RD );
  /* Start conversation */

  output_high( PORTC, CONVST );
  delay_ms(10); /* t_power-up */
  output_low( PORTC, CONVST );

  /* Make a debug interrupt */
  debug_interrupt();
}

void ADC_stop() {

  // output_low( PORTC, CS );

}


/* USART initialization */
void USART_Init( unsigned int ubrr ) {
  /* Set baud rate */
  UBRR0H = (unsigned char) (ubrr>>8);
  UBRR0L = (unsigned char) ubrr;
  /* Enable received and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Enable receive interrupt */
  UCSR0B |= ( 1 << RXCIE0 );
  /* Use 8bit chacter sizes */
  UCSR0C = ( 1 << URSEL0 ) | ( 1 << UCSZ00 ) | ( 1 << UCSZ01 );
}

/* USART send byte */ 
void USART_Send( unsigned char send_byte ) {

  /* Wait until USART is ready */
  while (( UCSR0A & (1 << UDRE0 ) ) == 0) {};

  /* Send the byte */
  UDR0 = send_byte;

}

/* Events */

/* USART receive event */
ISR ( USART0_RXC_vect ) {

  unsigned char received_byte;

  // Fetch the received byte value into the variable
  received_byte = UDR0;

  // Echo back
  USART_Send( received_byte );

}

/* Interrupts on ADC finish */
ISR ( ADC_INTERRUPT_INT_VECTOR ) {

  // debug_click();

  // unsigned char received_byte;
  
  // received_byte = 0xFF;

  // USART_Send(received_byte);
  
}

/**
* Delay utility
***/
void delay_ms( uint8_t ms ) {
  uint16_t delay_count = FOSC / 17500;

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
  set_output( DDRB, DEBUG_OUTPUT_PIN );
  /* Set debug interrupt pin as output */
  set_output( DDRB, DEBUG_INTERRUPT_PIN );

  /* Set debug output pin as low */
  output_low( PORTB, DEBUG_OUTPUT_PIN );
  /* Set debug interrupt pin as low */
  output_low( PORTB, DEBUG_INTERRUPT_PIN );
}

/**
* Makes debug pin `interrupt`
***/
void debug_interrupt() {
  
  output_low( PORTB, DEBUG_INTERRUPT_PIN );
  delay_ms(10);
  output_high( PORTB, DEBUG_INTERRUPT_PIN );
  delay_ms(10);
  output_low( PORTB, DEBUG_INTERRUPT_PIN );

}

/**
* Sets the debug pin to low - high - low
* Indication of the step was passed
***/
void debug_click() {
  output_low( PORTB, DEBUG_OUTPUT_PIN );
  delay_ms(100);
  output_low( PORTB, DEBUG_OUTPUT_PIN );
  delay_ms(100);
  output_low( PORTB, DEBUG_OUTPUT_PIN );
  delay_ms(100);
}