/*Stepper motor keyboard: clockwise/anticlockwise: key0/4=90deg, key1/5=180deg, key2/6=270deg, key3/7=360deg
 row0=clockwise 
 row1=anticlock row0=clockwise 
 display angle,dir on lcd.
0.23-0.26 lcd data lines
0.27 RS lcd
0.28 Enable lcd
0.15,0.18 cols
0.19-0.20 rows
COLUMNS GET THE LOWER 4PINS OF THE 8PINS SENT TO KEYBOARD
1.23-1.26 stepper motor*/

#include<LPC17xx.h>
unsigned int columns,temp,temp1,temp2,k1,i;
char Msg[]="Invalid";
char Msg0[]="90 Clk";
char Msg1[]="180 Clk";
char Msg2[]="270 Clk";
char Msg3[]="360 Clk";
char Msg4[]="90 Anti";
char Msg5[]="180 Anti";
char Msg6[]="270 Anti";
char Msg7[]="360 Anti";

void clear_ports(){
  LPC_GPIO0->FIOCLR=0xF<<23; //clear data lines 0.23-0.26
  LPC_GPIO0->FIOCLR=1<<27; //clear RS line 0.27
  LPC_GPIO0->FIOCLR=1<<28; //clear enable line 0.28
}
void delay(unsigned int k){
  for(k1=0;k1<k;k1++);
}
void write(int val,int RS){
  clear_ports();
  LPC_GPIO0->FIOSET=val;
  if(RS==0) LPC_GPIO0->FIOCLR=1<<27;
  else LPC_GPIO0->FIOSET=1<<27;
  LPC_GPIO0->FIOSET=1<<28; //Enable high
  delay(25); // generate enable high to low pulse
  LPC_GPIO0->FIOCLR=1<<28; //Enable Low
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
    delay(800);
  }
}
void lcd_init(){
  clear_ports();
  lcd_comdata(0x33,0);
  delay(30000);
  lcd_comdata(0x32,0);
  delay(30000);
  lcd_comdata(0x28,0); //function set.
  delay(30000);
  lcd_comdata(0x0C,0); //display on cursor off. 
  delay(800);
  lcd_comdata(0x06,0); //entry mode set. cursor is increment mode
  delay(800);
  lcd_comdata(0x01,0); //clear display
  delay(10000); 
}
void clockwise_steps(int mult){
	for(i=0;i<mult;i++){
	if((LPC_GPIO1->FIOPIN>>26)&1){//if D is on, turn D off, turn A on
		LPC_GPIO1->FIOPIN=1<<23;
	}
	else{
		LPC_GPIO1->FIOPIN=LPC_GPIO1->FIOPIN<<1;
	}
	delay(10000);
        }
}
void anti_clockwise_steps(int mult){
	for(i=0;i<mult;i++){
	if((LPC_GPIO1->FIOPIN>>23)&1){//if A is on, turn A off, turn D on
		LPC_GPIO1->FIOPIN=1<<26;
	}
	else{
		LPC_GPIO1->FIOPIN=LPC_GPIO1->FIOPIN>>1;
	}
	delay(10000);
        }
}
void read_columns(){
    columns=(LPC_GPIO0->FIOPIN>>15)&0xF; 
    delay(1000);
}

int main(){
	//0.15-18 cols 0.19,0.20 rows
    SystemInit();
    SystemCoreClockUpdate();
    LPC_PINCON->PINSEL0=0x0;
    LPC_PINCON->PINSEL1= 0x0; 
    LPC_GPIO0->FIODIR=0x0F<<23|1<<27|1<<28|3<<19;
    LPC_GPIO0->FIODIR&=~(0xF<<15);
    lcd_init();
    LPC_PINCON->PINSEL3=0x0; //1.23-1.26 as GPIO 
    LPC_GPIO1->FIODIR=0xF<<23; //1.23-1.26 as output stepper motor
    LPC_GPIO1->FIOSET=1<<23; // initial state of motor
	// keyboard 
	while(1){
		LPC_GPIO0->FIOSET=0x3<<19;//set all rows high initially
		LPC_GPIO0->FIOCLR=0x1<<19; //set row 0 low. enable row 0
		read_columns();
		
		switch(columns){
	        case(0): //no col pressed in row 0
	            break;

		case(1)://is col 0 pressed
			alphanum(Msg0);
			clockwise_steps(50);
			lcd_comdata(0x01,0);//clear lcd
		    	delay(10000);
			break;
		
        	case(2)://is col 1 pressed
			alphanum(Msg1);
			clockwise_steps(100);
			lcd_comdata(0x01,0);//clear lcd
	    		delay(10000);
			break;
		
		case(4)://is col 2 pressed
			alphanum(Msg2);
			clockwise_steps(150);
			lcd_comdata(0x01,0);
			delay(10000);
			break;
		
		case(8)://is col 3 pressed
			alphanum(Msg3);
			clockwise_steps(200);
			lcd_comdata(0x01,0);//clear lcd
	    		delay(10000);
			break;
			
		default: 
			alphanum(Msg);
			lcd_comdata(0x01,0);//clear lcd
			delay(10000);
			break;
		}
		
		LPC_GPIO0->FIOSET=0x3<<19;//set all rows high 
		LPC_GPIO0->FIOCLR=0x1<<20; //set row 1 low. enable row 1
		read_columns();
		
		switch(columns){
        	case(0): //if no col is pressed in row 1
           		break;

		case(1)://is col 0 pressed
			alphanum(Msg4);
			anti_clockwise_steps(50);
			lcd_comdata(0x01,0);//clear lcd
		    delay(10000);
			break;
		
        case(2)://is col 1 pressed
			alphanum(Msg5);
			anti_clockwise_steps(100);
			lcd_comdata(0x01,0);//clear lcd
	    	delay(10000);
			break;
		
		case(4)://is col 2 pressed
			alphanum(Msg6);
			anti_clockwise_steps(150);
			lcd_comdata(0x01,0);
			delay(10000);
			break;
		
		case(8)://is col 3 pressed
			alphanum(Msg7);
			anti_clockwise_steps(200);
			lcd_comdata(0x01,0);//clear lcd
	    		delay(10000);
			break;
			
		default: 
		    alphanum(Msg);
		    lcd_comdata(0x01,0);//clear lcd
	    	    delay(10000);
		    break;
		}
	} //step=1.8deg
}
