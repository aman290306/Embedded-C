// 4 digit hexadecimal up/down counter 
#include<LPC17xx.h>
int d[4]={0,0,0,0},value,i;
//65535=0xFFFF
unsigned int seg[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79};
void initTimers(){
  LPC_TIM0->CTCR=0x0;
  LPC_TIM0->TCR=0x2;
  LPC_TIM0->PR=11999; // Pclk=12MHz, TC increases every ms 
 }
void delay(int ms){
    LPC_TIM0->TCR=0x2; //reset timer
    LPC_TIM0->TCR=0x1; //enable timer
    while(LPC_TIM0->TC<ms);
    LPC_TIM0->TCR=0x0; //disable timer
}
void display(value){
  int j;
  for(j=0;j<4;j++){
    d[j]=value%16;
    value=value/16;
  }
  for(i=0;i<4;i++){
    LPC_GPIO0->FIOPIN=i<<15;
    LPC_GPIO1->FIOPIN=seg[d[i]]<<23;
    delay(500);
  }
}
int main(){
  LPC_PINCON->PINSEL0=0x0;
  LPC_PINCON->PINSEL1=0x0;
  LPC_PINCON->PINSEL3=0x0;
  LPC_PINCON->PINSEL4=0x0;
  LPC_GPIO0->FIODIR=0x00078000;//0.15-0.18 as Enable lines
  LPC_GPIO1->FIODIR=0x7F800000;// 1.23-1.30 as data lines
  LPC_GPIO2->FIODIR=0x0;// 2.12 as switch input
  value=0;
  initTimers();
while(1){
  if(!((LPC_GPIO2->FIOPIN>>12) & 1)){//pressed
    if (value==0) value=65535;
    else value=value-1;
  }
  else{
    if(value==65535) value=0;
    else value=value+1;
  }
  display(value);
}
}
