#include <avr/io.h>
#include <util/delay.h>

int main(void){
		int On = 0;
		int Off = 100;
	
		DDRD = (1 << PD6);
	double brightness2;
	int brightness = 0;
	
	while (1)
	{
		for (brightness = 0; brightness <= 100; brightness ++){ //For-løkke for å dimme opp.
			On = 0;
			Off = 100;
			
			brightness2 = brightness*brightness/100;
			
			PORTD |= (1 << PD6);
			
			while (On<brightness2){
				//_delay_ms(0.001);
				On++;	
			}
			
			PORTD &= ~(1 << PD6);
			
			while(Off > brightness2){
				//_delay_ms(0.001);
				Off--;
			}
		}
		//Ny for-løkke for å dimme ned
		for (brightness = 100; brightness >= 0; brightness --){
			On = 100;
			Off = 0;
			brightness2 = brightness*brightness/100;
			
			PORTD &= ~(1 << PD6);
			
			while (On > brightness2){
				//_delay_ms(0.001);
				On--;
			}
			
			PORTD |= (1 << PD6);
			
			while(Off < brightness2){
				//_delay_ms(0.001);
				Off++;
			}
		}
	}
}

			
			
			
			
		




		

		
			

