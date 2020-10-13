#include <xc.h>
 //Definitions for protoboard. Compare with drawing!
#define DATABUS PORTB
#define DBTRIS  TRISB
#define ADRPORT PORTC
#define APTRIS  TRISC
#define ADRENABLE RC5
#define PAROUTADR 2
#define PARINADR  0 //?         //Fill up with the other addr
#define _XTAL_FREQ  4000000
void ParallellOut( unsigned char b) {
  ADRPORT = PAROUTADR;
  DATABUS = b;
  ADRENABLE = 1;
  ADRENABLE = 0;
}

unsigned char ParallellIn( ) {
  ADRPORT = PARINADR;
  DBTRIS = 1;
  ADRENABLE = 1;
  unsigned char b = DATABUS;
  ADRENABLE = 0;
  DBTRIS = 0;
  return b
}

bolean reset() {
  parallelOut(0x0); //nollställ utsignaler
  unsigned char mask = 0x8; // maska parallelIn med 1000,
  while(1) {
    if( !(parallellIn & mask)) {  // 1xxx knappen är inte intryckt , 0xxx intryckt
      break;
    }
  }
  return true;
}

void main(void) {
  unsigned char x;
  DBTRIS = 0;
  APTRIS = 0;
  while(1) {
    ParallellOut(x);
    x++;
    __delay_ms(1000);
  }
  return;
