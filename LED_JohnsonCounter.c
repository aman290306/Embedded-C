#include<LPC17xx.h>
unsigned int i,j;
unsigned int LED;
int main(){
  SystemInit();
  SystemCoreClockUpdate();
//I'm going to use 0.4-0.11 as output pins.
  LPC_PINCON->PINSEL0=0x0; //Set 0.4-0.11 as GPIO
  LPC_GPIO0->FIODIR=0xFF0;  //Set 0.4-0.11 as output pins
while(1){
  LED=0x10;
  for(i=0;i<8;i++){
    LPC_GPIO0->FIOSET=LED; //Set 0.4-0.11 pins high
    for(j=0;j<50000;j++); //delay
    LPC_GPIO0->FIOCLR=0xFF0; //Clear 0.4-0.11 pins
    LED=LED|LED<<1;
  }
  LED=0x10;
  for(i=0;i<8;i++){
    LPC_GPIO0->FIOCLR=LED; //Set 0.4-0.11 pins high
    for(j=0;j<50000;j++); //delay
    LED=LED<<1;
  }
 }
}
//Output: Johnson Counter using 8 leds
/*
       1
      11
     111
    1111
   11111
  111111
 1111111
11111111
1111111
111111
11111
1111
111
11
1


*/
