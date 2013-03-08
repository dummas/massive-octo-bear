/* Main function */
int main ();

/* USART */

/* USART debug initialization */
void USART_debug_init( unsigned int );
/* USART debug send */
void USART_debug_send( unsigned char );
/* USART debug receive */
void USART_debug_recv( void );

/* USART Bluetooth initialization */
void USART_bluetooth_init( unsigned int );
/* USART Bluetooth send */
void USART_bluetooth_send( unsigned char );
/* USART Bluetooth receive */
void USART_bluetooth_recv( void );