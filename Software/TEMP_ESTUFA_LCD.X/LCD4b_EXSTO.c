#include "LCD4b_EXSTO.h"

void lcd_init()
{
    char i;
    
    TRISD = 0b00000000;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE0 = 0;   
    PORTD = 0b00000000;
    PORTEbits.RE0 = 0;  
    PORTEbits.RE1 = 0;   
    PORTEbits.RE2 = 0;
    
    __delay_ms(15);
    
    for(i=0;i<=2;i++)
    {
        lcd_cmd(0x30);
        __delay_ms(5);
    }
    
    lcd_cmd(0x02);
    __delay_us(40);
    lcd_cmd(0x28);
    __delay_us(40);
    lcd_cmd(0x01);
    __delay_ms(2);
    lcd_cmd(0x0C);
    __delay_us(40);
    lcd_cmd(0x06);
    __delay_us(40);
}

char set_bit(char cmd, char comp)
{
    if((cmd & comp) >= 1) return 1;
    else return 0;
}

void lcd_cmd(char cmd)
{
    PORTDbits.RD4 = set_bit(cmd, 0b00010000);
    PORTDbits.RD5 = set_bit(cmd, 0b00100000);
    PORTDbits.RD6 = set_bit(cmd, 0b01000000);
    PORTDbits.RD7 = set_bit(cmd, 0b10000000);
    
    PORTEbits.RE2 = 1;
    PORTEbits.RE2 = 0;
    
    __delay_us(100);
    
    PORTDbits.RD4 = set_bit(cmd, 0b00000001);
    PORTDbits.RD5 = set_bit(cmd, 0b00000010);
    PORTDbits.RD6 = set_bit(cmd, 0b00000100);
    PORTDbits.RD7 = set_bit(cmd, 0b00001000);
    
    PORTEbits.RE2 = 1;
    PORTEbits.RE2 = 0;   
}

void lcd_send_byte(char level, char data)
{
    PORTEbits.RE0 = level;          
    __delay_us(100);
    PORTEbits.RE2 = 0;
    lcd_cmd(data);
}

void lcd_clear()
{    
    lcd_write(1,1,"               ");
    lcd_write(2,1,"               ");
}

void lcd_write(char lin, char col, char *str)
{
    char end;
    
    if(lin == 1)
    {
        end = col + 0X80 - 1;
    }
    else if(lin == 2)
    {
        end = col + 0xC0 - 1;
    }
    lcd_send_byte(0,end);
    
    while(*str)
    {
        lcd_send_byte(1,*str);
        str++;
    }
}
