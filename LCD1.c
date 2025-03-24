#include<LPC17xx.h> 
// display 'Hello Aman!'
void clear_ports(){
  LPC_GPIO0->FIOCLR=0xF<<23; //clear data lines 0.23-0.26
  LPC_GPIO0->FIOCLR=1<<27; //clear RS line 0.27
  LPC_GPIO0->FIOCLR=1<<28; //clear enable line 0.28
}
void write(int val,int RS){
  clear_ports();
  LPC_GPIO0->FIOPIN=val;
  if(RS==0) LPC_GPIO0->FIOCLR=1<<27;
  else LPC_GPIO0->FIOSET=1<<27;
  LPC_GPIO0->FIOSET=1<<28; //Enable high
  delay_lcd(25); // generate enable high to low pulse
  LPC_GPIO0->FIOCLR=1<<28; //Enable Low
}
void delay_lcd(unsigned int k){
  int k1;
  for(k1=0;k1<k;k1++);
}
void lcd_comdata(int temp ,int type){
  int temp1=temp & 0xF0;
  temp1=temp1<<19;
  write(temp1,type);
  int temp2=temp & 0x0F;
  temp2=temp2<<23;
  write(temp2,type);
}
void alphanum(char* str){
  while(*str!='\0')
    lcd_comdata(*(str++),1);
}
void lcd_init(){
  LPC_PINCON->PINSEL = 0xFC003FFF; //0.23-0.26,0.27,0.28 as GPIO
  LPC_GPIO0->FIODIR=0x0F<<23|1<<27|1<<28; //set as output
  clear_ports();
  delay_lcd(3200);
  lcd_comdata(0x33,0);
  delay_lcd(30000);
  lcd_comdata(0x32,0);
  delay_lcd(30000);
  lcd_comdata(0x28,0); //function set.
  delay_lcd(30000);
  lcd_comdata(0x0F,0); //display on cursor on with blink. 
  lcd_comdata(0x06,0); //entry mode set. cursor is increment mode
  delay_lcd(800);
  lcd_comdata(0x01,0); //clear display
  delay_lcd(10000);
}
int main(){
  SystemInit();
  SystemCoreClockUpdate();
  lcd_init();
  char Msg[]={"Hello Aman!"};
  while(1){
  alphanum(&Msg[0]);
  delay_lcd(100000);
  lcd_comdata(0x01,0);
  delay_lcd(10000);
  }
}
