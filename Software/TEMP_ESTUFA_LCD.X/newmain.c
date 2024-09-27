// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include "LCD4b_EXSTO.h"
#define B1 PORTBbits.RB0
#define _XTAL_FREQ 8000000

unsigned int VdigADC_AN3;
     
char string_temp[16],
     string_percent[16],
     string_FAN1[8],
     string_FAN2[8],
     string_RES1[8];

float sensor_temp = 0,
      sensor_tensao = 0,
      percent = 0;
      
int FAN1 = 0,
    FAN2 = 0,
    RES1 = 0;

void config_FOSC()
{
    OSCCON = 0X00; //CONFIGURA OCILADOR EXTERNO
    OSCTUNE = 0X00; //DESABILITA AJUSTE DE COMPENSAÇÃO
}

void config_IO()
{
    TRISB = 0X01; //SELECIONA ENTRADA RBO
    PORTB = 0X00;
    TRISC = 0X00; //DEFINE PORTC COMO SAIDA
    PORTC = 0X00;
}

void config_ADC()
{
    ADCON0 = 0X0D; //SELEÇAO DO CANAL AN3 (0011), STATUS DE CONVERSAO, HABILITA O CONVERSOR     
    ADCON1 = 0X0B;  //TENSAO DE REFERENCIA, SELECIONA AN3 (1011) COMO ENTRADA ANALOGICA    
    ADCON2 = 0X80;  //ALINHAMENTO, TEMPO DE AQUISICAO, FONTE DO CLOCK PARA O A/D     
}

void conv_AN3()
{
    __delay_ms(50);
    ADCON0bits.GO = 1; //INICIA O CICLO DE CONVERCAO                              
    while(ADCON0bits.GO); //AGUARDA A CONVERSAO -> [(5 - TENSAO)/1023]                          
    VdigADC_AN3 = ADRESH; //ALINHA À DIREITA                         
    VdigADC_AN3 = (VdigADC_AN3 << 8) + ADRESL; //SOMA COM A ESQUERDA      
}

void conv_TEMP()
{
    sensor_tensao = VdigADC_AN3 * 0.0048875855327468; //5 - 0/1023 (TENSAO DE SAIDA Vout) -> CONVERTE ANALOGICO NOVAMENTE
    sensor_temp = sensor_tensao / 0.0119047619047619; //5 - 0/420-0 -> CONSTANTE EM FUNÇÃO DA TEMPEATURA
    percent = (sensor_temp * 100)/ 420; //conversao para porcentagem 
}

void logica_controle()
{
    if(sensor_temp <= (0.25*420))
    {
        FAN1 = 0;
        FAN2 = 0;
        RES1 = 1;
    }
    else if(sensor_temp > (0.25*420) && sensor_temp <= (0.50*420))
    {
        FAN1 = 1;
        FAN2 = 0;
        RES1 = 1;
    }
    else if(sensor_temp > (0.50*420) && sensor_temp <= (0.75*420))
    {
        FAN1 = 1;
        FAN2 = 1;
        RES1 = 1;
    }
    else if(sensor_temp > (0.75*420))
    {
        FAN1 = 1;
        FAN2 = 1;
        RES1 = 0;
    }
    
    PORTCbits.RC0 = FAN1;
    PORTCbits.RC1 = FAN2;
    PORTCbits.RC2 = RES1;
        
}

void LCD_tela1(){
    
    sprintf(string_temp,"%0.1f%cC", sensor_temp, 0xDF); //CONVERTE PARA STRING, ADICIONA CARACTERE ESPECIAL
    sprintf(string_percent,"%0.1f%c", percent, 0x25); //1101 + 1111 = DF & 0010 + 0101 = 25
   
    lcd_clear();
    __delay_ms(20);
    
    lcd_write(1,1,"TEMP:                  "); //IMPRIME NO LCD
    lcd_write(1,10,string_temp);
    
    lcd_write(2,1,"PERCENT:               ");  
    lcd_write(2,10,string_percent);
    
}

void LCD_tela2(){
    
    sprintf(string_FAN1, "%01d", FAN1); //CONVERTE PARA STRING
    sprintf(string_FAN2, "%01d", FAN2);
    sprintf(string_RES1, "%01d", RES1);
        
    lcd_clear();
    __delay_ms(20);
    
    lcd_write(1,1,"FAN 1:          "); //IMPRIME NO LCD
    lcd_write(1,7,string_FAN1);
    
    lcd_write(1,9,"FAN 2:          ");
    lcd_write(1,15,string_FAN2);
    
    lcd_write(2,1,"RES1:          ");
    lcd_write(2,6,string_RES1);
    
}

void main(void) {
    
    config_FOSC();
    config_IO();
    config_ADC();
    lcd_init();
        
    while(1)
    {
        conv_AN3();
        conv_TEMP();
        logica_controle();
        
        if(B1 == 0) {
            LCD_tela1();
        } 
        else if(B1 == 1){
            LCD_tela2();
        }
    }        
}
