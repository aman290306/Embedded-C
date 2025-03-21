#include<LPC17xx.h>
unsigned int i,j;
unsigned int LED;
int main(){
  SystemInit();
  SystemCoreClockUpdate();
//I'm going to use 0.4-0.11 as output pins
  LPC_PINCON->PINSEL0=0x0; //Set 0.4-0.11 as GPIO
  LPC_GPIO0->FIODIR=0xFF0;  //Set 0.4-0.11 as output pins
  
while(1){
  LED=0x800;
  for(i=0;i<8;i++){
  LPC_GPIO0->FIOSET=LED;
  for(j=0;j<50000;j++);
  LPC_GPIO0->FIOCLR=0xFF0;
  LED=LED>>1;
  }
 }
}
//Output: Ring Counter using 8 leds
/*
10000000
01000000
00100000
00010000
00001000
00000100
00000010
00000001
10000000
01000000
00100000
00010000
00001000
00000100
00000010
00000001
*/
