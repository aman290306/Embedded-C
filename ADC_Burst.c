#include<stdio.h>
#include<LPC17xx.h>
// include lcd functions
int main(){
  unsigned int i;
  float in_vtg4,in_vtg5;
  unsigned long adc_temp4,adc_temp5,status;
  char dval4[7],vtg4[7],dval5[7],vtg5[7];
  LPC_PINCON->PINSEL3=0xF<<28;
  while(1){
  LPC_ADC->ADCR=(1<<4)|(1<<5)|(1<<16)|(1<<21); //burst mode
  LPC_ADC->ADINTEN=(1<<4)|(1<<5);
  NVIC_EnableIRQ(ADC_IRQn);
  }
}
void ADC_IRQHandler(){
  status=(LPC_ADC->ADSTAT>>4)&3;
  if(status&1){
    adc_temp4=(LPC_ADC->ADDR4>>4)&0xFFF;
    sprintf(dval4,"%lu",adc_temp4);
    in_vtg4=(float)((adc_temp4*3.300)/4096);
    sprintf(vtg4,"%3.2fV",in_vtg4);
    alphanum(dval4);
    lcd_comdata(0x89,0);
    delay_lcd(800);
    alphanum(vtg4);
  }
  if((status>>1)&1){
  adc_temp5=(LPC_ADC->ADDR5>>4)&0xFFF;
  sprintf(dval5,"%lu",adc_temp5);
  in_vtg5=(float)((adc_temp5*3.300)/4096);
  sprintf(vtg5,"%3.2fV",in_vtg5);
  lcd_comdata(0xC0,0);
  delay_lcd(800);
  alphanum(dval5);
  lcd_comdata(0xC9,0);
  delay_lcd(800);
  alphanum(vtg5);
  }
  LPC_ADC->ADSTAT = (1 << 4) | (1 << 5);
  for(i=0;i<7;i++)
    dval4[i]=dval5[i]=vtg4[i]=vtg5[i]=0;
  adc_temp4=adc_temp5=0;
  in_vtg4=in_vtg5=0;
}
}
  
  
  
