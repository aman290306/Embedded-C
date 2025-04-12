/*Configure and read analog data from ADC channel 5 and display digital value in LCD*/
#include<LPC17xx.h>
#include<stdio.h>
#define Ref_Vtg 3.300
#define Full_scale 0xFFF
int main(){
  unsigned int i;
  unsigned long adc_temp;
  float in_vtg;
  unsigned char vtg[7],dval[7];
  unsigned char Msg0[11]={"Analaog IP:"};
  unsigned char Msg1[12]={"ADC OUTPUT:"};
  SystemInit();
  SystemCoreClockUpdate();
  lcd_init();
  LPC_PINCON->PINSEL3|=0xC0000000; //P1.31 as ADC0.5
  lcd_comdata(0x80,0);
  delay_lcd(800);
  alphanum(Msg0);
  lcd_comdata(0xC0);
  delay_lcd(800);
  alphanum(Msg1);
while(1){
  LPC_ADC->ADCR=(1<<5)|(1<<21)|(1<<24);
  while(!(LPC_ADC->ADDR5 & 0x80000000));
  adc_temp=LPC_ADC->ADDR5;
  adc_temp>>=4;
  adc_temp&=0xFFF;

  in_vtg=(((float)adc_temp*(float)Ref_Vtg)/(float)Full_Scale);
  sprintf(dval,"%lu",adc_temp);
  for(i=0;i<2000;i++);
  lcd_comdata(0x89,0);
  delay_lcd(800);
  alphanum(&vtg[0]);
  lcd_comdata(0xC8,0);
  delay_lcd(800);
  alphanum(&dval[0]);
  for(i=0;i<200000;i++);
  for(i=0;i<7;i++)
      vtg[i]=dval[i]=0;
  adc_temp=0;
  in_vtg=0;
  }
}
