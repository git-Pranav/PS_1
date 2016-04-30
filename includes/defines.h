

#ifndef DEFINES_H_
#define DEFINES_H_

# define F_CPU 8000000UL

#define BIT(x) (1<<x)
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))

#define READBIT(x,y) ((x) & BIT(y))

#define RS PC2
#define RW PC3
#define EN PC4


#define LEVEL_SENSOR PINA1
#define DRY_RUN PINA2
//#define MPCB PINA3
#define SET PINA5
#define UP PINA6
#define DOWN PINA7

#define DEBOUNCE_VALUE 20 //IN MS
#define PUMP PB0
#define LEVEL_SENSOR_LED PB1
#define DRY_RUN_LED PB2


#define LcdData PORTD
#define READ 1
#define WRITE 0
#define COMMAND 0
#define DATA 1
 
#define DS1307_ADDR (0x68 << 1)
const uint8_t DS1307_daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };


#define Pdef_EEPROM_LOCATION 46
#define Pcutoff_EEPROM_LOCATION 75
#define Tdelay_EEPROM_LOCATION 24
#define Tone_EEPROM_LOCATION 28
#define Ttwo_EEPROM_LOCATION 32   
#endif /* DEFINES_H_ */