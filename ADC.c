/*Configure and read analog data from ADC channel 5 and display digital value in LCD*/
#include<LPC17xx.h>
#include<stdio.h>
#define Ref_Vtg 3.300
#define Full_scale 0xFFF
void clear_ports(){
  LPC_GPIO0->FIOCLR=0xF<<23; //clear data lines 0.23-0.26
  LPC_GPIO0->FIOCLR=1<<27; //clear RS line 0.27
  LPC_GPIO0->FIOCLR=1<<28; //clear enable line 0.28
}
void write(int val,int RS){
  clear_ports();
  LPC_GPIO0->FIOSET=val;
  if(RS==0) LPC_GPIO0->FIOCLR=1<<27;
  else LPC_GPIO0->FIOSET=1<<27;
  LPC_GPIO0->FIOSET=1<<28; //Enable high
  delay_lcd(200); // generate enable high to low pulse
  LPC_GPIO0->FIOCLR=1<<28; //Enable Low
}
void delay_lcd(unsigned int k){
  int k1;
  for(k1=0;k1<k;k1++);
}
void lcd_comdata(int temp ,int type){
  clear_ports();
  int temp1=temp & 0xF0;
  temp1=temp1<<19;
  write(temp1,type);
  delay_lcd(500);
  int temp2=temp & 0x0F;
  temp2=temp2<<23;
  write(temp2,type);
  delay_lcd(500);
}
void alphanum(char* str){
  while(*str!='\0')
    lcd_comdata(*(str++),1);
  delay_lcd(500);
}
void lcd_init(){
  clear_ports();
  delay_lcd(500);
  lcd_comdata(0x33,0);
  delay_lcd(30000);
  lcd_comdata(0x32,0);
  delay_lcd(30000);
  lcd_comdata(0x28,0); //function set.
  delay_lcd(500);
  lcd_comdata(0x0C,0); //display on cursor off. 
  lcd_comdata(0x06,0); //entry mode set. cursor is increment mode
  delay_lcd(500);
  lcd_comdata(0x01,0); //clear display
  delay_lcd(10000);
}
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
