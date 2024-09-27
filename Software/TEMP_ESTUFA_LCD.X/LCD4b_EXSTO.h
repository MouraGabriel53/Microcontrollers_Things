#include <xc.h> 
#define _XTAL_FREQ 8000000

#define EN_DIR TRISEbits.TRISE2
#define RW_DIR TRISEbits.TRISE1
#define RS_DIR TRISEbits.TRISE0             
#define DT4_DIR TRISDbits.TRISD4
#define DT5_DIR TRISDbits.TRISD5
#define DT6_DIR TRISDbits.TRISD6
#define DT7_DIR TRISDbits.TRISD7

#define EN PORTEbits.RE2
#define RW PORTEbits.RE1
#define RS PORTEbits.RE0                    
#define DT4 PORTDbits.RD4
#define DT5 PORTDbits.RD5
#define DT6 PORTDbits.RD6
#define DT7 PORTDbits.RD7

void lcd_init(void);
char set_bit(char cmd, char comp);
void lcd_cmd(char cmd);
void lcd_send_byte(char level, char data);
void lcd_clear(void);
void lcd_write(char lin, char col, char *str);
