/*Write an embedded C program that displays a message for 5 seconds and then clears the 
LCD screen. Configure port pins 1.10 to 1.13 as data lines and pins 1.14 and 1.15 as control 
lines for LCD. Use timer0 for delay assuming PCLK=12 MHz and Timer resolution as 10 
microseconds.*/
#include<LPC17xx.h> 
void initTimer0(){
  LPC_TIM0->CTCR=0x0; //timer mode
  LPC_TIM0->PR=119;
  LPC_TIM0->TCR=0x02; //reset timer
}
void delay_lcd(val){
  int a;
  for(a=0;a<val;a++);
}

void clear_ports(){
  LPC_GPIO1->FIOCLR=0x3F<<10;
}

void write(int data, int type){
  clear_ports();
  LPC_GPIO1->FIOSET=data<<10;
  if (type==0) LPC_GPIO1->FIOCLR=1<<14;
  else LPC_GPIO1->FIOSET=1<<14;
  LPC_GPIO1->FIOSET=1<<15; //enable high
  delay_lcd(200);
  LPC_GPIO1->FIOCLR=1<<15; //enable low
}

void lcd_comdata(int val,int type){
  write((val & 0xF0)>>4, type);
  delay_lcd(500);
  write(val & 0x0F, type);
}

void lcd_init(){
  clear_ports();
  delay_lcd(500);
  lcd_comdata(0x33,0);
  delay_lcd(500);
  lcd_comdata(0x32,0);
  delay_lcd(500);
  lcd_comdata(0x28,0); //function set.
  delay_lcd(500);
  lcd_comdata(0x0C,0); //display on cursor off. 
  delay_lcd(500);
  lcd_comdata(0x06,0); //entry mode set. cursor is increment mode
  delay_lcd(500);
  lcd_comdata(0x01,0); //clear display
  delay_lcd(10000);
}

void alphanum(char* str){
  while(*str!='\0'){
    lcd_comdata(*(str++),1);
    delay_lcd(500);
  }
}
  
int main(){
  SystemInit();
  SystemCoreClockUpdate();
  char Msg[]={"Hello Aman!"};
  initTimer0();
  lcd_init();
  LPC_PINCON->PINSEL2=0x0;
  LPC_GPIO1->FIODIR=0x3F<<10; //1.10-1.13, RS-1.14, Enable-1.15 as o/p 
  alphanum(Msg);
  LPC_TIM0->TCR=0x01; //enable timer
  while(LPC_TIM0->TC<500000);
  LPC_TIM0->TCR=0x02;
  lcd_comdata(0x01,0);
  while(1);
}
