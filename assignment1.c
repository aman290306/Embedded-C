#include<LPC17xx.h>
unsigned int i,sw2,j;
//sw2 1 if not pressed, 0 if pressed
//Assume that the LED unit is connected to the port pins P0.4 to P0.11 and SW2 is connected  to P1.26
//pattern should be repeated as long as switch is pressed. if not pressed turn off all leds.
void read_sw2(){ 
   sw2=(LPC_GPIO1->FIOPIN>>26)&1;
}
void stop(){
  LPC_GPIO0->FIOCLR=0xFF<<4;
}
void delay(){
  for (j=0;j<20000;j++){
  read_sw2();
  if(sw2==1){
     stop();
     return;
  }
}
}
int main(){
  LPC_PINCON->PINSEL0=0x0; //0.4-0.11 as GPIO
  LPC_PINCON->PINSEL2=0x0; //1.26 as GPIO
  LPC_GPIO0->FIODIR|= 0xFF<<4; //0.4-0.11 as output
  LPC_GPIO1->FIODIR= 0; //1.26 as input 
   do{
     read_sw2(); // wait for user to press then start 
   }while(sw2==1);
while(1){
   LPC_GPIO0->FIOSET=0xFF<<4; // all leds on
   for(i=0;i<3;i++){
        read_sw2();
        if(sw2==1) {
           stop();
           return 0;
        }
        LPC_GPIO0->FIOCLR=1<<(4+i);
        LPC_GPIO0->FIOCLR=1<<(11-i);
        delay();
      }
      read_sw2();
      if(sw2==1){
         stop();
         return 0;
      }
     for(i=0;i<4;i++){
        read_sw2();
        if(sw2==1){
           stop();
           return 0;
        }
        LPC_GPIO0->FIOSET=1<<(8+i);
        LPC_GPIO0->FIOSET=1<<(7-i);
        delay();
  }
}
}
/*
1 1 1 1 1 1 1 1 
  1 1 1 1 1 1 
    1 1 1 1 
      1 1 
      1 1
    1 1 1 1 
  1 1 1 1 1 1 
1 1 1 1 1 1 1 1 
*/
