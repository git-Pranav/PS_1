/*
 * LcdFunctions.h
 *
 * Created: 24-04-2015 18:34:52
 *  Author: SONY
 */ 


#ifndef LCDFUNCTIONS_H_
#define LCDFUNCTIONS_H_

void sendToLcd(uint8_t registerSelect, uint8_t readOrWrite, uint8_t dataToSend)
{
	
	_delay_ms(2);
	if(registerSelect==COMMAND)
	CLEARBIT (PORTC,RS);
	
	else SETBIT (PORTC,RS);
	
	if (readOrWrite==WRITE)
	CLEARBIT (PORTC,RW);
	
	else SETBIT (PORTC,RW);
	
	LcdData = dataToSend;
	SETBIT(PORTC,EN);
	_delay_ms(10);
	CLEARBIT(PORTC,EN);
}
void sendStringToLcd(char *stringToSend)
{
	for(uint8_t i=0;stringToSend[i]!='\0';i++)
	{
		sendToLcd(DATA, WRITE, stringToSend[i]);
	}
}
void SendDigitsToLcd(int i, uint8_t address)
{
	char digits[5];
	digits[0] = (i/1000) + 0x30;
	int garbage = i%1000;
	digits[1] = (garbage/100) + 0x30;
	
	garbage = garbage%100;
	digits[2] = (garbage/10) + 0x30;
	
	garbage = garbage%10;
	digits[3] = garbage + 0x30;
	digits[4] = '\0';
	
	sendToLcd(COMMAND, WRITE, address);
	sendStringToLcd(digits);
}
void convertAndSend(float floatValue)
{
	char intPart = (char) floatValue;
	
	int decPart = (((int) ( floatValue * 10 )) % 10 );
	
	char intPartDigits[2] = { (intPart/10 + 0x30) , (intPart%10 + 0x30) };
	char decPartDigit = (decPart + 0x30);
	
	sendToLcd(DATA,WRITE,intPartDigits[0]);
	sendToLcd(DATA,WRITE,intPartDigits[1]);
	sendToLcd(DATA, WRITE, 0x2E); //decimal point
	sendToLcd(DATA,WRITE,decPartDigit);
}
void sendDigitsToLcd_withLength(int i, uint8_t address,uint8_t length)
{
	char digits[length+1];
	digits[length] = '\0';
	char garbage=i;
	uint8_t a,b;
	for (a=0,b=length-1 ; a < length-1 ; b--,a++)
	{
		digits[a] = ((garbage)/(10^b)) + 0x30;
		garbage = garbage%((10^b));
		
	}
	
	digits[a] = garbage + 0x30;
	sendToLcd(COMMAND, WRITE, address);
	sendStringToLcd(digits);
}
void displayTime(uint8_t i,uint8_t address)
{
	//uint8_t garbage = i;
	char digits[3] = {(i/10) + 0x30,(i%10) + 0x30,'\0'};
	sendToLcd(COMMAND,WRITE,address);
	sendStringToLcd(digits);
}


#endif /* LCDFUNCTIONS_H_ */