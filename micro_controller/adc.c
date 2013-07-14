#include "main.h"
#include "ports.h"
#include "adc.h"

/* ADC intialization with interrupt */
void ADC_init() {

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