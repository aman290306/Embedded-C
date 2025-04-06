#include<LPC17xx.h> 
// display message
char Msg[]="Hello Aman!";
char Msg2[]="How are you?";
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
  SystemInit();
  SystemCoreClockUpdate();
  LPC_PINCON->PINSEL1= 0x0; //0.23-0.26,0.27,0.28 as GPIO
  LPC_GPIO0->FIODIR=0x0F<<23|1<<27|1<<28; //set as output
  lcd_init();
  alphanum(&Msg[0]);
  lcd_comdata(0xC0,0); //goes to first position of next line
  alphanum(Msg2);
  while(1);
}
