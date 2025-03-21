#include<LPC17xx.h>
unsigned int i,j;
int main(){
  SystemInit();
  SystemCoreClockUpdate();
//I'm going to use 0.4-0.11 as output pins.
  LPC_PINCON->PINSEL0=0x0; //Set 0.4-0.11 as GPIO
  LPC_GPIO0->FIODIR=0xFF0;  //Set 0.4-0.11 as output pins
while(1){
  for(i=0;i<10;i++){
    LPC_GPIO0->FIOSET=0xFF0; //Set 0.4-0.11 pins high
    for(j=0;j<50000;j++); //delay
    LPC_GPIO0->FIOCLR=0xFF0; //Set 0.4-0.11 pins Low
    for(j=0;j<50000;j++); //delay
  }
  }
}
//Output: all 8 LEDS should blink 10times
