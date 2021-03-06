#include <C:\Keil\Labware\inc\tm4c123gh6pm.h>
#include "delay.h"
//connectiong the rows of keypad as output and on C4-C5-C6-C7
void PortC_Init(){
	SYSCTL_RCGCGPIO_R |= 0x04;        //Enable clock to PORTC  
    	while ((SYSCTL_RCGCGPIO_R&0x04)==0);  	//wait for clock to be setted
    	GPIO_PORTC_CR_R  |= 0xF0;             //Allow settings for all pins of PORTC
	GPIO_PORTE_AMSEL_R &= ~0xF0; // disable analog funtionality for Port C
	GPIO_PORTE_AFSEL_R &= ~0xF0;
    	GPIO_PORTC_DIR_R   |=0xF0; // set Portc output
    	GPIO_PORTC_DEN_R |= 0xF0;// digital enable C4-C7
	
           
}

//connectiong the columns of keypad as inpput and on E0-E1-E2-E3	and buzz on E4
void PortE_Init(){
		SYSCTL_RCGCGPIO_R |= 0x10;		//Enable clock to PORTE  
		while((SYSCTL_PRGPIO_R & 0x10)==0);	//wait for clock to be setted
		GPIO_PORTE_DIR_R &= ~0x0F; //set E0-E3 as input pins and E4 output
		GPIO_PORTE_DIR_R |=0x10; //buzz out
		GPIO_PORTE_DEN_R |=0x1F;
		GPIO_PORTE_AMSEL_R &= ~0x1F;
		GPIO_PORTE_AFSEL_R &= ~0x1F;
		GPIO_PORTE_PDR_R |= 0x0F;		
}

void row_activate(unsigned short column)	
{
		GPIO_PORTC_DATA_R = (1<<(column+4));
}
unsigned short portE_DATA(){
		return (GPIO_PORTE_DATA_R & 0x0F);
}
// declaring each button in the 4x4 Keypad
unsigned char matrix[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

char keypad(void){
		while(1){
			unsigned short row,column;
				for (row=0;row<4;row++){
						row_activate(row);
						delay_us(2);
						for(column=0;column<4;column++){
								if(portE_DATA() & (1<<column)){
										return matrix[row][column];
								}
						}
				}
		}
}

