/**
 * External ADC
 */

/* The interrupt initiative part */
void ADC_init( void );
/* Make start of the conversion */
void ADC_start( void );
/* Read the information from the ADC */
uint8_t ADC_read( void );
/* Stop reading the information from ADC */
void ADC_stop( void );