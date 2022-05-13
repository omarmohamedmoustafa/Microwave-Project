#include <stdint.h>
#include <C:\Keil\Labware\inc\tm4c123gh6pm.h>
#include "delay.h"
#define 	Rs 	5
#define 	E 	7

//Data lines --> B0 - B7
//command(RS/E) --> A (A5 --> RS , A7 --> EN)
void PortA_Init(){
//LCD RS-RW-E ==> A5-A7
	SYSCTL_RCGCGPIO_R |= 0x01; 
	while((SYSCTL_PRGPIO_R&0x00000001) == 0){};
	GPIO_PORTA_DIR_R |= 0xE0; //Pins5-7 RS-RW-E
	GPIO_PORTA_DEN_R |= 0xE0; 
	GPIO_PORTA_AMSEL_R &= ~0xE0;
	GPIO_PORTA_AFSEL_R &= ~0xE0;
	GPIO_PORTA_PCTL_R &= ~0xFFF00000;
	GPIO_PORTA_CR_R |= 0xE0;
	//GPIO_PORTA_DATA_R|= 0xE0; INIT Data for RS-RW-E
	}	
void PortB_Init(){
	// inti for LCD
	SYSCTL_RCGCGPIO_R |= 0x02; 
	while((SYSCTL_PRGPIO_R&0x00000002) == 0);
	GPIO_PORTB_DIR_R |= 0xFF; //Pins0,1,4 Output pins2,4,6,7 Input
	GPIO_PORTB_DEN_R |= 0xFF; 
	GPIO_PORTB_AMSEL_R &= ~0xFF;
	GPIO_PORTB_AFSEL_R &= ~0xFF;
	GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;
	GPIO_PORTB_CR_R |= 0xFF;
	}

void lcd_init2(void){ 
		  delay_ms(30);
		  delay_us(2000);
		  lcd_command2(0x38);                         //8-bit mode
			delay_ms(1);
		  lcd_command2(0x0E);                         //Display ON , Cursor blinking
			delay_ms(15);
		  lcd_command2(0x02);                         //Clear Screen
		  delay_ms(15);
		  lcd_command2(0x06);                        //Shift the cursor one position right 
			delay_ms(2);

	}

void lcd_command2(unsigned char cmd){ 
         		  
       GPIO_PORTA_DATA_R &=~ (1<<Rs) ;  //Reset Rs  for Command Write
       GPIO_PORTA_DATA_R |=(1<<E) ;     //Set Enable start of high to low pulse
       delay_ms(1); 
			 GPIO_PORTB_DATA_R = cmd;					//print_data(data) Data pins = command
			 delay_ms(1); 
       GPIO_PORTA_DATA_R &=~(1<<E) ;    //Reset Enable End of the high to low pulse
       delay_ms(5);
}

void lcdout( unsigned char value) {
       GPIO_PORTA_DATA_R |= (1<<Rs) ;   //set Rs  for Command Write
       GPIO_PORTA_DATA_R |=(1<<E) ;     //Set Enable start of high to low pulse
       delay_ms(1); 
			 GPIO_PORTB_DATA_R = value;				//print_data(data) Data pins = v
			 delay_ms(1); 
       GPIO_PORTA_DATA_R &=~(1<<E) ;    //Reset Enable End of the high to low pulse
       delay_ms(5);
      }


void lcdprint (char *str){ //To print String
      unsigned char i =0 ;
      while(str[i]!=0){lcdout(str[i]) ; i++;}
      }

void lcdoutNum (int number) {
	int p;
	int k=0;
	int num[4]={0};
	while(number>0)
	{
		num[k]=number%10;
		number=number/10;
		k++;
	}
	k--;
	
	for (p=k;p>=0;p--)
		{
			
			lcdout(num[p]+48);
			}
	

			
}
