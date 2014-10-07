#define BAUD(rate) (F_CPU / (rate * 16L) - 1)

uint8_t uart_haschar(void);
void uart_init(uint16_t);
int uart_getchar(FILE *f);
int uart_putchar(char data, FILE *f);
void uart_clear(void);
int getc_timeout(uint8_t sec);
void printx(uint16_t);
void print(uint16_t);
void printb(uint16_t number, uint8_t base);

