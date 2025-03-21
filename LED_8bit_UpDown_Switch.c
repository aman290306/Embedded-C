#include<LPC17xx.h>
unsigned int i,j;
unsigned int count;
int main(){
  SystemInit();
  SystemCoreClockUpdate();
//I'm going to use 0.4-0.11 as output pins and 2.12 as switch input pin which is active low
  LPC_PINCON->PINSEL0=0x0; //Set 0.4-0.11 as GPIO
  LPC_PINCON->PINSEL4=0x0;// Set 2.12 as GPIO
  LPC_GPIO2->FIODIR=0x0; // Set2.12 as input pin 
  LPC_GPIO0->FIODIR=0xFF0;  //Set 0.4-0.11 as output pins
  count=0;
while(1){
  if(!(LPC_GPIO2->FIOPIN>>12 & 1)){//Switch is pressed - Down 
    if (count == 0) count = 255;
    LPC_GPIO0->FIOSET=count<<4;
    for(j=0;j<50000;j++);
    LPC_GPIO0->FIOCLR=0xFF0;
    count --;
  }
  else{
  if (count == 255) count = 0; //Switch not pressed - Up
  LPC_GPIO0->FIOSET=count<<4;
  for(j=0;j<50000;j++);
  LPC_GPIO0->FIOCLR=0xFF0;
  count ++;
  }
 }
}
//Output: Up-down Counter using 8 leds
