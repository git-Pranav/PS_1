/*
 * DS1307.h
 *
 * Created: 26-04-2015 13:55:08
 *  Author: SONY
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#define SCL_CLOCK 80000U //IN Hz
#define I2C_WRITE 0 //Data direction
#define I2C_READ 1

void I2C_Init();
unsigned char I2C_Start(unsigned char address);
void I2C_StartWait(unsigned char address);
unsigned char I2C_Write( unsigned char data);
void I2C_Stop();
unsigned char I2C_RepStart(unsigned char address);
unsigned char I2C_ReadAck(void);
unsigned char I2C_ReadNak(void);

void DS1307_Init();
uint8_t DS1307_DECtoBCD(uint8_t val);
uint8_t DS1307_BCDtoDEC(uint8_t val);
uint16_t DS1307_dateToDays(uint8_t y, uint8_t m, uint8_t d); 
uint8_t DS1307_getDayOfWeek(uint8_t y, uint8_t m, uint8_t d);

uint8_t DS1307_setDate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void DS1307_getDate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second);

void I2C_Init()
{
	TWSR = 0;
	TWBR = ((F_CPU/SCL_CLOCK) - 16) / 2 ;
}
unsigned char I2C_Start(unsigned char address)
{
	uint8_t   twst;

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;

}
void I2C_StartWait(unsigned char address)
{
	uint8_t   twst;


	while ( 1 )
	{
		// send START condition
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
		
		// wait until transmission completed
		while(!(TWCR & (1<<TWINT)));
		
		// check value of TWI Status Register. Mask prescaler bits.
		twst = TW_STATUS & 0xF8;
		if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
		
		// send device address
		TWDR = address;
		TWCR = (1<<TWINT) | (1<<TWEN);
		
		// wail until transmission completed
		while(!(TWCR & (1<<TWINT)));
		
		// check value of TWI Status Register. Mask prescaler bits.
		twst = TW_STATUS & 0xF8;
		if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) )
		{
			/* device busy, send stop condition to terminate write operation */
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
			
			// wait until stop condition is executed and bus released
			while(TWCR & (1<<TWSTO));
			
			continue;
		}
		//if( twst != TW_MT_SLA_ACK) return 1;
		break;
	}

}
unsigned char I2C_Write( unsigned char data)
{
	uint8_t   twst;
	
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}
void I2C_Stop()
{
	/* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}
unsigned char	I2C_RepStart(unsigned char address)
{
	return I2C_Start( address );

}
unsigned char I2C_ReadAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));

	return TWDR;

}
unsigned char I2C_ReadNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
	return TWDR;

}

void DS1307_Init() {
	I2C_Init();
	_delay_us(10);
}
uint8_t DS1307_DECtoBCD(uint8_t val) {
	return val + 6 * (val / 10);
}
uint8_t DS1307_BCDtoDEC(uint8_t val) {
	return val - 6 * (val >> 4);
}
uint16_t DS1307_dateToDays(uint8_t y, uint8_t m, uint8_t d) {
	uint16_t days = d;
	for (uint8_t i = 1; i < m; ++i)
	days += pgm_read_byte(DS1307_daysInMonth + i - 1);
	if (m > 2 && y % 4 == 0)
	++days;
	return days + 365 * y + (y + 3) / 4 - 1;
}
uint8_t DS1307_getDayOfWeek(uint8_t y, uint8_t m, uint8_t d) {
	uint16_t day = DS1307_dateToDays(y, m, d);
	return (day + 6) % 7;
}

uint8_t DS1307_setDate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	//sanitize data
	if (second < 0 || second > 59 ||
	minute < 0 || minute > 59 ||
	hour < 0 || hour > 23 ||
	day < 1 || day > 31 ||
	month < 1 || month > 12 ||
	year < 0 || year > 99)
	return 8;

	//sanitize day based on month
	if(day > pgm_read_byte(DS1307_daysInMonth + month - 1))
	return 0;

	//get day of week
	uint8_t dayofweek = DS1307_getDayOfWeek(year, month, day);

	//write date
	I2C_StartWait(DS1307_ADDR | I2C_WRITE);
	I2C_Write(0x00);//stop oscillator
	I2C_Write(DS1307_DECtoBCD(second));
	I2C_Write(DS1307_DECtoBCD(minute));
	I2C_Write(DS1307_DECtoBCD(hour));
	I2C_Write(DS1307_DECtoBCD(dayofweek));
	I2C_Write(DS1307_DECtoBCD(day));
	I2C_Write(DS1307_DECtoBCD(month));
	I2C_Write(DS1307_DECtoBCD(year));
	I2C_Write(0x00); //start oscillator
	I2C_Stop();
	
	return 0;
}
void DS1307_getDate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second) {
	I2C_StartWait(DS1307_ADDR | I2C_WRITE);
	I2C_Write(0x00);//stop oscillator
	I2C_Stop();

	I2C_RepStart(DS1307_ADDR | I2C_READ);
	*second = DS1307_BCDtoDEC(I2C_ReadAck() & 0x7F);
	*minute = DS1307_BCDtoDEC(I2C_ReadAck());
	*hour = DS1307_BCDtoDEC(I2C_ReadAck());
	I2C_ReadAck();
	*day = DS1307_BCDtoDEC(I2C_ReadAck());
	*month = DS1307_BCDtoDEC(I2C_ReadAck());
	*year = DS1307_BCDtoDEC(I2C_ReadNak());
	I2C_Stop();
}

#endif /* DS1307_H_ */