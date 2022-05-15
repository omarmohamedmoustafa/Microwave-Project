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
void PortD_Init(){
	SYSCTL_RCGCGPIO_R |= 0x08; // activate Port D
	while((SYSCTL_PRGPIO_R&0x00000008) == 0){};
	GPIO_PORTD_LOCK_R = 0x4c4F434B;
	GPIO_PORTD_CR_R |= 0x09;
	GPIO_PORTD_DIR_R 	= 0x01; 
	GPIO_PORTD_DATA_R = 0x00;
	GPIO_PORTD_DEN_R |= 0x09; // digital Input on PF1-3
	GPIO_PORTD_AMSEL_R &= ~0x09;
	GPIO_PORTD_AFSEL_R &= ~0x09;
	GPIO_PORTD_PCTL_R &= ~0xF00F;	
	GPIO_PORTD_PUR_R |= 0x08;	//make pull up resistance for switch 3
	}

int buz = 0;
	//Buzz function
void buzz(bool num){
	if (num) GPIO_PORTD_DATA_R |= 0x01;
	else GPIO_PORTD_DATA_R &= ~0x01;
}
//Idle state
void idle(){ 
	lcd_command2(0x01); // clear LCD
	delay_us(2000);
	GPIO_PORTF_DATA_R &=~0XE; //turn off leds
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
}
