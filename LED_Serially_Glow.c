#include<LPC17xx.h>
unsigned int i,j;
unsigned int glow;
int main(){
  SystemInit();
  SystemCoreClockUpdate();
//I'm going to use 0.4-0.11 as output pins.
  LPC_PINCON->PINSEL0=0x0; //Set 0.4-0.11 as GPIO
  LPC_GPIO0->FIODIR=0xFF0;  //Set 0.4-0.11 as output pins
  glow=0x10;
while(1){
  for(i=0;i<8;i++){
    LPC_GPIO0->FIOSET=glow; //Set 0.4-0.11 pins high
    for(j=0;j<50000;j++); //delay
    LPC_GPIO0->FIOCLR=0xFF0; //Clear 0.4-0.11 pins
    glow=glow<<1;
  }
  glow=0x10;
 }
}
//Output: Leds glow one by one from LSB to MSB LEDS
