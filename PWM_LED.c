#include<LPC17xx.h>
int pulseWidths[]={0,3000,6000,9000,12000,15000.18000,21000,24000.27000};
int count=1;
int dir=0; //0=inc, 1=dec
void initPWM(){
  LPC_PINCON->PINSEL3|=0x8000; //PWM1.4 
  LPC_PWM1->PR=0;
  LPC_PWM->MR0=30000;
  LPC_PWM1->MCR=0x2;
  LPC_PWM1->LER=0xFF;
  LPC_PWM1->TCR=0x2;
  LPC_PWM1->TCR=0x9; 
}
void updatePulseWidth(int pulseWidth){
  LPC_PWM1->MR2=pulseWidth;
  LPC_PWM1->LER=0xFF;
}
void delayMs(int ms){
  LPC_TIM0->CTCR=0x0;
  LPC_TIM0->PR=2;
  LPC_TIM0->TCR=0x02;
  LPC_TIM0->TCR=0x01;
  while(LPC_TIM0->TC<ms);
  LPC_TIM0->TCR=0x0;
}
int main(){
  initPWM();
  while(1){
    updatePulseWidth(pulseWidths[count]);
    delayMs(10000);
    if(count==(numPulseWidths-1)||count==0){
      dir=!dir;
    }
    if(dir) count--;
    else count++;
  }
}
