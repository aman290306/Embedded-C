#include<LPC17xx.h>
/*Write an embedded C program to read input from 2*4 matrix of the keypad and to display 
the input value on SSD U8. The keypad is configured to read values from 0 to 7. 
Configure port pins 0.4 to 0.7 for columns of keypad, 0.8 to 0.11 for rows of keypad and 
1.15 to 1.18 as enable lines for SSDs and 2.0 to 2.7 as data lines for SSDs.
  keyboard:
  0  1  2  3
  4  5  6  7
*/
unsigned int seg[2][4]={{0x3F,0x06,0x5B,0x4F},{0x66,0x6D,0x7D,0x07}};
int i,j;
void delay(){
   for(j=0;j<50000;j++);
}
void SSD(int data){
  LPC_GPIO2->FIOCLR=0xFF; //clear old values
  LPC_GPIO1->FIOCLR=0xF<<15; //enable U8
  LPC_GPIO2->FIOSET=data;
  delay(); //delay before returning
}
int main(){
  LPC_PINCON->PINSEL0=0x0; //0.4-0.7, 0.8-0.11 as gpio
  LPC_PINCON->PINSEL1=0x0; //1.15 as gpio
  LPC_PINCON->PINSEL5=0x0; //2.0-2.7 as gpio
  LPC_GPIO0->FIODIR=0xF00; //0.4-0.7 as i/p 0.8-0.11 as o/p. Rows are output to keyboard, columns are input from the keyboard.
  LPC_GPIO1->FIODIR=0xF<<15; //1.15-1.18 as Enable lines
  LPC_GPIO2->FIODIR=0xFF; //2.0-2.7 as Data lines
  // 0 = pressed, 1 = not pressed
  while(1){
  LPC_GPIO0->FIOSET=0xF00; //Enable all rows = idle state 
  LPC_GPIO0->FIOCLR=0x100; //Set row 0 as Low 
  for(i=0;i<4;i++)
    if(((LPC_GPIO0->FIOPIN >> (4+i)) & 1) == 0) SSD(seg[0][i]);
  LPC_GPIO0->FIOSET=0xF00;
  LPC_GPIO0->FIOCLR=0x200; //Set row 1 as Low 
  for(i=0;i<4;i++)
    if(((LPC_GPIO0->FIOPIN >> (4+i)) & 1) == 0) SSD(seg[1][i]);
  }
return 0;
}
  
