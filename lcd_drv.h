#ifndef _lcd_drv_h_
#define _lcd_drv_h_

#include <stdio.h>

void lcd_command( u8 d );
void lcd_data( u8 d );
void lcd_text( const char *t );
void lcd_init( void );
void lcd_pos( u8 line, u8 column );
void lcd_clearrow(uint8_t r);
void lcd_setpos(uint8_t r, uint8_t c);
int lcd_putchar(char data, FILE*f);

#endif
