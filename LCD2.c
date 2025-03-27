#include<LPC17xx.h> 
/* yes i want it to look like its typing and then delete it as backspace yes. every character type or delete should take one second. it should loop
  Hello Aman_
*/
// 1ms=1000 micro s
int temp1,temp2,k1,len;
char Msg[]="Hello Aman!";
void initTimer0(){
  LPC_TIM0->CTCR=0x0; //Timer mode
  LPC_TIM0->PR= 599; //TC increments every 50micro s if pclk=12Mhz
  LPC_TIM0->TCR=0x02; // when bit 1 is 1 TC,PC are set to 0, when bit 0 is 0 timer is disabled
}
void delay(int v){
  LPC_TIM0->TCR=0x02; //reset timer
  LPC_TIM0->TCR=0x1; //Enable timer
  while(LPC_TIM0->TC<v);
  LPC_TIM0->TCR=0x0;
}
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
  delay_lcd(25); // generate enable high to low pulse
  LPC_GPIO0->FIOCLR=1<<28; //Enable Low
}
void delay_lcd(unsigned int k){
  for(k1=0;k1<k;k1++);
}
void lcd_comdata(int temp ,int type){
  temp1=temp & 0xF0;
  temp1=temp1<<19;
  write(temp1,type);
  temp2=temp & 0x0F;
  temp2=temp2<<23;
  write(temp2,type);
}
void alphanum(char* str){
  while(*str!='\0'){
    lcd_comdata(*(str++),1);
    delay(20000); //1 character 1 second
  }
}
void lcd_init(){
  clear_ports();
  lcd_comdata(0x33,0);
  delay_lcd(30000);
  lcd_comdata(0x32,0);
  delay_lcd(30000);
  lcd_comdata(0x28,0); //function set.
  delay_lcd(30000);
  lcd_comdata(0x0F,0); //display on cursor on with blink. 
  delay_lcd(800);
  lcd_comdata(0x06,0); //entry mode set. cursor is increment mode
  delay_lcd(800);
  lcd_comdata(0x01,0); //clear display
  delay_lcd(10000); 
}
void backspace(char* s){
   len=0;
  while(s[len]!='\0') len++;
  
  while(len!=0){
    lcd_comdata(0x10,0); //shifts cursor to the left by 1 place 
    lcd_comdata(' ',1); //cursor gets incremented and the letter is overwritten by blank
    lcd_comdata(0x10,0); //shift cursor left by 1 place again
    len--;
    delay(20000);
  }
}
int main(){
  SystemInit();
  SystemCoreClockUpdate();
  LPC_PINCON->PINSEL1= 0x0; //0.23-0.26,0.27,0.28 as GPIO
  LPC_GPIO0->FIODIR=0x0F<<23|1<<27|1<<28; //set as output
  lcd_init();
  initTimer0();
  while(1){
  alphanum(&Msg[0]);
  delay(100000); //start backspacing after 5s
  backspace(&Msg[0]);
  delay(20000); //start writing again after 1s
  }
}
