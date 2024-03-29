#include "main.h"
#include "lcd_drv.h"
#include <util/delay.h>
#include <stdio.h>

static volatile uint8_t row;
static volatile uint8_t col;

// clear the specified row
// and leave the LCD position there
void lcd_clearrow(uint8_t r) {
    row = r;
    lcd_pos(row, 0);
    lcd_text("                ");
}
void lcd_setpos(uint8_t r, uint8_t c) {
    row = r;
    col = c;
    lcd_pos(row, col);
}


int lcd_putchar(char data, FILE*f) {
    if(data == '\n') {
        if(row == 1) {
            _delay_ms(3000);
            lcd_clearrow(0);
            lcd_clearrow(1);
            lcd_setpos(0, 0);
        }
        else {
            lcd_setpos(1, 0);
        }
    }
    else if(col == 16) {
        if(row == 0) {
            lcd_setpos(1, 0);
            lcd_data(data);
            col++;
        }
        else {
            _delay_ms(3000);
            lcd_clearrow(0);
            lcd_clearrow(1);
            lcd_setpos(0, 0);
            lcd_data(data);
            col++;
        }
    }
    else {
        lcd_data(data);
        col++;
    }
    return 1;
}

static void lcd_nibble( u8 d )
{
  LCD_D4 = 0; if( d & 1<<4 ) LCD_D4 = 1;
  LCD_D5 = 0; if( d & 1<<5 ) LCD_D5 = 1;
  LCD_D6 = 0; if( d & 1<<6 ) LCD_D6 = 1;
  LCD_D7 = 0; if( d & 1<<7 ) LCD_D7 = 1;

  LCD_E0 = 1;
  _delay_us( 1 );			// 1us
  LCD_E0 = 0;
}


static void lcd_byte( u8 d )
{
  lcd_nibble( d );
  lcd_nibble( d<<4 );
  _delay_us( 45 );			// 45us
}


void lcd_command( u8 d )
{
  LCD_RS = 0;
  lcd_byte( d );
  switch( d ){
    case 1:
    case 2:
    case 3: _delay_ms( 2 );		// wait 2ms
  }
}


void lcd_data( u8 d )
{
  LCD_RS = 1;
  lcd_byte( d );
}


void lcd_text( const char *t )
{
  while( *t ){
    lcd_data( *t );
    t++;
  }
}


void lcd_init( void )
{
  LCD_DDR_D4 = 1;
  LCD_DDR_D5 = 1;
  LCD_DDR_D6 = 1;
  LCD_DDR_D7 = 1;
  LCD_DDR_RS = 1;
  LCD_DDR_E0 = 1;

  LCD_E0 = 0;
  LCD_RS = 0;				// send commands
  _delay_ms( 15 );			// wait 15ms

  lcd_nibble( 0x30 );
  _delay_ms( 5 );			// wait >4.1ms

  lcd_nibble( 0x30 );
  _delay_us( 100 );			// wait >100�s

  lcd_nibble( 0x30 );			// 8 bit mode
  _delay_us( 100 );			// wait >100�s

  lcd_nibble( 0x20 );			// 4 bit mode
  _delay_us( 100 );			// wait >100�s

  lcd_command( 0x28 );			// 2 lines 5*7
  lcd_command( 0x08 );			// display off
  lcd_command( 0x01 );			// display clear
  lcd_command( 0x06 );                  // cursor increment
  lcd_command( 0x0C );			// on, no cursor, no blink
}


void lcd_pos( u8 line, u8 column )
{
  if( line & 1 )
    column += 0x40;

  lcd_command( 0x80 + column );
}
