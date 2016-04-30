/*
 * variables.h
 *
 * Created: 24-04-2015 18:30:48
 *  Author: SONY
 */ 


#ifndef VARIABLES_H_
#define VARIABLES_H_

uint8_t flagWritten = 0;
uint8_t tenSecondTimerFlag = 0;
volatile uint8_t timerCount = 0;
int password;
float defPresSet;
float defaultPressure = 3.8;
float minPresSet;
uint8_t minPressure_Flag = 0;
uint8_t minPressure_Count = 0;
float minPressure = 2.5;
volatile float currentPressure;
volatile uint16_t timerValue;
float adcResult;
uint16_t blinkingTimerCount=0;

uint8_t startHour, startMinute, endHour, endMinute;

uint8_t EEMEM eeStartHour, eeStartMinute, eeEndHour, eeEndMinute;
uint8_t EEMEM eeChecksum = 0;
uint16_t EEMEM eeTimerDelay = 0;
float EEMEM eeMinPres = 0;
float EEMEM eeDefaultPres = 0;


uint8_t pressureLowerPumpOFF_Flag = 0;
uint8_t pressureLowerPumpOFF_Count = 0;
uint8_t pressureLowerPumpON_Flag = 0;
uint8_t pressureLowerPumpON_Count = 0;
uint8_t pumpOnFlag = 0;
uint8_t delayTimerElapsedFlag = 0;
uint8_t firstStartFlag=1;
uint8_t delayOnTimer_Flag = 0;
int delayOnTimer_Count = 0;
int presetPassword = 0;
uint8_t digitBlinkCount = 0;
uint8_t digitBlinkFlag = 0;

uint8_t year,month,day,hour,minute,second;
#endif /* VARIABLES_H_ */