
// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
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
#define SU PORTAbits.AN0 //Entrada Analógica
#define ST PORTAbits.AN1 //Entrada Analógica
#define L1 PORTBbits.RB0 //Saída Digital
#define L2 PORTBbits.RB1 //Saída Digital
#define L3 PORTBbits.RB2 //Saída Digital
#define EX1 PORTBbits.RB3 //Saída Digital
#define EX2 PORTBbits.RB4 //Saída Digital
#define RES PORTBbits.RB5 //Saída Digital
#define _XTAL_FREQ 8000000 //Delay para Clock de 8Mhz

unsigned int VdigADC_SU, VdigADC_ST; //4 Bytes de armazenamento

void config_IO(){ 

    TRISB = 0x00; //Habilita PORTB como Saída
    PORTB = 0xFF;
    
}

void config_FOSC(){

    OSCCON = 0x72; //Configura Frequência do Oscilador Interno para 8MHz
    OSCTUNE = 0x00;
    
}

void config_ADC(){

    ADCON0 = 0X01; // Seleção dos canais analógicos; Estado da conversão e Habilitação do Conversor A/D
    ADCON1 = 0X0D; // Tensão de referência; Seleção de entrada analógica
    ADCON2 = 0X87; // Alinhamento dos Bits (ADRES); Tempo de aquisição; Fonte de Clock para o converesor A/D

}

void conv_SU(){

    __delay_us(50);
    ADCON0 = 0X01; // Seleciona Canal 0 (AN0)
    ADCON0bits.GO = 1; // Inicia o ciclo de conversão
    while(ADCON0bits.GO); // Aguarda o término do ciclo de conversão
    VdigADC_SU = ADRESH; // Atribui os 2 bits + significativos do ADRES
    VdigADC_SU = (VdigADC_SU << 8) + ADRESL; // Mantém os 2 bits + significativos e soma os 8 bits - significativos do ADRES
    
}

void conv_ST(){

    __delay_us(50);
    ADCON0 = 0X05; // Seleciona Canal 1 (AN1)
    ADCON0bits.GO = 1; // Inicia o ciclo de conversão
    while(ADCON0bits.GO); // Aguarda o término do ciclo de conversão
    VdigADC_ST = ADRESH; // Atribui os 2 bits + significativos do ADRES
    VdigADC_ST = (VdigADC_ST << 8) + ADRESL; // Mantém os 2 bits + significativos e soma os 8 bits - significativos do ADRES
    
}

void main(void) 
{
    
    config_IO();
    config_FOSC();
    config_ADC();
    
    while(1)
    {
    
        conv_SU();
        conv_ST();
        
        //Controle de Umidade
        
        if(VdigADC_SU <= 101010010) //101010010 = 338 = aprox. 33% de 1023
        {
        
            L1 = 1;
            L2 = 0;
            L3 = 0;
                    
        }
        
        else if(VdigADC_SU > 101010010 && <= 1001100110) //1001100110 = 614 = aprox. 60% de 1023
        {
        
            L1 = 1;
            L2 = 1;
            L3 = 0;
        
        }
        
        else
        {
        
            L1 = 1;
            L2 = 1;
            L3 = 1;
        
        }
        
        //Controle de Temperatura
        
        if(VdigADC_ST <= 100000000) //100000000 = 256 = 25% de 1023 = 1.25V
        {
        
            EX1 = 0;
            EX2 = 0;
            RES = 1;
        
        }
        
        else if(VdigADC_ST > 100000000 && VdigADC_ST <= 1000000000) //1000000000 = 512 = 50% de 1023 = 2.5V
        
       {
        
            EX1 = 1;
            EX2 = 0;
            RES = 1;
        
       }
        
        else if(VdigADC_ST > 1000000000 && VdigADC_ST <= 1011111111) //1011111111 = 767 = 75% de 1023 = 3.75V
        
       {
        
            EX1 = 1;
            EX2 = 1;
            RES = 1;
        
       }
       
       else
       {
       
           EX1 = 1;
           EX2 = 1;
           RES = 0;
       
       }
    
    }
 
}
