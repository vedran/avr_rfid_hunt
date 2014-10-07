#include "main.h"
#include <stdio.h>
#include <util/delay.h>
#include "lcd_drv.h"
#include "uart.h"
#include <string.h>

static FILE lcd_stdio = FDEV_SETUP_STREAM(lcd_putchar, uart_getchar, _FDEV_SETUP_RW);

#define NUM_HINTS 10
#define ID_LEN 4

typedef struct {
    char id[ID_LEN];
    const char* hint;
} hint;

#define ID(i) {(i>>24)&0xff, (i>>16)&0xff, (i>>8)&0xff, i&0xff}
hint hints[] = {
    {ID(0x9464b8), "Hint1"},
    {ID(0xfff9c2), "Hint2"},
    {ID(0xb9a964), "Hint3"},
    {ID(0xfea258), "Hint4"},
    {ID(0xb9a8d4), "Hint5"},
    {ID(0x9472bd), "Hint6"},
    {ID(0xb9c5c8), "Hint7"},
    {ID(0x94a975), "Hint8"},
    {ID(0xfe9735), "Hint9"},
    {ID(0xb9a512), "Hint10"}
};

int lookup_id(char *id) {
    for(uint8_t i = 0; i < NUM_HINTS; i++) {
        // we have a match
        if(memcmp(id, hints[i].id, ID_LEN) == 0) {
            return i;
        }
    }
    return -1; // no matching entry
}

int main(void) {
    uart_init(BAUD(9600));
    uart_clear();

    lcd_init();
    lcd_setpos(0, 0);

    stdout = &lcd_stdio;

    _delay_ms(500);

    // add 1 for the null terminator
    char id[ID_LEN + 1];
    memset(id, 0, sizeof(char) * (ID_LEN + 1));

    for(;;) {
        if(!(PINB & (1<<PIN0))) {
            fgets(id, sizeof(id), stdin);
            int hint_id = lookup_id(id);

            if(hint_id >= 0) {
                lcd_clearrow(0);
                lcd_clearrow(1);
                lcd_setpos(0, 0);
                printf("%s", hints[hint_id].hint);
            }
        }
    }
}
