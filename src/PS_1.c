#include "includes.h"

void InitADC()
{
	ADMUX=(1<<REFS0);                         // For Aref=AVcc;
	ADCSRA=(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0); //Prescalar div factor =128
}
float ReadADC(uint8_t ch)
{
	//PORTA = 0xFE;
	
	
	ADCSRA |= (1<<ADEN);
	
	ch=(ADMUX & 0xf0)|(ch& 0x0f);
	ADMUX=ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	ADCSRA|=(1<<ADIF);

	ADCSRA &= ~(1<<ADEN);
	//PORTA = 0xFF;
	return(ADC);
}
ISR(TIMER0_OVF_vect)
{
	timerCount++;
	blinkingTimerCount++;
	if (blinkingTimerCount == 2000)
	{
		if (digitBlinkFlag == 1)
		{
			if (digitBlinkCount < 250)
			{
				digitBlinkCount++;
			}
			else digitBlinkCount=0;
		}
		blinkingTimerCount = 0;
	}
	
	if (timerCount == 61) //61 = 1 second
	{
		if (pressureLowerPumpOFF_Flag == 1)
		{
			pressureLowerPumpOFF_Count++;
		}
		
		
		if (pressureLowerPumpON_Flag == 1)
		{
			pressureLowerPumpON_Count++;
		}
		
		if (delayOnTimer_Flag == 1)
		{
			delayOnTimer_Count++;
		}
		
		if (minPressure_Flag == 1)
		{
			minPressure_Count++;
		}
		
		
		timerCount=0;
	}
}

