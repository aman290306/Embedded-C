// Display 1234 in SSD 
#include<LPC17xx.h>
int i,j;
unsigned int seg[4]={0x06,0x5B,0x4F,0x66};
int main(){
  LPC_PINCON->PINSEL0=0x0;
  LPC_PINCON->PINSEL1=0x0;
  LPC_PINCON->PINSEL3=0x0;
  LPC_GPIO0->FIODIR=0x00078000;//0.15-0.18 as Enable lines
  LPC_GPIO1->FIODIR=0x7F800000;// 1.23-1.30 as data lines
while(1){
  for(i=0;i<4;i++){
    LPC_GPIO0->FIOPIN=i<<15;
    LPC_GPIO1->FIOPIN=seg[i]<<23;
    for(j=0;j<5000;j++);
  } 
  }
}
