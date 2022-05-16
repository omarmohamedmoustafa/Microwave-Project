#include <stdint.h>
#include <stdio.h>
#include <C:\Keil\Labware\inc\tm4c123gh6pm.h>
#include "delay.h"
#include "Lcd.h"
#include "keypad.h"
#include <stdbool.h>

//intializing Ports
void PortF_Init(){
	SYSCTL_RCGCGPIO_R |= 0x20; // activate Port F
	while((SYSCTL_PRGPIO_R&0x00000020) == 0){};
	GPIO_PORTF_LOCK_R = 0x4c4F434B;
	GPIO_PORTF_CR_R |= 0x1F;
	GPIO_PORTF_DIR_R |= 0x0E; // PF4,PF0 in, PF3-1 out
	GPIO_PORTF_DEN_R |= 0x1F; // digital Input on PF1-3
	GPIO_PORTF_AMSEL_R &= ~0x1F;
	GPIO_PORTF_AFSEL_R &= ~0x1F;
	GPIO_PORTF_PCTL_R &= ~0x0FFFFF;	
	GPIO_PORTF_PUR_R |= 0x11; //make pull up resistance for switch 1 and 2
	}
void PortD_Init(){ // connecting sw3 (door) on PD3
	SYSCTL_RCGCGPIO_R |= 0x08; // activate Port D
	while((SYSCTL_PRGPIO_R&0x00000008) == 0){};
	GPIO_PORTD_LOCK_R = 0x4c4F434B;
	GPIO_PORTD_CR_R |= 0x08;
	GPIO_PORTD_DIR_R = 0x00; 
	GPIO_PORTD_DEN_R |= 0x08; // digital Input on PD3 SW3
	GPIO_PORTD_AMSEL_R &= ~ 0x08;
	GPIO_PORTD_AFSEL_R &= ~ 0x08;
	GPIO_PORTD_PCTL_R &= ~ 0xF000;	
	GPIO_PORTD_PUR_R |= 0x08;	//make pull up resistance for switch 3
	}

int buz = 0;
	//Buzz function
void buzz(bool num){
	if (num) GPIO_PORTE_DATA_R |= 0x10;
	else GPIO_PORTE_DATA_R &= ~ 0x10;
}
//Idle state
void idle(){ 
	lcd_command2(0x01); // clear LCD
	delay_us(2000);
	GPIO_PORTF_DATA_R &=~0XE; //turn off leds
}

//pause state
void pause(){
	while ((GPIO_PORTF_DATA_R & 0x10 ) != 0x00 && (GPIO_PORTF_DATA_R & 0x01 ) != 0x00)
		{ 	//out of pause if sw1 or sw 2 is pressed
				GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^ 0x0E; // blink leds
				delay_ms(90);
		}	
}



//Cooking state
int  cooking(int t){
	while (((GPIO_PORTF_DATA_R & 0x01) != 0 )|| ((GPIO_PORTD_DATA_R & 0x8) == 0x0 ) ) {}; //start cooking when sw2 is pressed
	buzz(0);
	for(;t>0;t--) //count down
	{	
		GPIO_PORTF_DATA_R |= 0x0E;

					if((GPIO_PORTF_DATA_R & 0x10) == 0x00 || ((GPIO_PORTD_DATA_R & 0x08)==0x00)) //Pause when sw1 is pressed or door is open
					{
							delay_ms(1000);
							pause();
							delay_ms(1000);

							if ((GPIO_PORTF_DATA_R & 0x10) ==0x00)				//idle state when sw1 pressed again 
							{
								idle();	
								return 0;
							}		
					}
					
		lcd_command2(0xC0);			// start write in second line
					
					if(t<10)	//print numbers below 10 in right digit
					{ 
							lcd_command2(0xC0);
							lcdout('0');
							lcdoutNum(t);
							delay_ms(1000);
					}
					else
					{
							lcdoutNum(t);
							delay_ms(1000);
					}
					
					if (t==1)
					{
							lcd_command2(0xC0);
							lcdprint("00");
						
							for (buz = 0 ; buz <3 ; buz++)		 // at end of cooking buzz and blink leds 3 times  
							{	
								buzz(1);
								GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^ 0x0E;
								delay_ms(300);
								buzz(0);
								GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^ 0x0E;
								delay_ms(1000);
							}			
					}
	}
	
	GPIO_PORTF_DATA_R &=~ 0x0E; 	//turn off leds after blinking
	idle();			  	//return back to idle state
	return 1;
}




int main()
{
//Ports calling 
	PortA_Init();
	PortD_Init();
	PortB_Init();
	PortE_Init();
	PortC_Init();
	PortF_Init();
	lcd_init2();
	
	
	while(1)
	{		
		buzz(0);
		switch (keypad())
		{	
			case 'A':{
						delay_ms(100);
						lcdprint("Popcorn");
						cooking(60);
						break;
			}
			case 'B':{
						delay_ms(100);
						lcdprint("Beef weight?");
						delay_ms(100);
						key =keypad();
						while(key<49 || key>57)			//check for valid number (from 1 to 9)
						{	
						  lcd_command2(0x01);
						  delay_us(2000);
						  lcdprint("Err");					//LCD should show �Err� for 2 seconds,
						  delay_ms(2000);
						  lcd_command2(0x01);				//clear LCD
						  delay_us(2000);
						  lcdprint("Beef weight?"); //show previous message.
						  key =keypad();
						}
						//Display weight of beef for 2 seconds
						lcd_command2(0x01);
						delay_us(2000);
						lcdout(keypad());
						lcd_command2(0x81);
						lcdprint(" kg");
						delay_ms(2000);
						lcd_command2(0x01);
						delay_us(2000);
						cooking(30*(key-48)); //calculate time , go to cooking state
						break;
				}
			
			case 'C':{
						delay_ms(100);
						lcdprint("Chicken weight?");
						delay_ms(100);
						key =keypad();
				
						while(key<49 || key>57)					//check for valid number (from 1 to 9)
						{	
						  lcd_command2(0x01);
					          delay_us(2000);
						  lcdprint("Err");							//LCD should show �Err� for 2 seconds,
						  delay_ms(2000);
						  lcd_command2(0x01);						//clear LCD
						  delay_us(2000);
						  lcdprint("Chicken weight?");	//show previous message.
						  key =keypad();
						}
						//Display weight of Chicken for 2 seconds
						lcd_command2(0x01);
						delay_us(2000);
						lcdout(keypad());
						lcd_command2(0x81);
						lcdprint(" kg");
						delay_ms(2000);
						lcd_command2(0x01);
						delay_us(2000);
						cooking(12*(key-48));						//calculate time , go to cooking state
						break;
				}	
}
