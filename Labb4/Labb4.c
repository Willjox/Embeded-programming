/*
 * File:   labb4.c
 * Author: ims20wa
 *
 * Created on October 14, 2020, 8:14 AM
 */

// PIC16F886 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define DATABUS PORTB

#define ANALOGBUS ADRESH

#define DBTRIS  TRISB

#define ADRPORT PORTC

#define APTRIS  TRISC

#define ANALOGTRIS TRISA



#define ADRENABLE RC5
#define PARLCDADR 0x00
#define PAROUTADR 0x02
#define PARINADR  0x03 //?         //Fill up with the other addr
#define _XTAL_FREQ  20000000

#define PUMPLARGE2SMALL 0xac
#define PUMPSMALL2LARGE 0xa3
#define LED 0x80
#define PUMP 0x20
#define HEAT 0x10
#define V4 0x08
#define V3 0x04
#define V2 0x02
#define V1 0x01

#define BUT 0x80

#define LEV1 0x08
#define LEV2 0x04
#define LEV3 0x02
#define LEV4 0x01
unsigned char mask = 0;
void ParallellOut( unsigned char b) {

  ADRPORT = PAROUTADR;

  DATABUS = b;

  ADRENABLE = 1;

  ADRENABLE = 0;
}



unsigned char ParallellIn() {
  ADRPORT = PARINADR;
  DBTRIS = 0xff;
  ADRENABLE = 1;
  unsigned char b = DATABUS;
  ADRENABLE = 0;
  DBTRIS = 0;
  return b;
}

void ParallellLcd(unsigned char b) {
  ADRPORT = PARLCDADR;

  DATABUS = b;

  ADRENABLE = 1;

  ADRENABLE = 0;
}


unsigned char AnalogIn() {
    unsigned char b = 0;
    ADCON0 = 0x81;
    ADCON1 = 0x00;
    GO_DONE = 1;
    while (GO_DONE) {
        __delay_ms(10);
    }
    b = ANALOGBUS;
    return b; 
}
void reset() {

  ParallellOut(0x0); //nollställ utsignaler
  //ParallellLcd(0xFF);
  unsigned char mask = 0x80; // maska parallelIn med 1000 0000,

  while(1) {
    
    if( !(ParallellIn() & mask)) {  // 1xxx xxxx knappen är inte intryckt , 0xxx intryck
      break;
    }
    __delay_ms(100);
  }
  return;

}

void fillAndEmpty() {
    mask = LEV1 + LEV2 + LEV3;
    __delay_ms(1000);
    ParallellOut(PUMP + V4 + V3);
    while((ParallellIn() & mask) != mask) {
        __delay_ms(100);
    }
    __delay_ms(100);
    ParallellOut(PUMP + V2 + V1);
        while((ParallellIn() & mask) != 0x00) {
        __delay_ms(100);
    }
    
}
unsigned char IOTEMP() {
    unsigned char temp = AnalogIn();
    ParallellLcd(temp);
    return temp;
}

void fill() {
    mask = LEV1 + LEV2 + LEV3;
    __delay_ms(1000);
    ParallellOut(PUMP + V4 + V3);
    while((ParallellIn() & mask) != mask) {
        __delay_ms(100);
    }
}

void heatStir() {
    mask =  PUMP + HEAT + V1 + V4;
    unsigned char temp = 0;
    __delay_ms(1000);
    ParallellOut(mask);
    while(IOTEMP() < 0x35) {
        __delay_ms(100);
    }
    ParallellOut(0x00);
}

void topOff() {
    mask = LEV1 + LEV2 + LEV3 + LEV4;
    __delay_ms(1000);
    ParallellOut(PUMP + V4 + V3);
    while((ParallellIn() & mask) != mask) {
        __delay_ms(100);
    }
    ParallellOut(0x00);
}
void stir() {
    mask =  PUMP + HEAT + V1 + V4;
    __delay_ms(1000);
    ParallellOut(mask);
    __delay_ms(10000);
    ParallellOut(0x00);
}

void empty() {
    mask = LEV1 + LEV2 + LEV3 + LEV4;
    __delay_ms(1000);
    ParallellOut(PUMP + V2 + V1);
    while((ParallellIn() & mask) != 0x00) {
        
        __delay_ms(100);
    }
}

void main(void) {
 ANSELH = 0;
 while(1)
 {
  DBTRIS = 0;
  APTRIS = 0;
  reset();
  fill();
  heatStir();
  topOff();
  stir();
  empty();
  //fillAndEmpty();
  }
}
