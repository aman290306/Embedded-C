/*Write an embedded C program to rotate a stepper motor by 90 degrees in both clockwise 
and counterclockwise directions using full-step control. Assume port pins 3.23 to 3.26 are 
connected to the stators of the stepper motor. Use timer0 for delay assuming PCLK=25 
MHz and Timer resolution as 100 microseconds.*/
#include<LPC17xx.h>
int i;
void initTimer0(){
  LPC_TIM0->CTCR=0x0; //timer mode
  LPC_TIM0->TCR=0x02; //reset timer
  LPC_TIM0->PR=2499; 
}
void delay(){
  LPC_TIM0->TCR=0x02; //reset timer;
  LPC_TIM0->TCR=0x01; // enable timer
  while(LPC_TIM0->TC<1); //100micro s delay
}
void clockwise_step(){ // takes 1 step
  if((LPC_GPIO3->FIOPIN>>26)&1){ //if D is on next step should D off and A on
    LPC_GPIO3->FIOPIN=1<<23;
    delay();
    return;
  }
  LPC_GPIO3->FIOPIN=LPC_GPIO3->FIOPIN<<1;
  delay();
}
void anti_clockwise_step(){ //takes 1 step
  if((LPC_GPIO3->FIOPIN>>23)&1){ //if A is on next step should A off and D on
    LPC_GPIO3->FIOPIN=1<<26;
    delay();
    return;
  }
  LPC_GPIO3->FIOPIN=LPC_GPIO3->FIOPIN>>1;
  delay();
}
int main(){
  SystemInit();
  SystemCoreClockUpdate();
  initTimer0();
  LPC_PINCON->PINSEL7=0x0;
  LPC_GPIO3->FIODIR=0xF<<23;
while(1){
  for(i=0;i<50;i++)//50 steps
    clockwise_step();
  for(i=0;i<50;i++)//50 steps
    anti_clockwise_step();
 }
}

