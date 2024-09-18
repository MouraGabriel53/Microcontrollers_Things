/*
 * File:   newmain.c
 * Author: gabri
 *
 * Created on 9 de Agosto de 2024, 08:44
 */


#include <xc.h>
#define S1 PORTBbits.RB0 //Sa�da Digital 
#define S2 PORTBbits.RB1 //Sa�da Digital 
#define S3 PORTBbits.RB2 //Sa�da Digital  
#define S4 PORTBbits.RB3 //Sa�da Digital   
#define _XTAL_FREQ 8000000 //Delay para Clock de 8Mhz

float pressao, tensao; 

unsigned int VdigADC_SP; //4 Bytes de armazenamento

void config_IO(){

    TRISB = 0x00; //Habilita PORTB como Sa�da
    PORTB = 0xFF;
    
}

void config_FOSC(){

    OSCCON = 0x00; //Configura Frequ�ncia do Oscilador Externo para 8MHz
    OSCTUNE = 0x00;
    
}

void config_ADC(){

    ADCON0 = 0X01; // Sele��o dos canais anal�gicos; Estado da convers�o e Habilita��o do Conversor A/D
    ADCON1 = 0X0D; // Tens�o de refer�ncia; Sele��o de entrada anal�gica
    ADCON2 = 0X87; // Alinhamento dos Bits (ADRES); Tempo de aquisi��o; Fonte de Clock para o converesor A/D

}

void conv_DIG(){

    __delay_us(50);
    ADCON0 = 0X01; // Seleciona Canal 0 (AN0)
    ADCON0bits.GO = 1; // Inicia o ciclo de convers�o
    while(ADCON0bits.GO); // Aguarda o t�rmino do ciclo de convers�o
    VdigADC_SP = ADRESH; // Atribui os 2 bits + significativos do ADRES
    VdigADC_SP = (VdigADC_SP << 8) + ADRESL; // Mant�m os 2 bits + significativos e soma os 8 bits - significativos do ADRES
    
}

void conv_Pressao(){

    tensao = VdigADC_SP * (4.84e-3);
    pressao = tensao / 0.07692;
    
}

void main(void) 
{
    
    config_IO();
    config_FOSC();
    config_ADC();
    
    while(1)
    {
    
        conv_DIG();
        conv_Pressao();
        
        //Controle de Press�o
        
        if(pressao <= (0.25 * 65)) 
        {
        
            S1 = 1;
            S2 = 0;
            S3 = 0;
            S4 = 0;
                    
        }
        
        else if(pressao > (0.25 * 65) && pressao <= (0.50 * 65))
        {
        
            S1 = 1;
            S2 = 1;
            S3 = 0;
            S4 = 0;
        
        }
        
        else if (pressao > (0.50 * 65) && pressao <= (0.75 * 65))
        {
        
            S1 = 1;
            S2 = 1;
            S3 = 1;
            S4 = 0;
        
        }
        
        else if (pressao > (0.75 * 65))
        {

            S1 = 1;
            S2 = 1;
            S3 = 1;
            S4 = 1;

        }
    
    }
 
}