int main(void)
{
	//SFIOR &= ~(1<<PUD);
	DDRA = 0x00; //Input port (analog and keyboard)
	PORTA = 0xFE; //Enable Pull-up resistors, and make ADC0 as tri-stated
	DDRC = 0xFF; //RS, RW, EN
	DDRD = 0xFF; //LcdData = output
	DDRB = 0xFF; //PortB for LED and motors
	
	
	
	SETBIT (PORTC,EN); //Disable LCD
	CLEARBIT(PORTB,PUMP); //Make sure the pump is switched off.
	InitADC();
	DS1307_Init();
	TIMSK |= (1<<TOIE0);
	
	TCCR0 |=  (1<<CS01); //prescalar = 8 for 8MHz oscillator
	TCNT0 = 0;
	//sei();
	//minPressure = eeprom_read_float(&eeMinPres);
	//defaultPressure = eeprom_read_float(&eeDefPres);
	//minPressure = 3.2;
	//timerValue = 0005;
		
	if ( (eeprom_read_byte(&eeChecksum)) != 1)
	{
		
		eeprom_update_float(&eeDefaultPres,0);
		eeprom_update_float(&eeMinPres,0);
		eeprom_update_word(&eeTimerDelay,0);
		
		eeprom_update_byte(&eeStartHour,0);
		eeprom_update_byte(&eeStartMinute,0);
		eeprom_update_byte(&eeEndHour,0);
		eeprom_update_byte(&eeEndMinute,0);
		
		eeprom_update_byte(&eeChecksum,1);
	}
	
	
	minPresSet = eeprom_read_float(&eeMinPres);
	defPresSet = eeprom_read_float(&eeDefaultPres);
	
	timerValue = eeprom_read_word(&eeTimerDelay);
	
	startHour = eeprom_read_byte(&eeStartHour);
	startMinute = eeprom_read_byte(&eeStartMinute);
	endHour = eeprom_read_byte(&eeEndHour);
	endMinute = eeprom_read_byte(&eeEndMinute);
		
	minPressure = minPresSet;
	defaultPressure = defPresSet;
	
	sei();
	
	sendToLcd(COMMAND, WRITE, 0x01) ; //Clear LCD
	sendToLcd(COMMAND, WRITE, 0x80) ; //Starting on 0,1
	sendToLcd(COMMAND, WRITE, 0x0C) ;
	sendToLcd(COMMAND, WRITE, 0x38) ; //LCD on, Cursor off
	
	while(1)
    {
		 if ( ( READBIT(PINA,SET) ) == 0)
		 {
			 sendToLcd(COMMAND,WRITE,0x01);
			 
			 _delay_ms(500);
			 if ( (READBIT(PINA,SET) ) == 0)
			 {
				 keyboardFunction();
			 }
		 }
		
		if ((READBIT(PINA,UP)) == 0)
		{
	while ((READBIT(PINA,UP)) == 0) //see time if up key is pressed
			{
				DS1307_getDate(&year,&month,&day,&hour,&minute,&second);
				
				sendToLcd(COMMAND,WRITE,0xC0);
				displayTime(hour,0xC0);
				sendStringToLcd(":");
				displayTime(minute,0xC3);
				sendStringToLcd(":");
				displayTime(second,0xC6);
			}
		sendToLcd(COMMAND,WRITE,0xC0);
		sendStringToLcd("          ");
		}
		
		
		
		if ( ((READBIT(PINA,LEVEL_SENSOR))==0) && (pressureLowerPumpON_Flag==0) )
		{
			CLEARBIT(PORTB,PUMP);
			pumpOnFlag = 0;
			SETBIT(PORTB,LEVEL_SENSOR_LED);
		}
		
		else CLEARBIT(PORTB,LEVEL_SENSOR_LED);
		
		if ( ((READBIT(PINA,DRY_RUN))==0)  && (pressureLowerPumpON_Flag==0))
		{
			CLEARBIT(PORTB,PUMP);
			pumpOnFlag = 0;
			SETBIT(PORTB,DRY_RUN_LED);
		}
		else CLEARBIT(PORTB,DRY_RUN_LED);
		
		adcResult = ReadADC(0);
			
		if (flagWritten==0)  //Write on LCD the data and wait till enter key is pressed for password and onwards
		{
			sendToLcd(COMMAND, WRITE, 0x01);
			sendStringToLcd("RUN: ");
			
			flagWritten = 1;
		}
		if (delayOnTimer_Flag == 0)
		{
			sendToLcd(COMMAND,WRITE,0xC0);
			sendStringToLcd("SET:  ");
			sendToLcd(COMMAND, WRITE, 0xC6);
			convertAndSend(defPresSet);
			sendStringToLcd(" Bar");
		}
		
		currentPressure = (( ((adcResult+1) - 196) *10) / (962-196)) ;
		sendToLcd(COMMAND,WRITE,0x80);
		sendStringToLcd("RUN: ");
		sendToLcd(COMMAND, WRITE, 0x86);
		convertAndSend(currentPressure);
		sendStringToLcd(" Bar");
		
		if (pressureLowerPumpON_Flag == 0 && (delayOnTimer_Flag == 0))
		{
			if (currentPressure < (defPresSet - 0.5))
			{
				pressureLowerPumpOFF_Flag = 1;
			}
		}
		
		if (pressureLowerPumpOFF_Count >= 10)
		{
			pressureLowerPumpOFF_Flag = 0;
			pressureLowerPumpOFF_Count = 0;	
			if (currentPressure < (defPresSet-0.5))
			{
				if ( ((READBIT(PINA,LEVEL_SENSOR)) != 0) && ((READBIT(PINA,DRY_RUN)) != 0))
				{
					SETBIT(PORTB,PUMP);
					delayTimerElapsedFlag = 0;
					firstStartFlag = 0;
					pumpOnFlag = 1;
					pressureLowerPumpON_Flag = 1;
				}
			}
		}
		
		if (pressureLowerPumpON_Count >= 10)
		{
			pressureLowerPumpON_Flag = 0;
			pressureLowerPumpON_Count = 0;
		}
		
		if ((currentPressure > (defPresSet + 0.5)) && (delayOnTimer_Flag == 0) && (delayTimerElapsedFlag==0) && (firstStartFlag==0) && (pressureLowerPumpON_Flag==0))
		{
			CLEARBIT(PORTB,PUMP);
			pumpOnFlag = 0;
			delayOnTimer_Flag = 1;
			
			
		}
		
		if (delayOnTimer_Flag == 1)
		{
			sendToLcd(COMMAND,WRITE,0xC0);
			sendStringToLcd("TIME LEFT:");
			SendDigitsToLcd((timerValue - delayOnTimer_Count),0xCA);
		}
		
		if (delayOnTimer_Count >= timerValue)
		{
			
			delayOnTimer_Flag = 0;
			delayOnTimer_Count = 0;
			delayTimerElapsedFlag = 1;
		}
		
		if ((currentPressure < minPresSet) && (pumpOnFlag==1))
		{
			minPressure_Flag = 1;
		}
		
		if (minPressure_Count >= 30 && pumpOnFlag == 1)
		{
			if (currentPressure < minPresSet)
			{
				CLEARBIT(PORTB,PUMP);
				pumpOnFlag = 0;
				minPressure_Count = 0;
				minPressure_Flag = 0;
			}
		}
		
       
	}
}

